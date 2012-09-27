/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/dir.h>
    #include <wx/filesys.h>
    #include <wx/intl.h>
    #include <wx/menu.h>
    #include <wx/string.h>

    #include "pluginmanager.h"
    #include "cbexception.h"
    #include "cbplugin.h"
    #include "logmanager.h"
    #include "macrosmanager.h"
    #include "manager.h"
    #include "editormanager.h"
    #include "configmanager.h"
    #include "personalitymanager.h"
    #include "scriptingmanager.h"
    #include "globals.h"
    #include "sdk_events.h"
#endif

#include <wx/dynlib.h>
#include <wx/filesys.h>
#include <wx/progdlg.h>
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
#endif // #ifndef CB_FOR_CONSOLE

#include "scripting/bindings/sc_plugin.h"

template<> PluginManager* Mgr<PluginManager>::instance = 0;
template<> bool  Mgr<PluginManager>::isShutdown = false;

void VersionStringToNumbers(const wxString& version, long* major, long* minor, long* release)
{
    wxString majorS = version.BeforeFirst(_T('.')); // 6.3.2 -> 6
    wxString minorS = version.AfterFirst(_T('.')); // 6.3.2 -> 3.2
    wxString releaseS = version.AfterLast(_T('.')); // 6.3.2 -> 2
    minorS = minorS.BeforeFirst(_T('.')); // 3.2 -> 3
    if (major)
        majorS.ToLong(major);
    if (minor)
        minorS.ToLong(minor);
    if (release)
        releaseS.ToLong(release);
}

// returns -1 if new is less then old, 0 if equal and 1 if new is greater than old
int CompareVersions(const wxString& new_version, const wxString& old_version)
{
    long new_major, new_minor, new_release;
    long old_major, old_minor, old_release;

    VersionStringToNumbers(new_version, &new_major, &new_minor, &new_release);
    VersionStringToNumbers(old_version, &old_major, &old_minor, &old_release);

#define SIGN(a) (a>0?1:(a<0?-1:0))
    int result = 0;
    result += SIGN(new_major - old_major) << 2;
    result += SIGN(new_minor - old_minor) << 1;
    result += SIGN(new_release - old_release) << 0;
#undef SIGN

    if (result < 0) return -1;
    else if (result > 0) return 1;
    return 0;
}

BEGIN_EVENT_TABLE(PluginManager, wxEvtHandler)
//
END_EVENT_TABLE()

// class constructor
PluginManager::PluginManager()
{
    Manager::Get()->GetAppWindow()->PushEventHandler(this);
}

void PluginManager::CreateMenu(wxMenuBar* /*menuBar*/)
{
}

void PluginManager::ReleaseMenu(wxMenuBar* /*menuBar*/)
{
}

