/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 */

#include <sdk.h>
#include <prep.h>
#include "compilerPGIFortran.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include "manager.h"
#include "logmanager.h"

#include <configmanager.h>

#ifdef __WXMSW__
    #include <wx/msw/registry.h>
#endif

CompilerPGIFortran::CompilerPGIFortran()
    : Compiler(_("PGI Fortran Compiler"), _T("pgfortran"))
{
    Reset();
}

CompilerPGIFortran::~CompilerPGIFortran()
{
    //dtor
}

Compiler * CompilerPGIFortran::CreateCopy()
{
    Compiler* c = new CompilerPGIFortran(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

AutoDetectResult CompilerPGIFortran::AutoDetectInstallationDir()
{
    // try to find MinGW in environment variable PATH first
    wxString pathValues;
    wxGetEnv(_T("PATH"), &pathValues);
    if (!pathValues.IsEmpty())
    {
        wxString sep = platform::windows ? _T(";") : _T(":");
        wxChar pathSep = platform::windows ? _T('\\') : _T('/');
        wxArrayString pathArray = GetArrayFromString(pathValues, sep);
        for (size_t i = 0; i < pathArray.GetCount(); ++i)
        {
            if (wxFileExists(pathArray[i] + pathSep + m_Programs.C))
            {
                if (pathArray[i].AfterLast(pathSep).IsSameAs(_T("bin")))
                {
                    m_MasterPath = pathArray[i].BeforeLast(pathSep);
                    return adrDetected;
                }
            }
        }
    }

    wxString sep = wxFileName::GetPathSeparator();
    if (m_MasterPath.IsEmpty())
    {
        if (platform::windows)
        {
            // just a guess the default installation dir
            wxString Programs = _T("C:\\Program Files");
            wxGetEnv(_T("ProgramFiles"), &Programs);
            m_MasterPath = Programs + _T("\\PGI\\win32");
        }
        else
        {
            m_MasterPath = _T("/opt/pgi/linux86");
        }
    }

    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;

    //SetVersionString();
    return ret;
}

