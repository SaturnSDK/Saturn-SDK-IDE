/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */
#include "sdk_precomp.h"
#ifndef CB_PRECOMP
    #include <wx/xrc/xmlres.h>

    #include "cbeditor.h"
    #include "cbproject.h"
    #include "editormanager.h"
    #include "projectmanager.h"
#endif

#include "debuggermenu.h"

#include <algorithm>
#include <wx/aui/aui.h> // wxAuiManager

#include "backtracedlg.h"
#include "breakpointsdlg.h"
#include "cbstyledtextctrl.h"
#include "cpuregistersdlg.h"
#include "disassemblydlg.h"
#include "examinememorydlg.h"
#include "threadsdlg.h"
#include "watchesdlg.h"

namespace
{
    const int idMenuDebug = XRCID("idDebuggerMenuDebug");
    const int idMenuRunToCursor = XRCID("idDebuggerMenuRunToCursor");
    const int idMenuSetNextStatement = XRCID("idDebuggerMenuSetNextStatement");
    const int idMenuNext = XRCID("idDebuggerMenuNext");
    const int idMenuStep = XRCID("idDebuggerMenuStep");
    const int idMenuNextInstr = XRCID("idDebuggerMenuNextInstr");
    const int idMenuStepIntoInstr = XRCID("idDebuggerMenuStepIntoInstr");
    const int idMenuStepOut = XRCID("idDebuggerMenuStepOut");
    const int idMenuBreak = XRCID("idDebuggerMenuBreak");
    const int idMenuStop = XRCID("idDebuggerMenuStop");
    const int idToolbarStop = XRCID("idDebuggerToolbarStop");
    const int idMenuContinue = XRCID("idDebuggerMenuContinue");
    const int idMenuToggleBreakpoint = XRCID("idDebuggerMenuToggleBreakpoint");
    const int idMenuRemoveAllBreakpoints = XRCID("idDebuggerMenuRemoveAllBreakpoints");
    const int idMenuAddDataBreakpoint = XRCID("idMenuAddDataBreakpoint");
    const int idMenuSendCommand = XRCID("idDebuggerMenuSendCommand");
    const int idMenuAddSymbolFile = XRCID("idDebuggerMenuAddSymbolFile");
    const int idMenuCPU = XRCID("idDebuggerMenuCPU");
    const int idMenuRegisters = XRCID("idDebuggerMenuRegisters");
    const int idMenuWatches = XRCID("idDebuggerMenuWatches");
    const int idMenuBacktrace = XRCID("idDebuggerMenuBacktrace");
    const int idMenuThreads = XRCID("idDebuggerMenuThreads");
    const int idMenuMemory = XRCID("idDebuggerMenuMemory");
    const int idMenuBreakpoints = XRCID("idDebuggerMenuBreakpoints");
    const int idMenuEditWatches = XRCID("idDebuggerMenuEditWatches");
    const int idMenuAttachToProcess = XRCID("idDebuggerMenuAttachToProcess");
    const int idMenuDetach = XRCID("idDebuggerMenuDetach");

    const int idDebuggerToolInfo = XRCID("idDebuggerToolInfo");
    const int idDebuggerToolWindows = XRCID("idDebuggerToolWindows");

    const int idMenuDebuggerAddWatch = wxNewId();
}

