/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */
#include "sdk.h"

#ifndef CB_PRECOMP
    #include <wx/menu.h>
    #include <wx/textdlg.h>
    #include <wx/toolbar.h>
    #include <wx/xrc/xmlres.h>

    #include "cbeditor.h"
    #include "cbproject.h"
    #include "editormanager.h"
    #include "logmanager.h"
    #include "projectmanager.h"
#endif

#include "debuggermenu.h"

#include <algorithm>
#include <wx/aui/aui.h> // wxAuiManager

#include "cbdebugger_interfaces.h"
#include "cbstyledtextctrl.h"
#include "debuggermanager.h"

namespace
{
    const int idMenuDebug = XRCID("idDebuggerMenuDebug");
    const int idMenuDebugActive = XRCID("idDebuggerMenuActive");
    const int idMenuDebugActiveTargetsDefault = wxNewId();
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
    const int idMenuAttachToProcess = XRCID("idDebuggerMenuAttachToProcess");
    const int idMenuDetach = XRCID("idDebuggerMenuDetach");

    const int idDebuggerToolInfo = XRCID("idDebuggerToolInfo");
    const int idDebuggerToolWindows = XRCID("idDebuggerToolWindows");

    const int idMenuDebuggerAddWatch = wxNewId();

    inline void HideValueTooltip() { Manager::Get()->GetDebuggerManager()->GetInterfaceFactory()->HideValueTooltip(); }
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
    EVT_MENU(idMenuBacktrace, DebuggerMenuHandler::OnBacktrace)
    EVT_MENU(idMenuThreads, DebuggerMenuHandler::OnThreads)
    EVT_MENU(idMenuMemory, DebuggerMenuHandler::OnExamineMemory)
    EVT_MENU(idMenuCPU, DebuggerMenuHandler::OnDisassembly)
    EVT_MENU(idMenuRegisters, DebuggerMenuHandler::OnCPURegisters)
    EVT_MENU(idMenuWatches, DebuggerMenuHandler::OnWatches)
    EVT_MENU(idMenuBreakpoints, DebuggerMenuHandler::OnBreakpoints)
    EVT_MENU(idMenuDebuggerAddWatch, DebuggerMenuHandler::OnAddWatch)
    EVT_MENU(idMenuAttachToProcess, DebuggerMenuHandler::OnAttachToProcess)
    EVT_MENU(idMenuDetach, DebuggerMenuHandler::OnDetachFromProcess)
    EVT_MENU(idMenuDebugActiveTargetsDefault, DebuggerMenuHandler::OnActiveDebuggerTargetsDefaultClick)
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

wxMenu* GetActiveDebuggersMenu(bool recreate = false)
{
    wxMenuBar* mbar = Manager::Get()->GetAppFrame()->GetMenuBar();
    wxMenuItem *item = mbar->FindItem(idMenuDebugActive);
    if (!item)
        return nullptr;
    if (recreate)
    {
        wxMenu *debugMenu = item->GetMenu();
        wxString label = item->GetLabel();
        debugMenu->Destroy(item);
        item = debugMenu->Insert(0, idMenuDebugActive, label, new wxMenu);
    }
    return item ? item->GetSubMenu() : nullptr;
}

void DebuggerMenuHandler::MarkActiveTargetAsValid(bool valid)
{
    wxMenu *menu = GetActiveDebuggersMenu();
    if (!menu)
        return;
    wxMenuItem *item = menu->FindItem(idMenuDebugActiveTargetsDefault);
    if (item)
        item->SetText(valid ? _("Target's default") : _("Target's default (invalid)"));
}

void DebuggerMenuHandler::RebuildActiveDebuggersMenu()
{
    DebuggerManager *dbgManager = Manager::Get()->GetDebuggerManager();
    wxMenu *menu = GetActiveDebuggersMenu(true);
    if (!menu)
        return;

    menu->AppendRadioItem(idMenuDebugActiveTargetsDefault, _("Target's default"),
                          _("Use the debugger associated with the compiler for the active target"));

    DebuggerManager::RegisteredPlugins &allDebugger = dbgManager->GetAllDebuggers();
    for (DebuggerManager::RegisteredPlugins::iterator it = allDebugger.begin(); it != allDebugger.end(); ++it)
    {
        DebuggerManager::ConfigurationVector &configs = it->second.GetConfigurations();
        for (DebuggerManager::ConfigurationVector::iterator itConf = configs.begin(); itConf != configs.end(); ++itConf)
        {
            long id = (*itConf)->GetMenuId();
            if (id == wxID_ANY)
            {
                id = wxNewId();
                (*itConf)->SetMenuId(id);
            }

            menu->AppendRadioItem(id, it->second.GetGUIName() + wxT(": ") + (*itConf)->GetName());
            Connect(id, -1, wxEVT_COMMAND_MENU_SELECTED,
                    (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
                    &DebuggerMenuHandler::OnActiveDebuggerClick);
        }
    }

    if (m_activeDebugger && !dbgManager->IsActiveDebuggerTargetsDefault())
    {
        DebuggerManager::RegisteredPlugins::const_iterator it = allDebugger.find(m_activeDebugger);
        cbAssert(it != allDebugger.end());

        const DebuggerManager::ConfigurationVector &configs = it->second.GetConfigurations();

        DebuggerManager::ConfigurationVector::const_iterator itConf = configs.begin();
        std::advance(itConf, m_activeDebugger->GetIndexOfActiveConfig());

        if (itConf != configs.end())
            menu->Check((*itConf)->GetMenuId(), true);
        else
            menu->Check(configs.front()->GetMenuId(), true);
    }
    else
        menu->Check(idMenuDebugActiveTargetsDefault, true);
}

void DebuggerMenuHandler::BuildContextMenu(wxMenu &menu, const wxString& word_at_caret, bool is_running)
{
    int item = 0;
    // Insert Run to Cursor
    menu.Insert(item++, idMenuRunToCursor, _("Run to cursor"));
    if (is_running)
    {
        menu.Insert(item++, idMenuSetNextStatement, _("Set next statement"));
        menu.InsertSeparator(item++);
        if (!word_at_caret.empty())
        {
            menu.Insert(item++, idMenuDebuggerAddWatch, wxString::Format(_("Watch '%s'"), word_at_caret.c_str()));
            // data breakpoint
            menu.Insert(item++, idMenuAddDataBreakpoint,
                        wxString::Format(_("Add data breakpoint for '%s'"), word_at_caret.c_str()));
        }
    }
    // Insert toggle breakpoint
    menu.Insert(item++, idMenuToggleBreakpoint, _("Toggle breakpoint"));
    menu.InsertSeparator(item++);
}

void DebuggerMenuHandler::OnUpdateUI(wxUpdateUIEvent& event)
{
    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    bool en = false, stopped = false, isRunning = false, isAttached = false;

    if (m_activeDebugger)
    {
        isAttached = m_activeDebugger->IsAttachedToProcess();
        en = (prj && !prj->GetCurrentlyCompilingTarget()) || isAttached;
        stopped = m_activeDebugger->IsStopped();
        isRunning = m_activeDebugger->IsRunning();
    }

    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    wxMenuBar* mbar = Manager::Get()->GetAppFrame()->GetMenuBar();

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
        mbar->Enable(idMenuToggleBreakpoint, ed && m_activeDebugger);
        mbar->Enable(idMenuRemoveAllBreakpoints, m_activeDebugger);
        mbar->Enable(idMenuSendCommand, isRunning && stopped);
        mbar->Enable(idMenuAddSymbolFile, isRunning && stopped);
        mbar->Enable(idMenuStop, isRunning && en);
        mbar->Enable(idMenuBreak, isRunning && !stopped && en);
        mbar->Enable(idMenuAttachToProcess, !isRunning && !otherPlugin && m_activeDebugger);
        mbar->Enable(idMenuDetach, isRunning && stopped && isAttached);

        wxMenu *activeMenu = GetActiveDebuggersMenu();
        if (activeMenu)
        {
            for (size_t ii = 0; ii < activeMenu->GetMenuItemCount(); ++ii)
                activeMenu->Enable(activeMenu->FindItemByPosition(ii)->GetId(), !isRunning);
        }

        mbar->Check(idMenuBreakpoints,  IsWindowReallyShown(dbg_manager->GetBreakpointDialog()->GetWindow()));
        mbar->Check(idMenuBacktrace,    IsWindowReallyShown(dbg_manager->GetBacktraceDialog()->GetWindow()));
        mbar->Check(idMenuRegisters,    IsWindowReallyShown(dbg_manager->GetCPURegistersDialog()->GetWindow()));
        mbar->Check(idMenuCPU,          IsWindowReallyShown(dbg_manager->GetDisassemblyDialog()->GetWindow()));
        mbar->Check(idMenuMemory,       IsWindowReallyShown(dbg_manager->GetExamineMemoryDialog()->GetWindow()));
        mbar->Check(idMenuThreads,      IsWindowReallyShown(dbg_manager->GetThreadsDialog()->GetWindow()));
        mbar->Check(idMenuWatches,      IsWindowReallyShown(dbg_manager->GetWatchesDialog()->GetWindow()));
    }

    // allow other UpdateUI handlers to process this event
    // *very* important! don't forget it...
    event.Skip();
}

void DebuggerMenuHandler::LogActiveConfig()
{
    DebuggerManager *dbgManager = Manager::Get()->GetDebuggerManager();
    const DebuggerManager::RegisteredPlugins &allDebuggers = dbgManager->GetAllDebuggers();
    DebuggerManager::RegisteredPlugins::const_iterator it = allDebuggers.find(m_activeDebugger);
    wxString configName;
    if (it != allDebuggers.end())
    {
        cbDebuggerConfiguration &config = m_activeDebugger->GetActiveConfig();
        configName = it->second.GetGUIName() + wxT(":") + config.GetName();
    }
    m_activeDebugger->Log(_("Active debugger config: ") + configName);
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

            m_activeDebugger->SaveActiveLog();
            m_activeDebugger->ClearLog(false);
            LogActiveConfig();

            if (!m_activeDebugger->Debug(false))
                manager->SetIsRunning(NULL);
        }
        m_disableContinue = false;
    }
    else if (m_activeDebugger->IsStopped() && !m_disableContinue)
    {
        HideValueTooltip();
        m_activeDebugger->Continue();
    }
}

