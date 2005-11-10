#ifndef EDITBREAKPOINT_H
#define EDITBREAKPOINT_H

#include <wx/dialog.h>
#include "debugger_defs.h"

class EditBreakpointDlg : public wxDialog
{
    public:
        EditBreakpointDlg(DebuggerBreakpoint* bp, wxWindow* parent = 0);
        virtual ~EditBreakpointDlg();
    protected:
        void OnUpdateUI(wxUpdateUIEvent& event);
        void EndModal(int retCode);

        DebuggerBreakpoint* m_Bp;
    private:
        DECLARE_EVENT_TABLE()
};

#endif // EDITBREAKPOINT_H
