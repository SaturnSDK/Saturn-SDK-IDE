/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include <algorithm> // std::remove_if

#ifndef CB_PRECOMP
    #include <wx/txtstrm.h>
    #include <wx/regex.h>
    #include <wx/msgdlg.h>
    #include <wx/frame.h> // GetMenuBar

    #include "cbproject.h"
    #include "manager.h"
    #include "configmanager.h"
    #include "logmanager.h"
    #include "projectmanager.h"
    #include "pluginmanager.h"
    #include "editormanager.h"
    #include "macrosmanager.h"
    #include "cbeditor.h"
    #include "projectbuildtarget.h"
    #include "sdk_events.h"
    #include "compilerfactory.h"
    #include "xtra_res.h"

    #include "scrollingdialog.h"
    #include "globals.h"
#endif

#ifdef __WXMSW__
    #include <winbase.h>
#else
    int GetShortPathName(const void*, void*, int){/* bogus */ return 0; };
#endif

#include <wx/tokenzr.h>
#include "editarraystringdlg.h"
#include "projectloader_hooks.h"
#include "annoyingdialog.h"
#include "cbstyledtextctrl.h"

#include "backtracedlg.h"
#include "breakpointsdlg.h"
#include "disassemblydlg.h"
#include "editbreakpointdlg.h"
#include "examinememorydlg.h"
#include "threadsdlg.h"
#include "watchesdlg.h"

#include "databreakpointdlg.h"
#include "debuggerdriver.h"
#include "debuggergdb.h"
#include "debuggeroptionsdlg.h"
#include "debuggeroptionsprjdlg.h"
#include "editwatchesdlg.h"
#include "editwatchdlg.h"


#if defined(__APPLE__) && defined(__MACH__)
    #define LIBRARY_ENVVAR _T("DYLD_LIBRARY_PATH")
#elif !defined(__WXMSW__)
    #define LIBRARY_ENVVAR _T("LD_LIBRARY_PATH")
#else
    #define LIBRARY_ENVVAR _T("PATH")
#endif

#define implement_debugger_toolbar

// function pointer to DebugBreakProcess under windows (XP+)
#if (_WIN32_WINNT >= 0x0501)
#include "Tlhelp32.h"
typedef BOOL WINAPI   (*DebugBreakProcessApiCall)       (HANDLE);
typedef HANDLE WINAPI (*CreateToolhelp32SnapshotApiCall)(DWORD  dwFlags,   DWORD             th32ProcessID);
typedef BOOL WINAPI   (*Process32FirstApiCall)          (HANDLE hSnapshot, LPPROCESSENTRY32W lppe);
typedef BOOL WINAPI   (*Process32NextApiCall)           (HANDLE hSnapshot, LPPROCESSENTRY32W lppe);

DebugBreakProcessApiCall        DebugBreakProcessFunc = 0;
CreateToolhelp32SnapshotApiCall CreateToolhelp32SnapshotFunc = 0;
Process32FirstApiCall           Process32FirstFunc = 0;
Process32NextApiCall            Process32NextFunc = 0;

HINSTANCE kernelLib = 0;

#endif

#ifdef __WXMSW__
// disable the CTRL_C event
BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
    return TRUE;
}
#endif

// valid debugger command constants
enum DebugCommandConst
{
    CMD_CONTINUE,
    CMD_STEP,
    CMD_STEPIN,
    CMD_STEPOUT,
    CMD_STEP_INSTR,
    CMD_STEP_INTO_INSTR,
    CMD_STOP,
    CMD_BACKTRACE,
    CMD_DISASSEMBLE,
    CMD_REGISTERS,
    CMD_MEMORYDUMP,
    CMD_RUNNINGTHREADS,
};

const wxString g_EscapeChar = wxChar(26);

int idMenuInfoFrame = XRCID("idDebuggerCurrentFrame");
int idMenuInfoDLL = XRCID("idDebuggerLoadedDLLs");
int idMenuInfoFiles = XRCID("idDebuggerFiles");
int idMenuInfoFPU = XRCID("idDebuggerFPU");
int idMenuInfoSignals = XRCID("idDebuggerSignals");

int idGDBProcess = wxNewId();
int idTimerPollDebugger = wxNewId();
int idMenuSettings = wxNewId();

int idMenuWatchDereference = wxNewId();

// this auto-registers the plugin
namespace
{
    PluginRegistrant<DebuggerGDB> reg(_T("Debugger"));
}

BEGIN_EVENT_TABLE(DebuggerGDB, cbDebuggerPlugin)
    EVT_MENU(idMenuInfoFrame, DebuggerGDB::OnInfoFrame)
    EVT_MENU(idMenuInfoDLL, DebuggerGDB::OnInfoDLL)
    EVT_MENU(idMenuInfoFiles, DebuggerGDB::OnInfoFiles)
    EVT_MENU(idMenuInfoFPU, DebuggerGDB::OnInfoFPU)
    EVT_MENU(idMenuInfoSignals, DebuggerGDB::OnInfoSignals)

    EVT_MENU(idMenuWatchDereference, DebuggerGDB::OnMenuWatchDereference)

    EVT_PIPEDPROCESS_STDOUT(idGDBProcess, DebuggerGDB::OnGDBOutput)
    EVT_PIPEDPROCESS_STDERR(idGDBProcess, DebuggerGDB::OnGDBError)
    EVT_PIPEDPROCESS_TERMINATED(idGDBProcess, DebuggerGDB::OnGDBTerminated)

    EVT_IDLE(DebuggerGDB::OnIdle)
    EVT_TIMER(idTimerPollDebugger, DebuggerGDB::OnTimer)

//    EVT_COMMAND(-1, cbCustom_WATCHES_CHANGED, DebuggerGDB::OnWatchesChanged)
    EVT_COMMAND(-1, DEBUGGER_CURSOR_CHANGED, DebuggerGDB::OnCursorChanged)
    EVT_COMMAND(-1, DEBUGGER_SHOW_FILE_LINE, DebuggerGDB::OnShowFile)
END_EVENT_TABLE()

DebuggerGDB::DebuggerGDB()
    : m_State(this),
    m_pProcess(0L),
    m_PageIndex(-1),
    m_DbgPageIndex(-1),
//    m_pCompiler(0L),
    m_LastExitCode(0),
    m_Pid(0),
    m_PidToAttach(0),
    m_NoDebugInfo(false),
    m_HasDebugLog(false),
    m_StoppedOnSignal(false),
    m_pProject(0),
    m_TemporaryBreak(false)
{
    if(!Manager::LoadResource(_T("debugger.zip")))
    {
        NotifyMissingFile(_T("debugger.zip"));
    }

    // get a function pointer to DebugBreakProcess under windows (XP+)
    #if (_WIN32_WINNT >= 0x0501)
    kernelLib = LoadLibrary(TEXT("kernel32.dll"));
    if (kernelLib)
    {
        DebugBreakProcessFunc = (DebugBreakProcessApiCall)GetProcAddress(kernelLib, "DebugBreakProcess");
        //Windows XP
        CreateToolhelp32SnapshotFunc = (CreateToolhelp32SnapshotApiCall)GetProcAddress(kernelLib, "CreateToolhelp32Snapshot");
        Process32FirstFunc = (Process32FirstApiCall)GetProcAddress(kernelLib, "Process32First");
        Process32NextFunc = (Process32NextApiCall)GetProcAddress(kernelLib, "Process32Next");
    }
    #endif
}

DebuggerGDB::~DebuggerGDB()
{
    #if (_WIN32_WINNT >= 0x0501)
    if (kernelLib)
        FreeLibrary(kernelLib);
    #endif
}

void DebuggerGDB::OnAttachReal()
{
    m_TimerPollDebugger.SetOwner(this, idTimerPollDebugger);

    DebuggerManager *dbg_manager = Manager::Get()->GetDebuggerManager();
    dbg_manager->RegisterDebugger(this, _T("gdb_debugger"));

    dbg_manager->GetLogger(false, m_PageIndex);

    m_HasDebugLog = Manager::Get()->GetConfigManager(_T("debugger"))->ReadBool(_T("debug_log"), false);
    if (m_HasDebugLog)
    {
        dbg_manager->GetLogger(true, m_DbgPageIndex);
    }

    {
        DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
        manager->GetBacktraceDialog();
        manager->GetBreakpointDialog();
        manager->GetCPURegistersDialog();
        manager->GetExamineMemoryDialog();
        manager->GetWatchesDialog();
        manager->GetThreadsDialog();
    }

    // hook to project loading procedure
    ProjectLoaderHooks::HookFunctorBase* myhook = new ProjectLoaderHooks::HookFunctor<DebuggerGDB>(this, &DebuggerGDB::OnProjectLoadingHook);
    m_HookId = ProjectLoaderHooks::RegisterHook(myhook);

    // register event sink
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_TOOLTIP, new cbEventFunctor<DebuggerGDB, CodeBlocksEvent>(this, &DebuggerGDB::OnValueTooltip));
    Manager::Get()->RegisterEventSink(cbEVT_BUILDTARGET_SELECTED, new cbEventFunctor<DebuggerGDB, CodeBlocksEvent>(this, &DebuggerGDB::OnBuildTargetSelected));
}

