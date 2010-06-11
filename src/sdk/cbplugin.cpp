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
    #include <wx/frame.h> // wxFrame
    #include <wx/menu.h>
    #include "cbplugin.h"
    #include "compiler.h" // GetSwitches
    #include "sdk_events.h"
    #include "manager.h"
    #include "projectbuildtarget.h"
    #include "cbproject.h"
    #include "logmanager.h"
    #include "editormanager.h"
    #include "cbeditor.h"
    #include "projectmanager.h"
#endif

#include <wx/toolbar.h>
#include "annoyingdialog.h"
#include "breakpointsdlg.h"
#include "cbstyledtextctrl.h"
#include "editor_hooks.h"
#include "loggers.h"



cbPlugin::cbPlugin()
    : m_Type(ptNone),
    m_IsAttached(false)
{
	SetEvtHandlerEnabled(false);
}

cbPlugin::~cbPlugin()
{
}

void cbPlugin::Attach()
{
	if (m_IsAttached)
		return;
    wxWindow* window = Manager::Get()->GetAppWindow();
    if (window)
    {
		// push ourself in the application's event handling chain...
        window->PushEventHandler(this);
    }
    m_IsAttached = true;
	OnAttach();
	SetEvtHandlerEnabled(true);

	CodeBlocksEvent event(cbEVT_PLUGIN_ATTACHED);
	event.SetPlugin(this);
	// post event in the host's event queue
	Manager::Get()->ProcessEvent(event);
}

void cbPlugin::Release(bool appShutDown)
{
	if (!m_IsAttached)
		return;
	m_IsAttached = false;
	SetEvtHandlerEnabled(false);
	OnRelease(appShutDown);

	CodeBlocksEvent event(cbEVT_PLUGIN_RELEASED);
	event.SetPlugin(this);
	// ask the host to process this event immediately
	// it must be done this way, because if the host references
	// us (through event.GetEventObject()), we might not be valid at that time
	// (while, now, we are...)
	Manager::Get()->ProcessEvent(event);

	if (appShutDown)
        return; // nothing more to do, if the app is shutting down

    wxWindow* window = Manager::Get()->GetAppWindow();
    if (window)
    {
		// remove ourself from the application's event handling chain...
		window->RemoveEventHandler(this);
    }
}

void cbPlugin::NotImplemented(const wxString& log) const
{
    Manager::Get()->GetLogManager()->DebugLog(log + _T(" : not implemented"));
}

/////
///// cbCompilerPlugin
/////

cbCompilerPlugin::cbCompilerPlugin()
{
    m_Type = ptCompiler;
}

/////
///// cbDebuggerPlugin
/////

cbDebuggerPlugin::cbDebuggerPlugin() :
    m_toolbar(NULL),
    m_pCompiler(NULL),
    m_WaitingCompilerToFinish(false),
    m_EditorHookId(-1),
    m_DragInProgress(false)
{
    m_Type = ptDebugger;
}


void cbDebuggerPlugin::OnAttach()
{
    OnAttachReal();
    typedef cbEventFunctor<cbDebuggerPlugin, CodeBlocksEvent> Event;

    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_OPEN, new Event(this, &cbDebuggerPlugin::OnEditorOpened));
    Manager::Get()->RegisterEventSink(cbEVT_PROJECT_ACTIVATE, new Event(this, &cbDebuggerPlugin::OnProjectActivated));
    Manager::Get()->RegisterEventSink(cbEVT_PROJECT_CLOSE, new Event(this, &cbDebuggerPlugin::OnProjectClosed));

    Manager::Get()->RegisterEventSink(cbEVT_COMPILER_FINISHED, new Event(this, &cbDebuggerPlugin::OnCompilerFinished));

    EditorHooks::HookFunctorBase *editor_hook;
    editor_hook = new EditorHooks::HookFunctor<cbDebuggerPlugin>(this, &cbDebuggerPlugin::OnEditorHook);
    m_EditorHookId = EditorHooks::RegisterHook(editor_hook);
}

void cbDebuggerPlugin::OnRelease(bool appShutDown)
{
    EditorHooks::UnregisterHook(m_EditorHookId, true);

    OnReleaseReal(appShutDown);
}

void cbDebuggerPlugin::BuildMenu(wxMenuBar* menuBar)
{
    if (!IsAttached())
        return;
    Manager::Get()->GetDebuggerManager()->GetMenu();
}

