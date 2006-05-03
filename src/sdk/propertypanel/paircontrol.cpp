/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"
#include "paircontrol.h"

#ifndef CB_PRECOMP
    #include <wx/wx.h>
    #include <wx/bitmap.h>
    #include <wx/button.h>
    #include <wx/bmpbuttn.h>
    #include "globals.h"
    #include "cbexception.h"
#endif

#include "paircontrol.h"
#include "xbuttonbitmap.h"


const int ID_FS = wxNewId();
const int ID_DESTROY = wxNewId();

BEGIN_EVENT_TABLE(PairControl, wxPanel)
    EVT_COMMAND_ENTER(-1, PairControl::Enter)
    EVT_BUTTON(ID_FS, PairControl::FS)
END_EVENT_TABLE()

PairControl::PairControl(wxWindow* parent, const wxString& caption, const wxString& value, int fflags)
            : wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxTE_PROCESS_ENTER), flags(fflags)
{
    cbAssert(parent > 0);

    wxFlexGridSizer *s = new wxFlexGridSizer(4, 0, 0);
    s->AddGrowableCol(1);

    if(flags & RENAME)
    {
        wxTextCtrl *t = new wxTextCtrl(this, -1, caption, wxDefaultPosition, wxSize(100,-1), wxNO_BORDER );
        s->Add(t, 0, wxALL, 2);
    }
    else
    {
        wxStaticText *t = new wxStaticText(this, -1, caption, wxDefaultPosition, wxSize(100,-1), 0 );
        wxFont f = t->GetFont();
        f.SetWeight(wxFONTWEIGHT_BOLD);
        t->SetFont(f);
        s->Add(t, 0, wxALL, 2);
    }

    m_value = new wxTextCtrl( this, -1, value, wxDefaultPosition, wxSize(180,-1), wxTE_PROCESS_ENTER);
    s->Add(m_value, 0, wxALL, 2);

    if(flags & FILESELECT)
        s->Add(new wxButton(this, ID_FS, _T("..."), wxDefaultPosition, wxSize(20,20), 0 ), 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 10 );
    else
        s->Add( 20, 20, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 10 );

    if(flags & DEL)
        {
        wxBitmapButton *bb = new wxBitmapButton(this, ID_DESTROY, XButtonBitmap(), wxDefaultPosition, wxSize(12,12), wxNO_BORDER);
        bb->SetBitmapSelected(XButtonBitmap2());
        s->Add(bb, 0, wxALIGN_CENTER, 5 );
        }
    else
        s->Add( 12, 12, 0, wxALIGN_CENTER, 10 );


    SetSizer(s);
}

void PairControl::Enter(wxCommandEvent& event)
{
//  Manager::Get()->GetMessageManager()->DebugLog(_T("enter"));
//
//if(flags & ADD)
    wxBell();
//else
//  Manager::Get()->GetMessageManager()->DebugLog(_T("ff"));

}

void PairControl::FS(wxCommandEvent& event)
{
wxFileDialog dlg(this, _T("Choose a file"), wxEmptyString, m_value->GetValue());
dlg.ShowModal();
m_value->SetValue(dlg.GetFilename());
}


BEGIN_EVENT_TABLE(PairControlPanel, wxPanel)
    EVT_BUTTON(ID_DESTROY, PairControlPanel::DeletePair)
    EVT_COMMAND_ENTER(-1, PairControlPanel::Enter)
END_EVENT_TABLE()

void PairControlPanel::DeletePair(wxCommandEvent& event)
{
    wxButton *b = (wxButton *) event.GetEventObject();
    if(b)
    {
    wxWindow *parent = GetParent();
    parent->RemoveChild(this);
    }
}
void PairControlPanel::Enter(wxCommandEvent& event)
{
}

