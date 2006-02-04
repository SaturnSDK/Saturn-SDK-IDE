/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/imaglist.h>
    #include <wx/menu.h>
    #include <wx/splitter.h>
    #include <wx/filename.h>

    #include "projectmanager.h" // class's header file
    #include "sdk_events.h"
    #include "manager.h"
    #include "configmanager.h"
    #include "cbproject.h"
    #include "messagemanager.h"
    #include "pluginmanager.h"
    #include "editormanager.h"
    #include "workspaceloader.h"
    #include "cbworkspace.h"
    #include "cbeditor.h"
    #include "xtra_classes.h"
    #include <wx/dir.h>
#endif

#include <wx/utils.h>
#include <wx/textdlg.h>
#include <wx/progdlg.h>
#include <wxFlatNotebook.h>

#include "incrementalselectlistdlg.h"
#include "filegroupsandmasks.h"
#include "projectsfilemasksdlg.h"
#include "projectdepsdlg.h"
#include "multiselectdlg.h"


// maximum number of items in "Open with" context menu
static const unsigned int MAX_OPEN_WITH_ITEMS = 20; // keep it in sync with below array!
static const int idOpenWith[] =
{
    wxNewId(), wxNewId(), wxNewId(), wxNewId(), wxNewId(),
    wxNewId(), wxNewId(), wxNewId(), wxNewId(), wxNewId(),
    wxNewId(), wxNewId(), wxNewId(), wxNewId(), wxNewId(),
    wxNewId(), wxNewId(), wxNewId(), wxNewId(), wxNewId(),
};
// special entry: force open with internal editor
static const int idOpenWithInternal = wxNewId();

// static
bool ProjectManager::s_CanShutdown = true;


int ID_ProjectManager = wxNewId();
int idMenuSetActiveProject = wxNewId();
int idMenuOpenFile = wxNewId();
int idMenuCloseProject = wxNewId();
int idMenuCloseFile = wxNewId();
int idMenuAddFilePopup = wxNewId();
int idMenuAddFilesRecursivelyPopup = wxNewId();
int idMenuAddFile = wxNewId();
int idMenuAddFilesRecursively = wxNewId();
int idMenuRemoveFolderFilesPopup = wxNewId();
int idMenuRemoveFilePopup = wxNewId();
int idMenuRemoveFile = wxNewId();
int idMenuProjectProperties = wxNewId();
int idMenuFileProperties = wxNewId();
int idMenuTreeProjectProperties = wxNewId();
int idMenuTreeFileProperties = wxNewId();
int idMenuGotoFile = wxNewId();
int idMenuExecParams = wxNewId();
int idMenuViewCategorize = wxNewId();
int idMenuViewUseFolders = wxNewId();
int idMenuViewFileMasks = wxNewId();
int idMenuNextProject = wxNewId();
int idMenuPriorProject = wxNewId();
int idMenuProjectTreeProps = wxNewId();
int idMenuProjectUp = wxNewId();
int idMenuProjectDown = wxNewId();
int idMenuViewCategorizePopup = wxNewId();
int idMenuViewUseFoldersPopup = wxNewId();
int idMenuTreeRenameWorkspace = wxNewId();
int idMenuTreeSaveWorkspace = wxNewId();
int idMenuTreeSaveAsWorkspace = wxNewId();
int idMenuTreeCloseWorkspace = wxNewId();

static const int idNB = wxNewId();
static const int idNB_TabTop = wxNewId();
static const int idNB_TabBottom = wxNewId();

#ifndef __WXMSW__
/*
    Under wxGTK, I have noticed that wxTreeCtrl is not sending a EVT_COMMAND_RIGHT_CLICK
    event when right-clicking on the client area.
    This is a "proxy" wxTreeCtrl descendant that handles this for us...
*/

class PrjTree : public wxTreeCtrl
{
	public:
		PrjTree(wxWindow* parent, int id) : wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxNO_BORDER) {}
	protected:
		void OnRightClick(wxMouseEvent& event)
		{
            if(!this) return;
		    //Manager::Get()->GetMessageManager()->DebugLog("OnRightClick");
		    int flags;
		    HitTest(wxPoint(event.GetX(), event.GetY()), flags);
		    if (flags & (wxTREE_HITTEST_ABOVE | wxTREE_HITTEST_BELOW | wxTREE_HITTEST_NOWHERE))
		    {
		    	// "proxy" the call
			    wxCommandEvent e(wxEVT_COMMAND_RIGHT_CLICK, ID_ProjectManager);
				wxPostEvent(GetParent(), e);
			}
			else
		    	event.Skip();
		}
		DECLARE_EVENT_TABLE();
};
BEGIN_EVENT_TABLE(PrjTree, wxTreeCtrl)
	EVT_RIGHT_DOWN(PrjTree::OnRightClick)
END_EVENT_TABLE()
#endif // !__WXMSW__

BEGIN_EVENT_TABLE(ProjectManager, wxEvtHandler)
    EVT_TREE_ITEM_ACTIVATED(ID_ProjectManager, ProjectManager::OnProjectFileActivated)
    EVT_TREE_ITEM_RIGHT_CLICK(ID_ProjectManager, ProjectManager::OnTreeItemRightClick)
    EVT_COMMAND_RIGHT_CLICK(ID_ProjectManager, ProjectManager::OnRightClick)
    EVT_MENU_RANGE(idOpenWith[0], idOpenWith[MAX_OPEN_WITH_ITEMS - 1], ProjectManager::OnOpenWith)
    EVT_MENU(idOpenWithInternal, ProjectManager::OnOpenWith)
    EVT_MENU(idNB_TabTop, ProjectManager::OnTabPosition)
    EVT_MENU(idNB_TabBottom, ProjectManager::OnTabPosition)
    EVT_MENU(idMenuSetActiveProject, ProjectManager::OnSetActiveProject)
    EVT_MENU(idMenuNextProject, ProjectManager::OnSetActiveProject)
    EVT_MENU(idMenuPriorProject, ProjectManager::OnSetActiveProject)
    EVT_MENU(idMenuProjectUp, ProjectManager::OnSetActiveProject)
    EVT_MENU(idMenuProjectDown, ProjectManager::OnSetActiveProject)
    EVT_MENU(idMenuTreeRenameWorkspace, ProjectManager::OnRenameWorkspace)
    EVT_MENU(idMenuTreeSaveWorkspace, ProjectManager::OnSaveWorkspace)
    EVT_MENU(idMenuTreeSaveAsWorkspace, ProjectManager::OnSaveAsWorkspace)
    EVT_MENU(idMenuTreeCloseWorkspace, ProjectManager::OnCloseWorkspace)
    EVT_MENU(idMenuAddFile, ProjectManager::OnAddFileToProject)
    EVT_MENU(idMenuAddFilesRecursively, ProjectManager::OnAddFilesToProjectRecursively)
    EVT_MENU(idMenuRemoveFile, ProjectManager::OnRemoveFileFromProject)
    EVT_MENU(idMenuAddFilePopup, ProjectManager::OnAddFileToProject)
    EVT_MENU(idMenuAddFilesRecursivelyPopup, ProjectManager::OnAddFilesToProjectRecursively)
    EVT_MENU(idMenuRemoveFolderFilesPopup, ProjectManager::OnRemoveFileFromProject)
    EVT_MENU(idMenuRemoveFilePopup, ProjectManager::OnRemoveFileFromProject)
    EVT_MENU(idMenuCloseProject, ProjectManager::OnCloseProject)
    EVT_MENU(idMenuCloseFile, ProjectManager::OnCloseFile)
    EVT_MENU(idMenuOpenFile, ProjectManager::OnOpenFile)
    EVT_MENU(idMenuProjectProperties, ProjectManager::OnProperties)
    EVT_MENU(idMenuFileProperties, ProjectManager::OnProperties)
    EVT_MENU(idMenuTreeProjectProperties, ProjectManager::OnProperties)
    EVT_MENU(idMenuTreeFileProperties, ProjectManager::OnProperties)
	EVT_MENU(idMenuGotoFile, ProjectManager::OnGotoFile)
    EVT_MENU(idMenuExecParams, ProjectManager::OnExecParameters)
    EVT_MENU(idMenuViewCategorize, ProjectManager::OnViewCategorize)
    EVT_MENU(idMenuViewCategorizePopup, ProjectManager::OnViewCategorize)
    EVT_MENU(idMenuViewUseFolders, ProjectManager::OnViewUseFolders)
    EVT_MENU(idMenuViewUseFoldersPopup, ProjectManager::OnViewUseFolders)
    EVT_MENU(idMenuViewFileMasks, ProjectManager::OnViewFileMasks)
    EVT_IDLE(ProjectManager::OnIdle)
