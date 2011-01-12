/** \file wxsbitmapcombobox.cpp
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
* This code is based in part on wxsimagecombobox from the wxSmithImage plug-in,
* copyright Ron Collins and released under the GPL.
*
*/

#include <wx/bmpcbox.h>
#include "wxsbitmapcombobox.h"
#include "wxsimagelist.h"
#include "../properties/wxsimagelisteditordlg.h"


namespace
{
    wxsRegisterItem<wxsBitmapComboBox> Reg(_T("BitmapComboBox"), wxsTWidget, _T("Standard"), 350);

    WXS_ST_BEGIN(wxsBitmapComboBoxStyles, wxEmptyString)
    WXS_ST_CATEGORY("wxBitmapComboBox")
    WXS_ST(wxCB_READONLY)
    WXS_ST(wxCB_SORT)
    WXS_ST(wxTE_PROCESS_ENTER)
    WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxsBitmapComboBoxEvents)
		WXS_EVI(EVT_COMBOBOX, wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEvent, Selected)
		WXS_EVI(EVT_TEXT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEvent, TextUpdated)
		WXS_EVI(EVT_TEXT_ENTER, wxEVT_COMMAND_TEXT_ENTER, wxCommandEvent, TextEnter)
    WXS_EV_END()
}

/*! \brief Ctor
 *
 * \param Data wxsItemResData*	The control's resource data.
 *
 */
wxsBitmapComboBox::wxsBitmapComboBox(wxsItemResData *Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsBitmapComboBoxEvents,
        wxsBitmapComboBoxStyles),
    m_defaultSelection(-1),
    m_sImageList(_("<none>"))
{
}

/*! \brief Create the initial control.
 *
 * \return void
 *
 */
void wxsBitmapComboBox::OnBuildCreatingCode()
{
    switch(GetLanguage())
    {
        case wxsCPP:
            {
                AddHeader(_T("<wx/bmpcbox.h>"), GetInfo().ClassName, hfInPCH);
                Codef(_T("%C(%W, %I, wxEmptyString, %P, %S, 0, 0, %T, %V, %N);\n"));

				// Find the image list.
				wxsImageList *imageList = (wxsImageList *) wxsImageListEditorDlg::FindTool(this, m_sImageList);

				wxString sItem, sText;
				int iPos;
				wxString vv = GetVarName();
				int iItemcount = m_arrItems.GetCount();
					// Locator comment.
					if(iItemcount > 0){
#if wxCHECK_VERSION(2, 9, 0)
						AddEventCode(wxString::Format(_("// Set the bitmaps for %s.\n"), vv.wx_str()));
#else
						AddEventCode(wxString::Format(_("// Set the bitmaps for %s.\n"), vv.c_str()));
#endif
					}
				for(int i = 2;i < iItemcount; i++) {
					sItem = m_arrItems.Item(i);
					ParseComboItem(sItem, sText, iPos);
					// Add the text item.
#if wxCHECK_VERSION(2, 9, 0)
					Codef(_T("%s->Append(_(\"%s\"));\n"), vv.wx_str(), sText.wx_str());
#else
					Codef(_T("%s->Append(_(\"%s\"));\n"), vv.c_str(), sText.c_str());
#endif

					if((imageList) && (iPos >= 0)){
#if wxCHECK_VERSION(2, 9, 0)
						sText.Printf(_T("%s->SetItemBitmap(%d, %s->GetBitmap(%d));\n"), vv.wx_str(), i - 2, m_sImageList.wx_str(), iPos);
#else
						sText.Printf(_T("%s->SetItemBitmap(%d, %s->GetBitmap(%d));\n"), vv.c_str(), i - 2, m_sImageList.c_str(), iPos);
#endif
						// Add all the bitmaps at the bottom of the code... after the wxsImages and wxsImageLists have been coded
						AddEventCode(sText);
					}
				}

				if(m_defaultSelection > -1){
					Codef(_T("%ASetSelection(%d);\n"), m_defaultSelection);
				}

				BuildSetupWindowCode();
				return;
            }

        default:
            {
                wxsCodeMarks::Unknown(_T("wxsBitmapComboBox::OnBuildCreatingCode"), GetLanguage());
            }
    }
}

/*! \brief	Build the control preview.
 *
 * \param parent wxWindow*	The parent window.
 * \param flags long					The control flags.
 * \return wxObject* 					The constructed control.
 *
 */
