/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Id$
* $Date$
*/

#include <sdk.h>
#include <wx/txtstrm.h>
#include <wx/regex.h>
#include <wx/dialog.h>
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>

#include <manager.h>
#include <configmanager.h>
#include <messagemanager.h>
#include <projectmanager.h>
#include <pluginmanager.h>
#include <editormanager.h>
#include <macrosmanager.h>
#include <projectbuildtarget.h>
#include <sdk_events.h>
#include <editarraystringdlg.h>
#include <compilerfactory.h>
#include <licenses.h>
#include <xtra_res.h>
#include <wx/xrc/xmlres.h>
#include <wx/fs_zip.h>

#include "debuggergdb.h"
#include "debugger_cmds.h"
#include "debuggeroptionsdlg.h"
#include "breakpointsdlg.h"
#include "editbreakpointdlg.h"
#include "editwatchesdlg.h"
#include "editwatchdlg.h"

#ifdef __WXMSW__
    #include <winbase.h> //For GetShortPathName()...only for windows systems
#endif

#define implement_debugger_toolbar

// valid debugger command constants
#define CMD_CONTINUE    1
#define CMD_STEP        2
#define CMD_STEPIN      3
#define CMD_STOP        4
#define CMD_BACKTRACE   5
#define CMD_DISASSEMBLE 6

#define GDB_PROMPT _T("(gdb)")

const wxString g_EscapeChars = wxChar(26);

int idMenuDebug = XRCID("idDebuggerMenuDebug");
int idMenuRunToCursor = XRCID("idDebuggerMenuRunToCursor");
int idMenuNext = XRCID("idDebuggerMenuNext");
int idMenuStep = XRCID("idDebuggerMenuStep");
int idMenuStepOut = XRCID("idDebuggerMenuStepOut");
int idMenuStop = XRCID("idDebuggerMenuStop");
int idMenuContinue = XRCID("idDebuggerMenuContinue");
int idMenuToggleBreakpoint = XRCID("idDebuggerMenuToggleBreakpoint");
int idMenuSendCommandToGDB = XRCID("idDebuggerMenuSendCommandToGDB");
int idMenuAddSymbolFile = XRCID("idDebuggerMenuAddSymbolFile");
int idMenuCPU = XRCID("idDebuggerMenuCPU");
int idMenuBacktrace = XRCID("idDebuggerMenuBacktrace");
int idMenuBreakpoints = XRCID("idDebuggerMenuBreakpoints");
int idMenuEditWatches = XRCID("idDebuggerMenuEditWatches");

int idGDBProcess = wxNewId();
int idTimerPollDebugger = wxNewId();
int idMenuDebuggerAddWatch = wxNewId();

CB_IMPLEMENT_PLUGIN(DebuggerGDB);

BEGIN_EVENT_TABLE(DebuggerGDB, cbDebuggerPlugin)
	EVT_UPDATE_UI_RANGE(idMenuContinue, idMenuDebuggerAddWatch, DebuggerGDB::OnUpdateUI)
	// these are different because they are loaded from the XRC
	EVT_UPDATE_UI(XRCID("idDebuggerMenuDebug"), DebuggerGDB::OnUpdateUI)
	EVT_UPDATE_UI(XRCID("idDebuggerMenuRunToCursor"), DebuggerGDB::OnUpdateUI)
	EVT_UPDATE_UI(XRCID("idDebuggerMenuNext"), DebuggerGDB::OnUpdateUI)
	EVT_UPDATE_UI(XRCID("idDebuggerMenuStep"), DebuggerGDB::OnUpdateUI)
	EVT_UPDATE_UI(XRCID("idDebuggerMenuStepOut"), DebuggerGDB::OnUpdateUI)
	EVT_UPDATE_UI(XRCID("idDebuggerMenuStop"), DebuggerGDB::OnUpdateUI)

	EVT_MENU(idMenuDebug, DebuggerGDB::OnDebug)
	EVT_MENU(idMenuContinue, DebuggerGDB::OnContinue)
	EVT_MENU(idMenuNext, DebuggerGDB::OnNext)
	EVT_MENU(idMenuStep, DebuggerGDB::OnStep)
	EVT_MENU(idMenuStepOut, DebuggerGDB::OnStepOut)
	EVT_MENU(idMenuToggleBreakpoint, DebuggerGDB::OnToggleBreakpoint)
	EVT_MENU(idMenuRunToCursor, DebuggerGDB::OnRunToCursor)
	EVT_MENU(idMenuStop, DebuggerGDB::OnStop)
	EVT_MENU(idMenuSendCommandToGDB, DebuggerGDB::OnSendCommandToGDB)
	EVT_MENU(idMenuAddSymbolFile, DebuggerGDB::OnAddSymbolFile)
	EVT_MENU(idMenuBacktrace, DebuggerGDB::OnBacktrace)
	EVT_MENU(idMenuCPU, DebuggerGDB::OnDisassemble)
	EVT_MENU(idMenuBreakpoints, DebuggerGDB::OnBreakpoints)
	EVT_MENU(idMenuEditWatches, DebuggerGDB::OnEditWatches)
    EVT_MENU(idMenuDebuggerAddWatch, DebuggerGDB::OnAddWatch)

	EVT_EDITOR_BREAKPOINT_ADD(DebuggerGDB::OnBreakpointAdd)
	EVT_EDITOR_BREAKPOINT_EDIT(DebuggerGDB::OnBreakpointEdit)
	EVT_EDITOR_BREAKPOINT_DELETE(DebuggerGDB::OnBreakpointDelete)
	EVT_EDITOR_TOOLTIP(DebuggerGDB::OnValueTooltip)
	EVT_EDITOR_OPEN(DebuggerGDB::OnEditorOpened)

	EVT_PIPEDPROCESS_STDOUT(idGDBProcess, DebuggerGDB::OnGDBOutput)
	EVT_PIPEDPROCESS_STDERR(idGDBProcess, DebuggerGDB::OnGDBError)
	EVT_PIPEDPROCESS_TERMINATED(idGDBProcess, DebuggerGDB::OnGDBTerminated)

	EVT_IDLE(DebuggerGDB::OnIdle)
	EVT_TIMER(idTimerPollDebugger, DebuggerGDB::OnTimer)

	EVT_COMMAND(-1, cbCustom_WATCHES_CHANGED, DebuggerGDB::OnWatchesChanged)
