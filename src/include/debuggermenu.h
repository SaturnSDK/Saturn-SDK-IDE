/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef X_DEBUGGER_MENU_H
#define X_DEBUGGER_MENU_H

#include <wx/event.h>

class cbDebuggerPlugin;
class wxToolBar;

class DebuggerMenuHandler : public wxEvtHandler
{
    public:
        DebuggerMenuHandler();
        void SetActiveDebugger(cbDebuggerPlugin *active);
        void RebuildActiveDebuggersMenu();
        void BuildContextMenu(wxMenu &menu, const wxString& word_at_caret, bool is_running);
    private:
        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnStart(wxCommandEvent& event);
        void OnBreak(wxCommandEvent& event);
        void OnStop(wxCommandEvent& event);

        void OnContinue(wxCommandEvent& event);
        void OnNext(wxCommandEvent& event);
        void OnNextInstr(wxCommandEvent& event);
        void OnStepIntoInstr(wxCommandEvent& event);
        void OnStep(wxCommandEvent& event);
        void OnStepOut(wxCommandEvent& event);
        void OnRunToCursor(wxCommandEvent& event);
        void OnSetNextStatement(wxCommandEvent& event);

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
        void OnActiveDebuggerTargetsDefaultClick(wxCommandEvent& event);

        DECLARE_EVENT_TABLE();
    private:
        cbDebuggerPlugin* m_activeDebugger;
        wxString m_lastCommand;
        bool m_disableContinue;
};

class DebuggerToolbarHandler : public wxEvtHandler
{
        friend class DebuggerManager;
    public:
        DebuggerToolbarHandler();
        wxToolBar* GetToolbar(bool create = true);
    private:
        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnToolInfo(wxCommandEvent& event);
        void OnDebugWindows(wxCommandEvent& event);
        void OnStop(wxCommandEvent& event);

        wxToolBar *m_Toolbar;

        DECLARE_EVENT_TABLE();
};

#endif // X_DEBUGGER_MENU_H

