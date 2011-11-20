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

#include "wxsImageTreeCtrl.h"

#include "ImageTreeCtrl16.xpm"
#include "ImageTreeCtrl32.xpm"


//------------------------------------------------------------------------------

namespace
{

    wxsRegisterItem<wxsImageTreeCtrl> Reg(
        _T("wxImageTreeCtrl"),          // Class name
        wxsTWidget,                     // Item type
        _T("wxWindows"),                // License
        _T("Ron Collins"),              // Author
        _T("rcoll@theriver.com"),       // Author's email
        _T(""),                         // Item's homepage
        _T("Image Tools"),              // Category in palette
        40,                             // Priority in palette
        _T("ImageTree"),                // Base part of names for new items
        wxsCPP,                         // List of coding languages supported by this item
        1, 0,                           // Version
        wxBitmap(ImageTreeCtrl32_xpm),  // 32x32 bitmap
        wxBitmap(ImageTreeCtrl16_xpm),  // 16x16 bitmap
        false);                         // We do not allow this item inside XRC files



    WXS_ST_BEGIN(wxsImageTreeCtrlStyles,_T("wxTR_DEFAULT_STYLE"))
        WXS_ST_CATEGORY("wxImageTreeCtrl")
        WXS_ST(wxTR_EDIT_LABELS)
        WXS_ST(wxTR_NO_BUTTONS)
        WXS_ST(wxTR_HAS_BUTTONS)
        WXS_ST(wxTR_TWIST_BUTTONS)
        WXS_ST(wxTR_NO_LINES)
        WXS_ST(wxTR_FULL_ROW_HIGHLIGHT)
        WXS_ST(wxTR_LINES_AT_ROOT)
        WXS_ST(wxTR_HIDE_ROOT)
        WXS_ST(wxTR_ROW_LINES)
        WXS_ST(wxTR_HAS_VARIABLE_ROW_HEIGHT)
        WXS_ST(wxTR_SINGLE)
        WXS_ST(wxTR_MULTIPLE)
        WXS_ST(wxTR_EXTENDED)
        WXS_ST(wxTR_DEFAULT_STYLE)
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxsImageTreeCtrlEvents)
        WXS_EVI(EVT_TREE_BEGIN_DRAG,wxEVT_COMMAND_TREE_BEGIN_DRAG,wxTreeEvent,BeginDrag)
        WXS_EVI(EVT_TREE_BEGIN_RDRAG,wxEVT_COMMAND_TREE_BEGIN_RDRAG,wxTreeEvent,BeginRDrag)
        WXS_EVI(EVT_TREE_END_DRAG,wxEVT_COMMAND_TREE_END_DRAG,wxTreeEvent,EndDrag)
        WXS_EVI(EVT_TREE_BEGIN_LABEL_EDIT,wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT,wxTreeEvent,BeginLabelEdit)
        WXS_EVI(EVT_TREE_END_LABEL_EDIT,wxEVT_COMMAND_TREE_END_LABEL_EDIT,wxTreeEvent,EndLabelEdit)
        WXS_EVI(EVT_TREE_DELETE_ITEM,wxEVT_COMMAND_TREE_DELETE_ITEM,wxTreeEvent,DeleteItem)
        WXS_EVI(EVT_TREE_GET_INFO,wxEVT_COMMAND_TREE_GET_INFO,wxTreeEvent,GetInfo)
        WXS_EVI(EVT_TREE_SET_INFO,wxEVT_COMMAND_TREE_SET_INFO,wxTreeEvent,SetInfo)
        WXS_EVI(EVT_TREE_ITEM_ACTIVATED,wxEVT_COMMAND_TREE_ITEM_ACTIVATED,wxTreeEvent,ItemActivated)
        WXS_EVI(EVT_TREE_ITEM_COLLAPSED,wxEVT_COMMAND_TREE_ITEM_COLLAPSED,wxTreeEvent,ItemCollapsed)
        WXS_EVI(EVT_TREE_ITEM_COLLAPSING,wxEVT_COMMAND_TREE_ITEM_COLLAPSING,wxTreeEvent,ItemCollapsing)
        WXS_EVI(EVT_TREE_ITEM_EXPANDED,wxEVT_COMMAND_TREE_ITEM_EXPANDED,wxTreeEvent,ItemExpanded)
        WXS_EVI(EVT_TREE_ITEM_EXPANDING,wxEVT_COMMAND_TREE_ITEM_EXPANDING,wxTreeEvent,ItemExpanding)
        WXS_EVI(EVT_TREE_ITEM_RIGHT_CLICK,wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,wxTreeEvent,ItemRightClick)
        WXS_EVI(EVT_TREE_ITEM_MIDDLE_CLICK,wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK,wxTreeEvent,ItemMiddleClick)
        WXS_EVI(EVT_TREE_SEL_CHANGED,wxEVT_COMMAND_TREE_SEL_CHANGED,wxTreeEvent,SelectionChanged)
        WXS_EVI(EVT_TREE_SEL_CHANGING,wxEVT_COMMAND_TREE_SEL_CHANGING,wxTreeEvent,SelectionChanging)
        WXS_EVI(EVT_TREE_KEY_DOWN,wxEVT_COMMAND_TREE_KEY_DOWN,wxTreeEvent,KeyDown)
        WXS_EVI(EVT_TREE_ITEM_GETTOOLTIP,wxEVT_COMMAND_TREE_ITEM_GETTOOLTIP,wxTreeEvent,ItemGetToolTip)
        WXS_EVI(EVT_TREE_ITEM_MENU,wxEVT_COMMAND_TREE_ITEM_MENU,wxTreeEvent,ItemMenu)
    WXS_EV_END()
}