void DebuggerGDB::OnReleaseReal(bool /*appShutDown*/)
{
    ProjectLoaderHooks::UnregisterHook(m_HookId, true);

    DebuggerManager *dbg_manager = Manager::Get()->GetDebuggerManager();

    //Close debug session when appShutDown
    if (m_State.HasDriver())
    {
        Stop();
        wxYieldIfNeeded();
    }

    m_State.CleanUp();

    if (Manager::Get()->GetLogManager())
    {
        if (m_HasDebugLog)
            dbg_manager->HideLogger(true);
        dbg_manager->HideLogger(false);
    }

    KillConsole();

    dbg_manager->UnregisterDebugger(this);
}

int DebuggerGDB::Configure()
{
//    DebuggerOptionsDlg dlg(Manager::Get()->GetAppWindow());
//    int ret = dlg.ShowModal();
//
//    bool needsRestart = Manager::Get()->GetConfigManager(_T("debugger"))->ReadBool(_T("debug_log"), false) != m_HasDebugLog;
//    if (needsRestart)
//        cbMessageBox(_("Code::Blocks needs to be restarted for the changes to take effect."), _("Information"), wxICON_INFORMATION);
    return 0;
}

cbConfigurationPanel* DebuggerGDB::GetConfigurationPanel(wxWindow* parent)
{
    DebuggerOptionsDlg* dlg = new DebuggerOptionsDlg(parent, this);
    return dlg;
}

cbConfigurationPanel* DebuggerGDB::GetProjectConfigurationPanel(wxWindow* parent, cbProject* project)
{
    DebuggerOptionsProjectDlg* dlg = new DebuggerOptionsProjectDlg(parent, this, project);
    return dlg;
}

void DebuggerGDB::RefreshConfiguration()
{
    // the only thing that we need to change on the fly, is the debugger's debug log
    bool log_visible = Manager::Get()->GetConfigManager(_T("debugger"))->ReadBool(_T("debug_log"), false);

    if (!log_visible && m_HasDebugLog)
    {
        Manager::Get()->GetDebuggerManager()->HideLogger(true);
        m_DbgPageIndex = -1;
    }
    else if (log_visible && !m_HasDebugLog)
    {
        Manager::Get()->GetDebuggerManager()->GetLogger(true, m_DbgPageIndex);
    }
    m_HasDebugLog = log_visible;
}

void DebuggerGDB::Log(const wxString& msg)
{
    if (IsAttached())
        Manager::Get()->GetLogManager()->Log(msg, m_PageIndex);
}

void DebuggerGDB::DebugLog(const wxString& msg)
{
    // gdb debug messages
    if (IsAttached() && m_HasDebugLog)
        Manager::Get()->GetLogManager()->Log(msg, m_DbgPageIndex);
}

wxArrayString& DebuggerGDB::GetSearchDirs(cbProject* prj)
{
    SearchDirsMap::iterator it = m_SearchDirs.find(prj);
    if (it == m_SearchDirs.end())
    {
        // create an empty set for this project
        it = m_SearchDirs.insert(m_SearchDirs.begin(), std::make_pair(prj, wxArrayString()));
    }

    return it->second;
}

RemoteDebuggingMap& DebuggerGDB::GetRemoteDebuggingMap(cbProject* project)
{
    if (!project)
        project = m_pProject;
    ProjectRemoteDebuggingMap::iterator it = m_RemoteDebugging.find(project);
    if (it == m_RemoteDebugging.end())
    {
        // create an empty set for this project
        it = m_RemoteDebugging.insert(m_RemoteDebugging.begin(), std::make_pair(project, RemoteDebuggingMap()));
    }
    return it->second;
}


void DebuggerGDB::OnProjectLoadingHook(cbProject* project, TiXmlElement* elem, bool loading)
{
    wxArrayString& pdirs = GetSearchDirs(project);
    RemoteDebuggingMap& rdprj = GetRemoteDebuggingMap(project);

    if (loading)
    {
        rdprj.clear();

        // Hook called when loading project file.
        TiXmlElement* conf = elem->FirstChildElement("debugger");
        if (conf)
        {
            TiXmlElement* pathsElem = conf->FirstChildElement("search_path");
            while (pathsElem)
            {
                if (pathsElem->Attribute("add"))
                {
                    wxString dir = cbC2U(pathsElem->Attribute("add"));
                    if (pdirs.Index(dir) == wxNOT_FOUND)
                        pdirs.Add(dir);
                }

                pathsElem = pathsElem->NextSiblingElement("search_path");
            }

            TiXmlElement* rdElem = conf->FirstChildElement("remote_debugging");
            while (rdElem)
            {
                wxString targetName = cbC2U(rdElem->Attribute("target"));
                ProjectBuildTarget* bt = project->GetBuildTarget(targetName);

                TiXmlElement* rdOpt = rdElem->FirstChildElement("options");

                if (rdOpt)
                {
                    RemoteDebugging rd;

                    if (rdOpt->Attribute("conn_type"))
                        rd.connType = (RemoteDebugging::ConnectionType)atol(rdOpt->Attribute("conn_type"));
                    if (rdOpt->Attribute("serial_port"))
                        rd.serialPort = cbC2U(rdOpt->Attribute("serial_port"));
                    if (rdOpt->Attribute("serial_baud"))
                        rd.serialBaud = cbC2U(rdOpt->Attribute("serial_baud"));
                    if (rdOpt->Attribute("ip_address"))
                        rd.ip = cbC2U(rdOpt->Attribute("ip_address"));
                    if (rdOpt->Attribute("ip_port"))
                        rd.ipPort = cbC2U(rdOpt->Attribute("ip_port"));
                    if (rdOpt->Attribute("additional_cmds"))
                        rd.additionalCmds = cbC2U(rdOpt->Attribute("additional_cmds"));
                    if (rdOpt->Attribute("additional_cmds_before"))
                        rd.additionalCmdsBefore = cbC2U(rdOpt->Attribute("additional_cmds_before"));
                    if (rdOpt->Attribute("skip_ld_path"))
                        rd.skipLDpath = cbC2U(rdOpt->Attribute("skip_ld_path")) != _T("0");
                    if (rdOpt->Attribute("additional_shell_cmds_after"))
                        rd.additionalShellCmdsAfter = cbC2U(rdOpt->Attribute("additional_shell_cmds_after"));
                    if (rdOpt->Attribute("additional_shell_cmds_before"))
                        rd.additionalShellCmdsBefore = cbC2U(rdOpt->Attribute("additional_shell_cmds_before"));

                    rdprj.insert(rdprj.end(), std::make_pair(bt, rd));
                }
//                else
//                    Manager::Get()->GetLogManager()->Log(_T("Unknown target in remote_debugging: ") + targetName, m_PageIndex, Logger::warning);

                rdElem = rdElem->NextSiblingElement("remote_debugging");
            }
        }
    }
    else
    {
        // Hook called when saving project file.

        // since rev4332, the project keeps a copy of the <Extensions> element
        // and re-uses it when saving the project (so to avoid losing entries in it
        // if plugins that use that element are not loaded atm).
        // so, instead of blindly inserting the element, we must first check it's
        // not already there (and if it is, clear its contents)
        TiXmlElement* node = elem->FirstChildElement("debugger");
        if (!node)
            node = elem->InsertEndChild(TiXmlElement("debugger"))->ToElement();
        node->Clear();

        if (pdirs.GetCount() > 0)
        {
            for (size_t i = 0; i < pdirs.GetCount(); ++i)
            {
                TiXmlElement* path = node->InsertEndChild(TiXmlElement("search_path"))->ToElement();
                path->SetAttribute("add", cbU2C(pdirs[i]));
            }
        }

        if (rdprj.size())
        {
            for (RemoteDebuggingMap::iterator it = rdprj.begin(); it != rdprj.end(); ++it)
            {
//                // valid targets only
//                if (!it->first)
//                    continue;

                RemoteDebugging& rd = it->second;

                // if no different than defaults, skip it
                if (rd.serialPort.IsEmpty() && rd.ip.IsEmpty() &&
                    rd.additionalCmds.IsEmpty() && rd.additionalCmdsBefore.IsEmpty() &&
                    !rd.skipLDpath)
                {
                    continue;
                }

                TiXmlElement* rdnode = node->InsertEndChild(TiXmlElement("remote_debugging"))->ToElement();
                if (it->first)
                    rdnode->SetAttribute("target", cbU2C(it->first->GetTitle()));

                TiXmlElement* tgtnode = rdnode->InsertEndChild(TiXmlElement("options"))->ToElement();
                tgtnode->SetAttribute("conn_type", (int)rd.connType);
                if (!rd.serialPort.IsEmpty())
                    tgtnode->SetAttribute("serial_port", cbU2C(rd.serialPort));
                if (!rd.serialBaud.IsEmpty())
                    tgtnode->SetAttribute("serial_baud", cbU2C(rd.serialBaud));
                if (!rd.ip.IsEmpty())
                    tgtnode->SetAttribute("ip_address", cbU2C(rd.ip));
                if (!rd.ipPort.IsEmpty())
                    tgtnode->SetAttribute("ip_port", cbU2C(rd.ipPort));
                if (!rd.additionalCmds.IsEmpty())
                    tgtnode->SetAttribute("additional_cmds", cbU2C(rd.additionalCmds));
                if (!rd.additionalCmdsBefore.IsEmpty())
                    tgtnode->SetAttribute("additional_cmds_before", cbU2C(rd.additionalCmdsBefore));
                if (rd.skipLDpath)
                    tgtnode->SetAttribute("skip_ld_path", "1");
                if (!rd.additionalShellCmdsAfter.IsEmpty())
                    tgtnode->SetAttribute("additional_shell_cmds_after", cbU2C(rd.additionalShellCmdsAfter));
                if (!rd.additionalShellCmdsBefore.IsEmpty())
                    tgtnode->SetAttribute("additional_shell_cmds_before", cbU2C(rd.additionalShellCmdsBefore));
            }
        }
    }
}

