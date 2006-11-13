#include "wxscontainer.h"
#include <messagemanager.h>

wxsContainer::wxsContainer(
    wxsItemRes* Resource,
    const wxsItemInfo* Info,
    long PropertiesFlags,
    const wxsEventDesc* EventArray,
    const wxsStyle* StyleSet,
    const wxString& DefaultStyle):
        wxsParent(Resource,Info,PropertiesFlags,EventArray),
        m_StyleSet(StyleSet),
        m_DefaultStyle(DefaultStyle),
        m_StyleBits(0),
        m_ExStyleBits(0)
{
    if ( m_StyleSet )
    {
        // TODO: Code it when stylesa re done
//        wxsStyleProperty::SetFromString(StyleBits,DefaultStyle,StyleSet,false);
    }
}

bool wxsContainer::OnCanAddChild(wxsItem* Item,bool ShowMessage)
{
    if ( Item->GetType() == wxsTSpacer )
    {
        if ( ShowMessage )
        {
            wxMessageBox(_("Spacer can be added into sizer only"));
        }
        return false;
    }

    if ( Item->GetType() == wxsTSizer )
    {
        if ( GetChildCount() > 0 )
        {
            if ( GetChild(0)->GetType() == wxsTSizer )
            {
                if ( ShowMessage )
                {
                    wxMessageBox(_("This item can contain only one sizer"));
                }
                return false;
            }
            else
            {
                if ( ShowMessage )
                {
                    wxMessageBox(_("Item can not contain sizer if it has other items inside"));
                }
                return false;
            }
        }
        return true;
    }

    if ( GetChildCount() > 0 )
    {
        if ( GetChild(0)->GetType() == wxsTSizer )
        {
            if ( ShowMessage )
            {
                wxMessageBox(_("Item contains sizer, can not add other items into it."));
            }
            return false;
        }
    }

    return true;
}

void wxsContainer::OnEnumItemProperties(long Flags)
{
    OnEnumContainerProperties(Flags);
    // TODO
//    if ( StyleSet )
//    {
//        WXS_STYLE(wxsContainer,StyleBits,0,_("Style"),_T("style"),StyleSet,DefaultStyle);
//        WXS_EXSTYLE(wxsContainer,ExStyleBits,0,_("Extra style"),_T("exstyle"),StyleSet,wxEmptyString);
//    }
}

void wxsContainer::OnAddItemQPP(wxsAdvQPP* QPP)
{
    OnAddContainerQPP(QPP);
}

wxWindow* wxsContainer::SetupWindow(wxWindow* Preview,bool IsExact)
{
    GetBaseProps()->SetupWindow(Preview,IsExact);
    // TODO
//    long ExStyle = wxsStyleProperty::GetWxStyle(ExStyleBits,StyleSet,true);
//    if ( ExStyle != 0 )
//    {
//        Preview->SetExtraStyle(Preview->GetExtraStyle() | ExStyle);
//    }
    return Preview;
}

void wxsContainer::SetupWindowCode(wxString& Code,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            GetBaseProps()->BuildSetupWindowCode(Code,GetVarName(),wxsCPP);
            // TODO
//            if ( ExStyleBits )
//            {
//                wxString ExStyleStr = wxsStyleProperty::GetString(ExStyleBits,StyleSet,true,wxsCPP);
//                if ( ExStyleStr != _T("0") )
//                {
//                    wxString VarAccess = GetVarName().empty() ? _T("") : GetVarName() + _T("->");
//
//                    Code << VarAccess << _T("SetExtraStyle(") <<
//                            VarAccess << _T("GetExtraStyle() | ") <<
//                            ExStyleStr << _T(");\n");
//                }
//            }
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsContainer::SetupWindowCode"),Language);
        }
    }

}

void wxsContainer::AddChildrenPreview(wxWindow* This,bool Exact,bool StoreInLastPreview)
{
    for ( int i=0; i<GetChildCount(); i++ )
    {
        GetChild(i)->BuildPreview(This,Exact,StoreInLastPreview);
    }

    // TODO: Move this into child classes since it's not what this function should do
//    if ( GetBaseProps()->Size.IsDefault )
//    {
//        This->Fit();
//    }
}

void wxsContainer::AddChildrenCode(wxString& Code,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            for ( int i=0; i<GetChildCount(); i++ )
            {
                wxString ThisName = GetParent()?GetVarName():_T("this");
                GetChild(i)->BuildCreatingCode(Code,ThisName,wxsCPP);
                if ( GetChild(i)->GetType() == wxsTSizer )
                {
                    wxString SizerName = GetChild(i)->GetVarName();
                    if ( GetParent() )
                    {
                        Code << GetVarName() << _T("->");
                    }

                    Code << _T("SetSizer(") << SizerName << _T(");\n");

                    // TODO
//                    if ( BaseProps.Size.IsDefault )
//                    {
//                        Code << SizerName << _T("->Fit(") << ThisName << _T(");\n");
//                    }

                    Code << SizerName << _T("->SetSizeHints(") << ThisName << _T(");\n");
                }
            }
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsContainer::AddChildrenCode"),Language);
        }
    }
}
