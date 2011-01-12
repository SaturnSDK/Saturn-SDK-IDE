/***************************************************************
 * Name:      SpeedTestMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ron Collins (rcoll@theriver.com)
 * Created:   2010-01-15
 * Copyright: Ron Collins ()
 * License:
 **************************************************************/
/*
* This file is part of a wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* wxSmith and this file is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*
* Ron Collins
* rcoll@theriver.com
* 4-Feb-2010
*
*/
#include "SpeedTestMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(SpeedTestFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(SpeedTestFrame)
const long SpeedTestFrame::ID_STATICTEXT1 = wxNewId();
const long SpeedTestFrame::ID_STATICTEXT2 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON1 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON2 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON3 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON4 = wxNewId();
const long SpeedTestFrame::ID_PANEL2 = wxNewId();
const long SpeedTestFrame::ID_STATICTEXT3 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON5 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON6 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON7 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON8 = wxNewId();
const long SpeedTestFrame::ID_PANEL3 = wxNewId();
const long SpeedTestFrame::ID_STATICTEXT4 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON9 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON10 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON11 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON12 = wxNewId();
const long SpeedTestFrame::ID_PANEL4 = wxNewId();
const long SpeedTestFrame::ID_STATICTEXT5 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON13 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON14 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON15 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON16 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON17 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON18 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON19 = wxNewId();
const long SpeedTestFrame::ID_SPEEDBUTTON20 = wxNewId();
const long SpeedTestFrame::ID_PANEL5 = wxNewId();
const long SpeedTestFrame::ID_PANEL1 = wxNewId();
const long SpeedTestFrame::idMenuQuit = wxNewId();
const long SpeedTestFrame::idMenuAbout = wxNewId();
//*)

BEGIN_EVENT_TABLE(SpeedTestFrame,wxFrame)
    //(*EventTable(SpeedTestFrame)
    //*)
END_EVENT_TABLE()

