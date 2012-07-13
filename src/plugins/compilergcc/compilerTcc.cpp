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
    #include <wx/string.h>
#endif
#include <wx/filefn.h> // wxFileExists
#include "compilerTcc.h"

CompilerTcc::CompilerTcc()
    : Compiler(_("Tiny C Compiler"), _T("tcc"))
{
    Reset();
}

CompilerTcc::~CompilerTcc()
{
    //dtor
}

Compiler * CompilerTcc::CreateCopy()
{
    Compiler* c = new CompilerTcc(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
} // end of CreateCopy

AutoDetectResult CompilerTcc::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    if (platform::windows)
        m_MasterPath = _T("C:\\tcc");
    else
        m_MasterPath = _T("/usr");
    wxString BinPath = m_MasterPath + sep + wxT("tcc");
    AutoDetectResult ret = wxFileExists(BinPath + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        AddIncludeDir(m_MasterPath + sep + _T("include"));
        AddLibDir(m_MasterPath + sep + _T("lib"));
        m_ExtraPaths.Add(BinPath);
    }
    return ret;
} // end of AutoDetectInstallationDir