//------------------------------------------------------------------------------

wxsImageTreeCtrl::wxsImageTreeCtrl(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsImageTreeCtrlEvents,
        wxsImageTreeCtrlStyles)
{

    mItems.Clear();
    mImageName = _("");
    mExpand    = false;

}

//------------------------------------------------------------------------------

void wxsImageTreeCtrl::OnBuildCreatingCode() {
int             i,n;
int             r,g,b;
int             jv, j1,j2,j3,j4;
wxColor         jc;
bool            jb;
wxString        jt;
wxString        jp[32];
wxString        ss,tt;
wxString        vv, ii, pp;
wxString        top;
wxsImageList    *ilist;

// we only handle C++ constructs here

    if (GetLanguage() != wxsCPP) wxsCodeMarks::Unknown(_T("wxsBitmapComboBox"),GetLanguage());

// header files

    AddHeader(_T("<wx/treectrl.h>"),GetInfo().ClassName,0);

// the basic constructor

    vv = GetVarName();
    Codef(_T("%s = new wxTreeCtrl(%W, %I, %P, %S, %T, %V, %N);\n"), vv.c_str());

// assign the image-list -- AFTER the image list has been built

    ilist = (wxsImageList *) wxsImageListDialog::FindTool(this, mImageName);
    if (ilist != NULL) {
        ss.Printf(_("%s->SetImageList(%s);\n"), vv.c_str(), mImageName.c_str());
        AddEventCode(ss);
    };

// and now each item in the tree data list

    top   = _("");
    jp[0] = _("");
    n     = 0;
    for(i=2; i<mItems.GetCount(); i++) {

// the string, broken into pieces

        ss = mItems.Item(i);
        wxsImageTreeEditDialog::ParseTreeItem(ss, jv, jc, jb, j1, j2, j3, j4, jt);

// make a name for the new item

        n += 1;
        ii.Printf(_("_Item%d"), n);
        ii = vv + ii;

        jp[jv] = ii;

// there is a problem with wxTR_HIDE_ROOT and ScrollTo(root), so make sure that
// we only scroll to first shown item

        if (i == 3) top = ii;

// ID of parent item

        if (jv <= 0) pp = _("");
        else         pp = jp[jv-1];

// make the new item -- level 0 is the root item

        if (jv <= 0) Codef(_T("wxTreeItemId %s = %s->AddRoot(%n);\n"), ii.c_str(), vv.c_str(), jt.c_str());
        else         Codef(_T("wxTreeItemId %s = %s->AppendItem(%s, %n);\n"), ii.c_str(), vv.c_str(), pp.c_str(), jt.c_str());

// set text color of current item if not the default color of black

        r = jc.Red();
        g = jc.Green();
        b = jc.Blue();
        if ((jc.IsOk()) && ((r+g+b) != 0)) {
            ss.Printf(_("%d,%d,%d"), r, g, b);
            Codef(_T("%ASetItemTextColour(%s, wxColour(%s));\n"), ii.c_str(), ss.c_str());
        };

        if (jb) {
            Codef(_T("%ASetItemBold(%s, true);\n"), ii.c_str());
        };

// and the image-list indices

        if (ilist != NULL) {
            if (j1 >= 0) {
                ss.Printf(_("%s->SetItemImage(%s, %d, wxTreeItemIcon_Normal);\n"), vv.c_str(), ii.c_str(), j1);
                AddEventCode(ss);
            };
            if (j2 >= 0) {
                ss.Printf(_("%s->SetItemImage(%s, %d, wxTreeItemIcon_Selected);\n"), vv.c_str(), ii.c_str(), j2);
                AddEventCode(ss);
            };
            if (j3 >= 0) {
                ss.Printf(_("%s->SetItemImage(%s, %d, wxTreeItemIcon_Expanded);\n"), vv.c_str(), ii.c_str(), j3);
                AddEventCode(ss);
            };
            if (j4 >= 0) {
                ss.Printf(_("%s->SetItemImage(%s, %d, wxTreeItemIcon_SelectedExpanded);\n"), vv.c_str(), ii.c_str(), j4);
                AddEventCode(ss);
            };
        };
    };

    ss = _("\n");
    AddEventCode(ss);

// show everything

    if (mExpand) Codef(_T("%AExpandAll();\n"));
    if (top.Length() > 0) Codef(_T("%AScrollTo(%s);\n"), top.c_str());

// and all the rest of it

    BuildSetupWindowCode();
}