SpeedTestFrame::SpeedTestFrame(wxWindow* parent,wxWindowID id)
{

    //(*Initialize(SpeedTestFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(443,600));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(88,128), wxSize(443,600), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Description"), wxPoint(8,16), wxSize(424,13), 0, _T("ID_STATICTEXT1"));
    Panel2 = new wxPanel(Panel1, ID_PANEL2, wxPoint(8,48), wxSize(424,88), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    Panel2->SetBackgroundColour(wxColour(255,255,0));
    StaticText2 = new wxStaticText(Panel2, ID_STATICTEXT2, _("These are simple buttons, all they do is click and they always\nappear UP"), wxPoint(8,8), wxSize(400,26), 0, _T("ID_STATICTEXT2"));
    wxBitmap SpeedButton1_BMP(ARROW1R_xpm);
    SpeedButton1 = new wxSpeedButton(Panel2, ID_SPEEDBUTTON1, _("Simple-1"), SpeedButton1_BMP, 0, 2, 0, true, wxPoint(16,48), wxDefaultSize, wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON1"));
    SpeedButton1->SetUserData(0);
    wxBitmap SpeedButton2_BMP(ARROW1L_xpm);
    SpeedButton2 = new wxSpeedButton(Panel2, ID_SPEEDBUTTON2, _("Simple-2"), SpeedButton2_BMP, 0, 2, 0, true, wxPoint(112,48), wxDefaultSize, wxBU_RIGHT|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON2"));
    SpeedButton2->SetUserData(0);
    wxBitmap SpeedButton3_BMP(ARROW1U_xpm);
    SpeedButton3 = new wxSpeedButton(Panel2, ID_SPEEDBUTTON3, _("Simple-3"), SpeedButton3_BMP, 0, 2, 0, true, wxPoint(216,32), wxSize(72,40), wxBU_BOTTOM|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON3"));
    SpeedButton3->SetUserData(0);
    wxBitmap SpeedButton4_BMP(ARROW1D_xpm);
    SpeedButton4 = new wxSpeedButton(Panel2, ID_SPEEDBUTTON4, _("Simple-4"), SpeedButton4_BMP, 0, 2, 0, true, wxPoint(304,32), wxSize(72,40), wxBU_TOP|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON4"));
    SpeedButton4->SetUserData(0);
    Panel3 = new wxPanel(Panel1, ID_PANEL3, wxPoint(8,146), wxSize(424,102), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    Panel3->SetBackgroundColour(wxColour(0,255,255));
    StaticText3 = new wxStaticText(Panel3, ID_STATICTEXT3, _("These are toggle buttons, they can be UP or DOWN without effecting \nany other button"), wxPoint(8,8), wxSize(420,24), 0, _T("ID_STATICTEXT3"));
    wxBitmap SpeedButton5_BMP(ARROW2R_xpm);
    SpeedButton5 = new wxSpeedButton(Panel3, ID_SPEEDBUTTON5, _("Toggle-1"), SpeedButton5_BMP, 0, 2, -1, true, wxPoint(16,64), wxDefaultSize, wxBU_LEFT|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON5"));
    SpeedButton5->SetUserData(0);
    wxBitmap SpeedButton6_BMP(ARROW2L_xpm);
    SpeedButton6 = new wxSpeedButton(Panel3, ID_SPEEDBUTTON6, _("Toggle-2"), SpeedButton6_BMP, 0, 2, -1, true, wxPoint(112,64), wxDefaultSize, wxBU_RIGHT|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON6"));
    SpeedButton6->SetUserData(0);
    wxBitmap SpeedButton7_BMP(ARROW2U_xpm);
    SpeedButton7 = new wxSpeedButton(Panel3, ID_SPEEDBUTTON7, _("Toggle-3"), SpeedButton7_BMP, 0, 2, -1, true, wxPoint(216,48), wxSize(72,40), wxBU_BOTTOM|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON7"));
    SpeedButton7->SetUserData(0);
    wxBitmap SpeedButton8_BMP(ARROW2D_xpm);
    SpeedButton8 = new wxSpeedButton(Panel3, ID_SPEEDBUTTON8, _("Toggle-4"), SpeedButton8_BMP, 0, 2, -1, true, wxPoint(312,48), wxSize(72,40), wxBU_TOP|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON8"));
    SpeedButton8->SetUserData(0);
    Panel4 = new wxPanel(Panel1, ID_PANEL4, wxPoint(8,256), wxSize(424,112), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    Panel4->SetBackgroundColour(wxColour(0,255,0));
    StaticText4 = new wxStaticText(Panel4, ID_STATICTEXT4, _("These are group toggle buttons; the single group is defined by the\npanel they are placed on; only one button may be DOWN at a time,\nand there must always be a DOWN button"), wxPoint(8,8), wxSize(420,40), 0, _T("ID_STATICTEXT4"));
    wxBitmap SpeedButton9_BMP(ARROW3R_xpm);
    SpeedButton9 = new wxSpeedButton(Panel4, ID_SPEEDBUTTON9, _("Group-1"), SpeedButton9_BMP, 0, 2, -2, false, wxPoint(16,72), wxDefaultSize, wxBU_LEFT|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON9"));
    SpeedButton9->SetUserData(0);
    wxBitmap SpeedButton10_BMP(ARROW3L_xpm);
    SpeedButton10 = new wxSpeedButton(Panel4, ID_SPEEDBUTTON10, _("Group-2"), SpeedButton10_BMP, 0, 2, -2, false, wxPoint(112,72), wxDefaultSize, wxBU_RIGHT|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON10"));
    SpeedButton10->SetDown(true);
    SpeedButton10->SetUserData(0);
    wxBitmap SpeedButton11_BMP(ARROW3U_xpm);
    SpeedButton11 = new wxSpeedButton(Panel4, ID_SPEEDBUTTON11, _("Group-3"), SpeedButton11_BMP, 0, 2, -2, false, wxPoint(216,56), wxSize(72,40), wxBU_BOTTOM|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON11"));
    SpeedButton11->SetUserData(0);
    wxBitmap SpeedButton12_BMP(ARROW3D_xpm);
    SpeedButton12 = new wxSpeedButton(Panel4, ID_SPEEDBUTTON12, _("Group-4"), SpeedButton12_BMP, 0, 2, -2, false, wxPoint(312,56), wxSize(72,40), wxBU_TOP|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON12"));
    SpeedButton12->SetUserData(0);
    Panel5 = new wxPanel(Panel1, ID_PANEL5, wxPoint(8,376), wxSize(424,184), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    Panel5->SetBackgroundColour(wxColour(255,0,255));
    StaticText5 = new wxStaticText(Panel5, ID_STATICTEXT5, _("These are group buttons, defined by indexes 10 and 20; the group\nindex numbers and the button groups are totaly independant of each\nother. Group-10 buttons must always have one button DOWN, while\ngroup-20 buttons may have all buttons UP."), wxPoint(8,8), wxSize(408,64), 0, _T("ID_STATICTEXT5"));
    wxBitmap SpeedButton13_BMP(FILECLOS_xpm);
    SpeedButton13 = new wxSpeedButton(Panel5, ID_SPEEDBUTTON13, _("X-10-1"), SpeedButton13_BMP, 0, 2, 10, false, wxPoint(16,96), wxDefaultSize, wxBU_LEFT|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON13"));
    SpeedButton13->SetUserData(0);
    wxBitmap SpeedButton14_BMP(FILENEW_xpm);
    SpeedButton14 = new wxSpeedButton(Panel5, ID_SPEEDBUTTON14, _("X-10-2"), SpeedButton14_BMP, 0, 2, 10, false, wxPoint(112,96), wxDefaultSize, wxBU_RIGHT|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON14"));
    SpeedButton14->SetUserData(0);
    wxBitmap SpeedButton15_BMP(FILEOPEN_xpm);
    SpeedButton15 = new wxSpeedButton(Panel5, ID_SPEEDBUTTON15, _("X-10-3"), SpeedButton15_BMP, 0, 2, 10, false, wxPoint(224,72), wxSize(72,48), wxBU_BOTTOM|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON15"));
    SpeedButton15->SetUserData(0);
    wxBitmap SpeedButton16_BMP(FILESAVE_xpm);
    SpeedButton16 = new wxSpeedButton(Panel5, ID_SPEEDBUTTON16, _("X-10-4"), SpeedButton16_BMP, 0, 2, 10, false, wxPoint(320,72), wxSize(72,48), wxBU_TOP|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON16"));
    SpeedButton16->SetUserData(0);
    wxBitmap SpeedButton17_BMP(CDDRIVE_xpm);
    SpeedButton17 = new wxSpeedButton(Panel5, ID_SPEEDBUTTON17, _("X-20-1"), SpeedButton17_BMP, 0, 2, 20, true, wxPoint(16,152), wxDefaultSize, wxBU_LEFT|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON17"));
    SpeedButton17->SetUserData(0);
    wxBitmap SpeedButton18_BMP(COMPMAC_xpm);
    SpeedButton18 = new wxSpeedButton(Panel5, ID_SPEEDBUTTON18, _("X-20-2"), SpeedButton18_BMP, 0, 2, 20, true, wxPoint(112,152), wxDefaultSize, wxBU_RIGHT|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON18"));
    SpeedButton18->SetUserData(0);
    wxBitmap SpeedButton19_BMP(COMPPC1_xpm);
    SpeedButton19 = new wxSpeedButton(Panel5, ID_SPEEDBUTTON19, _("X-20-3"), SpeedButton19_BMP, 0, 2, 20, true, wxPoint(224,128), wxSize(72,48), wxBU_BOTTOM|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON19"));
    SpeedButton19->SetUserData(0);
    wxBitmap SpeedButton20_BMP(COMPPC2_xpm);
    SpeedButton20 = new wxSpeedButton(Panel5, ID_SPEEDBUTTON20, _("X-20-4"), SpeedButton20_BMP, 0, 2, 20, true, wxPoint(320,128), wxSize(72,48), wxBU_TOP|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_SPEEDBUTTON20"));
    SpeedButton20->SetUserData(0);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);

    Connect(ID_SPEEDBUTTON1,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON1,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON2,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON2,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON3,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON3,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON4,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON4,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON5,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON5,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON6,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON6,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON7,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON7,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON8,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON8,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON9,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON9,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON10,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON10,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON11,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON11,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON12,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON12,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON13,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON13,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON14,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON14,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON15,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON15,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON16,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON16,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON17,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON17,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON18,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON18,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON19,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON19,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON20,wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(ID_SPEEDBUTTON20,wxEVT_COMMAND_RIGHT_CLICK,(wxObjectEventFunction)&SpeedTestFrame::OnSpeedButton1LeftClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SpeedTestFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SpeedTestFrame::OnAbout);
    //*)





}

SpeedTestFrame::~SpeedTestFrame()
{
    //(*Destroy(SpeedTestFrame)
    //*)
}

void SpeedTestFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void SpeedTestFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

//------------------------------------------------------------------------------

void SpeedTestFrame::SpeedButtonEvent(wxCommandEvent& event) {
wxString    s,t;
wxSpeedButton   *b;

    b = (wxSpeedButton *) event.GetEventObject();
    s = b->GetName();
    if (event.GetInt() > 0) s = s + _(" Left");
    else                    s = s + _(" Right");
    if (b->GetDown()) s = s + _(" Down");
    else              s = s + _(" Up");

    StaticText1->SetLabel(s);


}

//------------------------------------------------------------------------------

void SpeedTestFrame::OnSpeedButton1LeftClick(wxCommandEvent& event) {
int         n;
wxString    sn;
wxString    st;
wxString    sp;
wxString    sd;
wxString    s,t;
wxSpeedButton   *b;

// the button

    b  = (wxSpeedButton *) event.GetEventObject();

// the ID

    n  = b->GetId();
    sn.Printf(_("Button %d; "), n);

// the label

    st = b->GetLabel();
    st = st + _("; ");

// left or right

    if (event.GetInt() > 0) sp = _(" Left Click; ");
    else                    sp = _(" Right Click; ");

// up or down

    if (b->GetDown()) sd = _(" Button DOWN");
    else              sd = _(" Button UP");

// put it all together

    s = sn + st + sp + sd;
    StaticText1->SetLabel(s);
}