END_EVENT_TABLE()

// class constructor
ProjectManager::ProjectManager()
	: m_pTree(0),
    m_pWorkspace(0),
	m_pTopEditor(0),
    m_TreeCategorize(false),
    m_TreeUseFolders(true),
    m_TreeFreezeCounter(0),
    m_IsLoadingProject(false),
	m_IsLoadingWorkspace(false),
	m_InitialDir(_T(""))
{
    SC_CONSTRUCTOR_BEGIN

    m_pNotebook = new wxFlatNotebook(Manager::Get()->GetAppWindow(), idNB);
    m_pNotebook->SetWindowStyleFlag(Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/environment/project_tabs_style"), wxFNB_NO_X_BUTTON));
    m_pNotebook->SetImageList(new wxFlatNotebookImageList);

    wxMenu* NBmenu = new wxMenu(); // deleted automatically by wxFlatNotebook
    NBmenu->Append(idNB_TabTop, _("Tabs at top"));
    NBmenu->Append(idNB_TabBottom, _("Tabs at bottom"));
    m_pNotebook->SetRightClickMenu(NBmenu);

    m_InitialDir=wxFileName::GetCwd();
    m_pActiveProject = 0L;
    m_pProjects = new ProjectsArray;
    m_pProjects->Clear();
	// m_pPanel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxCLIP_CHILDREN);
    InitPane();

	m_pFileGroups = new FilesGroupsAndMasks;

	ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("project_manager"));
	m_TreeCategorize = cfg->ReadBool(_T("/categorize_tree"), true);
	m_TreeUseFolders = cfg->ReadBool(_T("/use_folders"), true);

    RebuildTree();

	// Event handling. This must be THE LAST THING activated on startup.
	// Constructors and destructors must always follow the LIFO rule:
	// Last in, first out.
    Manager::Get()->GetAppWindow()->PushEventHandler(this);
}

// class destructor
ProjectManager::~ProjectManager()
{
    SC_DESTRUCTOR_BEGIN

    // this is a core manager, so it is removed when the app is shutting down.
    // in this case, the app has already un-hooked us, so no need to do it ourselves...
//	Manager::Get()->GetAppWindow()->RemoveEventHandler(this);

    if (m_pWorkspace)
        delete m_pWorkspace;
    m_pWorkspace = 0;

    int count = m_pProjects->GetCount();
    for (int i = 0; i < count; ++i)
    {
        cbProject* project = m_pProjects->Item(i);
        if (project)
            delete project;
    }
    m_pProjects->Clear();

    delete m_pProjects;m_pProjects = 0;
    delete m_pImages;m_pImages = 0;
	delete m_pFileGroups;m_pFileGroups = 0;

    delete m_pNotebook->GetImageList();
    m_pNotebook->Destroy();

	SC_DESTRUCTOR_END
}

void ProjectManager::InitPane()
{
    SANITY_CHECK();
    if(Manager::isappShuttingDown())
        return;
    if(m_pTree)
        return;
    BuildTree();
    m_pNotebook->AddPage(m_pTree, _("Projects"));
}

int ProjectManager::WorkspaceIconIndex()
{
    return (int)fvsLast + 0;
}

int ProjectManager::ProjectIconIndex()
{
    return (int)fvsLast + 1;
}

int ProjectManager::FolderIconIndex()
{
    return (int)fvsLast + 2;
}

void ProjectManager::BuildTree()
{
    #ifndef __WXMSW__
        m_pTree = new PrjTree(m_pNotebook, ID_ProjectManager);
    #else
        m_pTree = new wxTreeCtrl(m_pNotebook, ID_ProjectManager, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxNO_BORDER);
    #endif

    static const wxString imgs[] = {
        _T("file.png"),
        _T("file-missing.png"),
        _T("file-modified.png"),
        _T("file-readonly.png"),
        _T("rc-file-added.png"),
        _T("rc-file-conflict.png"),
        _T("rc-file-missing.png"),
        _T("rc-file-modified.png"),
        _T("rc-file-outofdate.png"),
        _T("rc-file-uptodate.png"),
        _T("rc-file-requireslock.png"),
        _T("gohome.png"),
        _T("codeblocks.png"),
        _T("folder_open.png"),
    };
    wxBitmap bmp;
    m_pImages = new wxImageList(16, 16);
    wxString prefix = ConfigManager::ReadDataPath() + _T("/images/");

    for (int i = 0; i < 14; ++i)
    {
//        wxMessageBox(wxString::Format(_T("%d: %s"), i, wxString(prefix + imgs[i]).c_str()));
        bmp.LoadFile(prefix + imgs[i], wxBITMAP_TYPE_PNG); // workspace
        m_pImages->Add(bmp);
    }
    m_pTree->SetImageList(m_pImages);

//    // make sure tree is not "frozen"
//    UnfreezeTree(true);
}

void ProjectManager::CreateMenu(wxMenuBar* menuBar)
{
/* TODO (mandrav#1#): Move menu items from main.cpp, here */
	if (menuBar)
	{
		int pos = menuBar->FindMenu(_("Sea&rch"));
		wxMenu* menu = menuBar->GetMenu(pos);
		if (menu)
			menu->Append(idMenuGotoFile, _("Goto file...\tAlt-G"));

		pos = menuBar->FindMenu(_("&File"));
		menu = menuBar->GetMenu(pos);
		if (menu)
		{
            menu->Insert(menu->GetMenuItemCount() - 1, idMenuFileProperties, _("Properties"));
            menu->Insert(menu->GetMenuItemCount() - 1, wxID_SEPARATOR, _T("")); // instead of AppendSeparator();
        }

        pos = menuBar->FindMenu(_("&Project"));
		menu = menuBar->GetMenu(pos);
		if (menu)
        {
            menu->AppendSeparator();
            menu->Append(idMenuAddFile, _("Add files..."), _("Add files to the project"));
            menu->Append(idMenuAddFilesRecursively, _("Add files recursively..."), _("Add files recursively to the project"));
            menu->Append(idMenuRemoveFile, _("Remove files..."), _("Remove files from the project"));

/* FIXME (mandrav#1#): Move this submenu creation in a function.
It is duplicated in ShowMenu() */

            ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("project_manager"));

            wxMenu* treeprops = new wxMenu;
            treeprops->Append(idMenuProjectUp, _("Move project up\tCtrl-Shift-Up"), _("Move project up in project tree"));
            treeprops->Append(idMenuProjectDown, _("Move project down\tCtrl-Shift-Down"), _("Move project down in project tree"));
            treeprops->AppendSeparator();
            treeprops->Append(idMenuPriorProject, _("Activate prior project\tAlt-F5"), _("Activate prior project in open projects list"));
            treeprops->Append(idMenuNextProject, _("Activate next project\tAlt-F6"), _("Activate next project in open projects list"));
            treeprops->AppendSeparator();
            treeprops->AppendCheckItem(idMenuViewCategorize, _("Categorize by file types"));
            treeprops->AppendCheckItem(idMenuViewUseFolders, _("Display folders as on disk"));
            treeprops->Check(idMenuViewCategorize, cfg->ReadBool(_T("/categorize_tree"), true));
            treeprops->Check(idMenuViewUseFolders, cfg->ReadBool(_T("/use_folders"), true));
            treeprops->Append(idMenuViewFileMasks, _("Edit file types && categories..."));
            menu->AppendSeparator();
            menu->Append(idMenuProjectTreeProps, _("Project tree"), treeprops);

            menu->Append(idMenuExecParams, _("Set &programs' arguments..."), _("Set execution parameters for the targets of this project"));
            menu->Append(idMenuProjectProperties, _("Properties"));
        }
	}
}

void ProjectManager::ReleaseMenu(wxMenuBar* menuBar)
{
    SANITY_CHECK();
}

void ProjectManager::SetProject(cbProject* project, bool refresh)
{
    SANITY_CHECK();
	if (project != m_pActiveProject)
	{
        if (m_pWorkspace)
            m_pWorkspace->SetModified(true);
    }
    else
        return; // already active
    if (m_pActiveProject)
        m_pTree->SetItemBold(m_pActiveProject->GetProjectNode(), false);
    m_pActiveProject = project;
    if (m_pActiveProject)
        m_pTree->SetItemBold(m_pActiveProject->GetProjectNode(), true);
	if (refresh)
		RebuildTree();

    if (m_pActiveProject)
        m_pTree->EnsureVisible(m_pActiveProject->GetProjectNode());

	CodeBlocksEvent event(cbEVT_PROJECT_ACTIVATE);
	event.SetProject(m_pActiveProject);
	Manager::Get()->GetPluginManager()->NotifyPlugins(event);
}

