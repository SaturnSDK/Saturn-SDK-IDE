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
* $Id$
* $Date$
*/

#include <sdk.h>
#include "app.h"
#include "main.h"
#include "appglobals.h"
#include "environmentsettingsdlg.h"
#include <cbworkspace.h>

#if defined(_MSC_VER) && defined( _DEBUG )
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include <wx/tipdlg.h>
#include <wx/dnd.h>
#include <wx/sstream.h>
#include <wx/fileconf.h>

#include <configmanager.h>
#include <cbproject.h>
#include <cbplugin.h>
#include <sdk_events.h>
#include <projectmanager.h>
#include <editormanager.h>
#include <messagemanager.h>
#include <pluginmanager.h>
#include <templatemanager.h>
#include <toolsmanager.h>
#include <scriptingmanager.h>
#include <cbexception.h>
#include <annoyingdialog.h>
#include <editorcolorset.h>

#include "dlgaboutplugin.h"
#include "dlgabout.h"
#include "startherepage.h"
#include "printdlg.h"
#include <wx/printdlg.h>
#include <wx/filename.h>
#include <wxFlatNotebook.h>

#include "../sdk/uservarmanager.h"

class wxMyFileDropTarget : public wxFileDropTarget
{
public:
    wxMyFileDropTarget::wxMyFileDropTarget(MainFrame *frame):m_frame(frame){}
    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
    {
        if(!m_frame) return false;
        return m_frame->OnDropFiles(x,y,filenames);
    }
private:
    MainFrame* m_frame;
};

const static wxString gDefaultLayout = _T("Code::Blocks default");
static wxString gDefaultLayoutData; // this will keep the "hardcoded" default layout

int wxID_FILE10 = wxNewId();
int wxID_FILE11 = wxNewId();
int wxID_FILE12 = wxNewId();
int wxID_FILE13 = wxNewId();
int wxID_FILE14 = wxNewId();
int wxID_FILE15 = wxNewId();
int wxID_FILE16 = wxNewId();
int wxID_FILE17 = wxNewId();
int wxID_FILE18 = wxNewId();
int wxID_FILE19 = wxNewId();

int idFileNew = XRCID("idFileNew");
int idFileOpen = XRCID("idFileOpen");
int idFileReopen = XRCID("idFileReopen");
int idFileOpenRecentFileClearHistory = XRCID("idFileOpenRecentFileClearHistory");
int idFileOpenRecentProjectClearHistory = XRCID("idFileOpenRecentProjectClearHistory");
int idFileSave = XRCID("idFileSave");
int idFileSaveAs = XRCID("idFileSaveAs");
int idFileSaveAllFiles = XRCID("idFileSaveAllFiles");
int idFileSaveProject = XRCID("idFileSaveProject");
int idFileSaveProjectAs = XRCID("idFileSaveProjectAs");
int idFileSaveWorkspace = XRCID("idFileSaveWorkspace");
int idFileSaveWorkspaceAs = XRCID("idFileSaveWorkspaceAs");
int idFileCloseWorkspace = XRCID("idFileCloseWorkspace");
int idFileClose = XRCID("idFileClose");
int idFileCloseAll = XRCID("idFileCloseAll");
int idFilePrintSetup = XRCID("idFilePrintSetup");
int idFilePrint = XRCID("idFilePrint");
int idFileRunScript = XRCID("idFileRunScript");
int idFileExit = XRCID("idFileExit");
int idFileNext = wxNewId();
int idFilePrev = wxNewId();

int idEditUndo = XRCID("idEditUndo");
int idEditRedo = XRCID("idEditRedo");
int idEditCopy = XRCID("idEditCopy");
int idEditCut = XRCID("idEditCut");
int idEditPaste = XRCID("idEditPaste");
int idEditSwapHeaderSource = XRCID("idEditSwapHeaderSource");
int idEditGotoMatchingBrace = XRCID("idEditGotoMatchingBrace");
int idEditHighlightMode = XRCID("idEditHighlightMode");
int idEditHighlightModeText = XRCID("idEditHighlightModeText");
int idEditBookmarks = XRCID("idEditBookmarks");
int idEditBookmarksToggle = XRCID("idEditBookmarksToggle");
int idEditBookmarksPrevious = XRCID("idEditBookmarksPrevious");
int idEditBookmarksNext = XRCID("idEditBookmarksNext");
int idEditFolding = XRCID("idEditFolding");
int idEditFoldAll = XRCID("idEditFoldAll");
int idEditUnfoldAll = XRCID("idEditUnfoldAll");
int idEditToggleAllFolds = XRCID("idEditToggleAllFolds");
int idEditFoldBlock = XRCID("idEditFoldBlock");
int idEditUnfoldBlock = XRCID("idEditUnfoldBlock");
int idEditToggleFoldBlock = XRCID("idEditToggleFoldBlock");
int idEditEOLMode = XRCID("idEditEOLMode");
int idEditEOLCRLF = XRCID("idEditEOLCRLF");
int idEditEOLCR = XRCID("idEditEOLCR");
int idEditEOLLF = XRCID("idEditEOLLF");
int idEditSelectAll = XRCID("idEditSelectAll");
int idEditCommentSelected = XRCID("idEditCommentSelected");
int idEditUncommentSelected = XRCID("idEditUncommentSelected");
int idEditToggleCommentSelected = XRCID("idEditToggleCommentSelected");
int idEditAutoComplete = XRCID("idEditAutoComplete");

int idViewLayoutDelete = XRCID("idViewLayoutDelete");
int idViewLayoutSave = XRCID("idViewLayoutSave");
int idViewToolbars = XRCID("idViewToolbars");
int idViewToolMain = XRCID("idViewToolMain");
int idViewManager = XRCID("idViewManager");
int idViewOpenFilesTree = XRCID("idViewOpenFilesTree");
int idViewMessageManager = XRCID("idViewMessageManager");
int idViewStatusbar = XRCID("idViewStatusbar");
int idViewFocusEditor = XRCID("idViewFocusEditor");
int idViewFullScreen = XRCID("idViewFullScreen");

int idSearchFind = XRCID("idSearchFind");
int idSearchFindInFiles = XRCID("idSearchFindInFiles");
int idSearchFindNext = XRCID("idSearchFindNext");
int idSearchFindPrevious = XRCID("idSearchFindPrevious");
int idSearchReplace = XRCID("idSearchReplace");
int idSearchGotoLine = XRCID("idSearchGotoLine");

int idProjectNew = XRCID("idProjectNew");
int idProjectNewEmptyProject = XRCID("idProjectNewEmptyProject");
int idProjectOpen = XRCID("idProjectOpen");
int idProjectSaveProject = XRCID("idProjectSaveProject");
int idProjectSaveProjectAs = XRCID("idProjectSaveProjectAs");
int idProjectSaveAllProjects = XRCID("idProjectSaveAllProjects");
int idProjectSaveTemplate = XRCID("idProjectSaveTemplate");
int idProjectCloseProject = XRCID("idProjectCloseProject");
int idProjectCloseAllProjects = XRCID("idProjectCloseAllProjects");
int idProjectImport = XRCID("idProjectImport");
int idProjectImportDevCpp = XRCID("idProjectImportDevCpp");
int idProjectImportMSVC = XRCID("idProjectImportMSVC");
int idProjectImportMSVCWksp = XRCID("idProjectImportMSVCWksp");
int idProjectImportMSVS = XRCID("idProjectImportMSVS");
int idProjectImportMSVSWksp = XRCID("idProjectImportMSVSWksp");

int idSettingsEnvironment = XRCID("idSettingsEnvironment");
int idSettingsGlobalUserVars = XRCID("idSettingsGlobalUserVars");
int idSettingsEditor = XRCID("idSettingsEditor");
int idPluginsManagePlugins = XRCID("idPluginsManagePlugins");
int idSettingsConfigurePlugins = XRCID("idSettingsConfigurePlugins");

int idHelpTips = XRCID("idHelpTips");
int idHelpPlugins = XRCID("idHelpPlugins");

