#include <sdk.h>
#include "classbrowserbuilder.h"
#include "statemachinehelper.h"
#include <globals.h>
#include <manager.h>
#include <projectmanager.h>
#include <cbproject.h>

#include <wx/settings.h>
#include <wx/utils.h>
#include <stack>
#include <algorithm>


namespace compatibility { typedef TernaryCondTypedef<wxMinimumVersion<2,5>::eval, wxTreeItemIdValue, long int>::eval tree_cookie_t; };
static int idBuildClassBrowser = wxNewId();

BEGIN_EVENT_TABLE(ClassBrowserBuilder, wxEvtHandler)
    EVT_TIMER(idBuildClassBrowser, ClassBrowserBuilder::OnTimer)
END_EVENT_TABLE()


class ClassBrowserBuilderAuxData
{
    public:
        wxTreeItemId node;
        wxTreeItemId existing;
        wxTreeItemId parent;
        int tokenIdx;
        int tokenKindMask;
        bool ShowInheritance;
        bool AllowGlobals;
        TokenIdxSet::iterator start;
        TokenIdxSet::iterator end;
};

// Auxiliary functions for the state machine's stack handling
void StateMachineData<ClassBrowserBuilderAuxData>::CopyFrom(const ClassBrowserBuilderAuxData& src)
{
    node = src.node;
    existing = src.existing;
    parent = src.parent;
    ShowInheritance = src.ShowInheritance;
    tokenIdx = src.tokenIdx;
    tokenKindMask = src.tokenKindMask;
    ShowInheritance = src.ShowInheritance;
    AllowGlobals = src.AllowGlobals;
    start = src.start;
    end = src.end;
}

void StateMachineData<ClassBrowserBuilderAuxData>::CopyTo(ClassBrowserBuilderAuxData& dst)
{
    dst.node = node;
    dst.existing = existing;
    dst.parent = parent;
    dst.ShowInheritance = ShowInheritance;
    dst.tokenIdx = tokenIdx;
    dst.tokenKindMask = tokenKindMask;
    dst.ShowInheritance = ShowInheritance;
    dst.AllowGlobals = AllowGlobals;
    dst.start = start;
    dst.end = end;
}

// A rudimentary state machine with stacks for each function call
class ClassBrowserBuilderData : public StateMachineHelper<ClassBrowserBuilderAuxData>
{
    public:
        ClassBrowserBuilderData(ClassBrowserBuilder* owner);
        void RemoveInvalidNodes(wxTreeItemId param);
        void ExpandItem(wxTreeItemId param);
        void AddChildrenOf(wxTreeItemId param, int paramTokenIdx, int paramTokenKindMask = 0xffff);
        void AddAncestorsOf(wxTreeItemId param, int paramTokenIdx);
        void AddDescendantsOf(wxTreeItemId param, int paramTokenIdx, bool allowInheritance = true);
        void AddNodes(wxTreeItemId param, TokenIdxSet::iterator paramStart, TokenIdxSet::iterator paramEnd, int paramTokenKindMask = 0xffff, bool paramAllowGlobals = false);
        void ExpandNamespaces(wxTreeItemId param);

        wxTreeItemId root;
    private:
        ClassBrowserBuilder* m_pOwner;
};

ClassBrowserBuilderData::ClassBrowserBuilderData(ClassBrowserBuilder* owner) :
m_pOwner(owner)
{
    clear();
}

inline void ClassBrowserBuilderData::RemoveInvalidNodes(wxTreeItemId param)
{
    ClassBrowserBuilderAuxData tmp;
    CopyTo(tmp); // Variables are global
    tmp.parent = param;
    Go_sub(100,tmp);
}

inline void ClassBrowserBuilderData::ExpandItem(wxTreeItemId param)
{
    ClassBrowserBuilderAuxData tmp;
    CopyTo(tmp); // Variables are global
    tmp.parent = param;
    Go_sub(200,tmp);
}

inline void ClassBrowserBuilderData::AddChildrenOf(wxTreeItemId param, int paramTokenIdx, int paramTokenKindMask)
{
    ClassBrowserBuilderAuxData tmp;
    CopyTo(tmp);
    tmp.node = param;
    tmp.tokenIdx = paramTokenIdx;
    tmp.tokenKindMask = paramTokenKindMask;
    Go_sub(300,tmp);
}

inline void ClassBrowserBuilderData::AddAncestorsOf(wxTreeItemId param, int paramTokenIdx)
{
    ClassBrowserBuilderAuxData tmp;
    CopyTo(tmp);
    tmp.node = param;
    tmp.tokenIdx = paramTokenIdx;
    Go_sub(400,tmp);
}

inline void ClassBrowserBuilderData::AddDescendantsOf(wxTreeItemId param, int paramTokenIdx, bool allowInheritance)
{
    ClassBrowserBuilderAuxData tmp;
    CopyTo(tmp);
    tmp.node = param;
    tmp.tokenIdx = paramTokenIdx;
    tmp.ShowInheritance = allowInheritance;
    Go_sub(500,tmp);
}