bool PluginManager::InstallPlugin(const wxString& pluginName, bool forAllUsers, bool askForConfirmation)
{
    if (pluginName.IsEmpty())
        return false;

    wxString actualName = pluginName;
    Manager::Get()->GetMacrosManager()->ReplaceMacros(actualName);

    // base name
    wxString basename = wxFileName(actualName).GetName();
    wxString version;
    if (basename.Contains(_T('-')))
    {
        version = basename.AfterFirst(_T('-'));
        basename = basename.BeforeFirst(_T('-'));
    }

//    Manager::Get()->GetLogManager()->DebugLog(F(_T("InstallPlugin: basename='%s', version=%s"), basename.c_str(), version.c_str()));

    // if plugin with the same name exists, ask to uninstall first
    cbPlugin* existingPlugin = FindPluginByName(basename);
    if (existingPlugin)
    {
        if (askForConfirmation)
        {
            wxString msg = _("A plugin with the same name is already installed.\n");
            if (!version.IsEmpty())
            {
                const PluginInfo* existingInfo = GetPluginInfo(existingPlugin);
                if (CompareVersions(version, existingInfo->version) < 0)
                {
                    msg = _("The plugin you are trying to install, is older "
                            "than the one currently installed.");
                }
            }

            if (cbMessageBox(msg + _T('\n') +
                            _("If you want to proceed, the installed plugin will be "
                            "uninstalled first.\n"
                            "Do you want to proceed?"),
                            _("Confirmation"), wxICON_QUESTION | wxYES_NO) == wxID_NO)
            {
                return false;
            }
        }
        if (!UninstallPlugin(existingPlugin))
            return false;
    }

    wxString pluginDir;
    wxString resourceDir;
    if (forAllUsers)
    {
        pluginDir = ConfigManager::GetFolder(sdPluginsGlobal);
        resourceDir = ConfigManager::GetFolder(sdDataGlobal);
    }
    else
    {
        pluginDir = ConfigManager::GetFolder(sdPluginsUser);
        resourceDir = ConfigManager::GetFolder(sdDataUser);
    }

    wxProgressDialog pd(_("Installing: ") + basename, _T("A description wide enough for the dialog ;)"), 5);

    wxString localName = basename + FileFilters::DYNAMICLIB_DOT_EXT;
    wxString resourceName = basename + _T(".zip");
    wxString settingsOnName = basename + _T(".png");
    wxString settingsOffName = basename + _T("-off.png");
    if (!platform::windows && resourceName.StartsWith(_T("lib")))
        resourceName.Remove(0, 3);
    if (!platform::windows && settingsOnName.StartsWith(_T("lib")))
        settingsOnName.Remove(0, 3);
    if (!platform::windows && settingsOffName.StartsWith(_T("lib")))
        settingsOffName.Remove(0, 3);
    wxString pluginFilename = pluginDir + _T('/') + localName;
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Plugin filename: ") + pluginFilename));
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Plugin resources: ") + ConfigManager::GetDataFolder() + _T('/') + resourceName));

    pd.Update(1, _("Extracting plugin"));

    // extract plugin from bundle
    if (!ExtractFile(actualName,
                    localName,
                    pluginFilename))
        return false;
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Extracted plugin")));

    pd.Update(2, _("Extracting plugin resources"));

    // extract resources from bundle
    if (!ExtractFile(actualName,
                    resourceName,
                    resourceDir + _T('/') + resourceName))
        return false;
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Extracted resources")));

    pd.Update(3, _("Extracting plugin icons for \"Settings\" dialog"));

    // extract resources from bundle
    ExtractFile(actualName,
                settingsOnName,
                resourceDir + _T("/images/settings/") + settingsOnName,
                false);
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Extracted resources")));

    // extract resources from bundle
    ExtractFile(actualName,
                settingsOffName,
                resourceDir + _T("/images/settings/") + settingsOffName,
                false);
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Extracted resources")));

    // extract extra files
    wxArrayString extraFiles;
    ReadExtraFilesFromManifestFile(localName, extraFiles);
    for (size_t i = 0; i < extraFiles.GetCount(); ++i)
    {
        ExtractFile(actualName,
                    extraFiles[i],
                    resourceDir + _T("/") + extraFiles[i],
                    false);
    }

    pd.Update(4, _("Loading plugin"));

    // bundle extracted; now load the plugin on-the-fly
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Loading plugin...")));
    ScanForPlugins(pluginDir);
    LoadAllPlugins();
    cbPlugin* plugin = FindPluginByFileName(pluginFilename);
    const PluginInfo* info = GetPluginInfo(plugin);
    if (!plugin || !info)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Failed"));
        return false;
    }
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Succeeded")));

    // inform app to update menus and toolbars
    pd.Update(5, _("Updating menus and toolbars"));
    CodeBlocksEvent evt(cbEVT_PLUGIN_INSTALLED);
    evt.SetPlugin(plugin);
    Manager::Get()->ProcessEvent(evt);
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Menus updated")));

    return true;
}

