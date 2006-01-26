#ifndef DEBUGGERGDB_H
#define DEBUGGERGDB_H

#include <settings.h> // much of the SDK is here
#include <sdk_events.h>
#include <cbplugin.h>
#include <simpletextlog.h>
#include <pipedprocess.h>
#include <wx/regex.h>
#include <wx/tipwin.h>

#include "debuggerstate.h"
#include "debugger_defs.h"
#include "backtracedlg.h"
#include "disassemblydlg.h"
#include "cpuregistersdlg.h"
#include "breakpointsdlg.h"

extern const wxString g_EscapeChars;

class DebuggerDriver;
class DebuggerCmd;
class Compiler;

class DebuggerTree;
class DisassemblyDlg;
class CPURegistersDlg;
class BacktraceDlg;
class BreakpointsDlg;

class DebuggerGDB : public cbDebuggerPlugin
{
        DebuggerState m_State;
	public:
		DebuggerGDB();
		~DebuggerGDB();
		int Configure();
		int GetConfigurationGroup(){ return cgDebugger; }
        cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
		void BuildMenu(wxMenuBar* menuBar);
		void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0);
		bool BuildToolBar(wxToolBar* toolBar);
		void OnAttach(); // fires when the plugin is attached to the application
		void OnRelease(bool appShutDown); // fires when the plugin is released from the application

		void RunCommand(int cmd);
		void Disassemble();
		void Registers();
		void Backtrace();

		bool AddBreakpoint(const wxString& file, int line);
		bool AddBreakpoint(const wxString& functionSignature);
		bool RemoveBreakpoint(const wxString& file, int line);
		bool RemoveBreakpoint(const wxString& functionSignature);
		bool RemoveAllBreakpoints(const wxString& file = wxEmptyString);

		int Debug();
		void Continue();
		void Next();
		void Step();
		void StepOut();
		void RunToCursor();
		void ToggleBreakpoint();
		void Stop();
		bool Validate(const wxString& line, const char cb);
		bool IsRunning() const { return m_pProcess; }
		int GetExitCode() const { return m_LastExitCode; }

		void SyncEditor(const wxString& filename, int line);

		void Log(const wxString& msg);
		void DebugLog(const wxString& msg);
		void SendCommand(const wxString& cmd);

		DebuggerState& GetState(){ return m_State; }

		void BringAppToFront();

        static void ConvertToGDBFriendly(wxString& str);
        static void ConvertToGDBFile(wxString& str);
        static void ConvertToGDBDirectory(wxString& str, wxString base = _T(""), bool relative = true);
        static void StripQuotes(wxString& str);
	protected:
        void AddSourceDir(const wxString& dir);
	private:
        void DoSwitchLayout(const wxString& config_key);
		void ParseOutput(const wxString& output);
		void ClearActiveMarkFromAllEditors();
		void DoWatches();
        wxString GetEditorWordAtCaret();
        wxString FindDebuggerExecutable(Compiler* compiler);
        int LaunchProcess(const wxString& cmd, const wxString& cwd);
        wxString GetDebuggee(ProjectBuildTarget* target);
        bool IsStopped();

		void OnUpdateUI(wxUpdateUIEvent& event);
		void OnDebug(wxCommandEvent& event);
		void OnStop(wxCommandEvent& event);
		void OnSendCommandToGDB(wxCommandEvent& event);
		void OnAddSymbolFile(wxCommandEvent& event);
		void OnBacktrace(wxCommandEvent& event);
		void OnDisassemble(wxCommandEvent& event);
		void OnRegisters(wxCommandEvent& event);
		void OnViewWatches(wxCommandEvent& event);
		void OnBreakpoints(wxCommandEvent& event);
		void OnEditWatches(wxCommandEvent& event);
		void OnContinue(wxCommandEvent& event);
		void OnNext(wxCommandEvent& event);
		void OnStep(wxCommandEvent& event);
		void OnStepOut(wxCommandEvent& event);
		void OnToggleBreakpoint(wxCommandEvent& event);
		void OnRunToCursor(wxCommandEvent& event);
		void OnBreakpointAdd(CodeBlocksEvent& event);
		void OnBreakpointEdit(CodeBlocksEvent& event);
		void OnBreakpointDelete(CodeBlocksEvent& event);
		void OnValueTooltip(CodeBlocksEvent& event);
		void OnEditorOpened(CodeBlocksEvent& event);
        void OnGDBOutput(wxCommandEvent& event);
        void OnGDBError(wxCommandEvent& event);
        void OnGDBTerminated(wxCommandEvent& event);
        void OnIdle(wxIdleEvent& event);
		void OnTimer(wxTimerEvent& event);
		void OnWatchesChanged(wxCommandEvent& event);
		void OnCursorChanged(wxCommandEvent& event);
        void OnAddWatch(wxCommandEvent& event);
        void OnAttachToProcess(wxCommandEvent& event);
        void OnDetach(wxCommandEvent& event);
        void OnSettings(wxCommandEvent& event);

		wxMenu* m_pMenu;
        SimpleTextLog* m_pLog;
        SimpleTextLog* m_pDbgLog;
		PipedProcess* m_pProcess;
		wxToolBar* m_pTbar;
        int m_PageIndex;
        int m_DbgPageIndex;
		wxRegEx reSource;
		wxString m_LastCmd;
		wxString m_Variable;
		cbCompilerPlugin* m_pCompiler;
		bool m_LastExitCode;
		int m_TargetIndex;
		int m_Pid;
		int m_PidToAttach; // for "attach to process"
		wxTipWindow* m_EvalWin;
		wxString m_LastEval;
		wxRect m_EvalRect;
		wxTimer m_TimerPollDebugger;
		bool m_NoDebugInfo;
		bool m_BreakOnEntry;
		int m_HaltAtLine;
		bool m_HasDebugLog;
		bool m_StoppedOnSignal;

		// current frame info
		StackFrame m_CurrentFrame;

		// extra dialogs
		DebuggerTree* m_pTree;
		DisassemblyDlg* m_pDisassembly;
		CPURegistersDlg* m_pCPURegisters;
		BacktraceDlg* m_pBacktrace;
        BreakpointsDlg* m_pBreakpointsWindow;

		DECLARE_EVENT_TABLE()
};

CB_DECLARE_PLUGIN();

#endif // DEBUGGERGDB_H