wxString cbDebuggerPlugin::GetEditorWordAtCaret()
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return wxEmptyString;
    cbStyledTextCtrl* control = ed->GetControl();
    if (!control)
        return wxEmptyString;

    wxString selected_text = control->GetSelectedText();
    if (selected_text != wxEmptyString)
    {
        selected_text.Trim(true);
        selected_text.Trim(false);

        wxString::size_type pos = selected_text.find(wxT('\n'));
        if (pos != wxString::npos)
        {
            selected_text.Remove(pos, selected_text.length() - pos);
            selected_text.Trim(true);
            selected_text.Trim(false);
        }
        return selected_text;
    }

    int start = control->WordStartPosition(control->GetCurrentPos(), true);
    int end = control->WordEndPosition(control->GetCurrentPos(), true);
    return control->GetTextRange(start, end);
}

void cbDebuggerPlugin::BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    cbDebuggerPlugin *active_plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    if (active_plugin != this)
        return;

    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (!prj)
        return;
    if (!IsAttached())
        return;
    // we 're only interested in editor menus
    // we 'll add a "debug watches" entry only when the debugger is running...
    if (type != mtEditorManager || !menu)
        return;

    wxString word;
    if (IsRunning())
    {
        // has to have a word under the caret...
        word = GetEditorWordAtCaret();
    }
    Manager::Get()->GetDebuggerManager()->BuildContextMenu(*menu, word, IsRunning());
}

bool cbDebuggerPlugin::BuildToolBar(wxToolBar* toolBar)
{
    if (!IsAttached() || !toolBar)
        return false;

    m_toolbar = Manager::Get()->GetDebuggerManager()->GetToolbar();
    return false;
}

wxToolBar* cbDebuggerPlugin::GetToolbar()
{
    return m_toolbar;
}

bool cbDebuggerPlugin::ToolMenuEnabled() const
{
    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();

    bool en = (prj && !prj->GetCurrentlyCompilingTarget()) || IsAttachedToProcess();
    return IsRunning() && en;
}

void cbDebuggerPlugin::ClearActiveMarkFromAllEditors()
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    for (int i = 0; i < edMan->GetEditorsCount(); ++i)
    {
        cbEditor* ed = edMan->GetBuiltinEditor(i);
        if (ed)
            ed->SetDebugLine(-1);
    }
}

cbDebuggerPlugin::SyncEditorResult cbDebuggerPlugin::SyncEditor(const wxString& filename, int line, bool setMarker)
{
    int log_index;
    Manager::Get()->GetDebuggerManager()->GetLogger(false, log_index);

    if (setMarker)
    {
        EditorManager* edMan = Manager::Get()->GetEditorManager();
        for (int i = 0; i < edMan->GetEditorsCount(); ++i)
        {
            cbEditor* ed = edMan->GetBuiltinEditor(i);
            if (ed)
                ed->SetDebugLine(-1);
        }
    }
    FileType ft = FileTypeOf(filename);
    if (ft != ftSource && ft != ftHeader && ft != ftResource)
    {
        if(log_index != -1)
        {
            ShowLog(false);
            Manager::Get()->GetLogManager()->LogError(_("Unknown file: ") + filename, log_index);
        }

        InfoWindow::Display(_("Unknown file"), _("File: ") + filename, 5000);

        return SyncFileUnknown; // don't try to open unknown files
    }

    cbProject* project = Manager::Get()->GetProjectManager()->GetActiveProject();
    ProjectFile* f = project ? project->GetFileByFilename(filename, false, true) : 0;

    wxString unixfilename = UnixFilename(filename);
    wxFileName fname(unixfilename);

    if (project && fname.IsRelative())
        fname.MakeAbsolute(project->GetBasePath());

    // gdb can't work with spaces in filenames, so we have passed it the shorthand form (C:\MYDOCU~1 etc)
    // revert this change now so the file can be located and opened...
    // we do this by calling GetLongPath()
    cbEditor* ed = Manager::Get()->GetEditorManager()->Open(fname.GetLongPath());
    if (ed)
    {
        ed->Show(true);
        if (f && !ed->GetProjectFile())
            ed->SetProjectFile(f);
        ed->GotoLine(line - 1, false);
        if (setMarker)
            ed->SetDebugLine(line - 1);
        return SyncOk;
    }
    else
    {
        if(log_index != -1)
        {
            ShowLog(false);
            Manager::Get()->GetLogManager()->LogError(_("Cannot open file: ") + filename, log_index);
        }

        InfoWindow::Display(_("Cannot open file"), _("File: ") + filename, 5000);

        return SyncFileNotFound;
    }
}