wxObject *wxsBitmapComboBox::OnBuildPreview(wxWindow *Parent, long Flags)
{
    wxBitmapComboBox *preview = new wxBitmapComboBox(Parent, GetId(), wxEmptyString, Pos(Parent), Size(Parent), m_arrItems, Style());

	// Find the image list.
	wxsImageList *imageList = (wxsImageList *) wxsImageListEditorDlg::FindTool(this, m_sImageList);

	int				i, n;
	wxString	sItem, sText;
	wxBitmap	bmp;
	// make sure there is no random junk
    preview->Clear();

	// add items to combo-box
	// note: first 2 items are used only in the dialog
    for(i = 2;i < (int)m_arrItems.GetCount();i++){
        sItem = m_arrItems.Item(i);
        ParseComboItem(sItem, sText, n);

        preview->Append(sText);

        if((imageList != NULL) && (n >= 0)){
        	preview->SetItemBitmap(i - 2, imageList->GetPreview(n));
        }
    }

    if(m_defaultSelection > -1){
		preview->SetSelection(m_defaultSelection);
    }

    return SetupWindow(preview, Flags);
}

/*! \brief Enumerate the control's properties.
 *
 * \param flags long	The control flags.
 * \return void
 *
 */
void wxsBitmapComboBox::OnEnumWidgetProperties(long Flags)
{
	static wxString         	sImageNames[128];
	static const wxChar    *pImageNames[128];

	int                     		i, n;
	wxString                ss;
	wxArrayString       aa;

	// find available image lists and store them in our local static arrays
    FindAllImageLists(aa);
    n = aa.GetCount();
    if(n > 127){
    	n = 127;
    }

    for(i=0;i < n;i++){
        ss = aa.Item(i);
        sImageNames[i] = ss;
        pImageNames[i] = (const wxChar *) sImageNames[i];
    }
    pImageNames[n] = NULL;

    WXS_EDITENUM(wxsBitmapComboBox, m_sImageList, _("Image List"), _T("image_list"), pImageNames, _("<none>"))

	// The list of items to appear in the combo box.
    UpdateComboItemList();

    WXS_IMAGECOMBO(wxsBitmapComboBox, m_arrItems, _("Combo Items"), _T("items"));
    WXS_ARRAYSTRING(wxsBitmapComboBox, m_arrItems, _("Items as Text"), _T("items_text"), _T("item2"));
    WXS_LONG(wxsBitmapComboBox, m_defaultSelection, _("Selection"), _T("selection"), -1)
}

/*! \brief Find all tools that are image lists and return their names.
 *
 * \param aNames wxArrayString&
 * \return void
 *
 */
void wxsBitmapComboBox::FindAllImageLists(wxArrayString &aNames)
{
	int             					i, n;
	wxsItemResData  	*res;
	wxsTool         			*tool;
	wxString        			ss;

	// start the list with a chance to de-select any old list
    aNames.Clear();
    aNames.Add(_("<none>"));

	// find all tools that are "wxImageList"
    res = GetResourceData();
    n   = res->GetToolsCount();
    for(i = 0;i < n;i++){
        tool = res->GetTool(i);
        ss   = tool->GetUserClass();

        if((ss == _T("wxImageList")) && (n < 127)){
            ss = tool->GetVarName();
            aNames.Add(ss);
        }
    }
}

/*! \brief Update the list of combo items to send to the wsxImageComboEditDialog
 *
 * \param void
 * \return void
 *
 */
void wxsBitmapComboBox::UpdateComboItemList(void)
{
	int             			i, n;
	wxString        	sItem;
	wxArrayString	aa;

	// first 2 items are always our var name and the name of the image list
    aa.Clear();
    sItem = GetVarName();
    aa.Add(sItem);
    sItem = m_sImageList;
    aa.Add(sItem);

	// then copy over everything else the user entered last time
    n = m_arrItems.GetCount();
    for(i = 2;i < n; i++){
        sItem = m_arrItems.Item(i);
        aa.Add(sItem);
    }

	// then put back in original list
    m_arrItems.Clear();
    n = aa.GetCount();
    for(i = 0;i < n;i++){
        sItem = aa.Item(i);
        m_arrItems.Add(sItem);
    }

	// make sure that FindTool has a valid wxsItem* to work from in the dialog
    wxsImageListEditorDlg::FindTool(this, m_sImageList);
}

/*! \brief Parse a combo item string.
 *
 * \param inSource wxString
 * \param outItem wxString&
 * \param outIndex int&
 * \return void
 *
 */
void wxsBitmapComboBox::ParseComboItem(wxString inSource, wxString &outItem, int &outIndex)
{
	int         		i;
	long        	iLong;
	wxString 	sSource, sText;

	// working copy
    sSource = inSource;

	// a "," separates the image index from the text of the item
    i = sSource.Find(_(","));

	// if a "," was found, parse the index from the text
	// if no ",", then no index and the entire string is text
    if(i != wxNOT_FOUND){
        sText = sSource.Left(i);
        sSource.erase(0, i+1);
        outItem = sSource;
        outIndex = -1;
        if (sText.ToLong(&iLong)){
        	outIndex = iLong;
        }
    }
    else{
        outIndex = -1;
        outItem = sSource;
    }
}

