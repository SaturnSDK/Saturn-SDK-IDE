#ifndef DEBUGGER_COMMANDS_H
#define DEBUGGER_COMMANDS_H

#include <wx/string.h>
#include <wx/regex.h>
#include <wx/tipwin.h>
#include <wx/regex.h>
#include <globals.h>
#include "debuggergdb.h"
#include "debuggertree.h"
#include "debugger_defs.h"
#include "backtracedlg.h"

/** Basic interface for debugger commands.
  *
  * Each command sent to the debugger, is an DebuggerCmd.
  * It encapsulates the call and parsing of return values.
  * The most important function is ParseOutput() inside
  * which it must parse the commands output.
  *
  * @remarks This is not an abstract interface, i.e. you can
  * create instances of it. The default implementation just
  * logs the command's output. This way you can debug new commands.
  */
class DebuggerCmd
{
    public:
        DebuggerCmd(DebuggerGDB* gdb, const wxString& cmd = _T(""), bool logToNormalLog = false)
            : m_Cmd(cmd),
            m_pGDB(gdb),
            m_LogToNormalLog(logToNormalLog)
        {}
        virtual ~DebuggerCmd(){}

        /** Parses the command's output.
          * @param The output. This is the full output up to
          * (and including) the prompt.
          */
        virtual void ParseOutput(const wxString& output)
        {
            if (!output.IsEmpty())
            {
                if (m_LogToNormalLog)
                    m_pGDB->Log(output);
                else
                    m_pGDB->DebugLog(output);
            }
        }

        wxString m_Cmd;         ///< the actual command
    protected:
        DebuggerGDB* m_pGDB;    ///< the debugger plugin
        bool m_LogToNormalLog;  ///< if true, log to normal log, else the debug log
};

/**
  * Command to add a search directory for source files in debugger's paths.
  */
class DbgCmd_AddSourceDir : public DebuggerCmd
{
    public:
        /** If @c dir is empty, resets all search dirs to $cdir:$cwd, the default. */
        DbgCmd_AddSourceDir(DebuggerGDB* gdb, const wxString& dir)
            : DebuggerCmd(gdb)
        {
            m_Cmd << _T("directory ") << dir;
        }
        void ParseOutput(const wxString& output)
        {
            // Output:
            // Warning: C:\Devel\tmp\console\111: No such file or directory.
            // Source directories searched: <dir>;$cdir;$cwd
            if (output.StartsWith(_T("Warning: ")))
                m_pGDB->Log(output.BeforeFirst(_T('\n')));
        }
};

/**
  * Command to the set the file to be debugged.
  */
class DbgCmd_SetDebuggee : public DebuggerCmd
{
    public:
        /** @param file The file to debug. */
        DbgCmd_SetDebuggee(DebuggerGDB* gdb, const wxString& file)
            : DebuggerCmd(gdb)
        {
            m_Cmd << _T("file ") << file;
        }
        void ParseOutput(const wxString& output)
        {
            // Output:
            // Reading symbols from C:\Devel\tmp\console/console.exe...done.
            // or if it doesn't exist:
            // console.exe: No such file or directory.

            // just log everything before the prompt
            m_pGDB->Log(output.BeforeFirst(_T('\n')));
        }
};

/**
  * Command to the add symbol files.
  */
class DbgCmd_AddSymbolFile : public DebuggerCmd
{
    public:
        /** @param file The file which contains the symbols. */
        DbgCmd_AddSymbolFile(DebuggerGDB* gdb, const wxString& file)
            : DebuggerCmd(gdb)
        {
            m_Cmd << _T("add-symbol-file ") << file;
        }
        void ParseOutput(const wxString& output)
        {
            // Output:
            //
            // add symbol table from file "console.exe" at
            // Reading symbols from C:\Devel\tmp\console/console.exe...done.
            //
            // or if it doesn't exist:
            // add symbol table from file "console.exe" at
            // console.exe: No such file or directory.

            // just ignore the "add symbol" line and log the rest before the prompt
            m_pGDB->Log(output.AfterFirst(_T('\n')).BeforeLast(_T('\n')));
        }
};

/**
  * Command to set the arguments to the debuggee.
  */
class DbgCmd_SetArguments : public DebuggerCmd
{
    public:
        /** @param file The file which contains the symbols. */
        DbgCmd_SetArguments(DebuggerGDB* gdb, const wxString& args)
            : DebuggerCmd(gdb)
        {
            m_Cmd << _T("set args ") << args;
        }
        void ParseOutput(const wxString& output)
        {
            // No output
        }
};

/**
  * Command to add a breakpoint.
  */
