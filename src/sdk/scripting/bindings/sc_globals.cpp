/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk_precomp.h>
#ifndef CB_PRECOMP
    #include <wx/string.h>
    #include <globals.h>
    #include <settings.h>
    #include <manager.h>
    #include <logmanager.h>
    #include <configmanager.h>
    #include <editormanager.h>
    #include <projectmanager.h>
    #include <pluginmanager.h>
#endif

#include "sc_base_types.h"

#include <wx/colordlg.h>
#include <wx/numdlg.h>
#include <wx/textdlg.h>
#include <infowindow.h>

namespace ScriptBindings
{
    EditorManager* getEM()
    {
        return Manager::Get()->GetEditorManager();
    }
    bool InstallPlugin(const wxString& pluginName, bool allUsers, bool confirm)
    {
        if (cbMessageBox(_("A script is trying to install a Code::Blocks plugin.\n"
                            "Do you wish to allow this?\n\n") + pluginName,
                            _("Security warning"), wxICON_WARNING | wxYES_NO) == wxID_NO)
        {
            return false;
        }
        return Manager::Get()->GetPluginManager()->InstallPlugin(pluginName, allUsers, confirm);
    }
    int ConfigurePlugin(const wxString& pluginName)
    {
        return Manager::Get()->GetPluginManager()->ConfigurePlugin(pluginName);
    }
    // locate and call a menu from string (e.g. "/Valgrind/Run Valgrind::MemCheck")
    void CallMenu(const wxString& menuPath)
    {
        // this code is partially based on MenuItemsManager::CreateFromString()
        wxMenuBar* mbar = Manager::Get()->GetAppFrame()->GetMenuBar();
        wxMenu* menu = 0;
        size_t pos = 0;
        while (true)
        {
            // ignore consecutive slashes
            while (pos < menuPath.Length() && menuPath.GetChar(pos) == _T('/'))
                ++pos;

            // find next slash
            size_t nextPos = pos;
            while (nextPos < menuPath.Length() && menuPath.GetChar(++nextPos) != _T('/'))
                ;

            wxString current = menuPath.Mid(pos, nextPos - pos);
            if (current.IsEmpty())
                break;
            bool isLast = nextPos >= menuPath.Length();
            // current holds the current search string

            if (!menu) // no menu yet? look in menubar
            {
                int menuPos = mbar->FindMenu(current);
                if (menuPos == wxNOT_FOUND)
                    break; // failed
                else
                    menu = mbar->GetMenu(menuPos);
            }
            else
            {
                if (isLast)
                {
                    int id = menu->FindItem(current);
                    if (id != wxNOT_FOUND)
                    {
                        wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, id);
                        #if wxCHECK_VERSION(2, 9, 0)
                        mbar->GetEventHandler()->ProcessEvent(evt);
                        #else
                        if ( !mbar->ProcessEvent(evt) )
                        {
                            wxString msg; msg.Printf(_("Calling the menu '%s' with ID %d failed."), menuPath.wx_str(), id);
                            cbMessageBox(msg, _("Script error"), wxICON_WARNING);
                        }
                        #endif
                        // done
                    }
                    break;
                }
                int existing = menu->FindItem(current);
                if (existing != wxNOT_FOUND)
                    menu = menu->GetMenuItems()[existing]->GetSubMenu();
                else
                    break; // failed
            }
            pos = nextPos; // prepare for next loop
        }
    }
    SQInteger wx_GetColourFromUser(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        const wxColour& c = sa.GetParamCount() == 2 ? *SqPlus::GetInstance<wxColour,false>(v, 2) : *wxBLACK;
        return SqPlus::ReturnCopy(v, wxGetColourFromUser(Manager::Get()->GetAppWindow(), c));
    }
    long wx_GetNumberFromUser(const wxString& message, const wxString& prompt, const wxString& caption, long value)
    {
        return wxGetNumberFromUser(message, prompt, caption, value);
    }
    wxString wx_GetPasswordFromUser(const wxString& message, const wxString& caption, const wxString& default_value)
    {
        return wxGetPasswordFromUser(message, caption, default_value);
    }
    wxString wx_GetTextFromUser(const wxString& message, const wxString& caption, const wxString& default_value)
    {
        return wxGetTextFromUser(message, caption, default_value);
    }


    void Register_Globals()
    {
        // global funcs
        SqPlus::RegisterGlobal(getEM, "GetEditorManager");
        SqPlus::RegisterGlobal(ConfigurePlugin, "ConfigureToolPlugin");
        SqPlus::RegisterGlobal(InstallPlugin, "InstallPlugin");
        SqPlus::RegisterGlobal(CallMenu, "CallMenu");
        SqPlus::RegisterGlobal(InfoWindow::Display, "InfoWindow");
        SquirrelVM::CreateFunctionGlobal(wx_GetColourFromUser, "wxGetColourFromUser", "*");
        SqPlus::RegisterGlobal(wx_GetNumberFromUser, "wxGetNumberFromUser");
        SqPlus::RegisterGlobal(wx_GetPasswordFromUser, "wxGetPasswordFromUser");
        SqPlus::RegisterGlobal(wx_GetTextFromUser, "wxGetTextFromUser");
    }
}
