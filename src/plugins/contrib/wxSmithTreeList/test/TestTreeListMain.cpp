/***************************************************************
 * Name:      TestTreeListMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2010-02-09
 * Copyright:  ()
 * License:
 **************************************************************/

#include "TestTreeListMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(TestTreeListFrame)
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

//(*IdInit(TestTreeListFrame)
const long TestTreeListFrame::ID_TREELISTCTRL1 = wxNewId();
const long TestTreeListFrame::ID_PANEL1 = wxNewId();
const long TestTreeListFrame::idMenuQuit = wxNewId();
const long TestTreeListFrame::idMenuAbout = wxNewId();
const long TestTreeListFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(TestTreeListFrame,wxFrame)
    //(*EventTable(TestTreeListFrame)
    //*)
END_EVENT_TABLE()

TestTreeListFrame::TestTreeListFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(TestTreeListFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(380,420));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(120,128), wxSize(380,420), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    TreeListCtrl1 = new wxTreeListCtrl(Panel1, ID_TREELISTCTRL1, wxPoint(16,16), wxSize(352,392), wxTR_EDIT_LABELS|wxTR_FULL_ROW_HIGHLIGHT|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER, wxDefaultValidator, _T("ID_TREELISTCTRL1"));
    TreeListCtrl1->AddColumn(_("First"), 64);
    TreeListCtrl1->AddColumn(_("Second"), 64);
    TreeListCtrl1->AddColumn(_("Third"), 64);
    TreeListCtrl1->AddColumn(_("Fourth"), 64);
    TreeListCtrl1->AddColumn(_("Fifth"), 64);
    TreeListCtrl1ID[0] = TreeListCtrl1->AddRoot(_("(root)"));
    TreeListCtrl1ID[1] = TreeListCtrl1->AppendItem(TreeListCtrl1ID[0], _("First level, no tabs"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[1], 1, _("Second Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[1], 2, _("Third Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[1], 3, _("Fourth Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[1], 4, _("Fifth Column"));
    TreeListCtrl1ID[2] = TreeListCtrl1->AppendItem(TreeListCtrl1ID[1], _("Second level, 1 tab"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[2], 1, _("Second Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[2], 2, _("Third Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[2], 3, _("Fourth Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[2], 4, _("Fifth Column"));
    TreeListCtrl1ID[3] = TreeListCtrl1->AppendItem(TreeListCtrl1ID[2], _("Third level, 2 tabs"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[3], 1, _("Second Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[3], 2, _("Third Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[3], 3, _("Fourth Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[3], 4, _("Fifth Column"));
    TreeListCtrl1ID[4] = TreeListCtrl1->AppendItem(TreeListCtrl1ID[0], _("First level again, no tabs"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[4], 1, _("Second Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[4], 2, _("Third Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[4], 3, _("Fourth Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[4], 4, _("Fifth Column"));
    TreeListCtrl1ID[5] = TreeListCtrl1->AppendItem(TreeListCtrl1ID[4], _("Second level again, 1 tab"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[5], 1, _("Second Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[5], 2, _("Third Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[5], 3, _("Fourth Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[5], 4, _("Fifth Column"));
    TreeListCtrl1ID[6] = TreeListCtrl1->AppendItem(TreeListCtrl1ID[4], _("Another second level, 1 tab"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[6], 1, _("Second Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[6], 2, _("Third Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[6], 3, _("Fourth Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[6], 4, _("Fifth Column"));
    TreeListCtrl1ID[7] = TreeListCtrl1->AppendItem(TreeListCtrl1ID[6], _("Third level again, 2 tabs"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[7], 1, _("Second Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[7], 2, _("Third Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[7], 3, _("Fourth Column"));
    TreeListCtrl1->SetItemText(TreeListCtrl1ID[7], 4, _("Fifth Column"));
    TreeListCtrl1->ExpandAll(TreeListCtrl1ID[0]);
    TreeListCtrl1->SetColumnEditable(0, false);
    TreeListCtrl1->SetColumnEditable(1, false);
    TreeListCtrl1->SetColumnEditable(2, false);
    TreeListCtrl1->SetColumnEditable(3, false);
    TreeListCtrl1->SetColumnEditable(4, false);
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
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&TestTreeListFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&TestTreeListFrame::OnAbout);
    //*)
}

TestTreeListFrame::~TestTreeListFrame()
{
    //(*Destroy(TestTreeListFrame)
    //*)
}

void TestTreeListFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void TestTreeListFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
