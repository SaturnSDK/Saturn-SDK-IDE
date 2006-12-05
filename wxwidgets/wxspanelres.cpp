#include "wxspanelres.h"
#include "wxsitemresdata.h"

namespace
{
    class wxsPanelResPreview: public wxDialog
    {
        public:

            wxsPanelResPreview(wxWindow* Parent,wxsItemResData* Data): m_Data(Data)
            {
                Create(Parent,-1,_("Preview for wxPanel class"));
                wxObject* PreviewObj = m_Data->GetRootItem()->BuildPreview(this,wxsItem::pfExact);
                wxWindow* PreviewWnd = wxDynamicCast(PreviewObj,wxWindow);
                if ( !PreviewWnd )
                {
                    delete PreviewObj;
                }
                else
                {
                    wxSizer* Sizer = new wxBoxSizer(wxHORIZONTAL);
                    Sizer->Add(PreviewWnd,0,wxEXPAND,0);
                    SetSizer(Sizer);
                    Sizer->Layout();
                    Sizer->SetSizeHints(this);
                }
                Center();
                wxAcceleratorEntry Acc[1];
                Acc[0].Set(wxACCEL_NORMAL,WXK_ESCAPE,wxID_EXIT);
                wxAcceleratorTable Table(1,Acc);
                SetAcceleratorTable(Table);
            }

            ~wxsPanelResPreview()
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

    BEGIN_EVENT_TABLE(wxsPanelResPreview,wxDialog)
        EVT_MENU(wxID_EXIT,wxsPanelResPreview::OnEscape)
        EVT_CLOSE(wxsPanelResPreview::OnClose)
    END_EVENT_TABLE()
}

const wxString wxsPanelRes::ResType = _T("wxPanel");

wxWindow* wxsPanelRes::OnBuildExactPreview(wxWindow* Parent,wxsItemResData* Data)
{
    wxDialog* Dlg = new wxsPanelResPreview(Parent,Data);
    Dlg->Show();
    return Dlg;
}
