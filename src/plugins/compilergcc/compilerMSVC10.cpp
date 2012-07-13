/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#ifdef __WXMSW__
// this compiler is valid only in windows

#include <sdk.h>
#include "compilerMSVC10.h"
#include <wx/wx.h>
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/msw/registry.h>

CompilerMSVC10::CompilerMSVC10()
    : Compiler(_("Microsoft Visual C++ 2010"), _T("msvc10"))
{
    Reset();
}

CompilerMSVC10::~CompilerMSVC10()
{
    //dtor
}

Compiler * CompilerMSVC10::CreateCopy()
{
    Compiler* c = new CompilerMSVC10(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

AutoDetectResult CompilerMSVC10::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    wxString idepath;

    // Read the VS2010 environment variable
    wxGetEnv(_T("VS100COMNTOOLS"), &m_MasterPath);

    if ( !m_MasterPath.IsEmpty() )
    {
        wxFileName name = wxFileName::DirName(m_MasterPath);

        name.RemoveLastDir();
        name.AppendDir(_T("IDE"));
        idepath = name.GetPath();
        if ( !wxDirExists(idepath) )
            idepath = _T("");

        name.RemoveLastDir();
        name.RemoveLastDir();
        name.AppendDir(_T("VC"));
        m_MasterPath = name.GetPath();
        if ( !wxDirExists(m_MasterPath) )
            m_MasterPath = _T("");
    }

    if (m_MasterPath.IsEmpty())
    {
        // just a guess; the default installation dir
        wxString Programs = _T("C:\\Program Files");
        // what's the "Program Files" location
        wxGetEnv(_T("ProgramFiles"), &Programs);
        m_MasterPath = Programs + _T("\\Microsoft Visual Studio 10.0\\VC");
        idepath = Programs + _T("\\Microsoft Visual Studio 10.0\\Common7\\IDE");
    }

    if (!m_MasterPath.IsEmpty())
    {
        wxRegKey key; // defaults to HKCR
        bool sdkfound = false;
        wxString dir;

        // we need to add the IDE path, as the compiler requires some DLL present there
        m_ExtraPaths.Add(idepath);

        // try to detect Platform SDK (old versions)
        key.SetName(_T("HKEY_CURRENT_USER\\Software\\Microsoft\\Win32SDK\\Directories"));
        if (key.Exists() && key.Open(wxRegKey::Read))
        {
            key.QueryValue(_T("Install Dir"), dir);
            if (!dir.IsEmpty() && wxDirExists(dir))
                sdkfound = true;
            key.Close();
        }

        // try to detect Platform SDK (newer versions)
        wxString msPsdkKeyName[2] = { _T("HKEY_CURRENT_USER\\Software\\Microsoft\\MicrosoftSDK\\InstalledSDKs"),
                                      _T("HKEY_CURRENT_USER\\Software\\Microsoft\\Microsoft SDKs\\Windows") };
        wxString msPsdkKeyValue[2] = { _T("Install Dir"), _T("InstallationFolder") };
        for (int i = 0; i < 2; ++i)
        {
            if (sdkfound)
                break;

            key.SetName(msPsdkKeyName[i]);
            if (key.Exists() && key.Open(wxRegKey::Read))
            {
                wxString name;
                long idx;
                bool cont = key.GetFirstKey(name, idx);

                while(cont)
                {
                    wxRegKey subkey(key.GetName(), name);

                    if (subkey.Open(wxRegKey::Read) &&
                        (subkey.QueryValue(msPsdkKeyValue[i], dir), !dir.IsEmpty()) &&
                        wxDirExists(dir))
                    {
                        sdkfound = true;
                        cont = false;
                    }
                    else
                        cont = key.GetNextKey(name, idx);

                    subkey.Close();
                }
                key.Close();
            }
        }

        // take a guess
        if (!sdkfound)
        {
            dir = _T("C:\\Program Files");
            wxGetEnv(_T("ProgramFiles"), &dir);
            dir +=  _T("\\Microsoft SDKs\\Windows\\v7.0A");
            if (wxDirExists(dir))
                sdkfound = true;
        }

        // add include dirs for MS Platform SDK too (let them come before compiler's path)
        if (sdkfound)
        {
            if (dir.GetChar(dir.Length() - 1) != '\\')
                dir += sep;
            AddIncludeDir(dir + _T("include"));
            AddResourceIncludeDir(dir + _T("include"));
            AddLibDir(dir + _T("lib"));
            m_ExtraPaths.Add(dir + _T("bin"));
        }

        // now the compiler's include directories
        AddIncludeDir(m_MasterPath + sep + _T("include"));
        AddLibDir(m_MasterPath + sep + _T("lib"));
        AddResourceIncludeDir(m_MasterPath + sep + _T("include"));

        // add extra paths for "Debugging tools" too
        key.SetName(_T("HKEY_CURRENT_USER\\Software\\Microsoft\\DebuggingTools"));
        if (key.Exists() && key.Open(wxRegKey::Read))
        {
            key.QueryValue(_T("WinDbg"), dir);
            if (!dir.IsEmpty() && wxDirExists(dir))
            {
                if (dir.GetChar(dir.Length() - 1) == '\\')
                    dir.Remove(dir.Length() - 1, 1);
                m_ExtraPaths.Add(dir);
            }
        }
        key.Close();
    }

    return wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}

#endif // __WXMSW__
