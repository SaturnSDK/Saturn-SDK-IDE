/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CB_BACKTRACEDLG_H
#define CB_BACKTRACEDLG_H

#include <wx/panel.h>

class wxCommandEvent;
class wxListCtrl;
class wxListEvent;
class wxWindow;

class cbBacktraceDlg : public wxPanel
{
    public:
        cbBacktraceDlg(wxWindow* parent);

        void Reload();
    private:
        void OnListRightClick(wxListEvent& event);
        void OnDoubleClick(wxListEvent& event);
        void OnJump(wxCommandEvent& event);
        void OnSwitchFrame(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);
        void OnCopyToClipboard(wxCommandEvent& event);

        void OnSettingJumpDefault(wxCommandEvent& event);
        void OnSettingSwitchDefault(wxCommandEvent& event);

    private:
        DECLARE_EVENT_TABLE();
    private:
        wxListCtrl *m_list;
};

#endif // CB_BACKTRACEDLG_H
