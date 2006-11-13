#include "wxswidget.h"

#include <messagemanager.h>

wxsWidget::wxsWidget(
    wxsItemRes* Resource,
    const wxsItemInfo* Info,
    long PropertiesFlags,
    const wxsEventDesc* EventArray,
    const wxsStyle* StyleSet,
    const wxString& DefaultStyle):
        wxsItem(Resource,Info,PropertiesFlags,EventArray),
        m_StyleSet(StyleSet),
        m_DefaultStyle(DefaultStyle),
        m_StyleBits(0),
        m_ExStyleBits(0)
{
    if ( StyleSet )
    {
        // TODO
        // wxsStyleProperty::SetFromString(StyleBits,DefaultStyle,StyleSet,false);
    }

}

void wxsWidget::OnEnumItemProperties(long Flags)
{
    OnEnumWidgetProperties(Flags);
    // TODO
//    if ( StyleSet )
//    {
//        WXS_STYLE(wxsWidget,StyleBits,0,_("Style"),_T("style"),StyleSet,DefaultStyle);
//        WXS_EXSTYLE(wxsWidget,ExStyleBits,0,_("Extra style"),_T("exstyle"),StyleSet,wxEmptyString);
//    }
}

void wxsWidget::OnAddItemQPP(wxsAdvQPP* QPP)
{
    OnAddWidgetQPP(QPP);
}

wxWindow* wxsWidget::SetupWindow(wxWindow* Preview,bool IsExact)
{
    GetBaseProps()->SetupWindow(Preview,IsExact);
    // TODO
//    long ExStyle = wxsStyleProperty::GetWxStyle(ExStyleBits,StyleSet,true);
//    if ( ExStyle != 0 )
//    {
//        Preview->SetExtraStyle(Preview->GetExtraStyle() | ExStyle);
//    }
    return Preview;
}

void wxsWidget::SetupWindowCode(wxString& Code,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            GetBaseProps()->BuildSetupWindowCode(Code,GetVarName(),wxsCPP);
            // TODO
//            if ( ExStyleBits )
//            {
//                wxString ExStyleStr = wxsStyleProperty::GetString(ExStyleBits,StyleSet,true,wxsCPP);
//                if ( ExStyleStr != _T("0") )
//                {
//                    wxString VarAccess = GetVarName().empty() ? _T("") : GetVarName() + _T("->");
//
//                    Code << VarAccess << _T("SetExtraStyle(") <<
//                            VarAccess << _T("GetExtraStyle() | ") <<
//                            ExStyleStr << _T(");\n");
//                }
//            }
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsWidget::SetupWindowCode"),Language);
        }
    }

}
