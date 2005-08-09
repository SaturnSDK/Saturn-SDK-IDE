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
* $Id$
* $Date$
*/

#include "confirmreplacedlg.h"
#include "configmanager.h"
#include <wx/stc/stc.h>
#include <wx/xrc/xmlres.h>
#include <wx/stattext.h>

BEGIN_EVENT_TABLE(ConfirmReplaceDlg, wxDialog)
	EVT_BUTTON(XRCID("btnYes"), 	ConfirmReplaceDlg::OnYes)
	EVT_BUTTON(XRCID("btnNo"), 		ConfirmReplaceDlg::OnNo)
	EVT_BUTTON(XRCID("btnAll"), 	ConfirmReplaceDlg::OnAll)
	EVT_BUTTON(XRCID("btnCancel"),	ConfirmReplaceDlg::OnCancel)
END_EVENT_TABLE()

ConfirmReplaceDlg::ConfirmReplaceDlg(wxWindow* parent, const wxString& label)
{
	wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgConfirmReplace"));
	XRCCTRL(*this, "lblMessage", wxStaticText)->SetLabel(label);
}

ConfirmReplaceDlg::~ConfirmReplaceDlg()
{
}

void ConfirmReplaceDlg::OnYes(wxCommandEvent& event)
{
	EndModal(crYes);
}

void ConfirmReplaceDlg::OnNo(wxCommandEvent& event)
{
	EndModal(crNo);
}

void ConfirmReplaceDlg::OnAll(wxCommandEvent& event)
{
	EndModal(crAll);
}

void ConfirmReplaceDlg::OnCancel(wxCommandEvent& event)
{
	EndModal(crCancel);
}

void ConfirmReplaceDlg::CalcPosition(wxStyledTextCtrl* ed)
{
	if(!ed) 
        return;

	int w = 0, h = 0;
	GetSize(&w,&h);

	wxPoint pt = ed->PointFromPosition(ed->GetCurrentPos());
	int ed_width = ed->GetSize().x;
	pt.x = (ed_width - w) / 2;
	if(pt.x < 0)
        pt.x = 0;
	pt = ed->ClientToScreen(pt);
	
	int lineHeight = ed->TextHeight(ed->GetCurrentLine());
	pt.y += lineHeight;
	
	int screenW = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	int screenH = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
	// sanity check
	if (w > screenW)
		w = screenW;
	if (h > screenH)
		h = screenH;
	
	// now we 're where we want to be, but check that the whole window is visible...
	// the main goal here is that the caret *should* be visible...
	
	// for the horizontal axis, easy stuff
	if (pt.x + w > screenW)
		pt.x = screenW - w;

	// for the vertical axis, more work has to be done...
	if (pt.y + h > screenH)
	{
		// it doesn't fit to the bottom of the screen
		// check if it fits to the top
		if (h < pt.y)
			pt.y -= h + lineHeight; // fits
		else
		{
			// we have to shrink the height...
			// determine if pt.y is closer to top or bottom
			if (pt.y <= screenH / 2)
				h = screenH = pt.y; // to top
			else
			{
				h = pt.y - lineHeight; // to bottom
				pt.y = 0;
			}
		}
	}
	// we should be OK now
	SetSize(pt.x, pt.y, w, h);
}
