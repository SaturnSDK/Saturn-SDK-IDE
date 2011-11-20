#ifndef WXSIMAGECOMBOEDITDIALOG_H
#define WXSIMAGECOMBOEDITDIALOG_H

//(*Headers(wxsImageComboEditDialog)
#include <wx/treectrl.h>
#include <wx/stattext.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
//*)

#include "wxsImageList.h"
#include "wxsImageListDialog.h"
#include <wx/bmpcbox.h>
#include <wx/msgdlg.h>
#include <wx/imaglist.h>
#include <wx/bitmap.h>

class wxsImageComboEditDialog: public wxDialog
{
	public:

		wxsImageComboEditDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxsImageComboEditDialog();

        bool    Execute(wxArrayString &aItems);


		//(*Declarations(wxsImageComboEditDialog)
		wxStaticText* StaticText10;
		wxStaticText* StaticText9;
		wxBitmapButton* bDeleteAll;
		wxButton* bOK;
		wxButton* bCancel;
		wxStaticText* StaticText2;
		wxStaticText* StaticText6;
		wxTreeCtrl* Tree1;
		wxStaticText* StaticText8;
		wxBitmapButton* bAddItem;
		wxStaticText* StaticText1;
		wxBitmapButton* bDeleteItem;
		wxStaticText* StaticText3;
		wxTreeItemId    Tree1ID[8];
		wxBitmapComboBox* cbImage;
		wxBitmapButton* bEditItem;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxBitmapButton* bMoveUp;
		wxBitmapButton* bMoveDown;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(wxsImageComboEditDialog)
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_TREECTRL1;
		static const long ID_STATICTEXT1;
		static const long ID_BITMAPBUTTON1;
		static const long ID_BITMAPBUTTON2;
		static const long ID_BITMAPBUTTON3;
		static const long ID_BITMAPBUTTON4;
		static const long ID_BITMAPBUTTON5;
		static const long ID_STATICTEXT2;
		static const long ID_BITMAPBUTTON6;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT9;
		static const long ID_COMBOBOX1;
		static const long ID_STATICTEXT10;
		//*)

	private:

		//(*Handlers(wxsImageComboEditDialog)
		void OnbDeleteAllClick(wxCommandEvent& event);
		void OnbAddItemClick(wxCommandEvent& event);
		void OnbMoveUpClick(wxCommandEvent& event);
		void OnbMoveDownClick(wxCommandEvent& event);
		void OnbDeleteItemClick(wxCommandEvent& event);
		void OnbEditItemClick(wxCommandEvent& event);
		void OnbOKClick(wxCommandEvent& event);
		void OnbCancelClick(wxCommandEvent& event);
		void OncbImageSelect(wxCommandEvent& event);
		//*)

        wxString            mComboName;         // name of base combo-box
        wxString            mImageName;         // name of image-list
        wxImageList         mImageList;         // local copy of an image list

		DECLARE_EVENT_TABLE()
};

#endif
