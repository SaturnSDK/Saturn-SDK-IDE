#include "wxswidget.h"

#include <messagemanager.h>

wxsWidget::wxsWidget(
    wxsItemResData* Data,
    const wxsItemInfo* Info,
    long PropertiesFlags,
    const wxsEventDesc* EventArray,
    const wxsStyle* StyleSet,
    const wxString& DefaultStyle):
        wxsItem(Data,Info,PropertiesFlags,EventArray),
        m_StyleSet(StyleSet),
        m_DefaultStyle(DefaultStyle),
        m_StyleBits(0),
        m_ExStyleBits(0)
{
    if ( StyleSet )
    {
        wxsStyleProperty::SetFromString(m_StyleBits,DefaultStyle,StyleSet,false);
    }

}

void wxsWidget::OnEnumItemProperties(long Flags)
{
    OnEnumWidgetProperties(Flags);
    if ( m_StyleSet )
    {
        WXS_STYLE(wxsWidget,m_StyleBits,0,_("Style"),_T("style"),m_StyleSet,m_DefaultStyle);
        WXS_EXSTYLE(wxsWidget,m_ExStyleBits,0,_("Extra style"),_T("exstyle"),m_StyleSet,wxEmptyString);
    }
}

void wxsWidget::OnAddItemQPP(wxsAdvQPP* QPP)
{
    OnAddWidgetQPP(QPP);
}

wxWindow* wxsWidget::SetupWindow(wxWindow* Preview,bool IsExact)
{
    GetBaseProps()->SetupWindow(Preview,IsExact);
    long ExStyle = wxsStyleProperty::GetWxStyle(m_ExStyleBits,m_StyleSet,true);
    if ( ExStyle != 0 )
    {
        Preview->SetExtraStyle(Preview->GetExtraStyle() | ExStyle);
    }
    return Preview;
}

void wxsWidget::SetupWindowCode(wxString& Code,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            GetBaseProps()->BuildSetupWindowCode(Code,GetVarName(),wxsCPP);
            if ( m_ExStyleBits )
            {
                wxString ExStyleStr = wxsStyleProperty::GetString(m_ExStyleBits,m_StyleSet,true,wxsCPP);
                if ( ExStyleStr != _T("0") )
                {
                    wxString VarAccess = GetVarName().empty() ? _T("") : GetVarName() + _T("->");

                    Code << VarAccess << _T("SetExtraStyle(") <<
                            VarAccess << _T("GetExtraStyle() | ") <<
                            ExStyleStr << _T(");\n");
                }
            }
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsWidget::SetupWindowCode"),Language);
        }
    }

}
