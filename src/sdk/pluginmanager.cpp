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

#include <wx/string.h>
#include <wx/dir.h>
#include <wx/menu.h>
#include <wx/dynlib.h>

#include "dlgaboutplugin.h"

#include "pluginmanager.h" // class's header file
#include "cbplugin.h"
#include "messagemanager.h"
#include "manager.h"
#include "editormanager.h"
#include "pluginsconfigurationdlg.h"
#include "configmanager.h"
#include "managerproxy.h"

#include <wx/xrc/xmlres.h>

int idSettingsConfigurePlugins = XRCID("idSettingsConfigurePlugins");
int idHelpPlugins = XRCID("idHelpPlugins");

PluginManager* PluginManager::Get()
{
    if(Manager::isappShuttingDown()) // The mother of all sanity checks
        PluginManager::Free();
    else 
    if (!PluginManagerProxy::Get())
	{
        PluginManagerProxy::Set( new PluginManager() );
		Manager::Get()->GetMessageManager()->Log(_("PluginManager initialized"));
	}
    return PluginManagerProxy::Get();
}

void PluginManager::Free()
{
	if (PluginManagerProxy::Get())
	{
		delete PluginManagerProxy::Get();
		PluginManagerProxy::Set( 0L );
	}
}

// class constructor
PluginManager::PluginManager():m_ReconfiguringPlugins(false)
{
    SC_CONSTRUCTOR_BEGIN
	ConfigManager::AddConfiguration(_("Plugin Manager"), "/plugins");
    Manager::Get()->GetAppWindow()->PushEventHandler(this);	
}

// class destructor
PluginManager::~PluginManager()
{
	SC_DESTRUCTOR_BEGIN
	UnloadAllPlugins();
	SC_DESTRUCTOR_END
}

void PluginManager::CreateMenu(wxMenuBar* menuBar)
{
    SANITY_CHECK();
}

void PluginManager::ReleaseMenu(wxMenuBar* menuBar)
{
    SANITY_CHECK();
}

int PluginManager::ScanForPlugins(const wxString& path)
{
    SANITY_CHECK(0);
#ifdef __WXMSW__
	#define PLUGINS_MASK "*.dll"
#else
	#define PLUGINS_MASK "*.so"
#endif
    m_PluginIDsMap.clear();

    int count = 0;
    wxDir dir(path);

    if (!dir.IsOpened())
        return count;

    wxString filename;
    bool ok = dir.GetFirst(&filename, PLUGINS_MASK, wxDIR_FILES);
    while (ok)
    {
		//Manager::Get()->GetMessageManager()->AppendDebugLog(_("Trying %s: "), filename.c_str());
        if (LoadPlugin(path + '/' + filename))
            ++count;
        ok = dir.GetNext(&filename);
    }
    return count;

#undef PLUGINS_MASK
}

cbPlugin* PluginManager::LoadPlugin(const wxString& pluginName)
{
    SANITY_CHECK(0L);
    //wxLogNull zero; // no need for error messages; we check everything ourselves...
    //MessageManager* msgMan = Manager::Get()->GetMessageManager();

    wxDynamicLibrary* lib = new wxDynamicLibrary();
    lib->Load(pluginName);
    if (!lib->IsLoaded())
    {
        //msgMan->DebugLog(_("not loaded (file exists?)"));
        return 0L;
    }

    GetPluginProc proc = (GetPluginProc)lib->GetSymbol("GetPlugin");
    if (!proc)
    {
        lib->Unload();
        //msgMan->DebugLog(_("not a plugin"));
        return 0L;
    }

    cbPlugin* plug = proc();
    wxString plugName = plug->GetInfo()->name;

    // check if we have already loaded a plugin by that name
    if (FindPluginByName(plugName))
    {
        //msgMan->DebugLog(_("another plugin with name \"%s\" is already loaded..."), plugName.c_str());
        lib->Unload();
        return 0L;
    }
    
    PluginElement* plugElem = new PluginElement;
    plugElem->fileName = pluginName;
    plugElem->name = plugName;
    plugElem->library = lib;
	plugElem->plugin = plug;
    m_Plugins.Add(plugElem);

	//msgMan->DebugLog(_("loaded %s"), plugName.c_str());
    return plug;
}

