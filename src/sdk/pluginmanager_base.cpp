/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6521 $
 * $Id: pluginmanager.cpp 6521 2010-08-23 18:22:34Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/pluginmanager.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/dir.h>
    #include <wx/filesys.h>
    #include <wx/intl.h>
    #include <wx/menu.h>
    #include <wx/string.h>

    #include "pluginmanager_base.h"
    #include "cbexception.h"
    #include "cbplugin.h"
    #include "logmanager.h"
    #include "macrosmanager.h"
    #include "manager.h"
    #include "configmanager.h"
    #include "personalitymanager.h"
    #include "scriptingmanager.h"
    #include "globals.h"
    #include "sdk_events.h"
#endif

#include <wx/dynlib.h>
#include <wx/filesys.h>
#include <wx/utils.h>
#include <wx/filename.h>

#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/txtstrm.h>

#include "filefilters.h"
#include "tinyxml/tinyxml.h"

#include "annoyingdialog.h"
#ifndef CB_FOR_CONSOLE
#include "pluginsconfigurationdlg.h"
#else
#include "infowindow_base.h"
#endif // #ifndef CB_FOR_CONSOLE

#include "scripting/bindings/sc_plugin.h"

#ifdef CB_FOR_CONSOLE
    template<> PLUGIN_MANAGER_BASE* Mgr<PLUGIN_MANAGER_BASE>::instance = 0;
    template<> bool  Mgr<PLUGIN_MANAGER_BASE>::isShutdown = false;
#endif //#ifdef CB_FOR_CONSOLE

namespace LibLoader
{
    struct RefCountedLib
    {
        RefCountedLib() : lib(0), ref(0) {}
        wxDynamicLibrary* lib;
        int ref;
    };
    typedef std::map<wxString, RefCountedLib> Libs;
    Libs s_Libs;

    wxDynamicLibrary* LoadLibrary(const wxString& filename)
    {
        Libs::iterator it = s_Libs.find(filename);
        if (it != s_Libs.end())
        {
            // existing lib./codeblocks
            it->second.ref++;
            return it->second.lib;
        }
        // new lib
        it = s_Libs.insert(s_Libs.end(), std::make_pair(filename, RefCountedLib()));
        it->second.lib = new wxDynamicLibrary;
        it->second.ref = 1;
        it->second.lib->Load(filename);
        return it->second.lib;
    }

    void RemoveLibrary(wxDynamicLibrary* lib)
    {
        Libs::iterator it;
        for (it = s_Libs.begin(); it != s_Libs.end(); ++it)
        {
            RefCountedLib& rcl = it->second;
            if (rcl.lib == lib)
            {
                // found
                rcl.ref--;
                if (rcl.ref == 0)
                {
                    // only delete the lib if not shutting down
                    // if we are shutting down, it will be deleted automatically
                    if (!Manager::IsAppShuttingDown())
                        delete rcl.lib;
                    s_Libs.erase(it);
                }
                return;
            }
        }
        // if we reached here, it's a lib that was not handled by us
        // (or had wrong refcounting)
    }

    void Cleanup()
    {
        Libs::iterator it;
        for (it = s_Libs.begin(); it != s_Libs.end(); ++it)
        {
            RefCountedLib& rcl = it->second;
            // only delete the lib if not shutting down
            // if we are shutting down, it will be deleted automatically
            if (!Manager::IsAppShuttingDown())
                delete rcl.lib;
        }
        s_Libs.clear();
    }
};

//static
bool PLUGIN_MANAGER_BASE::s_SafeMode = false;

// class constructor
PLUGIN_MANAGER_BASE::PLUGIN_MANAGER_BASE()
    : m_pCurrentlyLoadingLib(0),
    m_pCurrentlyLoadingManifestDoc(0)
{
}

// class destructor
PLUGIN_MANAGER_BASE::~PLUGIN_MANAGER_BASE()
{
    UnloadAllPlugins();
}

