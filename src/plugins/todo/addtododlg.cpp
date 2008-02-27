/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk.h"
#ifndef CB_PRECOMP
  #include <wx/arrstr.h>
  #include <wx/choice.h>
  #include <wx/intl.h>
  #include <wx/spinctrl.h>
  #include <wx/textctrl.h>
  #include <wx/xrc/xmlres.h>
  #include "manager.h"
  #include "configmanager.h"
#endif
#include <wx/textdlg.h>
#include "addtododlg.h"

BEGIN_EVENT_TABLE(AddTodoDlg, wxDialog)
    EVT_BUTTON(XRCID("btAddUser"), AddTodoDlg::OnAddUser)
    EVT_BUTTON(XRCID("btDelUser"), AddTodoDlg::OnDelUser)
END_EVENT_TABLE()

AddTodoDlg::AddTodoDlg(wxWindow* parent, wxArrayString& types)
    : m_Types(types)
{
    wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgAddToDo"));
    LoadUsers();

    // load types
    wxChoice* cmb = XRCCTRL(*this, "chcType", wxChoice);
    cmb->Clear();
    for (unsigned int i = 0; i < m_Types.GetCount(); ++i)
    {
        cmb->Append(m_Types[i]);
    }
    if (m_Types.Index(_T("TODO")) == wxNOT_FOUND)
        cmb->Append(_T("TODO"));
    if (m_Types.Index(_T("@todo")) == wxNOT_FOUND)
        cmb->Append(_T("@todo"));
    if (m_Types.Index(_T("\\todo")) == wxNOT_FOUND)
        cmb->Append(_T("\\todo"));
    if (m_Types.Index(_T("FIXME")) == wxNOT_FOUND)
        cmb->Append(_T("FIXME"));
    if (m_Types.Index(_T("NOTE")) == wxNOT_FOUND)
        cmb->Append(_T("NOTE"));
    if (m_Types.Index(_T("@note")) == wxNOT_FOUND)
        cmb->Append(_T("@note"));
    if (m_Types.Index(_T("\\note")) == wxNOT_FOUND)
        cmb->Append(_T("\\note"));

    wxString lastType = Manager::Get()->GetConfigManager(_T("todo_list"))->Read(_T("last_used_type"));
    wxString lastStyle = Manager::Get()->GetConfigManager(_T("todo_list"))->Read(_T("last_used_style"));
    wxString lastPos = Manager::Get()->GetConfigManager(_T("todo_list"))->Read(_T("last_used_position"));
    if (!lastType.IsEmpty())
    {
        int sel = cmb->FindString(lastType);
        if (sel != -1)
            cmb->SetSelection(sel);
    }
    else
        cmb->SetSelection(0);


    cmb = XRCCTRL(*this, "chcStyle", wxChoice);
    if (!lastStyle.IsEmpty())
    {
        int sel = cmb->FindString(lastStyle);
        if (sel != -1)
            cmb->SetSelection(sel);
    }

    cmb = XRCCTRL(*this, "chcPosition", wxChoice);
    if (!lastPos.IsEmpty())
    {
        int sel = cmb->FindString(lastPos);
        if (sel != -1)
            cmb->SetSelection(sel);
    }
}

AddTodoDlg::~AddTodoDlg()
{
    //dtor
}

void AddTodoDlg::LoadUsers() const
{
    wxChoice* cmb = XRCCTRL(*this, "chcUser", wxChoice);

    wxArrayString users;
    Manager::Get()->GetConfigManager(_T("todo_list"))->Read(_T("users"), &users);

    cmb->Clear();
    cmb->Append(users);

    if (cmb->GetCount() == 0)
        cmb->Append(wxGetUserId());
    cmb->SetSelection(0);
}

void AddTodoDlg::SaveUsers() const
{
    wxChoice* cmb = XRCCTRL(*this, "chcUser", wxChoice);
    wxArrayString users;

    for (int i = 0; i < (int)cmb->GetCount(); ++i)
    {
        users.Add(cmb->GetString(i));
    }
    Manager::Get()->GetConfigManager(_T("todo_list"))->Write(_T("users"), users);
}

wxString AddTodoDlg::GetText() const
{
    return XRCCTRL(*this, "txtText", wxTextCtrl)->GetValue();
}

wxString AddTodoDlg::GetUser() const
{
    return XRCCTRL(*this, "chcUser", wxChoice)->GetStringSelection();
}

int AddTodoDlg::GetPriority() const
{
    int prio = XRCCTRL(*this, "spnPriority", wxSpinCtrl)->GetValue();
    if (prio < 1)
        prio = 1;
    else if (prio > 9)
        prio = 9;
    return prio;
}

ToDoPosition AddTodoDlg::GetPosition() const
{
    return (ToDoPosition)(XRCCTRL(*this, "chcPosition", wxChoice)->GetSelection());
}

wxString AddTodoDlg::GetType() const
{
    return XRCCTRL(*this, "chcType", wxChoice)->GetStringSelection();
}

ToDoCommentType AddTodoDlg::GetCommentType() const
{
    return (ToDoCommentType)(XRCCTRL(*this, "chcStyle", wxChoice)->GetSelection());
}

void AddTodoDlg::EndModal(int retVal)
{
    if (retVal == wxID_OK)
    {
        SaveUsers();

        // "save" types
        wxChoice* cmb = XRCCTRL(*this, "chcType", wxChoice);
        m_Types.Clear();
        if (cmb->FindString(cmb->GetStringSelection()) == wxNOT_FOUND)
            m_Types.Add(cmb->GetStringSelection());
        for (int i = 0; i < (int)cmb->GetCount(); ++i)
        {
            m_Types.Add(cmb->GetString(i));
        }

        Manager::Get()->GetConfigManager(_T("todo_list"))->Write(_T("last_used_type"), cmb->GetStringSelection());
        cmb = XRCCTRL(*this, "chcStyle", wxChoice);
        Manager::Get()->GetConfigManager(_T("todo_list"))->Write(_T("last_used_style"), cmb->GetStringSelection());
        cmb = XRCCTRL(*this, "chcPosition", wxChoice);
        Manager::Get()->GetConfigManager(_T("todo_list"))->Write(_T("last_used_position"), cmb->GetStringSelection());
    }

    wxDialog::EndModal(retVal);
}

void AddTodoDlg::OnAddUser(wxCommandEvent&)
{
    // ask for the new user to be added to the "choice" list
    wxTextEntryDialog dlg(this, _T("Enter the user you wish to add"), _T("Add user"), _T(""), wxOK|wxCANCEL);
    if(dlg.ShowModal() == wxID_OK)
    {
        wxString User = dlg.GetValue();
        if(!User.IsEmpty())
        {
            XRCCTRL(*this, "chcUser", wxChoice)->Append(User);
        }
    }
} // end of OnAddUser

void AddTodoDlg::OnDelUser(wxCommandEvent&)
{
    wxChoice* cmb = XRCCTRL(*this, "chcUser", wxChoice);
    int sel = cmb->GetCurrentSelection();
    if (sel == wxNOT_FOUND)
        return;

    wxString msg; msg.Printf(_T("Are you sure you want to delete the user '%s'?"), cmb->GetString(sel).c_str());
    if (cbMessageBox(msg, _T("Confirmation"), wxICON_QUESTION | wxYES_NO) == wxID_NO)
       return;

    cmb->Delete(sel);
    if (cmb->GetCount() == 0)
    {
        cmb->Append(wxGetUserId());
        cbMessageBox(_T("Default user has been re-added to list of users."), _T("Information"), wxICON_INFORMATION);
    }
    cmb->SetSelection(0);
} // end of OnDelUser
