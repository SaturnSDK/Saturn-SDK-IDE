#include "wxsbaseproperties.h"

#include <messagemanager.h>

void wxsBaseProperties::EnumProperties(long Flags)
{
//    WXS_POSITION(wxsBaseProperties,m_Position,flPosition,_("Default pos"),_("X"),_("Y"),_("Pos in dialog units"),_T("pos"));
//    WXS_SIZE    (wxsBaseProperties,m_Size,flSize,_("Default size"),_("Width"),_("Height"),_("Size in dialog units"),_T("size"));
    WXS_BOOL    (wxsBaseProperties,m_Enabled,flEnabled,_("Enabled"),_T("enabled"),true);
    WXS_BOOL    (wxsBaseProperties,m_Focused,flFocused,_("Focused"),_T("focused"),false);
    WXS_BOOL    (wxsBaseProperties,m_Hidden,flHidden,_("Hidden"),_T("hidden"),false);
//    WXS_COLOUR  (wxsBaseProperties,m_Fg,flColours,_("Foreground"),_T("fg"));
//    WXS_COLOUR  (wxsBaseProperties,m_Bg,flColours,_("Background"),_T("bg"));
//    WXS_FONT    (wxsBaseProperties,m_Font,flFont,_("Font"),_("font"));
    WXS_STRING  (wxsBaseProperties,m_ToolTip,flToolTip,_("Tooltip"),_T("tooltip"),wxEmptyString,true,false);
    WXS_STRING  (wxsBaseProperties,m_HelpText,flHelpText,_("Help text"),_T("helptext"),wxEmptyString,true,false);
}

void wxsBaseProperties::SetupWindow(wxWindow* Window,bool IsExact)
{
    if ( !m_Enabled ) Window->Disable();
    if ( m_Focused  ) Window->SetFocus();
    if ( m_Hidden && IsExact ) Window->Hide();
//    wxColour FGCol = wxsColourProperty::GetColour(m_Fg);
//    if ( FGCol.Ok() ) Window->SetForegroundColour(FGCol);
//    wxColour BGCol = wxsColourProperty::GetColour(m_Bg);
//    if ( BGCol.Ok() ) Window->SetBackgroundColour(BGCol);
//    wxFont FontVal = m_Font.BuildFont();
//    if ( FontVal.Ok() ) Window->SetFont(FontVal);
    if ( !m_ToolTip.empty() ) Window->SetToolTip(m_ToolTip);
    if ( !m_HelpText.empty() ) Window->SetHelpText(m_HelpText);
}

void wxsBaseProperties::BuildSetupWindowCode(wxString& Code,const wxString& WindowName,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            wxString VarAccess = WindowName.empty() ? _T("") : WindowName + _T("->");
            if ( !m_Enabled ) Code << VarAccess << _T("Disable();\n");
            if ( m_Focused  ) Code << VarAccess << _T("SetFocus();\n");
            if ( m_Hidden )   Code << VarAccess << _T("Hide();\n");

//            wxString FGCol = wxsColourProperty::GetColourCode(m_Fg,wxsCPP);
//            if ( !FGCol.empty() ) Code << VarAccess << _T("SetForegroundColour(") << FGCol << _T(");\n");
//
//            wxString BGCol = wxsColourProperty::GetColourCode(m_Bg,wxsCPP);
//            if ( !BGCol.empty() ) Code << VarAccess << _T("SetBackgroundColour(") << BGCol << _T(");\n");
//
//            wxString FontVal = m_Font.BuildFontCode(WindowName + _T("Font"), wxsCPP);
//            if ( !FontVal.empty() )
//            {
//                Code << FontVal;
//                Code << VarAccess << _T("SetFont(") << WindowName << _T("Font);\n");
//            }
            if ( !m_ToolTip.empty()  ) Code << VarAccess << _T("SetToolTip(") << wxsCodeMarks::WxString(wxsCPP,m_ToolTip) << _T(");\n");
            if ( !m_HelpText.empty() ) Code << VarAccess << _T("SetHelpText(") << wxsCodeMarks::WxString(wxsCPP,m_HelpText) << _T(");\n");
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsBaseProperties::BuildSetupWindowCode"),Language);
        }
    }

}

void wxsBaseProperties::AddQPPChild(wxsAdvQPP* QPP,long Flags)
{
    // TODO: Add it
}
