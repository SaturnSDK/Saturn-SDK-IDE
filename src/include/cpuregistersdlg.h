/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CPUREGISTERSDLG_H
#define CPUREGISTERSDLG_H

#include <wx/panel.h>

class wxListCtrl;

class cbCPURegistersDlg : public wxPanel
{
    public:
        cbCPURegistersDlg(wxWindow* parent);

        void Clear();
        void SetRegisterValue(const wxString& reg_name, unsigned long int value);
    protected:
        int RegisterIndex(const wxString& reg_name);
        void OnRefresh(wxCommandEvent& event);

    private:
        DECLARE_EVENT_TABLE();
    private:
        wxListCtrl* m_pList;
};

#endif // CPUREGISTERSDLG_H