//------------------------------------------------------------------------------

wxObject* wxsImageTreeCtrl::OnBuildPreview(wxWindow* Parent,long Flags) {
int             i,n;
wxTreeCtrl      *tree;
wxsImageList    *ilist;
int             jv, j1,j2,j3,j4;
wxColor         jc;
bool            jb;
wxString        jt;
wxTreeItemId    jp[32];
wxString        ss, tt;
wxTreeItemId    item;
wxTreeItemId    top;


// make the basic tree

    tree = new wxTreeCtrl(Parent,GetId(),Pos(Parent),Size(Parent),Style());
    tree->DeleteAllItems();

// save the hide-root flag

    top.Unset();

// make an image list

    mImageList.RemoveAll();
    ilist = (wxsImageList *) wxsImageListDialog::FindTool(this, mImageName);
    if (ilist != NULL) ilist->GetImageList(mImageList);

    tree->SetImageList(&mImageList);

// and all of the items

    n = mItems.GetCount();
    for(i=2; i<n; i++) {
        ss = mItems.Item(i);
        wxsImageTreeEditDialog::ParseTreeItem(ss, jv, jc, jb, j1, j2, j3, j4, jt);

        if (jv == 0) item = tree->AddRoot(jt);
        else         item = tree->AppendItem(jp[jv-1], jt);
        jp[jv] = item;

// there is a problem with wxTR_HIDE_ROOT and ScrollTo(root), so make sure that
// we only scroll to first shown item

        if (i == 3) top = item;

// set colors and images

        if (jc.IsOk()) tree->SetItemTextColour(item, jc);
        tree->SetItemBold(item, jb);
        tree->SetItemImage(item, j1, wxTreeItemIcon_Normal);
        tree->SetItemImage(item, j2, wxTreeItemIcon_Selected);
        tree->SetItemImage(item, j3, wxTreeItemIcon_Expanded);
        tree->SetItemImage(item, j4, wxTreeItemIcon_SelectedExpanded);
    };

// show it

    if (mExpand) tree->ExpandAll();
    if (top.IsOk()) tree->ScrollTo(top);

// done

    return SetupWindow(tree,Flags);
}

