#ifndef CLASSBROWSERBUILDER_H
#define CLASSBROWSERBUILDER_H

#include <wx/treectrl.h>


#include "parser/token.h"
#include "parser/parser.h"

static int idClassBrowserUpdated = wxNewId();

enum SpecialFolder
{
    sfToken         = 0x0001, // token node
    sfRoot          = 0x0002, // root node
    sfGFuncs        = 0x0004, // global funcs node
    sfGVars         = 0x0008, // global vars node
    sfPreproc       = 0x0010, // preprocessor symbols node
    sfTypedef       = 0x0020, // typedefs node
    sfBase          = 0x0040, // base classes node
    sfDerived       = 0x0080, // derived classes node
};

class CBTreeData : public wxTreeItemData
{
    public:
        CBTreeData(SpecialFolder sf = sfToken, Token* token = 0, int kindMask = 0xffffffff)
            : m_pToken(token),
            m_KindMask(kindMask),
            m_SpecialFolder(sf),
            m_TokenIndex(token ? token->GetSelf() : -1),
            m_TokenKind(token ? token->m_TokenKind : tkUndefined),
            m_Ticket(token ? token->GetTicket() : 0)
        {
        }
        Token* m_pToken;
        int m_KindMask;
        SpecialFolder m_SpecialFolder;
        int m_TokenIndex;
        TokenKind m_TokenKind;
        unsigned long m_Ticket;
};

class ClassBrowserBuilderData;

class ClassBrowserBuilder : public wxEvtHandler
{
    friend class ClassBrowserBuilderData;
    public:
        ClassBrowserBuilder();
        virtual ~ClassBrowserBuilder();

        void Init(Parser* parser,
                    wxTreeCtrl* treeTop,
                    wxTreeCtrl* treeBottom,
                    const wxString& active_filename,
                    void* user_data, // active project
                    const BrowserOptions& options,
                    TokensTree* pTokens);
        void AbortBuilding();
        void ExpandItem(wxTreeItemId item);
        void CollapseItem(wxTreeItemId item);
        void SelectItem(wxTreeItemId item);
        void Run();
        void Stop();
        bool IsRunning();
        bool IsNodeExpanding(wxTreeItemId node);
        void OnTimer(wxTimerEvent& evt);
        void OnFinished();
    protected:
        void MainLoop();

        wxTreeItemId AddNodeIfNotThere(wxTreeCtrl* tree, wxTreeItemId parent, const wxString& name, int imgIndex = -1, CBTreeData* data = 0, bool sorted = true);
        bool AddChildrenOf(wxTreeCtrl* tree, wxTreeItemId parent, int parentTokenIdx, int tokenKindMask = 0xffff);
        bool AddAncestorsOf(wxTreeCtrl* tree, wxTreeItemId parent, int tokenIdx);
        bool AddDescendantsOf(wxTreeCtrl* tree, wxTreeItemId parent, int tokenIdx, bool allowInheritance = true);
        bool AddNodes(wxTreeCtrl* tree, wxTreeItemId parent, TokenIdxSet::iterator start, TokenIdxSet::iterator end, int tokenKindMask = 0xffff, bool allowGlobals = false);
        void PreExpandItem(wxTreeItemId item);
        void SelectNode(wxTreeItemId node);
        bool TokenMatchesFilter(Token* token);
        bool TokenContainsChildrenOfKind(Token* token, int kind);
        bool CreateSpecialFolders(wxTreeCtrl* tree, wxTreeItemId parent);
		void ExpandNamespaces(wxTreeItemId node);

        Parser* m_pParser;
        wxTreeCtrl* m_pTreeTop;
        wxTreeCtrl* m_pTreeBottom;
        wxString m_ActiveFilename;
        void* m_pUserData; // active project
        BrowserOptions m_Options;
        TokensTree* m_pTokens;
        TokenIdxSet m_NodesWaitingForExpansion;
        unsigned long m_NodeRemovingTime;
        unsigned long m_NodeAddingTime;
        unsigned long m_NamespacesExpandingTime;
        unsigned long m_AddNodeSearchCount;
        unsigned long m_AddNodeCount;

        // pair of current-file-filter
        TokenFilesSet m_CurrentFileSet;

        wxTimer m_Timer;
        bool m_Aborted;
        bool m_IsRunning;
        bool m_Reentrant;
        ClassBrowserBuilderData* m_pData;
        unsigned int m_Interval;
        unsigned int m_IterationsPerCycle;
    private:
        DECLARE_EVENT_TABLE()
};

#endif // CLASSBROWSERBUILDERTHREAD_H
