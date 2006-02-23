/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/xrc/xmlres.h>
    #include <wx/intl.h>
    #include <wx/textctrl.h>
    #include <wx/button.h>
    #include "toolsmanager.h"
    #include "globals.h"
    #include "cbexception.h"
#endif

#include "edittooldlg.h"
#include <wx/filedlg.h>


BEGIN_EVENT_TABLE(EditToolDlg, wxDialog)
	EVT_BUTTON(XRCID("btnBrowseCommand"), 	EditToolDlg::OnBrowseCommand)
	EVT_BUTTON(XRCID("btnBrowseDir"), 		EditToolDlg::OnBrowseDir)
	EVT_UPDATE_UI(-1,						EditToolDlg::OnUpdateUI)
END_EVENT_TABLE()

EditToolDlg::EditToolDlg(wxWindow* parent, Tool* tool)
	: m_Tool(tool)
{
    if (!tool)
        cbThrow(_T("Tool* parameter is mandatory in EditToolDlg()"));

	wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgEditTool"));
	XRCCTRL(*this, "txtName", wxTextCtrl)->SetValue(m_Tool->name);
	XRCCTRL(*this, "txtCommand", wxTextCtrl)->SetValue(m_Tool->command);
	XRCCTRL(*this, "txtParams", wxTextCtrl)->SetValue(m_Tool->params);
	XRCCTRL(*this, "txtDir", wxTextCtrl)->SetValue(m_Tool->workingDir);
}

EditToolDlg::~EditToolDlg()
{
	//dtor
}

// events

void EditToolDlg::OnUpdateUI(wxUpdateUIEvent& event)
{
	wxString name = XRCCTRL(*this, "txtName", wxTextCtrl)->GetValue();
	wxString command = XRCCTRL(*this, "txtCommand", wxTextCtrl)->GetValue();
	bool en = !name.IsEmpty() && !command.IsEmpty();
	XRCCTRL(*this, "wxID_OK", wxButton)->Enable(en);
}

void EditToolDlg::OnBrowseCommand(wxCommandEvent& event)
{
	wxFileName file(XRCCTRL(*this, "txtCommand", wxTextCtrl)->GetValue());
	wxString filename = wxFileSelector(_("Select executable"), file.GetPath(wxPATH_GET_VOLUME), file.GetFullName());
	if (!filename.IsEmpty())
		XRCCTRL(*this, "txtCommand", wxTextCtrl)->SetValue(filename);
}

void EditToolDlg::OnBrowseDir(wxCommandEvent& event)
{
    wxString dir = ChooseDirectory(this, _("Select working directory"), XRCCTRL(*this, "txtDir", wxTextCtrl)->GetValue());
	if (!dir.IsEmpty())
		XRCCTRL(*this, "txtDir", wxTextCtrl)->SetValue(dir);
}

void EditToolDlg::EndModal(int retCode)
{
    if (retCode == wxID_OK)
    {
        m_Tool->name = XRCCTRL(*this, "txtName", wxTextCtrl)->GetValue();
        m_Tool->command = XRCCTRL(*this, "txtCommand", wxTextCtrl)->GetValue();
        m_Tool->params = XRCCTRL(*this, "txtParams", wxTextCtrl)->GetValue();
        m_Tool->workingDir = XRCCTRL(*this, "txtDir", wxTextCtrl)->GetValue();
    }

	wxDialog::EndModal(retCode);
}
