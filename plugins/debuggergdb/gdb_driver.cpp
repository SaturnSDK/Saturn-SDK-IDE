#include <sdk.h>
#include "gdb_driver.h"
#include "gdb_commands.h"
#include <manager.h>
#include <configmanager.h>
#include <scriptingmanager.h>
#include <scriptingcall.h>
#include <angelscript.h>
#include <globals.h>

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(TypesArray);

#define GDB_PROMPT _T("(gdb)")

static wxRegEx reBreak2(_T("^(0x[A-z0-9]+) in (.*) from (.*)"));
#ifdef __WXMSW__
    static wxRegEx reBreak(_T("([A-z]:)([^:]+):([0-9]+):[0-9]+:[begmidl]+:(0x[0-9A-z]+)"));
#else
    static wxRegEx reBreak(_T("\032\032([^:]+):([0-9]+):[0-9]+:[begmidl]+:(0x[0-9A-z]+)"));
#endif

GDB_driver::GDB_driver(DebuggerGDB* plugin)
    : DebuggerDriver(plugin)
{
    //ctor
}

GDB_driver::~GDB_driver()
{
    //dtor
}

void DummyAddRef(DebuggerDriver& p){}
void DummyRelease(DebuggerDriver& p){}

void GDB_driver::InitializeScripting()
{
    // get a pointer to scripting engine
    asIScriptEngine* engine = Manager::Get()->GetScriptingManager()->GetEngine();
    if (!engine)
    {
        m_pDBG->Log(_("Scripting engine not running. Debugger scripts disabled..."));
        return; // no scripting support...
    }

    const wxString module = _T("debugger-scripts");
    const wxString script = _T("gdb_types.script");

    // discard any old instance
    int r = engine->Discard(cbU2C(module));

    if (r != 0)
    {
        // create a new object type for scripts, named DebuggerDriver
        engine->RegisterObjectType("DebuggerDriver", 0, asOBJ_CLASS);
        engine->RegisterObjectBehaviour("DebuggerDriver", asBEHAVE_ADDREF, "void f()", asFUNCTION(DummyAddRef), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour("DebuggerDriver", asBEHAVE_RELEASE, "void f()", asFUNCTION(DummyRelease), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectMethod("DebuggerDriver", "void RegisterType(const wxString& in,const wxString& in,const wxString& in,const wxString& in)", asMETHOD(GDB_driver, RegisterType), asCALL_THISCALL);
    }

    // run all scripts
    Manager::Get()->GetScriptingManager()->LoadScript(script, module, false);

    int funcID = Manager::Get()->GetScriptingManager()->FindFunctionByDeclaration(_T("void RegisterTypes(DebuggerDriver@ driver)"), module);
    if (funcID < 0)
        m_pDBG->Log(wxString::Format(_T("Invalid debugger script: '%s'"), script.c_str()));

    VoidExecutor<DebuggerDriver*> exec(funcID);
    exec.Call(this);
}

void GDB_driver::RegisterType(const wxString& name, const wxString& regex, const wxString& eval_func, const wxString& parse_func)
{
    // check if this type already exists
    for (size_t i = 0; i < m_Types.GetCount(); ++i)
    {
        ScriptedType& st = m_Types[i];
        if (st.name == name)
            return; // exists already...
    }

    ScriptedType st;
    st.name = name;
    st.regex_str = regex;
    st.regex.Compile(regex);
    st.eval_func = eval_func;
    st.parse_func = parse_func;

    m_Types.Add(st);
    m_pDBG->Log(_("Registered new type: ") + st.name);
}

wxString GDB_driver::GetScriptedTypeCommand(const wxString& gdb_type, wxString& parse_func)
{
    for (size_t i = 0; i < m_Types.GetCount(); ++i)
    {
        ScriptedType& st = m_Types[i];
        if (st.regex.Matches(gdb_type))
        {
//            Log(_T("Function to print '") + gdb_type + _T("' is ") + st.parse_func);
            parse_func = st.parse_func;
            return st.eval_func;
        }
    }
    return _T("");
}

wxString GDB_driver::GetCommandLine(const wxString& debugger, const wxString& debuggee)
{
    wxString cmd;
    cmd << debugger;
    cmd << _T(" -nx");          // don't run .gdbinit
    cmd << _T(" -fullname ");   // report full-path filenames when breaking
    cmd << _T(" -args ") << debuggee;
    return cmd;
}

wxString GDB_driver::GetCommandLine(const wxString& debugger, int pid)
{
    wxString cmd;
    cmd << debugger;
    cmd << _T(" -nx");          // don't run .gdbinit
    cmd << _T(" -fullname ");   // report full-path filenames when breaking
    cmd << _T("-pid=") << wxString::Format(_T("%d"), pid);
    return cmd;
}

void GDB_driver::Prepare(bool isConsole)
{
    // default initialization

    // make sure we 're using the prompt that we know and trust ;)
	QueueCommand(new DebuggerCmd(this, wxString(_T("set prompt ")) + GDB_PROMPT));

    // send built-in init commands
	QueueCommand(new DebuggerCmd(this, _T("set confirm off")));
	// no wrapping lines
    QueueCommand(new DebuggerCmd(this, _T("set width 0")));
    // no pagination
    QueueCommand(new DebuggerCmd(this, _T("set height 0")));
    // allow pending breakpoints
    QueueCommand(new DebuggerCmd(this, _T("set breakpoint pending on")));
    // show pretty function names in disassembly
    QueueCommand(new DebuggerCmd(this, _T("set print asm-demangle on")));
#ifndef __WXMSW__
    QueueCommand(new DebuggerCmd(this, _T("set disassembly-flavor att")));
#else
	if (isConsole)
        QueueCommand(new DebuggerCmd(this, _T("set new-console on")));
    QueueCommand(new DebuggerCmd(this, _T("set disassembly-flavor intel")));
#endif

    // define all scripted types
    m_Types.Clear();
    InitializeScripting();

    // pass user init-commands
    wxString init = Manager::Get()->GetConfigManager(_T("debugger"))->Read(_T("init_commands"), wxEmptyString);
    // commands are passed in one go, in case the user defines functions in there
    // or else it would lock up...
    QueueCommand(new DebuggerCmd(this, init));
//    wxArrayString initCmds = GetArrayFromString(init, _T('\n'));
//    for (unsigned int i = 0; i < initCmds.GetCount(); ++i)
//    {
//        QueueCommand(new DebuggerCmd(this, initCmds[i]));
//    }

    // set working directory
    if (!m_WorkingDir.IsEmpty())
        QueueCommand(new DebuggerCmd(this, _T("cd ") + m_WorkingDir));

    // add search dirs
    for (unsigned int i = 0; i < m_Dirs.GetCount(); ++i)
    {
        QueueCommand(new GdbCmd_AddSourceDir(this, m_Dirs[i]));
    }
}

void GDB_driver::Start(bool breakOnEntry)
{
    // start the process
    ResetCursor();
    QueueCommand(new DebuggerCmd(this, breakOnEntry ? _T("start") : _T("run")));
}

void GDB_driver::Stop()
{
    ResetCursor();
    QueueCommand(new DebuggerCmd(this, _T("quit")));
}

void GDB_driver::Continue()
{
    ResetCursor();
    QueueCommand(new DebuggerCmd(this, _T("cont")));
}

void GDB_driver::Step()
{
    ResetCursor();
    QueueCommand(new DebuggerCmd(this, _T("next")));
}

void GDB_driver::StepInstruction()
{
    ResetCursor();
    QueueCommand(new DebuggerCmd(this, _T("nexti")));
}

void GDB_driver::StepIn()
{
    ResetCursor();
    QueueCommand(new DebuggerCmd(this, _T("step")));
}

void GDB_driver::StepOut()
{
    ResetCursor();
    QueueCommand(new DebuggerCmd(this, _T("finish")));
}

void GDB_driver::Backtrace()
{
    if (!m_pBacktrace)
        return;
    QueueCommand(new GdbCmd_Backtrace(this, m_pBacktrace));
}

void GDB_driver::Disassemble()
{
    if (!m_pDisassembly)
        return;
    QueueCommand(new GdbCmd_DisassemblyInit(this, m_pDisassembly));
}

void GDB_driver::CPURegisters()
{
    if (!m_pCPURegisters)
        return;
    QueueCommand(new GdbCmd_InfoRegisters(this, m_pCPURegisters));
}

void GDB_driver::SwitchToFrame(size_t number)
{
    ResetCursor();
    QueueCommand(new DebuggerCmd(this, wxString::Format(_T("frame %d"), number)));
}

void GDB_driver::SetVarValue(const wxString& var, const wxString& value)
{
    QueueCommand(new DebuggerCmd(this, wxString::Format(_T("set variable %s=%s"), var.c_str(), value.c_str())));
}

void GDB_driver::MemoryDump()
{
    if (!m_pExamineMemory)
        return;
    QueueCommand(new GdbCmd_ExamineMemory(this, m_pExamineMemory));
}

void GDB_driver::InfoFrame()
{
    QueueCommand(new DebuggerInfoCmd(this, _T("info frame"), _("Selected frame")));
}

void GDB_driver::InfoDLL()
{
    QueueCommand(new DebuggerInfoCmd(this, _T("info dll"), _("Loaded libraries")));
}

void GDB_driver::InfoFiles()
{
    QueueCommand(new DebuggerInfoCmd(this, _T("info files"), _("Files and targets")));
}

void GDB_driver::InfoFPU()
{
    QueueCommand(new DebuggerInfoCmd(this, _T("info float"), _("Floating point unit")));
}

void GDB_driver::InfoSignals()
{
    QueueCommand(new DebuggerInfoCmd(this, _T("info signals"), _("Signals handling")));
}

void GDB_driver::InfoThreads()
{
    QueueCommand(new DebuggerInfoCmd(this, _T("info threads"), _("Running threads")));
}

void GDB_driver::AddBreakpoint(DebuggerBreakpoint* bp)
{
    //Workaround for GDB to break on C++ constructor/destructor
    if (bp->func.IsEmpty() && !bp->lineText.IsEmpty())
    {
        wxRegEx reCtorDtor(_T("([0-9A-z_]+)::([~]?)([0-9A-z_]+)[ \t\(]*"));
        if (reCtorDtor.Matches(bp->lineText))
        {
            wxString strBase = reCtorDtor.GetMatch(bp->lineText, 1);
            wxString strDtor = reCtorDtor.GetMatch(bp->lineText, 2);
            wxString strMethod = reCtorDtor.GetMatch(bp->lineText, 3);
            if (strBase.IsSameAs(strMethod))
            {
                bp->func = strBase;
                bp->func << _T("::");
                bp->func << strDtor;
                bp->func << strMethod;
//                if (bp->temporary)
//                    bp->temporary = false;
                NotifyCursorChanged(); // to force breakpoints window update
            }
        }
    }
    //end GDB workaround

	QueueCommand(new GdbCmd_AddBreakpoint(this, bp));
}

void GDB_driver::RemoveBreakpoint(DebuggerBreakpoint* bp)
{
	QueueCommand(new GdbCmd_RemoveBreakpoint(this, bp));
}

void GDB_driver::EvaluateSymbol(const wxString& symbol, wxTipWindow** tipWin, const wxRect& tipRect)
{
    QueueCommand(new GdbCmd_FindTooltipType(this, symbol, tipWin, tipRect));
}

void GDB_driver::UpdateWatches(bool doLocals, bool doArgs, DebuggerTree* tree)
{
    // start updating watches tree
    tree->BeginUpdateTree();

    // locals before args because of precedence
    if (doLocals)
        QueueCommand(new GdbCmd_InfoLocals(this, tree));
    if (doArgs)
        QueueCommand(new GdbCmd_InfoArguments(this, tree));
    for (unsigned int i = 0; i < tree->GetWatches().GetCount(); ++i)
    {
        Watch& w = tree->GetWatches()[i];
        if (w.format == Undefined)
            QueueCommand(new GdbCmd_FindWatchType(this, tree, &w));
        else
            QueueCommand(new GdbCmd_Watch(this, tree, &w));
    }

    // run this action-only command to update the tree
    QueueCommand(new DbgCmd_UpdateWatchesTree(this, tree));
}

void GDB_driver::Detach()
{
    QueueCommand(new GdbCmd_Detach(this));
}

void GDB_driver::ParseOutput(const wxString& output)
{
    m_Cursor.changed = false;
    if (output.StartsWith(_T("gdb: ")) ||
        output.StartsWith(_T("ContinueDebugEvent ")))
    {
        return;
    }
    static wxString buffer;
	buffer << output << _T('\n');

	m_pDBG->DebugLog(output);

    int idx = buffer.First(GDB_PROMPT);
    if (idx != wxNOT_FOUND)
    {
        m_ProgramIsStopped = true;
        m_QueueBusy = false;
        DebuggerCmd* cmd = CurrentCommand();
        if (cmd)
        {
//            Log(_T("Command parsing output: ") + buffer.Left(idx));
            RemoveTopCommand(false);
            buffer.Remove(idx);
            if (buffer.Last() == _T('\n'))
                buffer.RemoveLast();
            cmd->ParseOutput(buffer.Left(idx));
            delete cmd;
            RunQueue();
        }
    }
    else
    {
//        m_ProgramIsStopped = false;
        return; // come back later
    }

    bool needsUpdate = false;

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
            m_pDBG->Log(_("Debugger name and version: ") + lines[i]);
            break;
        }

        // Is the program exited?
        else if (lines[i].StartsWith(_T("Program exited")))
        {
            m_pDBG->Log(lines[i]);
            QueueCommand(new DebuggerCmd(this, _T("quit")));
        }

        // no debug symbols?
        else if (lines[i].Contains(_T("(no debugging symbols found)")))
        {
            m_pDBG->Log(lines[i]);
        }

        // signal
        else if (lines[i].StartsWith(_T("Program received signal")))
        {
            Log(lines[i]);
            m_pDBG->BringAppToFront();
            if (IsWindowReallyShown(m_pBacktrace))
            {
                // don't ask; it's already shown
                // just grab the user's attention
                cbMessageBox(lines[i], _("Signal received"), wxICON_ERROR);
            }
            else if (cbMessageBox(wxString::Format(_("%s\nDo you want to view the backtrace?"), lines[i].c_str()), _("Signal received"), wxICON_ERROR | wxYES_NO) == wxID_YES)
            {
                // show the backtrace window
                CodeBlocksDockEvent evt(cbEVT_SHOW_DOCK_WINDOW);
                evt.pWindow = m_pBacktrace;
                Manager::Get()->GetAppWindow()->ProcessEvent(evt);
            }
            needsUpdate = true;
            // the backtrace will be generated when NotifyPlugins() is called
            // and only if the backtrace window is shown
        }

        // pending breakpoint resolved?
        // e.g.
        // Pending breakpoint "C:/Devel/codeblocks/trunk/src/sdk/cbproject.cpp:332" resolved
        else if (lines[i].StartsWith(_T("Pending breakpoint ")))
        {
            m_pDBG->Log(lines[i]);
        }

        // cursor change
        else if (lines[i].StartsWith(g_EscapeChars)) // ->->
        {
            // breakpoint, e.g.
            // C:/Devel/tmp/test_console_dbg/tmp/main.cpp:14:171:beg:0x401428
			if ( reBreak.Matches(lines[i]) )
			{
			#ifdef __WXMSW__
				m_Cursor.file = reBreak.GetMatch(lines[i], 1) + reBreak.GetMatch(lines[i], 2);
				wxString lineStr = reBreak.GetMatch(lines[i], 3);
				m_Cursor.address = reBreak.GetMatch(lines[i], 4);
            #else
				m_Cursor.file = reBreak.GetMatch(lines[i], 1);
				wxString lineStr = reBreak.GetMatch(lines[i], 2);
				m_Cursor.address = reBreak.GetMatch(lines[i], 3);
            #endif
				lineStr.ToLong(&m_Cursor.line);
                m_Cursor.changed = true;
                needsUpdate = true;
			}
        }
        else
        {
            // other break info, e.g.
            // 0x7c9507a8 in ntdll!KiIntSystemCall () from C:\WINDOWS\system32\ntdll.dll
			if ( reBreak2.Matches(lines[i]) )
			{
				m_Cursor.file = reBreak2.GetMatch(lines[i], 3);
				m_Cursor.function = reBreak2.GetMatch(lines[i], 2);
				wxString lineStr = _T("");
				m_Cursor.address = reBreak2.GetMatch(lines[i], 1);
				m_Cursor.line = -1;
                m_Cursor.changed = true;
                needsUpdate = true;
			}
        }
    }
    buffer.Clear();

    // if program is stopped, update various states
    if (needsUpdate)
    {
        if (m_Cursor.changed)
            NotifyCursorChanged();
    }
}