int idLeftSash = XRCID("idLeftSash");
int idBottomSash = XRCID("idBottomSash");
int idCloseFullScreen = XRCID("idCloseFullScreen");
int idShiftTab = wxNewId();

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_ERASE_BACKGROUND(MainFrame::OnEraseBackground)
    EVT_SIZE(MainFrame::OnSize)
    EVT_CLOSE(MainFrame::OnApplicationClose)

    EVT_UPDATE_UI(idFileOpenRecentFileClearHistory, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileOpenRecentProjectClearHistory, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSave, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveAs, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveAllFiles, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveWorkspace, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveWorkspaceAs, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileCloseWorkspace, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileClose, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileCloseAll, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFilePrintSetup, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFilePrint, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idProjectSaveProject, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectSaveProjectAs, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectSaveAllProjects, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectSaveTemplate, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectCloseProject, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectCloseAllProjects, MainFrame::OnProjectMenuUpdateUI)

    EVT_UPDATE_UI(idEditUndo, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditRedo, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCopy, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCut, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditPaste, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditSwapHeaderSource, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditGotoMatchingBrace, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditFoldAll, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditUnfoldAll, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditToggleAllFolds, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditFoldBlock, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditUnfoldBlock, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditToggleFoldBlock, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditEOLCRLF, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditEOLCR, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditEOLLF, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditSelectAll, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditBookmarksToggle, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditBookmarksNext, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditBookmarksPrevious, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCommentSelected, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditAutoComplete, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditUncommentSelected, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditToggleCommentSelected, MainFrame::OnEditMenuUpdateUI)

    EVT_UPDATE_UI(idSearchFind, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchFindInFiles, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchFindNext, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchFindPrevious, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchReplace, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchGotoLine, MainFrame::OnSearchMenuUpdateUI)

    EVT_UPDATE_UI(idViewToolMain, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewMessageManager, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewManager, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewStatusbar, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewFocusEditor, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewFullScreen, MainFrame::OnViewMenuUpdateUI)

    EVT_EDITOR_UPDATE_UI(MainFrame::OnEditorUpdateUI)
    EVT_NOTEBOOK_PAGE_CHANGED(-1, MainFrame::OnEditorUpdateUI_NB)   //tiwag 050917

    EVT_PLUGIN_ATTACHED(MainFrame::OnPluginLoaded)
    // EVT_PLUGIN_RELEASED(MainFrame::OnPluginUnloaded)

    EVT_MENU(idFileNew, MainFrame::OnFileNewEmpty)
    EVT_MENU(idFileOpen,  MainFrame::OnFileOpen)
    EVT_MENU(idFileOpenRecentProjectClearHistory, MainFrame::OnFileOpenRecentProjectClearHistory)
    EVT_MENU(idFileOpenRecentFileClearHistory, MainFrame::OnFileOpenRecentClearHistory)
    EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, MainFrame::OnFileReopen)
    EVT_MENU_RANGE(wxID_FILE10, wxID_FILE19, MainFrame::OnFileReopenProject)
    EVT_MENU(idFileSave,  MainFrame::OnFileSave)
    EVT_MENU(idFileSaveAs,  MainFrame::OnFileSaveAs)
    EVT_MENU(idFileSaveAllFiles,  MainFrame::OnFileSaveAllFiles)
    EVT_MENU(idFileSaveProject,  MainFrame::OnProjectSaveProject)
    EVT_MENU(idFileSaveProjectAs,  MainFrame::OnProjectSaveProjectAs)
    EVT_MENU(idFileSaveWorkspace,  MainFrame::OnFileSaveWorkspace)
    EVT_MENU(idFileSaveWorkspaceAs,  MainFrame::OnFileSaveWorkspaceAs)
    EVT_MENU(idFileCloseWorkspace,  MainFrame::OnFileCloseWorkspace)
    EVT_MENU(idFileClose,  MainFrame::OnFileClose)
    EVT_MENU(idFileCloseAll,  MainFrame::OnFileCloseAll)
    EVT_MENU(idFilePrint,  MainFrame::OnFilePrint)
    EVT_MENU(idFileRunScript,  MainFrame::OnFileRunScript)
    EVT_MENU(idFileExit,  MainFrame::OnFileQuit)
    EVT_MENU(idFileNext,  MainFrame::OnFileNext)
    EVT_MENU(idFilePrev,  MainFrame::OnFilePrev)

    EVT_MENU(idEditUndo,  MainFrame::OnEditUndo)
    EVT_MENU(idEditRedo,  MainFrame::OnEditRedo)
    EVT_MENU(idEditCopy,  MainFrame::OnEditCopy)
    EVT_MENU(idEditCut,  MainFrame::OnEditCut)
    EVT_MENU(idEditPaste,  MainFrame::OnEditPaste)
    EVT_MENU(idEditSwapHeaderSource,  MainFrame::OnEditSwapHeaderSource)
    EVT_MENU(idEditGotoMatchingBrace,  MainFrame::OnEditGotoMatchingBrace)
    EVT_MENU(idEditHighlightModeText,  MainFrame::OnEditHighlightMode)
    EVT_MENU(idEditFoldAll,  MainFrame::OnEditFoldAll)
    EVT_MENU(idEditUnfoldAll,  MainFrame::OnEditUnfoldAll)
    EVT_MENU(idEditToggleAllFolds,  MainFrame::OnEditToggleAllFolds)
    EVT_MENU(idEditFoldBlock,  MainFrame::OnEditFoldBlock)
    EVT_MENU(idEditUnfoldBlock,  MainFrame::OnEditUnfoldBlock)
    EVT_MENU(idEditToggleFoldBlock,  MainFrame::OnEditToggleFoldBlock)
    EVT_MENU(idEditEOLCRLF,  MainFrame::OnEditEOLMode)
    EVT_MENU(idEditEOLCR,  MainFrame::OnEditEOLMode)
    EVT_MENU(idEditEOLLF,  MainFrame::OnEditEOLMode)
    EVT_MENU(idEditSelectAll,  MainFrame::OnEditSelectAll)
    EVT_MENU(idEditBookmarksToggle,  MainFrame::OnEditBookmarksToggle)
    EVT_MENU(idEditBookmarksNext,  MainFrame::OnEditBookmarksNext)
    EVT_MENU(idEditBookmarksPrevious,  MainFrame::OnEditBookmarksPrevious)
    EVT_MENU(idEditCommentSelected, MainFrame::OnEditCommentSelected)
    EVT_MENU(idEditAutoComplete, MainFrame::OnEditAutoComplete)
    EVT_MENU(idEditUncommentSelected, MainFrame::OnEditUncommentSelected)
    EVT_MENU(idEditToggleCommentSelected, MainFrame::OnEditToggleCommentSelected)

    EVT_MENU(idSearchFind,  MainFrame::OnSearchFind)
    EVT_MENU(idSearchFindInFiles,  MainFrame::OnSearchFind)
    EVT_MENU(idSearchFindNext,  MainFrame::OnSearchFindNext)
    EVT_MENU(idSearchFindPrevious,  MainFrame::OnSearchFindNext)
    EVT_MENU(idSearchReplace,  MainFrame::OnSearchReplace)
    EVT_MENU(idSearchGotoLine,  MainFrame::OnSearchGotoLine)

    EVT_MENU(idViewLayoutSave, MainFrame::OnViewLayoutSave)
    EVT_MENU(idViewLayoutDelete, MainFrame::OnViewLayoutDelete)
    EVT_MENU(idViewToolMain, MainFrame::OnToggleBar)
    EVT_MENU(idViewMessageManager, MainFrame::OnToggleBar)
    EVT_MENU(idViewManager, MainFrame::OnToggleBar)
    EVT_MENU(idViewOpenFilesTree, MainFrame::OnToggleOpenFilesTree)
    EVT_MENU(idViewStatusbar, MainFrame::OnToggleStatusBar)
    EVT_MENU(idViewFocusEditor, MainFrame::OnFocusEditor)
    EVT_MENU(idViewFullScreen, MainFrame::OnToggleFullScreen)

    EVT_MENU(idProjectNewEmptyProject, MainFrame::OnProjectNewEmpty)
    EVT_MENU(idProjectNew, MainFrame::OnProjectNew)
    EVT_MENU(idProjectOpen,  MainFrame::OnProjectOpen)
    EVT_MENU(idProjectSaveProject,  MainFrame::OnProjectSaveProject)
    EVT_MENU(idProjectSaveProjectAs,  MainFrame::OnProjectSaveProjectAs)
    EVT_MENU(idProjectSaveAllProjects,  MainFrame::OnProjectSaveAllProjects)
    EVT_MENU(idProjectSaveTemplate,  MainFrame::OnProjectSaveTemplate)
    EVT_MENU(idProjectCloseProject,  MainFrame::OnProjectCloseProject)
    EVT_MENU(idProjectCloseAllProjects,  MainFrame::OnProjectCloseAllProjects)
    EVT_MENU(idProjectImportDevCpp,  MainFrame::OnProjectImportDevCpp)
    EVT_MENU(idProjectImportMSVC,  MainFrame::OnProjectImportMSVC)
    EVT_MENU(idProjectImportMSVCWksp,  MainFrame::OnProjectImportMSVCWksp)
    EVT_MENU(idProjectImportMSVS,  MainFrame::OnProjectImportMSVS)
    EVT_MENU(idProjectImportMSVSWksp,  MainFrame::OnProjectImportMSVSWksp)

	EVT_MENU(idSettingsEnvironment, MainFrame::OnSettingsEnvironment)
	EVT_MENU(idSettingsGlobalUserVars, MainFrame::OnGlobalUserVars)
	EVT_MENU(idSettingsEditor, MainFrame::OnSettingsEditor)
    EVT_MENU(idPluginsManagePlugins, MainFrame::OnSettingsPlugins)

    EVT_MENU(wxID_ABOUT, MainFrame::OnHelpAbout)
    EVT_MENU(idHelpTips, MainFrame::OnHelpTips)

	EVT_MENU(idStartHerePageLink, MainFrame::OnStartHereLink)
	EVT_MENU(idStartHerePageVarSubst, MainFrame::OnStartHereVarSubst)

	EVT_PROJECT_ACTIVATE(MainFrame::OnProjectActivated)
	EVT_PROJECT_OPEN(MainFrame::OnProjectOpened)
	EVT_PROJECT_CLOSE(MainFrame::OnProjectClosed)
	EVT_EDITOR_CLOSE(MainFrame::OnEditorClosed)
	EVT_EDITOR_OPEN(MainFrame::OnEditorOpened)
	EVT_EDITOR_SAVE(MainFrame::OnEditorSaved)

	// dock a window
	EVT_ADD_DOCK_WINDOW(MainFrame::OnRequestDockWindow)
	EVT_REMOVE_DOCK_WINDOW(MainFrame::OnRequestUndockWindow)
	EVT_SHOW_DOCK_WINDOW(MainFrame::OnRequestShowDockWindow)
	EVT_HIDE_DOCK_WINDOW(MainFrame::OnRequestHideDockWindow)

    EVT_SWITCH_VIEW_LAYOUT(MainFrame::OnLayoutSwitch)

    EVT_NOTEBOOK_PAGE_CHANGED(ID_NBEditorManager, MainFrame::OnPageChanged)

	/// CloseFullScreen event handling
	EVT_BUTTON( idCloseFullScreen, MainFrame::OnToggleFullScreen )

	/// Shift-Tab bug workaround
	EVT_MENU(idShiftTab,MainFrame::OnShiftTab)

END_EVENT_TABLE()

MainFrame::MainFrame(wxLocale& lang, wxWindow* parent)
       : wxFrame(parent, -1, _T("MainWin"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE),
	   m_pAccel(0L),
	   m_locale(lang),
	   m_FilesHistory(9, wxID_FILE1), // default ctor
	   m_ProjectsHistory(9, wxID_FILE10),
	   m_pCloseFullScreenBtn(0L),
	   m_pEdMan(0L),
	   m_pPrjMan(0L),
	   m_pMsgMan(0L),
	   m_pToolbar(0L),
       m_ToolsMenu(0L),
       m_SettingsMenu(0L),
       m_HelpPluginsMenu(0L),
       m_ReconfiguringPlugins(false),
       m_StartupDone(false) // one-time flag
{
    // tell wxFrameManager to manage this frame
    m_LayoutManager.SetFrame(this);

#if defined( _MSC_VER ) && defined( _DEBUG )
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	//tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag( tmpFlag );
#endif

    // New: Allow drag and drop of files into the editor
    SetDropTarget(new wxMyFileDropTarget(this));

    // Accelerator table
    wxAcceleratorEntry entries[7];

    entries[0].Set(wxACCEL_CTRL | wxACCEL_SHIFT,  (int) 'W', idFileCloseAll);
    entries[1].Set(wxACCEL_CTRL | wxACCEL_SHIFT,  WXK_F4, idFileCloseAll);
    entries[2].Set(wxACCEL_CTRL,  (int) 'W', idFileClose);
    entries[3].Set(wxACCEL_CTRL,  WXK_F4, idFileClose);
    entries[4].Set(wxACCEL_CTRL,  WXK_F6, idFileNext);
    entries[5].Set(wxACCEL_CTRL | wxACCEL_SHIFT,  WXK_F6, idFilePrev);
    entries[6].Set(wxACCEL_SHIFT,  WXK_TAB, idShiftTab);
    m_pAccel = new wxAcceleratorTable(7, entries);

    this->SetAcceleratorTable(*m_pAccel);

    m_SmallToolBar = Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/environment/toolbar_size"), true);
	CreateIDE();

#ifdef __WXMSW__
    SetIcon(wxICON(A_MAIN_ICON));
#else
    SetIcon(wxIcon(app));
#endif // __WXMSW__

    DoCreateStatusBar();
#if wxUSE_STATUSBAR
    SetStatusText(_("Welcome to ")+ APP_NAME + _T("!"));
#endif // wxUSE_STATUSBAR

    SetTitle(APP_NAME + _T(" v") + APP_VERSION);

    ScanForPlugins();
    // save default view
    wxString deflayout = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/default"));
    if (deflayout.IsEmpty())
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/default"), gDefaultLayout);
    DoFixToolbarsLayout();
    gDefaultLayoutData = m_LayoutManager.SavePerspective(); // keep the "hardcoded" layout handy
    SaveViewLayout(gDefaultLayout, gDefaultLayoutData);
    LoadWindowState();

    ShowHideStartPage();
    m_StartupDone = true;
    DoUpdateLayout();
}

MainFrame::~MainFrame()
{
    this->SetAcceleratorTable(wxNullAcceleratorTable);
    delete m_pAccel;

    DeInitPrinting();
	//Manager::Get()->Free();
}

void MainFrame::ShowTips(bool forceShow)
{
    bool showAtStartup = Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/show_tips"), true);
    if (forceShow || showAtStartup)
    {
        wxLogNull null; // disable error message if tips file does not exist
        wxString tipsFile = ConfigManager::GetDataFolder() + _T("/tips.txt");
        long tipsIndex = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/next_tip"), 0);
        wxTipProvider* tipProvider = wxCreateFileTipProvider(tipsFile, tipsIndex);
        showAtStartup = wxShowTip(this, tipProvider, showAtStartup);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/show_tips"), showAtStartup);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/next_tip"), (int)tipProvider->GetCurrentTip());
        delete tipProvider;
    }
}

void MainFrame::CreateIDE()
{
	int leftW = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/left_block_width"), 200);
	int bottomH = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/bottom_block_height"), 150);
	SetSize(800,600);
	wxSize clientsize = GetClientSize();

	// Create CloseFullScreen Button, and hide it initially
	m_pCloseFullScreenBtn = new wxButton(this, idCloseFullScreen, _( "Close Fullscreen" ), wxDefaultPosition );
	m_pCloseFullScreenBtn->Show( false );

    // project manager
	Manager::Get(this);
    m_LayoutManager.AddPane(Manager::Get()->GetProjectManager()->GetNotebook(), wxPaneInfo().
                              Name(wxT("ManagementPane")).Caption(_("Management")).
                              BestSize(wxSize(leftW, clientsize.GetHeight())).MinSize(wxSize(100,100)).
                              Left().Layer(1));

    // message manager
    m_LayoutManager.AddPane(Manager::Get()->GetMessageManager()->GetNotebook(), wxPaneInfo().
                              Name(wxT("MessagesPane")).Caption(_("Messages")).
                              BestSize(wxSize(clientsize.GetWidth(), bottomH)).//MinSize(wxSize(50,50)).
                              Bottom());

	CreateMenubar();

	m_pEdMan = Manager::Get()->GetEditorManager();
	m_pPrjMan = Manager::Get()->GetProjectManager();
	m_pMsgMan = Manager::Get()->GetMessageManager();

    CreateToolbars();
    SetToolBar(0);

    // editor manager
    m_LayoutManager.AddPane(m_pEdMan->GetNotebook(), wxPaneInfo().Name(wxT("MainPane")).
                            CenterPane());

    DoUpdateLayout();
    DoUpdateLayoutColors();
    DoUpdateEditorStyle();
}

wxMenu* MainFrame::RecreateMenu(wxMenuBar* mbar, const wxString& name)
{
    wxMenu* menu = 0;
    int idx = mbar->FindMenu(name);
    if (idx != wxNOT_FOUND)
        menu = mbar->GetMenu(idx);

    if (!menu)
    {
        menu = new wxMenu();
        mbar->Append(menu, name);
    }
    else
    {
        while (menu->GetMenuItemCount() > 0)
        {
            menu->Destroy(menu->GetMenuItems()[0]);
        }
    }

    return menu;
}

