#include "debuggerdriver.h"

DebuggerDriver::DebuggerDriver(DebuggerGDB* plugin)
    : m_pDBG(plugin)
{
    //ctor
}

DebuggerDriver::~DebuggerDriver()
{
    //dtor
}

void DebuggerDriver::ClearDirectories()
{
    m_Dirs.Clear();
}

void DebuggerDriver::AddDirectory(const wxString& dir)
{
    if (m_Dirs.Index(dir) == wxNOT_FOUND)
        m_Dirs.Add(dir);
}

void DebuggerDriver::SetWorkingDirectory(const wxString& dir)
{
    m_WorkingDir = dir;
}