bool PLUGIN_MANAGER_BASE::AttachPlugin(cbPlugin* plugin, bool ignoreSafeMode)
{
    if (!plugin)
        return false;
    if (plugin->IsAttached())
        return true;

    if (!s_SafeMode || ignoreSafeMode)
        plugin->Attach();
    return true;
}

bool PLUGIN_MANAGER_BASE::DetachPlugin(cbPlugin* plugin)
{
    if (!plugin)
        return false;
    if (!plugin->IsAttached())
        return true;

    Manager::Get()->RemoveAllEventSinksFor(plugin);
    plugin->Release(Manager::IsAppShuttingDown());
    return true;
}

void PLUGIN_MANAGER_BASE::RegisterPlugin(const wxString& name,
                                    CreatePluginProc createProc,
                                    FreePluginProc freeProc,
                                    PluginSDKVersionProc versionProc)
{
    // sanity checks
    if (name.IsEmpty() || !createProc || !freeProc || !versionProc)
        return;

    // first check to see it's not already loaded
    if (FindPluginByName(name))
        return; // yes, already loaded

    // read manifest file for plugin
    PluginInfo info;
    if (!ReadManifestFile(m_CurrentlyLoadingFilename, name, &info) ||
        info.name.IsEmpty())
    {
        Manager::Get()->GetLogManager()->LogError(_T("Invalid manifest file for: ") + name);
        return;
    }

    // now get the SDK version number (extra check)
    int major;
    int minor;
    int release;
    versionProc(&major, &minor, &release);
    if (major != PLUGIN_SDK_VERSION_MAJOR ||
        minor != PLUGIN_SDK_VERSION_MINOR ||
        release != PLUGIN_SDK_VERSION_RELEASE)
    {
        // wrong version: in this case, inform the user...
        wxString fmt;
        fmt.Printf(_("SDK version mismatch for %s (%d.%d.%d). Expecting %d.%d.%d"),
                    name.c_str(),
                    major,
                    minor,
                    release,
                    PLUGIN_SDK_VERSION_MAJOR,
                    PLUGIN_SDK_VERSION_MINOR,
                    PLUGIN_SDK_VERSION_RELEASE);
        Manager::Get()->GetLogManager()->LogError(fmt);
        return;
    }

    // all done
    // add this plugin in the temporary registration vector to be loaded
    // by LoadPlugin() (which triggered the call to this function).
    PluginRegistration pr;
    pr.name = name;
    pr.createProc = createProc;
    pr.freeProc = freeProc;
    pr.versionProc = versionProc;
    pr.info = info;
    m_RegisteredPlugins.push_back(pr);
}