void DebuggerGDB::DoWatches()
{
    if (!m_pProcess)
        return;

    ConfigManager *config_manager = Manager::Get()->GetConfigManager(_T("debugger"));
    m_State.GetDriver()->UpdateWatches(config_manager->ReadBool(_T("watch_locals"), true),
                                       config_manager->ReadBool(_T("watch_args"), true),
                                       m_watches);
}

int DebuggerGDB::LaunchProcess(const wxString& cmd, const wxString& cwd)
{
    if (m_pProcess)
        return -1;

    // start the gdb process
    m_pProcess = new PipedProcess((void**)&m_pProcess, this, idGDBProcess, true, cwd);
    Manager::Get()->GetLogManager()->Log(_("Starting debugger: "), m_PageIndex);
    m_Pid = wxExecute(cmd, wxEXEC_ASYNC, m_pProcess);

#ifdef __WXMAC__
    if (m_Pid == -1)
    {
        // Great! We got a fake PID. Time to Go Fish with our "ps" rod:

        m_Pid = 0;
        pid_t mypid = getpid();
        wxString mypidStr;
        mypidStr << mypid;

        long pspid = 0;
        wxString psCmd;
        wxArrayString psOutput;
        wxArrayString psErrors;

        psCmd << wxT("/bin/ps -o ppid,pid,command");
        DebugLog(wxString::Format( _("Executing: %s"), psCmd.c_str()) );
        int result = wxExecute(psCmd, psOutput, psErrors, wxEXEC_SYNC);

        mypidStr << wxT(" ");

        for (int i = 0; i < psOutput.GetCount(); ++i)
        { //  PPID   PID COMMAND
           wxString psLine = psOutput.Item(i);
           if (psLine.StartsWith(mypidStr) && psLine.Contains(wxT("gdb")))
           {
               wxString pidStr = psLine.Mid(mypidStr.Length());
               pidStr = pidStr.BeforeFirst(' ');
               if (pidStr.ToLong(&pspid))
               {
                   m_Pid = pspid;
                   break;
               }
           }
         }

        for (int i = 0; i < psErrors.GetCount(); ++i)
            DebugLog(wxString::Format( _("PS Error:%s"), psErrors.Item(i).c_str()) );
    }
#endif

    if (!m_Pid)
    {
        delete m_pProcess;
        m_pProcess = 0;
        Manager::Get()->GetLogManager()->Log(_("failed"), m_PageIndex);
        return -1;
    }
    else if (!m_pProcess->GetOutputStream())
    {
        delete m_pProcess;
        m_pProcess = 0;
        Manager::Get()->GetLogManager()->Log(_("failed (to get debugger's stdin)"), m_PageIndex);
        return -2;
    }
    else if (!m_pProcess->GetInputStream())
    {
        delete m_pProcess;
        m_pProcess = 0;
        Manager::Get()->GetLogManager()->Log(_("failed (to get debugger's stdout)"), m_PageIndex);
        return -2;
    }
    else if (!m_pProcess->GetErrorStream())
    {
        delete m_pProcess;
        m_pProcess = 0;
        Manager::Get()->GetLogManager()->Log(_("failed (to get debugger's stderr)"), m_PageIndex);
        return -2;
    }
    Manager::Get()->GetLogManager()->Log(_("done"), m_PageIndex);
    return 0;
}

bool DebuggerGDB::IsStopped() const
{
    return !m_State.HasDriver() || m_State.GetDriver()->IsProgramStopped();
}


bool DebuggerGDB::Debug(bool breakOnEntry)
{
    // if already running, return
    if (m_pProcess || WaitingCompilerToFinish())
        return false;

    m_pProject = 0;
    m_NoDebugInfo = false;

    // clear the debug log
    if (m_HasDebugLog)
        Manager::Get()->GetDebuggerManager()->GetLogger(true)->Clear();

    ShowLog(true);

    // can only debug projects or attach to processes
    ProjectManager* prjMan = Manager::Get()->GetProjectManager();
    cbProject* project = prjMan->GetActiveProject();
    if (!project && m_PidToAttach == 0)
        return false;

    m_pProject = project;
    if (m_pProject && m_ActiveBuildTarget.IsEmpty())
        m_ActiveBuildTarget = m_pProject->GetActiveBuildTarget();

    m_Canceled = false;
    if (!EnsureBuildUpToDate(breakOnEntry ? StartTypeStepInto : StartTypeRun))
        return false;

    // if not waiting for the compiler, start debugging now
    // but first check if the driver has already been started:
    // if the build process was ultra-fast (i.e. nothing to be done),
    // it may have already called DoDebug() and m_WaitingCompilerToFinish
    // would already be set to false
    // by checking the driver availability, we avoid calling DoDebug
    // a second consecutive time...
    // the same applies for m_Canceled: it is true if DoDebug() was launched but
    // returned an error
    if (!WaitingCompilerToFinish() && !m_State.HasDriver() && !m_Canceled)
    {
        return DoDebug(breakOnEntry) == 0;
    }

    return true;
}