bool HasBreakpoint(cbDebuggerPlugin &plugin, wxString const &filename, int line)
{
    int count = plugin.GetBreakpointsCount();
    for (int ii = 0; ii < count; ++ii)
    {
        const cbBreakpoint* b = plugin.GetBreakpoint(ii);

        if (b->GetFilename() == filename && b->GetLine() == line)
            return true;
    }
    return false;
}

void cbDebuggerPlugin::EditorLinesAddedOrRemoved(cbEditor* editor, int startline, int lines)
{
    // here we keep the breakpoints in sync with the editors
    // (whenever lines are added or removed)
    if (!editor || lines == 0)
        return;

    const wxString& filename = editor->GetFilename();

    std::vector<int> breakpoints_for_file;
    int count = GetBreakpointsCount();
    for (int ii = 0; ii < count; ++ii)
    {
        const cbBreakpoint* b = GetBreakpoint(ii);

        if (b->GetFilename() == filename)
        {
            breakpoints_for_file.push_back(ii);
        }
    }

    if (lines < 0)
    {
        // removed lines
        // make "lines" positive, for easier reading below
        lines = -lines;
        int endline = startline + lines - 1;

        std::vector<cbBreakpoint*> to_remove;

        for (std::vector<int>::iterator it = breakpoints_for_file.begin(); it != breakpoints_for_file.end(); ++it)
        {
            cbBreakpoint* b = GetBreakpoint(*it);
            if (b->GetLine() > endline)
                ShiftBreakpoint(*it, -lines);
            else if (b->GetLine() >= startline && b->GetLine() <= endline)
                to_remove.push_back(b);
        }

        for (std::vector<cbBreakpoint*>::iterator it = to_remove.begin(); it != to_remove.end(); ++it)
            DeleteBreakpoint(*it);

        // special case:
        // when deleting a block of lines, if these lines contain at least one marker,
        // one marker is retained at the cursor position.
        // In our case here, this means that all breakpoints will be deleted in the range
        // but one "orphan" breakpoint (i.e. editor mark only, no actual breakpoint behind it)
        // will be visible on the line with the cursor.
        //
        // If we really have an "orphan", we remove it.
        bool is_orphan = !HasBreakpoint(*this, editor->GetFilename(), endline - lines + 1);
        if (is_orphan)
            editor->RemoveBreakpoint(endline - lines, false);
    }
    else
    {
        for (std::vector<int>::iterator it = breakpoints_for_file.begin(); it != breakpoints_for_file.end(); ++it)
        {
            cbBreakpoint* b = GetBreakpoint(*it);
            if (b->GetLine() > startline)
                ShiftBreakpoint(*it, lines);
        }
    }
    cbBreakpointsDlg *dlg = Manager::Get()->GetDebuggerManager()->GetBreakpointDialog();
    dlg->Reload();
}

void cbDebuggerPlugin::OnEditorOpened(CodeBlocksEvent& event)
{
    // when an editor opens, look if we have breakpoints for it
    // and notify it...
    EditorBase* ed = event.GetEditor();
    if (ed)
    {
        wxFileName bpFileName, edFileName;

        edFileName.Assign(ed->GetFilename());
        edFileName.Normalize();

        int count = GetBreakpointsCount();
        for (int ii = 0; ii < count; ++ii)
        {
            const cbBreakpoint* breakpoint = GetBreakpoint(ii);

            bpFileName.Assign(breakpoint->GetFilename());
            bpFileName.Normalize();

            if (bpFileName.GetFullPath().Matches(edFileName.GetFullPath()))
                ed->ToggleBreakpoint(breakpoint->GetLine() - 1, false);
        }

        if (IsRunning())
        {
            wxString filename;
            int line;
            GetCurrentPosition(filename, line);

            wxFileName dbgFileName(filename);
            dbgFileName.Normalize();
            if (dbgFileName.GetFullPath().IsSameAs(edFileName.GetFullPath()) && line != -1)
            {
                ed->SetDebugLine(line - 1);
            }
        }
    }
    event.Skip(); // must do
}