void PluginManager::LoadAllPlugins()
{
    SANITY_CHECK();
    // check if a plugin crashed the app last time
    wxString probPlugin = ConfigManager::Get()->Read("/plugins/try_to_load", wxEmptyString);
    if (!probPlugin.IsEmpty())
    {
        wxString msg;
        msg.Printf(_("Plugin \"%s\" failed to load last time Code::Blocks was executed.\n"
                    "Do you want to disable this plugin from loading?"), probPlugin.c_str());
        if (wxMessageBox(msg, _("Confirmation"), wxICON_QUESTION | wxYES_NO) == wxNO)
            probPlugin = "";
    }

    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;

        // do not load it if the user has explicitly asked not to...
        wxString baseKey;
        baseKey << "/plugins/" << m_Plugins[i]->name;
        bool loadIt = ConfigManager::Get()->Read(baseKey, true);
        
        // if we have a problematic plugin, check if this is it
        if (loadIt && !probPlugin.IsEmpty())
        {
            loadIt = plug->GetInfo()->title != probPlugin;
            // if this is the problematic plugin, don't load it
            if (!loadIt)
                ConfigManager::Get()->Write(baseKey, false);
        }

        if (loadIt && !plug->IsAttached())
		{
            ConfigManager::Get()->Write("/plugins/try_to_load", plug->GetInfo()->title);
			Manager::Get()->GetMessageManager()->AppendLog(_("%s "), m_Plugins[i]->name.c_str());
            plug->Attach();
		}
    }
	Manager::Get()->GetMessageManager()->Log("");

    wxLogNull ln;
    ConfigManager::Get()->DeleteEntry("/plugins/try_to_load");
}

void PluginManager::UnloadAllPlugins()
{
    SANITY_CHECK_ADVANCED(); // don't use "normal" sanity check because
                             // this function is called by the destructor
//    Manager::Get()->GetMessageManager()->DebugLog("Count %d", m_Plugins.GetCount());
	unsigned int i = m_Plugins.GetCount();
    while (i > 0)
    {
		--i;
//		Manager::Get()->GetMessageManager()->DebugLog("At %d", i);
        cbPlugin* plug = m_Plugins[i]->plugin;
		if (!plug)
			continue;
//        Manager::Get()->GetMessageManager()->DebugLog("Doing '%s'", m_Plugins[i]->name.c_str());
        plug->Release(true);
        //it->first->library->Unload();
//        Manager::Get()->GetMessageManager()->DebugLog("Plugin '%s' unloaded", m_Plugins[i]->name.c_str());
        // FIXME: find a way to delete the toolbars too...
    }
}

cbPlugin* PluginManager::FindPluginByName(const wxString& pluginName)
{
    SANITY_CHECK(0L);
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        PluginElement* plugElem = m_Plugins[i];
        if (plugElem->name == pluginName)
            return m_Plugins[i]->plugin;
    }

    return NULL;
}

cbPlugin* PluginManager::FindPluginByFileName(const wxString& pluginFileName)
{
    SANITY_CHECK(0L);
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        PluginElement* plugElem = m_Plugins[i];
        if (plugElem->fileName == pluginFileName)
            return m_Plugins[i]->plugin;
    }

    return NULL;
}

const PluginInfo* PluginManager::GetPluginInfo(const wxString& pluginName)
{
    SANITY_CHECK(0L);
    cbPlugin* plug = FindPluginByName(pluginName);
    if (plug)
        return plug->GetInfo();
        
    return NULL;
}

int PluginManager::ExecutePlugin(const wxString& pluginName)
{
    SANITY_CHECK(0);
    cbPlugin* plug = FindPluginByName(pluginName);
    if (plug)
    {
        if (plug->GetType() != ptTool)
        {
            MessageManager* msgMan = Manager::Get()->GetMessageManager();
            msgMan->DebugLog(_("Plugin %s is not a tool to have Execute() method!"), plug->GetInfo()->name.c_str());
        }
        else
            return ((cbToolPlugin*)plug)->Execute();
    }
	
	return 0;
}