bool PLUGIN_MANAGER_BASE::ReadManifestFile(const wxString& pluginFilename,
                                    const wxString& pluginName,
                                    PluginInfo* infoOut)
{
    if (!m_pCurrentlyLoadingManifestDoc)
    {
        // find and load plugin's resource file
        // (pluginFilename contains no path info)
        wxFileName fname(pluginFilename);
        fname.SetExt(_T("zip"));
        wxString actual = fname.GetFullName();

        // remove 'lib' prefix from plugin name (if any)
        if (!platform::windows && actual.StartsWith(_T("lib")))
            actual.Remove(0, 3);

        actual = ConfigManager::LocateDataFile(actual, sdPluginsUser | sdDataUser | sdPluginsGlobal | sdDataGlobal);
        if (actual.IsEmpty())
        {
            Manager::Get()->GetLogManager()->LogError(_T("Plugin resource not found: ") + fname.GetFullName());
            return false; // not found
        }

        // load XML from ZIP
        wxString contents;
        wxFileSystem* fs = new wxFileSystem;
        wxFSFile* f = fs->OpenFile(actual + _T("#zip:manifest.xml"));
        if (f)
        {
            wxInputStream* is = f->GetStream();
            char tmp[1024] = {};
            while (!is->Eof() && is->CanRead())
            {
                memset(tmp, 0, sizeof(tmp));
                is->Read(tmp, sizeof(tmp) - 1);
                contents << cbC2U((const char*)tmp);
            }
            delete f;
        }
        else
        {
            Manager::Get()->GetLogManager()->LogError(_T("No plugin manifest file in resource: ") + actual);
            delete fs;
            return false;
        }
        delete fs;

        // actually load XML document
        m_pCurrentlyLoadingManifestDoc = new TiXmlDocument;
        if (!m_pCurrentlyLoadingManifestDoc->Parse(cbU2C(contents)))
        {
            Manager::Get()->GetLogManager()->LogError(_T("Plugin manifest could not be parsed: ") + actual);
            return false;
        }
    }

    TiXmlElement* root = m_pCurrentlyLoadingManifestDoc->FirstChildElement("CodeBlocks_plugin_manifest_file");
    if (!root)
    {
        Manager::Get()->GetLogManager()->LogError(_T("Plugin resource file not valid (no root element found) for: ") + pluginFilename);
        return false;
    }

    TiXmlElement* version = root->FirstChildElement("SdkVersion");
    if (!version)
    {
        Manager::Get()->GetLogManager()->LogError(_T("Plugin resource file not valid (no SdkVersion element found) for: ") + pluginFilename);
        return false;
    }

    // check version
//    int major;
//    int minor;
//    int release;
//    if (version->QueryIntAttribute("major", &major) != TIXML_SUCCESS)
//        major = 0;
//    if (version->QueryIntAttribute("minor", &minor) != TIXML_SUCCESS)
//        minor = 0;
//    if (version->QueryIntAttribute("release", &release) != TIXML_SUCCESS)
//        release = 0;
//
//    if (major != PLUGIN_SDK_VERSION_MAJOR ||
//        minor != PLUGIN_SDK_VERSION_MINOR ||
//        release != PLUGIN_SDK_VERSION_RELEASE)
//    {
//        // wrong version: in this case, inform the user...
//        wxString fmt;
//        fmt.Printf(_("SDK version mismatch for %s (%d.%d.%d). Expecting %d.%d.%d"),
//                    pluginName.c_str(),
//                    major,
//                    minor,
//                    release,
//                    PLUGIN_SDK_VERSION_MAJOR,
//                    PLUGIN_SDK_VERSION_MINOR,
//                    PLUGIN_SDK_VERSION_RELEASE);
//        Manager::Get()->GetLogManager()->LogError(fmt);
//        return false;
//    }

    // if no plugin name specified, we 're done here (successfully)
    if (pluginName.IsEmpty() || !infoOut)
        return true;

    TiXmlElement* plugin = root->FirstChildElement("Plugin");
    while (plugin)
    {
        const char* name = plugin->Attribute("name");
        if (name && cbC2U(name) == pluginName)
        {
            infoOut->name = pluginName;
            TiXmlElement* value = plugin->FirstChildElement("Value");
            while (value)
            {
                if (value->Attribute("title"))
                    infoOut->title = cbC2U(value->Attribute("title"));
                if (value->Attribute("version"))
                    infoOut->version = cbC2U(value->Attribute("version"));
                if (value->Attribute("description"))
                    infoOut->description = cbC2U(value->Attribute("description"));
                if (value->Attribute("author"))
                    infoOut->author = cbC2U(value->Attribute("author"));
                if (value->Attribute("authorEmail"))
                    infoOut->authorEmail = cbC2U(value->Attribute("authorEmail"));
                if (value->Attribute("authorWebsite"))
                    infoOut->authorWebsite = cbC2U(value->Attribute("authorWebsite"));
                if (value->Attribute("thanksTo"))
                    infoOut->thanksTo = cbC2U(value->Attribute("thanksTo"));
                if (value->Attribute("license"))
                    infoOut->license = cbC2U(value->Attribute("license"));

                value = value->NextSiblingElement("Value");
            }
            break;
        }

        plugin = plugin->NextSiblingElement("Plugin");
    }

    return true;
}