void ProjectManager::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
    SANITY_CHECK();
    if ( !id.IsOk() )
        return;

	wxString caption;
    wxMenu menu;

    FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(id);

    // if it is not the workspace, add some more options
    if (ftd)
    {
	    // if it is a project...
    	if (ftd->GetKind() == FileTreeData::ftdkProject)
    	{
			if (ftd->GetProject() != m_pActiveProject)
				menu.Append(idMenuSetActiveProject, _("Activate project"));
    		menu.Append(idMenuCloseProject, _("Close project"));
    		menu.AppendSeparator();
    	    menu.Append(idMenuAddFilePopup, _("Add files..."));
    	    menu.Append(idMenuAddFilesRecursivelyPopup, _("Add files recursively..."));
            menu.Append(idMenuRemoveFile, _("Remove files..."));
    	}

        // if it is a file...
    	else if (ftd->GetKind() == FileTreeData::ftdkFile)
    	{
			// selected project file
			ProjectFile* pf = ftd->GetProjectFile();
			// is it already open in the editor?
            EditorBase* ed = Manager::Get()->GetEditorManager()->IsOpen(pf->file.GetFullPath());

			if (ed)
			{
				// is it already active?
				bool active = Manager::Get()->GetEditorManager()->GetActiveEditor() == ed;

				if (!active)
				{
					caption.Printf(_("Switch to %s"), m_pTree->GetItemText(id).c_str());
					menu.Append(idMenuOpenFile, caption);
				}
				caption.Printf(_("Close %s"), m_pTree->GetItemText(id).c_str());
				menu.Append(idMenuCloseFile, caption);
			}
			else
			{
				caption.Printf(_("Open %s"), m_pTree->GetItemText(id).c_str());
				menu.Append(idMenuOpenFile, caption);
			}

			// add "Open with" menu
			wxMenu* openWith = new wxMenu;
            PluginsArray mimes = Manager::Get()->GetPluginManager()->GetMimeOffers();
            for (unsigned int i = 0; i < mimes.GetCount() && i < MAX_OPEN_WITH_ITEMS; ++i)
            {
                cbMimePlugin* plugin = (cbMimePlugin*)mimes[i];
                if (plugin && plugin->CanHandleFile(m_pTree->GetItemText(id)))
                    openWith->Append(idOpenWith[i], plugin->GetInfo()->title);
            }
            openWith->AppendSeparator();
            openWith->Append(idOpenWithInternal, _("Internal editor"));
            menu.Append(wxID_ANY, _("Open with..."), openWith);

    		menu.AppendSeparator();
    	    menu.Append(idMenuRemoveFilePopup, _("Remove file from project"));
    	}

        // if it is a folder...
    	else if (ftd->GetKind() == FileTreeData::ftdkFolder)
    	{
    	    menu.Append(idMenuAddFilePopup, _("Add files..."));
    	    menu.Append(idMenuAddFilesRecursivelyPopup, _("Add files recursively..."));
    		menu.AppendSeparator();
            menu.Append(idMenuRemoveFile, _("Remove files..."));
    		wxFileName f(ftd->GetFolder());
    		f.MakeRelativeTo(ftd->GetProject()->GetBasePath());
    	    menu.Append(idMenuRemoveFolderFilesPopup, wxString::Format(_("Remove %s*"), f.GetFullPath().c_str()));
    	}

        // ask any plugins to add items in this menu
        Manager::Get()->GetPluginManager()->AskPluginsForModuleMenu(mtProjectManager, &menu, ftd);

        // more project options
        if (ftd->GetKind() == FileTreeData::ftdkProject)
        {
            // project
/* FIXME (mandrav#1#): Move this submenu creation in a function.
It is duplicated in CreateMenu() */
            menu.AppendSeparator();

            ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("project_manager"));

            wxMenu* treeprops = new wxMenu;
            treeprops->Append(idMenuProjectUp, _("Move project up\tCtrl-Shift-Up"), _("Move project up in project tree"));
            treeprops->Append(idMenuProjectDown, _("Move project down\tCtrl-Shift-Down"), _("Move project down in project tree"));
            treeprops->AppendSeparator();
            treeprops->Append(idMenuPriorProject, _("Activate prior project\tAlt-F5"), _("Activate prior project in open projects list"));
            treeprops->Append(idMenuNextProject, _("Activate next project\tAlt-F6"), _("Activate next project in open projects list"));
            treeprops->AppendSeparator();
/* NOTE (mandrav#1#): If this is moved in a new function,\
it differs from the block currently in CreateMenu() by the following two IDs */
            treeprops->AppendCheckItem(idMenuViewCategorizePopup, _("Categorize by file types"));
            treeprops->AppendCheckItem(idMenuViewUseFoldersPopup, _("Display folders as on disk"));
            treeprops->Check(idMenuViewCategorizePopup, cfg->ReadBool(_T("/categorize_tree"), true));
            treeprops->Check(idMenuViewUseFoldersPopup, cfg->ReadBool(_T("/use_folders"), true));
            treeprops->Append(idMenuViewFileMasks, _("Edit file types && categories..."));

            menu.Append(idMenuProjectTreeProps, _("Project tree"), treeprops);
            menu.Append(idMenuTreeProjectProperties, _("Properties"));
        }

        // more file options
        else if (ftd->GetKind() == FileTreeData::ftdkFile)
        {
            menu.AppendSeparator();
            menu.Append(idMenuTreeFileProperties, _("Properties"));
        }
    }
    else if (!ftd && m_pWorkspace)
    {
        menu.Append(idMenuTreeRenameWorkspace, _("Rename workspace"));
        menu.AppendSeparator();
        menu.Append(idMenuTreeSaveWorkspace, _("Save workspace"));
        menu.Append(idMenuTreeSaveAsWorkspace, _("Save workspace as..."));
        menu.AppendSeparator();
        menu.Append(idMenuTreeCloseWorkspace, _("Close workspace"));
    }

    if (menu.GetMenuItemCount() != 0)
        m_pTree->PopupMenu(&menu, pt);
}

cbProject* ProjectManager::IsOpen(const wxString& filename)
{
    SANITY_CHECK(0L);
	if (filename.IsEmpty())
		return 0L;
    int count = m_pProjects->GetCount();
    for (int i = 0; i < count; ++i)
    {
        cbProject* project = m_pProjects->Item(i);
        if(project)
        {
#ifdef __WXMSW__
            // MS Windows Filenames are case-insensitive, we have to
            // avoid opening the same project if the files are only
            // different in upper/lowercase.
            if(project->GetFilename().Lower().Matches(filename.Lower()))
                return project;
#else
            if (project->GetFilename().Matches(filename))
                return project;
#endif
        }
    }
	return 0L;
}

wxMenu* ProjectManager::GetProjectMenu()
{
    SANITY_CHECK(0L);
    wxMenu* result = 0L;
    do
    {
        wxFrame* frame = Manager::Get()->GetAppWindow();
        if(!frame)
            break;
        wxMenuBar* mb = frame->GetMenuBar();
        if(!mb)
            break;
        result = mb->GetMenu(mb->FindMenu(_("&Project")));
        break;
    }while(false);
    return result;
}