bool PluginManager::UninstallPlugin(cbPlugin* plugin, bool removeFiles)
{
    if (!plugin)
        return false;

    wxString title;
    wxString pluginFilename;
    wxString resourceFilename;
    wxString settingsOnFilename;
    wxString settingsOffFilename;
    wxArrayString extrafiles;

    // find the plugin element
    for (size_t i = 0; i < m_Plugins.GetCount(); ++i)
    {
        PluginElement* elem = m_Plugins[i];
        if (elem && elem->plugin == plugin)
        {
            // got it
            title = elem->info.title;
            pluginFilename = elem->fileName;
            // now get the resource name
            wxFileName fname(pluginFilename);
            resourceFilename = fname.GetName() + _T(".zip");
            settingsOnFilename = fname.GetName() + _T(".png");
            settingsOffFilename = fname.GetName() + _T("-off.png");
            if (!platform::windows && resourceFilename.StartsWith(_T("lib")))
                resourceFilename.Remove(0, 3);
            if (!platform::windows && settingsOnFilename.StartsWith(_T("lib")))
                settingsOnFilename.Remove(0, 3);
            if (!platform::windows && settingsOffFilename.StartsWith(_T("lib")))
                settingsOffFilename.Remove(0, 3);
            resourceFilename = ConfigManager::LocateDataFile(resourceFilename, sdDataGlobal | sdDataUser);
            settingsOnFilename = ConfigManager::LocateDataFile(_T("images/settings/") + settingsOnFilename, sdDataGlobal | sdDataUser);
            settingsOffFilename = ConfigManager::LocateDataFile(_T("images/settings/") + settingsOffFilename, sdDataGlobal | sdDataUser);

            ReadExtraFilesFromManifestFile(resourceFilename, extrafiles);
            for (size_t n = 0; n < extrafiles.GetCount(); ++n)
            {
                extrafiles[n] = ConfigManager::LocateDataFile(extrafiles[n], sdDataGlobal | sdDataUser);
            }
            break;
        }
    }

    if (wxFileExists(pluginFilename) && !wxFile::Access(pluginFilename, wxFile::write))
    {
        // no write-access; abort
        cbMessageBox(_("You don't have the needed privileges to uninstall this plugin.\n"
                        "Ask your administrator to uninstall this plugin for you..."),
                        _("Warning"), wxICON_WARNING);
        return false;
    }

//    Manager::Get()->GetLogManager()->DebugLog(F(_T("UninstallPlugin:")));
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Plugin filename: ") + pluginFilename));
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Plugin resources: ") + resourceFilename));

    wxProgressDialog pd(wxString::Format(_("Uninstalling %s"), title.c_str()),
                        _T("A description wide enough for the dialog ;)"), 3);

    pd.Update(1, _("Detaching plugin"));
    DetachPlugin(plugin);
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Plugin released")));

    pd.Update(2, _("Updating menus and toolbars"));
    CodeBlocksEvent event(cbEVT_PLUGIN_UNINSTALLED);
    event.SetPlugin(plugin);
    Manager::Get()->ProcessEvent(event);
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Menus updated")));

    pd.Update(3, _("Unloading plugin"));
    UnloadPlugin(plugin);
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Plugin unloaded")));

    if (!removeFiles)
        return true;

    // under linux, if the progress dialog is still visible and updated
    // causes a crash because it re-enters gtk_main_iteration() calling
    // eventually OnUpdateUI() in the config dialog, which in turn references
    // an invalid plugin...
//    pd.Update(4, _("Removing files"));

    if (!pluginFilename.IsEmpty())
    {
        if (wxRemoveFile(pluginFilename))
        {
//            Manager::Get()->GetLogManager()->DebugLog(F(_T("Plugin file removed")));
            if (!resourceFilename.IsEmpty())
            {
                if (!wxRemoveFile(resourceFilename))
                    Manager::Get()->GetLogManager()->LogWarning(_T("Failed to remove plugin resources: ") + resourceFilename);
            }
            if (!settingsOnFilename.IsEmpty() && wxFileExists(settingsOnFilename))
            {
                if (!wxRemoveFile(settingsOnFilename))
                    Manager::Get()->GetLogManager()->LogWarning(_T("Failed to remove icon for \"Settings\" dialog: ") + settingsOnFilename);
            }
            if (!settingsOffFilename.IsEmpty() && wxFileExists(settingsOffFilename))
            {
                if (!wxRemoveFile(settingsOffFilename))
                    Manager::Get()->GetLogManager()->LogWarning(_T("Failed to remove icon for \"Settings\" dialog: ") + settingsOffFilename);
            }
            for (size_t i = 0; i < extrafiles.GetCount(); ++i)
            {
                if (!extrafiles[i].IsEmpty() && wxFileExists(extrafiles[i]))
                {
                    if (!wxRemoveFile(extrafiles[i]))
                        Manager::Get()->GetLogManager()->LogWarning(_T("Failed to remove extra file: ") + extrafiles[i]);
                }
            }
            return true;
        }
        else
        {
            Manager::Get()->GetLogManager()->LogWarning(_T("Failed to remove plugin file: ") + pluginFilename);
            cbMessageBox(_("Plugin could not be completely uninstalled because its files could not be removed.\n\n"
                            "This can happen if the plugin's file is in-use like, for "
                            "example, when the same plugin file provides more than one "
                            "plugin.\n"
                            "In this case either uninstall all other plugins "
                            "which are provided by the same file, or remove it yourself "
                            "(manually) when you shut down Code::Blocks.\n"
                            "The files that could not be deleted are:\n\n") +
                            pluginFilename + _T('\n') +
                            resourceFilename + _T('\n') +
                            settingsOnFilename + _T('\n') +
                            settingsOffFilename,
                            _("Warning"), wxICON_WARNING);
            return false;
        }
    }
    return false;
}

