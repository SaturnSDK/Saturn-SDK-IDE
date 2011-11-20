/***************************************************************
 * Name:      TestTreeListMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2010-02-09
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef TESTTREELISTMAIN_H
#define TESTTREELISTMAIN_H

//(*Headers(TestTreeListFrame)
#include <wx/menu.h>
#include <wx/panel.h>
#include <treelistctrl.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class TestTreeListFrame: public wxFrame
{
    public:

        TestTreeListFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~TestTreeListFrame();

    private:

        //(*Handlers(TestTreeListFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(TestTreeListFrame)
        static const long ID_TREELISTCTRL1;
        static const long ID_PANEL1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(TestTreeListFrame)
        wxTreeItemId    TreeListCtrl1ID[8];
        wxPanel* Panel1;
        wxStatusBar* StatusBar1;
        wxTreeListCtrl* TreeListCtrl1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // TESTTREELISTMAIN_H
