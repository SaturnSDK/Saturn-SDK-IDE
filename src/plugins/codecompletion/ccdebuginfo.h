/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CCDEBUGINFO_H
#define CCDEBUGINFO_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

//(*Headers(CCDebugInfo)
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/panel.h>
#include "scrollingdialog.h"
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
//*)


class Parser;
class Token;

class CCDebugInfo: public wxScrollingDialog
{
	public:

		CCDebugInfo(wxWindow* parent, Parser* parser, Token* token);
		virtual ~CCDebugInfo();

		Parser* m_pParser;
		Token* m_pToken;

		void FillFiles();
		void FillDirs();
		void DisplayTokenInfo();
		void FillChildren();
		void FillAncestors();
		void FillDescendants();

		//(*Identifiers(CCDebugInfo)
		static const long ID_STATICTEXT29;
		static const long ID_TEXTCTRL1;
		static const long ID_BUTTON1;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT17;
		static const long ID_STATICTEXT18;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXT11;
		static const long ID_STATICTEXT12;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT13;
		static const long ID_STATICTEXT14;
		static const long ID_STATICTEXT15;
		static const long ID_STATICTEXT16;
		static const long ID_STATICTEXT32;
		static const long ID_STATICTEXT33;
		static const long ID_STATICTEXT19;
		static const long ID_STATICTEXT20;
		static const long ID_STATICTEXT22;
		static const long ID_STATICTEXT24;
		static const long ID_BUTTON4;
		static const long ID_STATICTEXT30;
		static const long ID_COMBOBOX3;
		static const long ID_BUTTON5;
		static const long ID_STATICTEXT21;
		static const long ID_COMBOBOX2;
		static const long ID_BUTTON3;
		static const long ID_STATICTEXT23;
		static const long ID_COMBOBOX1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT25;
		static const long ID_STATICTEXT26;
		static const long ID_STATICTEXT27;
		static const long ID_STATICTEXT28;
		static const long ID_STATICTEXT34;
		static const long ID_STATICTEXT35;
		static const long ID_PANEL1;
		static const long ID_LISTBOX1;
		static const long ID_PANEL2;
		static const long ID_LISTBOX2;
		static const long ID_PANEL3;
		static const long ID_NOTEBOOK1;
		static const long ID_STATICTEXT31;
		static const long ID_STATICLINE2;
		//*)

	protected:

		//(*Handlers(CCDebugInfo)
		void OnInit(wxInitDialogEvent& event);
		void OnFindClick(wxCommandEvent& event);
		void OnGoAscClick(wxCommandEvent& event);
		void OnGoDescClick(wxCommandEvent& event);
		void OnGoParentClick(wxCommandEvent& event);
		void OnGoChildrenClick(wxCommandEvent& event);
		//*)

		//(*Declarations(CCDebugInfo)
		wxComboBox* cmbChildren;
		wxStaticText* StaticText22;
		wxFlexGridSizer* FlexGridSizer1;
		wxComboBox* cmbAncestors;
		wxPanel* Panel1;
		wxStaticText* StaticText21;
		wxTextCtrl* txtFilter;
		wxStaticText* StaticText13;
		wxStaticText* StaticText15;
		wxBoxSizer* BoxSizer3;
		wxStaticLine* StaticLine2;
		wxButton* btnGoParent;
		wxStaticText* txtID;
		wxStaticText* StaticText32;
		wxStaticText* StaticText17;
		wxStaticText* txtParent;
		wxStaticText* txtArgs;
		wxStaticText* txtNamespace;
		wxStaticText* txtType;
		wxStaticText* txtIsOp;
		wxBoxSizer* BoxSizer7;
		wxStaticText* txtScope;
		wxStaticText* StaticText1;
		wxPanel* Panel2;
		wxButton* btnFind;
		wxBoxSizer* BoxSizer9;
		wxStaticText* StaticText3;
		wxStaticText* txtKind;
		wxButton* btnGoAsc;
		wxStaticText* StaticText23;
		wxStaticText* txtDeclFile;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText34;
		wxBoxSizer* BoxSizer4;
		wxListBox* lstFiles;
		wxStaticText* StaticText7;
		wxStaticText* txtActualType;
		wxListBox* lstDirs;
		wxBoxSizer* BoxSizer8;
		wxButton* btnGoDesc;
		wxStaticText* lblInfo;
		wxStaticText* txtName;
		wxStaticText* StaticText5;
		wxStaticText* txtIsLocal;
		wxStaticText* StaticText30;
		wxStaticText* StaticText27;
		wxNotebook* Notebook1;
		wxStaticText* txtUserData;
		wxStaticText* txtIsTemp;
		wxStaticText* StaticText19;
		wxStaticText* txtImplFile;
		wxStaticText* StaticText29;
		wxComboBox* cmbDescendants;
		wxStaticText* StaticText9;
		wxBoxSizer* BoxSizer6;
		wxStdDialogButtonSizer* StdDialogButtonSizer1;
		wxButton* btnGoChildren;
		wxStaticText* StaticText11;
		wxStaticText* StaticText25;
		wxBoxSizer* BoxSizer5;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
