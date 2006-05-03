#include "buildtool.h"

#include <wx/filename.h>

BuildTool::BuildTool(const wxString& t_id, const wxString& t_family, const wxString& t_executable)
        : ID(t_id), family(t_family), executable(t_executable)
{
    //emtpy
};


void BuildTool::SetExecutable(const wxString& x)
{
    executable = x;
};