cbProject* ProjectManager::LoadProject(const wxString& filename, bool activateIt)
{
    SANITY_CHECK(0L);
    cbProject* result = 0;

    // disallow application shutdown while opening files
    // WARNING: remember to set it to true, when exiting this function!!!
    s_CanShutdown = false;
	cbProject* project = IsOpen(filename);

	// "Try" block (loop which only gets executed once)
	// These blocks are extremely useful in constructs that need
	// premature exits. Instead of having multiple return points,
	// multiple return values and/or gotos,
	// you just break out of the loop (which only gets executed once) to exit.
	do
	{
        if (project)
        {
            if (m_pWorkspace)
                m_pWorkspace->SetModified(true);
            // we 're done
            result = project;
            break;
        }
//        if (!wxFileExists(filename))
//            return 0;
        m_IsLoadingProject=true;
        project = new cbProject(filename);
        if(!sanity_check())
            break; // sanity check
        // We need to do this because creating cbProject allows the app to be
        // closed in the middle of the operation. So the class destructor gets
        // called in the middle of a method call.

        if (!project->IsLoaded())
        {
            delete project;
            break;
        }

        if(!sanity_check())
            break; // sanity check

        m_pProjects->Add(project);

        // to avoid tree flickering, we 'll manually call here the project's BuildTree
        // but before we do it, remove bold from current active project (if any)
// Commented it by Heromyth. I don't think it is a good thing to activate the project imediately after opening one.
//        if (m_pActiveProject)
//            m_pTree->SetItemBold(m_pActiveProject->GetProjectNode(), false);
        // ok, set the new project

        if (!m_IsLoadingWorkspace)
        {
            project->BuildTree(m_pTree, m_TreeRoot, m_TreeCategorize, m_TreeUseFolders, m_pFileGroups);
            m_pTree->Expand(project->GetProjectNode());
            m_pTree->Expand(m_TreeRoot); // make sure the root node is open
        }
        if (activateIt)
            SetProject(project, !m_IsLoadingWorkspace);

        if (!sanity_check())
            break; // sanity check

        project->LoadLayout();
        project->SetModified(false);
        if (!sanity_check())
            break; // sanity check
        if (m_pWorkspace)
            m_pWorkspace->SetModified(true);
        // we 're done

        result = project;

        // notify plugins that the project is loaded
        // moved here from cbProject::Open() because code-completion
        // kicks in too early and the perceived loading time is long...
        CodeBlocksEvent event(cbEVT_PROJECT_OPEN);
        event.SetProject(project);
        Manager::Get()->GetPluginManager()->NotifyPlugins(event);

        break;
    } while(false);
    // we 're done

    m_IsLoadingProject=false;

    #ifdef USE_OPENFILES_TREE
    Manager::Get()->GetEditorManager()->RebuildOpenedFilesTree();
    #endif
	// Restore child windows' display
	// if(mywin)
    //    mywin->Show();

    s_CanShutdown = true;
    return result;
}

cbProject* ProjectManager::NewProject(const wxString& filename)
{
    return LoadProject(filename);
}

bool ProjectManager::QueryCloseAllProjects()
{
    SANITY_CHECK(true);
    unsigned int i;

    if (!Manager::Get()->GetEditorManager()->QueryCloseAll())
        return false;

    for(i=0;i<m_pProjects->GetCount();i++)
    {
        // Ask for saving modified projects. However,
        // we already asked to save projects' files;
        // do not ask again
        if(!QueryCloseProject(m_pProjects->Item(i),true))
            return false;
    }
    return true;
}

bool ProjectManager::QueryCloseProject(cbProject *proj,bool dontsavefiles)
{
    SANITY_CHECK(true);
    if(!proj)
        return true;
    if(proj->GetCurrentlyCompilingTarget())
        return false;
    if(!dontsavefiles)
        if(!proj->QueryCloseAllFiles())
            return false;
    if (proj->GetModified())
    {
        wxString msg;
        msg.Printf(_("Project '%s' is modified...\nDo you want to save the changes?"), proj->GetTitle().c_str());
        switch (wxMessageBox(msg, _("Save project"), wxICON_QUESTION | wxYES_NO | wxCANCEL))
        {
            case wxYES:     if (!proj->Save()) return false;
            case wxNO:      break;
            case wxCANCEL:  return false;
        }
    }
    return true;
}

bool ProjectManager::CloseAllProjects(bool dontsave)
{
    SANITY_CHECK(true);
    if(!dontsave)
        if(!QueryCloseAllProjects())
            return false;
    FreezeTree();
    while (m_pProjects->GetCount() != 0)
    {
// Commented it by Heromyth
//        if (!CloseActiveProject(true))
        if (!CloseProject(m_pProjects->Item(0), true, false))
        {
            UnfreezeTree(true);
            return false;
        }
    }
    RebuildTree();
    UnfreezeTree(true);
    if(!m_InitialDir.IsEmpty())
        wxFileName::SetCwd(m_InitialDir);
    return true;
}

bool ProjectManager::CloseProject(cbProject* project, bool dontsave, bool refresh)
{
    SANITY_CHECK(false);
    if (!project)
        return true;
    if(m_sanitycheck_shutdown) // if shutdown, don't ask.
        dontsave = true;
    if(project->GetCurrentlyCompilingTarget())
        return false;
    if(!dontsave)
         if(!QueryCloseProject(project))
            return false;

    bool wasActive = project == m_pActiveProject;
	if (wasActive)
	    m_pActiveProject = 0L;

    int index = m_pProjects->Index(project);
    if (index == wxNOT_FOUND)
        return false;
    Manager::Get()->GetEditorManager()->UpdateProjectFiles(project);
//    project->SaveTreeState(m_pTree);
    project->SaveLayout();

    if (m_pWorkspace)
        m_pWorkspace->SetModified(true);

    project->CloseAllFiles(true);
    RemoveProjectFromAllDependencies(project);
    m_pProjects->Remove(project);
    if (refresh)
        m_pTree->Delete(project->GetProjectNode());
    if (wasActive && m_pProjects->GetCount())
        SetProject(m_pProjects->Item(0), refresh);
    delete project;
//    if (refresh)
//        RebuildTree();
    if(!m_InitialDir.IsEmpty()) // Restore the working directory
        wxFileName::SetCwd(m_InitialDir);
    return true;
}

bool ProjectManager::CloseActiveProject(bool dontsave)
{
    SANITY_CHECK(false);
    if (!CloseProject(m_pActiveProject, dontsave))
        return false;
    if (m_pProjects->GetCount() > 0)
        SetProject(m_pProjects->Item(0));
    return true;
}

bool ProjectManager::SaveProject(cbProject* project)
{
    SANITY_CHECK(false);

    if (!project)
        return false;
    if (project->Save())
    {
        RebuildTree();
        return true;
    }
    return false;
}

bool ProjectManager::SaveProjectAs(cbProject* project)
{
    SANITY_CHECK(false);
    if (!project)
        return false;

    if (project->SaveAs())
    {
        RebuildTree();
        return true;
    }
    return false;
}

bool ProjectManager::SaveActiveProject()
{
    SANITY_CHECK(false);
    return SaveProject(m_pActiveProject);
}

bool ProjectManager::SaveActiveProjectAs()
{
    SANITY_CHECK(false);
    return SaveProjectAs(m_pActiveProject);
}

bool ProjectManager::SaveAllProjects()
{
    SANITY_CHECK(false);
    FreezeTree();
    int prjCount = m_pProjects->GetCount();
    int count = 0;
    for (int i = 0; i < prjCount; ++i)
    {
        cbProject* project = m_pProjects->Item(i);
        if (SaveProject(project))
            ++count;
    }
    UnfreezeTree(true);
    return count == prjCount;
}

void ProjectManager::MoveProjectUp(cbProject* project, bool warpAround)
{
    SANITY_CHECK();
    if (!project)
        return;

    int idx = m_pProjects->Index(project);
    if (idx == wxNOT_FOUND)
        return; // project not opened in project manager???

    if (idx == 0)
    {
         if (!warpAround)
            return;
        else
            idx = m_pProjects->Count();
    }
    m_pProjects->RemoveAt(idx--);
    m_pProjects->Insert(project, idx);
    RebuildTree();
    if (m_pWorkspace)
        m_pWorkspace->SetModified(true);
}

void ProjectManager::MoveProjectDown(cbProject* project, bool warpAround)
{
    SANITY_CHECK();
    if (!project)
        return;

    int idx = m_pProjects->Index(project);
    if (idx == wxNOT_FOUND)
        return; // project not opened in project manager???

    if (idx == (int)m_pProjects->Count() - 1)
    {
         if (!warpAround)
            return;
        else
            idx = 0;
    }
    m_pProjects->RemoveAt(idx++);
    m_pProjects->Insert(project, idx);
    RebuildTree();
    if (m_pWorkspace)
        m_pWorkspace->SetModified(true);
}

cbWorkspace* ProjectManager::GetWorkspace()
{
    SANITY_CHECK(0L);
    if (!m_pWorkspace)
    {
        m_pWorkspace = new cbWorkspace(_T(""));
        m_pWorkspace->SetTitle(_("Workspace"));
        m_pWorkspace->SetModified(false);
    }
    return m_pWorkspace;
}

void ProjectManager::SetTopEditor(EditorBase* ed)
{
    SANITY_CHECK();
    m_pTopEditor = ed;
}

EditorBase* ProjectManager::GetTopEditor()
{
    SANITY_CHECK(0);
    return m_pTopEditor;
}