END_EVENT_TABLE()

DebuggerGDB::DebuggerGDB()
	: m_QueueBusy(true),
	m_pMenu(0L),
	m_pLog(0L),
	m_pDbgLog(0L),
	m_pProcess(0L),
	m_pTbar(0L),
	m_PageIndex(-1),
	m_DbgPageIndex(-1),
	m_ProgramIsStopped(true),
	m_pCompiler(0L),
	m_LastExitCode(0),
	m_TargetIndex(-1),
	m_Pid(0),
	m_EvalWin(0L),
	m_pTree(0L),
	m_NoDebugInfo(false),
	m_BreakOnEntry(false),
	m_HaltAtLine(0),
	m_HasDebugLog(false),
	m_StoppedOnSignal(false),
	m_LastBreakpointNum(1),
	m_pDisassembly(0),
	m_pBacktrace(0)
{
    Manager::Get()->Loadxrc(_T("/debugger_gdb.zip#zip:*.xrc"));

	m_PluginInfo.name = _T("DebuggerGDB");
	m_PluginInfo.title = _("GDB Debugger");
	m_PluginInfo.version = _T("0.1");
	m_PluginInfo.description = _("Plugin that interfaces the GNU GDB debugger.");
    m_PluginInfo.author = _T("Yiannis An. Mandravellos");
    m_PluginInfo.authorEmail = _T("info@codeblocks.org");
    m_PluginInfo.authorWebsite = _T("www.codeblocks.org");
	m_PluginInfo.thanksTo = _T("");
	m_PluginInfo.license = LICENSE_GPL;
	m_PluginInfo.hasConfigure = true;

	m_TimerPollDebugger.SetOwner(this, idTimerPollDebugger);

	ConfigManager::AddConfiguration(m_PluginInfo.title, _T("/debugger_gdb"));
}

void DebuggerGDB::OnAttach()
{
    MessageManager* msgMan = Manager::Get()->GetMessageManager();
    wxFont font(8, wxMODERN, wxNORMAL, wxNORMAL);
    m_pLog = new SimpleTextLog(msgMan, _("Debugger"));
    m_pLog->GetTextControl()->SetFont(font);
    m_PageIndex = msgMan->AddLog(m_pLog);
    // set log image
	wxBitmap bmp;
	wxString prefix = ConfigManager::Get()->Read(_T("data_path")) + _T("/images/");
    bmp.LoadFile(prefix + _T("misc_16x16.png"), wxBITMAP_TYPE_PNG);
    Manager::Get()->GetMessageManager()->SetLogImage(m_pLog, bmp);

    m_HasDebugLog = ConfigManager::Get()->Read(_T("debugger_gdb/debug_log"), (long int)0L);
    if (m_HasDebugLog)
    {
        m_pDbgLog = new SimpleTextLog(msgMan, m_PluginInfo.title + _(" (debug)"));
        m_pDbgLog->GetTextControl()->SetFont(font);
        m_DbgPageIndex = msgMan->AddLog(m_pDbgLog);
        // set log image
        bmp.LoadFile(prefix + _T("contents_16x16.png"), wxBITMAP_TYPE_PNG);
        Manager::Get()->GetMessageManager()->SetLogImage(m_pDbgLog, bmp);
    }

	if (!m_pTree)
		m_pTree = new DebuggerTree(this, Manager::Get()->GetNotebook());
}

void DebuggerGDB::OnRelease(bool appShutDown)
{
    if (m_pDisassembly)
        m_pDisassembly->Destroy();
    m_pDisassembly = 0;

    if (m_pBacktrace)
        m_pBacktrace->Destroy();
    m_pBacktrace = 0;

	if (m_pTree)
	{
		delete m_pTree;
		m_pTree = 0L;
	}

    //Close debug session when appShutDown
	CmdStop();

    if (Manager::Get()->GetMessageManager())
    {
        if (m_HasDebugLog)
            Manager::Get()->GetMessageManager()->DeletePage(m_DbgPageIndex);
        Manager::Get()->GetMessageManager()->DeletePage(m_PageIndex);
    }
}

