/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef EDITBREAKPOINT_H
#define EDITBREAKPOINT_H

#include "scrollingdialog.h"

class cbBreakpoint;

class EditBreakpointDlg : public wxScrollingDialog
{
    public:
        EditBreakpointDlg(cbBreakpoint* breakpoint, wxWindow* parent = 0);
        virtual ~EditBreakpointDlg();
    protected:
        void OnUpdateUI(wxUpdateUIEvent& event);
        void EndModal(int retCode);

        cbBreakpoint* m_breakpoint;
    private:
        DECLARE_EVENT_TABLE()
};

#endif // EDITBREAKPOINT_H