BEGIN_EVENT_TABLE(DebuggerMenuHandler, wxEvtHandler)
//    EVT_UPDATE_UI_RANGE(idMenuContinue, idMenuDebuggerAddWatch, DebuggerMenuHandler::OnUpdateUI)
    // these are different because they are loaded from the XRC
    EVT_UPDATE_UI(idMenuDebug, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuRunToCursor, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuSetNextStatement, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuNext, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuNextInstr, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuStepIntoInstr, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuStep, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuStepOut, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuBreak, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuStop, DebuggerMenuHandler::OnUpdateUI)

    EVT_UPDATE_UI(idMenuAttachToProcess, DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuDetach, DebuggerMenuHandler::OnUpdateUI)

    EVT_UPDATE_UI(XRCID("idDebuggerCurrentFrame"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerLoadedDLLs"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerFiles"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerFPU"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerSignals"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerThreads"), DebuggerMenuHandler::OnUpdateUI)

    EVT_UPDATE_UI(XRCID("idDebuggerToolInfo"), DebuggerMenuHandler::OnUpdateUI)

    EVT_MENU(idMenuDebug, DebuggerMenuHandler::OnStart)
    EVT_MENU(idMenuBreak, DebuggerMenuHandler::OnBreak)
    EVT_MENU(idMenuStop, DebuggerMenuHandler::OnStop)
    EVT_MENU(idMenuContinue, DebuggerMenuHandler::OnContinue)
    EVT_MENU(idMenuNext, DebuggerMenuHandler::OnNext)
    EVT_MENU(idMenuStep, DebuggerMenuHandler::OnStep)
    EVT_MENU(idMenuNextInstr, DebuggerMenuHandler::OnNextInstr)
    EVT_MENU(idMenuStepIntoInstr, DebuggerMenuHandler::OnStepIntoInstr)
    EVT_MENU(idMenuStepOut, DebuggerMenuHandler::OnStepOut)
    EVT_MENU(idMenuRunToCursor, DebuggerMenuHandler::OnRunToCursor)
    EVT_MENU(idMenuSetNextStatement, DebuggerMenuHandler::OnSetNextStatement)
    EVT_MENU(idMenuToggleBreakpoint, DebuggerMenuHandler::OnToggleBreakpoint)
    EVT_MENU(idMenuRemoveAllBreakpoints, DebuggerMenuHandler::OnRemoveAllBreakpoints)
    EVT_MENU(idMenuAddDataBreakpoint, DebuggerMenuHandler::OnAddDataBreakpoint)
    EVT_MENU(idMenuSendCommand, DebuggerMenuHandler::OnSendCommand)
//    EVT_MENU(idMenuAddSymbolFile, DebuggerMenuHandler::OnAddSymbolFile)
    EVT_MENU(idMenuBacktrace, DebuggerMenuHandler::OnBacktrace)
    EVT_MENU(idMenuThreads, DebuggerMenuHandler::OnThreads)
    EVT_MENU(idMenuMemory, DebuggerMenuHandler::OnExamineMemory)
    EVT_MENU(idMenuCPU, DebuggerMenuHandler::OnDisassembly)
    EVT_MENU(idMenuRegisters, DebuggerMenuHandler::OnCPURegisters)
    EVT_MENU(idMenuWatches, DebuggerMenuHandler::OnWatches)
    EVT_MENU(idMenuBreakpoints, DebuggerMenuHandler::OnBreakpoints)
//    EVT_MENU(idMenuEditWatches, DebuggerMenuHandler::OnEditWatches)
    EVT_MENU(idMenuDebuggerAddWatch, DebuggerMenuHandler::OnAddWatch)
    EVT_MENU(idMenuAttachToProcess, DebuggerMenuHandler::OnAttachToProcess)
    EVT_MENU(idMenuDetach, DebuggerMenuHandler::OnDetachFromProcess)
//    EVT_MENU(idMenuSettings, DebuggerMenuHandler::OnSettings)
END_EVENT_TABLE()


DebuggerMenuHandler::DebuggerMenuHandler() :
    m_activeDebugger(NULL),
    m_disableContinue(false)
{
}

void DebuggerMenuHandler::SetActiveDebugger(cbDebuggerPlugin *active)
{
    m_activeDebugger = active;
}

void DebuggerMenuHandler::BuildContextMenu(wxMenu &menu, const wxString& word_at_caret, bool is_running)
{
    int item = 0;
    // Insert toggle breakpoint
    menu.Insert(item++, idMenuToggleBreakpoint, _("Toggle breakpoint"));
    // Insert Run to Cursor
    menu.Insert(item++, idMenuRunToCursor, _("Run to cursor"));
    if (is_running)
        menu.Insert(item++, idMenuSetNextStatement, _("Set next statement"));
    menu.Insert(item++, wxID_SEPARATOR, _T("-"));

    if (!is_running && word_at_caret.empty())
        return;

    // data breakpoint
    menu.Insert(item++, idMenuAddDataBreakpoint,
                wxString::Format(_("Add data breakpoint for '%s'"), word_at_caret.c_str()));
    menu.Insert(item++, idMenuDebuggerAddWatch, wxString::Format(_("Watch '%s'"), word_at_caret.c_str()));
}

void DebuggerMenuHandler::OnUpdateUI(wxUpdateUIEvent& event)
{
    if(!m_activeDebugger)
        return;

    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    bool en = (prj && !prj->GetCurrentlyCompilingTarget()) || m_activeDebugger->IsAttachedToProcess();
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    wxMenuBar* mbar = Manager::Get()->GetAppFrame()->GetMenuBar();
    bool stopped = m_activeDebugger->IsStopped();
    bool isRunning = m_activeDebugger->IsRunning();

    DebuggerManager *dbg_manager = Manager::Get()->GetDebuggerManager();
    cbPlugin *runningPlugin = Manager::Get()->GetProjectManager()->GetIsRunning();

    bool otherPlugin = false;
    if (runningPlugin != NULL && runningPlugin != m_activeDebugger)
    {
        en = false;
        otherPlugin = true;
    }

    if (mbar)
    {
        mbar->Enable(idMenuDebug, (!isRunning || stopped) && en);
        mbar->Enable(idMenuContinue, isRunning && en && stopped);
        mbar->Enable(idMenuNext, isRunning && en && stopped);
        mbar->Enable(idMenuNextInstr, isRunning && en && stopped);
        mbar->Enable(idMenuStepIntoInstr, isRunning && en && stopped);
        mbar->Enable(idMenuStep, en && stopped);
        mbar->Enable(idMenuStepOut, isRunning && en && stopped);
        mbar->Enable(idMenuRunToCursor, en && ed && stopped);
        mbar->Enable(idMenuSetNextStatement, en && ed && stopped && isRunning);
        mbar->Enable(idMenuToggleBreakpoint, en && ed);
        mbar->Enable(idMenuRemoveAllBreakpoints, en && ed);
        mbar->Enable(idMenuSendCommand, isRunning && stopped);
        mbar->Enable(idMenuAddSymbolFile, isRunning && stopped);
        mbar->Enable(idMenuStop, isRunning && en);
        mbar->Enable(idMenuBreak, isRunning && !stopped && en);
        mbar->Enable(idMenuAttachToProcess, !isRunning && !otherPlugin);
        mbar->Enable(idMenuDetach, isRunning && stopped && m_activeDebugger->IsAttachedToProcess());

//        mbar->Enable(idMenuInfoFrame, isRunning && stopped);
//        mbar->Enable(idMenuInfoDLL, isRunning && stopped);
//        mbar->Enable(idMenuInfoFiles, isRunning && stopped);
//        mbar->Enable(idMenuInfoFPU, isRunning && stopped);
//        mbar->Enable(idMenuInfoSignals, isRunning && stopped);

        mbar->Check(idMenuBreakpoints,  IsWindowReallyShown(dbg_manager->GetBreakpointDialog()));
        mbar->Check(idMenuBacktrace,    IsWindowReallyShown(dbg_manager->GetBacktraceDialog()));
        mbar->Check(idMenuRegisters,    IsWindowReallyShown(dbg_manager->GetCPURegistersDialog()));
        mbar->Check(idMenuCPU,          IsWindowReallyShown(dbg_manager->GetDisassemblyDialog()));
        mbar->Check(idMenuMemory,       IsWindowReallyShown(dbg_manager->GetExamineMemoryDialog()));
        mbar->Check(idMenuThreads,      IsWindowReallyShown(dbg_manager->GetThreadsDialog()));
        mbar->Check(idMenuWatches,      IsWindowReallyShown(dbg_manager->GetWatchesDialog()));
    }

    // allow other UpdateUI handlers to process this event
    // *very* important! don't forget it...
    event.Skip();
}

void DebuggerMenuHandler::OnStart(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    if (!m_activeDebugger->IsRunning())
    {
        m_disableContinue = true;

        ProjectManager *manager = Manager::Get()->GetProjectManager();
        if (manager->GetIsRunning() == NULL)
        {
            manager->SetIsRunning(m_activeDebugger);

            if (!m_activeDebugger->Debug(false))
                manager->SetIsRunning(NULL);
        }
        m_disableContinue = false;
    }
    else if (m_activeDebugger->IsStopped() && !m_disableContinue)
        m_activeDebugger->Continue();
}

void DebuggerMenuHandler::OnBreak(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    m_activeDebugger->Break();
}

void DebuggerMenuHandler::OnStop(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    m_activeDebugger->Stop();
}

void DebuggerMenuHandler::OnContinue(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    if(!m_disableContinue)
        m_activeDebugger->Continue();
}

void DebuggerMenuHandler::OnNext(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    m_activeDebugger->Next();
}

void DebuggerMenuHandler::OnNextInstr(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    m_activeDebugger->NextInstruction();
}

void DebuggerMenuHandler::OnStepIntoInstr(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    m_activeDebugger->StepIntoInstruction();
}

void DebuggerMenuHandler::OnStep(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    if (m_activeDebugger->IsRunning())
    {
        if(!m_disableContinue)
            m_activeDebugger->Step();
    }
    else
    {
        m_disableContinue = true;
        ProjectManager *manager = Manager::Get()->GetProjectManager();
        if (manager->GetIsRunning() == NULL)
        {
            manager->SetIsRunning(m_activeDebugger);
            if (!m_activeDebugger->Debug(true))
                manager->SetIsRunning(NULL);
        }
        m_disableContinue = false;
    }
}

void DebuggerMenuHandler::OnStepOut(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    m_activeDebugger->StepOut();
}

void DebuggerMenuHandler::OnRunToCursor(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return;
    const wxString &line_text = ed->GetControl()->GetLine(ed->GetControl()->GetCurrentLine());

    ProjectManager *manager = Manager::Get()->GetProjectManager();
    if (manager->GetIsRunning() == NULL || manager->GetIsRunning() == m_activeDebugger)
    {
        manager->SetIsRunning(m_activeDebugger);
        if (!m_activeDebugger->RunToCursor(ed->GetFilename(), ed->GetControl()->GetCurrentLine() + 1, line_text))
            manager->SetIsRunning(NULL);
    }
}

void DebuggerMenuHandler::OnSetNextStatement(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return;
    m_activeDebugger->SetNextStatement(ed->GetFilename(), ed->GetControl()->GetCurrentLine() + 1);
}

void DebuggerMenuHandler::OnToggleBreakpoint(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return;
    ed->ToggleBreakpoint();
}

void DebuggerMenuHandler::OnRemoveAllBreakpoints(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);

    int count = m_activeDebugger->GetBreakpointsCount();
    for (int i = 0; i < count; ++i)
    {
        cbBreakpoint const &breakpoint = *m_activeDebugger->GetBreakpoint(i);

        cbEditor* editor = Manager::Get()->GetEditorManager()->GetBuiltinEditor(breakpoint.GetFilename());
        if (editor)
            editor->RemoveBreakpoint(breakpoint.GetLine() - 1, false);
    }

    m_activeDebugger->DeleteAllBreakpoints();
    Manager::Get()->GetDebuggerManager()->GetBreakpointDialog()->Reload();
}

void DebuggerMenuHandler::OnAddDataBreakpoint(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    const wxString& word_at_caret = m_activeDebugger->GetEditorWordAtCaret();
    if (!word_at_caret.empty())
    {
        if (m_activeDebugger->AddDataBreakpoint(word_at_caret))
            Manager::Get()->GetDebuggerManager()->GetBreakpointDialog()->Reload();
    }
}

void DebuggerMenuHandler::OnAttachToProcess(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    wxString pidStr = wxGetTextFromUser(_("PID to attach to:"));
    if (!pidStr.empty())
    {
        m_activeDebugger->AttachToProcess(pidStr);
    }
}

void DebuggerMenuHandler::OnDetachFromProcess(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    m_activeDebugger->DetachFromProcess();
}

void DebuggerMenuHandler::OnSendCommand(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    wxString cmd = wxGetTextFromUser(_("Enter command for Debugger:"), _("Send command to Debugger:"), m_lastCommand);
    if (cmd.IsEmpty())
        return;

    m_activeDebugger->SendCommand(cmd, false);
    m_lastCommand = cmd;
}

void DebuggerMenuHandler::OnAddWatch(wxCommandEvent& event)
{
    if (!m_activeDebugger)
        return;

    wxString const &src = m_activeDebugger->GetEditorWordAtCaret();
    if (!src.empty())
    {
        cbWatch *watch = m_activeDebugger->AddWatch(src);
        if (watch)
            Manager::Get()->GetDebuggerManager()->GetWatchesDialog()->AddWatch(watch);
    }
}

void DebuggerMenuHandler::OnBacktrace(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetBacktraceDialog();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::Backtrace);
}