bool PluginManager::ExportPlugin(cbPlugin* plugin, const wxString& filename)
{
    if (!plugin)
        return false;

    wxArrayString sourcefiles;
    wxArrayString extrafiles;
    wxArrayString extrafilesdest;
    wxFileName fname;
    wxString resourceFilename;

    // find the plugin element
    for (size_t i = 0; i < m_Plugins.GetCount(); ++i)
    {
        PluginElement* elem = m_Plugins[i];
        if (elem && elem->plugin == plugin)
        {
            // got it

            // plugin file
            sourcefiles.Add(elem->fileName);
            fname.Assign(elem->fileName);

            // now get the resource zip filename
            resourceFilename = fname.GetName() + _T(".zip");
            if (!platform::windows && resourceFilename.StartsWith(_T("lib")))
                resourceFilename.Remove(0, 3);
            resourceFilename = ConfigManager::LocateDataFile(resourceFilename, sdDataGlobal | sdDataUser);
            sourcefiles.Add(resourceFilename);

            // the highlighted icon the plugin may have for its "settings" page
            resourceFilename = fname.GetName() + _T(".png");
            if (!platform::windows && resourceFilename.StartsWith(_T("lib")))
                resourceFilename.Remove(0, 3);
            resourceFilename.Prepend(_T("images/settings/"));
            resourceFilename = ConfigManager::LocateDataFile(resourceFilename, sdDataGlobal | sdDataUser);
            if (!resourceFilename.IsEmpty())
                sourcefiles.Add(resourceFilename);

            // the non-highlighted icon the plugin may have for its "settings" page
            resourceFilename = fname.GetName() + _T("-off.png");
            if (!platform::windows && resourceFilename.StartsWith(_T("lib")))
                resourceFilename.Remove(0, 3);
            resourceFilename.Prepend(_T("images/settings/"));
            resourceFilename = ConfigManager::LocateDataFile(resourceFilename, sdDataGlobal | sdDataUser);
            if (!resourceFilename.IsEmpty())
                sourcefiles.Add(resourceFilename);

            // export extra files
            resourceFilename = fname.GetName() + _T(".zip");
            if (!platform::windows && resourceFilename.StartsWith(_T("lib")))
                resourceFilename.Remove(0, 3);
            ReadExtraFilesFromManifestFile(resourceFilename, extrafilesdest);
            for (size_t n = 0; n < extrafilesdest.GetCount(); ++n)
            {
                extrafiles.Add(ConfigManager::LocateDataFile(extrafilesdest[n], sdDataGlobal | sdDataUser));
            }

            break;
        }
    }

    if (wxFileExists(filename))
    {
        if (!wxFile::Access(filename, wxFile::write))
        {
            cbMessageBox(wxString::Format(_("%s is in use.\nAborting..."), filename.c_str()),
                        _("Warning"), wxICON_WARNING);
            return false;
        }
    }

//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Creating archive: ") + filename));
    wxFileOutputStream out(filename);
    wxZipOutputStream zip(out, 9); // max compression
    for (size_t i = 0; i < sourcefiles.GetCount(); ++i)
    {
        if (sourcefiles[i].IsEmpty())
            continue;

        wxFileInputStream in(sourcefiles[i]);
        zip.PutNextEntry(wxFileName(sourcefiles[i]).GetFullName());
        zip << in;
    }
    for (size_t i = 0; i < extrafiles.GetCount(); ++i)
    {
        if (extrafiles[i].IsEmpty() || extrafilesdest[i].IsEmpty())
            continue;

        wxFileInputStream in(extrafiles[i]);

        zip.PutNextEntry(extrafilesdest[i]);
        zip << in;
    }
    zip.SetComment(_T("This is a redistributable plugin for the Code::Blocks IDE.\n"
                        "See http://www.codeblocks.org for details..."));

    return true;
}

