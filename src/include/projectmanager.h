/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#ifndef CB_PRECOMP
    #include "globals.h" // DEFAULT_WORKSPACE
#endif

#include <wx/event.h>
#include <wx/dynarray.h>
#include <wx/hashmap.h>
#include <wx/treectrl.h>

#include "cbexception.h"
#include "cbtreectrl.h"
#include "settings.h"
#include "manager.h"

// forward decls
class cbProject;
class EditorBase;
class ProjectFile;
class FilesGroupsAndMasks;
class cbWorkspace;
class cbAuiNotebook;

WX_DEFINE_ARRAY(cbProject*, ProjectsArray);
WX_DECLARE_HASH_MAP(cbProject*, ProjectsArray*, wxPointerHash, wxPointerEqual, DepsMap); // for project dependencies


class DLLIMPORT cbProjectManagerUI
{
    public:
        virtual ~cbProjectManagerUI() {}

        virtual cbAuiNotebook* GetNotebook() = 0;
        /** Retrieve a pointer to the project manager's tree (GUI).
          * @return A pointer to a wxTreeCtrl window.
          */
        virtual cbTreeCtrl* GetTree() = 0;
        /// Rebuild the project manager's tree.
        virtual void RebuildTree() = 0;
        /** Stop the tree control from updating.
          * @note This operation is accumulative. This means you have to call
          * UnfreezeTree() as many times as you 've called FreezeTree() for the
          * tree control to un-freeze (except if you call UnfreezeTree(true)).
          */
        virtual void FreezeTree() = 0;
        /** Le the tree control be updated again.
          * @param force If true the tree control is forced to un-freeze. Else it
          * depends on freeze-unfreeze balance (see note).
          * @note This operation is accumulative. This means you have to call
          * UnfreezeTree() as many times as you 've called FreezeTree() for the
          * tree control to un-freeze (except if you call UnfreezeTree(true)).
          */
        virtual void UnfreezeTree(bool force = false) = 0;
        /** Get the selection of the project manager's tree (GUI).
          * This must be used instead of tree->GetSelection() because the tree
          * has the wxTR_MULTIPLE style.
          * This usually returns the first item in the selection list, but
          * if there is a right-click popup menu then the user may have
          * selected several items and right-clicked on one, so return the
          * right-click item instead.
          * of the first
          * @return A wxTreeItemId of the selected tree item.
          */
        virtual wxTreeItemId GetTreeSelection() = 0;

        virtual void ShowFileInTree(ProjectFile &projectFile) = 0;

        virtual void UpdateActiveProject(cbProject *oldProject, cbProject *newProject, bool refresh) = 0;
        virtual void RemoveProject(cbProject *project) = 0;
        virtual void BeginLoadingWorkspace() = 0;
        virtual void CloseWorkspace() = 0;
        virtual void FinishLoadingProject(cbProject *project, bool newAddition, FilesGroupsAndMasks* fileGroups) = 0;
        virtual void FinishLoadingWorkspace(cbProject *activeProject, const wxString &workspaceTitle) = 0;

        /** Checks whether all projects are saved. If not, asks
          *  the user to save and saves accordingly.
          *  @return False if the user pressed cancel.
          *  Note: calls QueryCloseProject for all projects.
          */
        virtual bool QueryCloseAllProjects() = 0;

        /** Checks whether project is saved. If not, asks
          *  the user to save and saves accordingly.
          *  @return False if the user pressed cancel.
          *  Note: By default this asks the user if he should
          *  save any unmodified files in the project.
          */
        virtual bool QueryCloseProject(cbProject *proj, bool dontsavefiles = false) = 0;

        /** Asks user to save the workspace, projects and files
          * (Yes/No/cancel). If user pressed Yes, it saves accordingly.
          * @return False if the user pressed cancel; true otherwise.
          * After this function is called and returns true, it
          * is safe to close the workspace, all files and projects
          * without asking the user later.
          */
        virtual bool QueryCloseWorkspace() = 0;

