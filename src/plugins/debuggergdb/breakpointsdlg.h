#ifndef BREAKPOINTS_H
#define BREAKPOINTS_H

#include <wx/dialog.h>

class BreakpointsDlg : public wxDialog
{
    public:
        BreakpointsDlg(wxWindow* parent = 0);
        virtual ~BreakpointsDlg();
    protected:
        void FillBreakpoints();
        void FillRecord(int sel);
        void EndModal(int retCode);

        void OnRemove(wxCommandEvent& event);
        void OnRemoveAll(wxCommandEvent& event);
        void OnListboxClick(wxCommandEvent& event);
        void OnUpdateUI(wxUpdateUIEvent& event);

        int m_LastSel;
    private:
        DECLARE_EVENT_TABLE()
};

#endif // BREAKPOINTS_H