inline void ClassBrowserBuilderData::AddNodes(wxTreeItemId param, TokenIdxSet::iterator paramStart, TokenIdxSet::iterator paramEnd, int paramTokenKindMask, bool paramAllowGlobals)
{
    ClassBrowserBuilderAuxData tmp;
    CopyTo(tmp);
    tmp.parent = param;
    tmp.start = paramStart;
    tmp.end = paramEnd;
    tmp.tokenKindMask = paramTokenKindMask;
    tmp.AllowGlobals = paramAllowGlobals;
    Go_sub(600,tmp);
}

inline void ClassBrowserBuilderData::ExpandNamespaces(wxTreeItemId param)
{
    ClassBrowserBuilderAuxData tmp;
    CopyTo(tmp);
    tmp.node = param;
    Go_sub(700,tmp);
}

ClassBrowserBuilder::ClassBrowserBuilder() :
    m_pParser(0),
    m_pTreeTop(0),
    m_pTreeBottom(0),
    m_pUserData(0),
    m_Options(),
    m_pTokens(0),
    m_Aborted(false),
    m_IsRunning(false),
    m_Reentrant(false),
    m_pData(0),
    m_Interval(1),  // Set the timer interval to 1 millisecond
    m_IterationsPerCycle(80)
{
    //ctor
    m_pData = new ClassBrowserBuilderData(this);
    m_Timer.SetOwner(this,idBuildClassBrowser);
    m_NodesWaitingForExpansion.clear();
//    Connect(idBuildClassBrowser,wxEVT_TIMER,wxTimerEventHandler(ClassBrowserBuilder::OnTimer) );
}

ClassBrowserBuilder::~ClassBrowserBuilder()
{
    //dtor
    Stop();
//    Disconnect(idBuildClassBrowser,wxEVT_TIMER,wxTimerEventHandler(ClassBrowserBuilder::OnTimer) );
    delete m_pData;
}

void ClassBrowserBuilder::Run()
{
    m_IsRunning = true;
    m_Timer.Start(m_Interval, wxTIMER_CONTINUOUS);
}

void ClassBrowserBuilder::Stop()
{
    m_Timer.Stop();
    m_IsRunning = false;
    m_Reentrant = false;
}

bool ClassBrowserBuilder::IsRunning()
{
    return m_IsRunning;
}

void ClassBrowserBuilder::AbortBuilding()
{
    m_Aborted = true;
}

void ClassBrowserBuilder::Init(Parser* parser,
                                    wxTreeCtrl* treeTop,
                                    wxTreeCtrl* treeBottom,
                                    const wxString& active_filename,
                                    void* user_data, // active project
                                    const BrowserOptions& options,
                                    TokensTree* pTokens)
{
    Stop(); // Better safe than sorry
    m_pParser = parser;
    m_pTreeTop = treeTop;
    m_pTreeBottom = treeBottom;
    m_ActiveFilename = active_filename;
    m_pUserData = user_data;
    m_Options = options;
    m_pTokens = pTokens;
    m_NodeRemovingTime = 0;
    m_NodeAddingTime = 0;
    m_NamespacesExpandingTime = 0;
    m_CurrentFileSet.clear();
    m_NodesWaitingForExpansion.clear();
    m_pData->clear();
    m_pData->ShowInheritance = m_Options.showInheritance;
    m_AddNodeSearchCount = 0;
    m_AddNodeCount = 0;

    // fill filter set for current-file-filter
    if (m_Options.displayFilter == bdfFile && !m_ActiveFilename.IsEmpty())
    {
        // m_ActiveFilename is the full filename up to the extension dot. No extension though.
        TokensTree* tree = m_pParser->GetTokens();

        // get all filenames' indices matching our mask
        tree->m_FilenamesMap.FindMatches(m_ActiveFilename, m_CurrentFileSet, true, true);
    }

}

void ClassBrowserBuilder::OnTimer(wxTimerEvent& evt)
{
    if(!m_IsRunning) // We shouldn't be here
        return;
    if(m_Reentrant)
        return;   // Avoid reentrancy due to tight loops
    m_Reentrant = true;
    if(m_Aborted || Manager::IsAppShuttingDown())
    {
        Stop();
    }
    else
    {
        m_Timer.Stop(); // Stop the timer
        MainLoop();
        if(m_IsRunning) // Still not finished?
        {
            Run(); // Restart the timer
        }
        else
        {
            m_NodesWaitingForExpansion.clear();
        }

    }
    m_Reentrant = false;
}

