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
    const int idMenuNext = XRCID("idDebuggerMenuNext");
    const int idMenuStep = XRCID("idDebuggerMenuStep");
    const int idMenuNextInstr = XRCID("idDebuggerMenuNextInstr");
    const int idMenuStepOut = XRCID("idDebuggerMenuStepOut");
    const int idMenuStop = XRCID("idDebuggerMenuStop");
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
    EVT_UPDATE_UI(XRCID("idDebuggerMenuDebug"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuRunToCursor"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuNext"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuNextInstr"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuStep"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuStepOut"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuStop"), DebuggerMenuHandler::OnUpdateUI)

    EVT_UPDATE_UI(XRCID("idDebuggerCurrentFrame"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerLoadedDLLs"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerFiles"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerFPU"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerSignals"), DebuggerMenuHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerThreads"), DebuggerMenuHandler::OnUpdateUI)

    EVT_UPDATE_UI(XRCID("idDebuggerToolInfo"), DebuggerMenuHandler::OnUpdateUI)

    EVT_MENU(idMenuDebug, DebuggerMenuHandler::OnStart)
    EVT_MENU(idMenuStop, DebuggerMenuHandler::OnStop)
    EVT_MENU(idMenuContinue, DebuggerMenuHandler::OnContinue)
    EVT_MENU(idMenuNext, DebuggerMenuHandler::OnNext)
    EVT_MENU(idMenuStep, DebuggerMenuHandler::OnStep)
    EVT_MENU(idMenuNextInstr, DebuggerMenuHandler::OnNextInstr)
    EVT_MENU(idMenuStepOut, DebuggerMenuHandler::OnStepOut)
    EVT_MENU(idMenuRunToCursor, DebuggerMenuHandler::OnRunToCursor)
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
    // Insert toggle breakpoint
    menu.Insert(0, idMenuToggleBreakpoint, _("Toggle breakpoint"));
    // Insert Run to Cursor
    menu.Insert(1, idMenuRunToCursor, _("Run to cursor"));
    menu.Insert(2, wxID_SEPARATOR, _T("-"));

    if (!is_running && word_at_caret.empty())
        return;

    // data breakpoint
    menu.Insert(2, idMenuAddDataBreakpoint, wxString::Format(_("Add data breakpoint for '%s'"), word_at_caret.c_str()));
    menu.Insert(3, idMenuDebuggerAddWatch, wxString::Format(_("Watch '%s'"), word_at_caret.c_str()));
}

void DebuggerMenuHandler::OnUpdateUI(wxUpdateUIEvent& event)
{
    if(!m_activeDebugger)
        return;

    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    // FIXME (obfuscated#) reimplement m_pidToAttach
    bool en = (prj && !prj->GetCurrentlyCompilingTarget()) /* || m_PidToAttach != 0*/;
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    wxMenuBar* mbar = Manager::Get()->GetAppFrame()->GetMenuBar();
    bool stopped = m_activeDebugger->IsStopped();
    bool isRunning = m_activeDebugger->IsRunning();

    DebuggerManager *dbg_manager = Manager::Get()->GetDebuggerManager();

    if (mbar)
    {
        mbar->Enable(idMenuDebug, !isRunning && en);
        mbar->Enable(idMenuContinue, isRunning && en && stopped);
        mbar->Enable(idMenuNext, isRunning && en && stopped);
        mbar->Enable(idMenuNextInstr, isRunning && en && stopped);
        mbar->Enable(idMenuStep, en && stopped);
        mbar->Enable(idMenuStepOut, isRunning && en && stopped);
        mbar->Enable(idMenuRunToCursor, en && ed && stopped);
        mbar->Enable(idMenuToggleBreakpoint, en && ed);
        mbar->Enable(idMenuRemoveAllBreakpoints, en && ed);
        mbar->Enable(idMenuSendCommand, isRunning && stopped);
        mbar->Enable(idMenuAddSymbolFile, isRunning && stopped);
        mbar->Enable(idMenuStop, isRunning && en);
        mbar->Enable(idMenuAttachToProcess, !isRunning);
        mbar->Enable(idMenuDetach, false); //isRunning && m_PidToAttach != 0);

//        mbar->Enable(idMenuInfoFrame, isRunning && stopped);
//        mbar->Enable(idMenuInfoDLL, isRunning && stopped);
//        mbar->Enable(idMenuInfoFiles, isRunning && stopped);
//        mbar->Enable(idMenuInfoFPU, isRunning && stopped);
//        mbar->Enable(idMenuInfoSignals, isRunning && stopped);
//
//        mbar->Check(idMenuThreads, IsWindowReallyShown(m_pThreadsDlg));
//        mbar->Check(idMenuMemory, IsWindowReallyShown(m_pExamineMemoryDlg));
//        mbar->Check(idMenuBacktrace, IsWindowReallyShown(m_pBacktrace));
//        mbar->Check(idMenuCPU, IsWindowReallyShown(m_pDisassembly));
//        mbar->Check(idMenuWatches, IsWindowReallyShown(m_pTree));
//        mbar->Check(idMenuRegisters, IsWindowReallyShown(m_pCPURegisters));
        mbar->Check(idMenuBreakpoints, IsWindowReallyShown(dbg_manager->GetBreakpointDialog()));
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
        m_activeDebugger->Debug(false);
        m_disableContinue = false;
    }
    else if (m_activeDebugger->IsStopped() && !m_disableContinue)
        m_activeDebugger->Continue();
}
void DebuggerMenuHandler::OnStop(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    if(m_activeDebugger->IsStopped())
        m_activeDebugger->Stop();
    else
        m_activeDebugger->Break();
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
        m_activeDebugger->Debug(true);
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
    m_activeDebugger->RunToCursor(ed->GetFilename(), ed->GetControl()->GetCurrentLine() + 1, line_text);
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

    m_activeDebugger->SendCommand(cmd);
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
//    EVT_UPDATE_UI_RANGE(idMenuContinue, idMenuDebuggerAddWatch, DebuggerMenuHandler::OnUpdateUI)
    // these are different because they are loaded from the XRC
    EVT_UPDATE_UI(XRCID("idDebuggerMenuDebug"), DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuRunToCursor"), DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuNext"), DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuNextInstr"), DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuStep"), DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuStepOut"), DebuggerToolbarHandler::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("idDebuggerMenuStop"), DebuggerToolbarHandler::OnUpdateUI)

    EVT_MENU(idDebuggerToolInfo, DebuggerToolbarHandler::OnToolInfo)
    EVT_MENU(idDebuggerToolWindows, DebuggerToolbarHandler::OnDebugWindows)

