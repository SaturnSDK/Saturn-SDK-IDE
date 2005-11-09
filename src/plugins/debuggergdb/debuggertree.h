#ifndef DEBUGGERTREE_H
#define DEBUGGERTREE_H

#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <wx/dynarray.h>

extern int cbCustom_WATCHES_CHANGED;

enum WatchFormat
{
    Undefined = 0,
    Decimal,
    Unsigned,
    Hex,
    Binary,
    Char,

    // do not remove this
    Last,
    Any
};

struct Watch
{
    Watch(const wxString& k, WatchFormat f = Undefined) : keyword(k), format(f) {}
    Watch(const Watch& rhs) : keyword(rhs.keyword), format(rhs.format) {}
    static wxString FormatCommand(WatchFormat format)
    {
        switch (format)
        {
            case Decimal:       return _T("/d");
            case Unsigned:      return _T("/u");
            case Hex:           return _T("/x");
            case Binary:        return _T("/t");
            case Char:          return _T("/c");
            default:            return wxEmptyString;
        }
    }
    wxString keyword;
    WatchFormat format;
};
WX_DECLARE_OBJARRAY(Watch, WatchesArray);

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