int DebuggerGDB::DoDebug(bool breakOnEntry)
{
    // set this to true before every error exit point in this function
    m_Canceled = false;

    LogManager* msgMan = Manager::Get()->GetLogManager();
    ProjectManager* prjMan = Manager::Get()->GetProjectManager();

    ShowLog(false);

    // select the build target to debug
    ProjectBuildTarget* target = 0;
    Compiler* actualCompiler = 0;
    if (m_PidToAttach == 0)
    {
        ShowLog(false);

        msgMan->Log(_("Selecting target: "), m_PageIndex);
        if (!m_pProject->BuildTargetValid(m_ActiveBuildTarget, false))
        {
            int tgtIdx = m_pProject->SelectTarget();
            if (tgtIdx == -1)
            {
                msgMan->Log(_("canceled"), m_PageIndex);
                m_Canceled = true;
                return 3;
            }
            target = m_pProject->GetBuildTarget(tgtIdx);
            m_ActiveBuildTarget = target->GetTitle();
        }
        else
            target = m_pProject->GetBuildTarget(m_ActiveBuildTarget);

        // make sure it's not a commands-only target
        if (target->GetTargetType() == ttCommandsOnly)
        {
            cbMessageBox(_("The selected target is only running pre/post build step commands\n"
                        "Can't debug such a target..."), _("Information"), wxICON_INFORMATION);
            msgMan->Log(_("aborted"), m_PageIndex);
            return 3;
        }
        msgMan->Log(target->GetTitle(), m_PageIndex);

        // find the target's compiler (to see which debugger to use)
        actualCompiler = CompilerFactory::GetCompiler(target ? target->GetCompilerID() : m_pProject->GetCompilerID());
    }
    else
        actualCompiler = CompilerFactory::GetDefaultCompiler();

    if (!actualCompiler)
    {
        wxString msg;
        msg.Printf(_("This %s is configured to use an invalid debugger.\nThe operation failed..."), target ? _("target") : _("project"));
        cbMessageBox(msg, _("Error"), wxICON_ERROR);
        m_Canceled = true;
        return 9;
    }

    // is gdb accessible, i.e. can we find it?
    wxString cmdexe;
    cmdexe = actualCompiler->GetPrograms().DBG;
    cmdexe.Trim();
    cmdexe.Trim(true);
    if(cmdexe.IsEmpty())
    {
        msgMan->Log(_("ERROR: You need to specify a debugger program in the compiler's settings."), m_PageIndex);

        if(platform::windows)
        {
            msgMan->Log(_("(For MinGW compilers, it's 'gdb.exe' (without the quotes))"), m_PageIndex);
            msgMan->Log(_("(For MSVC compilers, it's 'cdb.exe' (without the quotes))"), m_PageIndex);
        }
        else
        {
            msgMan->Log(_("(For GCC compilers, it's 'gdb' (without the quotes))"), m_PageIndex);
        }

        m_Canceled = true;
        return -1;
    }

    // access the gdb executable name
    cmdexe = FindDebuggerExecutable(actualCompiler);
    if (cmdexe.IsEmpty())
    {
        cbMessageBox(_("The debugger executable is not set.\n"
                       "To set it, go to \"Settings/Compiler and debugger\", switch to the\n"
                       "\"Toolchain executables\" tab, and select the debugger program."), _("Error"), wxICON_ERROR);
        msgMan->Log(_("Aborted"), m_PageIndex);
        m_Canceled = true;
        return 4;
    }

    // start debugger driver based on target compiler, or default compiler if no target
    if (!m_State.StartDriver(target))
    {
        cbMessageBox(_T("Could not decide which debugger to use!"), _T("Error"), wxICON_ERROR);
        m_Canceled = true;
        return -1;
    }

    // Notify debugger plugins so they could start a GDB server process
    PluginManager *plm = Manager::Get()->GetPluginManager();
    CodeBlocksEvent evt(cbEVT_DEBUGGER_STARTED);
    plm->NotifyPlugins(evt);
    int nRet = evt.GetInt();
    if (nRet < 0)
    {
        cbMessageBox(_T("A plugin interrupted the debug process."));
        msgMan->Log(_("Aborted by plugin"), m_PageIndex);
        m_Canceled = true;
        return -1;
    }
    // Continue

    // create gdb launch command
    wxString cmd;

    // prepare the driver
    wxString cmdline;
    if (m_PidToAttach == 0)
    {
        m_State.GetDriver()->ClearDirectories();
        // add other open projects dirs as search dirs (only if option is enabled)
        if (Manager::Get()->GetConfigManager(_T("debugger"))->ReadBool(_T("add_other_search_dirs"), false))
        {
            // add as include dirs all open project base dirs
            ProjectsArray* projects = prjMan->GetProjects();
            for (unsigned int i = 0; i < projects->GetCount(); ++i)
            {
                cbProject* it = projects->Item(i);
                // skip if it's THE project (added last)
                if (it == m_pProject)
                    continue;
                AddSourceDir(it->GetBasePath());
                AddSourceDir(it->GetCommonTopLevelPath());
            }
        }
        // now add all per-project user-set search dirs
        wxArrayString& pdirs = GetSearchDirs(m_pProject);
        for (size_t i = 0; i < pdirs.GetCount(); ++i)
        {
            AddSourceDir(pdirs[i]);
        }
        // lastly, add THE project as source dir
        AddSourceDir(m_pProject->GetBasePath());
        AddSourceDir(m_pProject->GetCommonTopLevelPath());

        // switch to output dir
        wxString path = UnixFilename(target->GetWorkingDir());
        if (!path.IsEmpty())
        {
            Manager::Get()->GetMacrosManager()->ReplaceEnvVars(path); // apply env vars
            cmd.Clear();
            ConvertToGDBDirectory(path);
            if (path != _T(".")) // avoid silly message "changing to ."
            {
                msgMan->Log(_("Changing directory to: ") + path, m_PageIndex);
                m_State.GetDriver()->SetWorkingDirectory(path);
            }
        }

        if (target && !target->GetExecutionParameters().IsEmpty())
            m_State.GetDriver()->SetArguments(target->GetExecutionParameters());

        // set the file to debug
        // (depends on the target type)
        wxString debuggee;
        if (!GetDebuggee(debuggee, target))
        {
            m_Canceled = true;
            return -3;
        }
        cmdline = m_State.GetDriver()->GetCommandLine(cmdexe, debuggee);
    }
    else // m_PidToAttach != 0
        cmdline = m_State.GetDriver()->GetCommandLine(cmdexe, m_PidToAttach);
    m_State.GetDriver()->SetTarget(target);

    RemoteDebuggingMap& rdprj = GetRemoteDebuggingMap();
    RemoteDebugging rd = rdprj[0]; // project settings
    RemoteDebuggingMap::iterator it = rdprj.find(target); // target settings
    if (it != rdprj.end())
        rd.MergeWith(it->second);
//////////////////killerbot : most probably here : execute the shell commands (we could access the per target debugger settings)
    wxString oldLibPath; // keep old PATH/LD_LIBRARY_PATH contents
    if (!rd.skipLDpath)
    {
        wxGetEnv(LIBRARY_ENVVAR, &oldLibPath);

        // setup dynamic linker path
        if (actualCompiler && target)
        {
            wxString newLibPath;
            const wxString libPathSep = platform::windows ? _T(";") : _T(":");
            newLibPath << _T(".") << libPathSep;
            newLibPath << GetStringFromArray(actualCompiler->GetLinkerSearchDirs(target), libPathSep);
            if (newLibPath.Mid(newLibPath.Length() - 1, 1) != libPathSep)
                newLibPath << libPathSep;
            newLibPath << oldLibPath;
            wxSetEnv(LIBRARY_ENVVAR, newLibPath);
            DebugLog(LIBRARY_ENVVAR _T("=") + newLibPath);
        }
    }

    #ifdef __WXMSW__
    if (!m_State.GetDriver()->UseDebugBreakProcess())
    {
        AllocConsole();
        SetConsoleTitleA("Codeblocks debug console - DO NOT CLOSE!");
        SetConsoleCtrlHandler(HandlerRoutine, TRUE);
        m_bIsConsole = true;

        HWND windowHandle = GetConsoleWindow();
        if (windowHandle)
            ShowWindow(windowHandle, SW_HIDE);
    }
    #endif
    // start the gdb process
    wxString wdir = m_pProject ? m_pProject->GetBasePath() : _T(".");
    DebugLog(_T("Command-line: ") + cmdline);
    DebugLog(_T("Working dir : ") + wdir);
    int ret = LaunchProcess(cmdline, wdir);

    if (!rd.skipLDpath)
    {
        // restore dynamic linker path
        wxSetEnv(LIBRARY_ENVVAR, oldLibPath);
    }

    if (ret != 0)
    {
        m_Canceled = true;
        return ret;
    }

    wxString out;
    // start polling gdb's output
    m_TimerPollDebugger.Start(20);

    // although I don't really like these do-nothing loops, we must wait a small amount of time
    // for gdb to see if it really started: it may fail to load shared libs or whatever
    // the reason this is added is because I had a case where gdb would error and bail out
    // *while* the driver->Prepare() call was running below and hell broke loose...
    int i = 50;
    while (i)
    {
        wxMilliSleep(1);
        Manager::Yield();
        --i;
    }
    if (!m_State.HasDriver())
        return -1;

    m_State.GetDriver()->Prepare(target && target->GetTargetType() == ttConsoleOnly);
    m_State.ApplyBreakpoints();

   #ifndef __WXMSW__
    // create xterm and issue tty "/dev/pts/#" to GDB where
    // # is the tty for the newly created xterm
    m_bIsConsole = target && target->GetUseConsoleRunner();
    if (m_bIsConsole)
    {
        wxString consoleTty;
        m_nConsolePid = RunNixConsole(consoleTty);
        if (m_nConsolePid > 0)
        {   wxString gdbTtyCmd;
            gdbTtyCmd << wxT("tty ") << consoleTty;
            m_State.GetDriver()->QueueCommand(new DebuggerCmd(m_State.GetDriver(), gdbTtyCmd, true));
            DebugLog(wxString::Format( _("Queued:[%s]"), gdbTtyCmd.c_str()) );
        }
    }//if
   #endif//ndef __WXMSW__

    // Don't issue 'run' if attaching to a process (Bug #1391904)
    if (m_PidToAttach == 0)
        m_State.GetDriver()->Start(breakOnEntry);
    else
        m_State.GetDriver()->Attach(m_PidToAttach);

    // switch to the user-defined layout for debugging
    if (m_pProcess)
        SwitchToDebuggingLayout();

    return 0;
} // Debug

