#include "wxschoice.h"

namespace
{
    wxsRegisterItem<wxsChoice> Reg(_T("Choice"),wxsTWidget,_T("Standard"),70);

    WXS_ST_BEGIN(wxsChoiceStyles,_T(""))
        WXS_ST_CATEGORY("wxChoice")
        WXS_ST(wxCB_SORT)
    WXS_ST_END()


    WXS_EV_BEGIN(wxsChoiceEvents)
        WXS_EVI(EVT_CHOICE,wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEvent,Select)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsChoice::wxsChoice(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsBaseProperties::flAll,
        wxsChoiceEvents,
        wxsChoiceStyles),
    DefaultSelection(-1)
{}


void wxsChoice::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            Code<< GetVarName() << _T(" = new wxChoice(")
                << WindowParent << _T(",")
                << GetIdName() << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(");\n");

            for ( size_t i = 0; i <  ArrayChoices.GetCount(); ++i )
            {
                if ( DefaultSelection == (int)i )
                {
                    Code << GetVarName() << _T("->SetSelection( ");
                }
                Code << GetVarName() << _T("->Append(") << wxsCodeMarks::WxString(wxsCPP,ArrayChoices[i]) << _T(")");
                if ( DefaultSelection == (int)i )
                {
                    Code << _T(" )");
                }
                Code << _T(";\n");
            }

            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsChoice::OnBuildCreatingCode"),Language);
        }
    }
}

wxObject* wxsChoice::OnBuildPreview(wxWindow* Parent,bool Exact,bool)
{
    wxChoice* Preview = new wxChoice(Parent,GetId(),Pos(Parent),Size(Parent),Style());

    for ( size_t i = 0; i <  ArrayChoices.GetCount(); ++i )
    {
        int Val = Preview->Append(ArrayChoices[i]);
        if ( (int)i == DefaultSelection )
        {
            Preview->SetSelection(Val);
        }
    }
    return SetupWindow(Preview,Exact);
}


void wxsChoice::OnEnumWidgetProperties(long Flags)
{
    WXS_ARRAYSTRING(wxsChoice,ArrayChoices,0,_("Choices"),_T("content"),_T("item"))
    WXS_LONG(wxsChoice,DefaultSelection,0,_("Default"),_T("default"),0)
}

void wxsChoice::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/choice.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsChoice::OnEnumDeclFiles"),Language);
    }
}