void MainFrame::CreateMenubar()
{
	int tmpidx;
	wxMenuBar* mbar=0L;
	wxMenu *hl=0L, *tools=0L, *plugs=0L, *pluginsM=0L, *settingsPlugins=0L;
	wxMenuItem *tmpitem=0L;

    wxString resPath = ConfigManager::GetDataFolder();
    wxXmlResource *myres = wxXmlResource::Get();
    myres->Load(resPath + _T("/resources.zip#zip:main_menu.xrc"));
    mbar = myres->LoadMenuBar(_T("main_menu_bar"));
    if(!mbar)
    {
      mbar = new wxMenuBar(); // Some error happened.
      SetMenuBar(mbar);
    }

    // Find Menus that we'll change later

    tmpidx=mbar->FindMenu(_("&Edit"));
    if(tmpidx!=wxNOT_FOUND)
    {
        mbar->FindItem(idEditHighlightModeText, &hl);
        if (hl)
        {
            EditorColorSet* theme = Manager::Get()->GetEditorManager()->GetColorSet();
            if (theme)
            {
                wxArrayString langs = theme->GetAllHighlightLanguages();
                for (size_t i = 0; i < langs.GetCount(); ++i)
                {
                    if (i == 20 || i == 40 || i ==60 || i == 80)
                        hl->Break(); // break into columns every 20 items
                    int id = wxNewId();
                    hl->Append(id, langs[i],
                                wxString::Format(_("Switch highlighting mode for current document to \"%s\""), langs[i].c_str()));
                    Connect(id, -1, wxEVT_COMMAND_MENU_SELECTED,
                            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
                            &MainFrame::OnEditHighlightMode);
                }
            }
        }
    }

    tmpidx=mbar->FindMenu(_("&Tools"));
    if(tmpidx!=wxNOT_FOUND)
        tools = mbar->GetMenu(tmpidx);

    tmpidx=mbar->FindMenu(_("P&lugins"));
    if(tmpidx!=wxNOT_FOUND)
        plugs = mbar->GetMenu(tmpidx);

    if((tmpitem = mbar->FindItem(idSettingsConfigurePlugins,NULL)))
        settingsPlugins = tmpitem->GetSubMenu();
    if((tmpitem = mbar->FindItem(idHelpPlugins,NULL)))
        pluginsM = tmpitem->GetSubMenu();

	m_ToolsMenu = tools ? tools : new wxMenu();
	m_PluginsMenu = plugs ? plugs : new wxMenu();
	m_SettingsMenu = settingsPlugins ? settingsPlugins : new wxMenu();
	m_HelpPluginsMenu = pluginsM ? pluginsM : new wxMenu();

	// core modules: create menus
	Manager::Get()->GetProjectManager()->CreateMenu(mbar);
	Manager::Get()->GetEditorManager()->CreateMenu(mbar);
	Manager::Get()->GetMessageManager()->CreateMenu(mbar);

	// ask all plugins to rebuild their menus
	PluginElementsArray plugins = Manager::Get()->GetPluginManager()->GetPlugins();
	for (unsigned int i = 0; i < plugins.GetCount(); ++i)
	{
		cbPlugin* plug = plugins[i]->plugin;
		if (plug && plug->IsAttached())
		{
			if (plug->GetType() == ptTool)
			{
                DoAddPlugin(plug);
            }
			else
			{
                AddPluginInSettingsMenu(plug);
                AddPluginInHelpPluginsMenu(plug);
                try
                {
                    plug->BuildMenu(mbar);
                }
                catch (cbException& e)
                {
                    e.ShowErrorMessage();
                }
            }
		}
	}

	Manager::Get()->GetToolsManager()->BuildToolsMenu(m_ToolsMenu);

	SetMenuBar(mbar);
    InitializeRecentFilesHistory();
}

void MainFrame::CreateToolbars()
{
	wxXmlResource *myres = wxXmlResource::Get();
	if (m_pToolbar)
	{
		SetToolBar(0L);
//		delete m_pToolbar;
		m_pToolbar = 0L;
	}

    wxString resPath = ConfigManager::GetDataFolder();
    wxString xrcToolbarName = _T("main_toolbar");
    if(m_SmallToolBar) // Insert logic here
        xrcToolbarName += _T("_16x16");
    myres->Load(resPath + _T("/resources.zip#zip:*.xrc"));
    Manager::Get()->GetMessageManager()->DebugLog(_("Loading toolbar..."));

    wxSize size = m_SmallToolBar ? wxSize(16, 16) : wxSize(22, 22);
    m_pToolbar = new wxToolBar(this, -1, wxDefaultPosition, size, wxTB_FLAT | wxTB_NODIVIDER);
    m_pToolbar->SetToolBitmapSize(size);
    Manager::Get()->AddonToolBar(m_pToolbar,xrcToolbarName);
	m_pToolbar->Realize();

    // add toolbars in docking system
    m_LayoutManager.AddPane(m_pToolbar, wxPaneInfo().
                          Name(wxT("MainToolbar")).Caption(_("Main Toolbar")).
                          ToolbarPane().Top());
    DoUpdateLayout();

	// ask all plugins to rebuild their toolbars
	PluginElementsArray plugins = Manager::Get()->GetPluginManager()->GetPlugins();
	for (unsigned int i = 0; i < plugins.GetCount(); ++i)
	{
		cbPlugin* plug = plugins[i]->plugin;
		if (plug && plug->IsAttached())
		{
            DoAddPluginToolbar(plug);
		}
	}

	wxSafeYield();
}

void MainFrame::AddToolbarItem(int id, const wxString& title, const wxString& shortHelp, const wxString& longHelp, const wxString& image)
{
	m_pToolbar->AddTool(id, title, wxBitmap(image, wxBITMAP_TYPE_PNG));
	m_pToolbar->SetToolShortHelp(id, shortHelp);
	m_pToolbar->SetToolLongHelp(id, longHelp);
}

void MainFrame::ScanForPlugins()
{
    m_PluginIDsMap.clear();

    PluginManager* m_PluginManager = Manager::Get()->GetPluginManager();

    wxString path = ConfigManager::GetDataFolder() + _T("/plugins");
    Manager::Get()->GetMessageManager()->Log(_("Scanning for plugins in %s..."), path.c_str());
    int count = m_PluginManager->ScanForPlugins(path);
    if (count > 0)
    {
        Manager::Get()->GetMessageManager()->AppendLog(_("Loading: "));
        // actually load plugins
        m_PluginManager->LoadAllPlugins();
    }
}

wxMenuItem* MainFrame::AddPluginInMenus(wxMenu* menu, cbPlugin* plugin, wxObjectEventFunction callback, int pos, bool checkable)
{
    wxMenuItem* item = 0;
    if (!plugin || !menu)
		return item;

    PluginIDsMap::iterator it;
    for (it = m_PluginIDsMap.begin(); it != m_PluginIDsMap.end(); ++it)
    {
        if (it->second == plugin->GetInfo()->name)
        {
            item = menu->FindItem(it->first);
            if (item)
                return item;
        }
    }

    int id = wxNewId();
    m_PluginIDsMap[id] = plugin->GetInfo()->name;
    if (pos == -1)
        item = menu->Append(id, plugin->GetInfo()->title, wxEmptyString, checkable ? wxITEM_CHECK : wxITEM_NORMAL);
    else
        item = menu->Insert(pos, id, plugin->GetInfo()->title, wxEmptyString, checkable ? wxITEM_CHECK : wxITEM_NORMAL);
    Connect( id,  wxEVT_COMMAND_MENU_SELECTED, callback );
    return item;
}

