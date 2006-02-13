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
    #include <wx/string.h>
    #include <wx/menu.h>
    #include <wx/intl.h>

    #include "pluginmanager.h"
    #include "cbexception.h"
    #include "cbplugin.h"
    #include "messagemanager.h"
    #include "manager.h"
    #include "editormanager.h"
    #include "configmanager.h"
    #include "personalitymanager.h"
    #include "globals.h"
    #include <wx/dir.h>
#endif

#include <wx/dynlib.h>

#include "annoyingdialog.h"
#include "pluginsconfigurationdlg.h"

// class constructor
PluginManager::PluginManager()
{
    SC_CONSTRUCTOR_BEGIN
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
	#define PLUGINS_MASK _T("*.dll")
#else
	#define PLUGINS_MASK _T("*.so")
#endif

    int count = 0;
    wxDir dir(path);

    if (!dir.IsOpened())
        return count;

    wxString filename;
	wxString failed;
    bool ok = dir.GetFirst(&filename, PLUGINS_MASK, wxDIR_FILES);
    while (ok)
    {
//		Manager::Get()->GetMessageManager()->AppendDebugLog(_("Trying %s: "), filename.c_str());
        if (LoadPlugin(path + _T('/') + filename))
            ++count;
		else
			failed << filename << _T(" ");
        ok = dir.GetNext(&filename);
    }
    Manager::Get()->GetMessageManager()->Log(_("Found %d plugins"), count);
	if (!failed.IsEmpty())
		Manager::Get()->GetMessageManager()->Log(_("Plugins that failed to load: %s"), failed.c_str());
    return count;

#undef PLUGINS_MASK
}

