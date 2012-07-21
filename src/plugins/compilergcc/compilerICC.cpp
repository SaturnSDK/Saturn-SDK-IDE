/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#ifndef CB_PRECOMP
	#include <wx/intl.h>
	#include <wx/msgdlg.h>
	#include <wx/regex.h>

	#include "compilerfactory.h"
	#include "logmanager.h"
	#include "manager.h"
#endif // CB_PRECOMP

#include <wx/config.h>
#include <wx/fileconf.h>

#include "compilerICC.h"

CompilerICC::CompilerICC()
    : Compiler(_("Intel C/C++ Compiler"), _T("icc"))
{
    m_Weight = 40;
    Reset();
}

CompilerICC::~CompilerICC()
{
    //dtor
}

Compiler * CompilerICC::CreateCopy()
{
    return (new CompilerICC(*this));
}

AutoDetectResult CompilerICC::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    wxString extraDir = _T("");
    if (platform::windows)
    {
        if (wxDirExists(_T("C:\\Program Files\\Intel\\Compiler")))
        {
            wxDir icc_dir(_T("C:\\Program Files\\Intel\\Compiler\\C++"));
            if (icc_dir.IsOpened())
            {
                wxArrayString dirs;
                wxIccDirTraverser IccDirTraverser(dirs);
                icc_dir.Traverse(IccDirTraverser);
                if (!dirs.IsEmpty())
                {
                    // Now sort the array in reverse order to get the latest version's path
                    dirs.Sort(true);
                    m_MasterPath = dirs[0];
                    m_MasterPath.Append(_T("\\IA32"));

                    // Now check for the installation of MSVC
                    const wxString msvcIds[4] = { _T("msvc6"),
                                                  _T("msvctk"),
                                                  _T("msvc8"),
                                                  _T("msvc10") };
                    bool msvcFound = false;
                    for (int i = 0; i < 4; ++i)
                    {
                        Compiler* vcComp = CompilerFactory::GetCompiler(msvcIds[i]);
                        if (vcComp)
                        {
                            if (vcComp->AutoDetectInstallationDir() == adrDetected)
                            {
                                const wxString& vcMasterPath = vcComp->GetMasterPath();
                                if (m_ExtraPaths.Index(vcMasterPath) == wxNOT_FOUND &&
                                    wxDirExists(vcMasterPath))
                                {
                                    m_ExtraPaths.Add(vcMasterPath);
                                }
                                AddIncludeDir(vcMasterPath + _T("\\Include"));
                                AddLibDir(vcMasterPath + _T("\\Lib"));
                                AddResourceIncludeDir(vcMasterPath + _T("\\Include"));

                                const wxArrayString& vcExtraPaths = vcComp->GetExtraPaths();
                                for (size_t i = 0; i < vcExtraPaths.GetCount(); ++i)
                                {
                                    if (m_ExtraPaths.Index(vcExtraPaths[i]) == wxNOT_FOUND &&
                                        wxDirExists(vcExtraPaths[i]))
                                    {
                                        m_ExtraPaths.Add(vcExtraPaths[i]);
                                    }
                                }
                                const wxArrayString& vcIncludeDirs = vcComp->GetIncludeDirs();
                                for (size_t i = 0; i < vcIncludeDirs.GetCount(); ++i)
                                {
                                    if (wxDirExists(vcIncludeDirs[i]))
                                    {
                                        if (m_IncludeDirs.Index(vcIncludeDirs[i]) == wxNOT_FOUND)
                                        {
                                            AddIncludeDir(vcIncludeDirs[i]);
                                        }
                                        if (m_ResIncludeDirs.Index(vcIncludeDirs[i]) == wxNOT_FOUND)
                                        {
                                            AddResourceIncludeDir(vcIncludeDirs[i]);
                                        }
                                    }
                                }
                                const wxArrayString& vcLibDirs = vcComp->GetLibDirs();
                                for (size_t i = 0; i < vcLibDirs.GetCount(); ++i)
                                {
                                    if (m_LibDirs.Index(vcLibDirs[i]) == wxNOT_FOUND &&
                                        wxDirExists(vcLibDirs[i]))
                                    {
                                        AddLibDir(vcLibDirs[i]);
                                    }
                                }
                                msvcFound = true;
                                break;
                            }
                        }
                    }

                    if (!msvcFound)
                    {
                        cbMessageBox(_T("It seems your computer doesn't have a working MSVC compiler.\n\n"
                                        "This compiler requires MS compiler for proper functioning and\n"
                                        "it may not work without it."),
                                     _T("Error"), wxOK | wxICON_ERROR);

                    }
                }
            }
        }

        // Read the ICPP_COMPILER90 environment variable
        wxGetEnv(_T("ICPP_COMPILER90"), &m_MasterPath);
        extraDir = sep + _T("IA32");// Intel also provides compiler for Itanium processors

        if (m_MasterPath.IsEmpty())
        {
            // just a guess the default installation dir
            wxString Programs = _T("C:\\Program Files");
            // what's the "Program Files" location
            // TO DO : support 64 bit ->    32 bit apps are in "ProgramFiles(x86)"
            //                              64 bit apps are in "ProgramFiles"
            wxGetEnv(_T("ProgramFiles"), &Programs);
            m_MasterPath = Programs + _T("\\Intel\\Compiler\\C++\\9.0");
        }
    }
    else
    {
        m_MasterPath = _T("/opt/intel/cc/9.0");
        if (wxDirExists(_T("/opt/intel")))
        {
            wxDir icc_dir(_T("/opt/intel/cc"));
            if (icc_dir.IsOpened())
            {
                wxArrayString dirs;
                wxIccDirTraverser IccDirTraverser(dirs);
                icc_dir.Traverse(IccDirTraverser);
                if (!dirs.IsEmpty())
                {
                    // Now sort the array in reverse order to get the latest version's path
                    dirs.Sort(true);
                    m_MasterPath = dirs[0];
                }
            }
        }
    }

    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        m_IncludeDirs.Insert(m_MasterPath + sep + _T("Include"), 0);
        m_LibDirs.Insert(m_MasterPath + sep + _T("Lib"), 0);
        m_ResIncludeDirs.Insert(m_MasterPath + sep + _T("Include"), 0);
    }
    // Try to detect the debugger. If not detected successfully the debugger plugin will
    // complain, so only the autodetection of compiler is considered in return value
    wxString path;
    wxString dbg;
    if (platform::windows)
    {
        dbg = _T("idb.exe");
        wxGetEnv(_T("IDB_PATH"), &path);
        path += _T("IDB\\9.0\\IA32");
    }
    else
    {
        dbg = _T("idb");
        path= _T("/opt/intel/idb/9.0");
        if (wxDirExists(_T("/opt/intel")))
        {
            wxDir icc_debug_dir(_T("/opt/intel/idb"));
            if (icc_debug_dir.IsOpened())
            {
                wxArrayString debug_dirs;
                wxIccDirTraverser IccDebugDirTraverser(debug_dirs);
                icc_debug_dir.Traverse(IccDebugDirTraverser);
                if (!debug_dirs.IsEmpty())
                {
                    // Now sort the array in reverse order to get the latest version's path
                    debug_dirs.Sort(true);
                    path = debug_dirs[0];
                }
            }
        }
    }

    if (wxFileExists(path + sep + _T("bin") + sep + dbg))
        m_ExtraPaths.Add(path);

    return ret;
}
