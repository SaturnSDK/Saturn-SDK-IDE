/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef THREADSDLG_H
#define THREADSDLG_H

#include <wx/panel.h>

class wxCommandEvent;
class wxListCtrl;
class wxListEvent;

class cbThreadsDlg : public wxPanel
{
    public:
        cbThreadsDlg(wxWindow* parent);

        void Reload();
    protected:
        void OnListRightClick(wxListEvent& event);
        void OnListDoubleClick(wxListEvent& event);
        void OnSwitchThread(wxCommandEvent& event);

    private:
        DECLARE_EVENT_TABLE();
    private:
        wxListCtrl *m_list;
};

#endif // THREADSDLG_H
