#ifndef WXWIDGETSGUIAPPADOPTINGDLG_H
#define WXWIDGETSGUIAPPADOPTINGDLG_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

//(*Headers(wxWidgetsGUIAppAdoptingDlg)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/gauge.h>
#include <wx/intl.h>
#include <wx/listbox.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
//*)

class wxWidgetsGUI;

class wxWidgetsGUIAppAdoptingDlg: public wxDialog
{
	public:

		wxWidgetsGUIAppAdoptingDlg(wxWindow* parent,wxWidgetsGUI* GUI,wxWindowID id = -1);
		virtual ~wxWidgetsGUIAppAdoptingDlg();

		//(*Identifiers(wxWidgetsGUIAppAdoptingDlg)
		enum Identifiers
		{
		    ID_LISTBOX1 = 0x1000,
		    ID_GAUGE1,
		    ID_STATICTEXT1,
		    ID_STATICTEXT2,
		    ID_BUTTON5,
		    ID_BUTTON2,
		    ID_BUTTON3,
		    ID_STATICLINE2,
		    ID_BUTTON4,
		    ID_STATICLINE1,
		    ID_BUTTON6
		};
		//*)

		wxString m_RelativeFileName;

	protected:

		//(*Handlers(wxWidgetsGUIAppAdoptingDlg)
		void OnButton6Click(wxCommandEvent& event);
		void OnButton4Click(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnTimer(wxTimerEvent& event);
		void OnUseFileBtnClick(wxCommandEvent& event);
		void OnSelectBtnClick(wxCommandEvent& event);
		void OnCreateBtnClick(wxCommandEvent& event);
		//*)

		//(*Declarations(wxWidgetsGUIAppAdoptingDlg)
		wxBoxSizer* BoxSizer1;
		wxBoxSizer* BoxSizer4;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxListBox* FoundFiles;
		wxGauge* Progress;
		wxBoxSizer* BoxSizer2;
		wxStaticText* ScanningTxt;
		wxStaticText* ScanningFile;
		wxBoxSizer* BoxSizer3;
		wxButton* UseFileBtn;
		wxButton* SelectBtn;
		wxButton* CreateBtn;
		wxStaticLine* StaticLine2;
		wxButton* Button4;
		wxStaticLine* StaticLine1;
		wxButton* Button6;
		//*)

	private:

        void Run();
        bool ScanFile(ProjectFile* File);
        void AddSmith(wxString RelativeFileName);

        cbProject* m_Project;
        wxWidgetsGUI* m_GUI;
        wxTimer m_Timer;
        bool m_Run;

		DECLARE_EVENT_TABLE()
};

#endif