void DebuggerGDB::AddSourceDir(const wxString& dir)
{
    if (!m_State.HasDriver() || dir.IsEmpty())
        return;
    wxString filename = dir;
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(filename); // apply env vars
    Manager::Get()->GetLogManager()->Log(_("Adding source dir: ") + filename, m_PageIndex);
    ConvertToGDBDirectory(filename, _T(""), false);
    m_State.GetDriver()->AddDirectory(filename);
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
void DebuggerGDB::ConvertToGDBFile(wxString& str)
{
    wxFileName fname = str;
    str = fname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    DebuggerGDB::ConvertToGDBDirectory(str);
    str << fname.GetFullName();
}

void DebuggerGDB::ConvertDirectory(wxString& str, wxString base, bool relative)
{
    ConvertToGDBDirectory(str, base, relative);
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

    if(platform::windows)
    {
        int ColonLocation = str.Find(_T(':'));
        wxChar buf[255];
        if(ColonLocation != -1)
        {
            //If can, get 8.3 name for path (Windows only)
            if (str.Contains(_T(' '))) // only if has spaces
            {
                GetShortPathName(str.c_str(), buf, 255);
                str = buf;
            }
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
            if (base.Contains(_T(' '))) // only if has spaces
            {
                GetShortPathName(base.c_str(), buf, 255);
                str = buf;
            }
        }

        if(ColonLocation == -1 || base.IsEmpty())
            relative = false;        //Can't do it
    }
    else
    {
        if((str.GetChar(0) != _T('/') && str.GetChar(0) != _T('~')) || base.IsEmpty())
            relative = false;
    }

    if(relative)
    {
        if(platform::windows)
        {
            if(str.Find(_T(':')) != -1) str = str.Mid(str.Find(_T(':')) + 2, str.Length());
            if(base.Find(_T(':')) != -1) base = base.Mid(base.Find(_T(':')) + 2, base.Length());
        }
        else
        {
            if(str.GetChar(0) == _T('/')) str = str.Mid(1, str.Length());
            else if(str.GetChar(0) == _T('~')) str = str.Mid(2, str.Length());
            if(base.GetChar(0) == _T('/')) base = base.Mid(1, base.Length());
            else if(base.GetChar(0) == _T('~')) base = base.Mid(2, base.Length());
        }

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

void DebuggerGDB::SendCommand(const wxString& cmd, bool debugLog)
{
    if (!debugLog)
        Manager::Get()->GetLogManager()->Log(_T("> ") + cmd, m_PageIndex);

    if (debugLog)
        DoSendCommand(cmd);
    else if (m_State.HasDriver())
        m_State.GetDriver()->QueueCommand(new DebuggerCmd(m_State.GetDriver(), cmd, true));
}

void DebuggerGDB::DoSendCommand(const wxString& cmd)
{
    if (!m_pProcess || !IsStopped())
        return;

    if (m_HasDebugLog)
        Manager::Get()->GetLogManager()->Log(_T("> ") + cmd, m_DbgPageIndex);

    m_pProcess->SendString(cmd);
}

void DebuggerGDB::RequestUpdate(DebugWindows window)
{
    switch (window)
    {
        case Backtrace:
            RunCommand(CMD_BACKTRACE);
            break;
        case CPURegisters:
            RunCommand(CMD_REGISTERS);
            break;
        case Disassembly:
            RunCommand(CMD_DISASSEMBLE);
            break;
        case ExamineMemory:
            RunCommand(CMD_MEMORYDUMP);
            break;
        case Threads:
            RunCommand(CMD_RUNNINGTHREADS);
            break;
        case Watches:
            if (IsWindowReallyShown(Manager::Get()->GetDebuggerManager()->GetWatchesDialog()))
                DoWatches();
            break;
    }
}

void DebuggerGDB::RunCommand(int cmd)
{
    // just check for the process
    if (!m_pProcess)
        return;

    switch (cmd)
    {
        case CMD_CONTINUE:
        {
            ClearActiveMarkFromAllEditors();
            if (m_State.HasDriver())
            {
                Manager::Get()->GetLogManager()->Log(_("Continuing..."), m_PageIndex);
                m_State.GetDriver()->Continue();
                m_State.GetDriver()->ResetCurrentFrame();
            }
            break;
        }

        case CMD_STEP:
        {
            ClearActiveMarkFromAllEditors();
            if (m_State.HasDriver())
            {
                m_State.GetDriver()->Step();
                m_State.GetDriver()->ResetCurrentFrame();
            }
            break;
        }

        case CMD_STEP_INSTR:
        {
            ClearActiveMarkFromAllEditors();
            if (!Manager::Get()->GetDebuggerManager()->UpdateDisassembly())
            {
                // first time users should have some help from us ;)
                RunCommand(CMD_DISASSEMBLE);
            }
            if (m_State.HasDriver())
            {
                m_State.GetDriver()->StepInstruction();
                m_State.GetDriver()->ResetCurrentFrame();
                m_State.GetDriver()->NotifyCursorChanged();
            }
            break;
        }

        case CMD_STEP_INTO_INSTR:
        {
            ClearActiveMarkFromAllEditors();
            if (!Manager::Get()->GetDebuggerManager()->UpdateDisassembly())
            {
                // first time users should have some help from us ;)
                RunCommand(CMD_DISASSEMBLE);
            }
            if (m_State.HasDriver())
            {
                m_State.GetDriver()->StepIntoInstruction();
                m_State.GetDriver()->ResetCurrentFrame();
                m_State.GetDriver()->NotifyCursorChanged();
            }
            break;
        }

        case CMD_STEPIN:
        {
            ClearActiveMarkFromAllEditors();
            if (m_State.HasDriver())
            {
                m_State.GetDriver()->StepIn();
                m_State.GetDriver()->ResetCurrentFrame();
            }
            break;
        }

        case CMD_STEPOUT:
        {
            ClearActiveMarkFromAllEditors();
            if (m_State.HasDriver())
            {
                m_State.GetDriver()->StepOut();
                m_State.GetDriver()->ResetCurrentFrame();
            }
            break;
        }

        case CMD_STOP:
        {
            ClearActiveMarkFromAllEditors();
            if (m_State.HasDriver())
            {
                m_State.GetDriver()->Stop();
                m_State.GetDriver()->ResetCurrentFrame();
                MarkAsStopped();
            }
            break;
        }

        case CMD_BACKTRACE:
        {
            if (m_State.HasDriver())
                m_State.GetDriver()->Backtrace();
            break;
        }

        case CMD_DISASSEMBLE:
        {
            if (m_State.HasDriver())
                m_State.GetDriver()->Disassemble();
            break;
        }

        case CMD_REGISTERS:
        {
            if (m_State.HasDriver())
                m_State.GetDriver()->CPURegisters();
            break;
        }

        case CMD_MEMORYDUMP:
        {
            if (m_State.HasDriver())
                m_State.GetDriver()->MemoryDump();
            break;
        }

        case CMD_RUNNINGTHREADS:
        {
            if (m_State.HasDriver())
                m_State.GetDriver()->RunningThreads();
            break;
        }

        default: break;
    }
}

int DebuggerGDB::GetStackFrameCount() const
{
    return m_State.GetDriver()->GetStackFrames().size();
}

const cbStackFrame& DebuggerGDB::GetStackFrame(int index) const
{
    return m_State.GetDriver()->GetStackFrames()[index];
}

void DebuggerGDB::SwitchToFrame(int number)
{
    if (m_State.HasDriver())
    {
        m_State.GetDriver()->SetCurrentFrame(number, true);
        m_State.GetDriver()->SwitchToFrame(number);

        if(Manager::Get()->GetDebuggerManager()->UpdateBacktrace())
           Manager::Get()->GetDebuggerManager()->GetBacktraceDialog()->Reload();
    }
}

int DebuggerGDB::GetActiveStackFrame() const
{
    return m_State.HasDriver() ? m_State.GetDriver()->GetCurrentFrame() : 0;
}

int DebuggerGDB::GetThreadsCount() const
{
    if (!m_State.HasDriver())
        return 0;
    else
        return m_State.GetDriver()->GetThreads().size();
}

const cbThread& DebuggerGDB::GetThread(int index) const
{
    return m_State.GetDriver()->GetThreads()[index];
}

bool DebuggerGDB::SwitchToThread(int thread_number)
{
    if (!m_State.HasDriver())
        return false;
    DebuggerDriver *driver = m_State.GetDriver();
    DebuggerDriver::ThreadsContainer const &threads = driver->GetThreads();

    for (DebuggerDriver::ThreadsContainer::const_iterator it = threads.begin(); it != threads.end(); ++it)
    {
        if (it->GetNumber() == thread_number)
        {
            if(!it->IsActive())
            {
                driver->SwitchThread(thread_number);
            }
            return true;
        }
    }
    return false;
}

cbBreakpoint* DebuggerGDB::AddBreakpoint(const wxString& filename, int line)
{
    bool debuggerIsRunning = !IsStopped();
    if (debuggerIsRunning)
        DoBreak(true);

    DebuggerBreakpoint *bp = m_State.AddBreakpoint(filename, line, false);

    if (debuggerIsRunning)
        Continue();

    BreakItem item;
    item.cb_break = std::tr1::shared_ptr<cbBreakpoint>(new cbBreakpoint(filename, line));
    item.debugger_breakpoint = bp;
    m_breakpoints.push_back(item);

    item.cb_break->SetCondition(bp->condition);
    item.cb_break->SetIgnoreCount(bp->ignoreCount);
    item.cb_break->SetEnabled(bp->enabled);
    item.cb_break->SetUseCondition(bp->useCondition);
    item.cb_break->SetUseIgnoreCount(bp->useIgnoreCount);
    return item.cb_break.get();
}

cbBreakpoint* DebuggerGDB::AddDataBreakpoint(const wxString& dataExpression)
{
    DataBreakpointDlg dlg(Manager::Get()->GetAppWindow(), dataExpression, true, 1);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        const wxString& newDataExpression = dlg.GetDataExpression();
        int sel = dlg.GetSelection();
        DebuggerBreakpoint *bp = m_State.AddBreakpoint(newDataExpression, sel != 1, sel != 0);

        BreakItem item;
        cbBreakpoint *temp = new cbBreakpoint(bp->breakAddress, bp->breakOnRead, bp->breakOnWrite);
        item.cb_break = std::tr1::shared_ptr<cbBreakpoint>(temp);
        item.debugger_breakpoint = bp;
        m_breakpoints.push_back(item);

        item.cb_break->SetEnabled(bp->enabled);
        return item.cb_break.get();
    }
    else
        return NULL;
}

int DebuggerGDB::GetBreakpointsCount() const
{
    return m_breakpoints.size();
}

cbBreakpoint* DebuggerGDB::GetBreakpoint(int index)
{
    return m_breakpoints[index].cb_break.get();
}

const cbBreakpoint* DebuggerGDB::GetBreakpoint(int index) const
{
    return m_breakpoints[index].cb_break.get();
}

void DebuggerGDB::UpdateBreakpoint(cbBreakpoint *breakpoint)
{
    int index = 0;
    for (BreakpointsContainer::iterator it = m_breakpoints.begin(); it != m_breakpoints.end(); ++it, ++index)
    {
        if (it->cb_break.get() == breakpoint)
        {
            switch (breakpoint->GetType())
            {
                case cbBreakpoint::Code:
                {
                    cbBreakpoint temp(*breakpoint);
                    EditBreakpointDlg dlg(&temp, Manager::Get()->GetAppWindow());
                    PlaceWindow(&dlg);
                    if (dlg.ShowModal() == wxID_OK)
                    {
                        DebuggerBreakpoint *bp = it->debugger_breakpoint;
                        bp->enabled = temp.IsEnabled();
                        bp->filename = temp.GetFilename();
                        bp->line = temp.GetLine() - 1;
                        bp->ignoreCount = temp.GetIgnoreCount();
                        bp->useIgnoreCount = temp.UseIgnoreCount();
                        bp->condition = temp.GetCondition();
                        bp->useCondition = temp.UseCondition();
                        *breakpoint = temp;

                        m_State.ResetBreakpoint(index);
                    }
                    break;
                }
                case cbBreakpoint::Data:
                {
                    DebuggerBreakpoint *bp = it->debugger_breakpoint;
                    int old_sel = 0;
                    if (bp->breakOnRead && bp->breakOnWrite)
                        old_sel = 2;
                    else if (!bp->breakOnRead && bp->breakOnWrite)
                        old_sel = 1;
                    DataBreakpointDlg dlg(Manager::Get()->GetAppWindow(),
                                          it->cb_break->GetDataExpression(),
                                          bp->enabled, old_sel);
                    PlaceWindow(&dlg);
                    if (dlg.ShowModal() == wxID_OK)
                    {
                        bp->enabled = dlg.IsEnabled();
                        bp->breakOnRead = dlg.GetSelection() != 1;
                        bp->breakOnWrite = dlg.GetSelection() != 0;

                        *it->cb_break = cbBreakpoint(it->cb_break->GetDataExpression(),
                                                     bp->breakOnRead, bp->breakOnWrite);
                        it->cb_break->SetEnabled(bp->enabled);

                        m_State.ResetBreakpoint(index);
                    }
                    break;
                }
            }
            return;
        }
    }
}

void DebuggerGDB::DeleteBreakpoint(cbBreakpoint* breakpoint)
{
    for (BreakpointsContainer::iterator it = m_breakpoints.begin(); it != m_breakpoints.end(); ++it)
    {
        if (it->cb_break.get() == breakpoint)
        {
            bool debuggerIsRunning = !IsStopped();
            if (debuggerIsRunning)
                DoBreak(true);

            m_State.RemoveBreakpoint(it->debugger_breakpoint, true);
            m_breakpoints.erase(it);

            if (debuggerIsRunning)
                Continue();
            return;
        }
    }
}

void DebuggerGDB::DeleteAllBreakpoints()
{
    bool debuggerIsRunning = !IsStopped();
    if (debuggerIsRunning)
        DoBreak(true);
    m_State.RemoveAllBreakpoints(wxEmptyString, true);
    m_breakpoints.clear();

    if (debuggerIsRunning)
        Continue();
}

void DebuggerGDB::ShiftBreakpoint(int index, int lines_to_shift)
{
    BreakpointsContainer::iterator it = m_breakpoints.begin();
    std::advance(it, index);
    if(it != m_breakpoints.end())
    {
        m_State.ShiftBreakpoint(it->debugger_breakpoint, lines_to_shift);
        it->cb_break->SetLine(it->cb_break->GetLine() + lines_to_shift);
    }
}

struct TestIfBelongToProject
{
    TestIfBelongToProject(cbProject *project) :
        m_project(project)
    {
    }

    bool operator()(DebuggerGDB::BreakItem const &item) const
    {
        return item.debugger_breakpoint->userData == m_project;
    }

    cbProject* m_project;
};

void DebuggerGDB::DeleteAllProjectBreakpoints(cbProject* project)
{
    BreakpointsContainer::iterator new_last = std::remove_if(m_breakpoints.begin(),
                                                             m_breakpoints.end(),
                                                             TestIfBelongToProject(project));
    m_breakpoints.erase(new_last, m_breakpoints.end());
    m_State.RemoveAllProjectBreakpoints(project);
}

void DebuggerGDB::Continue()
{
    RunCommand(CMD_CONTINUE);
}

void DebuggerGDB::Next()
{
    RunCommand(CMD_STEP);
}

void DebuggerGDB::NextInstruction()
{
    RunCommand(CMD_STEP_INSTR);
}

void DebuggerGDB::StepIntoInstruction()
{
    RunCommand(CMD_STEP_INTO_INSTR);
}

void DebuggerGDB::Step()
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

void DebuggerGDB::StepOut()
{
    RunCommand(CMD_STEPOUT);
}

bool DebuggerGDB::RunToCursor(const wxString& filename, int line, const wxString& line_text)
{
    if (m_pProcess)
    {
        m_State.AddBreakpoint(filename, line, true, line_text);
        Continue();
        return true;
    }
    else
    {
        if (!Manager::Get()->GetConfigManager(_T("debugger"))->ReadBool(_T("do_not_run"), false))
            m_State.AddBreakpoint(filename, line, true, line_text);
        return Debug(false);
    }
}

void DebuggerGDB::SetNextStatement(const wxString& filename, int line)
{
    if (m_State.HasDriver() && IsStopped())
    {
        m_State.GetDriver()->SetNextStatement(filename, line);
    }
}

void DebuggerGDB::Break()
{
    DoBreak(false);
}

void DebuggerGDB::DoBreak(bool temporary)
{
    m_TemporaryBreak = temporary;

    // m_Process is PipedProcess I/O; m_Pid is debugger pid
    if (m_pProcess && m_Pid && !IsStopped())
    {
        long pid = m_State.GetDriver()->GetChildPID();
    #ifndef __WXMSW__
        if (pid <= 0)
            pid = m_Pid; // try poking gdb directly
        // non-windows gdb can interrupt the running process. yay!
        if (pid <= 0) // look out for the "fake" PIDs (killall)
            cbMessageBox(_("Unable to stop the debug process!"), _("Error"), wxOK | wxICON_WARNING);
        else
            wxKill(pid, wxSIGINT);
    #else
        // windows gdb can interrupt the running process too. yay!
        if (   (pid <=0)
            && (CreateToolhelp32SnapshotFunc!=NULL)
            && (Process32FirstFunc!=NULL)
            && (Process32NextFunc!=NULL) )
        {
            HANDLE snap = CreateToolhelp32SnapshotFunc(TH32CS_SNAPALL,0);
            if (snap!=INVALID_HANDLE_VALUE)
            {
                PROCESSENTRY32 lppe;
                lppe.dwSize = sizeof(PROCESSENTRY32);
                BOOL ok = Process32FirstFunc(snap, &lppe);
                while ( ok == TRUE)
                {
                    if (static_cast<int>(lppe.th32ParentProcessID) == m_Pid) // Have my Child...
                    {
                        pid = lppe.th32ProcessID;
                    }
                    lppe.dwSize = sizeof(PROCESSENTRY32);
                    ok = Process32NextFunc(snap, &lppe);
                }
                CloseHandle(snap);
            }
            else
                Log(_("No handle created. Trying to pause directly with cbd.exe..."));
        }

        bool done = false;
        if (m_State.GetDriver()->UseDebugBreakProcess())
        {
            if (!DebugBreakProcessFunc)
                Log(_("DebugBreakProcess is not supported, you need Windows XP or newer..."));
            else if (pid > 0)
            {
                Log(_("Trying to pause the running process..."));
                HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);
                if (proc)
                {
                    DebugBreakProcessFunc(proc); // yay!
                    CloseHandle(proc);
                    done = true;
                }
                else
                    Log(_("Failed."));
            }
        }
        else
        {
            if (m_Pid > 0)
            {
                if (GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0) == 0)
                {
                    Log(wxT("Interupting debugger failed :("));
                    DebugLog(wxT("GenerateConsoleCtrlEvent failed :("));
                    return;
                }
            }
        }
    #endif
        // Notify debugger plugins for end of debug session
        PluginManager *plm = Manager::Get()->GetPluginManager();
        CodeBlocksEvent evt(cbEVT_DEBUGGER_PAUSED);
        plm->NotifyPlugins(evt);
    }
}

