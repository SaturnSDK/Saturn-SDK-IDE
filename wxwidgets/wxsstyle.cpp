#include "wxsstyle.h"
#include <wx/tokenzr.h>
#include <messagemanager.h>

wxsStyleSet::wxsStyleSet(const wxChar* DefaultStyle): DefaultStr(DefaultStyle)
{
}

wxsStyleSet::~wxsStyleSet()
{
}

void wxsStyleSet::AddStyle(const wxChar* Name,long Value,long Flags)
{
    if ( Value == ((long)-1) )
    {
        // Skipping style as it declares new category, not yet supported
        return;
    }

    if ( Flags & wxsSFExt )
    {
        DBGLOG(_T("Adding style: \"%s\" (Value:%d, Flags:%d)"),Name,Value,Flags);
        // Extra style
        ExStyleNames.Add(Name);
        ExStyleBits.Add(1L<<ExStyleBits.Count());
        ExStyleValues.Add(Value);
        ExStyleFlags.Add(Flags);
    }
    else
    {
        DBGLOG(_T("Adding extra style: \"%s\" (Value:%d, Flags:%d)"),Name,Value,Flags);
        // Normal style
        StyleNames.Add(Name);
        StyleBits.Add(1L<<StyleBits.Count());
        StyleValues.Add(Value);
        StyleFlags.Add(Flags);
    }
}

void wxsStyleSet::EndStyle()
{
    StyleNames.Shrink();
    StyleBits.Shrink();
    StyleValues.Shrink();
    StyleFlags.Shrink();
    ExStyleNames.Shrink();
    ExStyleBits.Shrink();
    ExStyleValues.Shrink();
    ExStyleFlags.Shrink();

    Default = GetBits(DefaultStr,false);
}

long wxsStyleSet::GetBits(const wxString& String,bool IsExtra) const
{
    long Result = 0;
    wxStringTokenizer Tkn(String, wxT("| \t\n"), wxTOKEN_STRTOK);
    const wxArrayString& NamesArray = IsExtra ? ExStyleNames : StyleNames;
    const wxArrayLong& BitsArray = IsExtra ? ExStyleBits : StyleBits;

    while ( Tkn.HasMoreTokens() )
    {
        int Index = NamesArray.Index(Tkn.GetNextToken());
        if ( Index != wxNOT_FOUND )
        {
            Result |= BitsArray[Index];
        }
    }
    return Result;
}

wxString wxsStyleSet::GetString(long Bits,bool IsExtra,wxsCodingLang Language) const
{
    switch ( Language )
    {
        case wxsCPP:
        {
            wxString Result;
            const wxArrayString& NamesArray = IsExtra ? ExStyleNames : StyleNames;
            const wxArrayLong& BitsArray = IsExtra ? ExStyleBits : StyleBits;
            size_t Cnt = BitsArray.Count();
            for ( size_t i=0; i<Cnt; i++ )
            {
                if ( Bits & BitsArray[i] )
                {
                    Result.Append(NamesArray[i]);
                    Result.Append(_T('|'));
                }
            }

            if ( Result.empty() )
            {
                return _T("0");
            }

            Result.RemoveLast();
            return Result;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsStyleSet::BitsToString"),Language);
        }
    }
    return wxEmptyString;
}

long wxsStyleSet::GetWxStyle(long Bits,bool IsExtra) const
{
    long Result = 0L;
    const wxArrayLong& BitsArray = IsExtra ? ExStyleBits : StyleBits;
    const wxArrayLong& ValuesArray = IsExtra ? ExStyleValues : StyleValues;

    for ( size_t i = BitsArray.Count(); i-->0; )
    {
        if ( BitsArray[i] & Bits )
        {
            Result |= ValuesArray[i];
        }
    }

    return Result;
}