class DbgCmd_AddBreakpoint : public DebuggerCmd
{
    public:
        /** @param bp The breakpoint to set. */
        DbgCmd_AddBreakpoint(DebuggerGDB* gdb, DebuggerBreakpoint* bp)
            : DebuggerCmd(gdb),
            m_BP(bp)
        {
            if (bp->enabled)
            {
                if (bp->func.IsEmpty())
                {
                    wxString out = m_BP->filename;
                    DebuggerGDB::ConvertToGDBDirectory(out);
                    // we add one to line,  because scintilla uses 0-based line numbers, while gdb uses 1-based
                    m_Cmd << _T("break ") << out << _T(":") << bp->line + 1;
                }
                //GDB workaround
                //Use function name if this is C++ constructor/destructor
                else
                {
                    m_Cmd << _T("break ") << bp->func;
                }
                //end GDB workaround

                // condition and ignore count will be set in ParseOutput, where we 'll have the bp number
            }
        }
        void ParseOutput(const wxString& output)
        {
            // possible outputs (we 're only interested in 1st sample):
            //
            // Breakpoint 1 at 0x4013d6: file main.cpp, line 8.
            // No line 100 in file "main.cpp".
            // No source file named main2.cpp.
            wxRegEx re(_T("Breakpoint ([0-9]+) at (0x[0-9A-Fa-f]+)"));
            if (re.Matches(output))
            {
                m_pGDB->DebugLog(wxString::Format(_("Breakpoint added: file %s, line %d"), m_BP->filename.c_str(), m_BP->line + 1));
                if (!m_BP->func.IsEmpty())
                    m_pGDB->DebugLog(_("(work-around for constructors activated)"));

                re.GetMatch(output, 1).ToLong(&m_BP->bpNum);
                re.GetMatch(output, 2).ToULong(&m_BP->address, 16);

                // conditional breakpoint
                if (m_BP->useCondition && !m_BP->condition.IsEmpty())
                {
                    wxString cmd;
                    cmd << _T("condition ") << m_BP->bpNum << _T(" ") << m_BP->condition;
                    m_pGDB->QueueCommand(new DebuggerCmd(m_pGDB, cmd));
                }

                // ignore count
                if (m_BP->useIgnoreCount && m_BP->ignoreCount > 0)
                {
                    wxString cmd;
                    cmd << _T("ignore ") << m_BP->bpNum << _T(" ") << m_BP->ignoreCount;
                    m_pGDB->QueueCommand(new DebuggerCmd(m_pGDB, cmd));
                }
            }
            else
                m_pGDB->Log(output); // one of the error responses
        }

        DebuggerBreakpoint* m_BP;
};

/**
  * Command to remove a breakpoint.
  */
class DbgCmd_RemoveBreakpoint : public DebuggerCmd
{
    public:
        /** @param bp The breakpoint to remove. If NULL, all breakpoints are removed.
            @param deleteBPwhenDone If bp is not NULL and this is true the breakpoint will be deleted after removal. */
        DbgCmd_RemoveBreakpoint(DebuggerGDB* gdb, DebuggerBreakpoint* bp, bool deleteBPwhenDone = false)
            : DebuggerCmd(gdb),
            m_BP(bp),
            m_DeleteBPwhenDone(deleteBPwhenDone)
        {
            if (!bp)
            {
                m_Cmd << _T("delete");
                return;
            }

            if (bp->enabled && bp->bpNum > 0)
            {
                m_Cmd << _T("delete ") << bp->bpNum;
            }
        }
        void ParseOutput(const wxString& output)
        {
            if (!m_BP)
                return;

            // invalidate bp number
            m_BP->bpNum = -1;

            if (!output.IsEmpty())
                m_pGDB->Log(output);
            m_pGDB->DebugLog(wxString::Format(_("Breakpoint removed: file %s, line %d"), m_BP->filename.c_str(), m_BP->line + 1));
            if (m_DeleteBPwhenDone)
                delete m_BP;
        }

        DebuggerBreakpoint* m_BP;
        bool m_DeleteBPwhenDone;
};

/**
  * Command to get info about local frame variables.
  */
class DbgCmd_InfoLocals : public DebuggerCmd
{
        DebuggerTree* m_pDTree;
    public:
        /** @param tree The tree to display the locals. */
        DbgCmd_InfoLocals(DebuggerGDB* gdb, DebuggerTree* dtree)
            : DebuggerCmd(gdb),
            m_pDTree(dtree)
        {
            m_Cmd << _T("info locals");
        }
        void ParseOutput(const wxString& output)
        {
            wxArrayString lines = GetArrayFromString(output, _T('\n'));
            wxString locals;
    		locals << _T("Local variables = {");
    		for (unsigned int i = 0; i < lines.GetCount(); ++i)
                locals << lines[i] << _T(',');
            locals << _T("}") << _T('\n');
            m_pDTree->BuildTree(locals);
        }
};

/**
  * Command to get info about current function arguments.
  */
class DbgCmd_InfoArguments : public DebuggerCmd
{
        DebuggerTree* m_pDTree;
    public:
        /** @param tree The tree to display the args. */
        DbgCmd_InfoArguments(DebuggerGDB* gdb, DebuggerTree* dtree)
            : DebuggerCmd(gdb),
            m_pDTree(dtree)
        {
            m_Cmd << _T("info args");
        }
        void ParseOutput(const wxString& output)
        {
            wxArrayString lines = GetArrayFromString(output, _T('\n'));
            wxString args;
    		args << _T("Function Arguments = {");
    		for (unsigned int i = 0; i < lines.GetCount(); ++i)
                args << lines[i] << _T(',');
            args << _T("}") << _T('\n');
            m_pDTree->BuildTree(args);
        }
};

