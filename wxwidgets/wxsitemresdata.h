#ifndef WXSITEMRESDATA_H
#define WXSITEMRESDATA_H

#include "wxsitem.h"
#include "wxsparent.h"
#include "wxsitemres.h"
#include "wxscorrector.h"
#include "wxsitemundobuffer.h"
#include "wxsitemresdataobject.h"
#include "../wxsresourcetree.h"


class wxsItemEditor;

/** \brief Class holding data for item resources and operating on it */
class wxsItemResData
{
    public:

        /** \brief Ctor
         *
         * Constructor tries to load resource. Depending on what
         * parameters are empty strings, given type of file is assumed.
         *  - If Wxs, Src and Hdr file names are empty, it's only Xrc file.
         *  - If Xrc is empty but no other, it's resource not using Xrc file
         *  - If all file names are not empty, it's resoure using Xrc file
         *  - Other combinations are invalid.
         *
         * Parameters passed to constructor are GLOBAL paths (opposite to
         * wxsItemRes where names are relative to .cbp's directory).
         */
        wxsItemResData(
            const wxString& WxsFileName,
            const wxString& SrcFileName,
            const wxString& HdrFileName,
            const wxString& XrcFileName,
            const wxString& ClassName,
            const wxString& ClassType,
            wxsCodingLang   Language,
            wxsResourceItemId TreeId,
            wxsItemEditor*  Editor
            );

        /** \brief Dctor
         *
         * \note When wxsItemResData is deleted and the
         *       data is in modified state (was not saved after modification),
         *       all changes are lost. Please call Save() to avoid this.
         */
        virtual ~wxsItemResData();

        /** \brief Loading resource
         *
         * This function (Re)loads resource from
         * files. Current resource content is moved into
         * undo buffer and resource data is replaced by the one
         * loaded from file.
         */
        bool Load();

        /** \brief Saving resource
         *
         * This function saves resource to wxs file
         * (note that Src / Hdr / Xrc files are not saved
         * because they're updated after each resource change)
         */
        bool Save();

        /** \brief Checking if resource was loaded properly
         *
         * This function may be used to check if resource was properly
         * loaded. It may be especially usual after checking if
         * constructor has loaded all data properly.
         */
        inline bool IsOk() { return m_IsOK; }

        /** \brief Function starting change of resource data
         *
         * This function Notifies that resource is going to change.
         * It locks data from other changes. Each resource
         * change must be finished with call to EndChange function.
         * Between BeginChagne and EndChange call there should not
         * be any call to gui item, so do not jump out of event
         * function before EndChange is called.
         *
         * Change of resource is any operation made on any wxsItem
         * class inside the resource (including the root class).
         */
        void BeginChange();

        /** \brief Function ending change of resource data
         *
         * This function must be paired with BeginChange() call.
         * it notifies that change of resource has been finished
         * and that it's good time to update all data on the screen
         * ans store new undo buffer entry.
         */
        void EndChange();

        /** \brief Checking if item has modified state */
        inline bool GetModified() { return m_Undo.IsModified(); }

        /** \brief Getting root item
         *  \return pointer to item on success, NULL when data wasn't initialized properly
         */
        inline wxsItem* GetRootItem() { return m_RootItem; }

        /** \brief Getting main item of selection */
        inline wxsItem* GetRootSelection() { return m_RootSelection; }

        /** \brief Getting properties filter based on current edit  mode */
        inline int GetPropertiesFilter() { return m_PropertiesFilter; }

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

		/* ********************** */
		/*  Clipboard operations  */
		/* ********************** */

		/** \brief Checking if we can paste current clipboard content */
		bool CanPaste();

		/** \brief Cutting current selection to clipboard */
		void Cut();

		/** \brief Copying current selectin to clipboard */
		void Copy();

		/** \brief Pasting current selection from clipboard */
		void Paste();


		/* ********************** */
		/*  Selection operations  */
		/* ********************** */

