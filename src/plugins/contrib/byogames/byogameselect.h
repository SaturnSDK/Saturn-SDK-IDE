#ifndef BYOGAMESELECT_H
#define BYOGAMESELECT_H

#include <wx/dialog.h>

class wxBoxSizer;
class wxStaticText;
class wxPanel;
class wxStaticBoxSizer;
class wxListBox;
class wxButton;
class wxStaticLine;
class wxCommandEvent;

class byoGameSelect: public wxDialog
{
	public:

		byoGameSelect(wxWindow* parent,wxWindowID id = -1);
		virtual ~byoGameSelect();

		//(*Identifiers(byoGameSelect)
		enum Identifiers
		{
		    ID_LISTBOX1 = 0x1000,
		    ID_PANEL1,
		    ID_STATICLINE1,
		    ID_STATICTEXT1
		};
		//*)

	protected:

		//(*Handlers(byoGameSelect)
		void OnCancel(wxCommandEvent& event);
		void OnPlay(wxCommandEvent& event);
		//*)

		//(*Declarations(byoGameSelect)
		wxBoxSizer* BoxSizer1;
		wxPanel* Panel1;
		wxBoxSizer* BoxSizer3;
		wxStaticText* StaticText1;
		wxBoxSizer* BoxSizer4;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxListBox* m_GamesList;
		wxBoxSizer* BoxSizer2;
		wxButton* Button1;
		wxStaticLine* StaticLine1;
		wxButton* Button2;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif // BYOGAMESELECT_H