bool PluginManager::ExtractFile(const wxString& bundlename,
                                const wxString& src_filename,
                                const wxString& dst_filename,
                                bool isMandatory)
{
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("ExtractFile:")));
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Plugin filename: ") + bundlename));
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Source filename: ") + src_filename));
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Destination filename: ") + dst_filename));

    // check if the destination file already exists
    if (wxFileExists(dst_filename) && !wxFile::Access(dst_filename, wxFile::write))
    {
//        Manager::Get()->GetLogManager()->DebugLog(F(_T("Destination file in use")));
        cbMessageBox(_("The destination file is in use.\nAborting..."), _("Warning"), wxICON_WARNING);
        return false;
    }

    // make sure destination dir exists
    CreateDirRecursively(wxFileName(dst_filename).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));

    // actually extract file
//    Manager::Get()->GetLogManager()->DebugLog(F(_T("Extracting...")));
    wxFileSystem* fs = new wxFileSystem;
    wxFSFile* f = fs->OpenFile(bundlename + _T("#zip:") + src_filename);
    if (f)
    {
        // open output file for writing
        wxFile output(dst_filename, wxFile::write);
        if (!output.IsOpened())
        {
//            Manager::Get()->GetLogManager()->DebugLog(F(_T("Can't open destination file for writing")));
            wxString msg = wxString::Format(_("Can't open destination file '%s' for writing..."),
                                            dst_filename.c_str());
            cbMessageBox(msg, _("Error"), wxICON_ERROR);
            delete f;
            delete fs;
            return false;
        }

        // copy file
        wxInputStream* is = f->GetStream();
        char tmp[1025] = {};
        while (!is->Eof() && is->CanRead())
        {
            memset(tmp, 0, sizeof(tmp));
            is->Read(tmp, sizeof(tmp) - 1);
            output.Write(tmp, is->LastRead());
        }
        delete f;
//        Manager::Get()->GetLogManager()->DebugLog(F(_T("Extracted")));
    }
    else
    {
//        Manager::Get()->GetLogManager()->DebugLog(F(_T("File not found in plugin")));
        if (isMandatory)
        {
            wxString msg = wxString::Format(_("File '%s' not found in plugin '%s'"),
                                            src_filename.c_str(), bundlename.c_str());
            cbMessageBox(msg, _("Error"), wxICON_ERROR);
            delete fs;
            return false;
        }
    }
    delete fs;
    return true;
}

int PluginManager::ConfigurePlugin(const wxString& pluginName)
{
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

void PluginManager::GetProjectConfigurationPanels(wxWindow* parent, cbProject* project, ConfigurationPanelsArray& arrayToFill)
{
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;
        if (plug && plug->IsAttached())
        {
            cbConfigurationPanel* pnl = plug->GetProjectConfigurationPanel(parent, project);
            if (pnl)
                arrayToFill.Add(pnl);
        }
    }
}

void PluginManager::AskPluginsForModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    for (unsigned int i = 0; i < m_Plugins.GetCount(); ++i)
    {
        cbPlugin* plug = m_Plugins[i]->plugin;
        if (plug && plug->IsAttached())
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

    // script plugins now
    wxArrayInt ids = ScriptBindings::ScriptPluginWrapper::CreateModuleMenu(type, menu, data);
    for (size_t i = 0; i < ids.GetCount(); ++i)
    {
        Connect(ids[i], -1, wxEVT_COMMAND_MENU_SELECTED,
                (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
                &PluginManager::OnScriptModuleMenu);
    }
}

void PluginManager::OnScriptMenu(wxCommandEvent& event)
{
    ScriptBindings::ScriptPluginWrapper::OnScriptMenu(event.GetId());
}

void PluginManager::OnScriptModuleMenu(wxCommandEvent& event)
{
    ScriptBindings::ScriptPluginWrapper::OnScriptModuleMenu(event.GetId());
}

int PluginManager::Configure()
{
    PluginsConfigurationDlg dlg(Manager::Get()->GetAppWindow());
    PlaceWindow(&dlg);
    return dlg.ShowModal();
}


