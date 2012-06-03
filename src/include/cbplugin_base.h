/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef CBPLUGIN_BASE_H
#define CBPLUGIN_BASE_H

#include <wx/arrstr.h>
#include <wx/event.h>
#include <wx/intl.h>
#include <wx/string.h>

#include "settings.h" // build settings
#include "globals_base.h"
#include "manager.h"
#include "pluginmanager.h"
#include "prep.h"

#ifdef __WXMSW__
    #ifndef PLUGIN_EXPORT
        #ifdef EXPORT_LIB
            #define PLUGIN_EXPORT __declspec (dllexport)
        #else // !EXPORT_LIB
            #ifdef BUILDING_PLUGIN
                #define PLUGIN_EXPORT __declspec (dllexport)
            #else // !BUILDING_PLUGIN
                #define PLUGIN_EXPORT __declspec (dllimport)
            #endif // BUILDING_PLUGIN
        #endif // EXPORT_LIB
    #endif // PLUGIN_EXPORT
#else
    #define PLUGIN_EXPORT
#endif

// this is the plugins SDK version number
// it will change when the SDK interface breaks
#define PLUGIN_SDK_VERSION_MAJOR 1
#define PLUGIN_SDK_VERSION_MINOR 13
#define PLUGIN_SDK_VERSION_RELEASE 3

#ifdef CB_FOR_CONSOLE
    #define CB_PLUGIN_BASE cbPlugin
#else // #ifdef CB_FOR_CONSOLE
    #define CB_PLUGIN_BASE cbPluginBase
#endif //#ifdef CB_FOR_CONSOLE


// class decls
class cbProject;
class Compiler;
class CompileTargetBase;
struct PluginInfo;
class ProjectBuildTarget;

// Define basic groups for plugins' configuration.
static const int cgCompiler         = 0x01; ///< Compiler related.
static const int cgUnknown          = 0x10; ///< Unknown. This will be probably grouped with cgContribPlugin.

/** @brief Base class for plugins
  * This is the most basic class a plugin must descend
  * from.
  * cbPlugin descends from wxEvtHandler, so it provides its methods as well...
  * \n \n
  * It's not enough to create a new plugin. You must also provide a resource
  * zip file containing a file named "manifest.xml". Check the manifest.xml
  * file of existing plugins to see how to create one (it's ultra-simple).
  */
class PLUGIN_EXPORT CB_PLUGIN_BASE : public wxEvtHandler
{
    public:
        /** In default cbPlugin's constructor the associated PluginInfo structure
          * is filled with default values. If you inherit from cbPlugin, you
          * should fill the m_PluginInfo members with the appropriate values.
          */
        CB_PLUGIN_BASE();

        /** cbPlugin destructor. */
        virtual ~CB_PLUGIN_BASE();

        /** The plugin must return its type on request. */
        virtual PluginType GetType() const { return m_Type; }

        /** See whether this plugin is attached or not. A plugin should not perform
          * any of its tasks, if not attached...
          * @note This function is *not* virtual.
          * @return Returns true if it attached, false if not.
          */
        bool IsAttached() const { return m_IsAttached; }

        /** See whether this plugin can be detached (unloaded) or not.
          * This function is called usually when the user requests to
          * uninstall or disable a plugin. Before disabling/uninstalling it, Code::Blocks
          * asks the plugin if it can be detached or not. In other words, it checks
          * to see if it can be disabled/uninstalled safely...
          * @par
          * A plugin should return true if it can be detached at this moment, false if not.
          * @return The default implementation returns true.
          */
        virtual bool CanDetach() const { return true; }
    protected:
        /** Any descendent plugin should override this virtual method and
          * perform any necessary initialization. This method is called by
          * Code::Blocks (PluginManager actually) when the plugin has been
          * loaded and should attach in Code::Blocks. When Code::Blocks
          * starts up, it finds and <em>loads</em> all plugins but <em>does
          * not</em> activate (attaches) them. It then activates all plugins
          * that the user has selected to be activated on start-up.\n
          * This means that a plugin might be loaded but <b>not</b> activated...\n
          * Think of this method as the actual constructor...
          */
        virtual void OnAttach(){}

        /** Any descendent plugin should override this virtual method and
          * perform any necessary de-initialization. This method is called by
          * Code::Blocks (PluginManager actually) when the plugin has been
          * loaded, attached and should de-attach from Code::Blocks.\n
          * Think of this method as the actual destructor...
          * @param appShutDown If true, the application is shutting down. In this
          *         case *don't* use Manager::Get()->Get...() functions or the
          *         behaviour is undefined...
          */
        virtual void OnRelease(bool /*appShutDown*/){}

        /** This method logs a "Not implemented" message and is provided for
          * convenience only.
          */
        virtual void NotImplemented(const wxString& log) const;

        /** Holds the plugin's type. Set in the default constructor. */
        PluginType m_Type;

        /** Holds the "attached" state. */
        bool m_IsAttached;

    private:
        friend class PluginManager; // only the plugin manager has access here

        /** Attach is <b>not</b> a virtual function, so you can't override it.
          * The default implementation hooks the plugin to Code::Block's
          * event handling system, so that the plugin can receive (and process)
          * events from Code::Blocks core library. Use OnAttach() for any
          * initialization specific tasks.
          * @see OnAttach()
          */
        void Attach();

        /** Release is <b>not</b> a virtual function, so you can't override it.
          * The default implementation un-hooks the plugin from Code::Blocks's
          * event handling system. Use OnRelease() for any clean-up specific
          * tasks.
          * @param appShutDown If true, the application is shutting down. In this
          *         case *don't* use Manager::Get()->Get...() functions or the
          *         behaviour is undefined...
          * @see OnRelease()
          */
        void Release(bool appShutDown);
};

