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
    } // namespace IOLib
} // namespace ScriptBindings

namespace ScriptBindings
{
    struct IONamespace {};

    void Register_IO()
    {
        SqPlus::SQClassDef<IONamespace>("IO").
                staticFunc(&IOLib::ChooseDir, "SelectDirectory").
                staticFunc(&IOLib::ChooseFile, "SelectFile");
    }
} // namespace ScriptBindings