void ClassBrowserBuilder::MainLoop()
{
    if(Manager::IsAppShuttingDown())
    {
        Stop();
        return;
    }
    if(!m_pTreeTop || !m_pTreeBottom)
        return;
    bool success = false;

    m_pTreeTop->Freeze();
    m_pTreeBottom->Freeze();

    // We run this once every millisecond, so we have to do it fast!
    for(size_t iteration = 0; iteration < m_IterationsPerCycle && !m_pData->m_Finished; iteration++)
    {
        if(m_Aborted || Manager::IsAppShuttingDown() || m_pData->m_StackError)
            break;
        switch(m_pData->state)
        {
            case 0: // Init
            {
                m_pTreeTop->SetImageList(m_pParser->GetImageList());
                m_pTreeBottom->SetImageList(m_pParser->GetImageList());
                m_pData->root = m_pTreeTop->GetRootItem();
                if (!(m_pData->root.IsOk()))
                {
                    m_pData->root = m_pTreeTop->AddRoot(_("Symbols"), PARSER_IMG_SYMBOLS_FOLDER, PARSER_IMG_SYMBOLS_FOLDER, new CBTreeData(sfRoot));
                    m_pTreeTop->SetItemHasChildren(m_pData->root);
                }
            }
            break;
            case 1: // Remove invalid nodes from the top tree
            {
                m_pData->RemoveInvalidNodes(m_pData->root);
                // IMPORTANT! This is *NOT* a function call, but a TRANSITION.
                // To be more precise, it's a transition which saves the stack,
                // so it DOES behave like a  function call, but it's a transition
                // anyway. (It's a "gosub" transition). Gosub and Goto transitions
                // must be the last operations done in a state.

                // Something worthy of note: In our state machine, states go
                // forward automatically, like a BASIC interpreter does with
                // line numbers. Right now we're at state #1, so after returning
                // from the "function call", m_pData->GetNextState() will advance
                // the state to #2.
            }
            break;
            case 2: // Remove Invalid nodes from bottom tree
            {
                iteration += 10;
                m_pTreeBottom->DeleteChildren(m_pTreeBottom->GetRootItem());
            }
            break;
            case 3: // Build nodes
            {
                m_pData->ExpandItem(m_pData->root);
            }
            break;
            case 4: // Expand Namespaces
            {
                // Another GOSUB transition.
                m_pData->ExpandNamespaces(m_pTreeTop->GetRootItem());
            }
            break;
            case 5: // Create bottom tree by selecting a node from the top tree
            {
                iteration += 100; // This is a very expensive operation
                SelectNode(m_pTreeTop->GetSelection());
            }
            break;
            case 6: // Done!
            {
                m_pData->End(); // Tell the state machine we're done
                success = true;
            }
            break;

            // *** RemoveInvalidNodes(parent): States 100 - 102 ***
            case 100: // Prepare variables for the beginning of loop
            {
                if(!m_pData->parent.IsOk())
                {
                    m_pData->Ret();
                }
                m_pData->existing = m_pTreeTop->GetLastChild(m_pData->parent);
            }
            break;
            case 101: // Begin of Loop
            {
                // We check this everytime because conditions could have changed
                // Between the states
                if(!m_pData->parent.IsOk())
                {
                    m_pData->Ret();
                }
                if(!(m_pData->existing.IsOk())) // Invalid node, or end of loop
                {
                    // Is parent left without children?
                    if(m_pTreeTop->GetChildrenCount(m_pData->parent) == 0 && m_pData->parent != m_pData->root)
                    {
                        m_pTreeTop->SetItemHasChildren(m_pData->parent, false);
                    }
                    m_pData->Ret(); // Return from subroutine
                    break;
                }

                // These local variables aren't used after recursion, so they don't need saving
                CBTreeData* data = (CBTreeData*)(m_pTreeTop->GetItemData(m_pData->existing));
                bool removeCurrent = false;
                bool hasChildren = (m_pTreeTop->GetChildrenCount(m_pData->existing) > 0);

                if (data && data->m_pToken)
                {
                    if (m_pTokens->at(data->m_TokenIndex) != data->m_pToken ||
                        data->m_Ticket != data->m_pToken->GetTicket() ||
                        !TokenMatchesFilter(data->m_pToken))
                    {
                        removeCurrent = true;
                    }
                }
                if(removeCurrent)
                {
                    if(hasChildren)
                    {
                        m_pTreeTop->DeleteChildren(m_pData->existing);
                    }
                    wxTreeItemId next = m_pTreeTop->GetPrevSibling(m_pData->existing);
                    m_pTreeTop->Delete(m_pData->existing);
                    m_pData->existing = next;
                    m_pData->Go_to(101); // Continue
                }
                else
                {
                    m_pData->RemoveInvalidNodes(m_pData->existing); // recurse
                    // (and remember, this is a transition, not a function call!)
                }
            }
            break;
            case 102: // End of Loop
            {
                if(!(m_pData->existing.IsOk()))
                {
                    m_pData->Ret();
                    break;
                }
                m_pData->existing = m_pTreeTop->GetPrevSibling(m_pData->existing);
                m_pData->Go_to(101);
            }
            break;

            // *** ExpandItem ***

            case 200:
            {
                wxTreeItemId item = m_pData->parent;
                if(!item.IsOk())
                {
                    m_pData->Ret();
                    break;
                }

                // Mark the item as expanding so the event handler won't mess up,
                // then expand the item in the tree
                PreExpandItem(item);

//                ExpandItem(item);   // In case of bug, uncomment these 3 lines
//                m_pData->Ret();
//                break;

                // And now, for the expansion itself.
                CBTreeData* data = (CBTreeData*)m_pTreeTop->GetItemData(item);
                if(!data)
                    break;

                switch (data->m_SpecialFolder)
                {
                    case sfRoot:
                    {
                        CreateSpecialFolders(m_pTreeTop, item);
                        m_pData->AddChildrenOf(item, -1, ~(tkFunction | tkVariable | tkPreprocessor | tkTypedef));
                        break;
                    }
                    case sfBase: m_pData->AddAncestorsOf(item, data->m_pToken->GetSelf()); break;
                    case sfDerived: m_pData->AddDescendantsOf(item, data->m_pToken->GetSelf(), false); break;
                    case sfToken:
                    {
                        int kind = 0;
                        switch (data->m_pToken->m_TokenKind)
                        {
                            case tkClass:
                            {
                                // add base and derived classes folders
                                if (m_pData->ShowInheritance)
                                {
                                    wxTreeItemId base = AddNodeIfNotThere(m_pTreeTop, item, _("Base classes"), PARSER_IMG_CLASS_FOLDER, new CBTreeData(sfBase, data->m_pToken, tkClass));
                                    if (!data->m_pToken->m_DirectAncestors.empty())
                                        m_pTreeTop->SetItemHasChildren(base);
                                    wxTreeItemId derived = AddNodeIfNotThere(m_pTreeTop, item, _("Derived classes"), PARSER_IMG_CLASS_FOLDER, new CBTreeData(sfDerived, data->m_pToken, tkClass));
                                    if (!data->m_pToken->m_Descendants.empty())
                                        m_pTreeTop->SetItemHasChildren(derived);
                                }
                                kind = tkClass | tkEnum;
                                break;
                            }
                            case tkNamespace:
                                kind = tkNamespace | tkClass | tkEnum;
                                break;
                            default:
                                break;
                        }
                        if (kind != 0)
                            m_pData->AddChildrenOf(item, data->m_pToken->GetSelf(), kind);
                        break;
                    }
                    default: break;
                }

            }
            break;

            case 201:
            {
                m_pData->Ret();
            }
            break;

            // *** AddChildrenOf ***
            // void AddChildrenOf(wxTreeItemId param, int paramTokenIdx, int paramTokenKindMask = 0xffff);

            case 300:
            {
                Token* parentToken = 0;
                TokenIdxSet::iterator it;
                TokenIdxSet::iterator it_end;

                if (m_pData->tokenIdx == -1)
                {
                    it = m_pTokens->m_GlobalNameSpace.begin();
                    it_end = m_pTokens->m_GlobalNameSpace.end();
                }
                else
                {
                    parentToken = m_pTokens->at(m_pData->tokenIdx);
                    if (!parentToken)
                    {
                        m_pData->Ret(); // Error: Token not found!!
                        break;
                    }
                    it = parentToken->m_Children.begin();
                    it_end = parentToken->m_Children.end();
                }
                m_pData->AddNodes(m_pData->node,it,it_end, m_pData->tokenKindMask);
            }
            break;
            case 301:
            {
                m_pData->Ret();
            }
            break;

            // *** AddAncestorsOf ***
            // void AddAncestorsOf(wxTreeItemId param, int paramTokenIdx);

            case 400:
            {
                Token* token = m_pTokens->at(m_pData->tokenIdx);
                if (!token)
                {
                    m_pData->Ret();
                    break;
                }
                m_pData->AddNodes(m_pData->node, token->m_DirectAncestors.begin(), token->m_DirectAncestors.end(), tkClass | tkTypedef, true);
            }
            break;

            case 401:
            {
                m_pData->Ret();
            }
            break;

            // *** AddDescendantsOf ***
            // void AddDescendantsOf(wxTreeItemId param, int paramTokenIdx, bool allowInheritance = true);

            case 500:
            {
                Token* token = m_pTokens->at(m_pData->tokenIdx);
                if (!token)
                {
                    m_pData->Ret();
                    break;
                }
                m_pData->AddNodes(m_pData->node, token->m_Descendants.begin(), token->m_Descendants.end(), tkClass | tkTypedef, true);
            }
            break;

            case 501:
            {
                m_pData->Ret();
            }
            break;

            // *** AddNodes ***
            // void AddNodes(wxTreeItemId param, TokenIdxSet::iterator paramStart, TokenIdxSet::iterator paramEnd, int paramTokenKindMask = 0xffff, bool paramAllowGlobals = false);

            case 600:
            {
                if(m_pData->start == m_pData->end)
                {
                    m_pData->Ret();
                    break;
                }
                Token* token = m_pTokens->at(*(m_pData->start));
                if (token &&
                    (token->m_TokenKind & m_pData->tokenKindMask) &&
                    (m_pData->AllowGlobals || token->m_IsLocal) &&
                    TokenMatchesFilter(token))
                {
                    int img = m_pParser->GetTokenKindImage(token);

                    wxString str = token->m_Name;
                    if (token->m_TokenKind == tkFunction || token->m_TokenKind == tkConstructor || token->m_TokenKind == tkDestructor)
                        str << token->m_Args;
                    if (!token->m_ActualType.IsEmpty())
                         str = str + _T(" : ") + token->m_ActualType;

                    wxTreeItemId child = AddNodeIfNotThere(m_pTreeTop, m_pData->parent, str, img, new CBTreeData(sfToken, token, m_pData->tokenKindMask));
                    // mark as expanding if it is a container
                    if (token->m_TokenKind == tkClass)
                        m_pTreeTop->SetItemHasChildren(child, m_pData->ShowInheritance || TokenContainsChildrenOfKind(token, tkClass | tkNamespace | tkEnum));
                    else if (token->m_TokenKind & (tkNamespace | tkEnum))
                        m_pTreeTop->SetItemHasChildren(child, TokenContainsChildrenOfKind(token, tkClass | tkNamespace | tkEnum));
                }
                m_pData->start++;
                m_pData->Go_to(600); // Loop
            }
            break;

            // *** ExpandNamespaces(node): States 700 - 702 ***

            case 700:
            {
                if( !m_Options.expandNS || !(m_pData->node.IsOk()))
                {
                    m_pData->Ret();
                    break;
                }
                wxTreeItemIdValue enumerationCookie;
                m_pData->existing = m_pTreeTop->GetFirstChild(m_pData->node, enumerationCookie);
            }
            break;

            case 701:
            {
                // Begin of Loop
                wxTreeItemId item = m_pData->existing;
                if(!item.IsOk())
                {
                    m_pData->Ret(); // Return from subroutine
                    break;
                }

                CBTreeData* data = (CBTreeData*)m_pTreeTop->GetItemData(item);
                if (data && data->m_pToken && data->m_pToken->m_TokenKind == tkNamespace)
                {
                    if(!IsNodeExpanding(item))
                    {
                        m_NodesWaitingForExpansion.insert(item); // Flag the item
                        m_pTreeTop->Expand(item); // Mark the item in the tree as open
                    }
                    m_pData->ExpandNamespaces(item); // Recurse
                }
            }
            break;

            case 702:
            {
                // Conditions could have changed. We need to check.
                if(!(m_pData->existing.IsOk()))
                {
                    m_pData->Ret();
                    break;
                }
                m_pData->existing = m_pTreeTop->GetNextSibling(m_pData->existing);
                m_pData->Go_to(701);
                // End of Loop
            }
            break;

            default: // Unknown (error)
            {
                m_pData->End();
            }
        }
        m_pData->GetNextState();
    }
    m_pTreeBottom->Thaw();
    m_pTreeTop->Thaw();

    if(m_pData->m_Finished)
    {
        Stop(); // Stop the timer
        if(success)
        {
            OnFinished();
        }
    }
    return;
}