DebuggerGDB::~DebuggerGDB()
{
}

int DebuggerGDB::Configure()
{
	DebuggerOptionsDlg dlg(Manager::Get()->GetAppWindow());
	int ret = dlg.ShowModal();

	bool needsRestart = ConfigManager::Get()->Read(_T("debugger_gdb/debug_log"), (long int)0L) != m_HasDebugLog;
	if (needsRestart)
        wxMessageBox(_("Code::Blocks needs to be restarted for the changes to take effect."), _("Information"), wxICON_INFORMATION);

	return ret;
}

void DebuggerGDB::BuildMenu(wxMenuBar* menuBar)
{
	if (!m_IsAttached)
		return;
    m_pMenu=Manager::Get()->LoadMenu(_T("debugger_menu"),true);

	// ok, now, where do we insert?
	// three possibilities here:
	// a) locate "Compile" menu and insert after it
	// b) locate "Project" menu and insert after it
	// c) if not found (?), insert at pos 5
	int finalPos = 5;
	int projcompMenuPos = menuBar->FindMenu(_("&Build"));
	if (projcompMenuPos == wxNOT_FOUND)
        projcompMenuPos = menuBar->FindMenu(_("&Compile"));

	if (projcompMenuPos != wxNOT_FOUND)
		finalPos = projcompMenuPos + 1;
	else
	{
		projcompMenuPos = menuBar->FindMenu(_("&Project"));
		if (projcompMenuPos != wxNOT_FOUND)
			finalPos = projcompMenuPos + 1;
	}
    menuBar->Insert(finalPos, m_pMenu, _("&Debug"));
}

void DebuggerGDB::BuildModuleMenu(const ModuleType type, wxMenu* menu, const wxString& arg)
{
	cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
	if (!m_IsAttached)
		return;
    // we 're only interested in editor menus
    // we 'll add a "debug watches" entry only when the debugger is running...
    if (type != mtEditorManager || !menu) return;
    if (!prj) return;
    // Insert toggle breakpoint
    menu->Insert(0,idMenuToggleBreakpoint, _("Toggle breakpoint"));
	// Insert Run to Cursor
	menu->Insert(1,idMenuRunToCursor, _("Run to cursor"));
	menu->Insert(2,wxID_SEPARATOR, _T("-"));

    if (!m_pProcess) return;
    // has to have a word under the caret...
    wxString w = GetEditorWordAtCaret();
    if (w.IsEmpty())
        return;

    wxString s;
    s.Printf(_("Watch '%s'"), w.c_str());
	menu->Insert(2, idMenuDebuggerAddWatch,  s);
}

bool DebuggerGDB::BuildToolBar(wxToolBar* toolBar)
{
	m_pTbar = toolBar;
    /* Loads toolbar using new Manager class functions */
#ifdef implement_debugger_toolbar
    if (!m_IsAttached || !toolBar)
		return false;
    wxString my_16x16=Manager::isToolBar16x16(toolBar) ? _T("_16x16") : _T("");
    Manager::AddonToolBar(toolBar,wxString(_T("debugger_toolbar"))+my_16x16);
    toolBar->Realize();
    return true;
#else
    return false;
#endif
}

void DebuggerGDB::QueueCommand(DebuggerCmd* dcmd)
{
//    Log(_T("Queueing command: ") + dcmd->m_Cmd);
    m_DCmds.Add(dcmd);
    RunQueue();
}

DebuggerCmd* DebuggerGDB::CurrentCommand()
{
    return m_DCmds.GetCount() ? m_DCmds[0] : 0;
}

void DebuggerGDB::RunQueue()
{
    if (m_QueueBusy || !m_DCmds.GetCount())
        return;

//    Log(_T("Running command: ") + CurrentCommand()->m_Cmd);
    SendCommand(CurrentCommand()->m_Cmd);
}

void DebuggerGDB::RemoveTopCommand(bool deleteIt)
{
    if (m_QueueBusy || !m_DCmds.GetCount())
        return;

//    Log(_T("Removing command: ") + CurrentCommand()->m_Cmd);
    if (deleteIt)
        delete m_DCmds[0];
    m_DCmds.RemoveAt(0);
}

void DebuggerGDB::ClearQueue()
{
    int idx = 0;
    // if the first command in the queue is running, delete all others
    // (this will be deleted when done)
    if (m_QueueBusy && !m_DCmds.GetCount())
        idx = 1;
    for (int i = idx; i < (int)m_DCmds.GetCount(); ++i)
    {
        delete m_DCmds[i];
        m_DCmds.RemoveAt(i);
    }
}

void DebuggerGDB::Log(const wxString& msg)
{
    Manager::Get()->GetMessageManager()->Log(m_PageIndex, msg);
}

void DebuggerGDB::DebugLog(const wxString& msg)
{
    // gdb debug messages
    if (m_HasDebugLog)
        Manager::Get()->GetMessageManager()->Log(m_DbgPageIndex, msg);
}

