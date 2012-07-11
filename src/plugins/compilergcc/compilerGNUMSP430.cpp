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
#include <wx/msgdlg.h>
#ifdef __WXMSW__
#include <wx/msw/registry.h>
#endif // __WXMSW__
#include "compilerGNUMSP430.h"

CompilerGNUMSP430::CompilerGNUMSP430()
        : Compiler(_("GNU GCC Compiler for MSP430"),_T("msp430-gcc"))
{
    Reset();
}

CompilerGNUMSP430::~CompilerGNUMSP430()
{
    //dtor
}

Compiler * CompilerGNUMSP430::CreateCopy()
{
    Compiler* c = new CompilerGNUMSP430(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerGNUMSP430::Reset()
{
    m_Options.ClearOptions();
    LoadDefaultOptions(wxT("msp430-gcc"));

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerGNUMSP430::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    LoadRegExArray(wxT("msp430-gcc"));
}

AutoDetectResult CompilerGNUMSP430::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
#ifdef __WXMSW__
    m_MasterPath = _T("C:\\HighTec\\Msp430"); // just a guess

    //    wxLogNull ln;
        wxRegKey key; // defaults to HKCR
        key.SetName(_T("HKEY_LOCAL_MACHINE\\Software\\HighTec EDV-Systeme\\Msp430\\"));
        if (key.Exists() && key.Open(wxRegKey::Read))
        {
            // found; read it
            if (key.HasValue(_T("InstallPath")))
            {
                key.QueryValue(_T("InstallPath"), m_MasterPath);
            }
        }
#else
        m_MasterPath = _T("/usr/local/msp430");
#endif // __WXMSW__
    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
          AddIncludeDir(m_MasterPath + sep + _T("msp430") + sep + _T("include"));
    }
    return ret;
}