void PLUGIN_MANAGER_BASE::ReadExtraFilesFromManifestFile(const wxString& pluginFilename,
                                                    wxArrayString& extraFiles)
{
    extraFiles.Clear();

    // find and load plugin's resource file
    // (pluginFilename contains no path info)
    wxFileName fname(pluginFilename);
    fname.SetExt(_T("zip"));
    wxString actual = fname.GetFullName();

    // remove 'lib' prefix from plugin name (if any)
    if (!platform::windows && actual.StartsWith(_T("lib")))
        actual.Remove(0, 3);

    actual = ConfigManager::LocateDataFile(actual, sdPluginsUser | sdDataUser | sdPluginsGlobal | sdDataGlobal);
    if (actual.IsEmpty())
    {
        Manager::Get()->GetLogManager()->LogError(_T("Plugin resource not found: ") + fname.GetFullName());
        return; // not found
    }

    // load XML from ZIP
    wxString contents;
    wxFileSystem* fs = new wxFileSystem;
    wxFSFile* f = fs->OpenFile(actual + _T("#zip:manifest.xml"));
    if (f)
    {
        wxInputStream* is = f->GetStream();
        char tmp[1024] = {};
        while (!is->Eof() && is->CanRead())
        {
            memset(tmp, 0, sizeof(tmp));
            is->Read(tmp, sizeof(tmp) - 1);
            contents << cbC2U((const char*)tmp);
        }
        delete f;
    }
    else
    {
        Manager::Get()->GetLogManager()->LogError(_T("No plugin manifest file in resource: ") + actual);
        delete fs;
        return;
    }
    delete fs;

    // actually load XML document
    TiXmlDocument doc;
    if (!doc.Parse(cbU2C(contents)))
        return;

    TiXmlElement* root = doc.FirstChildElement("CodeBlocks_plugin_manifest_file");
    if (!root)
        return;

    TiXmlElement* extra = root->FirstChildElement("Extra");
    while (extra)
    {
        const char* file = extra->Attribute("file");
        if (file && *file)
        {
            extraFiles.Add(cbC2U(file));
        }

        extra = extra->NextSiblingElement("Extra");
    }
}

int PLUGIN_MANAGER_BASE::ScanForPlugins(const wxString& path)
{
    static const wxString PluginsMask = platform::windows ? _T("*.dll") : _T("*.so");
    int count = 0;
    if(!wxDirExists(path))
        return count;
    wxDir dir(path);

    if (!dir.IsOpened())
        return count;

    bool batch = Manager::IsBatchBuild();
    wxArrayString bbplugins;
    if (batch)
    {
        ConfigManager *bbcfg = Manager::Get()->GetConfigManager(_T("plugins"));
        bbplugins = bbcfg->ReadArrayString(_T("/batch_build_plugins"));
        if (!bbplugins.GetCount())
        {
            // defaults
            if(platform::windows)
                bbplugins.Add(_T("compiler.dll"));
            else
#ifndef CB_FOR_CONSOLE
                bbplugins.Add(_T("libcompiler.so"));
#else // #ifndef CB_FOR_CONSOLE
                bbplugins.Add(_T("libcon_compiler.so"));
#endif // #ifndef CB_FOR_CONSOLE
        }
    }

    wxString filename;
    wxString failed;
    bool ok = dir.GetFirst(&filename, PluginsMask, wxDIR_FILES);
    while (ok)
    {
#ifndef CB_FOR_CONSOLE
        if (filename == _T("libcon_compiler.so"))
        {
            ok = dir.GetNext(&filename);
            if (ok)
                continue;
            else
                break;
        }
#endif // #ifndef CB_FOR_CONSOLE
        if (batch)
        {
            // for batch builds, we will load only those plugins that the
            // user has set (default only compiler.dll)
            bool matched = false;
            for (size_t i = 0; i < bbplugins.GetCount(); ++i)
            {
                if (bbplugins[i] == filename)
                {
                    matched = true;
                    break;
                }
            }
            if (!matched)
            {
                ok = dir.GetNext(&filename);
                continue;
            }
        }

        // load manifest
        m_pCurrentlyLoadingManifestDoc = 0;
        if (ReadManifestFile(filename))
        {
            if (LoadPlugin(path + _T('/') + filename))
                ++count;
            else
                failed << _T('\n') << filename;
        }
        if (m_pCurrentlyLoadingManifestDoc)
        {
            delete m_pCurrentlyLoadingManifestDoc;
            m_pCurrentlyLoadingManifestDoc = 0;
        }
        ok = dir.GetNext(&filename);
    }
    Manager::Get()->GetLogManager()->Log(F(_("Loaded %d plugins"), count));
    if (!failed.IsEmpty())
    {
        InfoWindow::Display(_("Warning"),
                            _("One or more plugins were not loaded.\n"
                            "This usually happens when a plugin is built for\n"
                            "a different version of the Code::Blocks SDK.\n"
                            "Check the application log for more info.\n\n"
                            "List of failed plugins:\n") + failed,
                            15000, 3000);
    }
    return count;
}

