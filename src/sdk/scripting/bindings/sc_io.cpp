/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk_precomp.h>
#include <manager.h>
#include <macrosmanager.h>
#include <configmanager.h>
#include <annoyingdialog.h>

#ifndef CB_PRECOMP
    #include <globals.h>
    #include <wx/string.h>
    #include <wx/filedlg.h>
#endif

#include <wx/filename.h>
#include <wx/utils.h>

#include "scriptsecuritywarningdlg.h"
#include "sc_base_types.h"

namespace ScriptBindings
{
    namespace IOLib
    {
        extern bool SecurityAllowsBase(const wxString& operation, const wxString& descr);
        // not exposed
        bool SecurityAllows(const wxString& operation, const wxString& descr)
        {
            if(/*ScriptBindings::IOLib::*/SecurityAllowsBase(operation, descr))
                return true;
            ScriptSecurityWarningDlg dlg(Manager::Get()->GetAppWindow(), operation, descr);
            if (dlg.ShowModal() != wxID_OK)
                return false;

            ScriptSecurityResponse response = dlg.GetResponse();
            switch (response)
            {
                case ssrAllow:
                    return true;

                case ssrAllowAll:
                    Manager::Get()->GetConfigManager(_T("security"))->Write(operation, true);
                    return true;

                case ssrTrust: // purposely fall through
                case ssrTrustPermanently:
                    Manager::Get()->GetScriptingManager()->TrustCurrentlyRunningScript(response == ssrTrustPermanently);
                    return true;

                default:
                    return false;
            }
            return false;
        }
        wxString ChooseDir(const wxString& message, const wxString& initialPath, bool showCreateDirButton)
        {
            return ChooseDirectory(0, message, Manager::Get()->GetMacrosManager()->ReplaceMacros(initialPath), wxEmptyString, false, showCreateDirButton);
        }
        wxString ChooseFile(const wxString& title, const wxString& defaultFile, const wxString& filter)
        {
            wxFileDialog dlg(0,
                            title,
                            wxEmptyString,
                            Manager::Get()->GetMacrosManager()->ReplaceMacros(defaultFile),
                            filter,
                            wxFD_OPEN | compatibility::wxHideReadonly);
            PlaceWindow(&dlg);
            if (dlg.ShowModal() == wxID_OK)
                return dlg.GetPath();
            return wxEmptyString;
        }
//<<<<<<< HEAD
//
//        wxString ReadFileContents(const wxString& filename)
//        {
//            wxFileName fname(Manager::Get()->GetMacrosManager()->ReplaceMacros(filename));
//            NormalizePath(fname, wxEmptyString);
//            wxFile f(fname.GetFullPath());
//            return cbReadFileContents(f);
//        }
//
//        bool WriteFileContents(const wxString& filename, const wxString& contents)
//        {
//            wxFileName fname(Manager::Get()->GetMacrosManager()->ReplaceMacros(filename));
//            NormalizePath(fname, wxEmptyString);
//            if (!SecurityAllows(_T("CreateFile"), fname.GetFullPath()))
//                return false;
//            wxFile f(fname.GetFullPath(), wxFile::write);
//            return cbWrite(f, contents);
//        }
//
//        int Execute(const wxString& command)
//        {
//            if (!SecurityAllows(_T("Execute"), command))
//                return -1;
//            wxArrayString output;
//            return wxExecute(command, output, wxEXEC_NODISABLE);
//        }
//
//        wxString ExecuteAndGetOutput(const wxString& command)
//        {
//            if (!SecurityAllows(_T("Execute"), command))
//                return wxEmptyString;
//
//            wxArrayString output;
//            wxExecute(command, output, wxEXEC_NODISABLE);
//
//            return GetStringFromArray(output, _T("\n"));
//        }
//
//        wxString ExecuteAndGetOutputAndError(const wxString& command, bool prepend_error = true)
//        {
//            if (!SecurityAllows(_T("Execute"), command))
//                return wxEmptyString;
//
//            wxArrayString output;
//            wxArrayString error;
//            wxExecute(command, output, error, wxEXEC_NODISABLE);
//
//            wxString str_out;
//
//            if ( prepend_error && !error.IsEmpty())
//                str_out += GetStringFromArray(error,  _T("\n"));
//
//            if (!output.IsEmpty())
//                str_out += GetStringFromArray(output, _T("\n"));
//
//            if (!prepend_error && !error.IsEmpty())
//                str_out += GetStringFromArray(error,  _T("\n"));
//
//            return  str_out;
//        }
//
//
//=======
//>>>>>>> * initial commit for new console branch; old one was broken because of too much merges and remerges in trunk
    } // namespace IOLib
} // namespace ScriptBindings

namespace ScriptBindings
{
    struct IONamespace {};

    void Register_IO()
    {
        SqPlus::SQClassDef<IONamespace>("IO").
//<<<<<<< HEAD
//
//                #ifndef NO_INSECURE_SCRIPTS
//                staticFunc(&IOLib::CreateDirRecursively,        "CreateDirectory").
//                staticFunc(&IOLib::RemoveDir,                   "RemoveDirectory").
//                staticFunc(&IOLib::CopyFile,                    "CopyFile").
//                staticFunc(&IOLib::RenameFile,                  "RenameFile").
//                staticFunc(&IOLib::RemoveFile,                  "RemoveFile").
//                staticFunc(&IOLib::WriteFileContents,           "WriteFileContents").
//                staticFunc(&IOLib::Execute,                     "Execute").
//                staticFunc(&IOLib::ExecuteAndGetOutput,         "ExecuteAndGetOutput").
//                staticFunc(&IOLib::ExecuteAndGetOutputAndError, "ExecuteAndGetOutputAndError").
//                #endif // NO_INSECURE_SCRIPTS
//
//                staticFunc(&IOLib::GetCwd, "GetCwd").
//                staticFunc(&IOLib::SetCwd, "SetCwd").
//
//                staticFunc(&IOLib::DirectoryExists,  "DirectoryExists").
//                staticFunc(&IOLib::ChooseDir,        "SelectDirectory").
//                staticFunc(&IOLib::FileExists,       "FileExists").
//                staticFunc(&IOLib::ChooseFile,       "SelectFile").
//                staticFunc(&IOLib::ReadFileContents, "ReadFileContents");
//
//        #ifndef NO_INSECURE_SCRIPTS
//        SqPlus::BindConstant(true,  "allowInsecureScripts");
//        #else
//        SqPlus::BindConstant(false, "allowInsecureScripts");
//        #endif // NO_INSECURE_SCRIPTS
//=======
                staticFunc(&IOLib::ChooseDir,        "SelectDirectory").
                staticFunc(&IOLib::ChooseFile,       "SelectFile");
//>>>>>>> * initial commit for new console branch; old one was broken because of too much merges and remerges in trunk
    }
} // namespace ScriptBindings