void DebuggerGDB::DoWatches()
{
    // clear watches tree
    m_pTree->ResetTree();
    m_pTree->SetNumberOfUpdates(2 + m_pTree->GetWatches().GetCount()); // watches + locals + args

	if (m_pProcess)
	{
	    // locals before args because of precedence
        if (ConfigManager::Get()->Read(_T("debugger_gdb/watch_locals"), 1))
            QueueCommand(new DbgCmd_InfoLocals(this, m_pTree));
        if (ConfigManager::Get()->Read(_T("debugger_gdb/watch_args"), 1))
            QueueCommand(new DbgCmd_InfoArguments(this, m_pTree));
		for (unsigned int i = 0; i < m_pTree->GetWatches().GetCount(); ++i)
		{
		    Watch& w = m_pTree->GetWatches()[i];
            QueueCommand(new DbgCmd_Watch(this, m_pTree, &w));
		}
	}
}

void DebuggerGDB::ClearBreakpointsArray()
{
	QueueCommand(new DbgCmd_RemoveBreakpoint(this, 0)); // clear all breakpoints
    for (unsigned int i = 0; i < m_Breakpoints.GetCount(); ++i)
    {
        DebuggerBreakpoint* bp = m_Breakpoints[i];
        delete bp;
    }
    m_Breakpoints.Clear();
}

int DebuggerGDB::HasBreakpoint(const wxString& file, int line)
{
//    Manager::Get()->GetMessageManager()->Log(m_PageIndex, _T("Looking for breakpoint at %s, line %d"), file.c_str(), line);
    for (unsigned int i = 0; i < m_Breakpoints.GetCount(); ++i)
    {
        DebuggerBreakpoint* bp = m_Breakpoints[i];
        if (bp->filename == file && bp->line == line)
            return i;
    }
    return -1;
}

void DebuggerGDB::SetBreakpoints()
{
	QueueCommand(new DbgCmd_RemoveBreakpoint(this, 0)); // clear all breakpoints

    for (unsigned int i = 0; i < m_Breakpoints.GetCount(); ++i)
    {
        DebuggerBreakpoint* bp = m_Breakpoints[i];
        bp->bpNum = -1;
        QueueCommand(new DbgCmd_AddBreakpoint(this, bp));
	}
}

