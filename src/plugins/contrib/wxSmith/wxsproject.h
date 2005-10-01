#ifndef WXSPROJECT_H
#define WXSPROJECT_H

/******************************************************************************/
/* Predefines                                                                 */
/******************************************************************************/

class wxSmith;
class wxsWidget;
class wxsWindowEditor;
class wxsResource;
class wxsDialogRes;
class wxsFrameRes;
class wxsPanelRes;
class wxsWindowRes;

/******************************************************************************/

#include <cbproject.h>
#include <vector>
#include <tinyxml/tinyxml.h>
#include "wxsresourcetree.h"

/** Directory relative to project's main file where wxSmith internal
 *  data should be stored.
 */
static const wxString wxSmithSubDirectory(_T("wxsmith"));

/** Name of main configuration for wxSmith */
static const wxString wxSmithMainConfigFile(_T("wxsmith.cfg"));

/** This class integrates current project with wxsmith.
 *
 * This class is responsible for:
 *  - adding wxSmith abilities into wxisting project
 *  - removing wxSmith elements from project
 *  - parsing project files (searching for widgest)
 *  - anything needed ? add here
 *  .
 */
class wxsProject
{

	public:

        /* Some strructures which could be used publically */

		wxsProject();
		virtual ~wxsProject();

        /** Type defining current state of integration with C::B project */
        enum IntegrationState
        {
            NotBinded = 0,  ///< This wxsProjecyt has not been integrated with C::B project
            NotWxsProject,  ///< Associated C::B project is not using wxSmith abilities
            Integrated      ///< Associated C::B project is fully integrated with wxSmith project
        };

        /** Getting current type of integration with C::B project */
        inline IntegrationState GetIntegration() { return Integration; }

        /** Connecting to existing project */
        virtual IntegrationState BindProject(cbProject* Project);

        /** Forcing project to save it's main configuration file */
        virtual void SaveProject();

        /** Getting C::B project */
        inline cbProject* GetCBProject() { return Project; }

        /** Generating full name of internal wxSmith file */
        virtual wxString GetInternalFileName(const wxString& FileName);

        /** Getting full name of project's file */
        virtual wxString GetProjectFileName(const wxString& FileName);

        /** Adding wxSmith configuration to this project */
        virtual bool AddSmithConfig();

        /** Searching for resource with given name */
        wxsResource* FindResource(const wxString& Name);

        /** Adding previously created dialog resource */
        void AddDialog(wxsDialogRes* Dialog);

        /** Adding previously created frame resource */
        void AddFrame(wxsFrameRes* Frame);

        /** Adding previously created panel resource */
        void AddPanel(wxsPanelRes* Panel);

        /** Spreading eventg to all resource editors opened inside this project */
        void SendEventToEditors(wxEvent& event);

        /** Deleting dialog resource from project */
        void DeleteDialog(wxsDialogRes* Resource);

        /** Deleting frame resource from project */
        void DeleteFrame(wxsFrameRes* Resource);

        /** Deleting panel resource from project */
        void DeletePanel(wxsPanelRes* Resource);
        
        /** Getting modified state */
        bool GetModified() { return Modified; }

    protected:

        /** Function loading all data from xml source */
        virtual bool LoadFromXml(TiXmlNode* MainNode);

        /** Function generating xml data */
        virtual TiXmlDocument* GenerateXml();

        /** Function loading dialog resource from xml tree */
        void AddDialogResource(
            const wxString& FileName,
            const wxString& ClassName,
            const wxString& SourceName,
            const wxString& HeaderName,
            const wxString& XrcName);

        /** Function loading frame resource from xml tree */
        void AddFrameResource(
            const wxString& FileName,
            const wxString& ClassName,
            const wxString& SourceName,
            const wxString& HeaderName,
            const wxString& XrcName);

        /** Function loading panel resource from xml tree */
        void AddPanelResource(
            const wxString& FileName,
            const wxString& ClassName,
            const wxString& SourceName,
            const wxString& HeaderName,
            const wxString& XrcName);

	private:

        /** Adding resourcee of given type */
        void AddWindowResource(
            const wxString& FileName,
            const wxString& ClassName,
            const wxString& SourceName,
            const wxString& HeaderName,
            const wxString& XrcName,
            const wxString& Type);

        /** Function building tree for resources in this project */
        void BuildTree(wxTreeCtrl* Tree,wxTreeItemId WhereToAdd);

        /** checling if given file is insided current project */
        bool CheckProjFileExists(const wxString& FileName);

        /** Function clearing project structures */
        inline void Clear();
        
        /** Changing modified state */
        inline void SetModified(bool modified) { Modified = modified; }

        IntegrationState Integration;   ///< Current integration state
        wxFileName ProjectPath;         ///< Base Directory of C::B project (where project filr is stored)
        wxFileName WorkingPath;         ///< Directory where wxSmith's private data will be stored
        cbProject* Project;             ///< Project associated with project
        wxTreeItemId TreeItem;          ///< Tree item where project's resources are kept
        wxTreeItemId DialogId;          ///< Tree item for dialog resourcecs
        wxTreeItemId FrameId;           ///< Tree item for frame resources
        wxTreeItemId PanelId;           ///< Tree item for panel resources

        /* Resources */

        typedef std::vector<wxsDialogRes*> DialogListT;
        typedef std::vector<wxsFrameRes*>  FrameListT;
        typedef std::vector<wxsPanelRes*>  PanelListT;

        typedef DialogListT::iterator DialogListI;
        typedef FrameListT ::iterator FrameListI;
        typedef PanelListT ::iterator PanelListI;


        DialogListT Dialogs;
        FrameListT Frames;
        PanelListT Panels;

        bool DuringClear;               ///< Set to true when inside Clear call
        bool Modified;                  ///< Set to true when there was any change inside wxSmith project

        friend class wxsWindowRes;
};


#endif // WXSPROJECT_H
