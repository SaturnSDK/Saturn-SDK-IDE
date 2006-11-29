#include "wxsgridsizer.h"

namespace
{
    wxsRegisterItem<wxsGridSizer> Reg(_T("GridSizer"),wxsTSizer,_T("Layout"),40);
}

wxsGridSizer::wxsGridSizer(wxsItemResData* Data):
    wxsSizer(Data,&Reg.Info),
    Cols(3),
    Rows(0)
{
}

wxSizer* wxsGridSizer::OnBuildSizerPreview(wxWindow* Parent)
{
	return new wxGridSizer(Rows,Cols,
        VGap.GetPixels(Parent),HGap.GetPixels(Parent));
}

void wxsGridSizer::OnBuildSizerCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            Code << GetVarName() << _T(" = new wxGridSizer(")
                 << wxString::Format(_T("%d,%d"),Rows,Cols) << _T(",")
                 << VGap.GetPixelsCode(WindowParent,wxsCPP) << _T(",")
                 << HGap.GetPixelsCode(WindowParent,wxsCPP) << _T(");\n");
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsGridSizer::OnBuildSizerCreatingCode"),Language);
        }
    }
}

void wxsGridSizer::OnEnumSizerProperties(long Flags)
{
    WXS_LONG(wxsGridSizer,Cols,0,_("Cols"),_T("cols"),0);
    WXS_LONG(wxsGridSizer,Rows,0,_("Rows"),_T("rows"),0);
    WXS_DIMENSION(wxsGridSizer,VGap,0,_("V-Gap"),_("V-Gap in dialog units"),_T("vgap"),0,false);
    WXS_DIMENSION(wxsGridSizer,HGap,0,_("H-Gap"),_("H,y-Gap in dialog units"),_T("hgap"),0,false);
}

void wxsGridSizer::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/sizer.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsGridSizer::OnEnumDeclFiles"),Language);
    }
}
