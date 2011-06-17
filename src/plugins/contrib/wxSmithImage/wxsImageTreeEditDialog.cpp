#include "wxsImageTreeEditDialog.h"

//(*InternalHeaders(wxsImageTreeEditDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wxsImageTreeEditDialog)
//*)

BEGIN_EVENT_TABLE(wxsImageTreeEditDialog,wxDialog)
	//(*EventTable(wxsImageTreeEditDialog)
	//*)
END_EVENT_TABLE()

wxsImageTreeEditDialog::wxsImageTreeEditDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(wxsImageTreeEditDialog)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
}

wxsImageTreeEditDialog::~wxsImageTreeEditDialog()
{
	//(*Destroy(wxsImageTreeEditDialog)
	//*)
}

