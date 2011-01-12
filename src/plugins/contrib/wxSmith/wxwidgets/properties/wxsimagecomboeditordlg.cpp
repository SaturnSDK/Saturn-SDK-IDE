/** \file wxsimagecomboeditordlg.cpp
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
* This code was taken from the wxSmithImage plug-in, copyright Ron CollinswxsImageComboEditorDlg
* and released under the GPL.
*
*/


//(*InternalHeaders(wxsImageComboEditorDlg)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)
#include "../wxsitemresdata.h"
#include "../wxsitem.h"
#include "wxsimagelisteditordlg.h"
#include "wxsimagecomboeditordlg.h"

//(*IdInit(wxsImageComboEditorDlg)
const long wxsImageComboEditorDlg::ID_BUTTON1 = wxNewId();
const long wxsImageComboEditorDlg::ID_BUTTON2 = wxNewId();
const long wxsImageComboEditorDlg::ID_TREECTRL1 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT1 = wxNewId();
const long wxsImageComboEditorDlg::ID_BITMAPBUTTON1 = wxNewId();
const long wxsImageComboEditorDlg::ID_BITMAPBUTTON2 = wxNewId();
const long wxsImageComboEditorDlg::ID_BITMAPBUTTON3 = wxNewId();
const long wxsImageComboEditorDlg::ID_BITMAPBUTTON4 = wxNewId();
const long wxsImageComboEditorDlg::ID_BITMAPBUTTON5 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT2 = wxNewId();
const long wxsImageComboEditorDlg::ID_BITMAPBUTTON6 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT3 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT4 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT5 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT6 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT7 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT8 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT9 = wxNewId();
const long wxsImageComboEditorDlg::ID_COMBOBOX1 = wxNewId();
const long wxsImageComboEditorDlg::ID_STATICTEXT10 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxsImageComboEditorDlg,wxScrollingDialog)
	//(*EventTable(wxsImageComboEditorDlg)
	//*)
