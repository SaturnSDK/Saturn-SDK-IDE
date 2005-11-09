#ifndef DEBUGGERTREE_H
#define DEBUGGERTREE_H

#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <wx/dynarray.h>

#include "debugger_defs.h"

extern int cbCustom_WATCHES_CHANGED;

class DebuggerTree : public wxPanel
{
	public:
		DebuggerTree(wxEvtHandler* debugger, wxNotebook* parent);
		virtual ~DebuggerTree();
		wxTreeCtrl* GetTree(){ return m_pTree; }
		void BuildTree(const wxString& infoText);

		void ClearWatches();
		void SetWatches(const WatchesArray& watches);
		const WatchesArray& GetWatches();
		void AddWatch(const wxString& watch, WatchFormat format = Undefined);
		void DeleteWatch(const wxString& watch, WatchFormat format = Any);
		void DeleteAllWatches();
		Watch* FindWatch(const wxString& watch, WatchFormat format = Any);
		int FindWatchIndex(const wxString& watch, WatchFormat format = Any);
	protected:
		void ShowMenu(wxTreeItemId id, const wxPoint& pt);
		void OnTreeRightClick(wxTreeEvent& event);
		void OnRightClick(wxCommandEvent& event);
		void OnAddWatch(wxCommandEvent& event);
		void OnEditWatch(wxCommandEvent& event);
		void OnDeleteWatch(wxCommandEvent& event);
		void OnDeleteAllWatches(wxCommandEvent& event);

		void ParseEntry(const wxTreeItemId& parent, wxString& text);
		wxTreeCtrl* m_pTree;
		wxNotebook* m_pParent;
		wxEvtHandler* m_pDebugger;
		int m_PageIndex;
		WatchesArray m_Watches;
	private:
        int FindCharOutsideQuotes(const wxString& str, wxChar ch); // returns position of ch in str
        int FindCommaPos(const wxString& str); // ignores commas in function signatures
		DECLARE_EVENT_TABLE()
};

#endif // DEBUGGERTREE_H