bool PLUGIN_MANAGER_BASE::LoadPlugin(const wxString& pluginName)
{
#ifndef CB_FOR_CONSOLE
        if(pluginName.EndsWith(_T("libcon_compiler.so")))
            return false;
#endif // #ifndef CB_FOR_CONSOLE
    // clear registration temporary vector
    m_RegisteredPlugins.clear();

    // load library
    m_CurrentlyLoadingFilename = pluginName;
    m_pCurrentlyLoadingLib = LibLoader::LoadLibrary(pluginName);
    if (!m_pCurrentlyLoadingLib->IsLoaded())
    {
        #if wxCHECK_VERSION(2, 9, 0)
        Manager::Get()->GetLogManager()->LogError(F(_T("%s: not loaded (missing symbols?)"), pluginName.wx_str()));
        #else
        Manager::Get()->GetLogManager()->LogError(F(_T("%s: not loaded (missing symbols?)"), pluginName.c_str()));
        #endif
        LibLoader::RemoveLibrary(m_pCurrentlyLoadingLib);
        m_pCurrentlyLoadingLib = 0;
        m_CurrentlyLoadingFilename.Clear();
        return false;
    }

    // by now, the library has loaded and its global variables are initialized.
    // this means it has already called RegisterPlugin()
    // now we can actually create the plugin(s) instance(s) :)

    // try to load the plugin(s)
    std::vector<PluginRegistration>::iterator it;
    for (it = m_RegisteredPlugins.begin(); it != m_RegisteredPlugins.end(); ++it)
    {
        PluginRegistration& pr = *it;
        cbPlugin* plug = 0L;
        try
        {
            plug = pr.createProc();
        }
        catch (cbException& exception)
        {
            exception.ShowErrorMessage(false);
            continue;
        }

        // all done; add it to our list
        PluginElement* plugElem = new PluginElement;
        plugElem->fileName = m_CurrentlyLoadingFilename;
        plugElem->info = pr.info;
        plugElem->library = m_pCurrentlyLoadingLib;
        plugElem->freeProc = pr.freeProc;
        plugElem->plugin = plug;
        m_Plugins.Add(plugElem);

        SetupLocaleDomain(pr.name);

        #if wxCHECK_VERSION(2, 9, 0)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("%s: loaded"), pr.name.wx_str()));
        #else
        Manager::Get()->GetLogManager()->DebugLog(F(_T("%s: loaded"), pr.name.c_str()));
        #endif
    }

    if (m_RegisteredPlugins.empty())
    {
        // no plugins loaded from this library, but it's not an error
        LibLoader::RemoveLibrary(m_pCurrentlyLoadingLib);
    }
    m_pCurrentlyLoadingLib = 0;
    m_CurrentlyLoadingFilename.Clear();
    return true;
}