void MainFrame::AddPluginInPluginsMenu(cbPlugin* plugin)
{
    // "Plugins" menu is special case because it contains "Manage plugins",
    // which must stay at the end of the menu
    // So we insert entries, not append...

    // this will insert a separator when the first plugin is added in the "Plugins" menu
    if (m_PluginsMenu->GetMenuItemCount() == 1)
         m_PluginsMenu->Insert(0, wxID_SEPARATOR, _T(""));

    AddPluginInMenus(m_PluginsMenu, plugin,
                    (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnPluginsExecuteMenu,
                    m_PluginsMenu->GetMenuItemCount() - 2);
}

void MainFrame::AddPluginInSettingsMenu(cbPlugin* plugin)
{
    if(!plugin)
        return;
    if (!plugin->GetInfo()->hasConfigure)
        return;
    AddPluginInMenus(m_SettingsMenu, plugin,
                    (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnPluginSettingsMenu);
}

void MainFrame::AddPluginInHelpPluginsMenu(cbPlugin* plugin)
{
    AddPluginInMenus(m_HelpPluginsMenu, plugin,
                    (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnHelpPluginMenu);
}

void MainFrame::RemovePluginFromMenus(const wxString& pluginName)
{
    //Manager::Get()->GetMessageManager()->DebugLog("Unloading %s plugin", pluginName.c_str());
	if (pluginName.IsEmpty())
		return;

	// look for plugin's id
	wxArrayInt id;
	PluginIDsMap::iterator it = m_PluginIDsMap.begin();
	while (it != m_PluginIDsMap.end())
	{
		if (pluginName.Matches(it->second))
		{
			id.Add(it->first);
			PluginIDsMap::iterator it2 = it;
			++it;
			m_PluginIDsMap.erase(it2);
        }
        else
            ++it;
	}
    //Manager::Get()->GetMessageManager()->DebugLog("id=%d", id);
	if (id.GetCount() == 0)
		return; // not found

	for (unsigned int i = 0; i < id.GetCount(); ++i)
	{
		Disconnect( id[i],  wxEVT_COMMAND_MENU_SELECTED,
			(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
			&MainFrame::OnPluginsExecuteMenu );
		m_PluginIDsMap.erase(id[i]);
		m_PluginsMenu->Delete(id[i]);
		m_HelpPluginsMenu->Delete(id[i]);
		m_SettingsMenu->Delete(id[i]);
	}
}

void MainFrame::LoadWindowState()
{
	wxLogNull ln; // no logging needed

    wxArrayString subs = Manager::Get()->GetConfigManager(_T("app"))->EnumerateSubPaths(_T("/main_frame/layout"));
    for (size_t i = 0; i < subs.GetCount(); ++i)
    {
        wxString name = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/") + subs[i] + _T("/name"));
        wxString layout = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/") + subs[i] + _T("/data"));
        SaveViewLayout(name, layout);
    }
    wxString deflayout = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/default"));
    LoadViewLayout(deflayout);

	// load manager and messages selected page
	Manager::Get()->GetProjectManager()->GetNotebook()->SetSelection(Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/left_block_selection"), 0));
	Manager::Get()->GetMessageManager()->GetNotebook()->SetSelection(Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/bottom_block_selection"), 0));

    if (!IsMaximized() && !IsIconized())
    {
        // load window size and position
        SetSize(Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/left"), 0),
                Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/top"), 0),
                Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/width"), 640),
                Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/height"), 480));
        // maximized?
        if (Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/main_frame/layout/maximized"), false))
            Maximize();
    }

    // close message manager (if auto-hiding)
    Manager::Get()->GetMessageManager()->Close();
}

void MainFrame::SaveWindowState()
{
	wxLogNull ln; // no logging needed
    DoCheckCurrentLayoutForChanges(false);

    int count = 0;
    for (LayoutViewsMap::iterator it = m_LayoutViews.begin(); it != m_LayoutViews.end(); ++it)
    {
        if (it->first.IsEmpty())
            continue;
        ++count;
        wxString key = wxString::Format(_T("/main_frame/layout/view%d/"), count);
        Manager::Get()->GetConfigManager(_T("app"))->Write(key + _T("name"), it->first);
        Manager::Get()->GetConfigManager(_T("app"))->Write(key + _T("data"), it->second);
    }

	// save manager and messages selected page
	Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/left_block_selection"), Manager::Get()->GetProjectManager()->GetNotebook()->GetSelection());
	Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/bottom_block_selection"), Manager::Get()->GetMessageManager()->GetNotebook()->GetSelection());

    // save window size and position
    Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/maximized"), IsMaximized());
    if (!IsMaximized() && !IsIconized())
    {
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/left"), GetPosition().x);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/top"), GetPosition().y);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/width"), GetSize().x);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/height"), GetSize().y);
    }

}

void MainFrame::LoadViewLayout(const wxString& name)
{
    if (m_LastLayoutName != name && !DoCheckCurrentLayoutForChanges(true))
        return;

    wxString layout = m_LayoutViews[name];
    if (layout.IsEmpty())
    {
        layout = m_LayoutViews[gDefaultLayout];
        DoSelectLayout(gDefaultLayout);
    }
    else
        DoSelectLayout(name);

    m_LayoutManager.LoadPerspective(layout, false);
    DoFixToolbarsLayout();
    DoUpdateLayout();

    m_LastLayoutName = name;
    m_LastLayoutData = layout;
}

void MainFrame::SaveViewLayout(const wxString& name, const wxString& layout, bool select)
{
    if (name.IsEmpty())
        return;
    m_LayoutViews[name] = layout;
    wxMenu* viewLayouts = 0;
    GetMenuBar()->FindItem(idViewLayoutSave, &viewLayouts);
    if (viewLayouts && viewLayouts->FindItem(name) == wxNOT_FOUND)
    {
        int id = wxNewId();
        viewLayouts->InsertCheckItem(viewLayouts->GetMenuItemCount() - 3, id, name, wxString::Format(_("Switch to %s layout"), name.c_str()));
        Connect( id,  wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnViewLayout);
        m_PluginIDsMap[id] = name;
    }
    if (select)
        DoSelectLayout(name);
}

bool MainFrame::DoCheckCurrentLayoutForChanges(bool canCancel)
{
    DoFixToolbarsLayout();
    wxString lastlayout = m_LayoutManager.SavePerspective();
    if (!m_LastLayoutName.IsEmpty() && lastlayout != m_LastLayoutData)
    {
        AnnoyingDialog dlg(_("Layout changed"),
                            wxString::Format(_("The layout '%s' has changed. Do you want to save it?"), m_LastLayoutName.c_str()),
                            wxART_QUESTION,
                            canCancel ? AnnoyingDialog::YES_NO_CANCEL : AnnoyingDialog::YES_NO,
                            wxID_YES);
        switch (dlg.ShowModal())
        {
            case wxID_YES:
                SaveViewLayout(m_LastLayoutName, lastlayout, false);
                break;
            case wxID_CANCEL:
                DoSelectLayout(m_LastLayoutName);
                return false;
            default:
                break;
        }
    }
    return true;
}

void MainFrame::DoFixToolbarsLayout()
{
    // because the user might change the toolbar icons size, we must cater for it...
    wxPaneInfoArray& panes = m_LayoutManager.GetAllPanes();
    for (size_t i = 0; i < panes.GetCount(); ++i)
    {
        wxPaneInfo& info = panes[i];
        if (info.state & wxPaneInfo::optionToolbar)
        {
            info.best_size = info.window->GetSize();
        }
    }
}

void MainFrame::DoSelectLayout(const wxString& name)
{
    wxMenu* viewLayouts = 0;
    GetMenuBar()->FindItem(idViewLayoutSave, &viewLayouts);
    if (viewLayouts)
    {
        wxMenuItemList& items = viewLayouts->GetMenuItems();
        for (size_t i = 0; i < items.GetCount(); ++i)
        {
//            if (!items[i]->IsCheckable())
//                continue;
            items[i]->Check(items[i]->GetText().IsSameAs(name));
        }
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/default"), name);
    }
}

void MainFrame::DoAddPluginToolbar(cbPlugin* plugin)
{
    wxSize size = m_SmallToolBar ? wxSize(16, 16) : wxSize(22, 22);
    wxToolBar* tb = new wxToolBar(this, -1, wxDefaultPosition, size, wxTB_FLAT | wxTB_NODIVIDER);
    tb->SetToolBitmapSize(size);
    if (plugin->BuildToolBar(tb))
    {
        SetToolBar(0);

        // add View->Toolbars menu item for toolbar
        wxMenu* viewToolbars = 0;
        GetMenuBar()->FindItem(idViewToolMain, &viewToolbars);
        if (viewToolbars)
        {
            wxMenuItem* item = AddPluginInMenus(viewToolbars, plugin,
                                                (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnToggleBar,
                                                -1, true);
            if (item)
            {
                item->Check(true);
                m_PluginsTools[plugin] = tb;
            }
        }

        m_LayoutManager.AddPane(tb, wxPaneInfo().
                              Name(plugin->GetInfo()->name + _T("Toolbar")).Caption(plugin->GetInfo()->title + _(" Toolbar")).
                              ToolbarPane().Top().Row(1));
        DoUpdateLayout();
    }
    else
        delete tb;
}

void MainFrame::DoAddPlugin(cbPlugin* plugin)
{
    //Manager::Get()->GetMessageManager()->DebugLog(_("Adding plugin: %s"), plugin->GetInfo()->name.c_str());
    AddPluginInSettingsMenu(plugin);
    AddPluginInHelpPluginsMenu(plugin);
    if (plugin->GetType() == ptTool)
    {
        AddPluginInPluginsMenu(plugin);
    }
    // offer menu and toolbar space for other plugins
	else
    {
        // menu
        try
        {
            plugin->BuildMenu(GetMenuBar());
        }
        catch (cbException& e)
        {
            e.ShowErrorMessage();
        }
        // toolbar
        DoAddPluginToolbar(plugin);
    }
}

bool MainFrame::Open(const wxString& filename, bool addToHistory)
{
    wxFileName fn(filename);
    fn.Normalize(); // really important so that two same files with different names are not loaded twice
    wxString name = fn.GetFullPath();
    //Manager::Get()->GetMessageManager()->DebugLog(_("Opening file '%s'"), sname.c_str());
    Manager::Get()->GetMessageManager()->DebugLog(_("Opening file %s"), name.c_str());
    bool ret = OpenGeneric(name, addToHistory);
	return ret;
}

wxString MainFrame::ShowOpenFileDialog(const wxString& caption, const wxString& filter)
{
    wxFileDialog* dlg = new wxFileDialog(this,
                            caption,
                            wxEmptyString,
                            wxEmptyString,
                            filter,
                            wxOPEN);
    wxString sel;
    if (dlg->ShowModal() == wxID_OK)
		sel = dlg->GetPath();
    delete dlg;
    return sel;
}

bool MainFrame::OpenGeneric(const wxString& filename, bool addToHistory)
{
    if (filename.IsEmpty())
        return false;
    switch(FileTypeOf(filename))
    {
        //
        // Workspaces
        //
        case ftCodeBlocksWorkspace:
            // fallthrough
        case ftMSVCWorkspace:
            // fallthrough
        case ftMSVSWorkspace:
            // verify that it's not the same as the one already open
            if (filename != Manager::Get()->GetProjectManager()->GetWorkspace()->GetFilename() &&
                DoCloseCurrentWorkspace())
            {
                return Manager::Get()->GetProjectManager()->LoadWorkspace(filename);
                AddToRecentProjectsHistory(filename);
            }
            else
                return false;
            break;

        //
        // Projects
        //
        case ftCodeBlocksProject:
            // fallthrough
        case ftDevCppProject:
            // fallthrough
        case ftMSVCProject:
            // fallthrough
        case ftMSVSProject:
            return DoOpenProject(filename, addToHistory);

        //
        // Source files
        //
        case ftHeader:
            // fallthrough
        case ftSource:
            // fallthrough
        case ftResource:
            return DoOpenFile(filename, addToHistory);

        //
        // For all other files, ask MIME plugin for a suitable handler
        //
        default:
        {
            cbMimePlugin* plugin = Manager::Get()->GetPluginManager()->GetMIMEHandlerForFile(filename);
            return plugin && plugin->OpenFile(filename) == 0;
        }
    }
    return true;
}

bool MainFrame::DoOpenProject(const wxString& filename, bool addToHistory)
{
//    Manager::Get()->GetMessageManager()->DebugLog(_("Opening project '%s'"), filename.c_str());
    if (!wxFileExists(filename))
    {
        wxMessageBox(_("The project file does not exist..."), _("Error"), wxICON_ERROR);
        return false;
    }

    cbProject* prj = Manager::Get()->GetProjectManager()->LoadProject(filename);
    if (prj)
    {
		if (addToHistory)
			AddToRecentProjectsHistory(prj->GetFilename());
        return true;
    }
    return false;
}

bool MainFrame::DoOpenFile(const wxString& filename, bool addToHistory)
{
    if (Manager::Get()->GetEditorManager()->Open(filename))
    {
		if (addToHistory)
			AddToRecentFilesHistory(filename);
        return true;
    }
    return false;
}

bool MainFrame::DoCloseCurrentWorkspace()
{
    return Manager::Get()->GetProjectManager()->CloseWorkspace();
}

void MainFrame::DoCreateStatusBar()
{
#if wxUSE_STATUSBAR
#define COUNT 5
    CreateStatusBar(COUNT);
    int statusWidths[COUNT] = {-1, 148, 64, 64, 96};
    SetStatusWidths(COUNT, statusWidths);
#undef COUNT
#endif // wxUSE_STATUSBAR
}

void MainFrame::DoUpdateStatusBar()
{
#if wxUSE_STATUSBAR
    if (!GetStatusBar())
        return;
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        int pos = ed->GetControl()->GetCurrentPos();
        wxString msg;
        msg.Printf(_("Line %d, Column %d"), ed->GetControl()->GetCurrentLine() + 1, ed->GetControl()->GetColumn(pos) + 1);
        SetStatusText(msg, 1);
        SetStatusText(ed->GetControl()->GetOvertype() ? _("Overwrite") : _("Insert"), 2);
        SetStatusText(ed->GetModified() ? _("Modified") : wxEmptyString, 3);
        SetStatusText(ed->GetControl()->GetReadOnly() ? _("Read only") : _("Read/Write"), 4);
        SetStatusText(ed->GetFilename(), 0);                    //tiwag 050917
    }
    else
    {
        SetStatusText(_("Welcome to ") + APP_NAME + _T("!"));             //tiwag 050917
        SetStatusText(wxEmptyString, 1);
        SetStatusText(wxEmptyString, 2);
        SetStatusText(wxEmptyString, 3);
        SetStatusText(wxEmptyString, 4);
    }
#endif // wxUSE_STATUSBAR
}

void MainFrame::DoUpdateEditorStyle(wxFlatNotebook* target, const wxString& prefix, long defaultStyle)
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("app"));
    long nbstyle = cfg->ReadInt(_T("/environment/tabs_style"), 0);
    switch (nbstyle)
    {
        case 1: // gradient
            nbstyle = wxFNB_FANCY_TABS;
            break;

        case 2: // vc71
            nbstyle = wxFNB_VC71;
            break;

        default:
            nbstyle = 0;
            break;
    }
    nbstyle |= defaultStyle;
    nbstyle |= cfg->ReadInt(_T("/environment/") + prefix + _T("_tabs_style"));

    target->SetBookStyle(nbstyle);
    target->SetGradientColorBorder(cfg->ReadColour(_T("/environment/gradient_border"), wxColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW))));
    target->SetGradientColorFrom(cfg->ReadColour(_T("/environment/gradient_from"), wxColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE))));
    target->SetGradientColorTo(cfg->ReadColour(_T("/environment/gradient_to"), *wxWHITE));
}

void MainFrame::DoUpdateEditorStyle()
{
    wxFlatNotebook* fn = Manager::Get()->GetEditorManager()->GetNotebook();
    DoUpdateEditorStyle(fn, _T("editor"), wxFNB_MOUSE_MIDDLE_CLOSES_TABS);

    fn = Manager::Get()->GetMessageManager()->GetNotebook();
    DoUpdateEditorStyle(fn, _T("message"), wxFNB_NO_X_BUTTON);

    fn = Manager::Get()->GetProjectManager()->GetNotebook();
    DoUpdateEditorStyle(fn, _T("project"), wxFNB_NO_X_BUTTON);
}

void MainFrame::DoUpdateLayoutColors()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("app"));
    wxDockArt* art = m_LayoutManager.GetArtProvider();

    m_LayoutManager.SetFlags(wxAUI_MGR_DEFAULT | wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_TRANSPARENT_DRAG);
    art->SetMetric(wxAUI_ART_PANE_BORDER_SIZE,   cfg->ReadInt(_T("/environment/aui/border_size"), art->GetMetric(wxAUI_ART_PANE_BORDER_SIZE)));
    art->SetMetric(wxAUI_ART_SASH_SIZE,          cfg->ReadInt(_T("/environment/aui/sash_size"), art->GetMetric(wxAUI_ART_SASH_SIZE)));
    art->SetMetric(wxAUI_ART_CAPTION_SIZE,       cfg->ReadInt(_T("/environment/aui/caption_size"), art->GetMetric(wxAUI_ART_CAPTION_SIZE)));
    art->SetColor(wxAUI_ART_BACKGROUND_COLOUR,   cfg->ReadColour(_T("/environment/aui/bg_color"), art->GetColor(wxAUI_ART_BACKGROUND_COLOUR)));
    art->SetColor(wxAUI_ART_SASH_COLOUR,         cfg->ReadColour(_T("/environment/aui/sash_color"), art->GetColor(wxAUI_ART_SASH_COLOUR)));
    art->SetColor(wxAUI_ART_ACTIVE_CAPTION_COLOUR,              cfg->ReadColour(_T("/environment/aui/active_caption_color"), art->GetColor(wxAUI_ART_ACTIVE_CAPTION_COLOUR)));
    art->SetColor(wxAUI_ART_ACTIVE_CAPTION_GRADIENT_COLOUR,     cfg->ReadColour(_T("/environment/aui/active_caption_gradient_color"), art->GetColor(wxAUI_ART_ACTIVE_CAPTION_GRADIENT_COLOUR)));
    art->SetColor(wxAUI_ART_ACTIVE_CAPTION_TEXT_COLOUR,         cfg->ReadColour(_T("/environment/aui/active_caption_text_color"), art->GetColor(wxAUI_ART_ACTIVE_CAPTION_TEXT_COLOUR)));
    art->SetColor(wxAUI_ART_INACTIVE_CAPTION_COLOUR,            cfg->ReadColour(_T("/environment/aui/inactive_caption_color"), art->GetColor(wxAUI_ART_INACTIVE_CAPTION_COLOUR)));
    art->SetColor(wxAUI_ART_INACTIVE_CAPTION_GRADIENT_COLOUR,   cfg->ReadColour(_T("/environment/aui/inactive_caption_gradient_color"), art->GetColor(wxAUI_ART_INACTIVE_CAPTION_GRADIENT_COLOUR)));
    art->SetColor(wxAUI_ART_INACTIVE_CAPTION_TEXT_COLOUR,       cfg->ReadColour(_T("/environment/aui/inactive_caption_text_color"), art->GetColor(wxAUI_ART_INACTIVE_CAPTION_TEXT_COLOUR)));
    art->SetColor(wxAUI_ART_BORDER_COLOUR,       cfg->ReadColour(_T("/environment/aui/border_color"), art->GetColor(wxAUI_ART_BORDER_COLOUR)));
    art->SetColor(wxAUI_ART_GRIPPER_COLOUR,      cfg->ReadColour(_T("/environment/aui/gripper_color"), art->GetColor(wxAUI_ART_GRIPPER_COLOUR)));

    DoUpdateLayout();
}

void MainFrame::DoUpdateLayout()
{
    if (!m_StartupDone)
        return;
    m_LayoutManager.Update();
}

