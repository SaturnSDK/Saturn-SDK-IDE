#include "wxslistctrl.h"

#include <wx/listctrl.h>
#include <messagemanager.h>

/* ************************************************************************** */
/* Styles are processed almost like in older code - using WXS_ST_BEGIN,       */
/* WXS_ST_END, WXS_ST_CATEGORY, WXS_ST ...                                    */
/* ************************************************************************** */

WXS_ST_BEGIN(wxsListCtrlStyles)
    WXS_ST_CATEGORY("wxListCtrl")
    WXS_ST(wxLC_LIST)
    WXS_ST(wxLC_REPORT)
    WXS_ST(wxLC_VIRTUAL)
    WXS_ST(wxLC_ICON)
    WXS_ST(wxLC_SMALL_ICON)
    WXS_ST_MASK(wxLC_ALIGN_TOP,wxsSFWin,0,true)
    WXS_ST(wxLC_ALIGN_LEFT)
    WXS_ST_MASK(wxLC_AUTOARRANGE,wxsSFWin,0,true)
    WXS_ST(wxLC_EDIT_LABELS)
    WXS_ST(wxLC_NO_HEADER)
    WXS_ST(wxLC_SINGLE_SEL)
    WXS_ST(wxLC_SORT_ASCENDING)
    WXS_ST(wxLC_SORT_DESCENDING)
    WXS_ST(wxLC_HRULES)
    WXS_ST(wxLC_VRULES)
WXS_ST_END()


/* ************************************************************************** */
/* Events have one additional argument - event type, these can be found       */
/* in WX_DIR/include/listbase.h                                                  */
/* ************************************************************************** */

WXS_EV_BEGIN(wxsListCtrlEvents)

    WXS_EVI(EVT_LIST_BEGIN_DRAG,wxEVT_COMMAND_LIST_BEGIN_DRAG,wxListEvent,BeginDrag)
    WXS_EVI(EVT_LIST_BEGIN_RDRAG,wxEVT_COMMAND_LIST_BEGIN_RDRAG,wxListEvent,BeginRDrag)
    WXS_EVI(EVT_LIST_BEGIN_LABEL_EDIT,wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT,wxListEvent,BeginLabelEdit)
    WXS_EVI(EVT_LIST_END_LABEL_EDIT,wxEVT_COMMAND_LIST_END_LABEL_EDIT,wxListEvent,EndLabelEdit)
    WXS_EVI(EVT_LIST_DELETE_ITEM,wxEVT_COMMAND_LIST_DELETE_ITEM,wxListEvent,DeleteItem)
    WXS_EVI(EVT_LIST_DELETE_ALL_ITEMS,wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS,wxListEvent,DeleteAllItems)
    WXS_EVI(EVT_LIST_ITEM_SELECTED,wxEVT_COMMAND_LIST_ITEM_SELECTED,wxListEvent,ItemSelect)
    WXS_EVI(EVT_LIST_ITEM_DESELECTED,wxEVT_COMMAND_LIST_ITEM_DESELECTED,wxListEvent,ItemDeselect)
    WXS_EVI(EVT_LIST_ITEM_ACTIVATED,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,wxListEvent,ItemActivated)
    WXS_EVI(EVT_LIST_ITEM_FOCUSED,wxEVT_COMMAND_LIST_ITEM_FOCUSED,wxListEvent,ItemFocused)
    WXS_EVI(EVT_LIST_ITEM_MIDDLE_CLICK,wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK,wxListEvent,ItemMClick)
    WXS_EVI(EVT_LIST_ITEM_RIGHT_CLICK,wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,wxListEvent,ItemRClick)
    WXS_EVI(EVT_LIST_KEY_DOWN,wxEVT_COMMAND_LIST_KEY_DOWN,wxListEvent,KeyDown)
    WXS_EVI(EVT_LIST_INSERT_ITEM,wxEVT_COMMAND_LIST_INSERT_ITEM,wxListEvent,InsertItem)
    WXS_EVI(EVT_LIST_COL_CLICK,wxEVT_COMMAND_LIST_COL_CLICK,wxListEvent,ColumnClick)
    WXS_EVI(EVT_LIST_COL_RIGHT_CLICK,wxEVT_COMMAND_LIST_COL_RIGHT_CLICK,wxListEvent,ColumnRClick)
    WXS_EVI(EVT_LIST_COL_BEGIN_DRAG,wxEVT_COMMAND_LIST_COL_BEGIN_DRAG,wxListEvent,ColumnBeginDrag)
    WXS_EVI(EVT_LIST_COL_DRAGGING,wxEVT_COMMAND_LIST_COL_DRAGGING,wxListEvent,ColumnDragging)
    WXS_EVI(EVT_LIST_COL_END_DRAG,wxEVT_COMMAND_LIST_COL_END_DRAG,wxListEvent,ColumnEndDrag)
    WXS_EVI(EVT_LIST_CACHE_HINT,wxEVT_COMMAND_LIST_CACHE_HINT,wxListEvent,CacheHint)

    WXS_EV_DEFAULTS()