int DebuggerGDB::Debug()
{
	if (m_pProcess)
		return 1;
    m_NoDebugInfo = false;
    m_LastBreakpointNum = 1;

	ProjectManager* prjMan = Manager::Get()->GetProjectManager();
	cbProject* project = prjMan->GetActiveProject();
	if (!project)
		return 2;

    MessageManager* msgMan = Manager::Get()->GetMessageManager();
    msgMan->SwitchTo(m_PageIndex);
	m_pLog->GetTextControl()->Clear();


    m_TargetIndex = project->GetActiveBuildTarget();
    msgMan->SwitchTo(m_PageIndex);
	msgMan->AppendLog(m_PageIndex, _("Selecting target: "));
	if (m_TargetIndex == -1)
	{
        m_TargetIndex = project->SelectTarget(m_TargetIndex);
        if (m_TargetIndex == -1)
        {
            msgMan->Log(m_PageIndex, _("canceled"));
            return 3;
        }
	}
	ProjectBuildTarget* target = project->GetBuildTarget(m_TargetIndex);
    if (target->GetTargetType() == ttCommandsOnly)
    {
        wxMessageBox(_("The selected target is only running pre/post build step commands\n"
                    "Can't debug such a target..."), _("Information"), wxICON_INFORMATION);
        msgMan->Log(m_PageIndex, _("aborted"));
        return 3;
    }
	msgMan->Log(m_PageIndex, target->GetTitle());

	Compiler* actualCompiler = CompilerFactory::Compilers[target ? target->GetCompilerIndex() : project->GetCompilerIndex()];
	if (!actualCompiler)
	{
		wxString msg;
		msg.Printf(_("This %s is configured to use an invalid debugger.\nThe operation failed..."), target ? _("target") : _("project"));
		wxMessageBox(msg, _("Error"), wxICON_ERROR);
		return 9;
	}

	if (actualCompiler->GetPrograms().DBG.IsEmpty() ||
        !wxFileExists(actualCompiler->GetMasterPath() + wxFileName::GetPathSeparator() + _T("bin") + wxFileName::GetPathSeparator() + actualCompiler->GetPrograms().DBG))
	{
        wxMessageBox(_("The debugger executable is not set.\n"
                    "To set it, go to \"Settings/Configure plugins/Compiler\", switch to the \"Programs\" tab\n"
                    "and select the debugger program."), _("Error"), wxICON_ERROR);
        msgMan->Log(m_PageIndex, _("Aborted"));
        return 4;
	}

    // make sure it is compiled
	PluginsArray plugins = Manager::Get()->GetPluginManager()->GetCompilerOffers();
	if (plugins.GetCount())
		m_pCompiler = (cbCompilerPlugin*)plugins[0];
	if (m_pCompiler)
	{
		msgMan->AppendLog(m_PageIndex, _("Compiling: "));
	    // is the compiler already running?
	    if (m_pCompiler->IsRunning())
        {
            msgMan->Log(m_PageIndex, _("compiler in use..."));
			msgMan->Log(m_PageIndex, _("Aborting debugging session"));
			return -1;
        }

		m_pCompiler->Compile(target);
		while (m_pCompiler->IsRunning())
			wxYield();
        msgMan->SwitchTo(m_PageIndex);
		if (m_pCompiler->GetExitCode() != 0)
		{
			msgMan->Log(m_PageIndex, _("failed"));
			msgMan->Log(m_PageIndex, _("Aborting debugging session"));
			return -1;
		}
		msgMan->Log(m_PageIndex, _("done"));
	}

	wxString cmdexe;
	cmdexe = actualCompiler->GetPrograms().DBG;
	cmdexe.Trim();
	cmdexe.Trim(true);
	if(cmdexe.IsEmpty())
    {
        msgMan->AppendLog(m_PageIndex,_("ERROR: You need to specify a debugger program in the compiler's settings."));
        #ifdef __WXMSW__
        msgMan->Log(m_PageIndex,_("\n(For MINGW compilers, it's 'gdb.exe' (without the quotes))"));
        #else
        msgMan->Log(m_PageIndex,_("\n(For GCC compilers, it's 'gdb' (without the quotes))"));
        #endif
        return -1;
    }

	wxString cmd;
	wxString sep = wxFileName::GetPathSeparator();
	cmd << actualCompiler->GetMasterPath() << sep << _T("bin") << sep << cmdexe << _T(" --fullname -nx");// -annotate=2");

	wxLogNull ln; // we perform our own error handling and logging
    m_pProcess = new PipedProcess((void**)&m_pProcess, this, idGDBProcess, true, project->GetBasePath());
	msgMan->AppendLog(m_PageIndex, _("Starting debugger: "));
//    msgMan->AppendLog(m_PageIndex, _(cmd));
    m_Pid = wxExecute(cmd, wxEXEC_ASYNC, m_pProcess);
//    m_Pid = m_pProcess->Launch(cmd);

    if (!m_Pid)
    {
		delete m_pProcess;
		m_pProcess = 0;
		msgMan->Log(m_PageIndex, _("failed"));
        return -1;
    }
    else if (!m_pProcess->GetOutputStream())
    {
		delete m_pProcess;
		m_pProcess = 0;
		msgMan->Log(m_PageIndex, _("failed (to get debugger's stdin)"));
        return -2;
    }
    else if (!m_pProcess->GetInputStream())
    {
		delete m_pProcess;
		m_pProcess = 0;
		msgMan->Log(m_PageIndex, _("failed (to get debugger's stdout)"));
        return -2;
    }
    else if (!m_pProcess->GetErrorStream())
    {
		delete m_pProcess;
		m_pProcess = 0;
		msgMan->Log(m_PageIndex, _("failed (to get debugger's stderr)"));
        return -2;
    }
	else
		msgMan->Log(m_PageIndex, _("done"));

	wxString out;
	m_TimerPollDebugger.Start(100);

    // send built-in init commands

// NOTE: we add an extra linefeed to the prompt because the process pipe
// reads whole lines. Without it, it 'd block...
// And we must send this immediately...
	SendCommand(wxString(_T("set prompt ")) + GDB_PROMPT + _T("\\n"));

	QueueCommand(new DebuggerCmd(this, _T("set confirm off")));
    QueueCommand(new DebuggerCmd(this, _T("set breakpoint pending on")));
#ifndef __WXMSW__
    QueueCommand(new DebuggerCmd(this, _T("set disassembly-flavor att")));
#else
	if (target->GetTargetType() == ttConsoleOnly)
        QueueCommand(new DebuggerCmd(this, _T("set new-console on")));
    QueueCommand(new DebuggerCmd(this, _T("set disassembly-flavor intel")));
#endif

    // pass user init-commands
    wxString init = ConfigManager::Get()->Read(_T("debugger_gdb/init_commands"), _T(""));
    wxArrayString initCmds = GetArrayFromString(init, _T('\n'));
    for (unsigned int i = 0; i < initCmds.GetCount(); ++i)
    {
        QueueCommand(new DebuggerCmd(this, initCmds[i]));
    }

    if (ConfigManager::Get()->Read(_T("debugger_gdb/add_other_search_dirs"), 0L))
    {
        // add as include dirs all open project base dirs
        ProjectsArray* projects = prjMan->GetProjects();
        for (unsigned int i = 0; i < projects->GetCount(); ++i)
        {
            cbProject* it = projects->Item(i);
            // skip if it's THE project (already added)
            if (it == project)
                continue;
            AddSourceDir(it->GetBasePath());
        }
    }
    // lastly, add THE project as source dir
	AddSourceDir(project->GetBasePath());

	cmd.Clear();
	switch (target->GetTargetType())
	{
		case ttExecutable:
		case ttConsoleOnly:
			// "-async" option is not really supported, at least under Win32, as far as I know
			out = UnixFilename(target->GetOutputFilename());
            Manager::Get()->GetMacrosManager()->ReplaceEnvVars(out); // apply env vars
			msgMan->Log(m_PageIndex, _("Adding file: %s"), out.c_str());
            ConvertToGDBDirectory(out);
			QueueCommand(new DbgCmd_SetDebuggee(this, out));
			break;

		case ttStaticLib:
		case ttDynamicLib:
			// check for hostapp
			if (target->GetHostApplication().IsEmpty())
			{
				wxMessageBox(_("You must select a host application to \"run\" a library..."));
				CmdStop();
				return 4;
			}
			out = UnixFilename(target->GetHostApplication());
            Manager::Get()->GetMacrosManager()->ReplaceEnvVars(out); // apply env vars
			msgMan->Log(m_PageIndex, _("Adding file: %s"), out.c_str());
			ConvertToGDBDirectory(out);
			QueueCommand(new DbgCmd_SetDebuggee(this, out));
			if (target->GetTargetType() == ttDynamicLib)
			{
				wxString symbols;
				out = UnixFilename(target->GetOutputFilename());
                Manager::Get()->GetMacrosManager()->ReplaceEnvVars(out); // apply env vars
				msgMan->Log(m_PageIndex, _("Adding symbol file: %s"), out.c_str());
                ConvertToGDBDirectory(out);
				QueueCommand(new DbgCmd_AddSymbolFile(this, out));
			}
			break;

        default: break;
	}

	if (!target->GetExecutionParameters().IsEmpty())
		QueueCommand(new DbgCmd_SetArguments(this, target->GetExecutionParameters()));

    // switch to output dir
	// wxFileName dir(target->GetOutputFilename());
	// wxString path = UnixFilename(dir.GetPath(wxPATH_GET_VOLUME));
    wxString path = UnixFilename(target->GetWorkingDir());
    if (!path.IsEmpty())
    {
        Manager::Get()->GetMacrosManager()->ReplaceEnvVars(path); // apply env vars
        cmd.Clear();
        ConvertToGDBDirectory(path);
        if (path != _T(".")) // avoid silly message "changing to ."
        {
            msgMan->Log(m_PageIndex, _("Changing directory to: %s"), path.c_str());
            QueueCommand(new DebuggerCmd(this, path));
        }
    }

    // set breakpoints
	SetBreakpoints();
	if (!m_Tbreak.IsEmpty())
	{
		QueueCommand(new DebuggerCmd(this, m_Tbreak));
		m_Tbreak.Clear();
	}

    // finally, run the process
    if (m_BreakOnEntry)
    {
    	m_BreakOnEntry = false;
    	QueueCommand(new DebuggerCmd(this, _T("start")));
	}
	else
        QueueCommand(new DebuggerCmd(this, _T("run")));

	return 0;
}

