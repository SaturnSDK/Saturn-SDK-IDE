/***************************************************************
 * Name:      wxsmith.cpp
 * Purpose:   Code::Blocks plugin
 * Author:    BYO<byo.spoon@gmail.com>
 * Created:   04/10/05 01:05:08
 * Copyright: (c) BYO
 * License:   GPL
 **************************************************************/

#include "wxsheaders.h"
#include <licenses.h>
#include <manager.h>
#include <tinyxml/tinyxml.h>
#include <messagemanager.h>
#include <cbeditor.h>
#include <projectmanager.h>
#include <wx/notebook.h>
#include <wx/sashwin.h>
#include <configmanager.h>

#include "wxsmith.h"
#include "wxswindoweditor.h"
#include "defwidgets/wxsstdmanager.h"
#include "wxscodegen.h"
#include "wxspropertiesman.h"
#include "wxsproject.h"
#include "wxswidgetfactory.h"
#include "wxsevent.h"
#include "wxsnewwindowdlg.h"
#include "wxsimportxrcdlg.h"
#include "wxsresourcetree.h"
#include "wxssettingsdlg.h"
#include "wxsmithmime.h"
#include "wxsmithwizzard.h"

static int NewDialogId = wxNewId();
static int NewFrameId = wxNewId();
static int NewPanelId = wxNewId();
static int ImportXrcId = wxNewId();
static int ConfigureId = wxNewId();

CB_IMPLEMENT_PLUGINS_3(wxSmith,wxSmithMime,wxSmithWizzard);

wxSmith* wxSmith::Singleton = NULL;

BEGIN_EVENT_TABLE(wxSmith, cbPlugin)
	EVT_PROJECT_CLOSE(wxSmith::OnProjectClose)
	EVT_PROJECT_OPEN(wxSmith::OnProjectOpen)
	EVT_PROJECT_ACTIVATE(wxSmith::OnProjectActivated)
	EVT_SELECT_RES(wxSmith::OnSelectRes)
	EVT_UNSELECT_RES(wxSmith::OnSpreadEvent)
	EVT_SELECT_WIDGET(wxSmith::OnSpreadEvent)
	EVT_UNSELECT_WIDGET(wxSmith::OnSpreadEvent)
	EVT_MENU(NewDialogId,wxSmith::OnNewWindow)
	EVT_MENU(NewFrameId,wxSmith::OnNewWindow)
	EVT_MENU(NewPanelId,wxSmith::OnNewWindow)
	EVT_MENU(ImportXrcId,wxSmith::OnImportXrc)
	EVT_MENU(ConfigureId,wxSmith::OnConfigure)
END_EVENT_TABLE()

wxSmith::wxSmith()
{
	//ctor
	m_PluginInfo.name = _("wxSmith");
	m_PluginInfo.title = _("wxSmith");
	m_PluginInfo.version = _("1.0");
	m_PluginInfo.description = _("RAD tool used to create wxWidgets forms");
	m_PluginInfo.author = _("BYO");
	m_PluginInfo.authorEmail = _("byo.spoon@gmail.com");
	m_PluginInfo.authorWebsite = _("");
	m_PluginInfo.thanksTo =
        _("Ann for Being\n"
          "Anha for Smile\n"
          "Gigi for Faworki\n"
          "\n"
          "God for Love\n"
          "\n"
          "Jaakko Salli for wxPropertyGrid");
	m_PluginInfo.license = LICENSE_GPL;
	m_PluginInfo.hasConfigure = true;

	if ( Singleton == NULL ) Singleton = this;
}

wxSmith::~wxSmith()
{
	if ( Singleton == this ) Singleton = NULL;
}

