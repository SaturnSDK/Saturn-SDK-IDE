/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/xrc/xmlres.h>
    #include <wx/msgdlg.h>
    #include "globals.h"
    #include <wx/checklst.h>
#endif

#include <wx/stattext.h>
#include <wx/textdlg.h>

#include "multiselectdlg.h"

BEGIN_EVENT_TABLE(MultiSelectDlg, wxDialog)
    EVT_CHECKLISTBOX(XRCID("lstItems"), MultiSelectDlg::OnItemToggle)
    EVT_BUTTON(XRCID("btnSelectWild"), MultiSelectDlg::OnWildcard)
    EVT_BUTTON(XRCID("btnToggle"), MultiSelectDlg::OnToggle)
END_EVENT_TABLE()

MultiSelectDlg::MultiSelectDlg(wxWindow* parent,
                                const wxArrayString& items,
                                const wxString& wildcard,
                                const wxString& label,
                                const wxString& title)
{
	//ctor
	wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgGenericMultiSelect"));

	SetTitle(title);
	XRCCTRL(*this, "lblLabel", wxStaticText)->SetLabel(label);
    Init(items, wildcard);
}

MultiSelectDlg::MultiSelectDlg(wxWindow* parent,
                                const wxArrayString& items,
                                bool selectall,
                                const wxString& label,
                                const wxString& title)
{
	//ctor
	wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgGenericMultiSelect"));

	SetTitle(title);
	XRCCTRL(*this, "lblLabel", wxStaticText)->SetLabel(label);
    Init(items, selectall ? _T("*") : _T(""));
}

MultiSelectDlg::~MultiSelectDlg()
{
	//dtor
}

void MultiSelectDlg::Init(const wxArrayString& items, const wxString& wildcard)
{
    wxCheckListBox* lst = XRCCTRL(*this, "lstItems", wxCheckListBox);
	for (size_t i = 0; i < items.GetCount(); ++i)
	{
        lst->Append(items[i]);
	}
	SelectWildCard(wildcard);
}

void MultiSelectDlg::UpdateStatus()
{
	int count = 0;
    wxCheckListBox* lst = XRCCTRL(*this, "lstItems", wxCheckListBox);
	for (int i = 0; i < lst->GetCount(); ++i)
	{
        if (lst->IsChecked(i))
            ++count;
	}
	wxString msg;
	msg << _("Selected: ") << wxString::Format(_T("%d"), count);
	XRCCTRL(*this, "lblStatus", wxStaticText)->SetLabel(msg);
}

wxArrayString MultiSelectDlg::GetSelectedStrings()
{
    wxArrayString ret;
    wxCheckListBox* lst = XRCCTRL(*this, "lstItems", wxCheckListBox);
	for (int i = 0; i < lst->GetCount(); ++i)
	{
        if (lst->IsChecked(i))
            ret.Add(lst->GetString(i));
	}
	return ret;
}

wxArrayInt MultiSelectDlg::GetSelectedIndices()
{
    wxArrayInt ret;
    wxCheckListBox* lst = XRCCTRL(*this, "lstItems", wxCheckListBox);
	for (int i = 0; i < lst->GetCount(); ++i)
	{
        if (lst->IsChecked(i))
            ret.Add(i);
	}
	return ret;
}

void MultiSelectDlg::SelectWildCard(const wxString& wild, bool select, bool clearOld)
{
    if (wild.IsEmpty())
        return;
	wxArrayString wilds = GetArrayFromString(wild, _T(";"));
    wxCheckListBox* lst = XRCCTRL(*this, "lstItems", wxCheckListBox);
	for (int i = 0; i < lst->GetCount(); ++i)
	{
		if (clearOld || !lst->IsChecked(i))
		{
            wxString entry = lst->GetString(i).Lower();
			for (unsigned int x = 0; x < wilds.GetCount(); ++x)
			{
				if (entry.Matches(wilds[x]))
				{
                    lst->Check(i, select);
                    break;
				}
			}
		}
	}
	UpdateStatus();
}

void MultiSelectDlg::OnWildcard(wxCommandEvent& event)
{
    wxString wild = wxGetTextFromUser(_("Enter a selection wildcard\n(e.g. \"dlg*.cpp\" "
                                        "would select all files starting with \"dlg\" and "
                                        "ending in \".cpp\")\nSeparate multiple masks with \";\":"),
                                        _("Wildcard selection"));
    if (wild.IsEmpty())
        return;

    bool clear = cbMessageBox(_("Do you want to clear the previous selections?"),
                            _("Question"),
                            wxICON_QUESTION | wxYES_NO) == wxYES;
    SelectWildCard(wild, true, clear);
}

void MultiSelectDlg::OnItemToggle(wxCommandEvent& event)
{
	UpdateStatus();
}

void MultiSelectDlg::OnToggle(wxCommandEvent& event)
{
    wxCheckListBox* lst = XRCCTRL(*this, "lstItems", wxCheckListBox);
	for (int i = 0; i < lst->GetCount(); ++i)
	{
        lst->Check(i, !lst->IsChecked(i));
	}
	UpdateStatus();
}