int PluginManager::ConfigurePlugin(const wxString& pluginName)
{
    SANITY_CHECK(0);
    cbPlugin* plug = FindPluginByName(pluginName);
    if (plug)
        return plug->Configure();
	return 0;
}

PluginsArray PluginManager::GetToolOffers()
{
    return DoGetOffersFor(ptTool);
}

PluginsArray PluginManager::GetMimeOffers()
{
    return DoGetOffersFor(ptMime);
}

PluginsArray PluginManager::GetCompilerOffers()
{
    return DoGetOffersFor(ptCompiler);
}

PluginsArray PluginManager::GetCodeCompletionOffers()
{
	return DoGetOffersFor(ptCodeCompletion);
}

PluginsArray PluginManager::DoGetOffersFor(PluginType type)
{
    PluginsArray arr;
    SANITY_CHECK(arr);

    // special case for MIME plugins
    // we 'll add the default MIME handler, last in the returned array
    cbPlugin* dflt = 0;

    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;
        if (plug->GetType() == type)
        {
            if (type == ptMime)
            {
                // default MIME handler?
                if (((cbMimePlugin*)plug)->HandlesEverything())
                    dflt = plug;
                else
                    arr.Add(plug);
            }
            else
                arr.Add(plug);
        }
    }

    // add default MIME handler last
    if (dflt)
        arr.Add(dflt);

    return arr;
}

void PluginManager::AskPluginsForModuleMenu(const ModuleType type, wxMenu* menu, const wxString& arg)
{
    SANITY_CHECK();
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;
        if (plug)
            plug->BuildModuleMenu(type, menu, arg);
    }
}

void PluginManager::NotifyPlugins(CodeBlocksEvent& event)
{
    SANITY_CHECK();
    /*
    A plugin might process the event we 'll send it or not. The event then
    "travels" up the chain of plugins. If one plugin (say in the middle) is
    disabled, it's not processing events and so the event "travelling" stops.
    The rest of the plugins never "hear" about this event.
    The solution is that the plugin manager checks for disabled plugins and
    posts the event not only to the last plugin but also to all plugins that
    are followed by a disabled plugin (skipping the chain-breakers that is)...
    */
    if (!m_Plugins.GetCount())
		return;

    bool sendEvt = true;
    for (unsigned int i = m_Plugins.GetCount() - 1; i > 0; --i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;
        if (!plug || (plug && !plug->IsAttached()))
            sendEvt = true;
        else if (sendEvt)
        {
            wxPostEvent(plug, event);
            sendEvt = false;
        }
    }
}

int PluginManager::Configure()
{
    SANITY_CHECK(wxID_CANCEL);
    PluginsConfigurationDlg dlg(Manager::Get()->GetAppWindow());
    if (dlg.ShowModal() == wxID_CANCEL)
        return wxID_CANCEL;
    return wxID_OK;
}

void PluginManager::OnPluginLoaded(CodeBlocksEvent& event,
    wxMenuBar *mb,wxToolBar *tb)
{
    SANITY_CHECK();
    cbPlugin* plug = event.GetPlugin();
    if (plug)
	{
        if (!m_ReconfiguringPlugins)
            DoAddPlugin(plug,mb,tb);
        wxString msg = plug->GetInfo()->title;
        Manager::Get()->GetMessageManager()->DebugLog(_("%s plugin loaded"), msg.c_str());
	}
}

void PluginManager::LoadPluginsToolBars(wxToolBar *toolBar)
{
    SANITY_CHECK();
	for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
	{
		cbPlugin* plug = m_Plugins[i]->plugin;
		if (plug && plug->IsAttached())
		{
			if (plug->GetType() != ptTool)
				BuildPluginToolBar(plug,toolBar);
		}
	}
}

void PluginManager::LoadPluginsMenus(wxMenuBar *menuBar)
{
    SANITY_CHECK();
	for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
	{
		cbPlugin* plug = m_Plugins[i]->plugin;
		if (plug && plug->IsAttached())
		{
			if (plug->GetType() != ptTool)
				BuildPluginMenu(plug,menuBar);
		}
	}
}

