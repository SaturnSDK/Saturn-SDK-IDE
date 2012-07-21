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
    : Compiler(_("GNU GCC Compiler for AVR"),_T("avr-gcc"))
{
    m_Weight = 60;
    Reset();
}

CompilerGNUAVR::~CompilerGNUAVR()
{
    //dtor
}

Compiler * CompilerGNUAVR::CreateCopy()
{
    return (new CompilerGNUAVR(*this));
}

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
