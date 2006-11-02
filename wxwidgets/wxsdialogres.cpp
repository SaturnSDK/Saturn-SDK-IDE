#include "wxsdialogres.h"

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
