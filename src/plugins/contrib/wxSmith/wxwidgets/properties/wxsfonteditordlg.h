/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
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
* $Revision$
* $Id$
* $HeadURL$
*/

#ifndef WXSFONTEDITORDLG_H
#define WXSFONTEDITORDLG_H

#include "wxsfontproperty.h"

//(*Headers(wxsFontEditorDlg)
#include <wx/spinctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include "scrollingdialog.h"
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/listbox.h>
//*)

class wxsFontEditorDlg: public wxScrollingDialog
{
	public:

		wxsFontEditorDlg(wxWindow* parent,wxsFontData& Data,wxWindowID id = -1);
		virtual ~wxsFontEditorDlg();

		//(*Identifiers(wxsFontEditorDlg)
		static const long ID_CHOICE1;
		static const long ID_CHECKBOX8;
		static const long ID_STATICTEXT1;
		static const long ID_CHOICE2;
		static const long ID_CHECKBOX2;
		static const long ID_COMBOBOX1;
		static const long ID_CHECKBOX1;
		static const long ID_CHOICE3;
		static const long ID_CHECKBOX7;
		static const long ID_SPINCTRL1;
		static const long ID_CHECKBOX6;
		static const long ID_TEXTCTRL1;
		static const long ID_CHECKBOX3;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON2;
		static const long ID_RADIOBUTTON3;
		static const long ID_CHECKBOX4;
		static const long ID_RADIOBUTTON4;
		static const long ID_RADIOBUTTON5;
		static const long ID_RADIOBUTTON6;
		static const long ID_CHECKBOX5;
		static const long ID_RADIOBUTTON7;
		static const long ID_RADIOBUTTON8;
		static const long ID_LISTBOX1;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_BUTTON8;
		static const long ID_BUTTON7;
		static const long ID_TEXTCTRL2;
		//*)

	protected:

		//(*Handlers(wxsFontEditorDlg)
		void OnButton1Click(wxCommandEvent& event);
		void OnUpdateContent(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnButton1Click1(wxCommandEvent& event);
		void OnFaceAddClick(wxCommandEvent& event);
		void OnFaceDelClick(wxCommandEvent& event);
		void OnFaceEditClick(wxCommandEvent& event);
		void OnFaceUpClick(wxCommandEvent& event);
		void OnFaceDownClick(wxCommandEvent& event);
		void OnUpdatePreview(wxCommandEvent& event);
		void OnSizeValChange(wxSpinEvent& event);
		void OnBaseFontUseChange(wxCommandEvent& event);
		//*)

		//(*Declarations(wxsFontEditorDlg)
		wxStaticBoxSizer* StaticBoxSizer2;
		wxCheckBox* BaseFontUse;
		wxFlexGridSizer* FlexGridSizer2;
		wxCheckBox* FamUse;
		wxBoxSizer* BoxSizer3;
		wxStaticBoxSizer* StaticBoxSizer7;
		wxBoxSizer* BaseFontSizer;
		wxRadioButton* WeightNorm;
		wxStaticBoxSizer* StaticBoxSizer5;
		wxRadioButton* StyleItal;
		wxChoice* BaseFontVal;
		wxStaticText* BaseFontTxt;
		wxCheckBox* EncodUse;
		wxBoxSizer* BoxSizer2;
		wxCheckBox* UnderUse;
		wxStaticBoxSizer* StaticBoxSizer3;
		wxRadioButton* WeightBold;
		wxCheckBox* RelSizeUse;
		wxButton* FaceAdd;
		wxRadioButton* StyleSlant;
		wxRadioButton* UnderNo;
		wxListBox* FaceList;
		wxButton* FaceUp;
		wxButton* FaceDel;
		wxRadioButton* WeightLight;
		wxTextCtrl* RelSizeVal;
		wxBoxSizer* BoxSizer4;
		wxStaticBoxSizer* StaticBoxSizer4;
		wxCheckBox* WeightUse;
		wxChoice* EncodVal;
		wxStaticBoxSizer* StaticBoxSizer6;
		wxCheckBox* SizeUse;
		wxBoxSizer* BoxSizer1;
		wxSpinCtrl* SizeVal;
		wxButton* FaceDown;
		wxRadioButton* StyleNorm;
		wxButton* FaceEdit;
		wxRadioButton* UnderYes;
		wxChoice* FontType;
		wxBoxSizer* BoxSizer6;
		wxCheckBox* StyleUse;
		wxComboBox* FamVal;
		wxStdDialogButtonSizer* StdDialogButtonSizer1;
		wxBoxSizer* BoxSizer5;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxTextCtrl* TestArea;
		//*)

	private:

        void UpdateContent();
        void UpdatePreview();
        void ReadData(wxsFontData& Data);
        void StoreData(wxsFontData& Data);

        wxsFontData& Data;
        wxArrayString Encodings;
        bool Initialized;

		DECLARE_EVENT_TABLE()
};

#endif