		/** \brief Checking of there's any selection */
		bool AnySelected();

		/** \brief Selecting one item */
		bool SelectItem(wxsItem* Item,bool UnselectOther);

		/** \brief Getting last selected item or NULL if there's no valid selection */
		inline wxsItem* GetLastSelection() { return m_RootSelection; }

		/* ******************* */
		/*  Operating on data  */
		/* ******************* */

		/** \brief Adding new item
		 *
		 * This function tries to add new item into
		 * given position. If it's possible, new item
		 * is added and true is returned. If it's
		 * impossible, new item is deleted internally
		 * and function returns false.
		 * \param New new item
		 * \param Parent item which will become parent of New
		 * \param Position position inside Parent (if <0 or  out of range,
		 *        appending New at the end of Parent's children)
		 */
        bool InsertNew(wxsItem* New,wxsParent* Parent,int Position);

        /** \brief Deleting all selected items */
        void DeleteSelected();

        /* ******************* */
        /*  Preview functions  */
        /* ******************* */

        /** \brief Checking if there's preview already */
        // TODO: Code it
        bool IsPreview() { return false; }

        /** \brief Showing preview of current resource content */
        // TODO: Code it
        bool ShowPreview() { return false; }

        /** \brief Closing window with current resource content */
        // TODO: Code it
        bool HidePreview() { return false; }

        /* *********************** */
        /*  Notification handlers  */
        /* *********************** */

        /** \brief Notification of change of data
         *
         * This function is called from wxsItem objects
         * notifying about change of such item.
         */
        void NotifyChange(wxsItem* Changed);

    private:

        WX_DECLARE_STRING_HASH_MAP(TiXmlElement*,IdToXmlMapT);

        /** \brief Generating string with xml data for this item
         *  \note used when creating undo enteries
         */
        wxString GetXmlData();

        /** \brief Restoring resource data from string with xml data */
        bool SetXmlData(const wxString& XmlData);

        /** \brief Rebuilding all files kept up-to-date after change in resource */
        void RebuildFiles();

        /** \brief Rebuilding sources for this resource */
        void RebuildSourceCode();

        /** \brief Rebuilding XRC file managed by this resource */
        bool RebuildXrcFile();

        // Various loading functinos
        bool SilentLoad();
        bool LoadInFileMode();
        bool LoadInMixedMode();
        bool LoadInSourceMode();
        void UpdateExtraDataReq(wxsItem* Item,IdToXmlMapT& Map);
        void RecreateRootItem();

        // Various saving function
        bool SaveInFileMode();
        bool SaveInMixedMode();
        bool SaveInSourceMode();
        void SaveExtraDataReq(wxsItem* Item,TiXmlElement* Node);

        // Some misc functions
        inline void StoreUndo() { m_Undo.StoreChange(GetXmlData()); }
        bool ValidateRootSelection();
        bool ValidateRootSelectionReq(wxsItem* Item,wxsItem*& NewSelection);
        void CopyReq(wxsItem* Item,wxsItemResDataObject* Data);
        bool AnySelectedReq(wxsItem* Item);
        void StoreTreeExpandState();
        void StoreTreeExpandStateReq(wxsItem* Item);
        void DeleteSelectedReq(wxsItem* Item);
        void RebuildTree();

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


        wxString m_WxsFileName;
        wxString m_SrcFileName;
        wxString m_HdrFileName;
        wxString m_XrcFileName;
        wxString m_ClassName;
        wxString m_ClassType;
        wxsCodingLang m_Language;
        wxsResourceItemId m_TreeId;
        wxsItemEditor* m_Editor;

        wxsItem* m_RootItem;
        wxsItem* m_RootSelection;
        unsigned long m_PropertiesFilter;

        wxsItemUndoBuffer m_Undo;
        wxsCorrector m_Corrector;

        bool m_IsOK;
        int m_LockCount;
};

#endif
