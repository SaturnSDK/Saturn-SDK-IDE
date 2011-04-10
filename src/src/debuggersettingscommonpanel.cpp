#include "sdk.h"
#include "debuggersettingscommonpanel.h"

#ifndef CB_PRECOMP
	//(*InternalHeadersPCH(DebuggerSettingsCommonPanel)
	#include <wx/string.h>
	#include <wx/intl.h>
	//*)

	#include <wx/textdlg.h>
#endif

#include "debuggermanager.h"

//(*InternalHeaders(DebuggerSettingsCommonPanel)
//*)

//(*IdInit(DebuggerSettingsCommonPanel)
const long DebuggerSettingsCommonPanel::ID_AUTOBUILD = wxNewId();
const long DebuggerSettingsCommonPanel::ID_AUTOSWITCH = wxNewId();
const long DebuggerSettingsCommonPanel::ID_DEBUGGERS_LOG = wxNewId();
const long DebuggerSettingsCommonPanel::ID_JUMP_ON_DOUBLE_CLICK = wxNewId();
//*)

BEGIN_EVENT_TABLE(DebuggerSettingsCommonPanel,wxPanel)
	//(*EventTable(DebuggerSettingsCommonPanel)
	//*)
END_EVENT_TABLE()

DebuggerSettingsCommonPanel::DebuggerSettingsCommonPanel(wxWindow* parent)
{
	//(*Initialize(DebuggerSettingsCommonPanel)
	wxFlexGridSizer* flexSizer;
	wxBoxSizer* mainSizer;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	mainSizer = new wxBoxSizer(wxVERTICAL);
	flexSizer = new wxFlexGridSizer(0, 1, 0, 0);
	m_autoBuild = new wxCheckBox(this, ID_AUTOBUILD, _("Auto-build project if it is not up-to-date"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_AUTOBUILD"));
	m_autoBuild->SetValue(false);
	flexSizer->Add(m_autoBuild, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	m_autoSwitch = new wxCheckBox(this, ID_AUTOSWITCH, _("When stopping, auto-switch to the first frame with valid source info"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_AUTOSWITCH"));
	m_autoSwitch->SetValue(false);
	flexSizer->Add(m_autoSwitch, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	m_debuggersLog = new wxCheckBox(this, ID_DEBUGGERS_LOG, _("Display Debugger\'s log"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DEBUGGERS_LOG"));
	m_debuggersLog->SetValue(false);
	flexSizer->Add(m_debuggersLog, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	m_jumpOnDoubleClick = new wxCheckBox(this, ID_JUMP_ON_DOUBLE_CLICK, _("Jump on Double-click in Stack trace window"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_JUMP_ON_DOUBLE_CLICK"));
	m_jumpOnDoubleClick->SetValue(false);
	flexSizer->Add(m_jumpOnDoubleClick, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	mainSizer->Add(flexSizer, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	SetSizer(mainSizer);
	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);
	//*)

    m_autoBuild->SetValue(cbDebuggerCommonConfig::GetFlag(cbDebuggerCommonConfig::AutoBuild));
    m_autoBuild->SetToolTip(_("Automatic project build before debug session is started"));

    m_autoSwitch->SetValue(cbDebuggerCommonConfig::GetFlag(cbDebuggerCommonConfig::AutoSwitchFrame));
    m_autoSwitch->SetToolTip(_("When stopping, auto-switch to first frame with valid source info"));

    m_debuggersLog->SetValue(cbDebuggerCommonConfig::GetFlag(cbDebuggerCommonConfig::ShowDebuggersLog));
    m_debuggersLog->SetToolTip(_("If enabled, the debugger&apos;s raw input/output will be logged in a separate log page"));

    m_jumpOnDoubleClick->SetValue(cbDebuggerCommonConfig::GetFlag(cbDebuggerCommonConfig::JumpOnDoubleClick));
}

DebuggerSettingsCommonPanel::~DebuggerSettingsCommonPanel()
{
	//(*Destroy(DebuggerSettingsCommonPanel)
	//*)
}

void DebuggerSettingsCommonPanel::SaveChanges()
{
    cbDebuggerCommonConfig::SetFlag(cbDebuggerCommonConfig::AutoBuild, m_autoBuild->GetValue());
    cbDebuggerCommonConfig::SetFlag(cbDebuggerCommonConfig::AutoSwitchFrame, m_autoSwitch->GetValue());
    cbDebuggerCommonConfig::SetFlag(cbDebuggerCommonConfig::ShowDebuggersLog, m_debuggersLog->GetValue());
    cbDebuggerCommonConfig::SetFlag(cbDebuggerCommonConfig::JumpOnDoubleClick, m_jumpOnDoubleClick->GetValue());
}