        /** Utility function. Displays a single selection list of a project's
          * build targets to choose from.
          * @param project The project to use. If NULL, the active project is used.
          * @return The selected build target's index, or -1 if no build target was selected.
          */
        virtual int AskForBuildTargetIndex(cbProject* project = nullptr) = 0;
        /** Utility function. Displays a multiple selection list of a project's
          * build targets to choose from.
          * @param project The project to use. If NULL, the active project is used.
          * @return An integer array containing the selected build targets indices.
          * This array will be empty if no build targets were selected.
          */
        virtual wxArrayInt AskForMultiBuildTargetIndex(cbProject* project = nullptr) = 0;

        /** Displays a dialog to setup project dependencies.
          * @param base The project to setup its dependencies. Can be NULL (default) because
          * there's a project selection combo in the dialog.
          */
        virtual void ConfigureProjectDependencies(cbProject* base = nullptr) = 0;
};


/** @brief The entry point singleton for working with projects.
  *
  * This is the project manager class. It handles all open projects and workspaces.
  * It is through this class that you make requests about projects, like open
  * a project, save it, set the active project (if more than one are opened), etc.
  * To access a project, you must retrieve it from this class.
  *
  * As all managers, this is a singleton class which you retrieve by asking
  * the global Manager singleton.
  * E.g. Manager::Get()->GetProjectManager()
  */
class DLLIMPORT ProjectManager : public Mgr<ProjectManager>, public wxEvtHandler
{
        static bool s_CanShutdown;
    public:
        friend class Mgr<ProjectManager>;
        friend class Manager; // give Manager access to our private members

        // FIXME(obfuscated#): Extract these two in their own class
        cbProjectManagerUI& GetUI() { return *m_ui; }
        void SetUI(cbProjectManagerUI *ui);

        const FilesGroupsAndMasks* GetFilesGroupsAndMasks() const { return m_pFileGroups; }
        FilesGroupsAndMasks* GetFilesGroupsAndMasks() { return m_pFileGroups; }

        /// Can the app shutdown? (actually: is ProjectManager busy at the moment?)
        static bool CanShutdown() { return s_CanShutdown; }

        /** Retrieve the default path for new projects.
          * @return The default path for new projects. Contains trailing path separator.
          * @note This might be empty if not configured before...
          */
        wxString GetDefaultPath();
        /** Set the default path for new projects.
          * @note ProjectManager doesn't use this by itself. It's only doing the book-keeping.
          * Usually this is set/queried from project wizards...*/
        void SetDefaultPath(const wxString& path);
        /** Is this a valid project? (i.e. is it still open?)
          * @param project The project to check its validity.
          * @return True if the project is valid (i.e. still open), false if not.
          */
        bool IsProjectStillOpen(cbProject* project);
        /** Retrieve the active project. Most of the times, this is the function
          * you 'll be calling in ProjectManager.
          * @return A pointer to the active project.
          */
        cbProject* GetActiveProject(){ return m_pActiveProject; }
        /** Retrieve an array of all the opened projects. This is a standard
          * wxArray containing pointers to projects. Using this array you can
          * iterate through all the opened projects.
          * @return A pointer to the array containing the projects.
          */
        ProjectsArray* GetProjects(){ return m_pProjects; }
        /** Check if a project is open based on the project's filename.
          * @param filename The project's filename. Must be an absolute path.
          * @return A pointer to the project if it is open, or NULL if it is not.
          */
        cbProject* IsOpen(const wxString& filename);
        /** Set the active project. Use this function if you want to change the
          * active project.
          * @param project A pointer to the new active project.
          * @param refresh If true, refresh the project manager's tree, else do not refresh it.
          */
        void SetProject(cbProject* project, bool refresh = true);
        /** Load a project from disk. This function, internally, uses IsOpen()
          * so that the same project can't be loaded twice.
          * @param filename The project file's filename.
          * @param activateIt Active the project after loading.
          * @return If the function succeeds, a pointer to the newly opened project
          * is returned. Else the return value is NULL.
          */
        cbProject* LoadProject(const wxString& filename, bool activateIt = true);

