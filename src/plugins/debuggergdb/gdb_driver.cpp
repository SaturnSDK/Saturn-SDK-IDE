#include "gdb_driver.h"

GDB_driver::GDB_driver(DebuggerGDB* plugin)
    : DebuggerDriver(plugin)
{
    //ctor
}

GDB_driver::~GDB_driver()
{
    //dtor
}

void GDB_driver::Start(const wxString& filename, bool isConsole)
{

}

void GDB_driver::Start(int pid)
{

}

void GDB_driver::Stop()
{

}

void GDB_driver::AddBreakpoint(DebuggerBreakpoint* bp)
{

}

void GDB_driver::RemoveBreakpoint(DebuggerBreakpoint* bp, bool deleteAlso)
{

}

void GDB_driver::EvaluateSymbol(const wxString& symbol, wxTipWindow** tipWin, const wxRect& tipRect)
{

}

void GDB_driver::UpdateWatches(bool doLocals, bool doArgs, DebuggerTree* tree)
{

}

void GDB_driver::ParseOutput(const wxString& output)
{

}
