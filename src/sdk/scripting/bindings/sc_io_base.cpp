/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6123 $
 * $Id: sc_io.cpp 6123 2010-01-31 03:52:19Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/scripting/bindings/sc_io.cpp $
 */

#include <sdk_precomp.h>
#include <manager.h>
#include <macrosmanager.h>
#include <configmanager.h>
#include <annoyingdialog.h>

#ifndef CB_PRECOMP
    #include <globals.h>
    #include <wx/string.h>
#endif

#include <wx/filename.h>
#include <wx/utils.h>

#include "sc_base_types.h"

namespace ScriptBindings
{
    namespace IOLib
    {
        // not exposed
        bool SecurityAllowsBase(const wxString& operation, const wxString& descr)
        {
            if (Manager::Get()->GetScriptingManager()->IsCurrentlyRunningScriptTrusted())
                return true;

            if (Manager::Get()->GetConfigManager(_T("security"))->ReadBool(operation, false))
                return true;

            return false;
        }

        wxString GetCwd()
        {
            return wxGetCwd();
        }

        void SetCwd(const wxString& dir)
        {
            wxSetWorkingDirectory(dir);
        }

        bool CreateDirRecursively(const wxString& full_path, int perms)
        {
            wxFileName fname(Manager::Get()->GetMacrosManager()->ReplaceMacros(full_path));
            NormalizePath(fname, wxEmptyString);
            if (!SecurityAllowsBase(_T("CreateDir"), fname.GetFullPath()))
                return false;
            return ::CreateDirRecursively(fname.GetFullPath(), perms);
        }


        bool RemoveDir(const wxString& src)
        {
            wxFileName fname(Manager::Get()->GetMacrosManager()->ReplaceMacros(src));
            NormalizePath(fname, wxEmptyString);
            if (!SecurityAllowsBase(_T("RemoveDir"), fname.GetFullPath()))
                return false;
            return wxRmdir(fname.GetFullPath());
        }

        bool DirectoryExists(const wxString& dir)
        {
            wxFileName fname(Manager::Get()->GetMacrosManager()->ReplaceMacros(dir));
            NormalizePath(fname, wxEmptyString);
            return wxDirExists(fname.GetFullPath());
        }

        bool CopyFile(const wxString& src, const wxString& dst, bool overwrite)
        {
            wxFileName fname1(Manager::Get()->GetMacrosManager()->ReplaceMacros(src));
            wxFileName fname2(Manager::Get()->GetMacrosManager()->ReplaceMacros(dst));
            NormalizePath(fname1, wxEmptyString);
            NormalizePath(fname2, wxEmptyString);
            if (!SecurityAllowsBase(_T("CopyFile"), wxString::Format(_T("%s -> %s"), src.c_str(), dst.c_str())))
                return false;
            if (!wxFileExists(fname1.GetFullPath())) return false;
            return wxCopyFile(fname1.GetFullPath(),
                            fname2.GetFullPath(),
                            overwrite);
        }

        bool RenameFile(const wxString& src, const wxString& dst)
        {
            wxFileName fname1(Manager::Get()->GetMacrosManager()->ReplaceMacros(src));
            wxFileName fname2(Manager::Get()->GetMacrosManager()->ReplaceMacros(dst));
            NormalizePath(fname1, wxEmptyString);
            NormalizePath(fname2, wxEmptyString);
            if (!SecurityAllowsBase(_T("RenameFile"), wxString::Format(_T("%s -> %s"),
                                            fname1.GetFullPath().c_str(), fname2.GetFullPath().c_str())))
                return false;
            if (!wxFileExists(fname1.GetFullPath())) return false;
            return wxRenameFile(fname1.GetFullPath(),
                                fname2.GetFullPath());
        }

        bool RemoveFile(const wxString& src)
        {
            wxFileName fname(Manager::Get()->GetMacrosManager()->ReplaceMacros(src));
            NormalizePath(fname, wxEmptyString);
            if (!SecurityAllowsBase(_T("RemoveFile"), fname.GetFullPath()))
                return false;
            if (!wxFileExists(fname.GetFullPath())) return false;
            return wxRemoveFile(fname.GetFullPath());
        }

