#include "wxsframe.h"

namespace
{
    wxsRegisterItem<wxsFrame> Reg( _T("Frame"), wxsTContainer, _T(""), 0 );
}


WXS_ST_BEGIN(wxsFrameStyles)
    WXS_ST_CATEGORY("wxFrame")
    WXS_ST(wxSTAY_ON_TOP)
    WXS_ST(wxCAPTION)
    WXS_ST(wxDEFAULT_DIALOG_STYLE)
    WXS_ST(wxDEFAULT_FRAME_STYLE)
    WXS_ST(wxTHICK_FRAME)
    WXS_ST(wxSYSTEM_MENU)
    WXS_ST(wxRESIZE_BORDER)
    WXS_ST(wxRESIZE_BOX)
    WXS_ST(wxCLOSE_BOX)

    WXS_ST(wxFRAME_NO_TASKBAR)
    WXS_ST(wxFRAME_SHAPED)
    WXS_ST(wxFRAME_TOOL_WINDOW)
    WXS_ST(wxFRAME_FLOAT_ON_PARENT)
    WXS_ST(wxMAXIMIZE_BOX)
    WXS_ST(wxMINIMIZE_BOX)
    WXS_ST(wxSTAY_ON_TOP)

 //   WXS_ST(wxNO_3D)
    WXS_ST(wxTAB_TRAVERSAL)
    WXS_ST(wxWS_EX_VALIDATE_RECURSIVELY)
    WXS_EXST_MASK(wxDIALOG_EX_CONTEXTHELP,wxsSFWin,0,true)
    WXS_EXST_MASK(wxDIALOG_EX_METAL,wxsSFOSX,0,true)

// NOTE (cyberkoa##): wxMINIMIZE, wxMAXIMIZE are in the HELP file but not in XRC
//#ifdef __WXMSW__
// NOTE (cyberkoa##): There is a style wxICONIZE which is identical to wxMINIMIZE , not included.
//    WXS_ST(wxMINIMIZE)
//    WXS_ST(wxMAXIMIZE)
//#endif

WXS_ST_END()

WXS_EV_BEGIN(wxsFrameEvents)
    WXS_EV_DEFAULTS()
WXS_EV_END()

wxsFrame::wxsFrame(wxsItemResData* Data):
    wxsContainer(
        Data,
        &Reg.Info,
        wxsBaseProperties::flContainer,
        wxsFrameEvents,
        wxsFrameStyles,
        _T("wxDEFAULT_FRAME_STYLE")),
    Title(_("Frame")),
    Centered(false)
{}

void wxsFrame::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            Code<< _T("Create(")
                << WindowParent << _T(",")
                << GetIdName() << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,Title) << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(");\n");

            SetupWindowCode(Code,Language);
            // TODO: Setup Icon

            AddChildrenCode(Code,wxsCPP);
            if ( Centered )
            {
                Code<<_T("Centre();\n");
            }

            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsFrame::OnBuildCreatingCode"),Language);
        }
    }
}

wxObject* wxsFrame::OnBuildPreview(wxWindow* Parent,bool Exact,bool Store)
{
    wxWindow* NewItem = NULL;
    wxFrame* Frm = NULL;

    // In case of frame and dialog when in "Exact" mode, we do not create
    // new object, but use Parent and call Create for it.
    if ( Exact )
    {
        Frm = wxDynamicCast(Parent,wxFrame);
        if ( Frm )
        {
            Frm->Create(NULL,GetId(),Title,Pos(wxTheApp->GetTopWindow()),Size(wxTheApp->GetTopWindow()),Style());
        }
        NewItem = Frm;
    }
    else
    {
        // In preview we simulate dialog using panel
        NewItem = new wxPanel(Parent,GetId(),wxDefaultPosition,wxDefaultSize,0/*wxRAISED_BORDER*/);
    }

    SetupWindow(NewItem,Exact);
    AddChildrenPreview(NewItem,Exact,Store);

    if ( Frm && Centered )
    {
        Frm->Centre();
    }

    return NewItem;
}

void wxsFrame::OnEnumContainerProperties(long Flags)
{
    WXS_STRING(wxsFrame,Title,0,_("Title"),_T("title"),_T(""),false,false)
    WXS_BOOL  (wxsFrame,Centered,0,_("Centered"),_T("centered"),false);
    WXS_ICON  (wxsFrame,Icon,0,_T("Icon"),_T("icon"),_T("wxART_FRAME_ICON"));
}


void wxsFrame::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/frame.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsFrame::OnEnumDeclFiles"),Language);
    }
}