void DebuggerGDB::AddSourceDir(const wxString& dir)
{
    if (dir.IsEmpty())
        return;
    wxString filename = dir;
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(filename); // apply env vars
    Manager::Get()->GetMessageManager()->Log(m_PageIndex, _("Adding source dir: %s"), filename.c_str());
    ConvertToGDBDirectory(filename, _T(""), false);
    QueueCommand(new DbgCmd_AddSourceDir(this, filename));
}

// static
void DebuggerGDB::StripQuotes(wxString& str)
{
	if (str.GetChar(0) == _T('\"') && str.GetChar(str.Length() - 1) == _T('\"'))
			str = str.Mid(1, str.Length() - 2);
}

// static
void DebuggerGDB::ConvertToGDBFriendly(wxString& str)
{
    if (str.IsEmpty())
        return;

    str = UnixFilename(str);
    while (str.Replace(_T("\\"), _T("/")))
        ;
    while (str.Replace(_T("//"), _T("/")))
        ;
//    str.Replace("/", "//");
    if (str.Find(_T(' ')) != -1 && str.GetChar(0) != _T('"'))
        str = _T("\"") + str + _T("\"");
}

// static
//if relative == false, try to leave as an absolute path
void DebuggerGDB::ConvertToGDBDirectory(wxString& str, wxString base, bool relative)
{
    if (str.IsEmpty())
        return;

	ConvertToGDBFriendly(str);
	ConvertToGDBFriendly(base);
	StripQuotes(str);
	StripQuotes(base);

	#ifdef __WXMSW__
		int ColonLocation = str.Find(_T(':'));
		wxChar buf[255];
		if(ColonLocation != -1)
		{
			//If can, get 8.3 name for path (Windows only)
			GetShortPathName(str.c_str(), buf, 255);
			str = buf;
		}
		else if(!base.IsEmpty() && str.GetChar(0) != _T('/'))
		{
			if(base.GetChar(base.Length()) == _T('/')) base = base.Mid(0, base.Length() - 2);
			while(!str.IsEmpty())
			{
				base += _T("/") + str.BeforeFirst(_T('/'));
				if(str.Find(_T('/')) != -1) str = str.AfterFirst(_T('/'));
				else str.Clear();
			}
			GetShortPathName(base.c_str(), buf, 255);
			str = buf;
		}

		if(ColonLocation == -1 || base.IsEmpty())
			relative = false;		//Can't do it
	#else
		if((str.GetChar(0) != _T('/') && str.GetChar(0) != _T('~')) || base.IsEmpty())
			relative = false;
	#endif

	if(relative)
	{
		#ifdef __WXMSW__
			if(str.Find(_T(':')) != -1) str = str.Mid(str.Find(_T(':')) + 2, str.Length());
			if(base.Find(_T(':')) != -1) base = base.Mid(base.Find(_T(':')) + 2, base.Length());
		#else
			if(str.GetChar(0) == _T('/')) str = str.Mid(1, str.Length());
			else if(str.GetChar(0) == _T('~')) str = str.Mid(2, str.Length());
			if(base.GetChar(0) == _T('/')) base = base.Mid(1, base.Length());
			else if(base.GetChar(0) == _T('~')) base = base.Mid(2, base.Length());
		#endif

		while(!base.IsEmpty() && !str.IsEmpty())
		{
			if(str.BeforeFirst(_T('/')) == base.BeforeFirst(_T('/')))
			{
				if(str.Find(_T('/')) == -1) str.Clear();
				else str = str.AfterFirst(_T('/'));

				if(base.Find(_T('/')) == -1) base.Clear();
				else base = base.AfterFirst(_T('/'));
			}
			else break;
		}
		while (!base.IsEmpty())
		{
			str = _T("../") + str;
			if(base.Find(_T('/')) == -1) base.Clear();
			else base = base.AfterFirst(_T('/'));
		}
	}
	ConvertToGDBFriendly(str);
}

