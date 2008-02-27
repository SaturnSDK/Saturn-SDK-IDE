/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include "classbrowser.h" // class's header file
#include "nativeparser.h"
#include <wx/intl.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/menu.h>
#include <wx/splitter.h>
#include <wx/button.h>
#include <wx/utils.h> // wxBusyCursor
#include <wx/xrc/xmlres.h>
#include <wx/tipwin.h>
#include <manager.h>
#include <configmanager.h>
#include <pluginmanager.h>
#include <editormanager.h>
#include <projectmanager.h>
#include <cbeditor.h>
#include <globals.h>
#include "cbstyledtextctrl.h"


#ifndef CB_PRECOMP
    #include "cbproject.h"
#endif

#include "ccdebuginfo.h"

class myTextCtrl : public wxTextCtrl
{
    public:
        myTextCtrl(ClassBrowser* cb,
                    wxWindow* parent,
                    wxWindowID id,
                    const wxString& value = wxEmptyString,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxTE_PROCESS_ENTER,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString& name = wxTextCtrlNameStr)
            : wxTextCtrl(parent, id, value, pos, size, style, validator, name),
            m_CB(cb)
            {}
        virtual ~myTextCtrl() {}
    protected:
        ClassBrowser* m_CB;

        // Intercept key presses to handle Enter. */
        void OnKey(wxKeyEvent& event)
        {
            if (event.GetKeyCode() == WXK_RETURN)
            {
                wxCommandEvent e;
                m_CB->OnSearch(e);
            }
            else
                event.Skip();
        }
        DECLARE_EVENT_TABLE()
};
BEGIN_EVENT_TABLE(myTextCtrl, wxTextCtrl)
	EVT_KEY_DOWN(myTextCtrl::OnKey)
END_EVENT_TABLE()

int idMenuJumpToDeclaration = wxNewId();
int idMenuJumpToImplementation = wxNewId();
int idMenuRefreshTree = wxNewId();
int idCBViewInheritance = wxNewId();
int idCBExpandNS = wxNewId();
int idCBViewModeFlat = wxNewId();
int idCBViewModeStructured = wxNewId();
int idMenuForceReparse = wxNewId();
int idMenuDebugSmartSense = wxNewId();

BEGIN_EVENT_TABLE(ClassBrowser, wxPanel)
	EVT_TREE_ITEM_ACTIVATED(XRCID("treeMembers"), ClassBrowser::OnTreeItemDoubleClick)
    EVT_TREE_ITEM_RIGHT_CLICK(XRCID("treeMembers"), ClassBrowser::OnTreeItemRightClick)

	EVT_TREE_ITEM_ACTIVATED(XRCID("treeAll"), ClassBrowser::OnTreeItemDoubleClick)
    EVT_TREE_ITEM_RIGHT_CLICK(XRCID("treeAll"), ClassBrowser::OnTreeItemRightClick)
    EVT_TREE_ITEM_EXPANDING(XRCID("treeAll"), ClassBrowser::OnTreeItemExpanding)
    EVT_TREE_ITEM_COLLAPSING(XRCID("treeAll"), ClassBrowser::OnTreeItemCollapsing)
    EVT_TREE_SEL_CHANGED(XRCID("treeAll"), ClassBrowser::OnTreeItemSelected)

    EVT_MENU(idMenuJumpToDeclaration, ClassBrowser::OnJumpTo)
    EVT_MENU(idMenuJumpToImplementation, ClassBrowser::OnJumpTo)
    EVT_MENU(idMenuRefreshTree, ClassBrowser::OnRefreshTree)
    EVT_MENU(idMenuForceReparse, ClassBrowser::OnForceReparse)
    EVT_MENU(idCBViewInheritance, ClassBrowser::OnCBViewMode)
    EVT_MENU(idCBExpandNS, ClassBrowser::OnCBExpandNS)
    EVT_MENU(idCBViewModeFlat, ClassBrowser::OnCBViewMode)
    EVT_MENU(idMenuDebugSmartSense, ClassBrowser::OnDebugSmartSense)
    EVT_CHOICE(XRCID("cmbView"), ClassBrowser::OnViewScope)
    EVT_BUTTON(XRCID("btnSearch"), ClassBrowser::OnSearch)
