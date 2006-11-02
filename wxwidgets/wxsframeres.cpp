#include "wxsframeres.h"

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