void ClassBrowserBuilder::PreExpandItem(wxTreeItemId item)
{
    if(!IsNodeExpanding(item))
    {
        m_NodesWaitingForExpansion.insert(item); // Flag the item
        m_pTreeTop->Expand(item); // Mark the item in the tree as open
    }
}

bool ClassBrowserBuilder::IsNodeExpanding(wxTreeItemId node)
{
    return (m_NodesWaitingForExpansion.find(node) != m_NodesWaitingForExpansion.end());
}

void ClassBrowserBuilder::OnFinished()
{
    wxString s;
    s.Printf(_T("Class browser updated (%d nodes added in %d search operations)."),m_AddNodeCount,m_AddNodeSearchCount);
    Manager::Get()->GetMessageManager()->DebugLog(s);
    if(m_pParser)
    {
        wxCommandEvent event(idClassBrowserUpdated);
        m_pParser->ProcessEvent(event);
    }
}

void ClassBrowserBuilder::ExpandNamespaces(wxTreeItemId node)
{
	if (!m_Options.expandNS || !node.IsOk())
		return;

	wxTreeItemIdValue enumerationCookie;
    wxTreeItemId existing = m_pTreeTop->GetFirstChild(node, enumerationCookie);
    while (existing.IsOk())
    {
		CBTreeData* data = (CBTreeData*)m_pTreeTop->GetItemData(existing);
		if (data && data->m_pToken && data->m_pToken->m_TokenKind == tkNamespace)
		{
//			DBGLOG(_T("Auto-expanding: ") + data->m_pToken->m_Name);
			m_pTreeTop->Expand(existing);
			ExpandNamespaces(existing); // recurse
		}

    	existing = m_pTreeTop->GetNextSibling(existing);
	}
}