void DebuggerMenuHandler::OnBreakpoints(wxCommandEvent& event)
{
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = Manager::Get()->GetDebuggerManager()->GetBreakpointDialog();
    Manager::Get()->ProcessEvent(evt);
}

void DebuggerMenuHandler::OnCPURegisters(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetCPURegistersDialog();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::CPURegisters);
}

void DebuggerMenuHandler::OnDisassembly(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetDisassemblyDialog();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::Disassembly);
}

void DebuggerMenuHandler::OnExamineMemory(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetExamineMemoryDialog();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::ExamineMemory);
}

void DebuggerMenuHandler::OnThreads(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetThreadsDialog();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::Threads);
}

void DebuggerMenuHandler::OnWatches(wxCommandEvent& event)
{
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = Manager::Get()->GetDebuggerManager()->GetWatchesDialog();
    Manager::Get()->ProcessEvent(evt);
}

void DebuggerMenuHandler::OnActiveDebuggerClick(wxCommandEvent& event)
{
    DebuggerManager::RegisteredPlugins &plugins = Manager::Get()->GetDebuggerManager()->GetAllDebuggers();

    for(DebuggerManager::RegisteredPlugins::iterator it = plugins.begin(); it != plugins.end(); ++it)
    {
        if(it->second.menu_id == event.GetId())
        {
            Manager::Get()->GetDebuggerManager()->SetActiveDebugger(it->first);
            break;
        }
    }
}


