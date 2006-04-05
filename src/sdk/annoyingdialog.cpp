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
    #include <wx/wx.h>
    #include "configmanager.h"
    #include "cbexception.h"
    #include "globals.h"
#endif

#include "annoyingdialog.h"

BEGIN_EVENT_TABLE(AnnoyingDialog, wxDialog)
    EVT_BUTTON(-1, AnnoyingDialog::OnButton)
END_EVENT_TABLE()

AnnoyingDialog::AnnoyingDialog(const wxString& caption, const wxString& message, const wxArtID icon,
                               dStyle style, int defaultReturn, bool separate,
                               const wxString& b1, const wxString& b2, const wxString& b3)
        : wxDialog(NULL, -1, caption, wxDefaultPosition, wxDefaultSize, wxCAPTION),
        cb(0),
        dontAnnoy(false),
        defRet(defaultReturn)
{
    ConfigManagerContainer::StringSet disabled = Manager::Get()->GetConfigManager(_T("an_dlg"))->ReadSSet(_T("/disabled"));
    if(disabled.find(caption) != disabled.end())
    {
        dontAnnoy = true;
        return;
    }

    wxBoxSizer *outerSizer = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer *mainArea = new wxFlexGridSizer(2, 0, 0);
    wxStaticBitmap *bitmap = new wxStaticBitmap(this, -1, wxArtProvider::GetBitmap(icon,  wxART_MESSAGE_BOX), wxDefaultPosition);
    mainArea->Add(bitmap, 0, wxALIGN_CENTER|wxALL, 10);

    wxStaticText *txt = new wxStaticText(this, -1, message, wxDefaultPosition, wxDefaultSize, 0);
    mainArea->Add( txt, 0, wxALIGN_CENTER|wxALL, 5 );

    mainArea->Add( 1, 1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    int numButtons = 0;
    int growableCol = 0;
    int id1 = -1;
    int id2 = -1;
    int id3 = -1;
    wxString bTxt1;
    wxString bTxt2;
    wxString bTxt3;

    if(style == OK || style == ONE_BUTTON)
    {
        numButtons = 1;
        growableCol = -1;
        id1 = (style == OK ? wxID_OK : 1);
        bTxt1 = b1.IsEmpty() ? wxString(_("&OK")) : b1;
    }
    else if(style == YES_NO || style == OK_CANCEL || style == TWO_BUTTONS)
    {
        numButtons = 2;
        growableCol = -1;
        id1 = (style == YES_NO ? wxID_YES : (style == OK_CANCEL ? wxID_OK     : 1));
        id2 = (style == YES_NO ? wxID_NO  : (style == OK_CANCEL ? wxID_CANCEL : 2));
        bTxt1 = b1.IsEmpty() ? (style == YES_NO ? wxString(_("&Yes")) : wxString(_("&OK")))     : b1;
        bTxt2 = b2.IsEmpty() ? (style == YES_NO ? wxString(_("&No"))  : wxString(_("&Cancel"))) : b2;
    }
    else if(style == YES_NO_CANCEL || style == THREE_BUTTONS)
    {
        numButtons = 3;
        growableCol = separate ? 1 : -1;
        id1 = (style == YES_NO_CANCEL ? wxID_YES    : 1);
        id2 = (style == YES_NO_CANCEL ? wxID_NO     : 2);
        id3 = (style == YES_NO_CANCEL ? wxID_CANCEL : 3);
        bTxt1 = b1.IsEmpty() ? wxString(_("&Yes")) : b1;
        bTxt2 = b2.IsEmpty() ? wxString(_("&No")) : b2;
        bTxt3 = b3.IsEmpty() ? wxString(_("&Cancel")) : b3;
    }
    else
        cbThrow(wxString(_T("Fatal error:\nUndefined style in dialog ")) << caption);

    wxFlexGridSizer *buttonArea = new wxFlexGridSizer(4, 0, 0);

    if(growableCol != -1)
        buttonArea->AddGrowableCol(growableCol);

    wxButton *but1 = new wxButton(this, id1, bTxt1, wxDefaultPosition, wxDefaultSize, 0);
    but1->SetDefault();
    buttonArea->Add(but1, 0, wxALIGN_CENTER|wxALL, 5);

    if(growableCol != -1)
        buttonArea->Add(1, 1, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5 );

    if(numButtons > 1)
    {
        wxButton *but2 = new wxButton(this, id2, bTxt2, wxDefaultPosition, wxDefaultSize, 0);
        but2->SetDefault();
        buttonArea->Add(but2, 0, wxALIGN_CENTER|wxTOP|wxBOTTOM|wxRIGHT, 5);
    }
    if(numButtons > 2)
    {
        wxButton *but3 = new wxButton(this, id3, bTxt3, wxDefaultPosition, wxDefaultSize, 0);
        but3->SetDefault();
        buttonArea->Add(but3, 0, wxALIGN_CENTER|wxTOP|wxBOTTOM|wxRIGHT, 5);
    }
//    mainArea->Add( buttonArea, 0, wxALIGN_CENTER|wxTOP, 10);

    outerSizer->Add( mainArea, 0, wxALIGN_CENTER|wxALL, 5);
    outerSizer->Add( buttonArea, 0, wxALIGN_CENTER);

    cb = new wxCheckBox(this, -1, _("Don't annoy me again!"), wxDefaultPosition, wxDefaultSize, 0);
    outerSizer->Add(cb, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    SetSizer( outerSizer );
    outerSizer->SetSizeHints(this);

    Centre();
}

void AnnoyingDialog::OnButton(wxCommandEvent& event)
{
    if(!cb)
        cbThrow(_T("Ow... null pointer."));

    if(cb->IsChecked())
    {
        ConfigManagerContainer::StringSet disabled = Manager::Get()->GetConfigManager(_T("an_dlg"))->ReadSSet(_T("/disabled"));
        disabled.insert(GetTitle());
        Manager::Get()->GetConfigManager(_T("an_dlg"))->Write(_T("/disabled"), disabled);
    }
    EndModal(event.GetId());
}

int AnnoyingDialog::ShowModal()
{
    if(dontAnnoy)
        return defRet;
    PlaceWindow(this);
    return wxDialog::ShowModal();
};
