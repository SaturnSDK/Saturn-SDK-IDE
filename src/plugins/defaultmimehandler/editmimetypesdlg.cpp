/***************************************************************
 * Name:      editmimetypesdlg.cpp
 * Purpose:   Window to edit the supported mime types handling
 * Author:    Yiannis An. Mandravellos<mandrav@codeblocks.org>
 * Created:   03/13/05 14:08:51
 * Copyright: (c) Yiannis An. Mandravellos
 * License:   GPL
 **************************************************************/

#include "editmimetypesdlg.h"
#include <wx/xrc/xmlres.h>
#include <wx/listbox.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/textdlg.h>
#include <configmanager.h>

BEGIN_EVENT_TABLE(EditMimeTypesDlg, wxDialog)
    EVT_BUTTON(XRCID("btnNew"), EditMimeTypesDlg::OnNew)
    EVT_BUTTON(XRCID("btnDelete"), EditMimeTypesDlg::OnDelete)
    EVT_BUTTON(XRCID("btnBrowse"), EditMimeTypesDlg::OnBrowseProgram)
    EVT_RADIOBOX(XRCID("rbOpen"), EditMimeTypesDlg::OnActionChanged)
    EVT_LISTBOX(XRCID("lstWild"), EditMimeTypesDlg::OnSelectionChanged)
END_EVENT_TABLE()

EditMimeTypesDlg::EditMimeTypesDlg(wxWindow* parent, MimeTypesArray& array)
    : m_Array(array),
    m_Selection(-1),
    m_LastSelection(-1)
{
	//ctor
	wxXmlResource::Get()->LoadDialog(this, parent, "dlgEditFilesHandling");
	FillList();
	UpdateDisplay();
}

EditMimeTypesDlg::~EditMimeTypesDlg()
{
	//dtor
}

void EditMimeTypesDlg::FillList()
{
    wxListBox* lst = XRCCTRL(*this, "lstWild", wxListBox);
    lst->Clear();
    for (size_t i = 0; i < m_Array.GetCount(); ++i)
    {
        cbMimeType* mt = m_Array[i];
        lst->Append(mt->wildcard);
    }
    
    m_Selection = m_Array.GetCount() != 0 ? 0 : -1;
    m_LastSelection = m_Selection;
}

void EditMimeTypesDlg::Save(int index)
{
    if (index == -1)
        return;
    cbMimeType* mt = m_Array[index];
    mt->wildcard = XRCCTRL(*this, "txtWild", wxTextCtrl)->GetValue().Lower();
    mt->useEditor = XRCCTRL(*this, "rbOpen", wxRadioBox)->GetSelection() == 1;
    mt->program = XRCCTRL(*this, "txtProgram", wxTextCtrl)->GetValue();
    mt->programIsModal = XRCCTRL(*this, "chkModal", wxCheckBox)->GetValue();
    // update list, in case the wildcard has changed
    XRCCTRL(*this, "lstWild", wxListBox)->SetString(index, mt->wildcard);
}

void EditMimeTypesDlg::UpdateDisplay()
{
    if (m_Selection == -1)
    {
        // disable everything
        XRCCTRL(*this, "txtWild", wxTextCtrl)->Enable(false);
        XRCCTRL(*this, "rbOpen", wxRadioBox)->Enable(false);
        XRCCTRL(*this, "txtProgram", wxTextCtrl)->Enable(false);
        XRCCTRL(*this, "btnBrowse", wxButton)->Enable(false);
        XRCCTRL(*this, "chkModal", wxCheckBox)->Enable(false);
        return;
    }

    if (m_LastSelection != -1 && m_LastSelection != m_Selection)
    {
        // selection changed; save changes
        Save(m_LastSelection);
    }

    cbMimeType* mt = m_Array[m_Selection];
    XRCCTRL(*this, "txtWild", wxTextCtrl)->SetValue(mt->wildcard);
    XRCCTRL(*this, "rbOpen", wxRadioBox)->SetSelection(mt->useEditor ? 1 : 0);
    XRCCTRL(*this, "txtProgram", wxTextCtrl)->SetValue(mt->program);
    XRCCTRL(*this, "chkModal", wxCheckBox)->SetValue(mt->programIsModal);

    XRCCTRL(*this, "txtWild", wxTextCtrl)->Enable(true);
    XRCCTRL(*this, "rbOpen", wxRadioBox)->Enable(true);
    XRCCTRL(*this, "txtProgram", wxTextCtrl)->Enable(!mt->useEditor);
    XRCCTRL(*this, "btnBrowse", wxButton)->Enable(!mt->useEditor);
    XRCCTRL(*this, "chkModal", wxCheckBox)->Enable(!mt->useEditor);
    
    m_LastSelection = m_Selection;
}

void EditMimeTypesDlg::OnSelectionChanged(wxCommandEvent& event)
{
    m_Selection = XRCCTRL(*this, "lstWild", wxListBox)->GetSelection();
    UpdateDisplay();
}

void EditMimeTypesDlg::OnActionChanged(wxCommandEvent& event)
{
    bool useEd = XRCCTRL(*this, "rbOpen", wxRadioBox)->GetSelection();
    XRCCTRL(*this, "txtProgram", wxTextCtrl)->Enable(!useEd);
    XRCCTRL(*this, "btnBrowse", wxButton)->Enable(!useEd);
    XRCCTRL(*this, "chkModal", wxCheckBox)->Enable(!useEd);
}

void EditMimeTypesDlg::OnNew(wxCommandEvent& event)
{
    wxString wild = wxGetTextFromUser(_("Enter the new wildcard to add:"));
    if (wild.IsEmpty())
        return;

    Save(m_Selection);

    cbMimeType* mt = new cbMimeType;
    mt->wildcard = wild;
    mt->useEditor = true;
    mt->program = "";
    mt->programIsModal = false;
    m_Array.Add(mt);

    FillList();
    m_Selection = m_Array.GetCount() - 1;
    m_LastSelection = m_Selection;
    UpdateDisplay();
}

void EditMimeTypesDlg::OnDelete(wxCommandEvent& event)
{
    if (m_Selection == -1)
        return;
    if (wxMessageBox(_("Are you sure you want to remove this wildcard?"), _("Confirmation"), wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT) == wxYES)
    {
        cbMimeType* mt = m_Array[m_Selection];
        m_Array.RemoveAt(m_Selection);
        delete mt;
    }
    FillList();
    UpdateDisplay();
}

void EditMimeTypesDlg::OnBrowseProgram(wxCommandEvent& event)
{
    wxFileDialog* dlg = new wxFileDialog(0,
                            _("Select program"),
                            wxEmptyString,
                            XRCCTRL(*this, "txtProgram", wxTextCtrl)->GetValue(),
                            ALL_FILES_FILTER,
                            wxOPEN);
    if (dlg->ShowModal() == wxID_OK)
        XRCCTRL(*this, "txtProgram", wxTextCtrl)->SetValue(dlg->GetPath());
}

void EditMimeTypesDlg::EndModal(int retCode)
{
    Save(m_Selection);
    wxDialog::EndModal(retCode);
}
