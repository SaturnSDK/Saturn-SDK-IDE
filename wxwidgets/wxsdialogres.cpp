#include "wxsdialogres.h"
#include "wxsitemresdata.h"

namespace
{
    class wxsDialogResPreview: public wxDialog
    {
        public:

            wxsDialogResPreview(wxWindow* Parent,wxsItemResData* Data): m_Data(Data)
            {
                m_Data->GetRootItem()->BuildPreview(this,wxsItem::pfExact);
                wxAcceleratorEntry Acc[1];
                Acc[0].Set(wxACCEL_NORMAL,WXK_ESCAPE,wxID_EXIT);
                wxAcceleratorTable Table(1,Acc);
                SetAcceleratorTable(Table);
            }

            ~wxsDialogResPreview()
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

    BEGIN_EVENT_TABLE(wxsDialogResPreview,wxDialog)
        EVT_MENU(wxID_EXIT,wxsDialogResPreview::OnEscape)
        EVT_CLOSE(wxsDialogResPreview::OnClose)
    END_EVENT_TABLE()
}

const wxString wxsDialogRes::ResType = _T("wxDialog");

wxString wxsDialogRes::OnGetAppBuildingCode()
{
    return wxString::Format(
        _T("%s Dlg(NULL);\n")
        _T("Dlg.ShowModal();\n")
        _T("SetTopWindow(&Dlg);\n")
        _T("wxsOK = false;\n"),
            GetResourceName().c_str());
}

wxWindow* wxsDialogRes::OnBuildExactPreview(wxWindow* Parent,wxsItemResData* Data)
{
    wxDialog* Dlg = new wxsDialogResPreview(Parent,Data);
    Dlg->Show();
    return Dlg;
}