void DebuggerGDB::Stop()
{
    // m_Process is PipedProcess I/O; m_Pid is debugger pid
    if (m_pProcess && m_Pid)
    {
        if (!IsStopped())
            Break();
        RunCommand(CMD_STOP);
    }
}

void DebuggerGDB::ParseOutput(const wxString& output)
{
    if (!output.IsEmpty() && m_State.HasDriver())
    {
        m_State.GetDriver()->ParseOutput(output);
    }
}

void DebuggerGDB::GetCurrentPosition(wxString &filename, int &line)
{
    if (m_State.HasDriver())
    {
        const Cursor& cursor = m_State.GetDriver()->GetCursor();
        filename = cursor.file;
        line = cursor.line;
    }
    else
    {
        filename = wxEmptyString;
        line = -1;
    }
}

void DebuggerGDB::OnAddSymbolFile(wxCommandEvent& WXUNUSED(event))
{
    wxString file = wxFileSelector(_("Choose file to read symbols from"),
                                    _T(""),
                                    _T(""),
                                    _T(""),
                                    _("Executables and libraries|*.exe;*.dll"),
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST | compatibility::wxHideReadonly);
    if (file.IsEmpty())
        return;
//    Manager::Get()->GetLogManager()->Log(m_PageIndex, _("Adding symbol file: %s"), file.c_str());
    ConvertToGDBDirectory(file);
//    QueueCommand(new DbgCmd_AddSymbolFile(this, file));
}

