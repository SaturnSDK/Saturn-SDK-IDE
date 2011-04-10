#ifndef DEBUGGERSETTINGSCOMMONPANEL_H
#define DEBUGGERSETTINGSCOMMONPANEL_H

#ifndef CB_PRECOMP
	//(*HeadersPCH(DebuggerSettingsCommonPanel)
	#include <wx/checkbox.h>
	#include <wx/sizer.h>
	#include <wx/panel.h>
	//*)
#endif
//(*Headers(DebuggerSettingsCommonPanel)
//*)

class DebuggerSettingsCommonPanel: public wxPanel
{
	public:

		DebuggerSettingsCommonPanel(wxWindow* parent);
		virtual ~DebuggerSettingsCommonPanel();

		void SaveChanges();

	private:

		//(*Declarations(DebuggerSettingsCommonPanel)
		wxCheckBox* m_autoBuild;
		wxCheckBox* m_jumpOnDoubleClick;
		wxCheckBox* m_autoSwitch;
		wxCheckBox* m_debuggersLog;
		//*)

		//(*Identifiers(DebuggerSettingsCommonPanel)
		static const long ID_AUTOBUILD;
		static const long ID_AUTOSWITCH;
		static const long ID_DEBUGGERS_LOG;
		static const long ID_JUMP_ON_DOUBLE_CLICK;
		//*)

		//(*Handlers(DebuggerSettingsCommonPanel)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
