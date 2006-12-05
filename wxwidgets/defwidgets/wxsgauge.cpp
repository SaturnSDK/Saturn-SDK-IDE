#include "wxsgauge.h"

namespace
{
    wxsRegisterItem<wxsGauge> Reg(_T("Gauge"),wxsTWidget,_T("Standard"),50);

    WXS_ST_BEGIN(wxsGaugeStyles,_T(""))
        WXS_ST(wxGA_HORIZONTAL)
        WXS_ST(wxGA_VERTICAL)
    // NOTE (cyberkoa#1#): according to Help file , wxGA_SMOOTH is not support by all platform but does not mention which platform is not support.
        WXS_ST(wxGA_SMOOTH)
    // NOTE (cyberkoa##): wxGA_PROGRESSBAR not in HELP file but in XRC code
    //    WXS_ST(wxGA_PROGRESSBAR)
    WXS_ST_END()


    WXS_EV_BEGIN(wxsGaugeEvents)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsGauge::wxsGauge(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsGaugeEvents,
        wxsGaugeStyles),
    Range(100),
    Value(0),
    Shadow(0),
    Bezel(0)
{}



void wxsGauge::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxGauge(");
            }
            else
            {
                Code<< _T("Create(");
            }
            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << wxString::Format(_T("%d"),Range) << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(",")
                << _T("wxDefaultValidator") << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetVarName(),false) << _T(");\n");

            if ( Value )  Code << GetVarName() << _T("->SetValue(") << wxString::Format(_T("%d"),Value) << _T(");\n");
            if ( Shadow ) Code << GetVarName() << _T("->SetShadowWidth(") << wxString::Format(_T("%d"),Shadow) << _T(");\n");
            if ( Bezel )  Code << GetVarName() << _T("->SetBezelFace(") << wxString::Format(_T("%d"),Bezel) << _T(");\n");

            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsGauge::OnBuildCreatingCode"),Language);
        }
    }
}


wxObject* wxsGauge::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxGauge* Preview = new wxGauge(Parent,GetId(),Range,Pos(Parent),Size(Parent),Style());
    if ( Value )  Preview->SetValue(Value);
    if ( Shadow ) Preview->SetShadowWidth(Shadow);
    if ( Bezel )  Preview->SetBezelFace(Bezel);
    return SetupWindow(Preview,Flags);
}


void wxsGauge::OnEnumWidgetProperties(long Flags)
{
    WXS_LONG(wxsGauge,Value,0,_("Value"),_T("value"),0)
    WXS_LONG(wxsGauge,Range,0,_("Range"),_T("range"),100)
    WXS_LONG(wxsGauge,Shadow,0,_("3D Shadow Width"),_T("shadow"),0)
    WXS_LONG(wxsGauge,Bezel,0,_("Bezel Face Width"),_T("bezel"),0)
}

void wxsGauge::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/gauge.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsGauge::OnEnumDeclFiles"),Language);
    }
}