void PLUGIN_MANAGER_BASE::LoadAllPlugins()
{
    // check if a plugin crashed the app last time
    wxString probPlugin = Manager::Get()->GetConfigManager(_T("plugins"))->Read(_T("/try_to_activate"), wxEmptyString);
    if (!probPlugin.IsEmpty())
    {
        wxString msg;
        msg.Printf(_("Plugin \"%s\" failed to load last time Code::Blocks was executed.\n"
                    "Do you want to disable this plugin from loading?"), probPlugin.c_str());
        if (cbMessageBox(msg, _("Warning"), wxICON_WARNING | wxYES_NO) == wxID_NO)
            probPlugin = _T("");
    }

    PluginElement* elem = 0;
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        elem = m_Plugins[i];
        cbPlugin* plug = elem->plugin;
        if (!plug || plug->IsAttached())
            continue;

        // do not load it if the user has explicitly asked not to...
        wxString baseKey;
        baseKey << _T("/") << elem->info.name;
        bool loadIt = Manager::Get()->GetConfigManager(_T("plugins"))->ReadBool(baseKey, true);

        // if we have a problematic plugin, check if this is it
        if (loadIt && !probPlugin.IsEmpty())
        {
            loadIt = elem->info.title != probPlugin;
            // if this is the problematic plugin, don't load it
            if (!loadIt)
                Manager::Get()->GetConfigManager(_T("plugins"))->Write(baseKey, false);
        }

        if (loadIt)
        {
            Manager::Get()->GetConfigManager(_T("plugins"))->Write(_T("/try_to_activate"), elem->info.title);
            Manager::Get()->GetLogManager()->Log(elem->info.name);
            try
            {
                AttachPlugin(plug);
                Manager::Get()->GetConfigManager(_T("plugins"))->Write(_T("/try_to_activate"), wxEmptyString, false);
            }
            catch (cbException& exception)
            {
                Manager::Get()->GetLogManager()->Log(_T("[failed]"));
                exception.ShowErrorMessage(false);

                wxString msg;
                msg.Printf(_("Plugin \"%s\" failed to load...\n"
                            "Do you want to disable this plugin from loading next time?"), elem->info.title.c_str());
                if (cbMessageBox(msg, _("Warning"), wxICON_WARNING | wxYES_NO) == wxID_YES)
                    Manager::Get()->GetConfigManager(_T("plugins"))->Write(baseKey, false);
            }
        }
    }
    Manager::Get()->GetConfigManager(_T("plugins"))->Write(_T("/try_to_activate"), wxEmptyString, false);
}

void PLUGIN_MANAGER_BASE::UnloadAllPlugins()
{
//    Manager::Get()->GetLogManager()->DebugLog("Count %d", m_Plugins.GetCount());

    while (m_Plugins.GetCount())
    {
        UnloadPlugin(m_Plugins[0]->plugin);
    }
    m_Plugins.Clear();
    LibLoader::Cleanup();
}

void PLUGIN_MANAGER_BASE::UnloadPlugin(cbPlugin* plugin)
{
    if (!plugin)
        return;

    // detach plugin if needed
    DetachPlugin(plugin);

    // find plugin element
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        PluginElement* plugElem = m_Plugins[i];
        if (plugElem->plugin == plugin)
        {
            // found
            // free plugin
            if (plugElem->freeProc)
                plugElem->freeProc(plugin);
            else
                delete plugin; // try to delete it ourselves...
            // remove lib
            LibLoader::RemoveLibrary(plugElem->library);
            // and delete plugin element
            delete plugElem;
            m_Plugins.RemoveAt(i);

            break;
        }
    }
}

PluginElement* PLUGIN_MANAGER_BASE::FindElementByName(const wxString& pluginName)
{
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        PluginElement* plugElem = m_Plugins[i];
        if (plugElem->info.name == pluginName)
            return plugElem;
    }

    return 0;
}

cbPlugin* PLUGIN_MANAGER_BASE::FindPluginByName(const wxString& pluginName)
{
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        PluginElement* plugElem = m_Plugins[i];
        if (plugElem->info.name == pluginName)
            return plugElem->plugin;
    }

    return 0;
}

cbPlugin* PLUGIN_MANAGER_BASE::FindPluginByFileName(const wxString& pluginFileName)
{
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        PluginElement* plugElem = m_Plugins[i];
        if (plugElem->fileName == pluginFileName)
            return plugElem->plugin;
    }

    return 0;
}

