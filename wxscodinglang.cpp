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
            case wxsCPP: return _T("//(*") + BlockName + _T("\n");
            default:     return wxEmptyString;
        }
    }

    wxString Beg(wxsCodingLang Lang,const wxString& BlockName,const wxString& Param)
    {
        switch ( Lang )
        {
            case wxsCPP: return wxString::Format(_T("//(*%s(%s)\n"),BlockName.c_str(),Param.c_str());
            default:     return wxEmptyString;
        }
    }

    wxString End(wxsCodingLang Lang)
    {
        switch ( Lang )
        {
            case wxsCPP: return _T("//*)'n");
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

    wxString String(wxsCodingLang Lang,const wxString& Source)
    {
        switch ( Lang )
        {
            case wxsCPP:
            {
                wxString Result = _T("\"");

                int Len = Source.Length();

                for ( int i=0; i<Len; i++ )
                {
                    wxChar ch = Source.GetChar(i);

                    if ( (unsigned)ch < _T(' ') )
                    {
                        switch ( ch )
                        {
                            case _T('\n') : Result.Append(_T("\\n")); break;
                            case _T('\t') : Result.Append(_T("\\t")); break;
                            case _T('\v') : Result.Append(_T("\\v")); break;
                            case _T('\b') : Result.Append(_T("\\b")); break;
                            case _T('\r') : Result.Append(_T("\\r")); break;
                            case _T('\f') : Result.Append(_T("\\f")); break;
                            case _T('\a') : Result.Append(_T("\\a")); break;
                            default   :
                                {
                                    wxString Formater = wxString::Format(_T("\\%d%d%d"),
                                        ( ch >> 6 ) & 7,
                                        ( ch >> 3 ) & 7,
                                        ( ch >> 0 ) & 7 );
                                    Result.Append(Formater.c_str());
                                }
                        }
                    }
                    else
                    {
                        switch ( ch )
                        {
                            case _T('\\'): Result.Append(_T("\\\\")); break;
                            case _T('\?'): Result.Append(_T("\\\?")); break;
                            case _T('\''): Result.Append(_T("\\\'")); break;
                            case _T('\"'): Result.Append(_T("\\\"")); break;
                            default  : Result.Append(ch);
                        }
                    }
                }

                Result.Append(_T('\"'));
                return Result;
            }

            default:
            {
                Unknown(_T("wxsCodeMarks::String"),Lang);
            }
        }
        return wxEmptyString;
    }

    wxString WxString(wxsCodingLang Lang, const wxString& Source, bool WithTranslation)
    {
        switch ( Lang )
        {
            case wxsCPP:
            {
                if ( Source.empty() )
                {
                    // Always empty string, no matter if we have translation
                    return _T("wxEmptyString");
                }

                if ( WithTranslation )
                {
                    return _T("_(") + String(Lang,Source) + _T(")");
                }
                else
                {
                    return _T("_T(") + String(Lang,Source) + _T(")");
                }
            }

            default:
            {
                Unknown(_T("wxsCodeMarks::WxString"),Lang);
            }
        }
        return wxEmptyString;
    }
}