void cbDebuggerPlugin::OnProjectActivated(CodeBlocksEvent& event)
{
    // allow others to catch this
    event.Skip();

    if(this != Manager::Get()->GetDebuggerManager()->GetActiveDebugger())
        return;
    // when a project is activated and it's not the actively debugged project,
    // ask the user to end debugging or re-activate the debugged project.

    if (!IsRunning())
        return;

    if (event.GetProject() != GetProject() && GetProject())
    {
        wxString msg = _("You can't change the active project while you 're actively debugging another.\n"
                        "Do you want to stop debugging?\n\n"
                        "Click \"Yes\" to stop debugging now or click \"No\" to re-activate the debuggee.");
        if (cbMessageBox(msg, _("Warning"), wxICON_WARNING | wxYES_NO) == wxID_YES)
        {
            Stop();
        }
        else
        {
            Manager::Get()->GetProjectManager()->SetProject(GetProject());
        }
    }
}

void cbDebuggerPlugin::OnProjectClosed(CodeBlocksEvent& event)
{
    // allow others to catch this
    event.Skip();

    if(this != Manager::Get()->GetDebuggerManager()->GetActiveDebugger())
        return;
    CleanupWhenProjectClosed(event.GetProject());

    // when a project closes, make sure it's not the actively debugged project.
    // if so, end debugging immediately!
    if (!IsRunning())
        return;

    if (event.GetProject() == GetProject())
    {
        AnnoyingDialog dlg(_("Project closed while debugging message"),
                           _("The project you were debugging has closed.\n"
                             "(The application most likely just finished.)\n"
                             "The debugging session will terminate immediately."),
                            wxART_WARNING, AnnoyingDialog::OK, wxID_OK);
        dlg.ShowModal();
        Stop();
        ResetProject();
    }
}

void cbDebuggerPlugin::OnEditorHook(cbEditor* editor, wxScintillaEvent& event)
{
    if (event.GetEventType() == wxEVT_SCI_START_DRAG)
        m_DragInProgress = true;
    else if (event.GetEventType() == wxEVT_SCI_FINISHED_DRAG)
        m_DragInProgress = false;
}

bool cbDebuggerPlugin::DragInProgress() const
{
    return m_DragInProgress;
}

void cbDebuggerPlugin::ShowLog(bool clear)
{
    TextCtrlLogger *log = Manager::Get()->GetDebuggerManager()->GetLogger(false);
    TextCtrlLogger *debug_log = Manager::Get()->GetDebuggerManager()->GetLogger(true);

    CodeBlocksLogEvent event_get_active(cbEVT_GET_ACTIVE_LOG_WINDOW);
    Manager::Get()->ProcessEvent(event_get_active);

    if (event_get_active.logger != debug_log || !debug_log)
    {
        // switch to the debugging log and clear it
        CodeBlocksLogEvent event_switch_log(cbEVT_SWITCH_TO_LOG_WINDOW, log);
        CodeBlocksLogEvent event_show_log(cbEVT_SHOW_LOG_MANAGER);
        Manager::Get()->ProcessEvent(event_switch_log);
        Manager::Get()->ProcessEvent(event_show_log);
    }

    if (clear && log)
        log->Clear();
}


void cbDebuggerPlugin::SwitchToDebuggingLayout()
{
    CodeBlocksLayoutEvent queryEvent(cbEVT_QUERY_VIEW_LAYOUT);
    CodeBlocksLayoutEvent switchEvent(cbEVT_SWITCH_VIEW_LAYOUT, _("Debugging"));

    #if wxCHECK_VERSION(2, 9, 0)
    Manager::Get()->GetLogManager()->DebugLog(F(_("Switching layout to \"%s\""), switchEvent.layout.wx_str()));
    #else
    Manager::Get()->GetLogManager()->DebugLog(F(_("Switching layout to \"%s\""), switchEvent.layout.c_str()));
    #endif

    // query the current layout
    Manager::Get()->ProcessEvent(queryEvent);
    m_PreviousLayout = queryEvent.layout;

    // switch to debugging layout
    Manager::Get()->ProcessEvent(switchEvent);
}