void MainFrame::DoUpdateAppTitle()
{
	EditorBase* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetActiveEditor() : 0L;
	cbProject* prj = 0;
	if(ed && ed->IsBuiltinEditor())
	{
	    ProjectFile* prjf = ((cbEditor*)ed)->GetProjectFile();
	    if(prjf)
            prj = prjf->GetParentProject();
	}
	else
        prj = Manager::Get()->GetProjectManager() ? Manager::Get()->GetProjectManager()->GetActiveProject() : 0L;
	wxString projname;
	wxString edname;
	wxString fulltitle;
	if(ed || prj)
	{
        if(prj)
        {
            if(Manager::Get()->GetProjectManager()->GetActiveProject() == prj)
                projname = wxString(_T(" [")) + prj->GetTitle() + _T("]");
            else
                projname = wxString(_T(" (")) + prj->GetTitle() + _T(")");
        }
        if(ed)
            edname = ed->GetTitle();
        fulltitle = edname + projname;
        if(!fulltitle.IsEmpty())
            fulltitle.Append(_T(" - "));
	}
    fulltitle.Append(APP_NAME);
    fulltitle.Append(_T(" v"));
    fulltitle.Append(APP_VERSION);
    SetTitle(fulltitle);
}

void MainFrame::ShowHideStartPage(bool forceHasProject)
{
    // we use the 'forceHasProject' param because when a project is opened
    // the EVT_PROJECT_OPEN event is fired *before* ProjectManager::GetProjects()
    // and ProjectManager::GetActiveProject() are updated...
    if(Manager::isappShuttingDown())
        return;
    bool show = !forceHasProject &&
                Manager::Get()->GetProjectManager()->GetProjects()->GetCount() == 0 &&
                Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/environment/start_here_page"), true);

    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
    if (show && !sh)
        sh = new StartHerePage(this, Manager::Get()->GetEditorManager()->GetNotebook());
    else if (!show && sh)
        sh->Destroy();
}

void MainFrame::OnStartHereLink(wxCommandEvent& event)
{
    wxCommandEvent evt;
    wxString link = event.GetString();
    if (link.Matches(_T("CB_CMD_NEW_PROJECT")))
        TemplateManager::Get()->NewProject();
    else if (link.Matches(_T("CB_CMD_OPEN_PROJECT")))
        OnProjectOpen(evt);
    else if (link.Matches(_T("CB_CMD_CONF_ENVIRONMENT")))
        OnSettingsEnvironment(evt);
    else if (link.Matches(_T("CB_CMD_CONF_EDITOR")))
        Manager::Get()->GetEditorManager()->Configure();
    else if (link.Matches(_T("CB_CMD_CONF_COMPILER")))
    {
        PluginsArray arr = Manager::Get()->GetPluginManager()->GetCompilerOffers();
        if (arr.GetCount() != 0)
            arr[0]->Configure();
    }
    else if (link.StartsWith(_T("CB_CMD_OPEN_HISTORY_")))
    {
        wxFileHistory* hist = link.StartsWith(_T("CB_CMD_OPEN_HISTORY_PROJECT_")) ? &m_ProjectsHistory : &m_FilesHistory;
		unsigned long count;
		link.AfterLast(_T('_')).ToULong(&count);
		--count;
        if(count < hist->GetCount())
        {
            if (!OpenGeneric(hist->GetHistoryFile(count), true))
            {
                // invalid file, update start here page
                hist->RemoveFileFromHistory(count);
                EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
                if (sh)
                    ((StartHerePage*)sh)->Reload();
            }
        }
    }
}

void MainFrame::OnStartHereVarSubst(wxCommandEvent& event)
{
    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
	if (!sh)
        return;

	// replace history vars

	wxString buf = event.GetString();
	wxString links;

    if (m_ProjectsHistory.GetCount())
    {
        links << _T("<b>Recent projects</b><br>\n");
        for (int i = 0; i < 5; ++i)
        {
            if (i >= (int)m_ProjectsHistory.GetCount())
                break;
            links << wxString::Format(_T("<a href=\"CB_CMD_OPEN_HISTORY_PROJECT_%d\">%s</a><br>\n"),
                                        i + 1, m_ProjectsHistory.GetHistoryFile(i).c_str());
        }
    }

    if (m_FilesHistory.GetCount())
    {
        links << _T("<br><b>Recent files</b><br>\n");
        for (int i = 0; i < 5; ++i)
        {
            if (i >= (int)m_FilesHistory.GetCount())
                break;
            links << wxString::Format(_T("<a href=\"CB_CMD_OPEN_HISTORY_FILE_%d\">%s</a><br>\n"),
                                        i + 1, m_FilesHistory.GetHistoryFile(i).c_str());
        }
    }

    // update page
    buf.Replace(_T("CB_VAR_RECENT_FILES_AND_PROJECTS"), links);
    ((StartHerePage*)sh)->SetPageContent(buf);
}

void MainFrame::InitializeRecentFilesHistory()
{
    wxMenuBar* mbar = GetMenuBar();
    if (!mbar)
        return;
    int pos = mbar->FindMenu(_("&File"));
    if (pos != wxNOT_FOUND)
    {
        wxMenu* menu = mbar->GetMenu(pos);
        if (!menu)
            return;
        wxMenu* recentFiles = 0;
        wxMenuItem* clear = menu->FindItem(idFileOpenRecentFileClearHistory, &recentFiles);
        if (recentFiles)
        {
            recentFiles->Remove(clear);

            wxArrayString files = Manager::Get()->GetConfigManager(_T("app"))->ReadArrayString(_T("/recent_files"));
            for (int i = (int)files.GetCount() - 1; i >= 0; --i)
            {
				if(wxFileExists(files[i]))
					m_FilesHistory.AddFileToHistory(files[i]);
            }
            m_FilesHistory.UseMenu(recentFiles);
            m_FilesHistory.AddFilesToMenu(recentFiles);
            if (recentFiles->GetMenuItemCount())
                recentFiles->AppendSeparator();
            recentFiles->Append(clear);
        }
        wxMenu* recentProjects = 0;
        clear = menu->FindItem(idFileOpenRecentProjectClearHistory, &recentProjects);
        if (recentProjects)
        {
            recentProjects->Remove(clear);

            wxArrayString files = Manager::Get()->GetConfigManager(_T("app"))->ReadArrayString(_T("/recent_projects"));
            for (int i = (int)files.GetCount() - 1; i >= 0; --i)
            {
            	if(wxFileExists(files[i]))
					m_ProjectsHistory.AddFileToHistory(files[i]);
            }
            m_ProjectsHistory.UseMenu(recentProjects);
            m_ProjectsHistory.AddFilesToMenu(recentProjects);
            if (recentProjects->GetMenuItemCount())
                recentProjects->AppendSeparator();
            recentProjects->Append(clear);
        }
    }
}

void MainFrame::AddToRecentFilesHistory(const wxString& filename)
{
    m_FilesHistory.AddFileToHistory(filename);

    // because we append "clear history" menu to the end of the list,
    // each time we must add a history item we have to:
    // a) remove "Clear history"
    // b) clear the menu
    // c) fill it with the history items
    // and d) append "Clear history"...
    wxMenuBar* mbar = GetMenuBar();
    if (!mbar)
        return;
    int pos = mbar->FindMenu(_("&File"));
    if (pos == wxNOT_FOUND)
        return;
    wxMenu* menu = mbar->GetMenu(pos);
    if (!menu)
        return;
    wxMenu* recentFiles = 0;
    wxMenuItem* clear = menu->FindItem(idFileOpenRecentFileClearHistory, &recentFiles);
    if (clear && recentFiles)
    {
        // a)
        recentFiles->Remove(clear);
        // b)
        m_FilesHistory.RemoveMenu(recentFiles);
        while (recentFiles->GetMenuItemCount())
            recentFiles->Delete(recentFiles->GetMenuItems()[0]);
        // c)
        m_FilesHistory.UseMenu(recentFiles);
        m_FilesHistory.AddFilesToMenu(recentFiles);
        // d)
        if (recentFiles->GetMenuItemCount())
            recentFiles->AppendSeparator();
        recentFiles->Append(clear);
    }
}

void MainFrame::AddToRecentProjectsHistory(const wxString& filename)
{
    m_ProjectsHistory.AddFileToHistory(filename);

    // because we append "clear history" menu to the end of the list,
    // each time we must add a history item we have to:
    // a) remove "Clear history"
    // b) clear the menu
    // c) fill it with the history items
    // and d) append "Clear history"...
    wxMenuBar* mbar = GetMenuBar();
    if (!mbar)
        return;
    int pos = mbar->FindMenu(_("&File"));
    if (pos == wxNOT_FOUND)
        return;
    wxMenu* menu = mbar->GetMenu(pos);
    if (!menu)
        return;
    wxMenu* recentProjects = 0;
    wxMenuItem* clear = menu->FindItem(idFileOpenRecentProjectClearHistory, &recentProjects);
    if (clear && recentProjects)
    {
        // a)
        recentProjects->Remove(clear);
        // b)
        m_ProjectsHistory.RemoveMenu(recentProjects);
        while (recentProjects->GetMenuItemCount())
            recentProjects->Delete(recentProjects->GetMenuItems()[0]);
        // c)
        m_ProjectsHistory.UseMenu(recentProjects);
        m_ProjectsHistory.AddFilesToMenu(recentProjects);
        // d)
        if (recentProjects->GetMenuItemCount())
            recentProjects->AppendSeparator();
        recentProjects->Append(clear);
    }
}

void MainFrame::TerminateRecentFilesHistory()
{
    wxArrayString files;
    for (unsigned int i = 0; i < m_FilesHistory.GetCount(); ++i)
        files.Add(m_FilesHistory.GetHistoryFile(i));
    Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/recent_files"), files);
    files.Clear();
    for (unsigned int i = 0; i < m_ProjectsHistory.GetCount(); ++i)
        files.Add(m_ProjectsHistory.GetHistoryFile(i));
    Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/recent_projects"), files);

    wxMenuBar* mbar = GetMenuBar();
    if (!mbar)
        return;
    int pos = mbar->FindMenu(_("&File"));
    if (pos != wxNOT_FOUND)
    {
        wxMenu* menu = mbar->GetMenu(pos);
        if (!menu)
            return;
        wxMenu* recentFiles = 0;
        menu->FindItem(idFileOpenRecentFileClearHistory, &recentFiles);
        if (recentFiles)
            m_FilesHistory.RemoveMenu(recentFiles);
        wxMenu* recentProjects = 0;
        menu->FindItem(idFileOpenRecentProjectClearHistory, &recentProjects);
        if (recentProjects)
            m_ProjectsHistory.RemoveMenu(recentProjects);
    }
}

// event handlers

void MainFrame::OnPluginsExecuteMenu(wxCommandEvent& event)
{
    wxString pluginName = m_PluginIDsMap[event.GetId()];
    if (!pluginName.IsEmpty())
        Manager::Get()->GetPluginManager()->ExecutePlugin(pluginName);
    else
        Manager::Get()->GetMessageManager()->DebugLog(_("No plugin found for ID %d"), event.GetId());
}

void MainFrame::OnPluginSettingsMenu(wxCommandEvent& event)
{
    wxString pluginName = m_PluginIDsMap[event.GetId()];
    if (!pluginName.IsEmpty())
        Manager::Get()->GetPluginManager()->ConfigurePlugin(pluginName);
    else
        Manager::Get()->GetMessageManager()->DebugLog(_("No plugin found for ID %d"), event.GetId());
}

void MainFrame::OnHelpPluginMenu(wxCommandEvent& event)
{
    wxString pluginName = m_PluginIDsMap[event.GetId()];
    if (!pluginName.IsEmpty())
    {
        const PluginInfo* pi = Manager::Get()->GetPluginManager()->GetPluginInfo(pluginName);
        if (!pi)
        {
            Manager::Get()->GetMessageManager()->DebugLog(_("No plugin info for %s!"), pluginName.c_str());
            return;
        }
        dlgAboutPlugin* dlg = new dlgAboutPlugin(this, pi);
        dlg->ShowModal();
        delete dlg;
    }
    else
        Manager::Get()->GetMessageManager()->DebugLog(_("No plugin found for ID %d"), event.GetId());
}

void MainFrame::OnFileNewEmpty(wxCommandEvent& event)
{
	cbProject* project = Manager::Get()->GetProjectManager()->GetActiveProject();
	if (project)
        wxSetWorkingDirectory(project->GetBasePath());
    cbEditor* ed = Manager::Get()->GetEditorManager()->New();
    if(ed)
    {
        AddToRecentFilesHistory(ed->GetFilename());
    }

	if (!ed || !project)
		return;

	if (wxMessageBox(_("Do you want to add this new file in the active project?"),
					_("Add file to project"),
					wxYES_NO | wxICON_QUESTION) == wxYES)
	{
        wxArrayInt targets;
		if (Manager::Get()->GetProjectManager()->AddFileToProject(ed->GetFilename(), project, targets) != 0)
		{
            ProjectFile* pf = project->GetFileByFilename(ed->GetFilename(), false);
			ed->SetProjectFile(pf);
			Manager::Get()->GetProjectManager()->RebuildTree();
		}
	}
	// verify that the open files are still in sync
	// the new file might have overwritten an existing one)
	Manager::Get()->GetEditorManager()->CheckForExternallyModifiedFiles();
}

bool MainFrame::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& files)
{
    // first check to see if a workspace is passed. If so, only this will be loaded
    wxString foundWorkspace;
    for (unsigned int i = 0; i < files.GetCount(); ++i)
    {
        FileType ft = FileTypeOf(files[i]);
        if (ft == ftCodeBlocksWorkspace || ft == ftMSVCWorkspace || ft == ftMSVSWorkspace)
        {
            foundWorkspace = files[i];
            break;
        }
    }

    if (!foundWorkspace.IsEmpty())
        OpenGeneric(foundWorkspace);
    else
    {
        for (unsigned int i = 0; i < files.GetCount(); ++i)
        {
            OpenGeneric(files[i]);
        }
    }
    return true;
}