END_EVENT_TABLE()

// class constructor
ClassBrowser::ClassBrowser(wxWindow* parent, NativeParser* np)
    : m_NativeParser(np),
    m_TreeForPopupMenu(0),
	m_pParser(0L),
	m_pActiveProject(0),
	m_Semaphore(0, 1),
	m_pBuilderThread(0)
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

	wxXmlResource::Get()->LoadPanel(this, parent, _T("pnlCB"));
    m_Search = new myTextCtrl(this, parent, XRCID("txtSearch"));
    wxXmlResource::Get()->AttachUnknownControl(_T("txtSearch"), m_Search);

	m_Tree = XRCCTRL(*this, "treeAll", wxTreeCtrl);

    int filter = cfg->ReadInt(_T("/browser_display_filter"), bdfWorkspace);
    XRCCTRL(*this, "cmbView", wxChoice)->SetSelection(filter);

    int pos = cfg->ReadInt(_T("/splitter_pos"), 250);
    XRCCTRL(*this, "splitterWin", wxSplitterWindow)->SetSashPosition(pos, false);

    // if the classbrowser is put under the control of a wxFlatNotebook,
    // somehow the main panel is like "invisible" :/
    // so we force the correct color for the panel here...
    XRCCTRL(*this, "MainPanel", wxPanel)->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
}

// class destructor
ClassBrowser::~ClassBrowser()
{
    int pos = XRCCTRL(*this, "splitterWin", wxSplitterWindow)->GetSashPosition();
    Manager::Get()->GetConfigManager(_T("code_completion"))->Write(_T("/splitter_pos"), pos);

    UnlinkParser();

    if (m_pBuilderThread)
    {
        m_Semaphore.Post();
        m_pBuilderThread->Delete();
        m_pBuilderThread->Wait();
    }
}

void ClassBrowser::SetParser(Parser* parser)
{
	if (parser != m_pParser || m_pParser->ClassBrowserOptions().displayFilter == bdfProject)
	{
		UnlinkParser();
		if(parser)
		{
//            parser->AbortBuildingTree();
            parser->m_pClassBrowser = this;
            m_pParser = parser;
            UpdateView();
		}
	}
}

void ClassBrowser::UnlinkParser()
{
    if(m_pParser)
    {
        if(m_pParser->m_pClassBrowser == this)
        {
//            m_pParser->AbortBuildingTree();
            m_pParser->m_pClassBrowser = NULL;
        }
        m_pParser = NULL;
    }
}

void ClassBrowser::UpdateView()
{
    m_pActiveProject = 0;
    m_ActiveFilename.Clear();
	if (m_pParser && !Manager::IsAppShuttingDown())
	{
	    m_pActiveProject = Manager::Get()->GetProjectManager()->GetActiveProject();
	    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
	    if (ed)
	    {
            //m_ActiveFilename = ed->GetFilename().BeforeLast(_T('.'));
            // the above line is a bug (see https://developer.berlios.de/patch/index.php?func=detailpatch&patch_id=1559&group_id=5358)
            m_ActiveFilename = ed->GetFilename().AfterLast(wxFILE_SEP_PATH);
            m_ActiveFilename = ed->GetFilename().BeforeLast(wxFILE_SEP_PATH) + wxFILE_SEP_PATH + m_ActiveFilename.BeforeLast(_T('.'));

            m_ActiveFilename.Append(_T('.'));
	    }
		BuildTree();
	}
	else
		m_Tree->DeleteAllItems();
}

