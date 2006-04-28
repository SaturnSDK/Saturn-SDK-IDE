#ifndef WXSEDITOR_H
#define WXSEDITOR_H

#include <editorbase.h>

class wxsResource;

/** \brief Base class for editors used inside wxSmith
 *
 * This class is responsible for proper binding / unbinging editor
 * from resource.
 *
 * Resource is only an information that given resource should exist.
 * Editor does load this resource and allow user to edit it.
 */
class wxsEditor: public EditorBase
{
	public:

        /** \brief Ctor */
		wxsEditor(wxWindow* parent,const wxString& title,wxsResource* Resource);

		/** \brief Dctor */
		virtual ~wxsEditor();

		/** \brief Getting current resouce */
		inline wxsResource* GetResource() { return Resource; }

		/** \brief Function called when resource is goingto change
		 *
		 * This function notifies editor that resource is going to change.
		 * During the change, resource is considered as unstable, and won't
		 * be processed by editor. After the change you MUST call UnlockResource().
		 */
		virtual void ResourceLock() = 0;

		/** \brief Function called when resource change is finished */
		virtual void ResourceUnlock() = 0;

	private:

        /** \brief Currently associated resource */
        wxsResource* Resource;
};

#endif