//------------------------------------------------------------------------------

void wxsImageTreeCtrl::OnEnumWidgetProperties(long Flags) {
static wxString         sImageNames[128];
static const wxChar    *pImageNames[128];

int                     i,n;
wxString                ss, tt;
wxArrayString           aa;

// find available image lists and store them in our local static arrays

    FindAllImageLists(aa);
    n = aa.GetCount();
    if (n > 127) n = 127;

    for(i=0; i<n; i++) {
        ss = aa.Item(i);
        sImageNames[i] = ss;
        pImageNames[i] = (const wxChar *) sImageNames[i];
    };
    pImageNames[n] = NULL;

    WXS_EDITENUM(wxsImageTreeCtrl, mImageName, _("Image List"), _("mImageName"), pImageNames, _("<none>"))

// and the tree items

    UpdateTreeItemList();

    WXS_IMAGETREE(wxsImageTreeCtrl, mItems, _("Tree Items"), _("mItems"));
    WXS_ARRAYSTRING(wxsImageTreeCtrl, mItems, _("Items as Text"), _("mItemsText"), _("item2"));

// show them expanded?

    WXS_BOOL(wxsImageTreeCtrl, mExpand, _("Expand All"), _("mExpand"), false);
}

//------------------------------------------------------------------------------
// declare the var as a simple wxPanel

void wxsImageTreeCtrl::OnBuildDeclarationsCode() {
int         i,n;
wxString    vname;

    vname = GetVarName();
    AddDeclaration(_T("wxTreeCtrl        *") + vname + _T(";"));
}

//------------------------------------------------------------------------------
// find all tools that are image lists and return their names

void wxsImageTreeCtrl::FindAllImageLists(wxArrayString &aNames) {
int             i,n;
wxsItemResData  *res;
wxsTool         *tool;
wxsImageList    *ilist;
wxString        ss, tt;

// start the list with a chance to de-select any old list

    aNames.Clear();
    aNames.Add(_("<none>"));

// find all tools that are "wxImageList"

    res = GetResourceData();
    n   = res->GetToolsCount();
    for(i=0; i<n; i++) {
        tool = res->GetTool(i);
        ss   = tool->GetUserClass();

        if ((ss == _("wxImageList")) && (n < 127)) {
            ss = tool->GetVarName();
            aNames.Add(ss);
        };
    };
}

//------------------------------------------------------------------------------
// update the list of combo items to send to the wsxImageComboEditDialog

void wxsImageTreeCtrl::UpdateTreeItemList(void) {
int             i,n;
wxString        ss, tt;
wxArrayString   aa;

// first 2 items are always our var name and the name of the image list

    aa.Clear();
    ss = GetVarName();
    aa.Add(ss);
    ss = mImageName;
    aa.Add(ss);

// then copy over everything else the user entered last time

    n = mItems.GetCount();
    for(i=2; i<n; i++) {
        ss = mItems.Item(i);
        aa.Add(ss);
    };

// then put back in original list

    mItems.Clear();
    n = aa.GetCount();
    for(i=0; i<n; i++) {
        ss = aa.Item(i);
        mItems.Add(ss);
    };

// make sure that FindTool has a valid wxsItem* to work from in the dialog

    wxsImageListDialog::FindTool(this, mImageName);
}

//------------------------------------------------------------------------------

void wxsImageTreeCtrl::ParseTreeItem(wxString inSource, wxString &outItem, int &outIndex) {
int         i,n;
long        ll;
wxString    ss, tt;

// working copy

    ss = inSource;

// a "," separates the image index from the text of the item

    i = ss.Find(_(","));

// if a "," was found, parse the index from the text
// if no ",", then no index and the entire string is text

    if (i != wxNOT_FOUND) {
        tt = ss.Left(i);
        ss.erase(0, i+1);
        outItem = ss;
        outIndex = -1;
        if (tt.ToLong(&ll)) outIndex = ll;
    }
    else {
        outIndex = -1;
        outItem = ss;
    };
}