bool ProjectManager::LoadWorkspace(const wxString& filename)
{
    SANITY_CHECK(false);
    m_pTopEditor = 0;
    if (!CloseWorkspace())
        return false; // didn't close
    m_IsLoadingWorkspace=true;
    FreezeTree();
    m_pTree->AppendItem(m_pTree->GetRootItem(), _("Loading workspace..."));
    m_pTree->Expand(m_pTree->GetRootItem());
    UnfreezeTree();
    m_pWorkspace = new cbWorkspace(filename);
    if (m_pWorkspace->IsOK())
    {
        RebuildTree();
        if (m_pActiveProject)
            m_pTree->Expand(m_pActiveProject->GetProjectNode());
        m_pTree->Expand(m_TreeRoot); // make sure the root node is open
        m_IsLoadingWorkspace=false;
        Manager::Get()->GetEditorManager()->RebuildOpenedFilesTree();
        SANITY_CHECK(false);
        m_pTree->SetItemText(m_TreeRoot, m_pWorkspace->GetTitle());
        if(m_pTopEditor)
            m_pTopEditor->Activate();
        Manager::Get()->GetEditorManager()->RefreshOpenedFilesTree(true);
        UnfreezeTree(true);
    }
    return m_pWorkspace->IsOK();
}

bool ProjectManager::SaveWorkspace()
{
    return GetWorkspace()->Save();
}

bool ProjectManager::SaveWorkspaceAs(const wxString& filename)
{
    return GetWorkspace()->SaveAs(filename);
}

bool ProjectManager::QueryCloseWorkspace()
{
    SANITY_CHECK(true);
    if(!m_pWorkspace)
        return true;

    // don't ask to save the default workspace, if blank workspace is used on app startup
    if (m_pWorkspace->IsDefault() && Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/environment/blank_workspace"), false) == true)
        return true;

    if (m_pWorkspace->GetModified())
    {
        // workspace needs save
        wxString msg;
        msg.Printf(_("Workspace '%s' is modified. Do you want to save it?"), m_pWorkspace->GetTitle().c_str());
        switch (wxMessageBox(msg,
                        _("Save workspace"),
                        wxYES_NO | wxCANCEL | wxICON_QUESTION))
        {
            case wxYES: SaveWorkspace(); break;
            case wxCANCEL: return false;
            default: break;
        }
    }
    if(!QueryCloseAllProjects())
        return false;
    return true;
}

bool ProjectManager::CloseWorkspace()
{
    SANITY_CHECK(false);
    if (m_pWorkspace)
    {
        if(!QueryCloseWorkspace())
            return false;
        if(!CloseAllProjects(true))
            return false;

        delete m_pWorkspace;
        m_pWorkspace = 0;
        m_pTree->SetItemText(m_TreeRoot, _("Workspace"));
    }
    else
        return CloseAllProjects(false);
    return true;
}

bool ProjectManager::IsLoading()
{
    SANITY_CHECK(false);
    return (m_IsLoadingProject || m_IsLoadingWorkspace);
}

void ProjectManager::FreezeTree()
{
    SANITY_CHECK();
    if (!m_pTree)
        return;
// wx 2.5.x implement nested Freeze()/Thaw() calls correctly
#if !wxCHECK_VERSION(2,5,0)
    ++m_TreeFreezeCounter;
#endif
    m_pTree->Freeze();
}

void ProjectManager::UnfreezeTree(bool force)
{
    SANITY_CHECK();
    if (!m_pTree)
        return;
// wx 2.5.x implement nested Freeze()/Thaw() calls correctly
#if !wxCHECK_VERSION(2,5,0)
    --m_TreeFreezeCounter;
    if (force || m_TreeFreezeCounter <= 0)
    {
        m_pTree->Thaw();
        m_TreeFreezeCounter = 0;
    }
#else
    m_pTree->Thaw();
#endif
}

void ProjectManager::RebuildTree()
{
    SANITY_CHECK();
    FreezeTree();
    int count = m_pProjects->GetCount();
    for (int i = 0; i < count; ++i)
    {
        cbProject* project = m_pProjects->Item(i);
        if (project)
            project->SaveTreeState(m_pTree);
    }
    m_pTree->DeleteAllItems();
    wxString title=_T("");
    if(m_pWorkspace)
        title=m_pWorkspace->GetTitle();
     if(title==_T(""))
         title=_("Workspace");
    m_TreeRoot = m_pTree->AddRoot(title, WorkspaceIconIndex(), WorkspaceIconIndex());
    for (int i = 0; i < count; ++i)
    {
        cbProject* project = m_pProjects->Item(i);
        if (project)
        {
            project->BuildTree(m_pTree, m_TreeRoot, m_TreeCategorize, m_TreeUseFolders, m_pFileGroups);
            m_pTree->SetItemBold(project->GetProjectNode(), project == m_pActiveProject);
        }
    }
    m_pTree->Expand(m_TreeRoot);

    for (int i = 0; i < count; ++i)
    {
        cbProject* project = m_pProjects->Item(i);
        if (project)
            project->RestoreTreeState(m_pTree);
    }
    UnfreezeTree();
}

int ProjectManager::DoAddFileToProject(const wxString& filename, cbProject* project, wxArrayInt& targets)
{
    SANITY_CHECK(0);
	if (!project)
		return 0;

	// do we have to ask for target?
	if (targets.GetCount() == 0)
	{
		// if project has only one target, use this
		if (project->GetBuildTargetsCount() == 1)
			targets.Add(0);
		// else display multiple target selection dialog
		else
		{
			targets = AskForMultiBuildTargetIndex(project);
			if (targets.GetCount() == 0)
				return 0;
		}
	}

    // make sure filename is relative to project path
    wxFileName fname(filename);
    fname.Normalize(wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE, project->GetBasePath());
    fname.MakeRelativeTo(project->GetBasePath());

    // add the file to the first selected target
    SANITY_CHECK(0);
    ProjectFile* pf = project->AddFile(targets[0], fname.GetFullPath());
    if (pf)
    {
        // if the file was added succesfully,
        // add to this file the rest of the selected targets...
        for (size_t i = 0; i < targets.GetCount(); ++i)
        {
            ProjectBuildTarget* target = project->GetBuildTarget(targets[i]);
            if (target)
                pf->AddBuildTarget(target->GetTitle());
        }
    }
    return targets.GetCount();
}

int ProjectManager::AddFileToProject(const wxString& filename, cbProject* project, int target)
{
    SANITY_CHECK(-1);

	if (!project)
		project = GetActiveProject();

    wxArrayInt targets;
    targets.Add(target);
    if (AddFileToProject(filename, project, targets) == 1)
        return targets[0];
    return -1;
}

int ProjectManager::AddFileToProject(const wxString& filename, cbProject* project, wxArrayInt& targets)
{
    SANITY_CHECK(0);

	if (!project)
		project = GetActiveProject();

    int ret = DoAddFileToProject(filename, project, targets);
    if (ret > 0)
    {
        CodeBlocksEvent event(cbEVT_PROJECT_FILE_ADDED);
        event.SetProject(project);
        event.SetString(filename);
        Manager::Get()->GetPluginManager()->NotifyPlugins(event);
    }
	return ret;
}

int ProjectManager::AddMultipleFilesToProject(const wxArrayString& filelist, cbProject* project, int target)
{
    SANITY_CHECK(-1);

	if (!project)
		project = GetActiveProject();

    wxArrayInt targets;
    targets.Add(target);
    if (AddMultipleFilesToProject(filelist, project, targets) == 1)
        return targets[0];
    return -1;
}

int ProjectManager::AddMultipleFilesToProject(const wxArrayString& filelist, cbProject* project, wxArrayInt& targets)
{
    SANITY_CHECK(0);

    wxProgressDialog progress(_("Project Manager"), _("Please wait while adding files to project..."), filelist.GetCount());

	if (!project)
		project = GetActiveProject();

    wxArrayString addedFiles; // to know which files were added succesfully
    for (unsigned int i = 0; i < filelist.GetCount(); ++i)
    {
    	if (DoAddFileToProject(filelist[i], project, targets) != 0)
            addedFiles.Add(filelist[i]);
        progress.Update(i);
    }

    if (addedFiles.GetCount() != 0)
    {
        for (unsigned int i = 0; i < addedFiles.GetCount(); ++i)
        {
            CodeBlocksEvent event(cbEVT_PROJECT_FILE_ADDED);
            event.SetProject(project);
            event.SetString(addedFiles[i]);
            Manager::Get()->GetPluginManager()->NotifyPlugins(event);
        }
    }
	return targets.GetCount();
}

int ProjectManager::AskForBuildTargetIndex(cbProject* project)
{
    SANITY_CHECK(-1);
	cbProject* prj = project;
	if (!prj)
		prj = GetActiveProject();
	if (!prj)
		return -1;

	// ask for target
	wxArrayString array;
	int count = prj->GetBuildTargetsCount();
	for (int i = 0; i < count; ++i)
		array.Add(prj->GetBuildTarget(i)->GetTitle());
	int target = wxGetSingleChoiceIndex(_("Select the target:"), _("Project targets"), array);

	return target;
}

wxArrayInt ProjectManager::AskForMultiBuildTargetIndex(cbProject* project)
{
    wxArrayInt indices;
    SANITY_CHECK(indices);
	cbProject* prj = project;
	if (!prj)
		prj = GetActiveProject();
	if (!prj)
		return indices;

	// ask for target
	wxArrayString array;
	int count = prj->GetBuildTargetsCount();
	for (int i = 0; i < count; ++i)
		array.Add(prj->GetBuildTarget(i)->GetTitle());

    MultiSelectDlg dlg(0, array, false, _("Select the targets this file should belong to:"));
    if (dlg.ShowModal() == wxID_OK)
        indices = dlg.GetSelectedIndices();

	return indices;
}

void ProjectManager::DoOpenFile(ProjectFile* pf, const wxString& filename)
{
    SANITY_CHECK();
	FileType ft = FileTypeOf(filename);

	if (ft == ftHeader || ft == ftSource)
	{
        // C/C++ header/source files, always get opened inside Code::Blocks
        cbEditor* ed = Manager::Get()->GetEditorManager()->Open(filename);
        if (ed)
        {
            ed->SetProjectFile(pf);
            ed->Activate();
        }
        else
        {
            wxString msg;
            msg.Printf(_("Failed to open '%s'."), filename.c_str());
            Manager::Get()->GetMessageManager()->DebugLogError(msg);
        }
	}
	else
	{
        // first look for custom editors
        // if that fails, try MIME handlers
		EditorBase* eb = Manager::Get()->GetEditorManager()->IsOpen(filename);
		if (eb && !eb->IsBuiltinEditor())
		{
            // custom editors just get activated
            eb->Activate();
            return;
        }

		// not a recognized file type
        cbMimePlugin* plugin = Manager::Get()->GetPluginManager()->GetMIMEHandlerForFile(filename);
        if (!plugin)
		{
            wxString msg;
            msg.Printf(_("Could not open file '%s'.\nNo handler registered for this type of file."), filename.c_str());
            Manager::Get()->GetMessageManager()->DebugLogError(msg);
		}
        else if (plugin->OpenFile(filename) != 0)
		{
            wxString msg;
            msg.Printf(_("Could not open file '%s'.\nThe registered handler (%s) could not open it."), filename.c_str(), plugin->GetInfo()->title.c_str());
            Manager::Get()->GetMessageManager()->DebugLogError(msg);
		}
	}
}

void ProjectManager::DoOpenSelectedFile()
{
    SANITY_CHECK();
    wxTreeItemId sel = m_pTree->GetSelection();
    FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);

    if (ftd)
    {
	    ProjectFile* f = ftd->GetProjectFile();
    	if (f)
        {
			DoOpenFile(f, f->file.GetFullPath());
        }
    }
}