void MainFrame::OnFileOpen(wxCommandEvent& event)
{
    wxFileDialog* dlg = new wxFileDialog(this,
                            _("Open file"),
                            wxEmptyString,
                            wxEmptyString,
                            SOURCE_FILES_DIALOG_FILTER,
                            wxOPEN | wxMULTIPLE);
    dlg->SetFilterIndex(SOURCE_FILES_FILTER_INDEX);

    if (dlg->ShowModal() == wxID_OK)
    {
        wxArrayString files;
        dlg->GetPaths(files);
        OnDropFiles(0,0,files);
    }

    delete dlg;
}

void MainFrame::OnFileReopenProject(wxCommandEvent& event)
{
    size_t id = event.GetId() - wxID_FILE10;
    wxString fname = m_ProjectsHistory.GetHistoryFile(id);
    if (!OpenGeneric(fname, true))
    {
        m_ProjectsHistory.RemoveFileFromHistory(id);
        // update start here page
        EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
        if (sh)
            ((StartHerePage*)sh)->Reload();
    }
}

void MainFrame::OnFileOpenRecentProjectClearHistory(wxCommandEvent& event)
{
    while (m_ProjectsHistory.GetCount())
	{
        m_ProjectsHistory.RemoveFileFromHistory(0);
	}
    Manager::Get()->GetConfigManager(_T("app"))->DeleteSubPath(_T("/recent_projects"));

    // update start here page
    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
	if (sh)
        ((StartHerePage*)sh)->Reload();
}

void MainFrame::OnFileReopen(wxCommandEvent& event)
{
    size_t id = event.GetId() - wxID_FILE1;
    wxString fname = m_FilesHistory.GetHistoryFile(id);
    if (!OpenGeneric(fname, true))
    {
        m_FilesHistory.RemoveFileFromHistory(id);
        // update start here page
        EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
        if (sh)
            ((StartHerePage*)sh)->Reload();
    }
}

void MainFrame::OnFileOpenRecentClearHistory(wxCommandEvent& event)
{
    while (m_FilesHistory.GetCount())
	{
        m_FilesHistory.RemoveFileFromHistory(0);
	}
    Manager::Get()->GetConfigManager(_T("app"))->DeleteSubPath(_T("/recent_files"));

    // update start here page
    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
    if (sh)
        ((StartHerePage*)sh)->Reload();
}

void MainFrame::OnFileSave(wxCommandEvent& event)
{
    if (!Manager::Get()->GetEditorManager()->SaveActive())
    {
        wxString msg;
        msg.Printf(_("File %s could not be saved..."), Manager::Get()->GetEditorManager()->GetActiveEditor()->GetFilename().c_str());
        wxMessageBox(msg, _("Error saving file"));
    }
    DoUpdateStatusBar();
}

void MainFrame::OnFileSaveAs(wxCommandEvent& event)
{
    if (!Manager::Get()->GetEditorManager()->SaveActiveAs())
    {
        wxString msg;
        msg.Printf(_("File %s could not be saved..."), Manager::Get()->GetEditorManager()->GetActiveEditor()->GetFilename().c_str());
        wxMessageBox(msg, _("Error saving file"));
    }
    DoUpdateStatusBar();
}

void MainFrame::OnFileSaveAllFiles(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->SaveAll();
    DoUpdateStatusBar();
}

void MainFrame::OnFileSaveWorkspace(wxCommandEvent& event)
{
    if (Manager::Get()->GetProjectManager()->SaveWorkspace())
        AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetWorkspace()->GetFilename());
}

void MainFrame::OnFileSaveWorkspaceAs(wxCommandEvent& event)
{
    if (Manager::Get()->GetProjectManager()->SaveWorkspaceAs(_T("")))
        AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetWorkspace()->GetFilename());
}

void MainFrame::OnFileCloseWorkspace(wxCommandEvent& event)
{
    Manager::Get()->GetProjectManager()->CloseWorkspace();
}

void MainFrame::OnFileClose(wxCommandEvent& WXUNUSED(event))
{
    Manager::Get()->GetEditorManager()->CloseActive();
    DoUpdateStatusBar();
    Refresh();
}

void MainFrame::OnFileCloseAll(wxCommandEvent& WXUNUSED(event))
{
    Manager::Get()->GetEditorManager()->CloseAll();
    DoUpdateStatusBar();
}

void MainFrame::OnFileNext(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->ActivateNext();
    DoUpdateStatusBar();
}

void MainFrame::OnFilePrev(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->ActivatePrevious();
    DoUpdateStatusBar();
}

void MainFrame::OnFilePrint(wxCommandEvent& event)
{
    PrintDialog dlg(this);
    if (dlg.ShowModal() == wxID_OK)
        Manager::Get()->GetEditorManager()->Print(dlg.GetPrintScope(), dlg.GetPrintColorMode());
}

void MainFrame::OnFileRunScript(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* dlg = new wxFileDialog(this,
                            _("Run script"),
                            wxEmptyString,
                            wxEmptyString,
                            _T("Script files (*.script)|*.script"),
                            wxOPEN);
    if (dlg->ShowModal() == wxID_OK)
        Manager::Get()->GetScriptingManager()->LoadScript(dlg->GetPath());
    delete dlg;
}

void MainFrame::OnFileQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MainFrame::OnEraseBackground(wxEraseEvent& event)
{
    // for flicker-free display
    event.Skip();
}

void MainFrame::OnSize(wxSizeEvent& event)
{
    // for flicker-free display
    event.Skip();
}

void MainFrame::OnApplicationClose(wxCloseEvent& event)
{
    Manager::BlockYields(true);

    ProjectManager* prjman = Manager::Get()->GetProjectManager();
    if(prjman)
    {
        cbProject* prj = prjman->GetActiveProject();
        if(prj && prj->GetCurrentlyCompilingTarget())
        {
            event.Veto();
            wxBell();
            Manager::BlockYields(false);
            return;
        }
    }
    if (!ProjectManager::CanShutdown() || !EditorManager::CanShutdown())
    {
        event.Veto();
        wxBell();
        Manager::BlockYields(false);
        return;
    }

    if (!DoCloseCurrentWorkspace())
    {
        event.Veto();
        Manager::BlockYields(false);
        return;
    }

    SaveWindowState();

    m_LayoutManager.DetachPane(Manager::Get()->GetProjectManager()->GetNotebook());
    m_LayoutManager.DetachPane(Manager::Get()->GetMessageManager()->GetNotebook());
    m_LayoutManager.DetachPane(Manager::Get()->GetEditorManager()->GetNotebook());

	m_LayoutManager.UnInit();
    TerminateRecentFilesHistory();

    // Hide the window
    Hide();

    // remove all other event handlers from this window
    // this stops it from crashing, when no plugins are loaded
    while (GetEventHandler() != this)
        PopEventHandler(false);
	Manager::Shutdown(); // Shutdown() is not Free(), Manager is automatically destroyed at exit
    Destroy();
}

void MainFrame::OnEditSwapHeaderSource(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->SwapActiveHeaderSource();
    DoUpdateStatusBar();
}

void MainFrame::OnEditGotoMatchingBrace(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->GotoMatchingBrace();
}

void MainFrame::OnEditBookmarksToggle(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->ToggleBookmark();
}

void MainFrame::OnEditBookmarksNext(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->GotoNextBookmark();
}

void MainFrame::OnEditBookmarksPrevious(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->GotoPreviousBookmark();
}

void MainFrame::OnEditUndo(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->Undo();
}

void MainFrame::OnEditRedo(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->Redo();
}

void MainFrame::OnEditCopy(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->Copy();
}

void MainFrame::OnEditCut(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->Cut();
}

void MainFrame::OnEditPaste(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->Paste();
}

void MainFrame::OnEditSelectAll(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->SelectAll();
}

/* This is a shameless rip-off of the original OnEditCommentSelected function,
 * now more suitingly named OnEditToggleCommentSelected (because that's what
 * it does :)
 */
void MainFrame::OnEditCommentSelected(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
	if( ed )
	{
        ed->GetControl()->BeginUndoAction();
		cbStyledTextCtrl *stc = ed->GetControl();
		if( wxSCI_INVALID_POSITION != stc->GetSelectionStart() )
		{
			int startLine = stc->LineFromPosition( stc->GetSelectionStart() );
			int endLine   = stc->LineFromPosition( stc->GetSelectionEnd() );
			wxString strLine, str;

            /**
                Fix a glitch: when selecting multiple lines and the caret
                is at the start of the line after the last line selected,
                the code would, wrongly, (un)comment that line too.
                This fixes it.
            */
            if (startLine != endLine && // selection is more than one line
                stc->GetColumn( stc->GetSelectionEnd() ) == 0) // and the caret is at the start of the line
            {
                // don't take into account the line the caret is on,
                // because it contains no selection (caret_column == 0)...
                --endLine;
            }

			while( startLine <= endLine )
			{
				// For each line: comment.
				strLine = stc->GetLine( startLine );

                // Comment
                /// @todo This should be language-dependent. We're currently assuming C++
                stc->InsertText( stc->PositionFromLine( startLine ), _T( "//" ) );

				startLine++;
			}
		}
		ed->GetControl()->EndUndoAction();
	}
}

/* See above (OnEditCommentSelected) for details. */
void MainFrame::OnEditUncommentSelected(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
	if( ed )
	{
        ed->GetControl()->BeginUndoAction();
		cbStyledTextCtrl *stc = ed->GetControl();
		if( wxSCI_INVALID_POSITION != stc->GetSelectionStart() )
		{
			int startLine = stc->LineFromPosition( stc->GetSelectionStart() );
			int endLine   = stc->LineFromPosition( stc->GetSelectionEnd() );
			wxString strLine, str;

            /**
                Fix a glitch: when selecting multiple lines and the caret
                is at the start of the line after the last line selected,
                the code would, wrongly, (un)comment that line too.
                This fixes it.
            */
            if (startLine != endLine && // selection is more than one line
                stc->GetColumn( stc->GetSelectionEnd() ) == 0) // and the caret is at the start of the line
            {
                // don't take into account the line the caret is on,
                // because it contains no selection (caret_column == 0)...
                --endLine;
            }

			while( startLine <= endLine )
			{
				// For each line: if it is commented, uncomment.
				strLine = stc->GetLine( startLine );
				int commentPos = strLine.Strip( wxString::leading ).Find( _T( "//" ) );

				if( commentPos ==0 )
				{
					// Uncomment
					strLine.Replace( _T( "//" ), _T( "" ), false );

					// Update
					int start = stc->PositionFromLine( startLine );
					stc->SetTargetStart( start );
					// The +2 is for the '//' we erased
					stc->SetTargetEnd( start + strLine.Length() + 2 );
					stc->ReplaceTarget( strLine );
				}

				startLine++;
			}
		}
		ed->GetControl()->EndUndoAction();
	}
}

void MainFrame::OnEditToggleCommentSelected(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
	if( ed )
	{
        ed->GetControl()->BeginUndoAction();
		cbStyledTextCtrl *stc = ed->GetControl();
		if( wxSCI_INVALID_POSITION != stc->GetSelectionStart() )
		{
			int startLine = stc->LineFromPosition( stc->GetSelectionStart() );
			int endLine   = stc->LineFromPosition( stc->GetSelectionEnd() );
			wxString strLine, str;

            /**
                Fix a glitch: when selecting multiple lines and the caret
                is at the start of the line after the last line selected,
                the code would, wrongly, (un)comment that line too.
                This fixes it.
            */
            if (startLine != endLine && // selection is more than one line
                stc->GetColumn( stc->GetSelectionEnd() ) == 0) // and the caret is at the start of the line
            {
                // don't take into account the line the caret is on,
                // because it contains no selection (caret_column == 0)...
                --endLine;
            }

			while( startLine <= endLine )
			{
				// For each line: If it's commented, uncomment. Otherwise, comment.
				strLine = stc->GetLine( startLine );
				int commentPos = strLine.Strip( wxString::leading ).Find( _T( "//" ) );

				if( -1 == commentPos || commentPos > 0 )
				{
					// Comment
					/// @todo This should be language-dependent. We're currently assuming C++
					stc->InsertText( stc->PositionFromLine( startLine ), _T( "//" ) );
				}
				else
				{
					// Uncomment
					strLine.Replace( _T( "//" ), _T( "" ), false );

					// Update
					int start = stc->PositionFromLine( startLine );
					stc->SetTargetStart( start );
					// The +2 is for the '//' we erased
					stc->SetTargetEnd( start + strLine.Length() + 2 );
					stc->ReplaceTarget( strLine );
				}

				startLine++;
			}
		}
		ed->GetControl()->EndUndoAction();
	}
}

void MainFrame::OnEditAutoComplete(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->AutoComplete();
}

void MainFrame::OnEditHighlightMode(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        EditorColorSet* theme = Manager::Get()->GetEditorManager()->GetColorSet();
        if (theme)
        {
            HighlightLanguage lang = theme->GetHighlightLanguage(_T(""));
            if (event.GetId() != idEditHighlightModeText)
            {
                wxMenu* hl = 0;
                GetMenuBar()->FindItem(idEditHighlightModeText, &hl);
                if (hl)
                {
                    wxMenuItem* item = hl->FindItem(event.GetId());
                    if (item)
                        lang = theme->GetHighlightLanguage(item->GetLabel());
                }
            }
            theme->Apply(lang, ed->GetControl());
        }
    }
}

