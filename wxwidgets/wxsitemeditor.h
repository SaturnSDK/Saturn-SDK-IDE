#ifndef WXSITEMEDITOR_H
#define WXSITEMEDITOR_H

#include "wxsitemres.h"
#include "../wxseditor.h"

class wxsItemEditorContent;
class wxsItemResData;
class wxsItem;

/** \brief This is root class for editing wxWidgets window resources
 */
class wxsItemEditor : public wxsEditor
{
	public:

        /** \brief Ctor */
		wxsItemEditor(wxWindow* parent,wxsItemRes* ItemRes);

        /** \brief Dctor */
		virtual ~wxsItemEditor();

		/** \brief Saving resource */
		virtual bool Save();

		/** \brief Returns true if resource is modified, false otherwise */
		virtual bool GetModified();

		/** \brief Checking if can Undo */
		virtual bool CanUndo();

		/** \brief Ckecing if can Redo */
		virtual bool CanRedo();

		/** \brief Checking if we can cut */
		virtual bool HasSelection();

		/** \brief Checking if we can paste */
		virtual bool CanPaste();

		/** \brief Undoing */
		virtual void Undo();

		/** \brief Redoing */
		virtual void Redo();

		/** \brief Cutting */
		virtual void Cut();

		/** \brief Copying */
		virtual void Copy();

		/** \brief Pasting */
		virtual void Paste();

//		/** \brief Getting content of this resource as xml string */
//		wxString GetXmlData();
//
//		/** \brief Updating editor's content with given xml data
//		 * \param XmlData string with xml data representing new resource
//		 * \param AddUndoEntry if true, this operation will create new
//		 *        entry inside undo buffer, if false, undo buffer won't
//		 *        be touched
//		 * \return true on success, false when there's no valid xml data
//		 *         in XmlData or when this data doesn't represent
//		 *         current resource
//		 */
//		bool SetXmlData(const wxString& XmlData,bool AddUndoEntry);
//
//        /** \brief Function notifying that selection has changed
//         *
//         * This function is called from wxsItemRes class after each
//         * selection change.
//         */
//        void SelectionChanged();
//
//        /** \brief Function notifying that properties of given item has changed
//         *  \note You should call wxsItem::NotifyPropertyChange rather than this
//         *        function.
//         */
//        void NotifyChange(wxsItem* Changed);
//

		/** \brief Reloading images in all editors */
		static void ReloadImages();

    protected:

//        /** \brief Getting wxsItemRes pointer to currently edited resource */
//        inline wxsItemRes* GetItemRes() { return (wxsItemRes*)GetResource(); }
//
//        /** \brief Getting project class of current resource file */
//        inline wxsProject* GetProject() { return GetResource()->GetProject(); }
//
//        /** \brief Getting current selection (main selected item) */
//        inline wxsItem* GetCurrentSelection() { return GetWinRes()->GetRootSelection(); }
//
//        /** \brief Helper function for fetching root item */
//        inline wxsItem* RootItem() { return GetWinRes()->GetRootItem(); }
//
	private:

        WX_DECLARE_HASH_SET(wxsItemEditor*,wxPointerHash,wxPointerEqual,WindowSet);
//        WX_DEFINE_ARRAY(wxsItem*,ItemArray);

		/* Event handlers */
        void OnMouseClick(wxMouseEvent& event);
        void OnButton(wxCommandEvent& event);
        void OnInsInto(wxCommandEvent& event);
        void OnInsBefore(wxCommandEvent& event);
        void OnInsAfter(wxCommandEvent& event);
        void OnDelete(wxCommandEvent& event);
        void OnPreview(wxCommandEvent& event);
        void OnQuickProps(wxCommandEvent& event);

        void InitializeVisualStuff();
        void InitializeResourceData();

        /** \brief Initializing editor images */
        static void InitializeImages();

        /** \brief Building palette */
        void BuildPalette(wxNotebook* Palette);

