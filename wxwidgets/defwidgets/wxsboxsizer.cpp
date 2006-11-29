#include "wxsboxsizer.h"

namespace
{
    const long    OrientValues[] = { wxHORIZONTAL, wxVERTICAL, 0 };
    const wxChar* OrientNames[]  = { _T("wxHORIZONTAL"), _T("wxVERTICAL"), NULL };

    class OrientProp: public wxsEnumProperty
    {
        public:
            OrientProp(int Offset):
                wxsEnumProperty(
                    _("Orientation"),
                    _T("orient"),
                    Offset,
                    OrientValues,
                    OrientNames,
                    false,
                    wxHORIZONTAL,
                    true)
            {}

            virtual const wxString GetTypeName()
            {
                return _T("Sizer Orientation");
            }
    };

    wxsRegisterItem<wxsBoxSizer> Reg(_T("BoxSizer"), wxsTSizer, _T("Layout"), 0);
}

wxsBoxSizer::wxsBoxSizer(wxsItemResData* Data):
    wxsSizer(Data,&Reg.Info),
    Orient(wxHORIZONTAL)
{
}

wxSizer* wxsBoxSizer::OnBuildSizerPreview(wxWindow* Parent)
{
    return new wxBoxSizer(Orient);
}

void wxsBoxSizer::OnBuildSizerCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            Code << GetVarName() << _T(" = new wxBoxSizer(");
            if ( Orient != wxHORIZONTAL ) Code << _T("wxVERTICAL"); else Code << _T("wxHORIZONTAL");
            Code << _T(");\n");
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsBoxSizer::OnBuildSizerCreatingCode"),Language);
        }
    }
}

void wxsBoxSizer::OnEnumSizerProperties(long Flags)
{
    static OrientProp Prop(wxsOFFSET(wxsBoxSizer,Orient));
    Property(Prop,0);
}

void wxsBoxSizer::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/sizer.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxBoxSizer::OnEnumDeclFiles"),Language);
    }
}
