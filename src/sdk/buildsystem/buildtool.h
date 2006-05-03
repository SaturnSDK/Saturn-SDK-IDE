#ifndef BUILDTOOL_H
#define BUILDTOOL_H
#undef new

#include <wx/string.h>
#include <id.h>

class BuildTool : public ID
{
    wxString family;
    wxString executable;

public:
    BuildTool(){};
    BuildTool(const wxString& t_id, const wxString& family, const wxString& t_executable);

    void SetExecutable(const wxString& x);

    wxString GetExecutable() const {return executable;};

    operator wxString() const {return GetExecutable();};              // wxExecute(buildtool, ...)
    bool operator == (const wxString& s) const {return family == s;}; // if(buildtool == _T("gcc 3.4"))
};


#endif