void wxSmith::OnAttach()
{
    wxNotebook* Notebook = Manager::Get()->GetNotebook();
	if ( Notebook )
	{
        // Creating main splitting objects

        LeftSplitter = new wxsSplitterWindowEx(Notebook);
        Notebook->AddPage(LeftSplitter,_("Resources"));

        wxPanel* ResourcesContainer = new wxPanel(LeftSplitter->GetSplitter(),-1,wxDefaultPosition,wxDefaultSize,0);
        wxPanel* PropertiesContainer = new wxPanel(LeftSplitter->GetSplitter(),-1,wxDefaultPosition,wxDefaultSize,0);

        // Adding resource browser

        wxSizer* Sizer = new wxGridSizer(1);
        ResourceBrowser = new wxsResourceTree(ResourcesContainer);
        ResourceBrowser->Expand(ResourceBrowser->AddRoot(_("Resources")));
        Sizer->Add(ResourceBrowser,1,wxGROW|wxALL);
        ResourcesContainer->SetSizer(Sizer);

        // Adding new page into Manager
        Sizer = new wxGridSizer(1);
        wxNotebook* LDNotebook = new wxNotebook(PropertiesContainer,-1,wxDefaultPosition,wxDefaultSize,wxSUNKEN_BORDER);
        PropertiesPanel = new wxScrolledWindow(LDNotebook);
        PropertiesPanel->SetScrollRate(5,5);
        EventsPanel = new wxScrolledWindow(LDNotebook);
        EventsPanel->SetScrollRate(5,5);
        LDNotebook->AddPage(PropertiesPanel,_("Properties"));
        LDNotebook->AddPage(EventsPanel,_("Events"));
        Sizer->Add(LDNotebook,1,wxGROW);
        PropertiesContainer->SetSizer(Sizer);

        wxsPropertiesMan::Get()->PropertiesPanel = PropertiesPanel;
        wxsPropertiesMan::Get()->EventsPanel = EventsPanel;

        LeftSplitter->Split(ResourcesContainer,PropertiesContainer);

//        MessageManager* Messages = Manager::Get()->GetMessageManager();
        Manager::Get()->Loadxrc(_T("/wxsmith.zip#zip:*"));

        // Initializing standard manager

        wxsStdManager.Initialize();

        if ( ! wxsStdManager.RegisterInFactory() )
        {
            //DebLog("Couldn't register standard widget's factory - this plugin will be useless");
        }
        // TODO (SpOoN#1#): Add other widgets

        /*
        if ( Messages )
        {
            wxString resPath = ConfigManager::GetDataFolder();
            wxString FileName = resPath + _T("/images/wxsmith/wxSmith16x16.png");

            wxBitmap SmithBitmap;
            SmithBitmap.LoadFile(FileName,wxBITMAP_TYPE_PNG);
            int ImageIndex = Messages->GetImageList()->Add(SmithBitmap);

            // Creating widgets palette ad the messages Notebook
            wxWindow* Palette = new wxsPalette((wxWindow*)Messages,Messages->GetPageCount());
            Messages->AddPage(Palette,_("Widgets"),false,ImageIndex);
        }
        */
	}
	else
	{
        LeftSplitter = NULL;
        PropertiesPanel = NULL;
        EventsPanel = NULL;
        ResourceBrowser = NULL;
	}

}

void wxSmith::OnRelease(bool appShutDown)
{
    for ( ProjectMapI i = ProjectMap.begin(); i!=ProjectMap.end(); ++i )
    {
        if ( (*i).second )
        {
            delete (*i).second;
            (*i).second = NULL;
        }
    }

    ProjectMap.clear();
}

int wxSmith::Configure()
{
	wxsSettingsDlg Dlg(NULL);
	Dlg.ShowModal();
	return 0;
}

void wxSmith::BuildMenu(wxMenuBar* menuBar)
{
	wxMenu* Menu = new wxMenu;
	Menu->Append(NewDialogId,_("Add &Dialog"));
	Menu->Append(NewFrameId,_("Add &Frame"));
	Menu->Append(NewPanelId,_("Add &Panel"));
	Menu->AppendSeparator();
	Menu->Append(ImportXrcId,_("Import &XRC file"));
	Menu->AppendSeparator();
	Menu->Append(ConfigureId,_("&Configure wxSmith for current project"));

	int ToolsPos = menuBar->FindMenu(_("&Tools"));

	if  ( ToolsPos == wxNOT_FOUND )
	{
        menuBar->Append(Menu,_("wxSmith"));
	}
	else
	{
        menuBar->Insert(ToolsPos,Menu,_("wxSmith"));
	}
}

void wxSmith::BuildModuleMenu(const ModuleType type, wxMenu* menu, const wxString& arg)
{
}

bool wxSmith::BuildToolBar(wxToolBar* toolBar)
{
	return false;
}

void wxSmith::OnProjectClose(CodeBlocksEvent& event)
{
    cbProject* Proj = event.GetProject();
    ProjectMapI i = ProjectMap.find(Proj);
    if ( i == ProjectMap.end() ) return;

    wxsProject* SmithProj = (*i).second;
    ProjectMap.erase(i);
    if ( SmithProj )
    {
        SmithProj->SaveProject();
        delete SmithProj;
    }

    event.Skip();
}

void wxSmith::OnProjectOpen(CodeBlocksEvent& event)
{
    wxsProject* NewProj = new wxsProject;
    NewProj->BindProject(event.GetProject());
    ProjectMap[event.GetProject()] = NewProj;
    event.Skip();
}

void wxSmith::OnProjectActivated(CodeBlocksEvent& event)
{
    event.Skip();
}

void wxSmith::OnSpreadEvent(wxsEvent& event)
{
    wxsPropertiesMan::Get()->ProcessEvent(event);
    wxsWindowEditor::SpreadEvent(event);
}

void wxSmith::OnSelectRes(wxsEvent& event)
{
    wxsResource* Res = event.GetResource();
    if ( Res )
    {
        Res->EditOpen();
        Res->OnSelect();
    }
    OnSpreadEvent(event);
}

void wxSmith::OnSelectWidget(wxsEvent& event)
{
    wxsResource* Res = event.GetWidget()->GetResource();
    if ( Res )
    {
        Res->EditOpen();
    }
    OnSpreadEvent(event);
}

cbProject* wxSmith::GetCBProject(wxsProject* Proj)
{
    return Proj->GetCBProject();
}