wxTreeItemId ClassBrowserBuilder::AddNodeIfNotThere(wxTreeCtrl* tree, wxTreeItemId parent, const wxString& name, int imgIndex, CBTreeData* data, bool sorted)
{
    sorted = sorted & tree == m_pTreeTop && data; // sorting only for the top tree
    SpecialFolder new_type = data->m_SpecialFolder;
    bool newIsNamespace = data->m_TokenKind == tkNamespace;

    compatibility::tree_cookie_t cookie = 0;

    wxTreeItemId insert_after; // node to insert after; we 'll be looping all children so we might as well sort at the same time
    wxTreeItemId existing = tree->GetFirstChild(parent, cookie);
    while (existing)
    {
        wxString itemText = tree->GetItemText(existing);
        if (itemText == name)
        {
            // update the existing node's image indices and user-data.
            // it's not possible to have the same token name more than once
            // under the same namespace anyway. if we do, there's a bug in the parser :(
            tree->SetItemImage(existing, imgIndex, wxTreeItemIcon_Normal);
            tree->SetItemImage(existing, imgIndex, wxTreeItemIcon_Selected);
            tree->SetItemData(existing, data);
            return existing;
        }

        if (sorted)
        {
            CBTreeData* existing_data = (CBTreeData*)tree->GetItemData(existing);
            if (existing_data)
            {
                SpecialFolder existing_type = existing_data->m_SpecialFolder;

                // first go special folders
                if ((existing_type & (sfGFuncs | sfGVars | sfPreproc | sfTypedef)) &&
                    !(new_type & (sfGFuncs | sfGVars | sfPreproc | sfTypedef)))
                {
                    insert_after = existing;
                }
                // then go namespaces, alphabetically
                else if (newIsNamespace &&
                        existing_data->m_TokenKind == tkNamespace &&
                        name.CompareTo(itemText, wxString::ignoreCase) >= 0)
                {
                    insert_after = existing;
                }
                // then everything else, alphabetically
                else if (!newIsNamespace &&
                        (existing_data->m_TokenKind == tkNamespace ||
                        name.CompareTo(itemText, wxString::ignoreCase) >= 0))
                {
                    insert_after = existing;
                }
            }
        }
        existing = tree->GetNextChild(parent, cookie);
        m_AddNodeSearchCount++;
    }

    m_AddNodeCount++;
    if (sorted)
        existing = tree->InsertItem(parent, insert_after, name, imgIndex, imgIndex, data);
    else
        existing = tree->AppendItem(parent, name, imgIndex, imgIndex, data);
    return existing;
}

