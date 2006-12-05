#include "wxsframeres.h"
#include "wxsitemresdata.h"

namespace
{
    class wxsFrameResPreview: public wxFrame
    {
        public:

            wxsFrameResPreview(wxWindow* Parent,wxsItemResData* Data): m_Data(Data)
            {
                m_Data->GetRootItem()->BuildPreview(this,wxsItem::pfExact);
                wxAcceleratorEntry Acc[1];
                Acc[0].Set(wxACCEL_NORMAL,WXK_ESCAPE,wxID_EXIT);
                wxAcceleratorTable Table(1,Acc);
                SetAcceleratorTable(Table);
            }

            ~wxsFrameResPreview()
            {
                m_Data->NotifyPreviewClosed();
            }

            void OnEscape(wxCommandEvent& event)
            {
                Close();
            }

            void OnClose(wxCloseEvent& event)
            {
                Destroy();
            }

            wxsItemResData* m_Data;

            DECLARE_EVENT_TABLE()
    };

    BEGIN_EVENT_TABLE(wxsFrameResPreview,wxFrame)
        EVT_MENU(wxID_EXIT,wxsFrameResPreview::OnEscape)
        EVT_CLOSE(wxsFrameResPreview::OnClose)
    END_EVENT_TABLE()
}

const wxString wxsFrameRes::ResType = _T("wxFrame");

wxString wxsFrameRes::OnGetAppBuildingCode()
{
    return wxString::Format(
        _T("%s* Frame = new %s(NULL);\n")
        _T("Frame->Show();\n")
        _T("SetTopWindow(Frame);\n"),
            GetResourceName().c_str(),
            GetResourceName().c_str());
}

wxWindow* wxsFrameRes::OnBuildExactPreview(wxWindow* Parent,wxsItemResData* Data)
{
    wxFrame* Frm = new wxsFrameResPreview(Parent,Data);
    Frm->Show();
    return Frm;
}
