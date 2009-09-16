/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef BREAKPOINTSDLG_H
#define BREAKPOINTSDLG_H

#include <wx/panel.h>

class wxCommandEvent;
class wxListCtrl;
class wxListEvent;

class cbBreakpointsDlg : public wxPanel
{
    public:
        cbBreakpointsDlg();

        bool AddBreakpoint(const wxString& filename, int line);
        bool RemoveBreakpoint(const wxString& filename, int line);

        void Reload();
    private:
        void RemoveBreakpoint(int sel);

        void OnRemove(wxCommandEvent& event);
        void OnRemoveAll(wxCommandEvent& event);
        void OnProperties(wxCommandEvent& event);
        void OnOpen(wxCommandEvent& event);
        void OnRightClick(wxListEvent& event);
        void OnDoubleClick(wxListEvent& event);
        void OnBreakpointAdd(CodeBlocksEvent& event);
        void OnBreakpointEdit(CodeBlocksEvent& event);
        void OnBreakpointDelete(CodeBlocksEvent& event);

    private:
        enum Columns
        {
            Type = 0,
            FilenameAddress,
            Line,
            Debugger
        };

        struct Item
        {
            Item(cbBreakpoint *breakpoint_,  cbDebuggerPlugin *plugin_, wxString const &pluginName_) :
                breakpoint(breakpoint_),
                plugin(plugin_),
                pluginName(pluginName_)
            {
            }

            cbBreakpoint *breakpoint;
            cbDebuggerPlugin *plugin;
            wxString pluginName;
        };
        typedef std::vector<Item> Items;
    private:
        wxListCtrl* m_pList;
        Items m_breakpoints;
    private:
        DECLARE_EVENT_TABLE()
};

#endif // BREAKPOINTSDLG_H