void ClassBrowser::ShowMenu(wxTreeCtrl* tree, wxTreeItemId id, const wxPoint& pt)
{
// NOTE: local variables are tricky! If you build two local menus
// and attach menu B to menu A, on function exit both menu A and menu B
// will be destroyed. But when destroying menu A, menu B will be destroyed
// again. Its already-freed memory will be accessed, generating a segfault.

// A safer approach is to make all menus heap-based, and delete the topmost
// on exit.

    m_TreeForPopupMenu = tree;
    if ( !id.IsOk() )
        return;

#if wxUSE_MENUS
	wxString caption;
    wxMenu *menu=new wxMenu(wxEmptyString);

	CBTreeData* ctd = (CBTreeData*)tree->GetItemData(id);
    if (ctd && ctd->m_pToken)
    {
        switch (ctd->m_pToken->m_TokenKind)
        {
            case tkConstructor:
            case tkDestructor:
            case tkFunction:
                if (ctd->m_pToken->m_ImplLine != 0 && !ctd->m_pToken->GetImplFilename().IsEmpty())
                    menu->Append(idMenuJumpToImplementation, _("Jump to &implementation"));
                // intentionally fall through
            default:
                menu->Append(idMenuJumpToDeclaration, _("Jump to &declaration"));
        }
    }

    if (tree == m_Tree)
    {
        // only in top tree
        if (menu->GetMenuItemCount() != 0)
            menu->AppendSeparator();

        menu->AppendCheckItem(idCBViewInheritance, _("Show inherited members"));
        menu->AppendCheckItem(idCBExpandNS, _("Auto-expand namespaces"));
        menu->Append(idMenuRefreshTree, _("&Refresh tree"));

        if (id == m_Tree->GetRootItem())
        {
            menu->AppendSeparator();
            menu->Append(idMenuForceReparse, _("Re-parse now"));
        }

        if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_SHIFT))
        {
            menu->AppendSeparator();
            menu->AppendCheckItem(idMenuDebugSmartSense, _("Debug SmartSense"));
            menu->Check(idMenuDebugSmartSense, s_DebugSmartSense);
        }

        menu->Check(idCBViewInheritance, m_pParser ? m_pParser->ClassBrowserOptions().showInheritance : false);
        menu->Check(idCBExpandNS, m_pParser ? m_pParser->ClassBrowserOptions().expandNS : false);
    }

    if (menu->GetMenuItemCount() != 0)
        PopupMenu(menu);
    delete menu; // Prevents memory leak
#endif // wxUSE_MENUS
}

bool ClassBrowser::FoundMatch(const wxString& search, wxTreeCtrl* tree, const wxTreeItemId& item)
{
    ClassTreeData* ctd = static_cast<ClassTreeData*>(tree->GetItemData(item));
    if (ctd && ctd->GetToken())
    {
        Token* token = ctd->GetToken();
        if (token->m_Name.Lower().StartsWith(search) ||
            token->m_Name.Lower().StartsWith(_T('~') + search))
        {
            return true;
        }
    }
    return false;
}

wxTreeItemId ClassBrowser::FindNext(const wxString& search, wxTreeCtrl* tree, const wxTreeItemId& start)
{
    wxTreeItemId ret;
    if (!start.IsOk())
        return ret;

    // look at siblings
    ret = tree->GetNextSibling(start);
    if (ret.IsOk())
        return ret;

    // ascend one level now and recurse
    return FindNext(search, tree, tree->GetItemParent(start));
}

bool ClassBrowser::RecursiveSearch(const wxString& search, wxTreeCtrl* tree, const wxTreeItemId& parent, wxTreeItemId& result)
{
    if (!parent.IsOk())
        return false;

    // first check the parent item
    if (FoundMatch(search, tree, parent))
    {
        result = parent;
        return true;
    }

    wxTreeItemIdValue cookie;
    wxTreeItemId child = tree->GetFirstChild(parent, cookie);

    if (!child.IsOk())
        return RecursiveSearch(search, tree, FindNext(search, tree, parent), result);

	while (child.IsOk())
	{
	    if (FoundMatch(search, tree, child))
        {
            result = child;
            return true;
        }
        if (tree->ItemHasChildren(child))
        {
            if (RecursiveSearch(search, tree, child, result))
                return true;
        }
        child = tree->GetNextChild(parent, cookie);
	}

    return RecursiveSearch(search, tree, FindNext(search, tree, parent), result);
}

// events

void ClassBrowser::OnTreeItemRightClick(wxTreeEvent& event)
{
    wxTreeCtrl* tree = (wxTreeCtrl*)event.GetEventObject();
	tree->SelectItem(event.GetItem());
    ShowMenu(tree, event.GetItem(), event.GetPoint());// + tree->GetPosition());
}