        inline wxsItemRes* GetItemRes() { return (wxsItemRes*)GetResource(); }
        inline const wxString& GetWxsFileName() { return GetItemRes()->GetWxsFileName(); }
        inline const wxString& GetSrcFileName() { return GetItemRes()->GetSrcFileName(); }
        inline const wxString& GetHdrFileName() { return GetItemRes()->GetHdrFileName(); }
        inline const wxString& GetXrcFileName() { return GetItemRes()->GetXrcFileName(); }

//        wxsWinUndoBuffer* m_UndoBuff;       ///< \brief Undo buffer
//        wxsCorrector* m_Corrector;          ///< \brief Data corrector

//        /** \brief Selecting one item and unselecting all others
//         *
//         * This function does not update screen nor resource tree but
//         * states inside wxsItem classes only.
//         */
//        void SelectOneItem(wxsItem* ItemToSelect);
//

        /** \brief Function inserting new item */
        void InsertRequest(const wxString& Name);

        /** \brief Setting mask for insertion type */
        void SetInsertionTypeMask(int Mask);

        /** \brief Setting new insertion type */
        void SetInsertionType(int Type);

        /** \brief Rebuilding pictures on insertion type buttons */
        void RebuildInsTypeIcons();

        /** \brief Rebuilding picture on Quick Props button */
        void RebuildQuickPropsIcon();

        /** \brief Rebuilding all icons */
        void RebuildIcons();

        /** \brief Building icon for one button */
        void BuildInsTypeIcon(wxBitmapButton* Btn,const wxImage& Original,int ButtonType);

        /** \brief Opening or closiung Quick Props panel */
        void ToggleQuickPropsPanel(bool Open);

        /** \brief Refreshing content of Quick Props panel */
        void RebuildQuickProps(wxsItem* Selection);

        /** \brief Rebuilding preview (and updating selection inside preview) */
        void RebuildPreview();

        /** \brief Updating things related to selected item */
        void UpdateSelection();

        /** \brief Getting item which will be used as reference item when adding new
         *         items
         */
        wxsItem* GetReferenceItem(int& InsertionType);

        wxsItemResData* m_Data;             ///< \brief Data managment object

        wxsItemEditorContent* m_Content;    ///< \brief Window with content area
        wxNotebook* m_WidgetsSet;           ///< \brief Notebook with all widgets inside
        wxBoxSizer* m_VertSizer;            ///< \brief Root sizer of this editor
        wxBoxSizer* m_HorizSizer;           ///< \brief Horizontal sizer managing items below palette
        wxBoxSizer* m_QPSizer;              ///< \brief Sizer for quick properties
        wxBoxSizer* m_OpsSizer;             ///< \brief Sizer for operations pane
        wxScrolledWindow* m_QPArea;         ///< \brief Scrolled window containing all QuickProps sturr
        wxBitmapButton* m_InsIntoBtn;
        wxBitmapButton* m_InsBeforeBtn;
        wxBitmapButton* m_InsAfterBtn;
        wxBitmapButton* m_DelBtn;
        wxBitmapButton* m_PreviewBtn;
        wxBitmapButton* m_QuickPanelBtn;
        wxWindow* m_TopPreview;             ///< \brief Top window of preview
        int m_InsType;                      ///< \brief Current insertion type
        int m_InsTypeMask;                  ///< \brief Current insertion type mask
        bool m_QuickPropsOpen;              ///< \brief Set to true if quick properties panel is opened
        bool m_DontStoreUndo;               ///< \brief When set to true, FinishChange() won't create new undo entry, usefull in Undo/Redo operations

        static wxImage m_InsIntoImg;
        static wxImage m_InsBeforeImg;
        static wxImage m_InsAfterImg;
        static wxImage m_DelImg;
        static wxImage m_PreviewImg;
        static wxImage m_QuickPropsImgOpen;
        static wxImage m_QuickPropsImgClose;
        static wxImage m_SelectedImg;
        static WindowSet m_AllEditors;
        static bool m_ImagesLoaded;

        static const int itBefore = 0x01;
        static const int itAfter  = 0x02;
        static const int itInto   = 0x04;

        DECLARE_EVENT_TABLE()

        friend class wxsItemEditorContent;
        friend class wxsItemResData;
};

#endif
