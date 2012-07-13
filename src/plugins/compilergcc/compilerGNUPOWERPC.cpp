/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include <prep.h>
#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/regex.h>
    #include <wx/utils.h> // wxGetOSDirectory, wxGetEnv
#endif
#include <wx/filefn.h> // wxFileExists
#include <wx/fileconf.h> // wxFileConfig
#ifdef __WXMSW__
#include <wx/msw/registry.h>
#endif // __WXMSW__
#include "compilerGNUPOWERPC.h"

CompilerGNUPOWERPC::CompilerGNUPOWERPC()
        : Compiler(_("GNU GCC Compiler for PowerPC"),_T("ppc-gcc"))
{
    Reset();
}

CompilerGNUPOWERPC::~CompilerGNUPOWERPC()
{
    //dtor
}

Compiler * CompilerGNUPOWERPC::CreateCopy()
{
    Compiler* c = new CompilerGNUPOWERPC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

AutoDetectResult CompilerGNUPOWERPC::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
#ifdef __WXMSW__
     m_MasterPath = _T("C:\\HighTec\\PowerPC"); // just a guess

    //    wxLogNull ln;
    wxRegKey key; // defaults to HKCR
     key.SetName(_T("HKEY_LOCAL_MACHINE\\Software\\HighTec EDV-Systeme\\PowerPC\\"));
     if (key.Exists() && key.Open(wxRegKey::Read))
     {
     // found; read it
    if (key.HasValue(_T("InstallPath")))
        {
            key.QueryValue(_T("InstallPath"), m_MasterPath);
        }
      }
#else
     m_MasterPath = _T("/usr/local/ppc");
#endif // __WXMSW__
    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
//    if (ret == adrDetected)
//    {
//          AddIncludeDir(m_MasterPath + sep + _T("ppc-ht-eabi") + sep + _T("include"));
//    }
    return ret;
}
