/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef WATCHESDLG_H
#define WATCHESDLG_H

#include <vector>
#include <wx/panel.h>
//#include <wx/propgrid/propgrid.h>

#include "debuggermanager.h"

class wxPropertyGrid;
class wxPropertyGridEvent;
class wxPGProperty;
class WatchesProperty;

class DLLIMPORT WatchesDlg : public wxPanel
{
    public:
        WatchesDlg();

        void UpdateWatches();
        void AddWatch(cbWatch *watch);
    private:
        void OnExpand(wxPropertyGridEvent &event);
        void OnCollapse(wxPropertyGridEvent &event);
        void OnPropertySelected(wxPropertyGridEvent &event);
        void OnPropertyChanged(wxPropertyGridEvent &event);
        void OnPropertyChanging(wxPropertyGridEvent &event);
        void OnPropertyLableEditBegin(wxPropertyGridEvent &event);
        void OnPropertyLableEditEnd(wxPropertyGridEvent &event);
        void OnPropertyRightClick(wxPropertyGridEvent &event);
        void OnIdle(wxIdleEvent &event);
        void OnKeyDown(wxKeyEvent &event);

        void OnMenuRename(wxCommandEvent &event);
        void OnMenuProperties(wxCommandEvent &event);
        void OnMenuDelete(wxCommandEvent &event);
        void OnMenuDeleteAll(wxCommandEvent &event);

        DECLARE_EVENT_TABLE()
    private:
        void DeleteProperty(WatchesProperty &prop);
    private:
        struct WatchItem
        {
            cbWatch      *watch;
            wxPGProperty *property;
        };
        typedef std::vector<WatchItem> WatchItems;

    private:
        wxPropertyGrid *m_grid;
        WatchItems m_watches;
        bool m_append_empty_watch;
};

#endif // WATCHESDLG_H