bool ClassBrowserBuilder::AddChildrenOf(wxTreeCtrl* tree, wxTreeItemId parent, int parentTokenIdx, int tokenKindMask)
{
    if (m_Aborted || Manager::IsAppShuttingDown())
        return false;

    Token* parentToken = 0;
    TokenIdxSet::iterator it;
    TokenIdxSet::iterator it_end;

    if (parentTokenIdx == -1)
    {
        it = m_pTokens->m_GlobalNameSpace.begin();
        it_end = m_pTokens->m_GlobalNameSpace.end();
    }
    else
    {
        parentToken = m_pTokens->at(parentTokenIdx);
        if (!parentToken)
        {
//            DBGLOG(_T("Token not found?!?"));
            return false;
        }
        it = parentToken->m_Children.begin();
        it_end = parentToken->m_Children.end();
    }

    return AddNodes(tree, parent, it, it_end, tokenKindMask);
}

bool ClassBrowserBuilder::AddAncestorsOf(wxTreeCtrl* tree, wxTreeItemId parent, int tokenIdx)
{
    if (m_Aborted || Manager::IsAppShuttingDown())
        return false;

    Token* token = m_pTokens->at(tokenIdx);
    if (!token)
        return false;

    return AddNodes(tree, parent, token->m_DirectAncestors.begin(), token->m_DirectAncestors.end(), tkClass | tkTypedef, true);
}

bool ClassBrowserBuilder::AddDescendantsOf(wxTreeCtrl* tree, wxTreeItemId parent, int tokenIdx, bool allowInheritance)
{
    if (m_Aborted || Manager::IsAppShuttingDown())
        return false;

    Token* token = m_pTokens->at(tokenIdx);
    if (!token)
        return false;

    bool inh = m_Options.showInheritance;
    m_Options.showInheritance = allowInheritance;

    bool ret = AddNodes(tree, parent, token->m_Descendants.begin(), token->m_Descendants.end(), tkClass | tkTypedef, true);

    m_Options.showInheritance = inh;
    return ret;
}