const PluginInfo* PLUGIN_MANAGER_BASE::GetPluginInfo(const wxString& pluginName)
{
    PluginElement* plugElem = FindElementByName(pluginName);
    if (plugElem && plugElem->info.name == pluginName)
        return &plugElem->info;

    return 0;
}

const PluginInfo* PLUGIN_MANAGER_BASE::GetPluginInfo(cbPlugin* plugin)
{
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        PluginElement* plugElem = m_Plugins[i];
        if (plugElem->plugin == plugin)
            return &plugElem->info;
    }

    return 0;
}

int PLUGIN_MANAGER_BASE::ExecutePlugin(const wxString& pluginName)
{
    PluginElement* elem = FindElementByName(pluginName);
    cbPlugin* plug = elem ? elem->plugin : 0;
    if (plug)
    {
        if (plug->GetType() != ptTool)
        {
            #if wxCHECK_VERSION(2, 9, 0)
            Manager::Get()->GetLogManager()->LogError(F(_T("Plugin %s is not a tool to have Execute() method!"), elem->info.name.wx_str()));
            #else
            Manager::Get()->GetLogManager()->LogError(F(_T("Plugin %s is not a tool to have Execute() method!"), elem->info.name.c_str()));
            #endif
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
    else
    {
        #if wxCHECK_VERSION(2, 9, 0)
        Manager::Get()->GetLogManager()->LogError(F(_T("No plugin registered by this name: %s"), pluginName.wx_str()));
        #else
        Manager::Get()->GetLogManager()->LogError(F(_T("No plugin registered by this name: %s"), pluginName.c_str()));
        #endif
    }
    return 0;
}

PluginsArray PLUGIN_MANAGER_BASE::GetToolOffers()
{
    return GetOffersFor(ptTool);
}

PluginsArray PLUGIN_MANAGER_BASE::GetMimeOffers()
{
    return GetOffersFor(ptMime);
}

PluginsArray PLUGIN_MANAGER_BASE::GetCompilerOffers()
{
    return GetOffersFor(ptCompiler);
}

PluginsArray PLUGIN_MANAGER_BASE::GetDebuggerOffers()
{
    return GetOffersFor(ptDebugger);
}

PluginsArray PLUGIN_MANAGER_BASE::GetCodeCompletionOffers()
{
    return GetOffersFor(ptCodeCompletion);
}

PluginsArray PLUGIN_MANAGER_BASE::GetOffersFor(PluginType type)
{
    PluginsArray arr;

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

cbMimePlugin* PLUGIN_MANAGER_BASE::GetMIMEHandlerForFile(const wxString& filename)
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

void PLUGIN_MANAGER_BASE::SetupLocaleDomain(const wxString& DomainName)
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

void PLUGIN_MANAGER_BASE::NotifyPlugins(CodeBlocksEvent& event)
{
    Manager::Get()->ProcessEvent(event);
}

void PLUGIN_MANAGER_BASE::NotifyPlugins(CodeBlocksDockEvent& event)
{
    Manager::Get()->ProcessEvent(event);
}

void PLUGIN_MANAGER_BASE::NotifyPlugins(CodeBlocksLayoutEvent& event)
{
    Manager::Get()->ProcessEvent(event);
}

bool PLUGIN_MANAGER_BASE::IsFileExtRegistered(const wxString& filename)
{
    if (filename.IsEmpty())
        return false;

    const wxString file = filename.AfterLast(wxFILE_SEP_PATH).Lower();
    const int      pos  = file.Find(_T('.'), true);
    wxString       ext;
    if (pos != wxNOT_FOUND)
        ext = file.Mid(pos+1);

    if (ext.IsEmpty())
        return false;

    bool found = false;
    std::map<wxString, std::set<wxString> >::iterator it;
    for (it=m_CCFileExts.begin(); it != m_CCFileExts.end(); ++it)
    {
        if ((*it).second.count(ext) != 0)
        {
            found = true;
            break;
        }
    }
    return found;
}
