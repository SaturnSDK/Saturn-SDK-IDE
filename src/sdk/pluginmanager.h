#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <wx/dynarray.h>
#include "settings.h"
#include "sdk_events.h"
#include "sanitycheck.h"

//forward decls
struct PluginInfo;
class cbPlugin;
class wxDynamicLibrary;
class wxMenuBar;
class wxMenu;
class wxToolBar;

struct PluginElement
{
    wxString name;
    wxString fileName;
    wxDynamicLibrary* library;
    cbPlugin* plugin;
};

WX_DEFINE_ARRAY(PluginElement*, PluginElementsArray);
WX_DEFINE_ARRAY(cbPlugin*, PluginsArray);

// MOVED FROM main.cpp:
WX_DECLARE_HASH_MAP(int, wxString, wxIntegerHash, wxIntegerEqual, PluginIDsMap);

/*
 * No description
 */
class DLLIMPORT PluginManager
      : public wxEvtHandler
{
    public:
        friend class Manager; // give Manager access to our private members
        void CreateMenu(wxMenuBar* menuBar);
        void ReleaseMenu(wxMenuBar* menuBar);
        int ScanForPlugins(const wxString& path);
        cbPlugin* LoadPlugin(const wxString& pluginName);
        void LoadAllPlugins();
        void UnloadAllPlugins();
        int ExecutePlugin(const wxString& pluginName);
        int ConfigurePlugin(const wxString& pluginName);
        const PluginInfo* GetPluginInfo(const wxString& pluginName);
        PluginElementsArray& GetPlugins() { return m_Plugins; }
        cbPlugin* FindPluginByName(const wxString& pluginName);
        cbPlugin* FindPluginByFileName(const wxString& pluginFileName);
        PluginsArray GetToolOffers();
        PluginsArray GetMimeOffers();
        PluginsArray GetCompilerOffers();
        PluginsArray GetCodeCompletionOffers();
        void AskPluginsForModuleMenu(const ModuleType type, wxMenu* menu, const wxString& arg);
    
        void NotifyPlugins(CodeBlocksEvent& event);
        void OnPluginLoaded(CodeBlocksEvent& event,wxMenuBar *mb,wxToolBar *tb);
    
        void BuildAllPluginsMenus(wxMenuBar *mbar);
        void LoadPluginsToolBars(wxToolBar *toolBar);
        void LoadPluginsMenus(wxMenuBar *menuBar);
    
        void OnPluginsExecuteMenu(wxCommandEvent& event);
        void OnPluginSettingsMenu(wxCommandEvent& event);
        void OnHelpPluginMenu(wxCommandEvent& event);
        void DoAddPlugin(cbPlugin* plugin,wxMenuBar *mbar,wxToolBar *tbar);
    
        // sets/resets reconfiguring flag - returns previous state
        bool ReconfigurePlugins(bool flag);
        bool isReconfiguringPlugins();
        int Configure();
        void DoConfigDialog();
    protected:
        static void BuildPluginToolBar(cbPlugin *plug,wxToolBar *toolBar);
        static void BuildPluginMenu(cbPlugin *plug,wxMenuBar *menuBar);
        void AddPluginInMenus(wxMenu* menu, cbPlugin* plugin, wxObjectEventFunction callback, int pos = -1);
        void AddPluginInPluginsMenu(cbPlugin* plugin);
        void AddPluginInSettingsMenu(cbPlugin* plugin);
        void AddPluginInHelpPluginsMenu(cbPlugin* plugin);
    
        bool m_ReconfiguringPlugins;
    
    private:
        static PluginManager* Get();
        static void Free();
        PluginManager();
        ~PluginManager();
        wxMenu *m_PluginsMenu,*m_SettingsMenu,*m_HelpPluginsMenu;
        PluginsArray DoGetOffersFor(PluginType type);
        PluginElementsArray m_Plugins;
        // MOVED (from main.h) :
        PluginIDsMap m_PluginIDsMap;
        DECLARE_SANITY_CHECK
};

#endif // PLUGINMANAGER_H

