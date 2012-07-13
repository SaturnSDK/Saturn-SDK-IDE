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
#include "compilerSDCC.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>

#ifdef __WXMSW__
    #include <wx/msw/registry.h>
#endif

CompilerSDCC::CompilerSDCC()
    : Compiler(_("SDCC Compiler"), _T("sdcc"))
{
    Reset();
}

CompilerSDCC::~CompilerSDCC()
{
    //dtor
}

Compiler * CompilerSDCC::CreateCopy()
{
    Compiler* c = new CompilerSDCC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

AutoDetectResult CompilerSDCC::AutoDetectInstallationDir()
{
    if (platform::windows)
    {
#ifdef __WXMSW__ // for wxRegKey
        wxRegKey key;   // defaults to HKCR
        key.SetName(wxT("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\SDCC"));
        if (key.Exists() && key.Open(wxRegKey::Read)) // found; read it
            key.QueryValue(wxT("UninstallString"), m_MasterPath);
#endif

        if (m_MasterPath.IsEmpty())
            // just a guess; the default installation dir
            m_MasterPath = wxT("C:\\sdcc");
        else {
            wxFileName fn(m_MasterPath);
            m_MasterPath = fn.GetPath();
        }

        if (!m_MasterPath.IsEmpty())
        {
            AddIncludeDir(m_MasterPath + wxFILE_SEP_PATH + wxT("include"));
            AddLibDir(m_MasterPath + wxFILE_SEP_PATH + wxT("lib"));
            m_ExtraPaths.Add(m_MasterPath + wxFILE_SEP_PATH + wxT("bin"));
        }
    }
    else
        m_MasterPath=_T("/usr/local/bin"); // default

    return wxFileExists(m_MasterPath + wxFILE_SEP_PATH + wxT("bin") + wxFILE_SEP_PATH + m_Programs.C) ? adrDetected : adrGuessed;
}
