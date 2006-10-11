#ifndef WXSRESOURCE_H
#define WXSRESOURCE_H

#include "wxseditor.h"
#include "wxscodinglang.h"
#include "wxsresourcetree.h"

class wxsProject;

/** \brief Class used to manage basic resource information
 *
 * This class represents resource inside project (or external one).
 * It should keep only basic resource information but not resource data.
 * It's stored inside internal wxsEditor's structures.
 */
class wxsResource: public wxObject
{
    DECLARE_CLASS(wxsResource)
    public:

        /** \brief Ctor
         *  \param Owher project owning resource
         *  \param ResourceName name of resource (f.ex. class name)
         *  \param ResourceType name of resource type (f.ex. wxDialog)
         *  \param GUI anme of gui using this resource, put empty string if this is universal resource like bitmap file
         *  \param Language coding language used for this resource
         */
        wxsResource(wxsProject* Owner,const wxString& ResourceName,const wxString& ResourceType,const wxString& GUI,wxsCodingLang Language=wxsCPP);

        /** \brief dctor */
        virtual ~wxsResource();

        /** \brief Getting resource type */
        inline const wxString& GetResourceType() { return m_ResourceType; }

        /** \brief Getting GUI */
        inline const wxString& GetGUI() { return m_GUI; }

        /** \brief Getting coding language used inside this resource */
        inline wxsCodingLang GetLanguage() { return m_Language; }

        /** \brief Getting resource name */
        inline const wxString& GetResourceName() { return m_ResourceName; }

        /** \brief Changing resource's name */
        inline void SetResourceName(const wxString& NewName) { m_ResourceName = NewName; }

        /** \brief Opening editor, if one exists, it will be activated in set of all C::B editors */
        void EditOpen();

        /** \brief Closing editor if exist */
        void EditClose();

        /** \brief Checking if editor for this resource is opened */
        inline bool IsEditorOpened() { return m_Editor!=NULL; }

        /** \brief Getting pointer to editor or NULL if there's none */
        inline wxsEditor* GetEditor() { return m_Editor; }

        /** \brief Getting tree item id in resource browser */
        inline const wxsResourceItemId& GetTreeItemId() { return m_TreeItemId; }

        /** \brief Creating entry in resoruce browser */
        void BuildTreeEntry(const wxsResourceItemId& Parent);

        /** \brief Getting name of declaration file
          * \note this function is only a wrapper to OnGetDeclarationFile (to hold ocnsistency of functions to override)
          */
        inline wxString GetDeclarationFile() { return OnGetDeclarationFile(); }

        /** \brief Getting code creating this resource
         *  \note this function is only a wrapper to OnGetAppBuildingCode (to hold ocnsistency of functions to override)
         */
        inline wxString GetAppBuildingCode() { return OnGetAppBuildingCode(); }

    protected:

        /** \brief Function called when there's need to create new editor
         *
         * This function is called when there's need to open editor and when
         * editor has not been created yet (or has been closed), so there's no
         * need to check whether editor is opened or not.
         * \return Pointer to wxsEditor class
         */
        virtual wxsEditor* OnCreateEditor() = 0;

        /** \brief Function called when reading resource configuration from .cbp file
         *
         * \param Node - Xml node in cbp file defined for this resource only.
         * It's in form:
         *  \code <{Resource_Type} name={Resource_Name} ... /> \endcode
         * where {Resource_Type} is type of resource returned from GetResourceType() function
         * and {Resource_Name} is name returned from GetResourceName(). Name attribute is always
         * present.
         */
        virtual bool OnReadConfig(const TiXmlElement* Node) = 0;

        /** \brief Function called when writing resource configuration to .cbp file
         *
         * \param Node - Xml node where all data should be written to. "name" attribute
         *        and Element's value should not be overwritten.
         */
        virtual bool OnWriteConfig(TiXmlElement* Node) = 0;

        /** \brief function called to check if this resource handles given file
         *
         * This function is called from mime plugin to check if opening some
         * file should open editor of this resource. If true, EditOpen is called
         * automatically.
         *
         * \return true when opening FileName should result in opening this resource's
         *         editor.
         */
        virtual bool OnCanHandleFile(const wxString& FileName) = 0;

        /** \brief Function which should return name of file
         *         with resource declaration (f.ex. in c++ it should be .h file)
         * \return File name relative to project's CBP file path
         */
        virtual wxString OnGetDeclarationFile() { return wxEmptyString; }

        /** \brief Function which should build this resource in application initializing code */
        virtual wxString OnGetAppBuildingCode() { return wxEmptyString; }

    private:

        /** \brief Function called from wxsEditor just before it's deletion */
        void EditorClosed();

        const wxString& m_ResourceType;
        wxString m_ResourceName;
        const wxString& m_GUI;
        wxsProject* m_Owner;
        wxsEditor* m_Editor;
        wxsResourceItemId m_TreeItemId;
        const wxsCodingLang m_Language;

        friend wxsEditor::~wxsEditor();
        friend class wxsProject;
};

#endif