        /** Reloads a project and tries to keep everything the same (project order, dependencies, active project)
          * @param project the project that will be reloaded, the pointer will be invalid after the call.
          */
        void ReloadProject(cbProject *project);

        /** Save a project to disk.
          * @param project A pointer to the project to save.
          * @return True if saving was succesful, false if not.
          */
        bool SaveProject(cbProject* project);
        /** Save a project to disk, asking for a filename.
          * @param project A pointer to the project to save.
          * @return True if saving was succesful, false if not.
          * @note A false return value doesn't necessarily mean failure. The user
          * might have cancelled the SaveAs dialog...
          */
        bool SaveProjectAs(cbProject* project);
        /** Save the active project to disk. Same as SaveProject(GetActiveProject()).
          * @return True if saving was succesful, false if not.
          */
        bool SaveActiveProject();
        /** Save the active project to disk, asking for a filename.
          * Same as SaveProjectAs(GetActiveProject()).
          * @return True if saving was succesful, false if not.
          * @note A false return value doesn't necessarily mean failure. The user
          * might have cancelled the SaveAs dialog...
          */
        bool SaveActiveProjectAs();
        /** Saves all projects to disk.
          * @return True if all projects were saved, false if even one save operation failed.
          */
        bool SaveAllProjects();
        /** Close a project.
          * @param project A pointer to the project to close.
          * @param dontsave Force not (!) saving the project on close.
          * @param refresh Force a refresh of the project tree after closing a project.
          * @return True if project was closed, false if not.
          */
        bool CloseProject(cbProject* project, bool dontsave = false, bool refresh = true);
        /** Close the active project. Same as CloseProject(GetActiveProject()).
          * @return True if project was closed, false if not.
          */
        bool CloseActiveProject(bool dontsave = false);
        /** Close all projects.
          * @return True if all projects were closed, false if even one close operation failed.
          */
        bool CloseAllProjects(bool dontsave = false);

