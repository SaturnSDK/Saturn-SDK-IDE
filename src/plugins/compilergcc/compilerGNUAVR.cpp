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
#endif
#include "compilerGNUAVR.h"

CompilerGNUAVR::CompilerGNUAVR()
    : Compiler(_("GNU AVR GCC Compiler"),_T("avr-gcc"))
{
    Reset();
}

CompilerGNUAVR::~CompilerGNUAVR()
{
    //dtor
}

Compiler * CompilerGNUAVR::CreateCopy()
{
    Compiler* c = new CompilerGNUAVR(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
} // end of CreateCopy

void CompilerGNUAVR::Reset()
{
    m_Options.ClearOptions();
    LoadDefaultOptions(wxT("avr-gcc"));

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
} // end of Reset

void CompilerGNUAVR::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    LoadRegExArray(wxT("avr-gcc"));
} // end of LoadDefaultRegExArray

AutoDetectResult CompilerGNUAVR::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    if (platform::windows)
    {
        m_MasterPath = wxFindFirstFile(_T("C:\\WinAVR*"), wxDIR);
        if (m_MasterPath.empty())
        {
            m_MasterPath = _T("C:\\WinAVR");
        }
    }
    else
        m_MasterPath = _T("/usr");

    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        if (platform::windows)
        {
            AddIncludeDir(m_MasterPath + sep + _T("avr\\include"));
            AddLibDir(m_MasterPath + sep + _T("avr\\lib"));
            m_ExtraPaths.Add(m_MasterPath + sep + _T("utils") + sep + _T("bin")); // for make
        }
        else
        {
            AddIncludeDir(m_MasterPath + sep + _T("include"));
            AddLibDir(m_MasterPath + sep + _T("lib"));
        }
    }
    return ret;
} // end of AutoDetectInstallationDir