void ClassBrowser::OnJumpTo(wxCommandEvent& event)
{
    wxTreeCtrl* tree = m_TreeForPopupMenu;
	wxTreeItemId id = tree->GetSelection();
	CBTreeData* ctd = (CBTreeData*)tree->GetItemData(id);
    if (ctd)
    {
        cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
        if (prj)
        {
            wxString base = prj->GetBasePath();
            wxFileName fname;
            if (event.GetId() == idMenuJumpToImplementation)
                fname.Assign(ctd->m_pToken->GetImplFilename());
            else
                fname.Assign(ctd->m_pToken->GetFilename());
            NormalizePath(fname,base);
        	cbEditor* ed = Manager::Get()->GetEditorManager()->Open(fname.GetFullPath());
			if (ed)
			{
                int line;
                if (event.GetId() == idMenuJumpToImplementation)
                    line = ctd->m_pToken->m_ImplLine - 1;
                else
                    line = ctd->m_pToken->m_Line - 1;
				ed->GotoLine(line);
//				// try to move the caret on the exact token
//				int lineOffset = ed->GetControl()->GetCurLine().Find(ctd->m_pToken->m_Name);
//				if (lineOffset != wxNOT_FOUND)
//				{
//                    int pos = ed->GetControl()->PositionFromLine(line) + lineOffset;
//                    ed->GetControl()->GotoPos(pos);
//                    // select the token
//                    int posend = ed->GetControl()->WordEndPosition(pos, true);
//                    ed->GetControl()->SetSelection(pos, posend);
//				}
			}
        }
    }
}

void ClassBrowser::OnTreeItemDoubleClick(wxTreeEvent& event)
{
    wxTreeCtrl* tree = (wxTreeCtrl*)event.GetEventObject();
	wxTreeItemId id = event.GetItem();
	CBTreeData* ctd = (CBTreeData*)tree->GetItemData(id);
    if (ctd && ctd->m_pToken)
    {
        if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_SHIFT))
        {
            CCDebugInfo info(tree, m_pParser, ctd->m_pToken);
            info.ShowModal();
            return;
        }

        cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
        if (prj)
        {
			bool toImp = false;
			switch (ctd->m_pToken->m_TokenKind)
			{
			case tkConstructor:
            case tkDestructor:
            case tkFunction:
                if (ctd->m_pToken->m_ImplLine != 0 && !ctd->m_pToken->GetImplFilename().IsEmpty())
                    toImp = true;
				break;
			default:
				break;
			}

            wxString base = prj->GetBasePath();
            wxFileName fname;
            if (toImp)
                fname.Assign(ctd->m_pToken->GetImplFilename());
            else
                fname.Assign(ctd->m_pToken->GetFilename());

            NormalizePath(fname, base);
        	cbEditor* ed = Manager::Get()->GetEditorManager()->Open(fname.GetFullPath());
			if (ed)
			{
				int line;
                if (toImp)
                    line = ctd->m_pToken->m_ImplLine - 1;
                else
                    line = ctd->m_pToken->m_Line - 1;
				ed->GotoLine(line);
//				// try to move the caret on the exact token
//				int lineOffset = ed->GetControl()->GetCurLine().Find(ctd->m_pToken->m_Name);
//				if (lineOffset != wxNOT_FOUND)
//				{
//                    int pos = ed->GetControl()->PositionFromLine(line) + lineOffset;
//                    ed->GetControl()->GotoPos(pos);
//                    // select the token
//                    int posend = ed->GetControl()->WordEndPosition(pos, true);
//                    ed->GetControl()->SetSelection(pos, posend);
//				}

				wxFocusEvent ev(wxEVT_SET_FOCUS);
				ev.SetWindow(this);
				ed->GetControl()->AddPendingEvent(ev);
			}
        }
    }
}

void ClassBrowser::OnRefreshTree(wxCommandEvent& event)
{
	UpdateView();
}

void ClassBrowser::OnForceReparse(wxCommandEvent& event)
{
    if (m_NativeParser)
        m_NativeParser->ForceReparseActiveProject();
}

void ClassBrowser::OnCBViewMode(wxCommandEvent& event)
{
	if (!m_pParser)
		return;

	if (event.GetId() == idCBViewInheritance)
		m_pParser->ClassBrowserOptions().showInheritance = event.IsChecked();

	m_pParser->WriteOptions();
	UpdateView();
}