void DebuggerGDB::ShowToolMenu()
{
    wxMenu m;
    m.Append(idMenuInfoFrame,   _("Current stack frame"));
    m.Append(idMenuInfoDLL,     _("Loaded libraries"));
    m.Append(idMenuInfoFiles,   _("Targets and files"));
    m.Append(idMenuInfoFPU,     _("FPU status"));
    m.Append(idMenuInfoSignals, _("Signal handling"));
    Manager::Get()->GetAppWindow()->PopupMenu(&m);
}

void DebuggerGDB::OnInfoFrame(wxCommandEvent& WXUNUSED(event))
{
    if (m_State.HasDriver())
    {
        m_State.GetDriver()->InfoFrame();
    }
}

void DebuggerGDB::OnInfoDLL(wxCommandEvent& WXUNUSED(event))
{
    if (m_State.HasDriver())
    {
        m_State.GetDriver()->InfoDLL();
    }
}

void DebuggerGDB::OnInfoFiles(wxCommandEvent& WXUNUSED(event))
{
    if (m_State.HasDriver())
    {
        m_State.GetDriver()->InfoFiles();
    }
}

void DebuggerGDB::OnInfoFPU(wxCommandEvent& WXUNUSED(event))
{
    if (m_State.HasDriver())
    {
        m_State.GetDriver()->InfoFPU();
    }
}

void DebuggerGDB::OnInfoSignals(wxCommandEvent& WXUNUSED(event))
{
    if (m_State.HasDriver())
    {
        m_State.GetDriver()->InfoSignals();
    }
}

void DebuggerGDB::OnGDBOutput(wxCommandEvent& event)
{
    wxString msg = event.GetString();
    if (!msg.IsEmpty())
    {
//        Manager::Get()->GetLogManager()->Log(m_PageIndex, _T("O>>> %s"), msg.c_str());
        ParseOutput(msg);
    }
}

void DebuggerGDB::OnGDBError(wxCommandEvent& event)
{
    wxString msg = event.GetString();
    if (!msg.IsEmpty())
    {
//        Manager::Get()->GetLogManager()->Log(m_PageIndex, _T("E>>> %s"), msg.c_str());
        ParseOutput(msg);
    }
}

void DebuggerGDB::OnGDBTerminated(wxCommandEvent& event)
{
    m_PidToAttach = 0;

    m_TimerPollDebugger.Stop();
    m_LastExitCode = event.GetInt();
    //the process deletes itself
//    m_pProcess = 0L;

    ClearActiveMarkFromAllEditors();
    m_State.StopDriver();
    Manager::Get()->GetLogManager()->Log(F(_("Debugger finished with status %d"), m_LastExitCode), m_PageIndex);

    if (m_NoDebugInfo)
    {
        cbMessageBox(_("This project/target has no debugging info."
                        "Please change this in the project's build options and retry..."),
                        _("Error"),
                        wxICON_STOP);
    }

    // Notify debugger plugins for end of debug session
    PluginManager *plm = Manager::Get()->GetPluginManager();
    CodeBlocksEvent evt(cbEVT_DEBUGGER_FINISHED);
    plm->NotifyPlugins(evt);

    // switch to the user-defined layout when finished debugging
    SwitchToPreviousLayout();
    KillConsole();
    MarkAsStopped();

    ///killerbot : run there the post shell commands ?
}

void DebuggerGDB::KillConsole()
{
#ifdef __WXMSW__
    if (m_bIsConsole)
    {
        // remove the CTRL_C handler
        SetConsoleCtrlHandler(HandlerRoutine, FALSE);
        FreeConsole();
        m_bIsConsole = false;
    }
#else
    // kill any linux console
    if ( m_bIsConsole && (m_nConsolePid > 0) )
    {
        ::wxKill(m_nConsolePid);
        m_nConsolePid = 0;
        m_bIsConsole = false;
    }
#endif
}

void DebuggerGDB::OnValueTooltip(CodeBlocksEvent& event)
{
    event.Skip();
    if (!m_pProcess || !IsStopped())
        return;

    if (!m_State.HasDriver() || !m_State.GetDriver()->IsDebuggingStarted())
        return;

    if (!Manager::Get()->GetConfigManager(_T("debugger"))->ReadBool(_T("eval_tooltip"), false))
        return;

    if (DragInProgress())
        return;

    EditorBase* base = event.GetEditor();
    cbEditor* ed = base && base->IsBuiltinEditor() ? static_cast<cbEditor*>(base) : 0;
    if (!ed)
        return;

    if (ed->IsContextMenuOpened())
        return;

    // get rid of other calltips (if any) [for example the code completion one, at this time we
    // want the debugger value call/tool-tip to win and be shown]
    if (ed->GetControl()->CallTipActive())
        ed->GetControl()->CallTipCancel();

    const int style = event.GetInt();
    if (style != wxSCI_C_DEFAULT && style != wxSCI_C_OPERATOR && style != wxSCI_C_IDENTIFIER)
        return;

    wxPoint pt;
    pt.x = event.GetX();
    pt.y = event.GetY();

    const wxString &token = GetEditorWordAtCaret(&pt);
    if (!token.empty())
    {
        pt = ed->GetControl()->ClientToScreen(pt);
        m_EvalRect.x = pt.x - 5;
        m_EvalRect.y = pt.y - 5;
        m_EvalRect.height = m_EvalRect.width = 10;
        m_State.GetDriver()->EvaluateSymbol(token, m_EvalRect);
    }
}