bool ProjectManager::CausesCircularDependency(cbProject* base, cbProject* dependsOn)
{
    if (!base || !dependsOn)
        return false;

    // 1st check is both projects are the same one
    if (base == dependsOn)
        return true;

    const ProjectsArray* arr = GetDependenciesForProject(dependsOn);
    if (arr)
    {
        // now check deeper
        for (size_t i = 0; i < arr->GetCount(); ++i)
        {
            if (CausesCircularDependency(base, arr->Item(i)))
                return true;
        }
    }

    // if we reached here, no possibility of circular dependency :)
    return false;
}

bool ProjectManager::AddProjectDependency(cbProject* base, cbProject* dependsOn)
{
    if (!base || !dependsOn)
        return false;

    // avoid circular dependencies
    if (CausesCircularDependency(base, dependsOn))
        return false;

    ProjectsArray* arr = 0;
    DepsMap::iterator it = m_ProjectDeps.find(base);
    if (it == m_ProjectDeps.end())
    {
        // create a ProjectsArray* to hold the dependencies for base
        arr = new ProjectsArray;
        m_ProjectDeps[base] = arr;
    }
    else
        arr = it->second;

    // add dependency only if not already there
    if (arr->Index(dependsOn) == wxNOT_FOUND)
    {
        arr->Add(dependsOn);
        if (m_pWorkspace)
            m_pWorkspace->SetModified(true);
        Manager::Get()->GetMessageManager()->DebugLog(_T("%s now depends on %s (%d deps)"), base->GetTitle().c_str(), dependsOn->GetTitle().c_str(), arr->GetCount());
    }
    return true;
}

void ProjectManager::RemoveProjectDependency(cbProject* base, cbProject* doesNotDependOn)
{
    if (!base || !doesNotDependOn)
        return;

    DepsMap::iterator it = m_ProjectDeps.find(base);
    if (it == m_ProjectDeps.end())
        return; // nothing to remove

    ProjectsArray* arr = it->second;
    arr->Remove(doesNotDependOn);

    Manager::Get()->GetMessageManager()->DebugLog(_T("%s now does not depend on %s (%d deps)"), base->GetTitle().c_str(), doesNotDependOn->GetTitle().c_str(), arr->GetCount());
    // if it was the last dependency, delete the array
    if (!arr->GetCount())
    {
        m_ProjectDeps.erase(it);
        delete arr;
    }
    if (m_pWorkspace)
        m_pWorkspace->SetModified(true);
}

void ProjectManager::ClearProjectDependencies(cbProject* base)
{
    if (!base)
        return;
    DepsMap::iterator it = m_ProjectDeps.find(base);
    if (it == m_ProjectDeps.end())
        return; // nothing to remove

    delete it->second;
    m_ProjectDeps.erase(it);
    if (m_pWorkspace)
        m_pWorkspace->SetModified(true);

    Manager::Get()->GetMessageManager()->DebugLog(_T("Removed all deps from %s"), base->GetTitle().c_str());
}

void ProjectManager::RemoveProjectFromAllDependencies(cbProject* base)
{
    if (!base)
        return;
    DepsMap::iterator it = m_ProjectDeps.find(base);
    while (it != m_ProjectDeps.end())
    {
        ProjectsArray* arr = it->second;
        // only check projects that do have a dependencies array
        if (!arr)
            continue;

        arr->Remove(base);
        if (m_pWorkspace)
            m_pWorkspace->SetModified(true);

        // if it was the last dependency, delete the array
        if (!arr->GetCount())
        {
            DepsMap::iterator it2 = it++;
            m_ProjectDeps.erase(it2);
            delete arr;
        }
        else
            ++it;
    }
    Manager::Get()->GetMessageManager()->DebugLog(_T("Removed %s from all deps"), base->GetTitle().c_str());
}

const ProjectsArray* ProjectManager::GetDependenciesForProject(cbProject* base)
{
    DepsMap::iterator it = m_ProjectDeps.find(base);
    if (it != m_ProjectDeps.end())
        return it->second;
    return 0;
}

void ProjectManager::ConfigureProjectDependencies(cbProject* base)
{
    ProjectDepsDlg dlg(Manager::Get()->GetAppWindow(), base);
    dlg.ShowModal();
}

// events

void ProjectManager::OnTabPosition(wxCommandEvent& event)
{
    long style = m_pNotebook->GetWindowStyleFlag();
    style &= ~wxFNB_BOTTOM;

    if (event.GetId() == idNB_TabBottom)
        style |= wxFNB_BOTTOM;
    m_pNotebook->SetWindowStyleFlag(style);
    // (style & wxFNB_BOTTOM) saves info only about the the tabs position
    Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/environment/project_tabs_bottom"), (bool)(style & wxFNB_BOTTOM));
}

void ProjectManager::OnProjectFileActivated(wxTreeEvent& event)
{
    SANITY_CHECK();
    #ifdef USE_OPENFILES_TREE
    if(!MiscTreeItemData::OwnerCheck(event,m_pTree,this))
        return;
    #endif
	DoOpenSelectedFile();
}

void ProjectManager::OnExecParameters(wxCommandEvent& event)
{
    SANITY_CHECK();
    if (m_pActiveProject)
		m_pActiveProject->SelectTarget(0, true);
}

