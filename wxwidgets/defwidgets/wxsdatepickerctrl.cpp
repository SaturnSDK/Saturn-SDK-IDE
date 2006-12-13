#include "wxsdatepickerctrl.h"

#include <wx/datectrl.h>

namespace
{
    wxsRegisterItem<wxsDatePickerCtrl> Reg(_T("DatePickerCtrl"),wxsTWidget,_T("Standard"),30);

    WXS_ST_BEGIN(wxsDatePickerCtrlStyles,_T("wxDP_DEFAULT|wxDP_SHOWCENTURY"))
        WXS_ST_CATEGORY("wxDatePickerCtrl")
        WXS_ST(wxDP_DEFAULT)
        WXS_ST(wxDP_SPIN)
        WXS_ST(wxDP_DROPDOWN)
        WXS_ST(wxDP_ALLOWNONE)
        WXS_ST(wxDP_SHOWCENTURY)
    WXS_ST_END()


    WXS_EV_BEGIN(wxsDatePickerCtrlEvents)
        WXS_EVI(EVT_DATE_CHANGED,wxEVT_DATE_CHANGED,wxDataEvent,Changed)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsDatePickerCtrl::wxsDatePickerCtrl(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsDatePickerCtrlEvents,
        wxsDatePickerCtrlStyles)
{}

void wxsDatePickerCtrl::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxDatePickerCtrl(");
            }
            else
            {
                Code<< _T("Create(");
            }
            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << _T("wxDefaultDateTime") << _T(",")   // TODO find a way to get the wxDateTime in code
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(",")
                << _T("wxDefaultValidator") << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetIdName(),false) << _T(");\n");

            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsDatePickerCtrl::OnBuildCreatingCode"),Language);
        }
    }
}


wxObject* wxsDatePickerCtrl::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxDatePickerCtrl* Preview = new wxDatePickerCtrl(Parent,GetId(),wxDefaultDateTime,Pos(Parent),Size(Parent),Style());
    return SetupWindow(Preview,Flags);
}

void wxsDatePickerCtrl::OnEnumWidgetProperties(long Flags)
{
}

void wxsDatePickerCtrl::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/datectrl.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsDatePickerCtrl::OnEnumDeclFiles"),Language);
    }
}