bool ClassBrowserBuilder::AddNodes(wxTreeCtrl* tree, wxTreeItemId parent, TokenIdxSet::iterator start, TokenIdxSet::iterator end, int tokenKindMask, bool allowGlobals)
{
    int count = 0;
    for ( ; start != end; ++start)
    {
        Token* token = m_pTokens->at(*start);
        if (token &&
            (token->m_TokenKind & tokenKindMask) &&
            (allowGlobals || token->m_IsLocal) &&
            TokenMatchesFilter(token))
        {
            ++count;
            int img = m_pParser->GetTokenKindImage(token);

            wxString str = token->m_Name;
            if (token->m_TokenKind == tkFunction || token->m_TokenKind == tkConstructor || token->m_TokenKind == tkDestructor)
                str << token->m_Args;
            if (!token->m_ActualType.IsEmpty())
                 str = str + _T(" : ") + token->m_ActualType;

            if (tree == m_pTreeTop)
            {
                wxTreeItemId child = AddNodeIfNotThere(tree, parent, str, img, new CBTreeData(sfToken, token, tokenKindMask));
                // mark as expanding if it is a container
                if (token->m_TokenKind == tkClass)
                    tree->SetItemHasChildren(child, m_Options.showInheritance || TokenContainsChildrenOfKind(token, tkClass | tkNamespace | tkEnum));
                else if (token->m_TokenKind & (tkNamespace | tkEnum))
                    tree->SetItemHasChildren(child, TokenContainsChildrenOfKind(token, tkClass | tkNamespace | tkEnum));
            }
            else // the bottom tree needs no checks
                tree->AppendItem(parent, str, img, img, new CBTreeData(sfToken, token));
        }
    }
//    DBGLOG(_T("Sorting..."));
    if (tree == m_pTreeBottom) // only sort alphabetically the bottom tree
        tree->SortChildren(parent);
//    DBGLOG(_T("Added %d nodes"), count);
    return count != 0;
}

bool ClassBrowserBuilder::TokenMatchesFilter(Token* token)
{
    if (token->m_IsTemp)
        return false;
    switch(m_Options.displayFilter)
    {
        case bdfWorkspace:
            return true;
        case bdfFile:
            return token->RecursiveMatchesFiles(m_CurrentFileSet);
        case bdfProject:
            return (m_pUserData && token->m_pUserData == m_pUserData);
        default:
            return false;
    }
    return false;
}

bool ClassBrowserBuilder::TokenContainsChildrenOfKind(Token* token, int kind)
{
    if (!token)
        return false;
    TokensTree* tt = token->GetTree();
    for (TokenIdxSet::iterator it = token->m_Children.begin(); it != token->m_Children.end(); ++it)
    {
        Token* child = tt->at(*it);
        if (child->m_TokenKind & kind)
            return true;
    }
    return false;
}

void ClassBrowserBuilder::SelectNode(wxTreeItemId node)
{
    if (m_Aborted || Manager::IsAppShuttingDown() || (!(node.IsOk())) || !m_pTreeBottom)
        return;

    m_pTreeBottom->Freeze();
    wxTreeItemId root = m_pTreeBottom->GetRootItem();
    if (!root)
        root = m_pTreeBottom->AddRoot(_T("Members")); // not visible, so don't translate
    else
        m_pTreeBottom->DeleteChildren(root);
    CBTreeData* data = (CBTreeData*)m_pTreeTop->GetItemData(node);
    if (data)
    {
        switch (data->m_SpecialFolder)
        {
            case sfGFuncs: AddChildrenOf(m_pTreeBottom, root, -1, tkFunction); break;
            case sfGVars: AddChildrenOf(m_pTreeBottom, root, -1, tkVariable); break;
            case sfPreproc: AddChildrenOf(m_pTreeBottom, root, -1, tkPreprocessor); break;
            case sfTypedef: AddChildrenOf(m_pTreeBottom, root, -1, tkTypedef); break;
            case sfToken:
            {
                // add all children, except containers
                AddChildrenOf(m_pTreeBottom, root, data->m_pToken->GetSelf(), ~(tkNamespace | tkClass | tkEnum));
                break;
            }
            default: break;
        }
    }
    m_pTreeBottom->Thaw();
}