void DebuggerGDB::CleanupWhenProjectClosed(cbProject *project)
{
    // remove all search dirs stored for this project so we don't have conflicts
    // if a newly opened project happens to use the same memory address
    GetSearchDirs(project).clear();

    // the same for remote debugging
    GetRemoteDebuggingMap(project).clear();

    // remove all breakpoints belonging to the closed project
    DeleteAllProjectBreakpoints(project);
    cbBreakpointsDlg *dlg = Manager::Get()->GetDebuggerManager()->GetBreakpointDialog();
    dlg->Reload();
}

void DebuggerGDB::OnIdle(wxIdleEvent& event)
{
    if (m_pProcess && ((PipedProcess*)m_pProcess)->HasInput())
        event.RequestMore();
    else
        event.Skip();
}

void DebuggerGDB::OnTimer(wxTimerEvent& event)
{
    // send any buffered (previous) output
    ParseOutput(wxEmptyString);

    wxWakeUpIdle();
}

void DebuggerGDB::OnShowFile(wxCommandEvent& event)
{
    SyncEditor(event.GetString(), event.GetInt(), false);
}

void DebuggerGDB::DebuggeeContinued()
{
    m_TemporaryBreak = false;
}

void DebuggerGDB::OnCursorChanged(wxCommandEvent& WXUNUSED(event))
{
    if(m_TemporaryBreak)
        return;

    if (m_State.HasDriver())
    {
        const Cursor& cursor = m_State.GetDriver()->GetCursor();
        // checking if driver is stopped is redundant because it would only
        // send us this event if it was stopped anyway
        if (/*m_State.GetDriver()->IsStopped() &&*/ cursor.changed)
        {
            bool autoSwitch = Manager::Get()->GetConfigManager(_T("debugger"))->ReadBool(_T("auto_switch_frame"), true);

            MarkAllWatchesAsUnchanged();

            // if the cursor line is invalid and the auto switch is on,
            // we don't sync the editor, because there is no line to sync to
            // and also we are going to execute a backtrace command hoping to find a valid frame.
            if (!autoSwitch || cursor.line != -1)
                SyncEditor(cursor.file, cursor.line);

            BringCBToFront();
            if (cursor.line != -1)
                Log(wxString::Format(_("At %s:%d"), cursor.file.c_str(), cursor.line));
            else
                Log(wxString::Format(_("In %s (%s)"), cursor.function.c_str(), cursor.file.c_str()));

            // update watches
            DebuggerManager *dbg_manager = Manager::Get()->GetDebuggerManager();

            if (IsWindowReallyShown(dbg_manager->GetWatchesDialog()))
                DoWatches();

            // update CPU registers
            if (dbg_manager->UpdateCPURegisters())
                RunCommand(CMD_REGISTERS);

            // update callstack
            if (dbg_manager->UpdateBacktrace())
                RunCommand(CMD_BACKTRACE);
            else
            {
                if (cursor.line == -1 && autoSwitch)
                    RunCommand(CMD_BACKTRACE);
            }

            // update disassembly
            if (dbg_manager->UpdateDisassembly())
            {
                unsigned long int addrL;
                cursor.address.ToULong(&addrL, 16);
                //if zero addr, don't attempt disassembly
                if (addrL && !dbg_manager->GetDisassemblyDialog()->SetActiveAddress(addrL))
                    RunCommand(CMD_DISASSEMBLE);
            }

            // update memory examiner
            if (dbg_manager->UpdateExamineMemory())
                RunCommand(CMD_MEMORYDUMP);

            // update running threads
            if (dbg_manager->UpdateThreads())
                RunCommand(CMD_RUNNINGTHREADS);
        }
    }
}

cbWatch* DebuggerGDB::AddWatch(const wxString& symbol)
{
    GDBWatch *watch = new GDBWatch(symbol);
    m_watches.push_back(std::tr1::shared_ptr<GDBWatch>(watch));

    if(m_pProcess)
        m_State.GetDriver()->UpdateWatch(m_watches.back());

    return watch;
}

void DebuggerGDB::DeleteWatch(cbWatch *watch)
{
    for(WatchesContainer::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        if(it->get() == watch)
        {
            m_watches.erase(it);
            return;
        }
    }
}

bool DebuggerGDB::HasWatch(cbWatch *watch)
{
    for(WatchesContainer::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        if(it->get() == watch)
            return true;
    }
    return false;
}

void DebuggerGDB::ShowWatchProperties(cbWatch *watch)
{
    // not supported for child nodes!
    if(watch->GetParent())
        return;

    GDBWatch *real_watch = static_cast<GDBWatch*>(watch);
    EditWatchDlg dlg(real_watch);
    if(dlg.ShowModal() == wxID_OK)
        DoWatches();
}

bool DebuggerGDB::SetWatchValue(cbWatch *watch, const wxString &value)
{
    cbWatch *root_watch = GetRootWatch(watch);
    bool found = false;
    for (WatchesContainer::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        if (it->get() == root_watch)
        {
            found = true;
            break;
        }
    }
    if(!found)
        return false;

    if (!m_State.HasDriver())
        return false;

    wxString full_symbol;
    cbWatch *temp_watch = watch;
    while(temp_watch)
    {
        wxString symbol;
        temp_watch->GetSymbol(symbol);
        temp_watch = temp_watch->GetParent();

        if(symbol.find(wxT('*')) != wxString::npos || symbol.find(wxT('&')) != wxString::npos)
            symbol = wxT('(') + symbol + wxT(')');

        if(full_symbol.empty())
            full_symbol = symbol;
        else
            full_symbol = symbol + wxT('.') + full_symbol;
    }

    DebuggerDriver* driver = m_State.GetDriver();
    driver->SetVarValue(full_symbol, value);
    DoWatches();
    return true;
}

void DebuggerGDB::ExpandWatch(cbWatch *watch)
{
}

void DebuggerGDB::CollapseWatch(cbWatch *watch)
{
}

void DebuggerGDB::MarkAllWatchesAsUnchanged()
{
    for(WatchesContainer::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
        (*it)->MarkAsChangedRecursive(false);
}

void DebuggerGDB::OnWatchesContextMenu(wxMenu &menu, const cbWatch &watch, wxObject *property)
{
    wxString type, symbol;
    watch.GetType(type);
    watch.GetSymbol(symbol);

    type.Trim(true);
    type.Trim(false);

    if (type.EndsWith(wxT("*")))
    {
        menu.InsertSeparator(0);
        menu.Insert(0, idMenuWatchDereference, _("Dereference ") + symbol);
        m_watchToDereferenceSymbol = symbol;
        m_watchToDereferenceProperty = property;
    }
}

void DebuggerGDB::OnMenuWatchDereference(wxCommandEvent& event)
{
    WatchesDlg *watches = Manager::Get()->GetDebuggerManager()->GetWatchesDialog();
    if (!watches)
        return;

    watches->RenameWatch(m_watchToDereferenceProperty, wxT("*") + m_watchToDereferenceSymbol);
    m_watchToDereferenceProperty = NULL;
    m_watchToDereferenceSymbol = wxEmptyString;
}

void DebuggerGDB::AttachToProcess(const wxString& pid)
{
    if (!pid.IsEmpty())
    {
        pid.ToLong((long*)&m_PidToAttach);
        Debug(false);
    }
}

void DebuggerGDB::DetachFromProcess()
{
    m_State.GetDriver()->Detach();
    m_PidToAttach = 0;
    m_State.GetDriver()->Stop();
}

bool DebuggerGDB::IsAttachedToProcess() const
{
    return m_PidToAttach != 0;
}

void DebuggerGDB::OnSettings(wxCommandEvent& event)
{
    Configure();
}

void DebuggerGDB::CompilerFinished(bool compilerFailed, StartType startType)
{
    if (compilerFailed || startType == StartTypeUnknown)
        return;
    if (DoDebug(startType == StartTypeStepInto) != 0)
    {
        ProjectManager *manager = Manager::Get()->GetProjectManager();
        if (manager->GetIsRunning() && manager->GetIsRunning() == this)
            manager->SetIsRunning(NULL);
    }
}

void DebuggerGDB::OnBuildTargetSelected(CodeBlocksEvent& event)
{
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("DebuggerGDB::OnBuildTargetSelected: target=%s"), event.GetBuildTargetName().c_str()));
    // verify that the project that sent it, is the one we 're debugging
    // and that a project is loaded
    if (m_pProject && event.GetProject() == m_pProject)
        m_ActiveBuildTarget = event.GetBuildTargetName();
}
