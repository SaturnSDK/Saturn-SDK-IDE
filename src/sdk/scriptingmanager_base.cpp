/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6392 $
 * $Id: scriptingmanager.cpp 6392 2010-07-12 08:38:40Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/scriptingmanager.cpp $
 */

#include <sdk_precomp.h>

#ifndef CB_PRECOMP
    #include "scriptingmanager_base.h"
    #include "cbexception.h"
    #include "manager.h"
    #include "logmanager.h"
    #include "configmanager.h"
    #include <settings.h>
    #include "globals.h"

    #include <wx/file.h>
    #include <wx/filename.h>
    #include <wx/regex.h>
#endif

#include "crc32.h"
#include "sqplus.h"
#include "scriptbindings.h"
#include "sc_plugin.h"
#include "sqstdstring.h"

#ifdef CB_FOR_CONSOLE
    template<> SCRIPTING_MANAGER_BASE* Mgr<SCRIPTING_MANAGER_BASE>::instance = 0;
    template<> bool  Mgr<SCRIPTING_MANAGER_BASE>::isShutdown = false;
#endif //#ifdef CB_FOR_CONSOLE

static wxString s_ScriptErrors;
static wxString capture;

static void ScriptsPrintFunc(HSQUIRRELVM /*v*/, const SQChar * s, ...)
{
    static SQChar temp[2048];
    va_list vl;
    va_start(vl,s);
    scvsprintf( temp,s,vl);
    wxString msg = cbC2U(temp);
    Manager::Get()->GetLogManager()->DebugLog(msg);
    va_end(vl);

    s_ScriptErrors << msg;
}

static void CaptureScriptOutput(HSQUIRRELVM /*v*/, const SQChar * s, ...)
{
    static SQChar temp[2048];
    va_list vl;
    va_start(vl,s);
    scvsprintf(temp,s,vl);
    ::capture.append(cbC2U(temp));
    va_end(vl);
};

SCRIPTING_MANAGER_BASE::SCRIPTING_MANAGER_BASE()
{
    //ctor

    // initialize but don't load the IO lib
    SquirrelVM::Init((SquirrelInitFlags)(sqifAll & ~sqifIO));

    if (!SquirrelVM::GetVMPtr())
        cbThrow(_T("Can't create scripting engine!"));

    sq_setprintfunc(SquirrelVM::GetVMPtr(), ScriptsPrintFunc);
    sqstd_register_stringlib(SquirrelVM::GetVMPtr());

    RefreshTrusts();

    // register types
    ScriptBindings::RegisterBaseBindings();
}

SCRIPTING_MANAGER_BASE::~SCRIPTING_MANAGER_BASE()
{
    //dtor
    // save trusted scripts set
    ConfigManagerContainer::StringToStringMap myMap;
    int i = 0;
    TrustedScripts::iterator it;
    for (it = m_TrustedScripts.begin(); it != m_TrustedScripts.end(); ++it)
    {
        if (!it->second.permanent)
            continue;
        wxString key = wxString::Format(_T("trust%d"), i++);
        wxString value = wxString::Format(_T("%s?%x"), it->first.c_str(), it->second.crc);
        myMap.insert(myMap.end(), std::make_pair(key, value));
    }
    Manager::Get()->GetConfigManager(_T("security"))->Write(_T("/trusted_scripts"), myMap);

    SquirrelVM::Shutdown();
}

void SCRIPTING_MANAGER_BASE::RegisterScriptFunctions()
{
    // done in scriptbindings.cpp
}

bool SCRIPTING_MANAGER_BASE::LoadScript(const wxString& filename)
{
//    wxCriticalSectionLocker c(cs);

    wxLogNull ln; // own error checking implemented -> avoid debug warnings
    wxString fname(filename);
    wxFile f(fname); // try to open
    if (!f.IsOpened())
    {
        bool found = false;

        // check in same dir as currently running script (if any)
        if (!m_CurrentlyRunningScriptFile.IsEmpty())
        {
            fname = wxFileName(m_CurrentlyRunningScriptFile).GetPath() + _T('/') + filename;
            f.Open(fname);
            found = f.IsOpened();
        }

        if (!found)
        {
            // check in standard script dirs
            fname = ConfigManager::LocateDataFile(filename, sdScriptsUser | sdScriptsGlobal);
            f.Open(fname);
            if (!f.IsOpened())
            {
                Manager::Get()->GetLogManager()->DebugLog(_T("Can't open script ") + filename);
                return false;
            }
        }
    }
    // read file
    wxString contents = cbReadFileContents(f);
    m_CurrentlyRunningScriptFile = fname;
    bool ret = LoadBuffer(contents, fname);
    m_CurrentlyRunningScriptFile.Clear();
    return ret;
}

