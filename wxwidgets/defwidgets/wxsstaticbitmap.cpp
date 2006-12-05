#include "wxsstaticbitmap.h"

namespace
{
    wxsRegisterItem<wxsStaticBitmap> Reg(_T("StaticBitmap"),wxsTWidget,_T("Standard"),70);

    WXS_ST_BEGIN(wxsStaticBitmapStyles,_T(""))
        WXS_ST_CATEGORY("wxStaticBitmap")
    WXS_ST_END()


    WXS_EV_BEGIN(wxsStaticBitmapEvents)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsStaticBitmap::wxsStaticBitmap(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsStaticBitmapEvents,
        wxsStaticBitmapStyles)
{}

void wxsStaticBitmap::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            wxString SizeName = GetVarName() + _T("_Size");
            Code << _T("wxSize ") << SizeName << _T(" = ") << SizeCode(WindowParent,wxsCPP) << _T(";\n");
            wxString BmpCode = Bitmap.BuildCode(GetBaseProps()->m_Size.IsDefault,SizeName,wxsCPP,wxART_OTHER);
            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxStaticBitmap(");
            }
            else
            {
                Code<< _T("Create(");
            }
            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << (BmpCode.empty() ? _T("wxNullBitmap") : BmpCode) << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeName << _T(",")
                << StyleCode(wxsCPP) << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetVarName(),false) << _T(");\n");

            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsStaticBitmap::OnBuildCreatingCode"),Language);
        }
    }
}


wxObject* wxsStaticBitmap::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxStaticBitmap* Preview = new wxStaticBitmap(Parent,GetId(),Bitmap.GetPreview(Size(Parent)),Pos(Parent),Size(Parent),Style());
    return SetupWindow(Preview,Flags);
}

void wxsStaticBitmap::OnEnumWidgetProperties(long Flags)
{
   WXS_BITMAP(wxsStaticBitmap,Bitmap,0,_("Bitmap"),_T("bitmap"),_T("wxART_OTHER"))
}

void wxsStaticBitmap::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            Decl.Add(_T("<wx/statbmp.h>"));
            Def.Add(_T("<wx/bitmap.h>"));
            Def.Add(_T("<wx/image.h>"));
            Def.Add(_T("<wx/artprov.h>"));
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsStaticBitmap::OnEnumDeclFiles"),Language);
        }
    }
}