void DebuggerGDB::SendCommand(const wxString& cmd)
{
//    Log(cmd);
    if (!m_pProcess || !m_ProgramIsStopped)
        return;
    if (m_HasDebugLog)
        Manager::Get()->GetMessageManager()->Log(m_DbgPageIndex, _T("> ") + cmd);
    m_QueueBusy = true;
	m_pProcess->SendString(cmd);
}

void DebuggerGDB::RunCommand(int cmd)
{
    if (!m_pProcess || !m_ProgramIsStopped)
        return;

    switch (cmd)
    {
        case CMD_CONTINUE:
            ClearActiveMarkFromAllEditors();
            Manager::Get()->GetMessageManager()->Log(m_PageIndex, _("Continuing..."));
            QueueCommand(new DebuggerCmd(this, _T("cont")));
            break;

        case CMD_STEP:
            ClearActiveMarkFromAllEditors();
            QueueCommand(new DebuggerCmd(this, _T("next")));
            break;

        case CMD_STEPIN:
            ClearActiveMarkFromAllEditors();
            QueueCommand(new DebuggerCmd(this, _T("step")));
            break;

        case CMD_STOP:
            ClearActiveMarkFromAllEditors();
            QueueCommand(new DebuggerCmd(this, _T("quit")));
            break;

        case CMD_BACKTRACE:
//            Manager::Get()->GetMessageManager()->Log(m_PageIndex, "Running back-trace...");
            QueueCommand(new DbgCmd_Backtrace(this, m_pBacktrace));
            break;

        case CMD_DISASSEMBLE:
        {
//            Manager::Get()->GetMessageManager()->Log(m_PageIndex, "Disassemblying...");
            QueueCommand(new DbgCmd_Disassembly(this, m_pDisassembly));
            break;
        }

        default: break;
    }
}

void DebuggerGDB::CmdDisassemble()
{
    if (!m_pDisassembly)
        m_pDisassembly = new DisassemblyDlg(Manager::Get()->GetAppWindow(), this);
    m_pDisassembly->Show();
    RunCommand(CMD_DISASSEMBLE);
}

void DebuggerGDB::CmdBacktrace()
{
    if (!m_pBacktrace)
        m_pBacktrace = new BacktraceDlg(Manager::Get()->GetAppWindow(), this);
    m_pBacktrace->Clear();
    m_pBacktrace->Show();
    RunCommand(CMD_BACKTRACE);
}

void DebuggerGDB::CmdContinue()
{
	if (!m_Tbreak.IsEmpty())
	{
		QueueCommand(new DebuggerCmd(this, m_Tbreak));
		m_Tbreak.Clear();
	}
    RunCommand(CMD_CONTINUE);
}

void DebuggerGDB::CmdNext()
{
    RunCommand(CMD_STEP);
}

void DebuggerGDB::CmdStep()
{
    RunCommand(CMD_STEPIN);
}

bool DebuggerGDB::Validate(const wxString& line, const char cb)
{
	bool bResult = false;

	int bep = line.Find(cb)+1;
	int scs = line.Find(_T('\''))+1;
	int sce = line.Find(_T('\''),true)+1;
	int dcs = line.Find(_T('"'))+1;
	int dce = line.Find(_T('"'),true)+1;
	//No single and double quote
	if(!scs && !sce && !dcs && !dce) bResult = true;
	//No single/double quote in pair
	if(!(sce-scs) && !(dce-dcs)) bResult = true;
	//Outside of single quote
	if((sce-scs) && ((bep < scs)||(bep >sce))) bResult = true;
	//Outside of double quote
	if((dce-dcs) && ((bep < dcs)||(bep >dce))) bResult = true;

	return bResult;
}