/**
  * Command to get info about a watched variable.
  */
class DbgCmd_Watch : public DebuggerCmd
{
        DebuggerTree* m_pDTree;
        Watch* m_pWatch;
    public:
        /** @param tree The tree to display the watch. */
        DbgCmd_Watch(DebuggerGDB* gdb, DebuggerTree* dtree, Watch* watch)
            : DebuggerCmd(gdb),
            m_pDTree(dtree),
            m_pWatch(watch)
        {
            m_Cmd << _T("output ") << Watch::FormatCommand(m_pWatch->format) << _T(" ") << m_pWatch->keyword;
        }
        void ParseOutput(const wxString& output)
        {
            wxArrayString lines = GetArrayFromString(output, _T('\n'));
            wxString w;
    		w << m_pWatch->keyword << _T(" = ");
    		for (unsigned int i = 0; i < lines.GetCount(); ++i)
                w << lines[i] << _T(',');
            w << _T('\n');
            m_pDTree->BuildTree(w);
        }
};

/**
  * Command to display a tooltip about a variables value.
  */
class DbgCmd_TooltipEvaluation : public DebuggerCmd
{
        DebuggerTree* m_pDTree;
        wxTipWindow** m_pWin;
        wxRect m_WinRect;
        wxString m_What;
    public:
        /** @param what The variable to evaluate.
            @param win A pointer to the tip window pointer.
            @param tiprect The tip window's rect.
        */
        DbgCmd_TooltipEvaluation(DebuggerGDB* gdb, const wxString& what, wxTipWindow** win, const wxRect& tiprect)
            : DebuggerCmd(gdb),
            m_pWin(win),
            m_WinRect(tiprect),
            m_What(what)
        {
            m_Cmd << _T("output ") << what;
        }
        void ParseOutput(const wxString& output)
        {
            wxString tip;
            if (output.StartsWith(_T("No symbol ")) || output.StartsWith(_T("Attempt to ")))
                tip = output;
            else
                tip = m_What + _T("=") + output;

            if (*m_pWin)
                (*m_pWin)->Destroy();
            *m_pWin = new wxTipWindow(Manager::Get()->GetAppWindow(), tip, 640, m_pWin, &m_WinRect);
//            m_pGDB->DebugLog(output);
        }
};

/**
  * Command to run a backtrace.
  */
class DbgCmd_Backtrace : public DebuggerCmd
{
        BacktraceDlg* m_pDlg;
    public:
        /** @param dlg The backtrace dialog. */
        DbgCmd_Backtrace(DebuggerGDB* gdb, BacktraceDlg* dlg)
            : DebuggerCmd(gdb),
            m_pDlg(dlg)
        {
            m_Cmd << _T("bt");
        }
        void ParseOutput(const wxString& output)
        {
            // output is a series of:
            // #0  main () at main.cpp:8
            wxArrayString lines = GetArrayFromString(output, _T('\n'));
    		for (unsigned int i = 0; i < lines.GetCount(); ++i)
    		{
    		    wxRegEx re(_T("#([0-9]+)[ \t]+([A-Za-z0-9_:]+) \\(\\) at (.*):([0-9]+)"));
    		    if (re.Matches(lines[i]))
    		    {
                    StackFrame sf;
                    sf.valid = true;
    		        re.GetMatch(lines[i], 1).ToLong(&sf.number);
    		        sf.function = re.GetMatch(lines[i], 2);
    		        sf.file = re.GetMatch(lines[i], 3);
    		        sf.line = re.GetMatch(lines[i], 4);
                    m_pDlg->AddFrame(sf);
    		    }
    		}
//            m_pGDB->DebugLog(output);
        }
};

/**
  * Command to initialize a disassembly.
  */
class DbgCmd_DisassemblyInit : public DebuggerCmd
{
        DisassemblyDlg* m_pDlg;
    public:
        /** @param dlg The disassembly dialog. */
        DbgCmd_DisassemblyInit(DebuggerGDB* gdb, DisassemblyDlg* dlg)
            : DebuggerCmd(gdb),
            m_pDlg(dlg)
        {
            m_Cmd << _T("frame");
        }
        void ParseOutput(const wxString& output)
        {
            // output is two lines describing the current frame:
            //
            // #0  main () at main.cpp:8
            // C:/Devel/tmp/console/main.cpp:8:63:beg:0x4013ba

            if (!m_pDlg)
                return;

            StackFrame sf;
            wxArrayString lines = GetArrayFromString(output, _T('\n'));
    		for (unsigned int i = 0; i < lines.GetCount(); ++i)
    		{
                if (!lines[i].StartsWith(g_EscapeChars)) // ->->
                {
                    // #0  main () at main.cpp:8
                    wxRegEx re(_T("#([0-9]+)[ \t]+([A-Za-z0-9_:]+) \\(\\) at"));
                    if (re.Matches(lines[i]))
                    {
                        re.GetMatch(lines[i], 1).ToLong(&sf.number);
                        sf.function = re.GetMatch(lines[i], 2);
                    }
                }
                else
                {
                    // 