// ****************** Toolbar and Menubar plugin loading *******************
// Loads each plugin's toolbar and menu items
// Currently it calls each plugin's functions, but later it will be changed
// *************************************************************************

void PluginManager::BuildPluginToolBar(cbPlugin *plug,wxToolBar *toolBar)
{
    if(!plug || !toolBar)
        return;
    plug->BuildToolBar(toolBar); // to be replaced in future versions
}

void PluginManager::BuildPluginMenu(cbPlugin *plug,wxMenuBar *menuBar)
{
    if(!plug || !menuBar)
        return;
    plug->BuildMenu(menuBar); // to be replaced in future versions
}

// ******************* FUNCTIONS MOVED FROM MAIN.CPP ************************
// These functions administrate the plugins' menus and toolbars
// Since all they need is a pointer to a menubar to work, these don't belong
// in main.cpp
// **************************************************************************

void PluginManager::BuildAllPluginsMenus(wxMenuBar *mbar)
{
    SANITY_CHECK();
    if(!mbar) 
        return;

    int tmpidx;
    wxMenuItem *tmpitem=0L;
    wxMenu *plugs=0L,*settingsPlugins=0L,*pluginsM=0L;
    
    tmpidx=mbar->FindMenu("P&lugins");
    if(tmpidx!=wxNOT_FOUND)
        plugs = mbar->GetMenu(tmpidx);
        
    if(tmpitem = mbar->FindItem(idSettingsConfigurePlugins,NULL))
        settingsPlugins = tmpitem->GetSubMenu();
    if(tmpitem = mbar->FindItem(idHelpPlugins,NULL))
        pluginsM = tmpitem->GetSubMenu();
    
	m_PluginsMenu = plugs ? plugs : new wxMenu();
	m_SettingsMenu = settingsPlugins ? settingsPlugins : new wxMenu();
	m_HelpPluginsMenu = pluginsM ? pluginsM : new wxMenu();

	for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
	{
		cbPlugin* plug = m_Plugins[i]->plugin;
		if (plug && plug->IsAttached())
		{
            AddPluginInSettingsMenu(plug);
            AddPluginInHelpPluginsMenu(plug);
			if (plug->GetType() == ptTool)
                AddPluginInPluginsMenu(plug);
		}
	}
    LoadPluginsMenus(mbar);
}

// MOVED (pluginmanager.cpp) :
void PluginManager::DoAddPlugin(cbPlugin* plugin,wxMenuBar *mbar,wxToolBar *tbar)
{
    SANITY_CHECK();

    //m_pMsgMan->DebugLog(_("Adding plugin: %s"), plugin->GetInfo()->name.c_str());
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
        if(mbar)
            BuildPluginMenu(plugin,mbar);
        // toolbar
        if(tbar)
            BuildPluginToolBar(plugin,tbar);
    }
}

// MOVED (from main.cpp) :
void PluginManager::AddPluginInMenus(wxMenu* menu, cbPlugin* plugin, wxObjectEventFunction callback, int pos)
{
    SANITY_CHECK();
    if (!plugin || !menu)
		return;

    PluginIDsMap::iterator it;
    for (it = m_PluginIDsMap.begin(); it != m_PluginIDsMap.end(); ++it)
    {
        if (it->second == plugin->GetInfo()->name)
        {
            if (menu->FindItem(it->first) != 0)
                return;
        }
    }

    int id = wxNewId();
    m_PluginIDsMap[id] = plugin->GetInfo()->name;
    if (pos == -1)
        menu->Append(id, plugin->GetInfo()->title);
    else
        menu->Insert(pos, id, plugin->GetInfo()->title);

    if(callback)
        Connect( id,  wxEVT_COMMAND_MENU_SELECTED, callback );
}