bool SCRIPTING_MANAGER_BASE::LoadBuffer(const wxString& buffer, const wxString& debugName)
{
    // includes guard to avoid recursion
    wxString incName = UnixFilename(debugName);
    if (m_IncludeSet.find(incName) != m_IncludeSet.end())
    {
        Manager::Get()->GetLogManager()->LogWarning(F(_T("Ignoring Include(\"%s\") because it would cause recursion..."), incName.wx_str()));
        return true;
    }
    m_IncludeSet.insert(incName);

//    wxCriticalSectionLocker c(cs);

    s_ScriptErrors.Clear();

    // compile script
    SquirrelObject script;
    try
    {
        script = SquirrelVM::CompileBuffer(cbU2C(buffer), cbU2C(debugName));
    }
    catch (SquirrelError e)
    {
        cbMessageBox(wxString::Format(_T("Filename: %s\nError: %s\nDetails: %s"), debugName.c_str(), cbC2U(e.desc).c_str(), s_ScriptErrors.c_str()), _("Script compile error"), wxICON_ERROR);
        m_IncludeSet.erase(incName);
        return false;
    }

    // run script
    try
    {
        SquirrelVM::RunScript(script);
    }
    catch (SquirrelError e)
    {
        cbMessageBox(wxString::Format(_T("Filename: %s\nError: %s\nDetails: %s"), debugName.c_str(), cbC2U(e.desc).c_str(), s_ScriptErrors.c_str()), _("Script run error"), wxICON_ERROR);
        m_IncludeSet.erase(incName);
        return false;
    }
    m_IncludeSet.erase(incName);
    return true;
}


wxString SCRIPTING_MANAGER_BASE::LoadBufferRedirectOutput(const wxString& buffer)
{
//    wxCriticalSectionLocker c(cs);

    s_ScriptErrors.Clear();
    ::capture.Clear();

    sq_setprintfunc(SquirrelVM::GetVMPtr(), CaptureScriptOutput);
    bool res = LoadBuffer(buffer);
    sq_setprintfunc(SquirrelVM::GetVMPtr(), ScriptsPrintFunc);

    return res ? ::capture : (wxString) wxEmptyString;
}

wxString SCRIPTING_MANAGER_BASE::GetErrorString(SquirrelError* exception, bool clearErrors)
{
    wxString msg;
    if (exception)
        msg << cbC2U(exception->desc);
    msg << s_ScriptErrors;

    if (clearErrors)
        s_ScriptErrors.Clear();

    return msg;
}

void SCRIPTING_MANAGER_BASE::DisplayErrors(SquirrelError* exception, bool clearErrors)
{
    wxString msg = GetErrorString(exception, clearErrors);
    if (!msg.IsEmpty())
        cbMessageBox(msg, _("Script errors"));
}

void SCRIPTING_MANAGER_BASE::InjectScriptOutput(const wxString& output)
{
    s_ScriptErrors << output;
}

int SCRIPTING_MANAGER_BASE::Configure()
{
    return -1;
}

bool SCRIPTING_MANAGER_BASE::IsScriptTrusted(const wxString& script)
{
    TrustedScripts::iterator it = m_TrustedScripts.find(script);
    if (it == m_TrustedScripts.end())
        return false;
    // check the crc too
    wxUint32 crc = wxCrc32::FromFile(script);
    if (crc == it->second.crc)
        return true;
    cbMessageBox(script + _T("\n\n") + _("The script was marked as \"trusted\" but it has been modified "
                    "since then.\nScript not trusted anymore."),
                _("Warning"), wxICON_WARNING);
    m_TrustedScripts.erase(it);
    return false;
}

bool SCRIPTING_MANAGER_BASE::IsCurrentlyRunningScriptTrusted()
{
    return IsScriptTrusted(m_CurrentlyRunningScriptFile);
}

void SCRIPTING_MANAGER_BASE::TrustScript(const wxString& script, bool permanently)
{
    // TODO: what should happen when script is empty()?

    TrustedScripts::iterator it = m_TrustedScripts.find(script);
    if (it != m_TrustedScripts.end())
    {
        // already trusted, remove it from the trusts (we recreate the trust below)
        m_TrustedScripts.erase(it);
    }

    TrustedScriptProps props;
    props.permanent = permanently;
    props.crc = wxCrc32::FromFile(script);

    m_TrustedScripts.insert(m_TrustedScripts.end(), std::make_pair(script, props));
}

void SCRIPTING_MANAGER_BASE::TrustCurrentlyRunningScript(bool permanently)
{
    TrustScript(m_CurrentlyRunningScriptFile, permanently);
}

bool SCRIPTING_MANAGER_BASE::RemoveTrust(const wxString& script)
{
    TrustedScripts::iterator it = m_TrustedScripts.find(script);
    if (it != m_TrustedScripts.end())
    {
        // already trusted, remove it from the trusts (we recreate the trust below)
        m_TrustedScripts.erase(it);
        return true;
    }
    return false;
}

void SCRIPTING_MANAGER_BASE::RefreshTrusts()
{
    // reload trusted scripts set
    m_TrustedScripts.clear();
    ConfigManagerContainer::StringToStringMap myMap;
    Manager::Get()->GetConfigManager(_T("security"))->Read(_T("/trusted_scripts"), &myMap);
    ConfigManagerContainer::StringToStringMap::iterator it;
    for (it = myMap.begin(); it != myMap.end(); ++it)
    {
        wxString key = it->second.BeforeFirst(_T('?'));
        wxString value = it->second.AfterFirst(_T('?'));

        TrustedScriptProps props;
        props.permanent = true;
        unsigned long tmp;
        value.ToULong(&tmp, 16);
        props.crc = tmp;
        m_TrustedScripts.insert(m_TrustedScripts.end(), std::make_pair(key, props));
    }
}

const SCRIPTING_MANAGER_BASE::TrustedScripts& SCRIPTING_MANAGER_BASE::GetTrustedScripts()
{
    return m_TrustedScripts;
}

