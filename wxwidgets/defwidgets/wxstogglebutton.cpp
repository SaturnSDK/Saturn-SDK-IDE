#include "wxstogglebutton.h"

#include <wx/tglbtn.h>

namespace
{
    wxsRegisterItem<wxsToggleButton> Reg(_T("ToggleButton"),wxsTWidget,_T("Standard"),50);

    WXS_ST_BEGIN(wxsToggleButtonStyles)
        WXS_ST_CATEGORY("wxToggleButton")
        WXS_ST_DEFAULTS()
    WXS_ST_END()


    WXS_EV_BEGIN(wxsToggleButtonEvents)
        WXS_EVI(EVT_TOGGLEBUTTON,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEvent,Toggle)
        WXS_EV_DEFAULTS()

    WXS_EV_END()
}

wxsToggleButton::wxsToggleButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsBaseProperties::flAll,
        wxsToggleButtonEvents,
        wxsToggleButtonStyles,
        _T("")),
   Label(_("Label")),
   IsChecked(false)
{}


void wxsToggleButton::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            Code<< GetVarName() << _T(" = new wxToggleButton(")
                << WindowParent << _T(",")
                << GetIdName() << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,Label) << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(");\n");

            if ( IsChecked ) Code << GetVarName() << _T("->SetValue(true);\n");

            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsToggleButton::OnBuildCreatingCode"),Language);
        }
    }
}

wxObject* wxsToggleButton::OnBuildPreview(wxWindow* Parent,bool Exact,bool)
{
    wxToggleButton* Preview = new wxToggleButton(Parent,GetId(),Label,Pos(Parent),Size(Parent),Style());
    Preview->SetValue(IsChecked);
    return SetupWindow(Preview,Exact);
}

void wxsToggleButton::OnEnumWidgetProperties(long Flags)
{
    WXS_STRING(wxsToggleButton,Label,0,_("Label"),_T("label"),_T(""),true,false)
    WXS_BOOL  (wxsToggleButton,IsChecked,0,_("Is checked"),_T("checked"),false)
}

void wxsToggleButton::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/tglbtn.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsToggleButton::OnEnumDeclFiles"),Language);
    }
}