void ProjectManager::OnRightClick(wxCommandEvent& event)
{
    //Manager::Get()->GetMessageManager()->DebugLog("OnRightClick");
    SANITY_CHECK();

    wxMenu menu;

    // ask any plugins to add items in this menu
    Manager::Get()->GetPluginManager()->AskPluginsForModuleMenu(mtProjectManager, &menu);

    // if plugins added to this menu, add a separator
    if (menu.GetMenuItemCount() != 0)
        menu.AppendSeparator();

    menu.AppendCheckItem(idMenuViewCategorizePopup, _("Categorize by file types"));
    menu.AppendCheckItem(idMenuViewUseFoldersPopup, _("Display folders as on disk"));
    menu.AppendSeparator();
    menu.Append(idMenuViewFileMasks, _("Edit file types && categories..."));

    menu.Check(idMenuViewCategorizePopup, m_TreeCategorize);
    menu.Check(idMenuViewUseFoldersPopup, m_TreeUseFolders);

    wxPoint pt = wxGetMousePosition();
    pt = m_pTree->ScreenToClient(pt);
    m_pTree->PopupMenu(&menu, pt);
}

void ProjectManager::OnTreeItemRightClick(wxTreeEvent& event)
{
    SANITY_CHECK();
    #ifdef USE_OPENFILES_TREE
    if(!MiscTreeItemData::OwnerCheck(event,m_pTree,this))
        return;
    #endif
    if(m_IsLoadingProject)
    {
        wxBell();
        return;
    }

    //Manager::Get()->GetMessageManager()->DebugLog("OnTreeItemRightClick");
	m_pTree->SelectItem(event.GetItem());
    ShowMenu(event.GetItem(), event.GetPoint());
}

void ProjectManager::OnRenameWorkspace(wxCommandEvent& event)
{
    SANITY_CHECK();
    if (m_pWorkspace)
    {
        wxString text = wxGetTextFromUser(_("Please enter the new name for the workspace:"), _("Rename workspace"), m_pWorkspace->GetTitle());
        if (!text.IsEmpty())
        {
            m_pWorkspace->SetTitle(text);
            m_pTree->SetItemText(m_TreeRoot, m_pWorkspace->GetTitle());
        }
    }
}

void ProjectManager::OnSaveWorkspace(wxCommandEvent& event)
{
    SANITY_CHECK();
    if (m_pWorkspace)
    {
        SaveWorkspace();
    }
}

void ProjectManager::OnSaveAsWorkspace(wxCommandEvent& event)
{
    SANITY_CHECK();
    if (m_pWorkspace)
    {
        SaveWorkspaceAs(_T(""));
    }
}

void ProjectManager::OnCloseWorkspace(wxCommandEvent& event)
{
    SANITY_CHECK();
    if (m_pWorkspace)
    {
        CloseWorkspace();
    }
}

void ProjectManager::OnSetActiveProject(wxCommandEvent& event)
{
    SANITY_CHECK();
    if (event.GetId() == idMenuSetActiveProject)
    {
        wxTreeItemId sel = m_pTree->GetSelection();
        FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);
        if (!ftd)
            return;

        SetProject(ftd->GetProject(), false);
	}
    else if (event.GetId() == idMenuPriorProject)
    {
        int index = m_pProjects->Index(m_pActiveProject);
        if (index == wxNOT_FOUND)
            return;
        --index;
        if (index < 0)
            index = m_pProjects->GetCount() - 1;
        SetProject(m_pProjects->Item(index), false);
    }
    else if (event.GetId() == idMenuNextProject)
    {
        int index = m_pProjects->Index(m_pActiveProject);
        if (index == wxNOT_FOUND)
            return;
        ++index;
        if (index == (int)m_pProjects->GetCount())
            index = 0;
        SetProject(m_pProjects->Item(index), false);
    }
    else if (event.GetId() == idMenuProjectUp)
    {
        MoveProjectUp(m_pActiveProject);
    }
    else if (event.GetId() == idMenuProjectDown)
    {
        MoveProjectDown(m_pActiveProject);
    }
}

void ProjectManager::OnAddFilesToProjectRecursively(wxCommandEvent& event)
{
    SANITY_CHECK();
    cbProject* prj = 0;
    if (event.GetId() == idMenuAddFilesRecursively)
        prj = GetActiveProject();
    else
    {
        wxTreeItemId sel = m_pTree->GetSelection();
        FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);
        if (ftd)
            prj = ftd->GetProject();
    }
    if (!prj)
        return;

    wxString dir = ChooseDirectory(m_pTree,
                                    _("Add files recursively..."),
                                    prj->GetBasePath(),
                                    wxEmptyString,
                                    false,
                                    false);
    if (dir.IsEmpty())
        return;

    wxArrayInt targets;
    // ask for target only if more than one
    if (prj->GetBuildTargetsCount() == 1)
         targets.Add(0);

    // generate list of files to add
    wxArrayString array;
    wxDir::GetAllFiles(dir, &array, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
    if (array.GetCount() == 0)
        return;

    // for usability reasons, remove any directory entries from the list...
    unsigned int i = 0;
    while (i < array.GetCount())
    {
    	// discard directories, as well as some well known SCMs control folders ;)
    	// also discard C::B project files
    	if (wxDirExists(array[i]) ||
            array[i].Contains(_T("\\.svn\\")) ||
            array[i].Contains(_T("/.svn/")) ||
            array[i].Contains(_T("\\CVS\\")) ||
            array[i].Contains(_T("/CVS/")) ||
            array[i].Lower().Matches(_T("*.cbp")))
        {
            array.RemoveAt(i);
        }
        else
            ++i;
    }

    // ask the user which files to add
// TODO (mandrav#1#): Make these masks configurable
    wxString wild = _T("*.c;*.cc;*.cpp;*.cxx;*.h;*.hh;*.hpp;*.hxx;*.inl;*.rc;*.xrc");
    MultiSelectDlg dlg(0, array, wild, _("Select the files to add to the project:"));
    if (dlg.ShowModal() != wxID_OK)
        return;
    array = dlg.GetSelectedStrings();

    // finally add the files
    AddMultipleFilesToProject(array, prj, targets);
    RebuildTree();
}

void ProjectManager::OnAddFileToProject(wxCommandEvent& event)
{
    SANITY_CHECK();
    cbProject* prj = 0;
    if (event.GetId() == idMenuAddFile)
        prj = GetActiveProject();
    else
    {
        wxTreeItemId sel = m_pTree->GetSelection();
        FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);
        if (ftd)
            prj = ftd->GetProject();
    }
    if (!prj)
        return;

    wxFileDialog dlg(m_pTree,
                    _("Add files to project..."),
                    prj->GetBasePath(),
                    wxEmptyString,
                    KNOWN_SOURCES_DIALOG_FILTER,
                    wxOPEN | wxMULTIPLE | wxFILE_MUST_EXIST);
    dlg.SetFilterIndex(KNOWN_SOURCES_FILTER_INDEX);

    if (dlg.ShowModal() == wxID_OK)
    {
		wxArrayInt targets;
        // ask for target only if more than one
		if (prj->GetBuildTargetsCount() == 1)
             targets.Add(0);

		wxArrayString array;
        dlg.GetPaths(array);
        AddMultipleFilesToProject(array, prj, targets);
        RebuildTree();
    }
}

