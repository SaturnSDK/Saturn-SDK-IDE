#ifndef WXSIMAGETREEEDITDIALOG_H
#define WXSIMAGETREEEDITDIALOG_H

//(*Headers(wxsImageTreeEditDialog)
#include <wx/dialog.h>
//*)

class wxsImageTreeEditDialog: public wxDialog
{
	public:

		wxsImageTreeEditDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxsImageTreeEditDialog();

		//(*Declarations(wxsImageTreeEditDialog)
		//*)

	protected:

		//(*Identifiers(wxsImageTreeEditDialog)
		//*)

	private:

		//(*Handlers(wxsImageTreeEditDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
