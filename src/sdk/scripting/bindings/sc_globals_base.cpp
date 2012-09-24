/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6124 $
 * $Id: sc_globals.cpp 6124 2010-01-31 03:53:50Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/scripting/bindings/sc_globals.cpp $
 */

#include <sdk_precomp.h>
#ifndef CB_PRECOMP
    #include <wx/string.h>
    #include <globals.h>
    #include <settings.h>
    #include <manager.h>
    #include <logmanager.h>
    #include <configmanager.h>
    #include <projectmanager.h>
    #include <pluginmanager.h>
#endif

#include "sc_base_types.h"


namespace ScriptBindings
{
    // global funcs
    void gDebugLog(const wxString& msg){ Manager::Get()->GetLogManager()->DebugLog(msg); }
    void gErrorLog(const wxString& msg){ Manager::Get()->GetLogManager()->LogError(msg); }
    void gWarningLog(const wxString& msg){ Manager::Get()->GetLogManager()->LogWarning(msg); }
    void gLog(const wxString& msg){ Manager::Get()->GetLogManager()->Log(msg); }
    int gMessage(const wxString& msg, const wxString& caption, int buttons){ return cbMessageBox(msg, caption, buttons); }
    void gShowMessage(const wxString& msg){ cbMessageBox(msg, _("Script message")); }
    void gShowMessageWarn(const wxString& msg){ cbMessageBox(msg, _("Script warning"), wxICON_WARNING); }
    void gShowMessageError(const wxString& msg){ cbMessageBox(msg, _("Script error"), wxICON_ERROR); }
    void gShowMessageInfo(const wxString& msg){ cbMessageBox(msg, _("Script information"), wxICON_INFORMATION); }
    wxString gReplaceMacros(const wxString& buffer){ return Manager::Get()->GetMacrosManager()->ReplaceMacros(buffer); }

    SQInteger IsNull(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        SQUserPointer up = 0;
        sq_getinstanceup(v, 2, &up, 0);
        return sa.Return(up == 0L);
    }

    ProjectManager* getPM()
    {
        return Manager::Get()->GetProjectManager();
    }
    ConfigManager* getCM()
    {
        return Manager::Get()->GetConfigManager(_T("scripts"));
    }
    CompilerFactory* getCF()
    {
        static CompilerFactory cf; // all its members are static functions anyway
        return &cf;
    }
    UserVariableManager* getUVM()
    {
        return Manager::Get()->GetUserVariableManager();
    }
    ScriptingManager* getSM()
    {
        return Manager::Get()->GetScriptingManager();
    }
    int ExecutePlugin(const wxString& pluginName)
    {
        return Manager::Get()->GetPluginManager()->ExecutePlugin(pluginName);
    }
    void Include(const wxString& filename)
    {
        getSM()->LoadScript(filename);
    }
    SQInteger Require(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        const wxString& filename = *SqPlus::GetInstance<wxString,false>(v, 2);
        if (!getSM()->LoadScript(filename))
        {
            wxString msg = wxString::Format(_("Failed to load required script: %s"), filename.c_str());
            return sa.ThrowError(cbU2C(msg));
        }
        return sa.Return(static_cast<SQInteger>(0));
    }

    long wxString_ToLong(wxString const &str)
    {
        long value;
        if(!str.ToLong(&value))
            return -1;
        return value;
    }


    void Register_Base_Globals()
    {
        // global funcs
        SqPlus::RegisterGlobal(gLog, "Log");
        SqPlus::RegisterGlobal(gDebugLog, "LogDebug");
        SqPlus::RegisterGlobal(gWarningLog, "LogWarning");
        SqPlus::RegisterGlobal(gErrorLog, "LogError");

        SqPlus::RegisterGlobal(gMessage, "Message");
        SqPlus::RegisterGlobal(gShowMessage, "ShowMessage");
        SqPlus::RegisterGlobal(gShowMessageWarn, "ShowWarning");
        SqPlus::RegisterGlobal(gShowMessageError, "ShowError");
        SqPlus::RegisterGlobal(gShowMessageInfo, "ShowInfo");
        SqPlus::RegisterGlobal(gReplaceMacros, "ReplaceMacros");

        SqPlus::RegisterGlobal(getPM, "GetProjectManager");
        SqPlus::RegisterGlobal(getCM, "GetConfigManager");
        SqPlus::RegisterGlobal(getUVM, "GetUserVariableManager");
        SqPlus::RegisterGlobal(getSM, "GetScriptingManager");
        SqPlus::RegisterGlobal(getCF, "GetCompilerFactory");

        // from globals.h
        SqPlus::RegisterGlobal(GetArrayFromString, "GetArrayFromString");
        SqPlus::RegisterGlobal(GetStringFromArray, "GetStringFromArray");
        SqPlus::RegisterGlobal(EscapeSpaces, "EscapeSpaces");
        SqPlus::RegisterGlobal(UnixFilename, "UnixFilename");
        SqPlus::RegisterGlobal(FileTypeOf, "FileTypeOf");
        SqPlus::RegisterGlobal(URLEncode, "URLEncode");
        SqPlus::RegisterGlobal(NotifyMissingFile, "NotifyMissingFile");
        SqPlus::RegisterGlobal(GetPlatformsFromString, "GetPlatformsFromString");
        SqPlus::RegisterGlobal(GetStringFromPlatforms, "GetStringFromPlatforms");

        SqPlus::RegisterGlobal(ConfigManager::GetFolder, "GetFolder");
        SqPlus::RegisterGlobal(ConfigManager::LocateDataFile, "LocateDataFile");

        SqPlus::RegisterGlobal(ExecutePlugin, "ExecuteToolPlugin");

        SqPlus::RegisterGlobal(Include, "Include");
        SquirrelVM::CreateFunctionGlobal(Require, "Require", "*");


        SquirrelVM::CreateFunctionGlobal(IsNull, "IsNull", "*");

        // now for some wx globals (utility) functions
        SqPlus::RegisterGlobal(wxLaunchDefaultBrowser, "wxLaunchDefaultBrowser");

        SqPlus::RegisterGlobal(wxString_ToLong, "wxString_ToLong");
    }
}