void DebuggerGDB::CmdStepOut()
{
	cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
	if (!ed) return;
	ProjectFile* pf = ed->GetProjectFile();
	if (!pf) return;
	wxString filename = pf->file.GetFullName(), lineBuf, cmd;
	cbStyledTextCtrl* stc = ed->GetControl();
	int line = m_HaltAtLine;
	lineBuf = stc->GetLine(line);

	unsigned int nLevel = 1;
	while(nLevel){
		 if ((lineBuf.Find(_T('{'))+1) && Validate(lineBuf, _T('{')) &&
			 (line > m_HaltAtLine)) nLevel++;
		 if ((lineBuf.Find(_T('}'))+1) && Validate(lineBuf, _T('}'))) nLevel--;
		 if (nLevel) lineBuf = stc->GetLine(++line);
	}
	if (line == stc->GetCurrentLine())
		CmdNext();
	else
	{
        wxString out = filename;
        ConvertToGDBDirectory(out);
		cmd << _T("tbreak ") << out << _T(":") << line+1;
		m_Tbreak = cmd;
		CmdContinue();
	}
}

void DebuggerGDB::CmdRunToCursor()
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
	if (!ed)
		return;

	ProjectFile* pf = ed->GetProjectFile();
	if (!pf)
		return;
	wxString cmd, filename = pf->file.GetFullName();
    wxString out = filename;
    ConvertToGDBDirectory(out);
	cmd << _T("tbreak ") << out << _T(":") << ed->GetControl()->GetCurrentLine()+1;
	m_Tbreak = cmd;
	if (m_pProcess)
	{
		CmdContinue();
	}
	else
	{
		Debug();
	}
}

void DebuggerGDB::CmdToggleBreakpoint()
{
	ClearActiveMarkFromAllEditors();
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
	if (!ed)
		return;
	ed->ToggleBreakpoint();
}

void DebuggerGDB::CmdStop()
{
	if (m_pProcess && m_Pid)
	{
		if (m_ProgramIsStopped)
		{
            RunCommand(CMD_STOP);
            m_pProcess->CloseOutput();
		}
		else
		{
            m_pProcess->CloseOutput();
			wxKillError err = m_pProcess->Kill(m_Pid, wxSIGKILL);
			if (err == wxKILL_OK){
/*
				wxMessageBox(_("Debug session terminated!"),
					_("Debug"), wxOK | wxICON_EXCLAMATION);
*/
			}
			m_ProgramIsStopped = true;
		}
	}
}

void DebuggerGDB::ParseOutput(const wxString& output)
{
    static wxString buffer;
	buffer << output << _T('\n');

    int idx = buffer.First(GDB_PROMPT);
    if (idx != wxNOT_FOUND)
    {
        m_QueueBusy = false;
//        Log(_T("Parsing output: ") + buffer);
        DebuggerCmd* cmd = CurrentCommand();
        if (!cmd)
            DebugLog(buffer);
        else
        {
//            Log(_T("Command parsing output: ") + buffer.Left(idx));
            RemoveTopCommand(false);
            buffer.Remove(idx);
            if (buffer[buffer.Length() - 1] == _T('\n'))
                buffer.Remove(buffer.Length() - 1);
            cmd->ParseOutput(buffer.Left(idx));
            delete cmd;
            RunQueue();
        }
    }
    else
        return; // come back later

    // non-command messages (e.g. breakpoint hits)
    // break them up in lines
    wxArrayString lines = GetArrayFromString(buffer, _T('\n'));
    for (unsigned int i = 0; i < lines.GetCount(); ++i)
    {
//            Log(_T("DEBUG: ") + lines[i]); // write it in the full debugger log

        // log GDB's version
        if (lines[i].StartsWith(_T("GNU gdb")))
        {
            // it's the gdb banner. Just display the version and "eat" the rest
            Log(_("Debugger name and version: ") + lines[i]);
            break;
        }

        // Is the program running?
        else if (lines[i].StartsWith(_T("Starting program:")))
            m_ProgramIsStopped = false;

        // Is the program exited?
        else if (lines[i].StartsWith(_T("Program exited")))
        {
            m_ProgramIsStopped = true;
            Manager::Get()->GetMessageManager()->Log(m_PageIndex, lines[i]);
            CmdStop();
        }

        // no debug symbols?
        else if (lines[i].Contains(_T("(no debugging symbols found)")))
        {
            m_NoDebugInfo = true;
        }

        // signal
        else if (lines[i].StartsWith(_T("Program received signal")))
        {
            BringAppToFront();
            m_StoppedOnSignal = true;
            Log(lines[i]);
            if (wxMessageBox(wxString::Format(_("%s\nDo you want to view the backtrace?"), lines[i].c_str()), _("Question"), wxICON_QUESTION | wxYES_NO) == wxYES)
            {
                // TODO: queue backtrace command
            }
        }

        // cursor change
        else if (lines[i].StartsWith(g_EscapeChars)) // ->->
        {
            // 