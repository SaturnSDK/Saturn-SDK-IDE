#ifndef GDB_DRIVER_H
#define GDB_DRIVER_H

#include "debuggerdriver.h"

class GDB_driver : public DebuggerDriver
{
    public:
        GDB_driver(DebuggerGDB* plugin);
        virtual ~GDB_driver();

        virtual void Start(const wxString& filename, bool isConsole);
        virtual void Start(int pid);
        virtual void Stop();

        virtual void AddBreakpoint(DebuggerBreakpoint* bp);
        virtual void RemoveBreakpoint(DebuggerBreakpoint* bp, bool deleteAlso = false);
        virtual void EvaluateSymbol(const wxString& symbol, wxTipWindow** tipWin, const wxRect& tipRect);
        virtual void UpdateWatches(bool doLocals, bool doArgs, DebuggerTree* tree);
        virtual void ParseOutput(const wxString& output);
    protected:
    private:
};

#endif // GDB_DRIVER_H
