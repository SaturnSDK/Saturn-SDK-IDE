#include "wxsbutton.h"

namespace
{
    wxsRegisterItem<wxsButton> Reg(_T("Button"),wxsTWidget,_T("Standard"),90);

    WXS_ST_BEGIN(wxsButtonStyles,_T(""))
        WXS_ST_CATEGORY("wxButton")
        WXS_ST(wxBU_LEFT)
        WXS_ST(wxBU_TOP)
        WXS_ST(wxBU_RIGHT)
        WXS_ST(wxBU_BOTTOM)
        WXS_ST(wxBU_EXACTFIT)
        WXS_ST_MASK(wxNO_BORDER,wxsSFWin| wxsSFGTK ,0,true)
    WXS_ST_END()

    WXS_EV_BEGIN(wxsButtonEvents)
        WXS_EVI(EVT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent,Click)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsButton::wxsButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsButtonEvents,
        wxsButtonStyles),
    Label(_("Label")),
    IsDefault(false)
{}

void wxsButton::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxButton(");
            }
            else
            {
                Code<< _T("Create(");
            }
            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,Label) << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(",")
                << _T("wxDefaultValidator") << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetVarName(),false) << _T(");\n");
            if ( IsDefault ) Code << GetVarName() << _T("->SetDefault();\n");
            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsButton::OnBuildCreatingCode"),Language);
        }
    }
}

wxObject* wxsButton::OnBuildPreview(wxWindow* Parent,bool Exact,bool Store)
{
    wxButton* Preview = new wxButton(Parent,GetId(),Label,Pos(Parent),Size(Parent),Style());
    if ( IsDefault ) Preview->SetDefault();
    return SetupWindow(Preview,Exact);
}

void wxsButton::OnEnumWidgetProperties(long Flags)
{
    WXS_STRING(wxsButton,Label,0,_("Label"),_T("label"),_T(""),true,false)
    WXS_BOOL  (wxsButton,IsDefault,0,_("Is default"),_T("default"),false)
}

void wxsButton::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/button.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsButton::OnEnumDeclFiles"),Language);
    }
}