BEGIN_EVENT_TABLE(DebuggerToolbarHandler, wxEvtHandler)
    // these are different because they are loaded from the XRC
    EVT_UPDATE_UI(idMenuDebug, DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuRunToCursor, DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuNext, DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuNextInstr, DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuStepIntoInstr, DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuStep, DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuStepOut, DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(idMenuBreak, DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(idToolbarStop, DebuggerToolbarHandler::OnUpdateUI)

    EVT_MENU(idDebuggerToolInfo, DebuggerToolbarHandler::OnToolInfo)
    EVT_MENU(idDebuggerToolWindows, DebuggerToolbarHandler::OnDebugWindows)
    EVT_MENU(idToolbarStop, DebuggerToolbarHandler::OnStop)
END_EVENT_TABLE()

DebuggerToolbarHandler::DebuggerToolbarHandler() : m_Toolbar(NULL)
{
}

wxToolBar* DebuggerToolbarHandler::GetToolbar(bool create)
{
    if (!m_Toolbar)
    {
        if (!create)
            return NULL;

        m_Toolbar = Manager::Get()->CreateEmptyToolbar();
        wxString my_16x16 = Manager::isToolBar16x16(m_Toolbar) ? _T("_16x16") : _T("");
        Manager::AddonToolBar(m_Toolbar, wxString(_T("debugger_toolbar")) + my_16x16);

        m_Toolbar->Realize();
        m_Toolbar->SetInitialSize();
    }
    return m_Toolbar;
}

void DebuggerToolbarHandler::OnUpdateUI(wxUpdateUIEvent& event)
{
    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    if (!plugin)
        return;

    ProjectManager *manager = Manager::Get()->GetProjectManager();

    cbProject* prj = manager->GetActiveProject();
    bool en = (prj && !prj->GetCurrentlyCompilingTarget()) || plugin->IsAttachedToProcess();
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    bool stopped = plugin->IsStopped();
    bool isRunning = plugin->IsRunning();

    cbPlugin *runningPlugin = manager->GetIsRunning();
    if (runningPlugin != NULL && runningPlugin != plugin)
        en = false;

    m_Toolbar->EnableTool(idMenuDebug, (!isRunning || stopped) && en);
    m_Toolbar->EnableTool(idMenuRunToCursor, en && ed && stopped);
    m_Toolbar->EnableTool(idMenuNext, isRunning && en && stopped);
    m_Toolbar->EnableTool(idMenuNextInstr, isRunning && en && stopped);
    m_Toolbar->EnableTool(idMenuStepIntoInstr, isRunning && en && stopped);
    m_Toolbar->EnableTool(idMenuStep, en && stopped);
    m_Toolbar->EnableTool(idMenuStepOut, isRunning && en && stopped);
    m_Toolbar->EnableTool(idToolbarStop, isRunning && en);
    m_Toolbar->EnableTool(idMenuBreak, isRunning && !stopped && en);
    m_Toolbar->EnableTool(idDebuggerToolInfo, plugin->ToolMenuEnabled());

    // allow other UpdateUI handlers to process this event
    // *very* important! don't forget it...
    event.Skip();
}

void DebuggerToolbarHandler::OnToolInfo(wxCommandEvent& event)
{
    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    if (plugin)
        plugin->ShowToolMenu();
}

void DebuggerToolbarHandler::OnDebugWindows(wxCommandEvent& event)
{
    wxMenu m;

    m.AppendCheckItem(idMenuBreakpoints,    _("Breakpoints"));
    m.AppendCheckItem(idMenuBacktrace,      _("Call stack"));
    m.AppendCheckItem(idMenuRegisters,      _("CPU Registers"));
    m.AppendCheckItem(idMenuCPU,            _("Disassembly"));
    m.AppendCheckItem(idMenuMemory,         _("Memory dump"));
    m.AppendCheckItem(idMenuThreads,        _("Running threads"));
    m.AppendCheckItem(idMenuWatches,        _("Watches"));


    DebuggerManager *dbg_manager = Manager::Get()->GetDebuggerManager();

    m.Check(idMenuBreakpoints,  IsWindowReallyShown(dbg_manager->GetBreakpointDialog()));
    m.Check(idMenuBacktrace,    IsWindowReallyShown(dbg_manager->GetBacktraceDialog()));
    m.Check(idMenuRegisters,    IsWindowReallyShown(dbg_manager->GetCPURegistersDialog()));
    m.Check(idMenuCPU,          IsWindowReallyShown(dbg_manager->GetDisassemblyDialog()));
    m.Check(idMenuMemory,       IsWindowReallyShown(dbg_manager->GetExamineMemoryDialog()));
    m.Check(idMenuThreads,      IsWindowReallyShown(dbg_manager->GetThreadsDialog()));
    m.Check(idMenuWatches,      IsWindowReallyShown(dbg_manager->GetWatchesDialog()));

    Manager::Get()->GetAppWindow()->PopupMenu(&m);
}

void DebuggerToolbarHandler::OnStop(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    cbDebuggerPlugin *plugin = manager->GetActiveDebugger();
    if (!plugin)
        return;

    if (plugin->IsAttachedToProcess())
    {
        wxMenu m;

        if (plugin->IsStopped())
            m.Append(idMenuDetach, _("Detach"));
        else
        {
            wxMenuItem *detach_item = m.Append(idMenuDetach, _("Detach (debugger is running)"));
            detach_item->Enable(false);
        }

        m.Append(idMenuStop, _("Stop debugger (kills the debuggee)"));

        Manager::Get()->GetAppWindow()->PopupMenu(&m);
    }
    else
    {
        wxCommandEvent event(wxEVT_COMMAND_TOOL_CLICKED, idMenuStop);
        m_Toolbar->ProcessEvent(event);
    }
}