        /** Create a new empty project.
          * @param filename the project's filename
          * @return A pointer to the new project if succesful, or NULL if not.
          * @note When the new project is created, if no filename parameter was supplied,
          * it asks the user where to save it.
          * If the user cancels the Save dialog, then NULL is returned from this function.
          */
        cbProject* NewProject(const wxString& filename = wxEmptyString);
        /** Add a file to a project. This function comes in two versions. This version,
          * expects a single build target index for the added file to belong to.
          * @param filename The file to add to the project.
          * @param project The project to add this file to. If NULL, the active project is used.
          * @param target The index of the project's build target to add this file.
          * @return The build target's index that this file was added to.
          * @note If the target index supplied is -1 then, if the project has exactly
          * one build target, the file is added to this single build target.
          * If the project has more than one build targets, a dialog appears so
          * that the user can select which build target this file should belong to.
          */
        int AddFileToProject(const wxString& filename, cbProject* project = nullptr, int target = -1);
        /** Add a file to a project. This function comes in two versions. This version,
          * expects an array of build target indices for the added file to belong to.
          * @param filename The file to add to the project.
          * @param project The project to add this file to. If NULL, the active project is used.
          * @param targets The array of the project's build targets indices to add this file.
          * @return The number of build targets this file was added to.
          * @note If the targets array is empty then, if the project has exactly
          * one build target, the file is added to this single build target.
          * If the project has more than one build targets, a dialog appears so
          * that the user can select which build targets (multiple) this file should belong to.\n
          * Also note than when this function returns, the targets array will contain
          * the user-selected build targets.
          */
        int AddFileToProject(const wxString& filename, cbProject* project, wxArrayInt& targets);
        /** Add multiple files to a project. This function comes in two versions. This version,
          * expects a single build target index for the added files to belong to.
          * @param filelist The files to add to the project.
          * @param project The project to add these files to. If NULL, the active project is used.
          * @param target The index of the project's build target to add these files.
          * @return The build target's index that these files were added to.
          * @note If the target index supplied is -1 then, if the project has exactly
          * one build target, the files are added to this single build target.
          * If the project has more than one build targets, a dialog appears so
          * that the user can select which build target these files should belong to.
          */
        int AddMultipleFilesToProject(const wxArrayString& filelist, cbProject* project, int target = -1);
        /** Add multiple files to a project. This function comes in two versions. This version,
          * expects an array of build target indices for the added files to belong to.
          * @param filelist The files to add to the project.
          * @param project The project to add this file to. If NULL, the active project is used.
          * @param targets The array of the project's build targets indices to add this file.
          * @return The number of build targets these files were added to.
          * @note If the targets array is empty then, if the project has exactly
          * one build target, the files are added to this single build target.
          * If the project has more than one build targets, a dialog appears so
          * that the user can select which build targets (multiple) these files should belong to.\n
          * Also note than when this function returns, the targets array will contain
          * the user-selected build targets.
          */
        int AddMultipleFilesToProject(const wxArrayString& filelist, cbProject* project, wxArrayInt& targets);
        /** Remove a file from a project.
          * @param pfile The file to remove from the project.
          * @param project The project to remove this file from. If NULL, the active project is used.
          */
        void RemoveFileFromProject(ProjectFile* pfile, cbProject* project);
        /** Load a workspace.
          * @param filename The workspace to open.
          * @return True if the workspace loads succefully, false if not.
          */
        bool LoadWorkspace(const wxString& filename = DEFAULT_WORKSPACE);
        /** Save the open workspace.
          * @return True if the workspace is saved succefully, false if not.
          */
        bool SaveWorkspace();
        /** Save the open workspace under a different filename.
          * @param filename The workspace to save.
          * @return True if the workspace is saved succefully, false if not.
          */
        bool SaveWorkspaceAs(const wxString& filename);
        /** Close the workspace.
          * @return True if the workspace closes, false if not (the user is asked to save
          * the workspace, if it is modified)
          */
        bool CloseWorkspace();

        /** Check if the project manager is loading a project.
          * @return True if it's loading a project, false otherwise
          */
        bool IsLoadingProject();
        /** Check if the project manager is loading a workspace.
          * @return True if it's loading a workspace, false otherwise
          */
        bool IsLoadingWorkspace();
        /** Check if the project manager is loading a project/workspace.
          * @return True if it's loading a project/workspace, false otherwise
          */
        bool IsLoading();
        /** Check if the project manager is closing a project.
          * @return True if it's closing a project, false otherwise
          */
        bool IsClosingProject();
        /** Check if the project manager is closing a workspace.
          * @return True if it's closing a workspace, false otherwise
          */
        bool IsClosingWorkspace();
        /** Check if the project manager is loading/closing a project/workspace.
          * @return True if it is loading/closing something, false otherwise
          */
        bool IsLoadingOrClosing();
        /** For use with plugins. Checks whether files in the project MAY be processed.
          * This function has been made static for your convenience :)
          * @return true if Loading,closing a project/workspace, or if the app is shutting down. False otherwise
          */
        static bool IsBusy();
        /** Get the current workspace filename.
          * @return The current workspace filename.
          */
        cbWorkspace* GetWorkspace();