void cbDebuggerPlugin::SwitchToPreviousLayout()
{
    CodeBlocksLayoutEvent switchEvent(cbEVT_SWITCH_VIEW_LAYOUT, m_PreviousLayout);

    wxString const &name = !switchEvent.layout.IsEmpty() ? switchEvent.layout : wxString(_("Code::Blocks default"));

    #if wxCHECK_VERSION(2, 9, 0)
    Manager::Get()->GetLogManager()->DebugLog(F(_("Switching layout to \"%s\""), name.wx_str());
    #else
    Manager::Get()->GetLogManager()->DebugLog(F(_("Switching layout to \"%s\""), name.c_str()));
    #endif

    // switch to previous layout
    Manager::Get()->ProcessEvent(switchEvent);
}


wxString cbDebuggerPlugin::GetDebuggee(ProjectBuildTarget* target)
{
    if (!target)
        return wxEmptyString;

    int log_index;
    Manager::Get()->GetDebuggerManager()->GetLogger(false, log_index);

    wxString out;
    switch (target->GetTargetType())
    {
        case ttExecutable:
        case ttConsoleOnly:
            out = UnixFilename(target->GetOutputFilename());
            Manager::Get()->GetMacrosManager()->ReplaceEnvVars(out); // apply env vars
            Manager::Get()->GetLogManager()->Log(_("Adding file: ") + out, log_index);
            ConvertDirectory(out);
            break;

        case ttStaticLib:
        case ttDynamicLib:
            // check for hostapp
            if (target->GetHostApplication().IsEmpty())
            {
                cbMessageBox(_("You must select a host application to \"run\" a library..."));
                return wxEmptyString;
            }
            out = UnixFilename(target->GetHostApplication());
            Manager::Get()->GetMacrosManager()->ReplaceEnvVars(out); // apply env vars
            Manager::Get()->GetLogManager()->Log(_("Adding file: ") + out, log_index);
            ConvertDirectory(out);
            break;
//            // for DLLs, add the DLL's symbols
//            if (target->GetTargetType() == ttDynamicLib)
//            {
//                wxString symbols;
//                out = UnixFilename(target->GetOutputFilename());
//                Manager::Get()->GetMacrosManager()->ReplaceEnvVars(out); // apply env vars
//                msgMan->Log(m_PageIndex, _("Adding symbol file: %s"), out.c_str());
//                ConvertToGDBDirectory(out);
//                QueueCommand(new DbgCmd_AddSymbolFile(this, out));
//            }
//            break;

        default: break;
    }
    return out;
}

wxString cbDebuggerPlugin::FindDebuggerExecutable(Compiler* compiler)
{
    if (compiler->GetPrograms().DBG.IsEmpty())
        return wxEmptyString;
//    if (!wxFileExists(compiler->GetMasterPath() + wxFILE_SEP_PATH + _T("bin") + wxFILE_SEP_PATH + compiler->GetPrograms().DBG))
//        return wxEmptyString;

    wxString masterPath = compiler->GetMasterPath();
    while (masterPath.Last() == '\\' || masterPath.Last() == '/')
        masterPath.RemoveLast();
    wxString gdb = compiler->GetPrograms().DBG;
    const wxArrayString& extraPaths = compiler->GetExtraPaths();

    wxPathList pathList;
    pathList.Add(masterPath + wxFILE_SEP_PATH + _T("bin"));
    for (unsigned int i = 0; i < extraPaths.GetCount(); ++i)
    {
        if (!extraPaths[i].IsEmpty())
            pathList.Add(extraPaths[i]);
    }
    pathList.AddEnvList(_T("PATH"));
    wxString binPath = pathList.FindAbsoluteValidPath(gdb);
    // it seems, under Win32, the above command doesn't search in paths with spaces...
    // look directly for the file in question in masterPath
    if (binPath.IsEmpty() || !(pathList.Index(wxPathOnly(binPath)) != wxNOT_FOUND))
    {
        if (wxFileExists(masterPath + wxFILE_SEP_PATH + _T("bin") + wxFILE_SEP_PATH + gdb))
            binPath = masterPath + wxFILE_SEP_PATH + _T("bin");
        else if (wxFileExists(masterPath + wxFILE_SEP_PATH + gdb))
            binPath = masterPath;
        else
        {
            for (unsigned int i = 0; i < extraPaths.GetCount(); ++i)
            {
                if (!extraPaths[i].IsEmpty())
                {
                    if (wxFileExists(extraPaths[i] + wxFILE_SEP_PATH + gdb))
                    {
                        binPath = extraPaths[i];
                        break;
                    }
                }
            }
        }
    }

    return binPath;
}

bool cbDebuggerPlugin::EnsureBuildUpToDate()
{
    m_WaitingCompilerToFinish = false;

    // compile project/target (if not attaching to a PID)
    if (!IsAttachedToProcess())
    {
        // should we build to make sure project is up-to-date?
        if (!Manager::Get()->GetConfigManager(_T("debugger"))->ReadBool(_T("auto_build"), true))
        {
            m_WaitingCompilerToFinish = false;
            m_pCompiler = NULL;
            return true;
        }

        LogManager* msgMan = Manager::Get()->GetLogManager();

        // make sure the target is compiled
        PluginsArray plugins = Manager::Get()->GetPluginManager()->GetCompilerOffers();
        if (plugins.GetCount())
            m_pCompiler = (cbCompilerPlugin*)plugins[0];
        else
            m_pCompiler = NULL;
        if (m_pCompiler)
        {
            int page_index;
            Manager::Get()->GetDebuggerManager()->GetLogger(false, page_index);
            // is the compiler already running?
            if (m_pCompiler->IsRunning())
            {
                msgMan->Log(_("Compiler in use..."), page_index);
                msgMan->Log(_("Aborting debugging session"), page_index);
                cbMessageBox(_("The compiler is currently in use. Aborting debugging session..."),
                             _("Compiler running"), wxICON_WARNING);
                return false;
            }

            msgMan->Log(_("Building to ensure sources are up-to-date"), page_index);
            m_WaitingCompilerToFinish = true;
            m_pCompiler->Build();
            // now, when the build is finished, DoDebug will be launched in OnCompilerFinished()
        }
    }
    return true;
}

//bool cbDebuggerPlugin::CheckBuild()
//{
//    if (m_pCompiler)
//    {
//        LogManager* msgMan = Manager::Get()->GetLogManager();
//        int page_index;
//        Manager::Get()->GetDebuggerManager()->GetLogger(false, page_index);
//
//        if (m_pCompiler->GetExitCode() != 0)
//        {
//            msgMan->Log(_("Build failed..."), page_index);
//            msgMan->Log(_("Aborting debugging session"), page_index);
//            cbMessageBox(_("Build failed. Aborting debugging session..."), _("Build failed"), wxICON_WARNING);
//            return false;
//        }
//        msgMan->Log(_("Build succeeded"), page_index);
//        return true;
//    }
//    else
//        return true;
//}

void cbDebuggerPlugin::OnCompilerFinished(CodeBlocksEvent& event)
{
    if (m_WaitingCompilerToFinish)
    {
        m_WaitingCompilerToFinish = false;
        // only proceed if build succeeeded
        if (m_pCompiler && m_pCompiler->GetExitCode() != 0)
        {
            AnnoyingDialog dlg(_("Debug anyway?"), _("Build failed, do you want to debug the program?"),
                               wxART_QUESTION, AnnoyingDialog::YES_NO, wxID_NO);
            if (dlg.ShowModal() != wxID_YES)
                return;
        }
        CompilerFinished();
    }
}

int cbDebuggerPlugin::RunNixConsole(wxString &consoleTty)
{
    consoleTty = wxEmptyString;
#ifndef __WXMSW__

    // start the xterm and put the shell to sleep with -e sleep 80000
    // fetch the xterm tty so we can issue to gdb a "tty /dev/pts/#"
    // redirecting program stdin/stdout/stderr to the xterm console.

    wxString cmd;
    wxString title = wxT("Program Console");
    int consolePid = 0;
    // for non-win platforms, use m_ConsoleTerm to run the console app
    wxString term = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/console_terminal"), DEFAULT_CONSOLE_TERM);
    term.Replace(_T("$TITLE"), _T("'") + title + _T("'"));
    cmd << term << _T(" ");
    cmd << wxT("sleep ");
    cmd << wxString::Format(wxT("%d"),80000 + ::wxGetProcessId());

    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(cmd);
//    DebugLog(wxString::Format( _("Executing: %s"), cmd.c_str()) );
    //start xterm -e sleep {some unique # of seconds}
    consolePid = wxExecute(cmd, wxEXEC_ASYNC);
    if (consolePid <= 0) return -1;

    // Issue the PS command to get the /dev/tty device name
    // First, wait for the xterm to settle down, else PS won't see the sleep task
    Manager::Yield();
    ::wxSleep(1);
    consoleTty = GetConsoleTty(consolePid);
    if (!consoleTty.IsEmpty() )
    {
        // show what we found as tty
//        DebugLog(wxString::Format(wxT("GetConsoleTTY[%s]ConsolePid[%d]"), m_ConsoleTty.c_str(), consolePid));
        return consolePid;
    }
    // failed to find the console tty
//    DebugLog( wxT("Console Execution error:failed to find console tty."));
    if (consolePid != 0)
        ::wxKill(consolePid);
    consolePid = 0;
#endif // !__WWXMSW__
    return -1;
}

wxString cbDebuggerPlugin::GetConsoleTty(int ConsolePid)
{
#ifndef __WXMSW__

    // execute the ps x -o command  and read PS output to get the /dev/tty field

    unsigned long ConsPid = ConsolePid;
    wxString psCmd;
    wxArrayString psOutput;
    wxArrayString psErrors;

    psCmd << wxT("ps x -o tty,pid,command");
//    DebugLog(wxString::Format( _("Executing: %s"), psCmd.c_str()) );
    int result = wxExecute(psCmd, psOutput, psErrors, wxEXEC_SYNC);
    psCmd.Clear();
    if (result != 0)
    {
        psCmd << wxT("Result of ps x:") << result;
//        DebugLog(wxString::Format( _("Execution Error:"), psCmd.c_str()) );
        return wxEmptyString;
    }

    wxString ConsTtyStr;
    wxString ConsPidStr;
    ConsPidStr << ConsPid;
    //find task with our unique sleep time
    wxString uniqueSleepTimeStr;
    uniqueSleepTimeStr << wxT("sleep ") << wxString::Format(wxT("%d"),80000 + ::wxGetProcessId());
    // search the output of "ps pid" command
    int knt = psOutput.GetCount();
    for (int i=knt-1; i>-1; --i)
    {
        psCmd = psOutput.Item(i);
//        DebugLog(wxString::Format( _("PS result: %s"), psCmd.c_str()) );
        // find the pts/# or tty/# or whatever it's called
        // by seaching the output of "ps x -o tty,pid,command" command.
        // The output of ps looks like:
        // TT       PID   COMMAND
        // pts/0    13342 /bin/sh ./run.sh
        // pts/0    13343 /home/pecanpecan/devel/trunk/src/devel/codeblocks
        // pts/0    13361 /usr/bin/gdb -nx -fullname -quiet -args ./conio
        // pts/0    13362 xterm -font -*-*-*-*-*-*-20-*-*-*-*-*-*-* -T Program Console -e sleep 93343
        // pts/2    13363 sleep 93343
        // ?        13365 /home/pecan/proj/conio/conio
        // pts/1    13370 ps x -o tty,pid,command

        if (psCmd.Contains(uniqueSleepTimeStr))
        do
        {
            // check for correct "sleep" line
            if (psCmd.Contains(wxT("-T")))
                break; //error;wrong sleep line.
            // found "sleep 93343" string, extract tty field
            ConsTtyStr = wxT("/dev/") + psCmd.BeforeFirst(' ');
//            DebugLog(wxString::Format( _("TTY is[%s]"), ConsTtyStr.c_str()) );
            return ConsTtyStr;
        } while(0);//if do
    }//for

    knt = psErrors.GetCount();
//    for (int i=0; i<knt; ++i)
//        DebugLog(wxString::Format( _("PS Error:%s"), psErrors.Item(i).c_str()) );
#endif // !__WXMSW__
    return wxEmptyString;
}


void cbDebuggerPlugin::BringCBToFront()
{
    wxWindow* app = Manager::Get()->GetAppWindow();
    if (app)
        app->Raise();
}


/////
///// cbToolPlugin
/////

cbToolPlugin::cbToolPlugin()
{
    m_Type = ptTool;
}

/////
///// cbMimePlugin
/////

cbMimePlugin::cbMimePlugin()
{
    m_Type = ptMime;
}

/////
///// cbCodeCompletionPlugin
/////

cbCodeCompletionPlugin::cbCodeCompletionPlugin()
{
    m_Type = ptCodeCompletion;
}

/////
///// cbWizardPlugin
/////

cbWizardPlugin::cbWizardPlugin()
{
    m_Type = ptWizard;
}