bool PluginManager::LoadPlugin(const wxString& pluginName)
{
    SANITY_CHECK(false);
    wxLogNull zero; // no need for error messages; we check everything ourselves...
    MessageManager* msgMan = Manager::Get()->GetMessageManager();

    wxDynamicLibrary* lib = new wxDynamicLibrary();
    lib->Load(pluginName);
    if (!lib->IsLoaded())
    {
        msgMan->DebugLog(_("%s: not loaded (file exists?)"), pluginName.c_str());
        delete lib;
        return false;
    }

    // first, see if we have a name function
    PluginNameProc nameproc = (PluginNameProc)lib->GetSymbol(_T("PluginName"));
    if (!nameproc)
    {
        msgMan->DebugLog(_("%s: not a plugin (no PluginName)"), pluginName.c_str());
        delete lib;
        return false;
    }

    // now get the SDK version entry points
    int major;
    int minor;
    int release;
    PluginSDKVersionProc versionproc = (PluginSDKVersionProc)lib->GetSymbol(_T("PluginSDKVersion"));
    // if no SDK version entry point, abort
    if (!versionproc)
    {
        msgMan->DebugLog(_("%s: no SDK version entry point"), pluginName.c_str());
        delete lib;
        return false;
    }

	// check if it is the correct SDK version
	versionproc(&major, &minor, &release);
	if (major != PLUGIN_SDK_VERSION_MAJOR ||
		minor != PLUGIN_SDK_VERSION_MINOR ||
		release != PLUGIN_SDK_VERSION_RELEASE)
	{
		// in this case, inform the user...
		wxString fmt;
		fmt.Printf(_("A plugin failed to load because it was built "
                    "with a different Code::Blocks SDK version.\n\n"
					"Plugin: %s\n"
					"Plugin's SDK version: %d.%d.%d\n"
					"Your SDK version: %d.%d.%d"),
					pluginName.c_str(),
					major,
					minor,
					release,
					PLUGIN_SDK_VERSION_MAJOR,
					PLUGIN_SDK_VERSION_MINOR,
					PLUGIN_SDK_VERSION_RELEASE);
        AnnoyingDialog dlg(_("Plugin loading error"),
                            fmt,
                            wxART_WARNING,
                            AnnoyingDialog::OK,
                            wxID_OK);
        dlg.ShowModal();
        lib->Unload();
        delete lib;
        return false;
	}

    // get the plugin's count inside this library
    GetPluginsCountProc countproc = (GetPluginsCountProc)lib->GetSymbol(_T("GetPluginsCount"));
    // and the factory function too
    CreatePluginProc factory = (CreatePluginProc)lib->GetSymbol(_T("CreatePlugin"));
    if (!countproc || !factory)
    {
        lib->Unload();
        delete lib;
        msgMan->DebugLog(_("%s: not a plugin"), pluginName.c_str());
        return false;
    }

    // loop for all plugins contained in this library
    size_t pluginsCount = countproc();
    int libUseCount = 0;
    for (size_t i = 0; i < pluginsCount; ++i)
    {
        // first check if it is allowed to load
        if (!Manager::Get()->GetConfigManager(_T("plugins"))->ReadBool(_T("/") + nameproc(i), true))
        {
            // we 'll keep the record, so that it is shown in "Plugins->Manage plugins..."
            PluginElement* plugElem = new PluginElement;
            plugElem->fileName = pluginName;
            plugElem->name = nameproc(i);
            plugElem->library = lib;
            plugElem->freeProc = 0;
            plugElem->plugin = 0;
            m_Plugins.Add(plugElem);
            continue; // do not load it at all
        }

        // try to load the plugin
        cbPlugin* plug = 0L;
        try
        {
            plug = factory(i);
        }
        catch (cbException& exception)
        {
            exception.ShowErrorMessage(false);
            continue;
        }

        // check if we have already loaded a plugin by that name
        wxString plugName = plug->GetInfo()->name;
        if (FindPluginByName(plugName))
        {
            msgMan->DebugLog(_("%s: another plugin with name \"%s\" is already loaded..."), pluginName.c_str(), plugName.c_str());
            delete plug;
            continue;
        }

        // all done; add it to our list
        PluginElement* plugElem = new PluginElement;
        plugElem->fileName = pluginName;
        plugElem->name = plugName;
        plugElem->library = lib;
        plugElem->freeProc = (FreePluginProc)lib->GetSymbol(_T("FreePlugin"));
        plugElem->plugin = plug;
        m_Plugins.Add(plugElem);

        SetupLocaleDomain(plugName);

        ++libUseCount;

        msgMan->DebugLog(_("%s: loaded"), plugName.c_str());
    }

    if (libUseCount == 0)
    {
        // not even one plugin was loaded
        // free this library
        lib->Unload();
        delete lib;
        return true; // but it's not an error ;)
    }

    return true;
}

