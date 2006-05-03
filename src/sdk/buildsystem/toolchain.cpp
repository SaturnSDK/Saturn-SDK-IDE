#include "toolchain.h"


ToolChain::ToolChain(const wxString& t_family, const wxString& t_id, const wxString& t_title) : ID(t_id), family(t_family), title(t_title)
{

}

ToolChain::ToolChain() : ID(wxEmptyString), family(wxEmptyString), title(wxEmptyString)
{

}

