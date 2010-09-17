/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef DEBUGGERGDB_H
#define DEBUGGERGDB_H

#include <map>
#include <tr1/memory>

#include <settings.h> // much of the SDK is here
#include <sdk_events.h>
#include <cbplugin.h>
#include <loggers.h>
#include <pipedprocess.h>
#include <wx/regex.h>

#include "remotedebugging.h"
#include "debuggerstate.h"
#include "debugger_defs.h"

extern const wxString g_EscapeChar;

class cbProject;
class TiXmlElement;
class DebuggerDriver;
class DebuggerCmd;
class Compiler;
struct TestIfBelogToProject;

class DebuggerGDB : public cbDebuggerPlugin
{
        DebuggerState m_State;
    public:
        DebuggerGDB();
        ~DebuggerGDB();
        int Configure();
        int GetConfigurationPriority() const { return 0; }
        int GetConfigurationGroup() const { return cgDebugger; }
        cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
        cbConfigurationPanel* GetProjectConfigurationPanel(wxWindow* parent, cbProject* project);
        void OnAttachReal(); // fires when the plugin is attached to the application
        void OnReleaseReal(bool appShutDown); // fires when the plugin is released from the application

        void RunCommand(int cmd);

        cbBreakpoint* AddBreakpoint(const wxString& filename, int line);
        cbBreakpoint* AddDataBreakpoint(const wxString& dataExpression);
        int GetBreakpointsCount() const;
        cbBreakpoint* GetBreakpoint(int index);
        const cbBreakpoint* GetBreakpoint(int index) const;
        void UpdateBreakpoint(cbBreakpoint *breakpoint);
        void DeleteBreakpoint(cbBreakpoint* breakpoint);
        void DeleteAllBreakpoints();
        void ShiftBreakpoint(int index, int lines_to_shift);

        // stack frame calls;
        int GetStackFrameCount() const;
        const cbStackFrame& GetStackFrame(int index) const;
        void SwitchToFrame(int number);
        int GetActiveStackFrame() const;

        // threads
        int GetThreadsCount() const;
        const cbThread& GetThread(int index) const;
        bool SwitchToThread(int thread_number);

        int Debug(bool breakOnEntry);
        void Continue();
        void Next();
        void NextInstruction();
        void StepIntoInstruction();
        void Step();
        void StepOut();
        void RunToCursor(const wxString& filename, int line, const wxString& line_text);
        void SetNextStatement(const wxString& filename, int line);
        void Break();
        void Stop();
        bool Validate(const wxString& line, const char cb);
        bool IsRunning() const { return m_pProcess; }
        bool IsStopped() const;
        int GetExitCode() const { return m_LastExitCode; }

        cbWatch* AddWatch(const wxString& symbol);
        void DeleteWatch(cbWatch *watch);
        bool HasWatch(cbWatch *watch);
        void ShowWatchProperties(cbWatch *watch);
        bool SetWatchValue(cbWatch *watch, const wxString &value);
        void ExpandWatch(cbWatch *watch);
        void CollapseWatch(cbWatch *watch);

        void GetCurrentPosition(wxString &filename, int &line);
        void RequestUpdate(DebugWindows window);

        void AttachToProcess(const wxString& pid);
        void DetachFromProcess();
        bool IsAttachedToProcess() const;

        void Log(const wxString& msg);
        void DebugLog(const wxString& msg);
        void SendCommand(const wxString& cmd, bool debugLog);
        void DoSendCommand(const wxString& cmd);

        DebuggerState& GetState(){ return m_State; }

        void RefreshConfiguration();

        wxArrayString& GetSearchDirs(cbProject* prj);
        RemoteDebuggingMap& GetRemoteDebuggingMap(cbProject* project = 0);

        void OnProjectLoadingHook(cbProject* project, TiXmlElement* elem, bool loading);

        static void ConvertToGDBFriendly(wxString& str);
        static void ConvertToGDBFile(wxString& str);
        static void ConvertToGDBDirectory(wxString& str, wxString base = _T(""), bool relative = true);
        static void StripQuotes(wxString& str);

        void DebuggeeContinued();

    protected:
        cbProject* GetProject() { return m_pProject; }
        void ResetProject() { m_pProcess = NULL; }
        void ConvertDirectory(wxString& str, wxString base, bool relative);
        void CleanupWhenProjectClosed(cbProject *project);
        void CompilerFinished();
    protected:
        void AddSourceDir(const wxString& dir);
    private:
        void ParseOutput(const wxString& output);
        void DoWatches();
        void MarkAllWatchesAsUnchanged();
        int LaunchProcess(const wxString& cmd, const wxString& cwd);
        int DoDebug(bool breakOnEntry);
        void DoBreak(bool temporary);

        void OnAddSymbolFile(wxCommandEvent& event);
        void OnValueTooltip(CodeBlocksEvent& event);
        void DeleteAllProjectBreakpoints(cbProject* project);
        void OnBuildTargetSelected(CodeBlocksEvent& event);
        void OnGDBOutput(wxCommandEvent& event);
        void OnGDBError(wxCommandEvent& event);
        void OnGDBTerminated(wxCommandEvent& event);
        void OnIdle(wxIdleEvent& event);
        void OnTimer(wxTimerEvent& event);
        void OnShowFile(wxCommandEvent& event);
        void OnCursorChanged(wxCommandEvent& event);
        void OnSettings(wxCommandEvent& event);

        void ShowToolMenu();
        void KillConsole();

        void OnInfoFrame(wxCommandEvent& event);
        void OnInfoDLL(wxCommandEvent& event);
        void OnInfoFiles(wxCommandEvent& event);
        void OnInfoFPU(wxCommandEvent& event);
        void OnInfoSignals(wxCommandEvent& event);

    private:
        PipedProcess* m_pProcess;
        int m_PageIndex;
        int m_DbgPageIndex;
        wxRegEx reSource;
        wxString m_LastCmd;
        wxString m_Variable;
        bool m_LastExitCode;
        int m_Pid;
        int m_PidToAttach; // for "attach to process"
        wxString m_LastEval;
        wxRect m_EvalRect;
        wxTimer m_TimerPollDebugger;
        bool m_NoDebugInfo;

        int m_HaltAtLine;
        bool m_HasDebugLog;
        bool m_StoppedOnSignal;

        // extra dialogs
        cbProject* m_pProject; // keep the currently debugged project handy
        wxString m_ActiveBuildTarget;

        // per-project debugger search-dirs
        typedef std::map<cbProject*, wxArrayString> SearchDirsMap;
        SearchDirsMap m_SearchDirs;

        typedef std::map<cbProject*, RemoteDebuggingMap> ProjectRemoteDebuggingMap;
        ProjectRemoteDebuggingMap m_RemoteDebugging;

        int m_HookId; // project loader hook ID

        // Linux console support
        bool     m_bIsConsole;
        int      m_nConsolePid;

        bool m_Canceled; // flag to avoid re-entering DoDebug when we shouldn't

        bool m_TemporaryBreak;

        WatchesContainer m_watches;

        struct BreakItem
        {
            std::tr1::shared_ptr<cbBreakpoint> cb_break;
            DebuggerBreakpoint* debugger_breakpoint;
        };
        typedef std::vector<BreakItem> BreakpointsContainer;
        BreakpointsContainer m_breakpoints;

        friend struct TestIfBelongToProject;

        DECLARE_EVENT_TABLE()
};

#endif // DEBUGGERGDB_H