        bool FileExists(const wxString& file)
        {
            wxFileName fname(Manager::Get()->GetMacrosManager()->ReplaceMacros(file));
            NormalizePath(fname, wxEmptyString);
            return wxFileExists(fname.GetFullPath());
        }


        wxString ReadFileContents(const wxString& filename)
        {
            wxFileName fname(Manager::Get()->GetMacrosManager()->ReplaceMacros(filename));
            NormalizePath(fname, wxEmptyString);
            wxFile f(fname.GetFullPath());
            return cbReadFileContents(f);
        }

        bool WriteFileContents(const wxString& filename, const wxString& contents)
        {
            wxFileName fname(Manager::Get()->GetMacrosManager()->ReplaceMacros(filename));
            NormalizePath(fname, wxEmptyString);
            if (!SecurityAllowsBase(_T("CreateFile"), fname.GetFullPath()))
                return false;
            wxFile f(fname.GetFullPath(), wxFile::write);
            return cbWrite(f, contents);
        }

        int Execute(const wxString& command)
        {
            if (!SecurityAllowsBase(_T("Execute"), command))
                return -1;
            wxArrayString output;
            return wxExecute(command, output, wxEXEC_NODISABLE);
        }

        wxString ExecuteAndGetOutput(const wxString& command)
        {
            if (!SecurityAllowsBase(_T("Execute"), command))
                return wxEmptyString;

            wxArrayString output;
            wxExecute(command, output, wxEXEC_NODISABLE);

            return GetStringFromArray(output, _T("\n"));
        }


        wxString ExecuteAndGetOutputAndError(const wxString& command, bool prepend_error = true)
        {
            if (!SecurityAllowsBase(_T("Execute"), command))
                return wxEmptyString;

            wxArrayString output;
            wxArrayString error;
            wxExecute(command, output, error, wxEXEC_NODISABLE);

            wxString str_out;

            if ( prepend_error && !error.IsEmpty())
                str_out += GetStringFromArray(error,  _T("\n"));

            if (!output.IsEmpty())
                str_out += GetStringFromArray(output, _T("\n"));

            if (!prepend_error && !error.IsEmpty())
                str_out += GetStringFromArray(error,  _T("\n"));

            return  str_out;
        }
    } // namespace IOLib
} // namespace ScriptBindings

namespace ScriptBindings
{
    struct IONamespace {};

    void Register_Base_IO()
    {
        SqPlus::SQClassDef<IONamespace>("IO").


                #ifndef NO_INSECURE_SCRIPTS
                staticFunc(&IOLib::CreateDirRecursively,        "CreateDirectory").
                staticFunc(&IOLib::RemoveDir,                   "RemoveDirectory").
                staticFunc(&IOLib::CopyFile,                    "CopyFile").
                staticFunc(&IOLib::RenameFile,                  "RenameFile").
                staticFunc(&IOLib::RemoveFile,                  "RemoveFile").
                staticFunc(&IOLib::WriteFileContents,           "WriteFileContents").
                staticFunc(&IOLib::Execute,                     "Execute").
                staticFunc(&IOLib::ExecuteAndGetOutput,         "ExecuteAndGetOutput").
                staticFunc(&IOLib::ExecuteAndGetOutputAndError, "ExecuteAndGetOutputAndError").
                #endif // NO_INSECURE_SCRIPTS

                staticFunc(&IOLib::GetCwd, "GetCwd").
                staticFunc(&IOLib::SetCwd, "SetCwd").

                staticFunc(&IOLib::DirectoryExists,  "DirectoryExists").
                staticFunc(&IOLib::FileExists,       "FileExists").
                staticFunc(&IOLib::ReadFileContents, "ReadFileContents");

        #ifndef NO_INSECURE_SCRIPTS
        SqPlus::BindConstant(true,  "allowInsecureScripts");
        #else
        SqPlus::BindConstant(false, "allowInsecureScripts");
        #endif // NO_INSECURE_SCRIPTS
    }
} // namespace ScriptBindings
