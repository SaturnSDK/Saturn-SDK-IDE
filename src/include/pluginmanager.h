/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "pluginmanager_base.h"

#ifndef CB_FOR_CONSOLE

//forward decls
class cbConfigurationPanel;
class wxMenuBar;
class wxMenu;
class FileTreeData;

WX_DEFINE_ARRAY(cbConfigurationPanel*, ConfigurationPanelsArray);

/**
 * PluginManager manages plugins.
 *
 * There are two plugin types: binary and scripted.
 *
 * Binary plugins are dynamically loaded shared libraries (dll/so) which
 * can do pretty much anything with the SDK.
 *
 * Script plugins are more lightweight and are very convenient for
 * smaller scale/functionality plugins.
 */
class DLLIMPORT PluginManager : public PluginManagerBase, public Mgr<PluginManager>, public wxEvtHandler
{
    public:
        friend class Mgr<PluginManager>;
        friend class Manager; // give Manager access to our private members
        void CreateMenu(wxMenuBar* menuBar);
        void ReleaseMenu(wxMenuBar* menuBar);

        int ConfigurePlugin(const wxString& pluginName);

        bool InstallPlugin(const wxString& pluginName, bool forAllUsers = true, bool askForConfirmation = true);
        bool UninstallPlugin(cbPlugin* plugin, bool removeFiles = true);
        bool ExportPlugin(cbPlugin* plugin, const wxString& filename);
        void AskPluginsForModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0);
        void GetConfigurationPanels(int group, wxWindow* parent, ConfigurationPanelsArray& arrayToFill);
        void GetProjectConfigurationPanels(wxWindow* parent, cbProject* project, ConfigurationPanelsArray& arrayToFill);
        int Configure();
        void RegisterCCFileExts(const wxString& pluginName, const std::set<wxString>& fileExts);
    private:
        PluginManager();
        virtual ~PluginManager(){};

        void OnScriptMenu(wxCommandEvent& event);
        void OnScriptModuleMenu(wxCommandEvent& event);

        bool ExtractFile(const wxString& bundlename,
                        const wxString& src_filename,
                        const wxString& dst_filename,
                        bool isMandatory = true);


        DECLARE_EVENT_TABLE()
};

#endif // #ifndef CB_FOR_CONSOLE

#endif // PLUGINMANAGER_H