void DebuggerMenuHandler::OnBreak(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    HideValueTooltip();
    m_activeDebugger->Break();
}

void DebuggerMenuHandler::OnStop(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    HideValueTooltip();
    m_activeDebugger->Stop();
}

void DebuggerMenuHandler::OnContinue(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    if(!m_disableContinue)
    {
        HideValueTooltip();
        m_activeDebugger->Continue();
    }
}

void DebuggerMenuHandler::OnNext(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    HideValueTooltip();
    m_activeDebugger->Next();
}

void DebuggerMenuHandler::OnNextInstr(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    HideValueTooltip();
    m_activeDebugger->NextInstruction();
}

void DebuggerMenuHandler::OnStepIntoInstr(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    HideValueTooltip();
    m_activeDebugger->StepIntoInstruction();
}

void DebuggerMenuHandler::OnStep(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    if (m_activeDebugger->IsRunning())
    {
        if(!m_disableContinue)
        {
            HideValueTooltip();
            m_activeDebugger->Step();
    	}
    }
    else
    {
        m_disableContinue = true;
        ProjectManager *manager = Manager::Get()->GetProjectManager();
        if (manager->GetIsRunning() == NULL)
        {
            manager->SetIsRunning(m_activeDebugger);
            m_activeDebugger->SaveActiveLog();
            m_activeDebugger->ClearLog(false);
            LogActiveConfig();

            if (!m_activeDebugger->Debug(true))
                manager->SetIsRunning(NULL);
        }
        m_disableContinue = false;
    }
}