// checks if there are respective children and colors the nodes
bool ClassBrowserBuilder::CreateSpecialFolders(wxTreeCtrl* tree, wxTreeItemId parent)
{
    bool hasGF = false;
    bool hasGV = false;
    bool hasGP = false;
    bool hasTD = false;

    // loop all tokens in global namespace and see if we have matches
    TokensTree* tt = m_pParser->GetTokens();
    for (TokenIdxSet::iterator it = tt->m_GlobalNameSpace.begin(); it != tt->m_GlobalNameSpace.end(); ++it)
    {
        Token* token = tt->at(*it);
        if (token && token->m_IsLocal && TokenMatchesFilter(token))
        {
            if (!hasGF && token->m_TokenKind == tkFunction)
                hasGF = true;
            else if (!hasGV && token->m_TokenKind == tkVariable)
                hasGV = true;
            else if (!hasGP && token->m_TokenKind == tkPreprocessor)
                hasGP = true;
            else if (!hasTD && token->m_TokenKind == tkTypedef)
                hasTD = true;
        }

        if (hasGF && hasGV && hasGP && hasTD)
            break; // we have everything, stop iterating...
    }

    wxTreeItemId gfuncs = AddNodeIfNotThere(m_pTreeTop, parent, _("Global functions"), PARSER_IMG_OTHERS_FOLDER, new CBTreeData(sfGFuncs, 0, tkFunction));
    wxTreeItemId tdef = AddNodeIfNotThere(m_pTreeTop, parent, _("Global typedefs"), PARSER_IMG_TYPEDEF_FOLDER, new CBTreeData(sfTypedef, 0, tkTypedef));
    wxTreeItemId gvars = AddNodeIfNotThere(m_pTreeTop, parent, _("Global variables"), PARSER_IMG_OTHERS_FOLDER, new CBTreeData(sfGVars, 0, tkVariable));
    wxTreeItemId preproc = AddNodeIfNotThere(m_pTreeTop, parent, _("Preprocessor symbols"), PARSER_IMG_PREPROC_FOLDER, new CBTreeData(sfPreproc, 0, tkPreprocessor));

    wxColour black = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    wxColour grey = wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);

    tree->SetItemTextColour(gfuncs, hasGF ? black : grey);
    tree->SetItemTextColour(gvars, hasGV ? black : grey);
    tree->SetItemTextColour(preproc, hasGP ? black : grey);
    tree->SetItemTextColour(tdef, hasTD ? black : grey);

    return hasGF || hasGV || hasGP || hasTD;
}

void ClassBrowserBuilder::ExpandItem(wxTreeItemId item)
{
    if (m_Aborted || Manager::IsAppShuttingDown() || !m_pTreeTop)
        return;

    CBTreeData* data = (CBTreeData*)m_pTreeTop->GetItemData(item);
    if (data)
    {
        switch (data->m_SpecialFolder)
        {
            case sfRoot:
            {
                CreateSpecialFolders(m_pTreeTop, item);
                AddChildrenOf(m_pTreeTop, item, -1, ~(tkFunction | tkVariable | tkPreprocessor | tkTypedef));
                break;
            }
            case sfBase: AddAncestorsOf(m_pTreeTop, item, data->m_pToken->GetSelf()); break;
            case sfDerived: AddDescendantsOf(m_pTreeTop, item, data->m_pToken->GetSelf(), false); break;
            case sfToken:
            {
                int kind = 0;
                switch (data->m_pToken->m_TokenKind)
                {
                    case tkClass:
                    {
                        // add base and derived classes folders
                        if (m_Options.showInheritance)
                        {
                            wxTreeItemId base = AddNodeIfNotThere(m_pTreeTop, item, _("Base classes"), PARSER_IMG_CLASS_FOLDER, new CBTreeData(sfBase, data->m_pToken, tkClass));
                            if (!data->m_pToken->m_DirectAncestors.empty())
                                m_pTreeTop->SetItemHasChildren(base);
                            wxTreeItemId derived = AddNodeIfNotThere(m_pTreeTop, item, _("Derived classes"), PARSER_IMG_CLASS_FOLDER, new CBTreeData(sfDerived, data->m_pToken, tkClass));
                            if (!data->m_pToken->m_Descendants.empty())
                                m_pTreeTop->SetItemHasChildren(derived);
                        }
                        kind = tkClass | tkEnum;
                        break;
                    }
                    case tkNamespace:
                        kind = tkNamespace | tkClass | tkEnum;
                        break;
                    default:
                        break;
                }
                if (kind != 0)
                    AddChildrenOf(m_pTreeTop, item, data->m_pToken->GetSelf(), kind);
                break;
            }
            default: break;
        }
    }
//    DBGLOG(_("E: %d items"), m_pTreeTop->GetCount());
}

void ClassBrowserBuilder::CollapseItem(wxTreeItemId item)
{
    if (m_Aborted || Manager::IsAppShuttingDown() || !m_pTreeTop)
        return;
//TODO: Check if wxGTK doesn't freeze anymore, now that we're not using threads
#ifndef __WXGTK__
    m_pTreeTop->CollapseAndReset(item); // this freezes gtk
#else
    m_pTreeTop->DeleteChildren(item);
#endif
    m_pTreeTop->SetItemHasChildren(item);
}

void ClassBrowserBuilder::SelectItem(wxTreeItemId item)
{
    if (m_Aborted || Manager::IsAppShuttingDown() || !m_pTreeTop)
        return;

    SelectNode(item);
}