/** @brief Base class for compiler plugins
  *
  * This plugin type must offer some pre-defined build facilities, on top
  * of the generic plugin's.
  */
class PLUGIN_EXPORT cbCompilerPlugin: public cbPlugin
{
    public:
        cbCompilerPlugin();

        /** @brief Run the project/target.
          *
          * Running a project means executing its build output. Of course
          * this depends on the selected build target and its type.
          *
          * @param target The specific build target to "run". If NULL, the plugin
          * should ask the user which target to "run" (except maybe if there is
          * only one build target in the project).
          */
        virtual int Run(ProjectBuildTarget* target = 0L) = 0;

        /** Same as Run(ProjectBuildTarget*) but with a wxString argument. */
        virtual int Run(const wxString& target) = 0;

        /** @brief Clean the project/target.
          *
          * Cleaning a project means deleting any files created by building it.
          * This includes any object files, the binary output file, etc.
          *
          * @param target The specific build target to "clean". If NULL, it
          * cleans all the build targets of the current project.
          */
        virtual int Clean(ProjectBuildTarget* target = 0L) = 0;

        /** Same as Clean(ProjectBuildTarget*) but with a wxString argument. */
        virtual int Clean(const wxString& target) = 0;

        /** @brief DistClean the project/target.
          *
          * DistClean will typically remove any config files
          * and anything else that got created as part of
          * building a software package.
          *
          * @param target The specific build target to "distclean". If NULL, it
          * cleans all the build targets of the current project.
          */
        virtual int DistClean(ProjectBuildTarget* target = 0L) = 0;

        /** Same as DistClean(ProjectBuildTarget*) but with a wxString argument. */
        virtual int DistClean(const wxString& target) = 0;

        /** @brief Build the project/target.
          *
          * @param target The specific build target to build. If NULL, it
          * builds all the targets of the current project.
          */
        virtual int Build(ProjectBuildTarget* target = 0L) = 0;

        /** Same as Build(ProjectBuildTarget*) but with a wxString argument. */
        virtual int Build(const wxString& target) = 0;

        /** @brief Rebuild the project/target.
          *
          * Rebuilding a project is equal to calling Clean() and then Build().
          * This makes sure that all compilable files in the project will be
          * compiled again.
          *
          * @param target The specific build target to rebuild. If NULL, it
          * rebuilds all the build targets of the current project.
          */
        virtual int Rebuild(ProjectBuildTarget* target = 0L) = 0;

        /** Same as Rebuild(ProjectBuildTarget*) but with a wxString argument. */
        virtual int Rebuild(const wxString& target) = 0;

        /** @brief Build all open projects.
          * @param target If not empty, the target to build in each project. Else all targets.
          */
        virtual int BuildWorkspace(const wxString& target = wxEmptyString) = 0;

        /** @brief Rebuild all open projects.
          * @param target If not empty, the target to rebuild in each project. Else all targets.
          */
        virtual int RebuildWorkspace(const wxString& target = wxEmptyString) = 0;

        /** @brief Clean all open projects.
          * @param target If not empty, the target to clean in each project. Else all targets.
          */
        virtual int CleanWorkspace(const wxString& target = wxEmptyString) = 0;

        /** @brief Compile a specific file.
          *
          * @param file The file to compile (must be a project file!)
          */
        virtual int CompileFile(const wxString& file) = 0;

        /** @brief Abort the current build process. */
        virtual int KillProcess() = 0;

        /** @brief Is the plugin currently compiling? */
        virtual bool IsRunning() const = 0;

        /** @brief Get the exit code of the last build process. */
        virtual int GetExitCode() const = 0;

        virtual int Configure() { return 0; }
        /** @brief Display configuration dialog.
          *
          * @param project The selected project (can be NULL).
          * @param target The selected target (can be NULL).
          */
        virtual int Configure(cbProject* project, ProjectBuildTarget* target = 0L) = 0;
    private:
};

/** @brief Base class for tool plugins
  *
  * This plugin is automatically managed by Code::Blocks, so the inherited
  * functions to build menus/toolbars are hidden.
  *
  * Tool plugins are automatically added under the "Plugins" menu.
  */
class PLUGIN_EXPORT cbToolPlugin : public cbPlugin
{
    public:
        cbToolPlugin();

        /** @brief Execute the plugin.
          *
          * This is the only function needed by a cbToolPlugin.
          * This will be called when the user selects the plugin from the "Plugins"
          * menu.
          */
        virtual int Execute() = 0;
};

/** @brief Plugin registration object.
  *
  * Use this class to register your new plugin with Code::Blocks.
  * All you have to do is instantiate a PluginRegistrant object.
  * @par
  * Example code to use in one of your plugin's source files (supposedly called "MyPlugin"):
  * @code
  * namespace
  * {
  *     PluginRegistrant<MyPlugin> registration("MyPlugin");
  * }
  * @endcode
  */
template<class T> class PluginRegistrant
{
    public:
        /// @param name The plugin's name.
        PluginRegistrant(const wxString& name)
        {
            Manager::Get()->GetPluginManager()->RegisterPlugin(name, // plugin's name
                                                                &CreatePlugin, // creation
                                                                &FreePlugin, // destruction
                                                                &SDKVersion); // SDK version
        }

        static cbPlugin* CreatePlugin()
        {
            return new T;
        }

        static void FreePlugin(cbPlugin* plugin)
        {
            delete plugin;
        }

        static void SDKVersion(int* major, int* minor, int* release)
        {
            if (major) *major = PLUGIN_SDK_VERSION_MAJOR;
            if (minor) *minor = PLUGIN_SDK_VERSION_MINOR;
            if (release) *release = PLUGIN_SDK_VERSION_RELEASE;
        }
};

#endif // CBPLUGIN_BASE_H