WXS_EV_END()

/* ************************************************************************** */
/* Widget info                                                                */
/* ************************************************************************** */

wxsItemInfo wxsListCtrl::Info =
{
    _T("wxListCtrl"),           // Name of class
    wxsTWidget,                 // Type, always wxsTWidget for widget classes
    _("wxWidgets license"),     // License, any type
    _("wxWidgets team"),        // Author
    _T(""),                     // No default e-mail for standard widgets
    _T("www.wxwidgets.org"),    // Site
    _T("Standard"),             // Groud for widget, note that _T() instead of _() is used
    60,                         // Button is one of most commonly used widgets - we give it high priority
    _T("ListCtrl"),             // Standard prefix for variable names and identifiers
    2, 6,                       // Widget version
    NULL,                       // Bitmaps will be loaded later in manager
    NULL,                       // --------------------''-------------------
    0                           // --------------------''-------------------
};


/* ************************************************************************** */
/* Constructur for wxsWidget need style set, event set and widget info, so    */
/* these things should be created before ctor.                                */
/* ************************************************************************** */

wxsListCtrl::wxsListCtrl(wxsWindowRes* Resource):
    wxsWidget(
        Resource,                   // Resource is passed to wxsItem's constructor
        wxsBaseProperties::flAll,   // Using all base properties
        &Info,                      // Taking local info
        wxsListCtrlEvents,          // Pointer to local events
        wxsListCtrlStyles,          // Pointer to local styles
        _T(""))                      // Default style = 0
{}


/* ************************************************************************** */
/* Function building code - it should append new code generating this widget  */
/* to the end of prevous code. Currently it support only C++ langage but some */
/* other languages may be added in future.                                    */
/* ************************************************************************** */

void wxsListCtrl::BuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            // Because c_str() may lead to unspecified behaviour
            // it's better to use << operator instead of wxString::Format.
            // But be carefull when using << to add integers and longs.
            // Because of some wxWidgets bugs use '<< wxString::Format(_T("%d"),Value)'
            // instead of '<< Value'
            Code<< GetVarName() << _T(" = new wxListCtrl(")
                << WindowParent << _T(",")
                << GetIdName() << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(");\n");

            SetupWindowCode(Code,Language);
            return;
        }
    }

    wxsLANGMSG(wxsListCtrl::BuildCreatingCode,Language);
}

/* ************************************************************************** */
/* Function building preview item. This should simply return previewed object */
/* with all properties set-up.                                                */
/* ************************************************************************** */

wxObject* wxsListCtrl::DoBuildPreview(wxWindow* Parent,bool Exact)
{
    // Exact argument is not used here because list control preview is
    // exactly the same in editor and in preview window
    wxListCtrl* Preview = new wxListCtrl(Parent,GetId(),Pos(Parent),Size(Parent),Style());

    return SetupWindow(Preview,Exact);
}


/* ************************************************************************** */
/* Function enumerating all list control-specific properties.                       */
/* ************************************************************************** */

void wxsListCtrl::EnumWidgetProperties(long Flags)
{
//    WXS_STRING(wxsListCtrl,Label,0,_("Label"),_T("label"),_T(""),true,false)
//    WXS_BOOL  (wxsListCtrl,IsDefault,0,_("Is default"),_("default"),false)
}