void PluginManager::LoadAllPlugins()
{
    SANITY_CHECK();

    // check if a plugin crashed the app last time
    wxString probPlugin = Manager::Get()->GetConfigManager(_T("plugins"))->Read(_T("/try_to_activate"), wxEmptyString);
    if (!probPlugin.IsEmpty())
    {
        wxString msg;
        msg.Printf(_("Plugin \"%s\" failed to load last time Code::Blocks was executed.\n"
                    "Do you want to disable this plugin from loading?"), probPlugin.c_str());
        if (cbMessageBox(msg, _("Warning"), wxICON_WARNING | wxYES_NO) == wxNO)
            probPlugin = _T("");
    }

    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;
        if (!plug)
            continue;

        // do not load it if the user has explicitly asked not to...
        wxString baseKey;
        baseKey << _T("/") << m_Plugins[i]->name;
        bool loadIt = Manager::Get()->GetConfigManager(_T("plugins"))->ReadBool(baseKey, true);

        // if we have a problematic plugin, check if this is it
        if (loadIt && !probPlugin.IsEmpty())
        {
            loadIt = plug->GetInfo()->title != probPlugin;
            // if this is the problematic plugin, don't load it
            if (!loadIt)
                Manager::Get()->GetConfigManager(_T("plugins"))->Write(baseKey, false);
        }

        if (loadIt && !plug->IsAttached())
		{
            Manager::Get()->GetConfigManager(_T("plugins"))->Write(_T("/try_to_activate"), plug->GetInfo()->title);
			Manager::Get()->GetMessageManager()->AppendLog(_("%s "), m_Plugins[i]->name.c_str());
            try
            {
                plug->Attach();
                Manager::Get()->GetConfigManager(_T("plugins"))->Write(_T("/try_to_activate"), wxEmptyString, false);
            }
            catch (cbException& exception)
            {
                Manager::Get()->GetMessageManager()->AppendLog(_T("[failed] "));
                exception.ShowErrorMessage(false);

                wxString msg;
                msg.Printf(_("Plugin \"%s\" failed to load...\n"
                            "Do you want to disable this plugin from loading next time?"), plug->GetInfo()->title.c_str());
                if (cbMessageBox(msg, _("Warning"), wxICON_WARNING | wxYES_NO) == wxYES)
                    Manager::Get()->GetConfigManager(_T("plugins"))->Write(baseKey, false);
            }
		}
    }
	Manager::Get()->GetMessageManager()->Log(_T(""));

    wxLogNull ln;
    Manager::Get()->GetConfigManager(_T("plugins"))->Write(_T("/try_to_activate"), wxEmptyString, false);
}

void PluginManager::UnloadAllPlugins()
{
    SANITY_CHECK_ADVANCED(); // don't use "normal" sanity check because
                             // this function is called by the destructor
//    Manager::Get()->GetMessageManager()->DebugLog("Count %d", m_Plugins.GetCount());

    // first loop to release all plugins
	unsigned int i = m_Plugins.GetCount();
    while (i > 0)
    {
		--i;
//		Manager::Get()->GetMessageManager()->DebugLog(_("At %d"), i);
        cbPlugin* plug = m_Plugins[i]->plugin;
		if (!plug)
			continue;
//        Manager::Get()->GetMessageManager()->DebugLog(_("Doing '%s'"), m_Plugins[i]->name.c_str());
        plug->Release(true);
        //it->first->library->Unload();
//        Manager::Get()->GetMessageManager()->DebugLog(_("Plugin '%s' unloaded"), m_Plugins[i]->name.c_str());
        // FIXME: find a way to delete the toolbars too...
    }

    // second loop to delete all plugins from memory
	i = m_Plugins.GetCount();
    while (i > 0)
    {
		--i;
        cbPlugin* plug = m_Plugins[i]->plugin;
        if (m_Plugins[i]->freeProc)
            m_Plugins[i]->freeProc(plug);
        else
            delete plug; // try to delete it ourselves...
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
        {
            try
            {
                return ((cbToolPlugin*)plug)->Execute();
            }
            catch (cbException& exception)
            {
                exception.ShowErrorMessage(false);
            }
        }
    }

	return 0;
}

int PluginManager::ConfigurePlugin(const wxString& pluginName)
{
    SANITY_CHECK(0);
    cbPlugin* plug = FindPluginByName(pluginName);
    if (plug)
    {
        try
        {
            return plug->Configure();
        }
        catch (cbException& exception)
        {
            exception.ShowErrorMessage(false);
        }
    }
	return 0;
}

int SortByConfigurationPriority(cbPlugin** first, cbPlugin** second)
{
    return (*first)->GetConfigurationPriority() - (*second)->GetConfigurationPriority();
}

void PluginManager::GetConfigurationPanels(int group, wxWindow* parent, ConfigurationPanelsArray& arrayToFill)
{
    // build an array of Plugins* because we need to order it by configuration priority
    PluginsArray arr;
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;
        // all check are done here
        if (plug && plug->IsAttached() && (plug->GetConfigurationGroup() & group))
            arr.Add(plug);
    }

    // sort the array
    arr.Sort(SortByConfigurationPriority);

    // now enumerate the array and fill the supplied configurations panel array
    arrayToFill.Clear();
    for (unsigned int i = 0; i < arr.GetCount(); ++i)
    {
        cbPlugin* plug = arr[i];
        cbConfigurationPanel* pnl = plug->GetConfigurationPanel(parent);
        if (pnl)
            arrayToFill.Add(pnl);
    }
}

