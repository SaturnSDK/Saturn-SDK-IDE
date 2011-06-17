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
* $Revision: 4850 $
* $Id: wxsbitmapbutton.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsbitmapbutton.cpp $
*/

#include    <iostream>

#include <wx/bmpbuttn.h>
#include "wxsImageButton.h"

using namespace std;

//------------------------------------------------------------------------------


namespace
{

#include "ImageButton16.xpm"
#include "ImageButton32.xpm"

    wxsRegisterItem<wxsImageButton> Reg(
        _T("wxImageButton"),            // Class name
        wxsTWidget,                     // Item type
        _T("wxWindows"),                // License
        _T("Ron Collins"),              // Author
        _T("rcoll@theriver.com"),       // Author's email
        _T(""),                         // Item's homepage
        _T("Image Tools"),              // Category in palette
        70,                             // Priority in palette
        _T("ImageButton"),              // Base part of names for new items
        wxsCPP,                         // List of coding languages supported by this item
        1, 0,                           // Version
        wxBitmap(ImageButton32_xpm),    // 32x32 bitmap
        wxBitmap(ImageButton16_xpm),    // 16x16 bitmap
        false);                         // We do not allow this item inside XRC files


    WXS_ST_BEGIN(wxsImageButtonStyles,_T("wxBU_AUTODRAW"))
        WXS_ST_CATEGORY("wxImageButton")
        WXS_ST(wxBU_LEFT)
        WXS_ST(wxBU_TOP)
        WXS_ST(wxBU_RIGHT)
        WXS_ST(wxBU_BOTTOM)
        WXS_ST(wxBU_AUTODRAW)
        // cyberkoa: "The help mentions that wxBU_EXACTFIX is not used but the XRC code yes
        //  WXS_ST(wxBU_EXACTFIX)
        WXS_ST_DEFAULTS()
    WXS_ST_END()


    WXS_EV_BEGIN(wxsImageButtonEvents)
        WXS_EVI(EVT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent,Click)
    WXS_EV_END()
}

//------------------------------------------------------------------------------

wxsImageButton::wxsImageButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsImageButtonEvents,
        wxsImageButtonStyles)
{
int         i,n;
wxString    ss;

// default values

    mIsDefault      = false;
    mImageList      = _("<none>");
    mLabelIndex     = _("<none>");
    mDisabledIndex  = _("<none>");
    mSelectedIndex  = _("<none>");
    mFocusIndex     = _("<none>");

    mCount = 0;
}

//------------------------------------------------------------------------------

void wxsImageButton::OnBuildCreatingCode() {
int             i,n;
long            ll;
wxString        ss, tt, vv;
wxsItem         *item;
wxsImageList    *ilist;

// we only handle C++ constructs here

    if (GetLanguage() != wxsCPP) wxsCodeMarks::Unknown(_T("wxsImageButton"),GetLanguage());

// header files

    AddHeader(_T("<wx/bmpbuttn.h>"),GetInfo().ClassName,hfInPCH);

// the basic constructor

    vv = GetVarName();
    Codef(_T("%s = new wxBitmapButton(%W, %I, wxNullBitmap, %P, %S, %T, %V, %N);\n"), vv.c_str());

// default button?

    if (mIsDefault) Codef(_T("%ASetDefault();\n"));

// add all the bitmaps at the bottom of the code... after the wxsImage's and wxsImageList's
// have been coded

    ilist = (wxsImageList *) wxsImageListDialog::FindTool(this, mImageList);
    if (ilist != NULL) {
        ss = ilist->GetVarName();

        if (mLabelIndex.ToLong(&ll)) {
            tt.Printf(_("%s->SetBitmapLabel(%s->GetBitmap(%d));\n"), vv.c_str(), ss.c_str(), ll);
            AddEventCode(tt);
        };

        if (mDisabledIndex.ToLong(&ll)) {
            tt.Printf(_("%s->SetBitmapDisabled(%s->GetBitmap(%d));\n"), vv.c_str(), ss.c_str(), ll);
            AddEventCode(tt);
        };

        if (mSelectedIndex.ToLong(&ll)) {
            tt.Printf(_("%s->SetBitmapSelected(%s->GetBitmap(%d));\n"), vv.c_str(), ss.c_str(), ll);
            AddEventCode(tt);
        };

        if (mFocusIndex.ToLong(&ll)) {
            tt.Printf(_("%s->SetBitmapFocus(%s->GetBitmap(%d));\n"), vv.c_str(), ss.c_str(), ll);
            AddEventCode(tt);
        };

        AddEventCode(_("\n"));
    };


// finish setup

    BuildSetupWindowCode();

}

