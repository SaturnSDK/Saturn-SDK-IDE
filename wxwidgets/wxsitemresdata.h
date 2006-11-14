#ifndef WXSITEMRESDATA_H
#define WXSITEMRESDATA_H

#include "wxsitem.h"
#include "wxsitemres.h"
#include "wxsitemundobuffer.h"

/** \brief Class holding data for item resources and operating on it */
class wxsItemResData
{
    public:

        /** \brief Ctor */
        wxsItemResData(wxsItemRes* Resource);

        /** \brief Dctor */
        virtual ~wxsItemResData();

        /** \brief Loading resource data */
        bool LoadData();

        /** \brief Getting root item
         *  \return pointer to item on success, NULL when data wasn't initialized properly
         */
        inline wxsItem* GetRootItem() { return m_RootItem; }

        /* ************************ */
        /*  Undo buffer operations  */
        /* ************************ */

		/** \brief Checking if can Undo */
		inline bool CanUndo() { return m_Undo.CanUndo(); }

		/** \brief Ckecing if can Redo */
		inline bool CanRedo() { return m_Undo.CanRedo(); }

		/** \brief Undoing */
		inline void Undo() { SetXmlData(m_Undo.Undo()); }

		/** \brief Redoing */
		inline void Redo() { SetXmlData(m_Undo.Redo()); }

    private:

        /** \brief Generating string with xml data for this item
         *  \note used when creating undo enteries
         */
        wxString GetXmlData();

        /** \brief Restoring resource data from string with xml data */
        bool SetXmlData(const wxString& XmlData);

        /** \brief Rebuilding sources for this resource */
        void RebuildSourceCode();

        /** \brief Rebuilding XRC file managed by this resource */
        void RebuildXrcFile();

        // Functions used by RebuildSourceCode
        void BuildVariablesCode(wxsCodingLang Lang,wxString& LocalCode, wxString& GlobalCode);
        void BuildVariablesCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& LocalCode, wxString& GlobalCode);
        void BuildCreatingCode(wxsCodingLang Lang,wxString& Code);
        void BuildXrcLoadingCode(wxsCodingLang Lang,wxString& Code);
        void BuildXrcItemsFetchingCode(wxsCodingLang Lang,wxString& Code);
        void BuildXrcItemsFetchingCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& Code);
        void BuildEventHandlersCode(wxsCodingLang Lang,wxString& Code);
        void BuildEventHandlersCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& Code);
        void BuildIdentifiersCode(wxsCodingLang Lang,wxString& Code,wxString& IdInitCode);
        void BuildIdsArrayReq(wxsItem* Item,wxArrayString& Array);
        void BuildIncludesCode(wxsCodingLang Lang,wxString& LocalIncludes,wxString& GlobalIncludes);
        void BuildHeadersReq(wxsCodingLang Lang,wxsItem* Item,wxArrayString& LocalHeaders,wxArrayString& GlobalHeaders);

        // Internal Wrappers
        inline wxString GetWxsFileName()          { return GetProjectPath() + m_Resource->m_WxsFileName; }
        inline wxString GetSrcFileName()          { return GetProjectPath() + m_Resource->m_SrcFileName; }
        inline wxString GetHdrFileName()          { return GetProjectPath() + m_Resource->m_HdrFileName; }
        inline wxString GetXrcFileName()          { return GetProjectPath() + m_Resource->m_XrcFileName; }
        inline wxString GetProjectPath()          { return m_Resource->GetProjectPath(); }
        inline wxString GetClassName()            { return m_Resource->GetResourceName(); }
        inline wxString GetResourceType()         { return m_Resource->GetResourceType(); }
        inline wxsCodingLang GetLanguage()        { return m_Resource->GetLanguage(); }
        inline wxsItemRes::EditMode GetEditMode() { return m_Resource->GetEditMode(); }

        wxsItemRes* m_Resource;     ///< \brief Resource pointint to this data
        wxsItem* m_RootItem;        ///< \brief Root item of resource
        wxsItemUndoBuffer m_Undo;   ///< \brief Undo buffer
};



#endif
