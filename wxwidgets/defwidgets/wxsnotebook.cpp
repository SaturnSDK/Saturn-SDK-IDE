#include "wxsnotebook.h"

// TODO: Add notebook images
namespace
{
    wxsRegisterItem<wxsNotebook> Reg(_T("Notebook"),wxsTContainer,_T("Standard"),61);

    /** \brief Extra parameters for notebook's children */
    class wxsNotebookExtra: public wxsPropertyContainer
    {
        public:

            wxsNotebookExtra():
                m_Label(_("Page name")),
                m_Selected(false)
            {}

            wxString m_Label;
            bool m_Selected;

        protected:

            virtual void OnEnumProperties(long Flags)
            {
                WXS_STRING(wxsNotebookExtra,m_Label,0,_("Page name"),_T("label"),_T(""),false,false);
                WXS_BOOL(wxsNotebookExtra,m_Selected,0,_("Page selected"),_T("selected"),false);
            }
    };

    WXS_ST_BEGIN(wxsNotebookStyles,_T(""))
        WXS_ST_CATEGORY("wxNotebook")
        WXS_ST(wxNB_DEFAULT)
        WXS_ST(wxNB_LEFT)
        WXS_ST(wxNB_RIGHT)
        WXS_ST(wxNB_TOP)
        WXS_ST(wxNB_BOTTOM)
        WXS_ST_MASK(wxNB_FIXEDWIDTH,wxsSFWin,0,true)
        WXS_ST_MASK(wxNB_MULTILINE,wxsSFWin,0,true)
        WXS_ST_MASK(wxNB_NOPAGETHEME,wxsSFWin,0,true)
        // NOTE (cyberkoa##): wxNB_FLAT is in HELP (WinCE only) file but not in wxMSW's XRC
        WXS_ST_MASK(wxNB_FLAT,wxsSFWinCE,0,true)
    WXS_ST_END()

    WXS_EV_BEGIN(wxsNotebookEvents)
        WXS_EVI(EVT_NOTEBOOK_PAGE_CHANGED,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,wxNotebookEvent,PageChanged)
        WXS_EVI(EVT_NOTEBOOK_PAGE_CHANGING,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,wxNotebookEvent,PageChanging)
    WXS_EV_END()
}


wxsNotebook::wxsNotebook(wxsItemResData* Data):
    wxsContainer(
        Data,
        &Reg.Info,
        wxsNotebookEvents,
        wxsNotebookStyles),
    m_CurrentSelection(0)
{
}

void wxsNotebook::OnEnumContainerProperties(long Flags)
{
}

bool wxsNotebook::OnCanAddChild(wxsItem* Item,bool ShowMessage)
{
    if ( Item->GetType() == wxsTSizer )
    {
        if ( ShowMessage )
        {
            wxMessageBox(_("Can not add sizer into Notebook.\nAdd panels first"));
        }
        return false;
    }

	return wxsContainer::OnCanAddChild(Item,ShowMessage);
}

wxsPropertyContainer* wxsNotebook::OnBuildExtra()
{
    return new wxsNotebookExtra();
}

wxString wxsNotebook::OnXmlGetExtraObjectClass()
{
    return _T("notebookpage");
}

void wxsNotebook::OnAddChildQPP(wxsItem* Child,wxsAdvQPP* QPP)
{
    // TODO: Code it
}

wxObject* wxsNotebook::OnBuildPreview(wxWindow* Parent,long PreviewFlags)
{
	wxNotebook* Notebook = new wxNotebook(Parent,-1,Pos(Parent),Size(Parent),Style());

	if ( !GetChildCount() && !(PreviewFlags&pfExact) )
	{
	    // Adding additional empty notebook to prevent from having zero-sized notebook
	    Notebook->AddPage(
            new wxPanel(Notebook,-1,wxDefaultPosition,wxSize(50,50)),
            _("No pages"));
	}

	AddChildrenPreview(Notebook,PreviewFlags);

	for ( int i=0; i<GetChildCount(); i++ )
	{
	    wxsItem* Child = GetChild(i);
	    wxsNotebookExtra* Extra = (wxsNotebookExtra*)GetChildExtra(i);

	    wxWindow* ChildPreview = wxDynamicCast(GetChild(i)->GetLastPreview(),wxWindow);
	    if ( !ChildPreview ) continue;

	    bool Selected = (Child == m_CurrentSelection);
	    if ( PreviewFlags & pfExact ) Selected = Extra->m_Selected;

	    Notebook->AddPage(ChildPreview,Extra->m_Label,Selected);
	}

	return Notebook;
}

void wxsNotebook::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            if ( GetParent() )
            {
                Code << GetVarName() << _T(" = new wxNotebook(");
            }
            else
            {
                Code << _T("Create(");
            }
            Code << WindowParent << _T(",")
                 << GetIdName() << _T(",")
                 << PosCode(WindowParent,wxsCPP) << _T(",")
                 << SizeCode(WindowParent,wxsCPP) << _T(",")
                 << StyleCode(wxsCPP) << _T(",")
                 << wxsCodeMarks::WxString(wxsCPP,GetIdName(),false) << _T(");\n");

            AddChildrenCode(Code,wxsCPP);

            for ( int i=0; i<GetChildCount(); i++ )
            {
                wxsNotebookExtra* Extra = (wxsNotebookExtra*)GetChildExtra(i);
                Code << GetVarName() << _T("->AddPage")
                     << GetChild(i)->GetVarName() << _T(",")
                     << wxsCodeMarks::WxString(wxsCPP,Extra->m_Label) << _T(",")
                     << (Extra->m_Selected ? _T("true") : _T("false")) << _T(");\n");
            }

            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsNotebook::OnBuildCreatingCode"),Language);
        }
    }
}

void wxsNotebook::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/notebook.h>")); break;
        default: wxsCodeMarks::Unknown(_T("wxsNotebook::OnEnumDeclFiles"),Language);
    }
}

/*
void wxsNotebook::PreviewMouseEvent(wxMouseEvent& event)
{
	if ( GetPreview() && event.LeftDown() )
	{
	    wxNotebook* Preview = (wxNotebook*)GetPreview();
	    int Hit = Preview->HitTest(wxPoint(event.GetX(),event.GetY()));
        if ( Hit != wxNOT_FOUND )
        {
            CurrentSelection = GetChild(Hit);
            PropertiesChanged(false,false);
        }
	}
}

void wxsNotebook::EnsurePreviewVisible(wxsWidget* Child)
{
	CurrentSelection = NULL;
	for ( int i=0; i<GetChildCount(); ++i )
	{
		if ( GetChild(i) == Child )
		{
			CurrentSelection = Child;
			if ( GetPreview() )
			{
				((wxNotebook*)GetPreview())->SetSelection(i);
			}
		}
	}

	wxsWidget::EnsurePreviewVisible(Child);
}

bool wxsNotebook::ChildReallyVisible(wxsWidget* Child)
{
    return Child == CurrentSelection;
}

*/
