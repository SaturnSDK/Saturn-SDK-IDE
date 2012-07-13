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
#include "compilerDMC.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>

CompilerDMC::CompilerDMC()
    : Compiler(_("Digital Mars Compiler"), _T("dmc"))
{
    Reset();
}

CompilerDMC::~CompilerDMC()
{
    //dtor
}

Compiler * CompilerDMC::CreateCopy()
{
    Compiler* c = new CompilerDMC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

AutoDetectResult CompilerDMC::AutoDetectInstallationDir()
{
    // just a guess; the default installation dir
    m_MasterPath = _T("C:\\dm");
    wxString sep = wxFileName::GetPathSeparator();

    // NOTE (hd#1#): dmc uses sc.ini for compiler's master directories
    // NOTE (mandrav#1#): which doesn't seem to exist if you don't have the CD version ;)
    if (!m_MasterPath.IsEmpty())
    {
        AddIncludeDir(m_MasterPath + sep + _T("stlport") + sep + _T("stlport"));
        AddIncludeDir(m_MasterPath + sep + _T("include"));
        AddLibDir(m_MasterPath + sep + _T("lib"));
    }

    return wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}

#endif // __WXMSW__
