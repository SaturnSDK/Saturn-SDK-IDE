#ifndef WXIMAGETREEEDITDIALOG_H
#define WXIMAGETREEEDITDIALOG_H

//(*Headers(wxTreeDialog)
#include <wx/treectrl.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/bmpcbox.h>
#include <wx/bmpbuttn.h>
#include <wx/imaglist.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include <wx/imaglist.h>
#include <wx/colordlg.h>

#include "wxsImageList.h"



class wxsImageTreeEditDialog: public wxDialog
{
	public:

		wxsImageTreeEditDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxsImageTreeEditDialog();

                bool        Execute(wxArrayString &aItems);
static          void        ParseTreeItem(wxString aSource, int &outLevel, wxColour &outColour, bool &outBold, int &outImage1, int &outImage2, int &outImage3, int &outImage4, wxString &outText);



		//(*Declarations(wxsImageTreeEditDialog)
		wxStaticText* StaticText10;
		wxStaticText* StaticText9;
		wxButton* bOK;
		wxBitmapComboBox* cbSelected;
		wxBitmapComboBox* cbExpanded;
		wxBitmapButton        *bAddSubItem;
		wxStaticText* StaticText13;
		wxButton* bCancel;
		wxStaticText* StaticText2;
		wxStaticText* StaticText6;
		wxTreeCtrl* Tree1;
		wxStaticText* StaticText8;
		wxStaticText* StaticText11;
		wxImageList* ImageList1;
		wxBitmapButton        *bEditItem;
		wxPanel* Panel1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxStaticBox* StaticBox1;
		wxBitmapButton        *bDelAllItems;
		wxTreeItemId    Tree1ID[8];
		wxBitmapButton        *bDelItem;
		wxStaticBox* StaticBox2;
		wxCheckBox* cxItemBold;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxStaticText* StaticText12;
		wxBitmapComboBox* cbSelExpanded;
		wxStaticText* StaticText4;
		wxButton* bItemColor;
		wxBitmapButton        *bAddItem;
		wxBitmapComboBox* cbNormal;
		//*)



	protected:

		//(*Identifiers(wxsImageTreeEditDialog)
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT1;
		static const long ID_STATICBOX1;
		static const long ID_TREECTRL1;
		static const long ID_STATICBOX2;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_BUTTON3;
		static const long ID_STATICTEXT6;
		static const long ID_CHECKBOX1;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT10;
		static const long ID_COMBOBOX1;
		static const long ID_COMBOBOX2;
		static const long ID_COMBOBOX3;
		static const long ID_COMBOBOX4;
		static const long ID_STATICTEXT11;
		static const long ID_IMAGEBUTTON1;
		static const long ID_IMAGEBUTTON2;
		static const long ID_IMAGEBUTTON3;
		static const long ID_IMAGEBUTTON4;
		static const long ID_IMAGEBUTTON5;
		static const long ID_STATICTEXT12;
		static const long ID_STATICTEXT13;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(wxsImageTreeEditDialog)
		void OnbAddItemClick(wxCommandEvent& event);
		void OnbAddSubItemClick(wxCommandEvent& event);
		void OnbDelItemClick(wxCommandEvent& event);
		void OnbDelAllItemsClick(wxCommandEvent& event);
		void OnbItemColorClick(wxCommandEvent& event);
		void OncxItemBoldClick(wxCommandEvent& event);
		void OnbEditItemClick(wxCommandEvent& event);
		void OncbNormalSelect(wxCommandEvent& event);
		void OncbSelectedSelect(wxCommandEvent& event);
		void OncbExpandedSelect(wxCommandEvent& event);
		void OncbSelExpandedSelect(wxCommandEvent& event);
		void OnTreeCtrl1SelectionChanged(wxTreeEvent& event);
		void OnbOKClick(wxCommandEvent& event);
		void OnbCancelClick(wxCommandEvent& event);
		//*)

        void        SetImageList(wxImageList &inImageList);
        void        AddItem(wxTreeItemId &inParent);
        void        EncodeTreeItems(wxTreeItemId inParent, int inLevel, wxArrayString &outList);



        wxString                mTreeName;
        wxString                mImageName;
        wxImageList             mImageList;

        wxColourDialog          mColourDialog;

		DECLARE_EVENT_TABLE()
};

#endif
