#include <sdk_precomp.h>
#ifndef CB_PRECOMP
    #include <wx/string.h>
    #include <globals.h>
#endif

#include "sc_base_types.h"

namespace ScriptBindings
{
    // the _T() function for scripts
    wxString static_T(const SQChar* str)
    {
        return cbC2U(str);
    }

    // wxString operator+
    SQInteger wxString_OpAdd(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        wxString result;
        wxString str1 = *SqPlus::GetInstance<wxString>(v, 1);
        if (sa.GetType(2) == OT_INTEGER)
            result.Printf(_T("%s%d"), str1.c_str(), sa.GetInt(2));
        else if (sa.GetType(2) == OT_FLOAT)
            result.Printf(_T("%s%f"), str1.c_str(), sa.GetFloat(2));
        else if (sa.GetType(2) == OT_USERPOINTER)
            result.Printf(_T("%s%p"), str1.c_str(), sa.GetUserPointer(2));
        else if (sa.GetType(2) == OT_STRING)
            result.Printf(_T("%s%s"), str1.c_str(), cbC2U(sa.GetString(2)).c_str());
        else
            result = str1 + *SqPlus::GetInstance<wxString>(v, 2);
        return SqPlus::ReturnCopy(v, result);
    }

    void Register_wxTypes()
    {
        SqPlus::RegisterGlobal(&static_T, "_T");

        typedef int(wxString::*WXSTR_FIRST_STR)(const wxString&)const;

        SqPlus::SQClassDef<wxString>("wxString").
                emptyCtor().
                staticFuncVarArgs(&wxString_OpAdd, "_add").
                func<WXSTR_FIRST_STR>(&wxString::First, "Find").
                func(&wxString::IsEmpty, "IsEmpty").
                func(&wxString::Length, "Length").
                func(&wxString::Length, "length").
                func(&wxString::Length, "len").
                func(&wxString::Length, "size");

        SqPlus::SQClassDef<wxArrayString>("wxArrayString").
                emptyCtor().
                func(&wxArrayString::Add, "Add").
                func(&wxArrayString::Clear, "Clear").
//                func(&wxArrayString::Index, "Index").
                func(&wxArrayString::GetCount, "GetCount").
                func(&wxArrayString::Item, "Item");
    }
};