//------------------------------------------------------------------------------

wxObject* wxsImageButton::OnBuildPreview(wxWindow* Parent,long Flags) {
wxBitmapButton  *button;
wxsImageList    *ilist;
long            ll;

// the basic button

    button = new wxBitmapButton(Parent, GetId(), wxNullBitmap,Pos(Parent),Size(Parent),Style());

// find the image list

    ilist = (wxsImageList *) wxsImageListDialog::FindTool(this, mImageList);

// make the preview bitmaps

    if (ilist != NULL) {
        if (mLabelIndex.ToLong(&ll))    button->SetBitmapLabel(ilist->GetPreview(ll));
        if (mDisabledIndex.ToLong(&ll)) button->SetBitmapDisabled(ilist->GetPreview(ll));
        if (mSelectedIndex.ToLong(&ll)) button->SetBitmapSelected(ilist->GetPreview(ll));
        if (mFocusIndex.ToLong(&ll))    button->SetBitmapFocus(ilist->GetPreview(ll));
    };

    if (mIsDefault) button->SetDefault();

// done

    return SetupWindow(button,Flags);
}

//------------------------------------------------------------------------------

void wxsImageButton::OnEnumWidgetProperties(long Flags) {
static wxString         sImageNames[128];
static const wxChar    *pImageNames[128];
static wxString         sIndexNames[1024];
static const wxChar    *pIndexNames[1024];
static long             iIndexNames[1024];

int                     i,n,k;
wxsItemResData         *res;
wxsTool                *tool;
wxsImageList           *ilist;
wxString                ss, tt;


// find available images, and pointer to current imagelist

    ilist = NULL;
    res = GetResourceData();
    n = 0;
    sImageNames[n] = _("<none>");
    pImageNames[n] = (const wxChar *) sImageNames[n];
    n += 1;
    k = res->GetToolsCount();
    for(i=0; i<k; i++) {
        tool = res->GetTool(i);
        ss = tool->GetUserClass();

        if ((ss == _("wxImageList")) && (n < 127)) {
            ss = tool->GetVarName();
            sImageNames[n] = ss;
            pImageNames[n] = (const wxChar *) sImageNames[n];
            n += 1;

            if (ss == mImageList) ilist = (wxsImageList *) tool;
        };
    };
    pImageNames[n] = NULL;

    WXS_EDITENUM(wxsImageButton, mImageList, _("Image List"), _("mImageList"), pImageNames, _("<none>"))

// make drop-down list for image index selection

    n = 0;
    sIndexNames[n] = _("<none>");
    pIndexNames[n] = (const wxChar *) sIndexNames[n];
    iIndexNames[n] = -1;
    n += 1;
    if (ilist == NULL) k = 0;
    else               k = ilist->GetCount();

    for(i=0; i<k; i++) {
        sIndexNames[n].Printf(_("%d"), i);
        pIndexNames[n] = (const wxChar *) sIndexNames[n];
        iIndexNames[n] = i;
        n += 1;
    };
    pIndexNames[n] = NULL;

    WXS_EDITENUM(wxsImageButton, mLabelIndex,    _("Label Index"),    _("mLabelIndex"),    pIndexNames, _("<none>"));
    WXS_EDITENUM(wxsImageButton, mDisabledIndex, _("Disabled Index"), _("mDisabledImage"), pIndexNames, _("<none>"));
    WXS_EDITENUM(wxsImageButton, mSelectedIndex, _("Selected Index"), _("mSelectedImage"), pIndexNames, _("<none>"));
    WXS_EDITENUM(wxsImageButton, mFocusIndex,    _("Focus Index"),    _("mFocusImage"),    pIndexNames, _("<none>"));

// the only "normal" property here

    WXS_BOOL(wxsImageButton,mIsDefault,_("Is default"),_T("mIsDefault"),false)
}

//------------------------------------------------------------------------------
// declare the var as a simple wxPanel

void wxsImageButton::OnBuildDeclarationsCode() {
int         i,n;
wxString    vname;

    vname = GetVarName();
    AddDeclaration(_T("wxBitmapButton        *") + vname + _T(";"));
}