void MainFrame::OnEditFoldAll(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->FoldAll();
}

void MainFrame::OnEditUnfoldAll(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->UnfoldAll();
}

void MainFrame::OnEditToggleAllFolds(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->ToggleAllFolds();
}

void MainFrame::OnEditFoldBlock(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->FoldBlockFromLine();
}

void MainFrame::OnEditUnfoldBlock(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->UnfoldBlockFromLine();
}

void MainFrame::OnEditToggleFoldBlock(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
		ed->ToggleFoldBlockFromLine();
}

void MainFrame::OnEditEOLMode(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        int mode = -1;

        if (event.GetId() == idEditEOLCRLF)
            mode = wxSCI_EOL_CRLF;
        else if (event.GetId() == idEditEOLCR)
            mode = wxSCI_EOL_CR;
        else if (event.GetId() == idEditEOLLF)
            mode = wxSCI_EOL_LF;

		if (mode != -1 && mode != ed->GetControl()->GetEOLMode())
		{
            ed->GetControl()->BeginUndoAction();
            ed->GetControl()->ConvertEOLs(mode);
            ed->GetControl()->SetEOLMode(mode);
            ed->GetControl()->EndUndoAction();
        }
    }
}

void MainFrame::OnViewLayout(wxCommandEvent& event)
{
    LoadViewLayout(m_PluginIDsMap[event.GetId()]);
}

void MainFrame::OnViewLayoutSave(wxCommandEvent& event)
{
    wxString def = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/default"));
    wxString name = wxGetTextFromUser(_("Enter the name for this layout"), _("Save current layout"), def);
    if (!name.IsEmpty())
    {
        DoFixToolbarsLayout();
        SaveViewLayout(name, m_LayoutManager.SavePerspective(), true);
    }
}

void MainFrame::OnViewLayoutDelete(wxCommandEvent& event)
{
    if (m_LastLayoutName == gDefaultLayout)
    {
        if (wxMessageBox(_("The default layout cannot be deleted. It can always be reverted to "
                        "a predefined state though.\nDo you want to revert it now?"),
                        _("Confirmation"),
                        wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT) == wxYES)
        {
            m_LayoutViews[gDefaultLayout] = gDefaultLayoutData;
            LoadViewLayout(gDefaultLayout);
            return;
        }
    }

    if (wxMessageBox(wxString::Format(_("Are you really sure you want to delete the layout '%s'?"), m_LastLayoutName.c_str()),
                    _("Confirmation"),
                    wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT) == wxYES)
    {
        // first delete it from the hashmap
        LayoutViewsMap::iterator it = m_LayoutViews.find(m_LastLayoutName);
        if (it != m_LayoutViews.end())
            m_LayoutViews.erase(it);

        // now delete the menu item too
        wxMenu* viewLayouts = 0;
        GetMenuBar()->FindItem(idViewLayoutSave, &viewLayouts);
        if (viewLayouts)
        {
            int id = viewLayouts->FindItem(m_LastLayoutName);
            if (id != wxNOT_FOUND)
                viewLayouts->Delete(id);
            // delete the id from the map too
            PluginIDsMap::iterator it2 = m_PluginIDsMap.find(id);
            if (it2 != m_PluginIDsMap.end())
                m_PluginIDsMap.erase(it2);
        }

        wxMessageBox(wxString::Format(_("Layout '%s' deleted.\nWill now revert to layout '%s'..."), m_LastLayoutName.c_str(), gDefaultLayout.c_str()),
                        _("Information"), wxICON_INFORMATION);

        // finally, revert to the default layout
        m_LastLayoutName = gDefaultLayout; // do not ask to save old layout ;)
        LoadViewLayout(gDefaultLayout);
    }
}

void MainFrame::OnSearchFind(wxCommandEvent& event)
{
	Manager::Get()->GetEditorManager()->ShowFindDialog(false, event.GetId() == idSearchFindInFiles);
}

void MainFrame::OnSearchFindNext(wxCommandEvent& event)
{
	if (event.GetId() == idSearchFindPrevious)
		Manager::Get()->GetEditorManager()->FindNext(false);
	else
		Manager::Get()->GetEditorManager()->FindNext(true);
}

void MainFrame::OnSearchReplace(wxCommandEvent& event)
{
	Manager::Get()->GetEditorManager()->ShowFindDialog(true);
}

void MainFrame::OnSearchGotoLine(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
	if (!ed)
		return;

	int max = ed->GetControl()->LineFromPosition(ed->GetControl()->GetLength()) + 1;

	/**
	@remarks We use wxGetText instead of wxGetNumber because wxGetNumber *must*
	provide an initial line number...which doesn't make sense, and just keeps the
	user deleting the initial line number everytime he instantiates the dialog.
	However, this is just a temporary hack, because the default dialog used isn't
	that suitable either.
	*/
    wxString strLine = wxGetTextFromUser( _("Line: "),
                                        _("Goto line"),
                                        _T( "" ),
                                        this );
	long int line = 0;
	strLine.ToLong(&line);
	if ( line > 1 && line <= max )
	{
		ed->UnfoldBlockFromLine(line - 1);
		ed->GotoLine(line - 1);
	}
}

void MainFrame::OnProjectNewEmpty(wxCommandEvent& event)
{
    cbProject* prj = Manager::Get()->GetProjectManager()->NewProject();
	// verify that the open files are still in sync
	// the new file might have overwritten an existing one)
	Manager::Get()->GetEditorManager()->CheckForExternallyModifiedFiles();
	if(prj)
	    AddToRecentProjectsHistory(prj->GetFilename());
}

void MainFrame::OnProjectNew(wxCommandEvent& event)
{
    cbProject* prj = TemplateManager::Get()->NewProject();
	// verify that the open files are still in sync
	// the new file might have overwritten an existing one)
	Manager::Get()->GetEditorManager()->CheckForExternallyModifiedFiles();
	if(prj)
	{
	    prj->Save();
	    prj->SaveAllFiles();
	    AddToRecentProjectsHistory(prj->GetFilename());
	}
}

void MainFrame::OnProjectOpen(wxCommandEvent& event)
{
    wxFileDialog* dlg = new wxFileDialog(this,
                            _("Open project"),
                            wxEmptyString,
                            wxEmptyString,
                            CODEBLOCKS_FILES_FILTER,
                            wxOPEN | wxMULTIPLE);

    if (dlg->ShowModal() == wxID_OK)
    {
        wxArrayString files;
        dlg->GetPaths(files);
        OnDropFiles(0,0,files);
    }

    delete dlg;
}

void MainFrame::OnProjectSaveProject(wxCommandEvent& event)
{
    if (Manager::Get()->GetProjectManager()->SaveActiveProject() ||
        Manager::Get()->GetProjectManager()->SaveActiveProjectAs())
        AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetActiveProject()->GetFilename());
    DoUpdateStatusBar();
}

void MainFrame::OnProjectSaveProjectAs(wxCommandEvent& event)
{
    if (Manager::Get()->GetProjectManager()->SaveActiveProjectAs())
        AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetActiveProject()->GetFilename());
    DoUpdateStatusBar();
}

void MainFrame::OnProjectSaveAllProjects(wxCommandEvent& event)
{
    Manager::Get()->GetProjectManager()->SaveAllProjects();
    DoUpdateStatusBar();
}

void MainFrame::OnProjectSaveTemplate(wxCommandEvent& event)
{
    TemplateManager::Get()->SaveUserTemplate(Manager::Get()->GetProjectManager()->GetActiveProject());
}

void MainFrame::OnProjectCloseProject(wxCommandEvent& event)
{
    // we 're not actually shutting down here, but we want to check if the
    // active project is still opening files (still busy)
    if (!ProjectManager::CanShutdown() || !EditorManager::CanShutdown())
    {
        wxBell();
        return;
    }
    Manager::Get()->GetProjectManager()->CloseActiveProject();
    DoUpdateStatusBar();
}

void MainFrame::OnProjectCloseAllProjects(wxCommandEvent& event)
{
    if (!ProjectManager::CanShutdown() || !EditorManager::CanShutdown())
    {
        wxBell();
        return;
    }
    Manager::Get()->GetProjectManager()->CloseWorkspace();
    DoUpdateStatusBar();
}

void MainFrame::OnProjectImportDevCpp(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import Dev-C++ project"), DEVCPP_FILES_FILTER), false);
}

void MainFrame::OnProjectImportMSVC(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import MS Visual C++ project"), MSVC_FILES_FILTER), false);
}

void MainFrame::OnProjectImportMSVCWksp(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import MS Visual C++ workspace"), MSVC_WORKSPACE_FILES_FILTER), false);
}

void MainFrame::OnProjectImportMSVS(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import MS Visual Studio project"), MSVS_FILES_FILTER), false);
}

void MainFrame::OnProjectImportMSVSWksp(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import MS Visual Studio solution"), MSVS_WORKSPACE_FILES_FILTER), false);
}

void MainFrame::OnHelpAbout(wxCommandEvent& WXUNUSED(event))
{
    dlgAbout* dlg = new dlgAbout(this);
    dlg->ShowModal();
    delete dlg;
}

void MainFrame::OnHelpTips(wxCommandEvent& event)
{
    ShowTips(true);
}

void MainFrame::OnFileMenuUpdateUI(wxUpdateUIEvent& event)
{
    if(Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }
    EditorBase* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetActiveEditor() : 0;
    cbProject* prj = Manager::Get()->GetProjectManager() ? Manager::Get()->GetProjectManager()->GetActiveProject() : 0L;
    wxMenuBar* mbar = GetMenuBar();

    bool canCloseProject = (ProjectManager::CanShutdown() && EditorManager::CanShutdown());
    if(prj && prj->GetCurrentlyCompilingTarget())
        canCloseProject = false;
    mbar->Enable(idProjectCloseProject,canCloseProject);
    mbar->Enable(idFileOpenRecentFileClearHistory, m_FilesHistory.GetCount());
    mbar->Enable(idFileOpenRecentProjectClearHistory, m_ProjectsHistory.GetCount());
    mbar->Enable(idFileClose, ed);
    mbar->Enable(idFileCloseAll, ed);
    mbar->Enable(idFileSave, ed && ed->GetModified());
    mbar->Enable(idFileSaveAs, ed);
    mbar->Enable(idFileSaveAllFiles, ed);
    mbar->Enable(idFileSaveProject, prj && prj->GetModified() && canCloseProject);
    mbar->Enable(idFileSaveProjectAs, prj && canCloseProject);
    mbar->Enable(idFileSaveWorkspace, Manager::Get()->GetProjectManager() && canCloseProject);
    mbar->Enable(idFileSaveWorkspaceAs, Manager::Get()->GetProjectManager() && canCloseProject);
    mbar->Enable(idFileCloseWorkspace, Manager::Get()->GetProjectManager() && canCloseProject);
    mbar->Enable(idFilePrint, Manager::Get()->GetEditorManager() && Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor());

	if (m_pToolbar)
	{
		m_pToolbar->EnableTool(idFileSave, ed && ed->GetModified());
        m_pToolbar->EnableTool(idFilePrint, Manager::Get()->GetEditorManager() && Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor());
	}

	event.Skip();
}

void MainFrame::OnEditMenuUpdateUI(wxUpdateUIEvent& event)
{
    if(Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }

    cbEditor* ed = NULL;
    bool hasSel = false;
    bool canUndo = false;
    bool canRedo = false;
    bool canPaste = false;
    int eolMode = -1;

    if(Manager::Get()->GetEditorManager() && !Manager::isappShuttingDown())
        ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();

    wxMenuBar* mbar = GetMenuBar();

    if(ed)
    {
        eolMode = ed->GetControl()->GetEOLMode();
        canUndo = ed->GetControl()->CanUndo();
        canRedo = ed->GetControl()->CanRedo();
        hasSel = (ed->GetControl()->GetSelectionStart() != ed->GetControl()->GetSelectionEnd());
#ifdef __WXGTK__
        canPaste = true;
#else
        canPaste = ed->GetControl()->CanPaste();
#endif
    }

    mbar->Enable(idEditUndo, ed && canUndo);
    mbar->Enable(idEditRedo, ed && canRedo);
    mbar->Enable(idEditCut, ed && hasSel);
    mbar->Enable(idEditCopy, ed && hasSel);
    mbar->Enable(idEditPaste, ed && canPaste);
    mbar->Enable(idEditSwapHeaderSource, ed);
    mbar->Enable(idEditGotoMatchingBrace, ed);
    mbar->Enable(idEditHighlightMode, ed);
    mbar->Enable(idEditSelectAll, ed);
    mbar->Enable(idEditBookmarks, ed);
    mbar->Enable(idEditFolding, ed);
    mbar->Enable(idEditEOLMode, ed);
	mbar->Check(idEditEOLCRLF, eolMode == wxSCI_EOL_CRLF);
	mbar->Check(idEditEOLCR, eolMode == wxSCI_EOL_CR);
	mbar->Check(idEditEOLLF, eolMode == wxSCI_EOL_LF);
	mbar->Enable(idEditCommentSelected, ed);
	mbar->Enable(idEditAutoComplete, ed);
	mbar->Enable(idEditUncommentSelected, ed);
	mbar->Enable(idEditToggleCommentSelected, ed);

	if (m_pToolbar)
	{
		m_pToolbar->EnableTool(idEditUndo, ed && canUndo);
		m_pToolbar->EnableTool(idEditRedo, ed && canRedo);
		m_pToolbar->EnableTool(idEditCut, ed && hasSel);
		m_pToolbar->EnableTool(idEditCopy, ed && hasSel);
		m_pToolbar->EnableTool(idEditPaste, ed && canPaste);
	}

	event.Skip();
}

