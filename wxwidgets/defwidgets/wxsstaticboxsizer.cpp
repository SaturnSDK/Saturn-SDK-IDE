#include "wxsstaticboxsizer.h"

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

    wxsRegisterItem<wxsStaticBoxSizer> Reg(_T("StaticBoxSizer"),wxsTSizer,_T("Layout"),20);
}


wxsStaticBoxSizer::wxsStaticBoxSizer(wxsItemResData* Data):
    wxsSizer(Data,&Reg.Info),
    Orient(wxHORIZONTAL),
    Label(_("Label"))
{
}


wxSizer* wxsStaticBoxSizer::OnBuildSizerPreview(wxWindow* Parent)
{
    return new wxStaticBoxSizer(Orient,Parent,Label);
}

void wxsStaticBoxSizer::OnBuildSizerCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            Code << GetVarName() << _T(" = new wxStaticBoxSizer(");
            if ( Orient != wxHORIZONTAL ) Code << _T("wxVERTICAL"); else Code << _T("wxHORIZONTAL");
            Code << _T(",") << WindowParent << _T(",") << wxsCodeMarks::WxString(wxsCPP,Label) << _T(");\n");
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsStaticBoxSizer::OnBuildSizerCreatingCode"),Language);
        }
    }
}

void wxsStaticBoxSizer::OnEnumSizerProperties(long Flags)
{
    WXS_STRING(wxsStaticBoxSizer,Label,0,_("Label"),_T("label"),wxEmptyString,false,false);
    static OrientProp Prop(wxsOFFSET(wxsStaticBoxSizer,Orient));
    Property(Prop,0);
}

void wxsStaticBoxSizer::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/sizer.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsStaticBoxSizer::OnEnumDeclFiles"),Language);
    }
}
