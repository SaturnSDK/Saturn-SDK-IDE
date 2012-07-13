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
#include "compilerBCC.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/msw/registry.h>

CompilerBCC::CompilerBCC()
    : Compiler(_("Borland C++ Compiler (5.5, 5.82)"), _T("bcc"))
{
    Reset();
}

CompilerBCC::~CompilerBCC()
{
	//dtor
}

Compiler * CompilerBCC::CreateCopy()
{
    Compiler* c = new CompilerBCC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

AutoDetectResult CompilerBCC::AutoDetectInstallationDir()
{
    wxArrayString l_MasterPath_Arr, l_RegKey_Arr;
    // Fill assumed masterpaths and reg keys
    l_MasterPath_Arr.Add(_T("C:\\Program Files\\Borland\\BDS\\4.0"));
    l_MasterPath_Arr.Add(_T("C:\\Borland\\BCC55"));
    l_RegKey_Arr.Add(_T("HKEY_LOCAL_MACHINE\\SOFTWARE\\Borland\\BDS\\4.0"));
    l_RegKey_Arr.Add(_T("HKEY_LOCAL_MACHINE\\SOFTWARE\\Borland\\C++Builder\\5.0"));

    // try to detect Installation dir
    int match = -1;
    for (int i = 0; i < 2; ++i)
    {
        wxRegKey tkey(l_RegKey_Arr[i]);
        if (tkey.Exists() || wxDirExists(l_MasterPath_Arr[i]))
        {
            match = i;
            break;
        }
    }
    if (match < 0)
        match = 0;
    m_MasterPath = l_MasterPath_Arr[match];
    wxRegKey key(l_RegKey_Arr[match]);
    if(key.Exists() && key.Open(wxRegKey::Read))
    {
        wxString dir;
        key.QueryValue(_T("RootDir"), dir);
        if (!dir.IsEmpty() && wxDirExists(dir))
        {
            m_MasterPath = dir;
        }
        key.Close();
    }

    wxString sep = wxString(wxFileName::GetPathSeparator());
    if (!m_MasterPath.IsEmpty())
    {
        AddIncludeDir(m_MasterPath + sep + _T("include"));
        AddLibDir(m_MasterPath + sep + _T("lib"));
        AddLibDir(m_MasterPath + sep + _T("lib") + sep + _T("psdk"));
    }

    return wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}

#endif // __WXMSW__