void MainFrame::OnViewMenuUpdateUI(wxUpdateUIEvent& event)
{
    if(Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }
    wxMenuBar* mbar = GetMenuBar();
    cbEditor* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor() : 0;
    bool manVis = m_LayoutManager.GetPane(Manager::Get()->GetProjectManager()->GetNotebook()).IsShown();

    mbar->Check(idViewManager, manVis);
    mbar->Check(idViewOpenFilesTree, m_pEdMan && m_pEdMan->IsOpenFilesTreeVisible());
    mbar->Enable(idViewOpenFilesTree, m_pEdMan);
    mbar->Check(idViewMessageManager, m_LayoutManager.GetPane(Manager::Get()->GetMessageManager()->GetNotebook()).IsShown());
    mbar->Check(idViewStatusbar, GetStatusBar() && GetStatusBar()->IsShown());
    mbar->Check(idViewFullScreen, IsFullScreen());
    mbar->Enable(idViewFocusEditor, ed);

    // toolbars
    mbar->Check(idViewToolMain, m_LayoutManager.GetPane(m_pToolbar).IsShown());
    wxMenu* viewToolbars = 0;
    GetMenuBar()->FindItem(idViewToolMain, &viewToolbars);
    if (viewToolbars)
    {
        for (size_t i = 0; i < viewToolbars->GetMenuItemCount(); ++i)
        {
            wxMenuItem* item = viewToolbars->GetMenuItems()[i];
            wxString pluginName = m_PluginIDsMap[item->GetId()];
            if (!pluginName.IsEmpty())
            {
                cbPlugin* plugin = Manager::Get()->GetPluginManager()->FindPluginByName(pluginName);
                if (plugin)
                    item->Check(m_LayoutManager.GetPane(m_PluginsTools[plugin]).IsShown());
            }
        }
    }

	event.Skip();
}

void MainFrame::OnSearchMenuUpdateUI(wxUpdateUIEvent& event)
{
    if(Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }
    cbEditor* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetBuiltinEditor(Manager::Get()->GetEditorManager()->GetActiveEditor()) : 0;
    wxMenuBar* mbar = GetMenuBar();

    // 'Find' is always enabled for find-in-files
    mbar->Enable(idSearchFindNext, ed);
    mbar->Enable(idSearchFindPrevious, ed);
    mbar->Enable(idSearchReplace, ed);
    mbar->Enable(idSearchGotoLine, ed);

	if (m_pToolbar)
	{
		m_pToolbar->EnableTool(idSearchFind, ed);
		m_pToolbar->EnableTool(idSearchReplace, ed);
	}

	event.Skip();
}

void MainFrame::OnProjectMenuUpdateUI(wxUpdateUIEvent& event)
{
    if(Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }
    cbProject* prj = Manager::Get()->GetProjectManager() ? Manager::Get()->GetProjectManager()->GetActiveProject() : 0L;
    wxMenuBar* mbar = GetMenuBar();

    bool canCloseProject = (ProjectManager::CanShutdown() && EditorManager::CanShutdown());
	mbar->Enable(idProjectCloseProject, prj && canCloseProject);
    mbar->Enable(idProjectCloseAllProjects, prj && canCloseProject);
    mbar->Enable(idProjectSaveProject, prj && prj->GetModified() && canCloseProject);
    mbar->Enable(idProjectSaveProjectAs, prj && canCloseProject);
    mbar->Enable(idProjectSaveAllProjects, prj && canCloseProject);
    mbar->Enable(idProjectSaveTemplate, prj && canCloseProject);

	event.Skip();
}

void MainFrame::OnEditorUpdateUI(CodeBlocksEvent& event)
{
    if(Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }
	if (Manager::Get()->GetEditorManager() && event.GetEditor() == Manager::Get()->GetEditorManager()->GetActiveEditor())
	{
		DoUpdateStatusBar();
	}
	event.Skip();
}

void MainFrame::OnEditorUpdateUI_NB(wxNotebookEvent& event)     //tiwag 050917
{                                                               //tiwag 050917
    if (m_pEdMan ) DoUpdateStatusBar();                         //tiwag 050917
    event.Skip();                                               //tiwag 050917
}

void MainFrame::OnToggleOpenFilesTree(wxCommandEvent& event)
{
    if (Manager::Get()->GetEditorManager()->OpenFilesTreeSupported())
        Manager::Get()->GetEditorManager()->ShowOpenFilesTree(!Manager::Get()->GetEditorManager()->IsOpenFilesTreeVisible());
}

void MainFrame::OnToggleBar(wxCommandEvent& event)
{
    wxWindow* win = 0;
    if (event.GetId() == idViewManager)
        win = Manager::Get()->GetProjectManager()->GetNotebook();
    else if (event.GetId() == idViewMessageManager)
        win = Manager::Get()->GetMessageManager()->GetNotebook();
    else if (event.GetId() == idViewToolMain)
        win = m_pToolbar;
    else
    {
        wxString pluginName = m_PluginIDsMap[event.GetId()];
        if (!pluginName.IsEmpty())
        {
            cbPlugin* plugin = Manager::Get()->GetPluginManager()->FindPluginByName(pluginName);
            if (plugin)
                win = m_PluginsTools[plugin];
        }
    }

    if (win)
    {
        m_LayoutManager.GetPane(win).Show(event.IsChecked());
        DoUpdateLayout();
    }
}

void MainFrame::OnToggleStatusBar(wxCommandEvent& event)
{
    wxStatusBar* sb = GetStatusBar();
    if (sb)
    {
        sb->Hide();
        SetStatusBar(NULL);
    }
    else
        DoCreateStatusBar();

// under Windows, the statusbar doesn't disappear immediately...
#ifdef __WXMSW__
    SendSizeEvent(); // make sure everything is laid out properly
	wxSafeYield();
#endif // __WXMSW__
	DoUpdateStatusBar();
}

void MainFrame::OnFocusEditor(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetBuiltinEditor(Manager::Get()->GetEditorManager()->GetActiveEditor()) : 0;
    if (ed)
        ed->GetControl()->SetFocus();
}

void MainFrame::OnToggleFullScreen(wxCommandEvent& event)
{
    ShowFullScreen( !IsFullScreen(), wxFULLSCREEN_NOTOOLBAR// | wxFULLSCREEN_NOSTATUSBAR
                    | wxFULLSCREEN_NOBORDER | wxFULLSCREEN_NOCAPTION );

    // Create fullscreen-close button if we're in fullscreen
    if( IsFullScreen() )
    {
        //
        // Show the button to the bottom-right of the container
        //
        wxSize containerSize = GetClientSize();
        wxSize buttonSize = m_pCloseFullScreenBtn->GetSize();

        // Align
        m_pCloseFullScreenBtn->Move( containerSize.GetWidth() - buttonSize.GetWidth(),
                    containerSize.GetHeight() - buttonSize.GetHeight() );

        m_pCloseFullScreenBtn->Show( true );
        m_pCloseFullScreenBtn->Raise();
    }
    else
    {
        m_pCloseFullScreenBtn->Show( false );
    }
}

void MainFrame::OnPluginLoaded(CodeBlocksEvent& event)
{
    cbPlugin* plug = event.GetPlugin();
    if (plug)
	{
        if (!m_ReconfiguringPlugins)
            DoAddPlugin(plug);
        wxString msg = plug->GetInfo()->title;
        Manager::Get()->GetMessageManager()->DebugLog(_("%s plugin loaded"), msg.c_str());
	}
}

#if 0
void MainFrame::OnPluginUnloaded(CodeBlocksEvent& event)
{
    cbPlugin* plug = event.GetPlugin();
    if (plug)
    {
        if (!m_ReconfiguringPlugins)
        {
            RemovePluginFromMenus(plug->GetInfo()->name);
//            CreateToolbars();
            CreateMenubar();
		}
        wxString msg = plug->GetInfo()->title;
        Manager::Get()->GetMessageManager()->DebugLog(_("%s plugin unloaded"), msg.c_str());
    }
}
#endif

void MainFrame::OnSettingsEnvironment(wxCommandEvent& event)
{
    bool tbarsmall = m_SmallToolBar;
    bool needRestart = false;
    bool edmanCloseBtn = Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/show_close_button"), false);

	EnvironmentSettingsDlg dlg(this, m_LayoutManager.GetArtProvider());
	if (dlg.ShowModal() == wxID_OK)
	{
        DoUpdateEditorStyle();
        DoUpdateLayoutColors();

        m_SmallToolBar = Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/environment/toolbar_size"), true);
        needRestart = m_SmallToolBar != tbarsmall;
        bool autoHide = Manager::Get()->GetConfigManager(_T("message_manager"))->ReadBool(_T("/auto_hide"), false);
        Manager::Get()->GetMessageManager()->EnableAutoHide(autoHide);
//        if (!autoHide)
//            pDockWindow2->Show(true); // make sure it's shown
        ShowHideStartPage();

        if (Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/show_close_button"), false) != edmanCloseBtn)
        {
        	wxMessageBox(_("Some of the changes you made will be applied after you restart Code::Blocks."),
                            _("Information"),
                            wxICON_INFORMATION);
        }
	}
	if (needRestart)
        wxMessageBox(_("Code::Blocks needs to be restarted for the changes to take effect."), _("Information"), wxICON_INFORMATION);
}

void MainFrame::OnGlobalUserVars(wxCommandEvent& event)
{
	Manager::Get()->GetUserVariableManager()->Configure();
}

void MainFrame::OnSettingsEditor(wxCommandEvent& event)
{
	Manager::Get()->GetEditorManager()->Configure();
}

void MainFrame::OnSettingsPlugins(wxCommandEvent& event)
{
    m_ReconfiguringPlugins = true;
	if (Manager::Get()->GetPluginManager()->Configure() == wxID_OK)
	{
        // mandrav: disabled on-the-fly plugins enabling/disabling (still has glitches)
        wxMessageBox(_("Changes will take effect on the next startup."),
                    _("Information"),
                    wxICON_INFORMATION);
//        wxBusyCursor busy;
//        CreateMenubar();
//        CreateToolbars();
	}
    m_ReconfiguringPlugins = false;
}

void MainFrame::OnProjectActivated(CodeBlocksEvent& event)
{
	DoUpdateAppTitle();
	event.Skip();
}

void MainFrame::OnProjectOpened(CodeBlocksEvent& event)
{
    ShowHideStartPage(true);
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnEditorOpened(CodeBlocksEvent& event)
{
//    UpdateKeyBinder(m_KeyProfiles);
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnEditorClosed(CodeBlocksEvent& event)
{
//    UpdateKeyBinder(m_KeyProfiles);
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnEditorSaved(CodeBlocksEvent& event)
{
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnProjectClosed(CodeBlocksEvent& event)
{
    ShowHideStartPage();
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnPageChanged(wxNotebookEvent& event)
{
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnShiftTab(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor(); // Must make sure it's cbEditor and not EditorBase
    if(ed)
        ed->DoUnIndent();
}

void MainFrame::OnRequestDockWindow(CodeBlocksDockEvent& event)
{
    if (Manager::isappShuttingDown())
        return;

    wxPaneInfo info;
    wxString name = event.name;
    if (name.IsEmpty())
    {
        static int idx = 1;
        name = wxString::Format(_T("UntitledPane%d"), idx++);
    }
// TODO (mandrav##): Check for existing pane with the same name
    info = info.Name(name);
    info = info.Caption(event.title.IsEmpty() ? name : event.title);
    switch (event.dockSide)
    {
        case CodeBlocksDockEvent::dsLeft: info = info.Left(); break;
        case CodeBlocksDockEvent::dsRight: info = info.Right(); break;
        case CodeBlocksDockEvent::dsTop: info = info.Top(); break;
        case CodeBlocksDockEvent::dsBottom: info = info.Bottom(); break;
        case CodeBlocksDockEvent::dsFloating: info = info.Float(); break;

        default: break;
    }
    info = info.Show(event.shown);
    info = info.BestSize(event.desiredSize);
    info = info.FloatingSize(event.floatingSize);
    info = info.MinSize(event.minimumSize);
    info = info.Layer(event.stretch ? 1 : 0);
    if (event.row != -1)
        info = info.Row(event.row);
    if (event.column != -1)
        info = info.Position(event.column);
    info = info.CloseButton(event.hideable ? true : false);
    m_LayoutManager.AddPane(event.pWindow, info);
    DoUpdateLayout();
}

void MainFrame::OnRequestUndockWindow(CodeBlocksDockEvent& event)
{
    m_LayoutManager.DetachPane(event.pWindow);
    DoUpdateLayout();
}

void MainFrame::OnRequestShowDockWindow(CodeBlocksDockEvent& event)
{
    m_LayoutManager.GetPane(event.pWindow).Show();
    DoUpdateLayout();
}

void MainFrame::OnRequestHideDockWindow(CodeBlocksDockEvent& event)
{
    m_LayoutManager.GetPane(event.pWindow).Hide();
    DoUpdateLayout();
}

void MainFrame::OnLayoutSwitch(CodeBlocksLayoutEvent& event)
{
    LoadViewLayout(event.layout);
}