END_EVENT_TABLE()

	wxsImageComboEditorDlg::wxsImageComboEditorDlg(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
	{
		//(*Initialize(wxsImageComboEditorDlg)
		Create(parent, wxID_ANY, _("wxBitmapComboBox Editor"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
		SetClientSize(wxSize(414,350));
		SetMinSize(wxSize(-1,-1));
		SetMaxSize(wxSize(-1,-1));
		wxFont thisFont(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
		SetFont(thisFont);
		bOK = new wxButton(this, ID_BUTTON1, _("OK"), wxPoint(64,320), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
		bOK->SetDefault();
		bCancel = new wxButton(this, ID_BUTTON2, _("Cancel"), wxPoint(272,320), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
		Tree1 = new wxTreeCtrl(this, ID_TREECTRL1, wxPoint(0,32), wxSize(240,272), wxTR_EDIT_LABELS|wxTR_NO_BUTTONS|wxTR_NO_LINES|wxTR_HIDE_ROOT|wxRAISED_BORDER, wxDefaultValidator, _T("ID_TREECTRL1"));
		StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Combo-Box"), wxPoint(8,8), wxSize(224,14), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
		bAddItem = new wxBitmapButton(this, ID_BITMAPBUTTON1, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON), wxPoint(256,32), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
		bAddItem->SetDefault();
		bMoveUp = new wxBitmapButton(this, ID_BITMAPBUTTON2, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_UP")),wxART_BUTTON), wxPoint(256,64), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
		bMoveUp->SetDefault();
		bMoveDown = new wxBitmapButton(this, ID_BITMAPBUTTON3, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_DOWN")),wxART_BUTTON), wxPoint(256,96), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON3"));
		bMoveDown->SetDefault();
		bDeleteItem = new wxBitmapButton(this, ID_BITMAPBUTTON4, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DEL_BOOKMARK")),wxART_BUTTON), wxPoint(256,128), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON4"));
		bDeleteAll = new wxBitmapButton(this, ID_BITMAPBUTTON5, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DELETE")),wxART_BUTTON), wxPoint(256,160), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON5"));
		StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Edit Items"), wxPoint(264,8), wxSize(136,14), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT2"));
		bEditItem = new wxBitmapButton(this, ID_BITMAPBUTTON6, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_HELP_SETTINGS")),wxART_BUTTON), wxPoint(256,200), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON6"));
		bEditItem->SetDefault();
		StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Add Item"), wxPoint(288,36), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
		StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Move Item Up"), wxPoint(288,68), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
		StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Move Item Down"), wxPoint(288,100), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
		StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Delete Item"), wxPoint(288,132), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
		StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Delete All Items"), wxPoint(288,164), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
		StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Edit Item Text"), wxPoint(288,204), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
		StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Image-List"), wxPoint(256,240), wxSize(144,14), wxST_NO_AUTORESIZE, _T("ID_STATICTEXT9"));
		cbImage = new wxBitmapComboBox(this, ID_COMBOBOX1, wxEmptyString, wxPoint(256,272), wxSize(88,22), 0, 0, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX1"));
		StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Item Image"), wxPoint(352,276), wxDefaultSize, 0, _T("ID_STATICTEXT10"));

		Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OnbOKClick);
		Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OnbCancelClick);
		Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OnTree1SelectionChanged);
		Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OnbAddItemClick);
		Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OnbMoveUpClick);
		Connect(ID_BITMAPBUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OnbMoveDownClick);
		Connect(ID_BITMAPBUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OnbDeleteItemClick);
		Connect(ID_BITMAPBUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OnbDeleteAllClick);
		Connect(ID_BITMAPBUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OnbEditItemClick);
		Connect(ID_COMBOBOX1,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&wxsImageComboEditorDlg::OncbImageSelect);
		//*)
	}

	wxsImageComboEditorDlg::~wxsImageComboEditorDlg()
	{
		//(*Destroy(wxsImageComboEditorDlg)
		//*)
	}

	/*! \brief Run the dialogue.
	 *
	 * \param aItems wxArrayString&
	 * \return bool
	 *
	 */
	bool wxsImageComboEditorDlg::Execute(wxArrayString &aItems)
	{
		int             i,n;
		int             j,k;
		long            ll;
		wxString        ss, tt;
		wxTreeItemId    root;
		wxTreeItemId    item;
		wxTreeItemIdValue   cookie;
		wxBitmap        bmp;
		wxsImageList    *ilist;


		// get name of combo-box and image-list
		n = aItems.GetCount();
		if(n == 0){
			m_sComboName = _("<unknown>");
			m_sImageName = _("<none>");
		}
		else if(n == 1){
			m_sComboName = aItems.Item(0);
			m_sImageName = _("<none>");
		}
		else{
			m_sComboName = aItems.Item(0);
			m_sImageName = aItems.Item(1);
		}

		// show the names
		ss = _("Combo Box: ") + m_sComboName;
		StaticText1->SetLabel(ss);

		ss = _("Image List: ") + m_sImageName;
		StaticText9->SetLabel(ss);

		// a valid image-list given?
		m_imageList.RemoveAll();
		ilist = (wxsImageList *) wxsImageListEditorDlg::FindTool(NULL, m_sImageName);
		if(ilist == NULL){
			cbImage->Enable(false);
		}
		else{
			cbImage->Enable(true);
			ilist->GetImageList(m_imageList);
		}

		// setup the combo-box image selector
		cbImage->Clear();
		cbImage->Append(_("<none>"));

		n = m_imageList.GetImageCount();
		for(i=0; i<n; i++){
			ss.Printf(_T("%3d"), i);
			bmp = m_imageList.GetBitmap(i);

			cbImage->Append(ss, bmp);
		}

		cbImage->SetSelection(0);

		// clear old junk
		Tree1->DeleteAllItems();

		// make a root item
		root = Tree1->AddRoot(_("root"));

		// make sure we are using the image list -- even if it is empty
		Tree1->SetImageList(&m_imageList);

		// add all the new items
		n = aItems.GetCount();
		for(i = 2;i < n;i++){
			ss = aItems.Item(i);
			j  = ss.Find(_T(","));
			k = -1;
			if(j != wxNOT_FOUND){
				tt = ss.Left(j);
				ss.erase(0, j + 1);
				if(tt.ToLong(&ll)) k = ll;
			}
			item = Tree1->AppendItem(root, ss, k);
		}

		// show the dialog and wait for a response
		n = ShowModal();

		// save all new stuff?
		if(n == wxOK){
			// must save combo-box name and image-list name
			aItems.Clear();
			aItems.Add(m_sComboName);
			aItems.Add(m_sImageName);

			// save text of all children of the root item
			// these are actually the only things seen by the user
			item = Tree1->GetFirstChild(root, cookie);
			while(item.IsOk()){
				ss = Tree1->GetItemText(item);
				k  = Tree1->GetItemImage(item, wxTreeItemIcon_Normal);

				tt.Printf(_T("%d,"), k);
				ss = tt + ss;

				aItems.Add(ss);

				item = Tree1->GetNextChild(root, cookie);
			}
		}

		// done
		return (n == wxOK);
	}

    /*! \brief The Delete All button was clicked.
     *
     * \param event wxCommandEvent&
     * \return void
     *
     */
	void wxsImageComboEditorDlg::OnbDeleteAllClick(wxCommandEvent& event)
	{
		int         n;

		n = wxMessageBox(_("Delete All Items?"), _("Confirm"), wxYES_NO );
		if(n == wxYES){
			Tree1->DeleteAllItems();
		}
	}

    /*! \brief The Add Item button was clicked.
     *
     * \param event wxCommandEvent&
     * \return void
     *
     */
	void wxsImageComboEditorDlg::OnbAddItemClick(wxCommandEvent& event)
	{
		int             n;
		wxTreeItemId    root;
		wxTreeItemId    item;
		wxString        ss;

		// must have a root item, even though it is hidden
		n = Tree1->GetCount();
		if(n == 0){
			root = Tree1->AddRoot(_("root"));
		}
		else{
			root = Tree1->GetRootItem();
		}

		// add a new child under the root
		ss.Printf(_("new item %d"), n);
		item = Tree1->AppendItem(root, ss);

		// new image index
		n = cbImage->GetSelection();
		if(n == wxNOT_FOUND){
			n  = -1;
		}
		else{
			n -=  1;
		}

		// assign image
		Tree1->SetItemImage(item, n, wxTreeItemIcon_Normal);

		// done
		Tree1->SelectItem(item);
		Tree1->EditLabel(item);
	}

    /*! \brief The Move Up button was clicked.
     *
     * \param event wxCommandEvent&
     * \return void
     *
     */
	void wxsImageComboEditorDlg::OnbMoveUpClick(wxCommandEvent& event)
	{
		int         xi, xp;
		wxString    si, sp;
		wxTreeItemId    prev;
		wxTreeItemId    item;

		// current item and the one directly above
		item = Tree1->GetSelection();
		prev = Tree1->GetPrevSibling(item);

		// no such thing?
		if(! item.IsOk()){
			return;
		}
		if(! prev.IsOk()){
			return;
		}

		// swap strings and image indices
		si = Tree1->GetItemText(item);
		xi = Tree1->GetItemImage(item, wxTreeItemIcon_Normal);

		sp = Tree1->GetItemText(prev);
		xp = Tree1->GetItemImage(prev, wxTreeItemIcon_Normal);

		// and swap them
		Tree1->SetItemText(item, sp);
		Tree1->SetItemImage(item, xp);

		Tree1->SetItemText(prev, si);
		Tree1->SetItemImage(prev, xi);

		// done
		Tree1->SelectItem(prev);
		Tree1->Refresh();
	}

    /*! \brief The Move Down button was clicked.
     *
     * \param event wxCommandEvent&
     * \return void
     *
     */
	void wxsImageComboEditorDlg::OnbMoveDownClick(wxCommandEvent& event)
	{
		int         xi, xp;
		wxString    si, sp;
		wxTreeItemId    next;
		wxTreeItemId    item;

		// current item and the one directly above
		item = Tree1->GetSelection();
		next = Tree1->GetNextSibling(item);

		// no such thing?
		if(!item.IsOk()){
			return;
		}
		if(!next.IsOk()){
			return;
		}

		// swap strings and image indices
		si = Tree1->GetItemText(item);
		xi = Tree1->GetItemImage(item, wxTreeItemIcon_Normal);

		sp = Tree1->GetItemText(next);
		xp = Tree1->GetItemImage(next, wxTreeItemIcon_Normal);

		// and swap them
		Tree1->SetItemText(item, sp);
		Tree1->SetItemImage(item, xp);

		Tree1->SetItemText(next, si);
		Tree1->SetItemImage(next, xi);

		// done
		Tree1->SelectItem(next);
		Tree1->Refresh();
	}

    /*! \brief The Delete Item button was clicked.
     *
     * \param event wxCommandEvent&
     * \return void
     *
     */
	void wxsImageComboEditorDlg::OnbDeleteItemClick(wxCommandEvent& event)
	{
		wxTreeItemId    item;

		// delete only current item
		item = Tree1->GetSelection();
		if(item.IsOk()){
			Tree1->Delete(item);
		}

		// done
		Tree1->Refresh();
	}

    /*! \brief The Edit Item button was clicked.
     *
     * \param event wxCommandEvent&
     * \return void
     *
     */
	void wxsImageComboEditorDlg::OnbEditItemClick(wxCommandEvent& event)
	{
		wxTreeItemId    item;

		// delete only current item
		item = Tree1->GetSelection();
		if(item.IsOk()){
			Tree1->EditLabel(item);
		}

		// done
		Tree1->Refresh();
	}

    /*! \brief The OK button was clicked.
     *
     * \param event wxCommandEvent&
     * \return void
     *
     */
	void wxsImageComboEditorDlg::OnbOKClick(wxCommandEvent& event)
	{
		EndModal(wxOK);
	}

    /*! \brief The Cancel button was clicked.
     *
     * \param event wxCommandEvent&
     * \return void
     *
     */
	void wxsImageComboEditorDlg::OnbCancelClick(wxCommandEvent& event)
	{
		EndModal(wxCANCEL);
	}

    /*! \brief Assign a new image to a text item.
     *
     * \param event wxCommandEvent&
     * \return void
     *
     */
	void wxsImageComboEditorDlg::OncbImageSelect(wxCommandEvent& event)
	{
		int             n;
		wxTreeItemId    item;

		// current item
		item = Tree1->GetSelection();
		if(! item.IsOk()){
			return;
		}

		// new image index
		n = cbImage->GetSelection();
		if(n == wxNOT_FOUND){
			n  = -1;
		}
		else{
			n -=  1;
		}

		// assign image
		Tree1->SetItemImage(item, n, wxTreeItemIcon_Normal);
	}

/*! \brief A list item was clicked in the item list.
 *
 * \param event wxTreeEvent&
 * \return void
 *
 * Sets the image combo to display the correct image for the item selected.
 */
void wxsImageComboEditorDlg::OnTree1SelectionChanged(wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();
	if(item.IsOk()){
			// Set the combo box to display 1 position higher than the number returned by GetItemImage() as the first item is "<none>" .
			// i.e. image 0 in the imagelist is at position 1 in the combo box, etc.
			cbImage->SetSelection(Tree1->GetItemImage(item) + 1);
	}

}
