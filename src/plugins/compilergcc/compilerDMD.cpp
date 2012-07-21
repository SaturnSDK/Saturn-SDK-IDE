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
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include "compilerDMD.h"

CompilerDMD::CompilerDMD()
    : Compiler(_("Digital Mars D Compiler"), _T("dmd"))
{
    m_Weight = 84;
    Reset();
}

CompilerDMD::~CompilerDMD()
{
    //dtor
}

Compiler * CompilerDMD::CreateCopy()
{
    return (new CompilerDMD(*this));
}

void CompilerDMD::Reset()
{
    m_Options.ClearOptions();
    LoadDefaultOptions(GetID());

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    if (!platform::windows)
    {
      m_LinkLibs.Add(_("pthread"));
      m_LinkLibs.Add(_("m"));
    }
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

AutoDetectResult CompilerDMD::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();

    // NOTE (hd#1#): dmc uses sc.ini for compiler's master directories
    // NOTE (mandrav#1#): which doesn't seem to exist if you don't have the CD version ;)

    // just a guess; the default installation dir
    wxString incPath;
    wxString libPath;
    wxString libName;
    if (platform::windows)
    {
        m_MasterPath = _T("C:\\dmd");
        incPath = m_MasterPath + sep + _T("src") + sep + _T("phobos");
        libPath = m_MasterPath + sep + _T("lib");
        libName = _T("phobos.lib");
        m_ExtraPaths.Add(_T("C:\\dm\\bin"));
    }
    else
    {
      m_MasterPath = wxFileExists(_T("/usr/local/bin/dmd")) ? _T("/usr/local") : _T("/usr");
      incPath = m_MasterPath + sep + _T("lib") + sep + _T("phobos");
      libPath = m_MasterPath + sep + _T("lib");
      libName = _T("phobos");
    }

    if (!m_MasterPath.IsEmpty())
    {
        AddIncludeDir(incPath);
        AddLibDir(libPath);
    }
    AddLinkLib(libName);

    return wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}
