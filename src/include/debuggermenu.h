/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef X_DEBUGGER_MENU_H
#define X_DEBUGGER_MENU_H

class cbDebuggerPlugin;

class DebuggerMenuHandler : public wxEvtHandler
{
        friend class DebuggerManager;
    public:
        DebuggerMenuHandler();
        void SetActiveDebugger(cbDebuggerPlugin *active);
        void BuildContextMenu(wxMenu &menu, const wxString& word_at_caret, bool is_running);
    private:
        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnStart(wxCommandEvent& event);
        void OnStop(wxCommandEvent& event);

        void OnContinue(wxCommandEvent& event);
        void OnNext(wxCommandEvent& event);
        void OnNextInstr(wxCommandEvent& event);
        void OnStep(wxCommandEvent& event);
        void OnStepOut(wxCommandEvent& event);
        void OnRunToCursor(wxCommandEvent& event);

        void OnToggleBreakpoint(wxCommandEvent& event);
        void OnRemoveAllBreakpoints(wxCommandEvent& event);
        void OnAddDataBreakpoint(wxCommandEvent& event);
        void OnSendCommand(wxCommandEvent& event);
        void OnAttachToProcess(wxCommandEvent& event);
        void OnDetachFromProcess(wxCommandEvent& event);
        void OnAddWatch(wxCommandEvent& event);

        void OnBacktrace(wxCommandEvent& event);
        void OnBreakpoints(wxCommandEvent& event);
        void OnCPURegisters(wxCommandEvent& event);
        void OnDisassembly(wxCommandEvent& event);
        void OnExamineMemory(wxCommandEvent& event);
        void OnThreads(wxCommandEvent& event);
        void OnWatches(wxCommandEvent& event);

        void OnActiveDebuggerClick(wxCommandEvent& event);

        DECLARE_EVENT_TABLE();
    private:
        cbDebuggerPlugin* m_activeDebugger;
        wxString m_lastCommand;
};

class DebuggerToolbarHandler : public wxEvtHandler
{
        friend class DebuggerManager;
    public:
        DebuggerToolbarHandler();
        void LoadToolbar(wxToolBar *toolbar);
    private:
        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnToolInfo(wxCommandEvent& event);
        void OnDebugWindows(wxCommandEvent& event);

        DECLARE_EVENT_TABLE();
};

#endif // X_DEBUGGER_MENU_H

