/** \file wxsimagecomboeditordlg.h
*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2010 Gary Harris
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* This code was taken from the wxSmithImage plug-in, copyright Ron Collins
* and released under the GPL.
*
*/

#ifndef WXSIMAGECOMBOEDITORDLG_H
#define WXSIMAGECOMBOEDITORDLG_H

//(*Headers(wxsImageComboEditorDlg)
#include <wx/treectrl.h>
#include <wx/stattext.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include "scrollingdialog.h"
#include <wx/combobox.h>
//*)

#include "../defitems/wxsimagelist.h"
#include <wx/bmpcbox.h>
#include <wx/msgdlg.h>
#include <wx/imaglist.h>
#include <wx/bitmap.h>

class wxsItem;

///*! \brief wxBitmapComboBox item editor dialogue. */
class wxsImageComboEditorDlg: public wxScrollingDialog
{
	public:

        /*! \brief Constructor.
         *
         * \param parent wxWindow*
         * \param id wxWindowID
         * \param pos const wxPoint&
         * \param size const wxSize&
         */
		wxsImageComboEditorDlg(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		/** \brief Destructor. */
		virtual ~wxsImageComboEditorDlg();

        bool    Execute(wxArrayString &aItems);


		//(*Declarations(wxsImageComboEditorDlg)
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
		wxBitmapComboBox* cbImage;
		wxBitmapButton* bEditItem;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxBitmapButton* bMoveUp;
		wxBitmapButton* bMoveDown;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(wxsImageComboEditorDlg)
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

		//(*Handlers(wxsImageComboEditorDlg)
		void OnbDeleteAllClick(wxCommandEvent& event);
		void OnbAddItemClick(wxCommandEvent& event);
		void OnbMoveUpClick(wxCommandEvent& event);
		void OnbMoveDownClick(wxCommandEvent& event);
		void OnbDeleteItemClick(wxCommandEvent& event);
		void OnbEditItemClick(wxCommandEvent& event);
		void OnbOKClick(wxCommandEvent& event);
		void OnbCancelClick(wxCommandEvent& event);
		void OncbImageSelect(wxCommandEvent& event);
		void OnTree1SelectionChanged(wxTreeEvent& event);
		//*)

        wxString			m_sComboName;		//!< The name of the base combobox.
        wxString			m_sImageName;		//!< The name of the image list.
        wxImageList		m_imageList;         		//!< Local copy of an image list

		DECLARE_EVENT_TABLE()
};

#endif
