#include "wxspanel.h"

namespace
{
    wxsRegisterItem<wxsPanel> Reg(_T("Panel"),wxsTContainer, _T("Standard"), 20);

    WXS_ST_BEGIN(wxsPanelStyles)
        WXS_ST_CATEGORY("wxPanel")
        WXS_ST(wxNO_3D)
        WXS_ST(wxTAB_TRAVERSAL)
        WXS_ST(wxWS_EX_VALIDATE_RECURSIVELY)
    WXS_ST_END()

    WXS_EV_BEGIN(wxsPanelEvents)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsPanel::wxsPanel(wxsItemResData* Data):
    wxsContainer(
        Data,
        &Reg.Info,
        wxsBaseProperties::flContainer|wxsItem::flId|wxsItem::flVariable,
        wxsPanelEvents,
        wxsPanelStyles,
        _T("wxTAB_TRAVERSAL"))
{}

void wxsPanel::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            if ( GetParent() ) Code<< GetVarName() << _T(" = new wxPanel(");
            else               Code<< _T("Create(");

            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(");\n");
            SetupWindowCode(Code,Language);
            AddChildrenCode(Code,wxsCPP);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsPanel::OnBuildCreatingCode"),Language);
        }
    }
}

wxObject* wxsPanel::OnBuildPreview(wxWindow* Parent,bool Exact,bool Store)
{
    wxWindow* NewItem = new wxPanel( Parent,GetId(),wxDefaultPosition,wxDefaultSize,Style());
    SetupWindow(NewItem,Exact);
    AddChildrenPreview(NewItem,Exact,Store);
    return NewItem;
}

void wxsPanel::OnEnumContainerProperties(long Flags)
{
}

void wxsPanel::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/panel.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsPanel::OnEnumDeclFiles"),Language);
    }
}
