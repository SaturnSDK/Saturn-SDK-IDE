#include "wxsbitmapbutton.h"

namespace
{
    wxsRegisterItem<wxsBitmapButton> Reg(_T("BitmapButton"),wxsTWidget,_T("Standard"),50);

    WXS_ST_BEGIN(wxsBitmapButtonStyles,_T("wxBU_AUTODRAW"))
        WXS_ST_CATEGORY("wxBitmapButton")
        WXS_ST_MASK(wxBU_LEFT,wxsSFWin,0,true)
        WXS_ST_MASK(wxBU_TOP,wxsSFWin,0,true)
        WXS_ST_MASK(wxBU_RIGHT,wxsSFWin,0,true)
        WXS_ST_MASK(wxBU_BOTTOM,wxsSFWin,0,true)
        WXS_ST_MASK(wxBU_AUTODRAW,wxsSFWin,0,true)
        // cyberkoa: "The help mentions that wxBU_EXACTFIX is not used but the XRC code yes
        //  WXS_ST(wxBU_EXACTFIX)
    WXS_ST_END()


    WXS_EV_BEGIN(wxsBitmapButtonEvents)
        WXS_EVI(EVT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent,Click)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsBitmapButton::wxsBitmapButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsBitmapButtonEvents,
        wxsBitmapButtonStyles)
{}

void wxsBitmapButton::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            wxString SizeName = GetVarName() + _T("_Size");

            // TODO: What's going on with these 2 pixels ?????
            Code << _T("wxSize ") << SizeName << _T(" = ") << _T("wxSize(")
                 << wxString::Format(_T("%d"),(GetBaseProps()->m_Size.X > 2)? (GetBaseProps()->m_Size.X - 2):GetBaseProps()->m_Size.X) << _T(",")
                 << wxString::Format(_T("%d"),(GetBaseProps()->m_Size.Y > 2)? (GetBaseProps()->m_Size.Y - 2):GetBaseProps()->m_Size.Y) << _T(");\n");

            wxString BmpCode = BitmapLabel.BuildCode(GetBaseProps()->m_Size.IsDefault,SizeName,wxsCPP,wxART_BUTTON);

            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxBitmapButton(");
            }
            else
            {
                Code<< _T("Create(");
            }
            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << (BmpCode.empty() ? _T("wxNullBitmap") : BmpCode) << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << _T("wxDefaultSize,")
                << StyleCode(wxsCPP) << _T(",")
                << _T("wxDefaultValidator") << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetVarName(),false) << _T(");\n");

            if ( !(BitmapDisabled.Id.empty()||BitmapDisabled.Client.empty())|| !(BitmapDisabled.FileName.empty()) )
            {
                BmpCode = BitmapDisabled.BuildCode(GetBaseProps()->m_Size.IsDefault,SizeName,wxsCPP,wxART_OTHER);
                Code << GetVarName() << _T("->SetBitmapDisabled(") << BmpCode << _T(");\n");
            }
            if ( !(BitmapSelected.Id.empty()||BitmapSelected.Client.empty())|| !(BitmapSelected.FileName.empty()) )
            {
                BmpCode = BitmapSelected.BuildCode(GetBaseProps()->m_Size.IsDefault,SizeName,wxsCPP,wxART_OTHER);
                Code << GetVarName() << _T("->SetBitmapSelected(") << BmpCode << _T(");\n");
            }
            if ( !(BitmapFocus.Id.empty()||BitmapFocus.Client.empty())|| !(BitmapFocus.FileName.empty()) )
            {
                BmpCode = BitmapFocus.BuildCode(GetBaseProps()->m_Size.IsDefault,SizeName,wxsCPP,wxART_OTHER);
                Code << GetVarName() << _T("->SetBitmapFocus(") << BmpCode << _T(");\n");
            }

            if ( IsDefault ) Code << GetVarName() << _T("->SetDefault();\n");
            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsBitmapButton::OnBuildCreatingCode"),Language);
        }
    }
}


wxObject* wxsBitmapButton::OnBuildPreview(wxWindow* Parent,long Flags)
{
    // TODO: What's going on with these pixels ?????
    // Reduce the size of the bitmap by 2 pixel for width and height of the bitmap button
    wxSize BitmapSize = wxSize((GetBaseProps()->m_Size.X > 2)? (GetBaseProps()->m_Size.X - 2):GetBaseProps()->m_Size.X,(GetBaseProps()->m_Size.Y > 2)? (GetBaseProps()->m_Size.Y - 2):GetBaseProps()->m_Size.Y);

    wxBitmapButton* Preview = new wxBitmapButton(Parent,GetId(),BitmapLabel.GetPreview(BitmapSize),Pos(Parent),BitmapSize,Style());

    if ( !(BitmapDisabled.Id.empty()||BitmapDisabled.Client.empty())|| !(BitmapDisabled.FileName.empty()) )
        Preview->SetBitmapDisabled(BitmapDisabled.GetPreview(BitmapSize));

    if ( !(BitmapSelected.Id.empty()||BitmapSelected.Client.empty())|| !(BitmapSelected.FileName.empty()) )
        Preview->SetBitmapSelected(BitmapSelected.GetPreview(BitmapSize));

    if ( !(BitmapFocus.Id.empty()||BitmapFocus.Client.empty())|| !(BitmapFocus.FileName.empty()) )
        Preview->SetBitmapFocus(BitmapFocus.GetPreview(BitmapSize));

    if ( IsDefault ) Preview->SetDefault();
    return SetupWindow(Preview,Flags);
}


void wxsBitmapButton::OnEnumWidgetProperties(long Flags)
{
    WXS_BITMAP(wxsBitmapButton,BitmapLabel,0,_("Bitmap"),_T("bitmap"),_T("wxART_OTHER"))
    WXS_BITMAP(wxsBitmapButton,BitmapDisabled,0,_("Disabled bmp."),_T("disabled"),_T("wxART_OTHER"))
    WXS_BITMAP(wxsBitmapButton,BitmapSelected,0,_("Pressed bmp."),_T("selected"),_T("wxART_OTHER"))
    WXS_BITMAP(wxsBitmapButton,BitmapFocus,0,_("Focused bmp."),_T("focus"),_T("wxART_OTHER"))
    WXS_BOOL  (wxsBitmapButton,IsDefault,0,_("Is default"),_T("default"),false)
}

void wxsBitmapButton::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            Decl.Add(_T("<wx/bmpbuttn.h>"));
            Def.Add(_T("<wx/bitmap.h>"));
            Def.Add(_T("<wx/image.h>"));
            Def.Add(_T("<wx/artprov.h>"));
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsBitmapButtonn::OnEnumDeclFiles"),Language);
        }
    }
}