        /** @brief Adds a project as a dependency of another project.
          * Projects inside workspaces allow you to set dependencies between them.
          * When project A depends on project B, this means that before building
          * project A, project B will be built because it obviously generates code
          * that project A depends upon.
          * @param base The project to set a dependency for.
          * @param dependsOn the project that must be built before @c base project.
          */
        bool AddProjectDependency(cbProject* base, cbProject* dependsOn);
        /** @brief Removes a project dependency.
          * @see AddProjectDependency()
          * @param base The project to remove a dependency from.
          * @param doesNotDependOn The project that is to stop being a dependency of project @c base.
          */
        void RemoveProjectDependency(cbProject* base, cbProject* doesNotDependOn);
        /** @brief Removes all dependencies from project @c base.
          * @see AddProjectDependency()
          * @param base The project to remove all dependencies from.
          */
        void ClearProjectDependencies(cbProject* base);
        /** @brief Removes the project @c base from being a dependency of any other project.
          * @see AddProjectDependency()
          * @param base The project to remove from all dependencies.
          */
        void RemoveProjectFromAllDependencies(cbProject* base);
        /** @brief Get the array of projects @c base depends on.
          * @param base The project to get its dependencies.
          * @return An array of project dependencies, or NULL if no dependencies are set for @c base.
          */
        const ProjectsArray* GetDependenciesForProject(cbProject* base);
        /** Checks for circular dependencies between @c base and @c dependsOn.
          * @return True if circular dependency is detected, false if it isn't.
          */
        bool CausesCircularDependency(cbProject* base, cbProject* dependsOn);

        /** Sends message to the plugins that the workspace has been changed */
        void WorkspaceChanged();

        /** Begins the project loading process. Only to be used by code that needs it (e.g. project importers).
          * @return True on success, false on failure.
          * @note This call *must* be "closed" by a call to EndLoadingProject()!
          */
        bool BeginLoadingProject();

        /** Ends the project loading process. Only to be used by code that needs it (e.g. project importers).
          * @param project The loaded project.
          * @note A call to BeginLoadingProject() must have preceded.
          */
        void EndLoadingProject(cbProject* project);

        /** Begins the workspace loading process. Only to be used by code that needs it (e.g. workspace importers).
          * @return True on success, false on failure.
          * @note This call *must* be "closed" by a call to EndLoadingWorkspace()!
          */
        bool BeginLoadingWorkspace();

        /** Ends the workspace loading process. Only to be used by code that needs it (e.g. workspace importers).
          * @note A call to BeginLoadingWorkspace() must have preceded.
          */
        void EndLoadingWorkspace();

        ProjectManager& operator=(cb_unused const ProjectManager& rhs) // prevent assignment operator
        {
            cbThrow(_T("Can't assign a ProjectManager* !!!"));
            return *this;
        }

        /** If a plugin runs the project executable, should not be able to run it too */
        void SetIsRunning(cbPlugin *plugin);
        cbPlugin* GetIsRunning() const;

        /** Return the project which has the file in it, also return the pointer to the ProjectFile object. */
        cbProject* FindProjectForFile(const wxString& file, ProjectFile **resultFile,
                                      bool isRelative, bool isUnixFilename);

    private:
        ProjectManager(cb_unused const ProjectManager& rhs); // prevent copy construction

        ProjectManager();
        ~ProjectManager();
        void OnAppDoneStartup(CodeBlocksEvent& event);
        int  DoAddFileToProject(const wxString& filename, cbProject* project, wxArrayInt& targets);

        cbProjectManagerUI *m_ui;
        cbProject*           m_pActiveProject;
        cbProject*           m_pProjectToActivate;
        ProjectsArray*       m_pProjects;
        DepsMap              m_ProjectDeps;
        cbWorkspace*         m_pWorkspace;
        FilesGroupsAndMasks* m_pFileGroups;
        bool                 m_IsLoadingProject;
        bool                 m_IsLoadingWorkspace;
        bool                 m_IsClosingProject;
        bool                 m_IsClosingWorkspace;
        wxString             m_InitialDir;
        bool                 m_CanSendWorkspaceChanged;
        cbPlugin*            m_RunningPlugin;

        DECLARE_EVENT_TABLE()
};

#endif // PROJECTMANAGER_H

