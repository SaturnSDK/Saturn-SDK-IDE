#include "wxschecklistbox.h"

namespace
{
    wxsRegisterItem<wxsCheckListBox> Reg(_T("CheckListBox"),wxsTWidget,_T("Standard"),50);

    WXS_ST_BEGIN(wxsCheckListBoxStyles,_T(""))
        WXS_ST_CATEGORY("wxCheckListBox")
        WXS_ST_MASK(wxLB_HSCROLL,wxsSFWin,0,true) // Windows ONLY
        WXS_ST(wxLB_SINGLE)
        WXS_ST(wxLB_MULTIPLE)
        WXS_ST(wxLB_EXTENDED)
        WXS_ST(wxLB_ALWAYS_SB)
        WXS_ST(wxLB_NEEDED_SB)
        WXS_ST(wxLB_SORT)
    WXS_ST_END()


    WXS_EV_BEGIN(wxsCheckListBoxEvents)
        WXS_EVI(EVT_CHECKLISTBOX,wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,wxCommandEvent,Toggled)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsCheckListBox::wxsCheckListBox(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsCheckListBoxEvents,
        wxsCheckListBoxStyles)
{}


void wxsCheckListBox::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxCheckListBox(");
            }
            else
            {
                Code<< _T("Create(");
            }
            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << _T("0,NULL,")
                << StyleCode(wxsCPP) << _T(",")
                << _T("wxDefaultValidator") << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetVarName(),false) << _T(");\n");

            for ( size_t i = 0; i <  ArrayChoices.GetCount(); ++i )
            {
                if ( ArrayChecks[i] )
                {
                    Code << GetVarName() << _T("->Check( ");
                }
                Code << GetVarName() << _T("->Append(") << wxsCodeMarks::WxString(wxsCPP,ArrayChoices[i]) << _T(")");
                if ( ArrayChecks[i] )
                {
                    Code << GetVarName() << _T(" )");
                }
                Code << _T(";\n");
            }
            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsCheckListBox::OnBuildCreatingCode"),Language);
        }
    }
}


wxObject* wxsCheckListBox::OnBuildPreview(wxWindow* Parent,bool Exact,bool)
{
    wxCheckListBox* Preview = new wxCheckListBox(Parent,GetId(),Pos(Parent),Size(Parent),0,NULL,Style());
    for ( size_t i = 0; i < ArrayChoices.GetCount(); ++i )
    {
        int Val = Preview->Append(ArrayChoices[i]);
        if ( ArrayChecks[i] )
        {
            Preview->Check(Val);
        }
    }
    return SetupWindow(Preview,Exact);
}

void wxsCheckListBox::OnEnumWidgetProperties(long Flags)
{
    WXS_ARRAYSTRINGCHECK(wxsCheckListBox,ArrayChoices,ArrayChecks,0,_("Choices"),_T("content"),_T("item"));
}

void wxsCheckListBox::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/checklst.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsCheckListBox::OnEnumDeclFiles"),Language);
    }
}
