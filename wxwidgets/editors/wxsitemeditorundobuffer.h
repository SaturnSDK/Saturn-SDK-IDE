#ifndef WXSITEMEDITORUNDOBUFFER_H
#define WXSITEMEDITORUNDOBUFFER_H

#include <wx/arrstr.h>

class wxsItemEditor;

/** \brief This class is used to handle all Undo and Redo actions.
 *
 * This class bases on idea that any class can be represented
 * as Xml structure which can be hold inside simple string.
 * Each uundo position entry holds whole structure of resource, what
 * in case of standard resources should not exceed 50 kB. In case
 * of memory consumption problems this can be changed to hold differences
 * between two xml nodes only.
 */
class wxsItemEditorUndoBuffer
{
	public:

        /** \brief Ctor */
		wxsItemEditorUndoBuffer(wxsItemEditor* Editor,int MaxEnteries=100);

		/** \brief Dctor */
		virtual ~wxsItemEditorUndoBuffer();

        /** \brief Getting number of enteries in undo array */
        inline int GetCount() { return (int)Enteries.Count(); }

        /** \brief Getting current undo position (counting from 0) */
        inline int GetCurrent() { return CurrentPos; }

		/** \brief Clearing undo table */
		void Clear();

		/** \brief Checkign if we can undo */
		inline bool CanUndo() { return CurrentPos > 0; }

		/** \brief Checking if we can redo */
		bool CanRedo() { return CurrentPos < GetCount() - 1; }

		/** \brief Checking if current undo position is modified relatively to form saved on disk */
		bool IsModified() { return CurrentPos != SavedPos; }

		/** \brief Adding new undo position */
		void StoreChange();

		/** \brief Setting last store point as Saved point */
		inline void Saved() { SavedPos = CurrentPos; }

		/** \brief Undoing */
        bool Undo();

        /** \brief Redoing */
        bool Redo();

	private:

        wxArrayString m_Enteries;   ///< \brief Array enteries
        wxsItemEditor* m_Editor;    ///< \brief Associated item editor
        int m_CurrentPos;           ///< \brief Current position in undo buffer
        int m_SavedPos;             ///< \brief Undo position representing not-changed resource (in form it's on disk)
        int m_MaxEnteries;          ///< \brief Max enteries in undo buffer
};

#endif
