#include "wxscodinglang.h"

namespace wxsCodeMarks
{
    wxString Beg(wxsCodingLang Lang,const wxString& BlockName)
    {
        switch ( Lang )
        {
            case wxsCPP: return _T("//(*") + BlockName;
            default:     return wxEmptyString;
        }
    }

    wxString Beg(wxsCodingLang Lang,const wxString& BlockName,const wxString& Param)
    {
        switch ( Lang )
        {
            case wxsCPP: return wxString::Format(_T("//(*%s(%s)"),BlockName.c_str(),Param.c_str());
            default:     return wxEmptyString;
        }
    }

    wxString End(wxsCodingLang Lang)
    {
        switch ( Lang )
        {
            case wxsCPP: return _T("//*)");
            default:     return wxEmptyString;
        }
    }
}