// MOVED (from main.cpp) :
void PluginManager::AddPluginInPluginsMenu(cbPlugin* plugin)
{
    SANITY_CHECK();
    if(!plugin || !m_PluginsMenu) 
        return;
    wxObjectEventFunction PluginsMenuCallback;
    PluginsMenuCallback=(wxObjectEventFunction)&PluginManager::OnPluginsExecuteMenu;
        
    // "Plugins" menu is special case because it contains "Manage plugins",
    // which must stay at the end of the menu
    // So we insert entries, not append...
    
    // this will insert a separator when the first plugin is added in the "Plugins" menu
    if (m_PluginsMenu->GetMenuItemCount() == 1)
        m_PluginsMenu->Insert(0, wxID_ANY, "");

    AddPluginInMenus(m_PluginsMenu, plugin, PluginsMenuCallback, m_PluginsMenu->GetMenuItemCount() - 2);
}

// MOVED (from main.cpp) :
void PluginManager::AddPluginInSettingsMenu(cbPlugin* plugin)
{
    SANITY_CHECK();
    if(!plugin || !m_SettingsMenu) 
        return;
    if (!plugin->GetInfo()->hasConfigure)
        return;
    wxObjectEventFunction SettingsMenuCallback;
    SettingsMenuCallback=(wxObjectEventFunction)&PluginManager::OnPluginSettingsMenu;
        
    AddPluginInMenus(m_SettingsMenu, plugin, SettingsMenuCallback);
}

// MOVED (from main.cpp) :
void PluginManager::AddPluginInHelpPluginsMenu(cbPlugin* plugin)
{
    SANITY_CHECK();
    if(!plugin || !m_HelpPluginsMenu)
        return;
    wxObjectEventFunction HelpMenuCallback;
    HelpMenuCallback=(wxObjectEventFunction)&PluginManager::OnHelpPluginMenu;
    AddPluginInMenus(m_HelpPluginsMenu, plugin, HelpMenuCallback);
}

// **************************************************************************

// event handlers

// MOVED (from main.cpp)
void PluginManager::OnPluginsExecuteMenu(wxCommandEvent& event)
{
    SANITY_CHECK();
    wxString pluginName = m_PluginIDsMap[event.GetId()];
    if (!pluginName.IsEmpty())
        ExecutePlugin(pluginName);
    else
        Manager::Get()->GetMessageManager()->DebugLog(_("No plugin found for ID %d"), event.GetId());
}

void PluginManager::OnPluginSettingsMenu(wxCommandEvent& event)
{
    SANITY_CHECK();
    wxString pluginName = m_PluginIDsMap[event.GetId()];
    if (!pluginName.IsEmpty())
        ConfigurePlugin(pluginName);
    else
        Manager::Get()->GetMessageManager()->DebugLog(_("No plugin found for ID %d"), event.GetId());
}

void PluginManager::OnHelpPluginMenu(wxCommandEvent& event)
{
    SANITY_CHECK();
    wxString pluginName = m_PluginIDsMap[event.GetId()];
    if (!pluginName.IsEmpty())
    {
        const PluginInfo* pi = GetPluginInfo(pluginName);
        if (!pi)
        {
            Manager::Get()->GetMessageManager()->DebugLog(_("No plugin info for %s!"), pluginName.c_str());
            return;
        }
        dlgAboutPlugin* dlg = new dlgAboutPlugin(Manager::Get()->GetAppWindow(), pi);
        dlg->ShowModal();
        delete dlg;
    }
    else
        Manager::Get()->GetMessageManager()->DebugLog(_("No plugin found for ID %d"), event.GetId());
}

bool PluginManager::ReconfigurePlugins(bool flag)
{
    SANITY_CHECK(false);
    bool oldflag=m_ReconfiguringPlugins;
    m_ReconfiguringPlugins=flag;
    return oldflag;
}

bool PluginManager::isReconfiguringPlugins()
{
    SANITY_CHECK(false);
    return m_ReconfiguringPlugins;
}

void PluginManager::DoConfigDialog()
{
    SANITY_CHECK();
    // bool oldflag=ReconfigurePlugins(true);
	if (Configure() == wxID_OK)
	{
        wxMessageBox(_("Changes will take effect on the next startup."),
                    _("Information"),
                    wxICON_INFORMATION);
	}
    // ReconfigurePlugins(oldflag);
}
