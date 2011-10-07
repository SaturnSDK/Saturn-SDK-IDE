/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef DEBUGGERSTATE_H
#define DEBUGGERSTATE_H

#include <wx/string.h>
#include "debugger_defs.h"

class DebuggerGDB;
class ProjectBuildTarget;
class cbProject;

class DebuggerState
{
    public:
        DebuggerState(DebuggerGDB* plugin);
        ~DebuggerState();

        BreakpointsList const & GetBreakpoints() const { return m_Breakpoints; }

        bool StartDriver(ProjectBuildTarget* target);
        void StopDriver();

        /// Check so see if Driver exists before getting it
        bool HasDriver() const;

        /// Will always return a driver, or throw a code assertion error
        // (to fix multiple bugs in use of GetDriver without checking return value)
        DebuggerDriver* GetDriver();
        const DebuggerDriver* GetDriver() const;

        void CleanUp();

        int AddBreakpoint(DebuggerBreakpoint::Pointer bp); // returns -1 if not found
        DebuggerBreakpoint::Pointer AddBreakpoint(const wxString& file, int line, bool temp = false,
                                          const wxString& lineText = wxEmptyString);
        DebuggerBreakpoint::Pointer AddBreakpoint(const wxString& dataAddr, bool onRead = false, bool onWrite = true);
        void RemoveBreakpoint(int idx, bool removeFromDriver = true);
        void RemoveBreakpoint(DebuggerBreakpoint::Pointer bp, bool removeFromDriver = true);
        void RemoveAllBreakpoints();
        void RemoveAllProjectBreakpoints(cbProject* prj);

        // helpers to keep in sync with the editors
        void ShiftBreakpoint(DebuggerBreakpoint::Pointer bp, int nroflines);

        int HasBreakpoint(const wxString& file, int line); // returns -1 if not found
        DebuggerBreakpoint::Pointer GetBreakpoint(int idx);
        DebuggerBreakpoint::Pointer GetBreakpointByNumber(int num);
        const DebuggerBreakpoint::Pointer GetBreakpointByNumber(int num) const;
        void ResetBreakpoint(DebuggerBreakpoint::Pointer bp);
        void ApplyBreakpoints();
    protected:
        wxString ConvertToValidFilename(const wxString& filename);
        cbProject* FindProjectForFile(const wxString& file);

        DebuggerGDB* m_pPlugin;
        DebuggerDriver* m_pDriver;
        BreakpointsList m_Breakpoints;
};

#endif // DEBUGGERSTATE_H