END_EVENT_TABLE()

DebuggerToolbarHandler::DebuggerToolbarHandler()
{
}

void DebuggerToolbarHandler::LoadToolbar(wxToolBar *toolbar)
{
//    if(m_toolbar)
//        return;
//    m_toolbar = Manager::Get()->CreateEmptyToolbar();
    wxString my_16x16 = Manager::isToolBar16x16(toolbar) ? _T("_16x16") : _T("");
    Manager::AddonToolBar(toolbar, wxString(_T("debugger_toolbar")) + my_16x16);
//    m_toolbar->Realize();
    toolbar->SetInitialSize();
//    m_toolbar->Show();
//
//    wxAuiManager* layout = Manager::Get()->GetAppMainFrame()->GetLayoutManager();
////    layout->AddPane(m_toolbar, wxAuiPaneInfo().
////                              Name(_T("Debugger Toolbar")).Caption(_("Debugger Toolbar")).
////                              ToolbarPane().Top().Row(1));
//    layout->AddPane(m_toolbar, wxAuiPaneInfo().Name(_T("dbg_Toolbar")).Caption(_T("dbg_toolbar")).Floatable().Float());
//    Manager::Get()->GetAppMainFrame()->DoUpdateLayout();
//    Manager::ProcessPendingEvents();
}


// FIXME (obfuscated#) this code should be removed when we can have one toolbar for more than one plugin
void EnableToolbar(DebuggerManager::RegisteredPlugins::reference pair)
{
    cbDebuggerPlugin *plugin = pair.first;
    wxToolBar *toolbar = plugin->GetToolbar();

    if(plugin == Manager::Get()->GetDebuggerManager()->GetActiveDebugger())
    {
        cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
        // FIXME (obfuscated#) reimplement m_pidToAttach
        bool en = (prj && !prj->GetCurrentlyCompilingTarget()) /* || m_PidToAttach != 0*/;
        cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
        bool stopped = plugin->IsStopped();
        bool isRunning = plugin->IsRunning();

        toolbar->EnableTool(idMenuDebug, (!isRunning || stopped) && en);
        toolbar->EnableTool(idMenuRunToCursor, en && ed && stopped);
        toolbar->EnableTool(idMenuNext, isRunning && en && stopped);
        toolbar->EnableTool(idMenuNextInstr, isRunning && en && stopped);
        toolbar->EnableTool(idMenuStep, en && stopped);
        toolbar->EnableTool(idMenuStepOut, isRunning && en && stopped);
        toolbar->EnableTool(idMenuStop, isRunning && en);
        toolbar->EnableTool(idDebuggerToolInfo, isRunning && en);
    }
    else
    {
        toolbar->EnableTool(idMenuDebug, false);
        toolbar->EnableTool(idMenuRunToCursor, false);
        toolbar->EnableTool(idMenuNext, false);
        toolbar->EnableTool(idMenuNextInstr, false);
        toolbar->EnableTool(idMenuStep, false);
        toolbar->EnableTool(idMenuStepOut, false);
        toolbar->EnableTool(idMenuStop, false);
        toolbar->EnableTool(idDebuggerToolInfo, false);
    }
}

void DebuggerToolbarHandler::OnUpdateUI(wxUpdateUIEvent& event)
{
    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    if(!plugin)
        return;

    DebuggerManager::RegisteredPlugins &plugins = Manager::Get()->GetDebuggerManager()->GetAllDebuggers();
    std::for_each(plugins.begin(), plugins.end(), EnableToolbar);

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