void DebuggerMenuHandler::OnStepOut(wxCommandEvent& event)
{
    cbAssert(m_activeDebugger);
    HideValueTooltip();
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
        if (!m_activeDebugger->IsRunning())
        {
            m_activeDebugger->SaveActiveLog();
            m_activeDebugger->ClearLog(false);
            LogActiveConfig();
        }
        HideValueTooltip();
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
    HideValueTooltip();
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
        cbWatch::Pointer watch = m_activeDebugger->AddWatch(src);
        if (watch.get())
            Manager::Get()->GetDebuggerManager()->GetWatchesDialog()->AddWatch(watch.get());
    }
}

void DebuggerMenuHandler::OnBacktrace(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetBacktraceDialog()->GetWindow();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::Backtrace);
}

void DebuggerMenuHandler::OnBreakpoints(wxCommandEvent& event)
{
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = Manager::Get()->GetDebuggerManager()->GetBreakpointDialog()->GetWindow();
    Manager::Get()->ProcessEvent(evt);
}

void DebuggerMenuHandler::OnCPURegisters(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetCPURegistersDialog()->GetWindow();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::CPURegisters);
}

void DebuggerMenuHandler::OnDisassembly(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetDisassemblyDialog()->GetWindow();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::Disassembly);
}

void DebuggerMenuHandler::OnExamineMemory(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetExamineMemoryDialog()->GetWindow();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::ExamineMemory);
}

