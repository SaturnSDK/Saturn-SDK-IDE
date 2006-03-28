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

wxBitmap XButtonBitmap();
wxBitmap XButtonBitmap2();

const int ID_FS = wxNewId();
const int ID_DESTROY = wxNewId();

BEGIN_EVENT_TABLE(PairControl, wxPanel)
    EVT_COMMAND_ENTER(-1, PairControl::Enter)
    EVT_BUTTON(ID_FS, PairControl::FS)
END_EVENT_TABLE()

PairControl::PairControl(wxWindow* parent, const wxString& caption, const wxString& value, int fflags)
            : wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL), flags(fflags)
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
//    wxBell();
//else
//  Manager::Get()->GetMessageManager()->DebugLog(_T("ff"));

}

void PairControl::FS(wxCommandEvent& event)
{
wxFileDialog dlg(this, _T("Choose a file"), wxEmptyString, m_value->GetValue());
dlg.ShowModal();
m_value->SetValue(dlg.GetFilename());
}


wxBitmap XButtonBitmap()
{
    static const char *xpm[] =
    {
        "12 12 23 1",
        "  c red",
        ". c #B4B4B2",
        "X c #B5B5B3",
        "o c #BABAB8",
        "O c #BDBDBB",
        "+ c #BFBFBD",
        "@ c #C5C5C2",
        "# c #CECECC",
        "$ c #D2D2D0",
        "% c #DBDBD8",
        "& c #DFDFDC",
        "* c #E4E4E1",
        "= c #E7E7E4",
        "- c #E8E8E5",
        "; c #E8E8E6",
        ": c #EBEBE9",
        "> c #EFEFEC",
        ", c #F6F6F3",
        "< c #F8F8F6",
        "1 c #F9F9F7",
        "2 c #FBFBF9",
        "3 c gray100",
        "4 c None",
        /* pixels */
        "44:;====*%44",
        "4><2<2<<,;$4",
        ":<  ,,,,  &@",
        ";2   ,,   =+",
        "=<,      ,;O",
        "=2,,    ,,;O",
        "=<,,    ,,;O",
        "=2,      ,;O",
        "*,   ,,   *o",
        "%=  ,,,,  #.",
        "4$&=;;;;*#.4",
        "44@+OOOOo.44"
    };
    wxBitmap bmp(xpm);
    return bmp;
}

wxBitmap XButtonBitmap2()
{
    static const char *xpm[] =
    {
        "12 12 23 1",
        "  c red",
        ". c #A7A7A5",
        "X c #A8A8A6",
        "o c #ADADAB",
        "O c #B1B1AE",
        "+ c #B3B3B1",
        "@ c #B9B9B6",
        "# c #C2C2C0",
        "$ c #C7C7C5",
        "% c #D0D0CD",
        "& c #D4D4D1",
        "* c #DADAD6",
        "= c #DDDDDA",
        "- c #DEDEDB",
        "; c #DEDEDC",
        ": c #E1E1DF",
        "> c #E5E5E2",
        ", c #ECECE9",
        "< c #EEEEEC",
        "1 c #F0F0ED",
        "2 c #F2F2F0",
        "3 c gray100",
        "4 c None",
        /* pixels */
        "44.oOOOO+@44",
        "4X#*;;;;=&$4",
        ".#  ,,,,  =%",
        "o*   ,,   ,*",
        "O;,      ,1=",
        "O;,,    ,,1=",
        "O;,,    ,,1=",
        "O;,      ,1=",
        "+=   ,,   2;",
        "@&  ,,,,  <:",
        "4$=,11112<>4",
        "44%*====;:44"
    };
    wxBitmap bmp(xpm);
    return bmp;
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
    wxWindow *panel = b->GetParent();
    wxWindow *tree = panel->GetParent();
    tree->RemoveChild(panel);
    }
}
void PairControlPanel::Enter(wxCommandEvent& event)
{
}

