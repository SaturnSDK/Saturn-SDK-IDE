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
*/


#include "wxsImagePanel.h"



//------------------------------------------------------------------------------

namespace
{

// Loading images from xpm files

    #include "ImagePanel16.xpm"
    #include "ImagePanel32.xpm"

    wxsRegisterItem<wxsImagePanel> Reg(
        _T("wxImagePanel"),             // Class name
        wxsTContainer,                  // Item type
        _T("wxWindows"),                // License
        _T("Ron Collins"),              // Author
        _T("rcoll@theriver.com"),       // Author's email
        _T(""),                         // Item's homepage
        _T("Image Tools"),              // Category in palette
        60,                             // Priority in palette
        _T("ImagePanel"),               // Base part of names for new items
        wxsCPP,                         // List of coding languages supported by this item
        1, 0,                           // Version
        wxBitmap(ImagePanel32_xpm),     // 32x32 bitmap
        wxBitmap(ImagePanel16_xpm),     // 16x16 bitmap
        false);                         // We do not allow this item inside XRC files


    WXS_ST_BEGIN(wxsImagePanelStyles,_T("wxRAISED_BORDER|wxTAB_TRAVERSAL"))
        WXS_ST_CATEGORY("wxImagePanel")
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxsImagePanelEvents)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}


//------------------------------------------------------------------------------

wxsImagePanel::wxsImagePanel(wxsItemResData* Data):
    wxsContainer(
        Data,
        &Reg.Info,
        wxsImagePanelEvents,
        wxsImagePanelStyles)
{
    mImage   = _("<none>");
    mStretch = false;
}

//------------------------------------------------------------------------------

wxsImagePanel::~wxsImagePanel() {

}

//------------------------------------------------------------------------------

void wxsImagePanel::OnBuildCreatingCode() {
int         i,n;
wxString    vname;
wxString    iname;
wxsImage    *image;
wxString    ss, tt;

// we only handle C++ constructs here

    if (GetLanguage() != wxsCPP) wxsCodeMarks::Unknown(_T("wxsImagePanel"),GetLanguage());

// who we are

    vname = GetVarName();

// get the image record, and the name of the bitmap associated with it

    image = (wxsImage *) wxsImageListDialog::FindTool(this, mImage);

    if (image == NULL) {
        iname = _("");
    }
    else {
        iname  = image->GetVarName();
        iname += _("_BMP");
    };

// include files

    AddHeader(_("<wxImagePanel.h>"), GetInfo().ClassName, 0);

// create the panel

    Codef(_T("%C(%W, %I, %P, %S, %T, %N);\n"));

// the stretching flag

    Codef(_T("%ASetStretch(%b);\n"), mStretch);

// the image has to be assigned to the panel AFTER the image is created
// since wxsImage is a wxsTool type, that all happens after the panel is created

    if (iname.Length() > 0) {
        tt.Printf(_("%s->SetBitmap(*%s);\n"), vname.c_str(), iname.c_str());
        AddEventCode(tt);
        AddEventCode(_("\n"));
    };

// do the rest of it

    BuildSetupWindowCode();

// add children

    AddChildrenCode();
}

//------------------------------------------------------------------------------

wxObject* wxsImagePanel::OnBuildPreview(wxWindow* Parent, long Flags) {
int             i,n;
wxImagePanel    *ap;
wxsImage        *image;
wxBitmap        bmp;

// make a panel

    ap = new wxImagePanel(Parent, GetId(), Pos(Parent), Size(Parent), Style());
    if (ap == NULL) return NULL;

// get the wxsImage pointer

    image = (wxsImage *) wxsImageListDialog::FindTool(this, mImage);

// and make the preview image

    if (image != NULL) {
        bmp = ((wxsImage *) image)->GetPreview();
        ap->SetBitmap(bmp);
    };

// and stretch it?

    ap->SetStretch(mStretch);

// set all decorations

    SetupWindow(ap, Flags);

// add the children

    AddChildrenPreview(ap, Flags);

// done

    return ap;
}

//------------------------------------------------------------------------------

void wxsImagePanel::OnEnumContainerProperties(long Flags) {
static wxString         sImageNames[128];
static const wxChar    *pImageNames[128];

int                     i,n,k;
wxsItemResData         *res;
wxsTool                *tool;
wxString                ss, tt;


// find available images, and pointer to current imagelist

    res = GetResourceData();
    n = 0;
    sImageNames[n] = _("<none>");
    pImageNames[n] = (const wxChar *) sImageNames[n];
    n += 1;
    k = res->GetToolsCount();
    for(i=0; i<k; i++) {
        tool = res->GetTool(i);
        ss = tool->GetUserClass();

        if ((ss == _("wxImage")) && (n < 127)) {
            ss = tool->GetVarName();
            sImageNames[n] = ss;
            pImageNames[n] = (const wxChar *) sImageNames[n];
            n += 1;
        };
    };
    pImageNames[n] = NULL;

    WXS_EDITENUM(wxsImagePanel, mImage, _("Image"), _("mImage"), pImageNames, _("<none>"))

// stretch to fit panel?

    WXS_BOOL(wxsImagePanel, mStretch, _("Stretch"), _("mStretch"), false);
};