void DebuggerMenuHandler::OnThreads(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = manager->GetThreadsDialog()->GetWindow();
    Manager::Get()->ProcessEvent(evt);

    if (event.IsChecked() && manager->GetActiveDebugger())
        manager->GetActiveDebugger()->RequestUpdate(cbDebuggerPlugin::Threads);
}

void DebuggerMenuHandler::OnWatches(wxCommandEvent& event)
{
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = Manager::Get()->GetDebuggerManager()->GetWatchesDialog()->GetWindow();
    Manager::Get()->ProcessEvent(evt);

    cbDebuggerPlugin *activeDebugger = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    if (activeDebugger && event.IsChecked())
        activeDebugger->RequestUpdate(cbDebuggerPlugin::Watches);
}

void DebuggerMenuHandler::OnActiveDebuggerClick(wxCommandEvent& event)
{
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    DebuggerManager::RegisteredPlugins &plugins = manager->GetAllDebuggers();

    for(DebuggerManager::RegisteredPlugins::iterator it = plugins.begin(); it != plugins.end(); ++it)
    {
        DebuggerManager::ConfigurationVector &configs = it->second.GetConfigurations();
        for (DebuggerManager::ConfigurationVector::iterator itConf = configs.begin(); itConf != configs.end(); ++itConf)
        {
            if((*itConf)->GetMenuId() == event.GetId())
            {
                manager->SetActiveDebugger(it->first, itConf);
                return;
            }
        }
    }
}

void  DebuggerMenuHandler::OnActiveDebuggerTargetsDefaultClick(wxCommandEvent& event)
{
    Manager::Get()->GetDebuggerManager()->SetTargetsDefaultAsActiveDebugger();
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
    ProjectManager *manager = Manager::Get()->GetProjectManager();

    bool en = false;
    bool stopped = false, isRunning = false;

    if (plugin)
    {
        cbProject* prj = manager->GetActiveProject();
        en = (prj && !prj->GetCurrentlyCompilingTarget()) || plugin->IsAttachedToProcess();
        stopped = plugin->IsStopped();
        isRunning = plugin->IsRunning();
    }
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();

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
    m_Toolbar->EnableTool(idDebuggerToolInfo, plugin && plugin->ToolMenuEnabled());

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

    m.Check(idMenuBreakpoints,  IsWindowReallyShown(dbg_manager->GetBreakpointDialog()->GetWindow()));
    m.Check(idMenuBacktrace,    IsWindowReallyShown(dbg_manager->GetBacktraceDialog()->GetWindow()));
    m.Check(idMenuRegisters,    IsWindowReallyShown(dbg_manager->GetCPURegistersDialog()->GetWindow()));
    m.Check(idMenuCPU,          IsWindowReallyShown(dbg_manager->GetDisassemblyDialog()->GetWindow()));
    m.Check(idMenuMemory,       IsWindowReallyShown(dbg_manager->GetExamineMemoryDialog()->GetWindow()));
    m.Check(idMenuThreads,      IsWindowReallyShown(dbg_manager->GetThreadsDialog()->GetWindow()));
    m.Check(idMenuWatches,      IsWindowReallyShown(dbg_manager->GetWatchesDialog()->GetWindow()));

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