void ProjectManager::OnRemoveFileFromProject(wxCommandEvent& event)
{
    SANITY_CHECK();
    wxTreeItemId sel = m_pTree->GetSelection();
    FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);
    if (!ftd)
        return;
    cbProject* prj = ftd->GetProject();
    if (!prj)
        return;
    wxString oldpath = prj->GetCommonTopLevelPath();

    if (event.GetId() == idMenuRemoveFile)
    {
        // remove multiple-files
        wxArrayString files;
        for (int i = 0; i < prj->GetFilesCount(); ++i)
        {
            files.Add(prj->GetFile(i)->relativeFilename);
        }
        wxString msg;
        msg.Printf(_("Select files to remove from %s:"), prj->GetTitle().c_str());
        MultiSelectDlg dlg(0, files, false, msg);
        if (dlg.ShowModal() == wxID_OK)
        {
            wxArrayInt indices = dlg.GetSelectedIndices();
            if (indices.GetCount() == 0)
                return;
            if (wxMessageBox(_("Are you sure you want to remove these files from the project?"),
                            _("Confirmation"),
                            wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT) != wxYES)
            {
                return;
            }
            // we iterate the arry backwards, because if we iterate it normally,
            // when we remove the first index, the rest become invalid...
            for (int i = (int)indices.GetCount() - 1; i >= 0; --i)
            {
                ProjectFile* pf = prj->GetFile(indices[i]);
                if (!pf)
                {
                    Manager::Get()->GetMessageManager()->DebugLog(_("Invalid project file: Index %d"), indices[i]);
                    continue;
                }
                wxString filename = pf->file.GetFullPath();
                Manager::Get()->GetMessageManager()->DebugLog(_("Removing index %d, %s"), indices[i], filename.c_str());
                prj->RemoveFile(indices[i]);
                CodeBlocksEvent evt(cbEVT_PROJECT_FILE_REMOVED);
                evt.SetProject(prj);
                evt.SetString(filename);
                Manager::Get()->GetPluginManager()->NotifyPlugins(evt);
            }
            prj->CalculateCommonTopLevelPath();
            RebuildTree();
        }
    }
    else if (event.GetId() == idMenuRemoveFilePopup)
    {
        // remove single file
        wxString filename = ftd->GetProjectFile()->file.GetFullPath();
        prj->RemoveFile(ftd->GetFileIndex());
        prj->CalculateCommonTopLevelPath();
        if (prj->GetCommonTopLevelPath() != oldpath)
            RebuildTree();
        else
            m_pTree->Delete(sel);
        CodeBlocksEvent evt(cbEVT_PROJECT_FILE_REMOVED);
        evt.SetProject(prj);
        evt.SetString(filename);
        Manager::Get()->GetPluginManager()->NotifyPlugins(evt);
    }
    else if (event.GetId() == idMenuRemoveFolderFilesPopup)
    {
        // remove all files from a folder
        if (wxMessageBox(_("Are you sure you want to recursively remove from the project all the files under this folder?"),
                        _("Confirmation"),
                        wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT) != wxYES)
        {
            return;
        }
        int i = 0;
        while (i >= 0 && i < prj->GetFilesCount())
        {
            ProjectFile* pf = prj->GetFile(i);
            // ftd->GetFolder() ends with the path separator, so it is
            // safe to just compare the two strings...
            if (pf->file.GetFullPath().StartsWith(ftd->GetFolder()))
            {
                wxString filename = pf->file.GetFullPath();
                prj->RemoveFile(i);
                CodeBlocksEvent evt(cbEVT_PROJECT_FILE_REMOVED);
                evt.SetProject(prj);
                evt.SetString(filename);
                Manager::Get()->GetPluginManager()->NotifyPlugins(evt);
            }
            else
                ++i;
        }
        prj->CalculateCommonTopLevelPath();
        if (prj->GetCommonTopLevelPath() != oldpath)
            RebuildTree();
        else
            m_pTree->Delete(sel);
    }
}

void ProjectManager::OnCloseProject(wxCommandEvent& event)
{
    SANITY_CHECK();
    wxTreeItemId sel = m_pTree->GetSelection();
    FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);
    cbProject *proj=NULL;
    if (ftd)
        proj = ftd->GetProject();
    if(proj)
    {
        if(m_IsLoadingProject || proj->GetCurrentlyCompilingTarget())
        {
            wxBell();
        }
        else
            CloseProject(proj);
    }
    if (m_pProjects->GetCount() > 0 && !m_pActiveProject)
        SetProject(m_pProjects->Item(0), false);
    Manager::Get()->GetAppWindow()->Refresh();
}

void ProjectManager::OnCloseFile(wxCommandEvent& event)
{
    SANITY_CHECK();
    wxTreeItemId sel = m_pTree->GetSelection();
    FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);

    if (ftd)
    {
	    cbProject* project = ftd->GetProject();
	    ProjectFile* f = project->GetFile(ftd->GetFileIndex());
    	if (f)
        	Manager::Get()->GetEditorManager()->Close(f->file.GetFullPath());
    }
}

void ProjectManager::OnOpenFile(wxCommandEvent& event)
{
	DoOpenSelectedFile();
}

void ProjectManager::OnOpenWith(wxCommandEvent& event)
{
    wxTreeItemId sel = m_pTree->GetSelection();
    FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);

    if (ftd)
    {
	    cbProject* project = ftd->GetProject();
	    ProjectFile* f = project->GetFile(ftd->GetFileIndex());
    	if (f)
        {
            wxString filename = f->file.GetFullPath();
            if (event.GetId() == idOpenWithInternal)
            {
                cbEditor* ed = Manager::Get()->GetEditorManager()->Open(filename);
                if (ed)
                {
                    ed->SetProjectFile(f);
                    ed->Show(true);
                    return;
                }
            }
            else
            {
                PluginsArray mimes = Manager::Get()->GetPluginManager()->GetMimeOffers();
                cbMimePlugin* plugin = (cbMimePlugin*)mimes[event.GetId() - idOpenWith[0]];
                if (plugin && plugin->OpenFile(filename) == 0)
                    return;
            }
            wxString msg;
            msg.Printf(_("Failed to open '%s'."), filename.c_str());
            Manager::Get()->GetMessageManager()->DebugLogError(msg);
        }
    }
}

void ProjectManager::OnProperties(wxCommandEvent& event)
{
    SANITY_CHECK();
    if (event.GetId() == idMenuProjectProperties)
    {
        wxString backupTitle = m_pActiveProject ? m_pActiveProject->GetTitle() : _T("");
        if (m_pActiveProject && m_pActiveProject->ShowOptions())
        {
            // make sure that cbEVT_PROJECT_ACTIVATE
            // is sent (maybe targets have changed)...
            // rebuild tree  only if title has changed
            SetProject(m_pActiveProject, backupTitle != m_pActiveProject->GetTitle());
        }
    }
    else if (event.GetId() == idMenuTreeProjectProperties)
    {
        wxTreeItemId sel = m_pTree->GetSelection();
        FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);

        cbProject* project = ftd ? ftd->GetProject() : m_pActiveProject;
        wxString backupTitle = project ? project->GetTitle() : _T("");
        if (project && project->ShowOptions() && project == m_pActiveProject)
        {
            // rebuild tree and make sure that cbEVT_PROJECT_ACTIVATE
            // is sent (maybe targets have changed)...
            // rebuild tree  only if title has changed
            SetProject(project, backupTitle != project->GetTitle());
        }
    }
    else if (event.GetId() == idMenuTreeFileProperties)
    {
        wxTreeItemId sel = m_pTree->GetSelection();
        FileTreeData* ftd = (FileTreeData*)m_pTree->GetItemData(sel);

        cbProject* project = ftd ? ftd->GetProject() : m_pActiveProject;
        if (project)
        {
            if (ftd && ftd->GetFileIndex() != -1)
            {
                ProjectFile* pf = project->GetFile(ftd->GetFileIndex());
                if (pf)
                    pf->ShowOptions(m_pTree);
            }
        }
    }
    else // active editor properties
    {
        cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
        if (ed)
        {
            ProjectFile* pf = ed->GetProjectFile();
            if (pf)
                pf->ShowOptions(m_pTree);
        }
    }
}

void ProjectManager::OnGotoFile(wxCommandEvent& event)
{
    SANITY_CHECK();
	if (!m_pActiveProject)
	{
		Manager::Get()->GetMessageManager()->DebugLog(_("No active project!"));
		return;
	}

	wxArrayString files;
	for (int i = 0; i < m_pActiveProject->GetFilesCount(); ++i)
		files.Add(m_pActiveProject->GetFile(i)->relativeFilename);

	IncrementalSelectListDlg dlg(m_pTree, files, _("Select file..."), _("Please select file to open:"));
	if (dlg.ShowModal() == wxID_OK)
	{
		ProjectFile* pf = m_pActiveProject->GetFileByFilename(dlg.GetStringSelection(), true);
		if (pf)
		{
			DoOpenFile(pf, pf->file.GetFullPath());
		}
	}
}

void ProjectManager::OnViewCategorize(wxCommandEvent& event)
{
    SANITY_CHECK();
    m_TreeCategorize = event.IsChecked();
    Manager::Get()->GetAppWindow()->GetMenuBar()->Check(idMenuViewCategorize, m_TreeCategorize);
	Manager::Get()->GetConfigManager(_T("project_manager"))->Write(_T("/categorize_tree"), m_TreeCategorize);
    RebuildTree();
}

void ProjectManager::OnViewUseFolders(wxCommandEvent& event)
{
    SANITY_CHECK();
    m_TreeUseFolders = event.IsChecked();
    Manager::Get()->GetAppWindow()->GetMenuBar()->Check(idMenuViewUseFolders, m_TreeUseFolders);
	Manager::Get()->GetConfigManager(_T("project_manager"))->Write(_T("/use_folders"), m_TreeUseFolders);
    RebuildTree();
}

void ProjectManager::OnViewFileMasks(wxCommandEvent& event)
{
    SANITY_CHECK();
	ProjectsFileMasksDlg dlg(Manager::Get()->GetAppWindow(), m_pFileGroups);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_pFileGroups->Save();
		RebuildTree();
	}
}

void ProjectManager::OnIdle(wxIdleEvent& event)
{
    event.Skip();
}

