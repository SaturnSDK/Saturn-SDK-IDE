#include "wxscodinglang.h"
#include <messagemanager.h>

namespace wxsCodeMarks
{
    wxString Name(wxsCodingLang Lang)
    {
        switch ( Lang )
        {
            case wxsCPP: return _T("CPP");
            default:;
        }
        return wxEmptyString;
    }

    wxsCodingLang Id(const wxString& Name)
    {
        if ( Name == _T("CPP") ) return wxsCPP;
        return wxsUnknownLanguage;
    }

    wxsCodingLang IdFromExt(const wxString& Extension)
    {
        wxString ExtLower = Extension.Lower();
        if ( (ExtLower==_T("c")) ||
             (ExtLower==_T("h")) ||
             (ExtLower==_T("cpp")) ||
             (ExtLower==_T("hpp")) ) return wxsCPP;
        return wxsUnknownLanguage;
    }

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

    void Unknown(const wxString& Function,wxsCodingLang Lang)
    {
        DBGLOG(
            _T("Unknown coding language %s (%d) in function %s"),
            Name(Lang).c_str(),
            (int)Lang,
            Function.c_str());
    }
}

