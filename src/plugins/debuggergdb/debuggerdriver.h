#ifndef DEBUGGERDRIVER_H
#define DEBUGGERDRIVER_H

#include "debugger_defs.h"
#include <wx/regex.h>
#include <wx/tipwin.h>
#include <globals.h>

class DebuggerGDB;
class DebuggerTree;
class Compiler;

class DebuggerDriver
{
    public:
        DebuggerDriver(DebuggerGDB* plugin);
        virtual ~DebuggerDriver();

        ////////////////////////////////
        // BEFORE PROCESS STARTS - BEGIN
        ////////////////////////////////

        /** Add a directory in search list. */
        virtual void AddDirectory(const wxString& dir);

        /** Clear directories search list. */
        virtual void ClearDirectories();

        /** Set the working directory. */
        virtual void SetWorkingDirectory(const wxString& dir);

        /** Begin the debugging process by launching a program.
            @param filename The program to launch (containing any command-line arguments).
//            @param isConsole If true, the debuggee is a console executable.
        */
        virtual void Start(const wxString& filename, bool isConsole) = 0;

        /** Begin the debugging process by attaching to an already running process.
            @param pid The PID to attach to.
        */
        virtual void Start(int pid) = 0;

        ////////////////////////////////
        // BEFORE PROCESS STARTS - END
        ////////////////////////////////

        /** Stop debugging. */
        virtual void Stop() = 0;

        /** Add a breakpoint.
            @param bp The breakpoint to add.
        */
        virtual void AddBreakpoint(DebuggerBreakpoint* bp) = 0;

        /** Remove a breakpoint.
            @param bp The breakpoint to remove.
            @param deleteAlso If true, @c bp will be deleted when done.
        */
        virtual void RemoveBreakpoint(DebuggerBreakpoint* bp, bool deleteAlso = false) = 0;

        /** Evaluate a symbol.
            @param symbol The symbol to evaluate.
            @param tipWin The wxTipWindow* variable's address.
            @param tipRect The rect to use for the tip window.
        */
        virtual void EvaluateSymbol(const wxString& symbol, wxTipWindow** tipWin, const wxRect& tipRect) = 0;

        /** Update watches.
            @param doLocals Display values of local variables.
            @param doArgs Display values of function arguments.
            @param tree The watches tree control.
        */
        virtual void UpdateWatches(bool doLocals, bool doArgs, DebuggerTree* tree) = 0;

        /** Parse debugger's output. */
        virtual void ParseOutput(const wxString& output) = 0;
    protected:
        DebuggerGDB* m_pDBG;
        wxArrayString m_Dirs;
        wxString m_WorkingDir;
    private:
};

#endif // DEBUGGERDRIVER_H