void ClassBrowser::OnCBExpandNS(wxCommandEvent& event)
{
	if (!m_pParser)
		return;

	if (event.GetId() == idCBExpandNS)
		m_pParser->ClassBrowserOptions().expandNS = event.IsChecked();

	m_pParser->WriteOptions();
	UpdateView();
}

void ClassBrowser::OnViewScope(wxCommandEvent& event)
{
	if (m_pParser)
	{
		m_pParser->ClassBrowserOptions().displayFilter = (BrowserDisplayFilter)event.GetSelection();
		m_pParser->WriteOptions();
        UpdateView();
	}
	else
	{
	    // we have no parser; just write the setting in the configuration
        Manager::Get()->GetConfigManager(_T("code_completion"))->Write(_T("/browser_display_filter"), (int)event.GetSelection());
	}
}

void ClassBrowser::OnDebugSmartSense(wxCommandEvent& event)
{
    s_DebugSmartSense = !s_DebugSmartSense;
}

void ClassBrowser::OnSearch(wxCommandEvent& event)
{
    new wxTipWindow(this, _("Searching the symbols tree is currently disabled.\nWe are sorry for the inconvenience."), 240);
    return;

    wxString search = m_Search->GetValue().Lower();
    if (search.IsEmpty())
        return;

    // search under the selected node
    wxTreeItemId start = m_Tree->GetSelection();
    if (!start.IsOk()) // if it's not valid, search the whole tree
        start = m_Tree->GetRootItem();

    // if the selection matches, skip it; the user already knows...
    if (FoundMatch(search, m_Tree, start))
    {
        if (m_Tree->ItemHasChildren(start))
        {
            wxTreeItemIdValue cookie;
            start = m_Tree->GetFirstChild(start, cookie);
        }
        else
            start = FindNext(search, m_Tree, start);
    }

    // as a fallback, start from the root node
    if (!start.IsOk())
        start = m_Tree->GetRootItem();

    wxTreeItemId result;
    if (RecursiveSearch(search, m_Tree, start, result))
    {
        m_Tree->SelectItem(result);
        m_Tree->EnsureVisible(result);
    }
    else
    {
        // if the search failed and the start node wasn't the root, search again from the root
        if (start != m_Tree->GetRootItem())
        {
            if (RecursiveSearch(m_Search->GetValue().Lower(), m_Tree, m_Tree->GetRootItem(), result))
            {
                m_Tree->SelectItem(result);
                m_Tree->EnsureVisible(result);
            }
        }
    }
}

void ClassBrowser::BuildTree()
{
    if (Manager::IsAppShuttingDown())
        return;

    bool created_builderthread = false;

    // create the thread if needed
    if (!m_pBuilderThread)
    {
        m_pBuilderThread = new ClassBrowserBuilderThread(m_Semaphore, &m_pBuilderThread);
        m_pBuilderThread->Create();
        m_pBuilderThread->Run();
#ifdef __WXMSW__
        // workaround only needed for windows (DDE server issue)
        created_builderthread = true;
#endif
    }

    // initialise it
    m_pBuilderThread->Init(m_pParser,
                            m_Tree,
                            XRCCTRL(*this, "treeMembers", wxTreeCtrl),
                            m_ActiveFilename,
                            m_pActiveProject,
                            m_pParser->ClassBrowserOptions(),
                            m_pParser->GetTokens(),
                            created_builderthread);

    // and launch it
    if (!created_builderthread)
    {
        m_Semaphore.Post();
    }
} // end of BuildTree

void ClassBrowser::OnTreeItemExpanding(wxTreeEvent& event)
{
    if (m_pBuilderThread)
        m_pBuilderThread->ExpandItem(event.GetItem());
    event.Allow();
}

void ClassBrowser::OnTreeItemCollapsing(wxTreeEvent& event)
{
    if (m_pBuilderThread)
        m_pBuilderThread->CollapseItem(event.GetItem());
    event.Allow();
}

void ClassBrowser::OnTreeItemSelected(wxTreeEvent& event)
{
    if (m_pBuilderThread)
        m_pBuilderThread->SelectItem(event.GetItem());
    event.Allow();
}
