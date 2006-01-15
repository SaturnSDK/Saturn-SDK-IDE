#ifndef SELECTINCLUDEFILE_H
#define SELECTINCLUDEFILE_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
	//(*Headers(SelectIncludeFile)
	#include <wx/button.h>
	#include <wx/dialog.h>
	#include <wx/intl.h>
	#include <wx/listbox.h>
	#include <wx/sizer.h>
	#include <wx/xrc/xmlres.h>
	//*)
#endif

class SelectIncludeFile: public wxDialog
{
	public:

		SelectIncludeFile(wxWindow* parent,wxWindowID id = -1);
		virtual ~SelectIncludeFile();
		void AddListEntries(const wxArrayString& IncludeFile);
		wxString GetIncludeFile() const {return m_SelectedIncludeFile;}

		//(*Identifiers(SelectIncludeFile)
		//*)

	protected:

		//(*Handlers(SelectIncludeFile)
		void OnOk(wxCommandEvent& event);
		void OnCancel(wxCommandEvent& event);
		//*)

		//(*Declarations(SelectIncludeFile)
		wxListBox* ListBox1;
		//*)

	private:
		wxString m_SelectedIncludeFile;
		DECLARE_EVENT_TABLE()
};

#endif // SELECTINCLUDEFILE_H