wxsProject* wxSmith::GetSmithProject(cbProject* Proj)
{
    ProjectMapI i = ProjectMap.find(Proj);

    if ( i == ProjectMap.end() ) return NULL;

    return (*i).second;
}

void wxSmith::OnNewWindow(wxCommandEvent& event)
{
    if ( !CheckIntegration() ) return;

    wxString ResType = _T("Dialog");

    if ( event.GetId() == NewDialogId )      ResType = _T("Dialog");
    else if ( event.GetId() == NewFrameId  ) ResType = _T("Frame");
    else if ( event.GetId() == NewPanelId  ) ResType = _T("Panel");
    else
    {
    	wxMessageBox(_("Internal error - invalid resource type"));
    	return;
    }

    wxsNewWindowDlg Dlg(Manager::Get()->GetAppWindow(),ResType);
    Dlg.ShowModal();
}

void wxSmith::OnImportXrc(wxCommandEvent& event)
{
    if ( !CheckIntegration() ) return;

	wxString FileName = ::wxFileSelector(
        _("Select XRC file"),
        _T(""),
        _T(""),
        _T("xrc"),
        _("XRC files (*.xrc)|*.xrc"),
        wxOPEN|wxFILE_MUST_EXIST);

    if ( FileName.empty() ) return;

    // Loading xrc file into xml document

    TiXmlDocument Doc(FileName.mb_str());
    TiXmlElement* Resource;     if (! Doc.LoadFile() ||
        ! (Resource = Doc.FirstChildElement("resource")) )
    {
    	wxMessageBox(_("Couldn't load XRC file."));
        return;
    }     // Generating list of objects
    wxArrayString Resources;     TiXmlElement* Element = Resource->FirstChildElement("object");     while ( Element )
    {
    	const char* Class = Element->Attribute("class");
    	const char* Name = Element->Attribute("name");
    	if ( !Class || !Name ) continue;

    	if ( !strcmp(Class,"wxDialog") ||
    	     !strcmp(Class,"wxPanel") ||
    	     !strcmp(Class,"wxFrame") )
        {
        	Resources.Add(_U(Name));
        }

        Element = Element->NextSiblingElement("object");
    }

    if ( Resources.Count() == 0 )
    {
    	wxMessageBox(_("Didn't find any editable resources"));
    	return;
    }

    // Selecting resource to edit

    wxString Name;

    if ( Resources.Count() == 1 )
    {
    	Name = Resources[0];
    }
    else
    {
        int Index = ::wxGetSingleChoiceIndex(
            _("Select resource to import"),
            _("Importing XRC"),
            Resources );
        if ( Index == -1 ) return;
        Name = Resources[Index];
    }

    Element = Resource->FirstChildElement("object");
    while ( Element )
    {
        if ( !strcmp(Element->Attribute("name"),Name.mb_str()) )
        {
            break;
        }
        Element = Element->NextSiblingElement("object");
    }     if ( !Element ) return;

    // Creating fake resource and testing if xrc can be loaded without any errors
    wxsWidget* Test = wxsGEN(_U(Element->Attribute("class")),NULL);
    if ( !Test )
    {
    	// Something went wrong - default factory is not working ?
    	wxMessageBox(_("Internal error"));
    	return;
    }

    if ( !Test->XmlLoad(Element) )
    {
		if ( wxMessageBox(_("Resource was not loaded properly. Some widgets may be\n"
                            "damaged or will be removed. Continue ?"),
                          _("XRC Load error"),
                          wxYES_NO|wxICON_QUESTION) == wxNO )
        {
        	delete Test;
        	return;
        }
    }
    delete Test;

    // Displaying configuration dialog - it will handle adding resource to project
    wxsImportXrcDlg Dlg(Manager::Get()->GetAppWindow(),Element);
    Dlg.ShowModal();
}

bool wxSmith::CheckIntegration()
{
    cbProject* Project = Manager::Get()->GetProjectManager()->GetActiveProject();

    if ( !Project )
    {
        wxMessageBox(_("Please open project first"),_("Error"),wxOK|wxICON_ERROR);
        return false;
    }

    wxsProject* Proj = GetSmithProject(Project);

    if ( !Proj )
    {
        DebLog(_("Something wrong - couldn't find assciated wxsProject"));
        return false;
    }

    switch ( Proj->GetIntegration() )
    {
        case wxsProject::NotBinded:
            return false;

        case wxsProject::NotWxsProject:
            if ( wxMessageBox(_("Active project doesn't use wxSmith.\nShould I change it ?"),
                              _("Not wxSmith project"),wxYES_NO|wxICON_EXCLAMATION ) == wxYES )
            {
                if ( !Proj->AddSmithConfig() ) return false;
            }
            else
            {
                return false;
            }
            break;

        default:
            break;
    }

    return true;
}

void wxSmith::OnConfigure(wxCommandEvent& event)
{
    cbProject* CP = PRJMAN()->GetActiveProject();
    if ( !CP ) return;
    wxsProject* SP = GetSmithProject(CP);
    if ( !SP ) return;
    SP->Configure();
}