PluginsArray PluginManager::GetToolOffers()
{
    return GetOffersFor(ptTool);
}

PluginsArray PluginManager::GetMimeOffers()
{
    return GetOffersFor(ptMime);
}

PluginsArray PluginManager::GetCompilerOffers()
{
    return GetOffersFor(ptCompiler);
}

PluginsArray PluginManager::GetDebuggerOffers()
{
    return GetOffersFor(ptDebugger);
}

PluginsArray PluginManager::GetCodeCompletionOffers()
{
	return GetOffersFor(ptCodeCompletion);
}

PluginsArray PluginManager::GetOffersFor(PluginType type)
{
    PluginsArray arr;
    SANITY_CHECK(arr);

    // special case for MIME plugins
    // we 'll add the default MIME handler, last in the returned array
    cbPlugin* dflt = 0;

    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;
        if (plug && plug->IsAttached() && plug->GetType() == type)
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

void PluginManager::AskPluginsForModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    SANITY_CHECK();
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;
        if (plug)
        {
            try
            {
                plug->BuildModuleMenu(type, menu, data);
            }
            catch (cbException& exception)
            {
                exception.ShowErrorMessage(false);
            }
        }
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
            if (plug)
                plug->ProcessEvent(event); // Plugins require immediate attention
            if (!event.GetSkipped())
                break; // if someone handled it, abort the rest
            // wxPostEvent(plug, event);
            sendEvt = false;
        }
    }
}

cbMimePlugin* PluginManager::GetMIMEHandlerForFile(const wxString& filename)
{
    PluginsArray mimes = GetMimeOffers();
    for (unsigned int i = 0; i < mimes.GetCount(); ++i)
    {
        cbMimePlugin* plugin = (cbMimePlugin*)mimes[i];
        if (plugin && plugin->CanHandleFile(filename))
            return plugin;
    }
    return 0;
}

int PluginManager::Configure()
{
    SANITY_CHECK(wxID_CANCEL);
    PluginsConfigurationDlg dlg(Manager::Get()->GetAppWindow());
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_CANCEL)
        return wxID_CANCEL;

    // mandrav: disabled on-the-fly plugins enabling/disabling (still has glitches)
/*
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;

        // do not load it if the user has explicitely asked not to...
        wxString baseKey;
        baseKey << _T("/plugins/") << m_Plugins[i]->name;
        bool loadIt = OldConfigManager::Get()->Read(baseKey, true);

        if (!loadIt && plug->IsAttached())
            plug->Release(false);
        else if (loadIt && !plug->IsAttached())
        {
            OldConfigManager::Get()->Write(_("/plugins/try_to_activate"), plug->GetInfo()->title);
            plug->Attach();
        }
    }
    wxLogNull ln;
    OldConfigManager::Get()->DeleteEntry(_T("/plugins/try_to_activate"));
*/
    return wxID_OK;
}

void PluginManager::SetupLocaleDomain(const wxString& DomainName)
{
    int catalogNum=Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/locale/catalogNum"),(int)0);
    int i = 1;
    for (; i <= catalogNum; ++i)
    {
        wxString catalogName=Manager::Get()->GetConfigManager(_T("app"))->Read(wxString::Format(_T("/locale/Domain%d"), i), wxEmptyString);
        if (catalogName.Cmp(DomainName) == 0)
            break;
    }
    if (i > catalogNum)
    {
        ++catalogNum;
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/locale/catalogNum"), (int)catalogNum);
        Manager::Get()->GetConfigManager(_T("app"))->Write(wxString::Format(_T("/locale/Domain%d"), i), DomainName);
    }
}
