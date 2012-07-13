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
#include "compilerLDC.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include "manager.h"
#include "logmanager.h"

#include <configmanager.h>

CompilerLDC::CompilerLDC()
    : Compiler(_("LLVM D Compiler"), _T("ldc"))
{
    Reset();
}

CompilerLDC::~CompilerLDC()
{
    //dtor
}

Compiler * CompilerLDC::CreateCopy()
{
    Compiler* c = new CompilerLDC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

AutoDetectResult CompilerLDC::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    m_MasterPath = wxFileExists(_T("/usr/local/bin/ldc")) ? _T("/usr/local") : _T("/usr");

    AddIncludeDir(m_MasterPath + sep + _T("import"));
    AddLibDir(m_MasterPath + sep + _T("lib"));

    return wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}
