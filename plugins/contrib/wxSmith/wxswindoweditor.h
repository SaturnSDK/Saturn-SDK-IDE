#ifndef WXSWINDOWEDITOR_H
#define WXSWINDOWEDITOR_H

#include <wx/wx.h>

#include "wxsproject.h"
#include "wxseditor.h"
#include "wxscoder.h"
#include "wxsevent.h"

class wxsWidget;
class wxsDragWindow;
class wxsWindowRes;

class wxsWindowEditor : public wxsEditor
{
	public:
		wxsWindowEditor(wxWindow* parent, wxsWindowRes* Resource);

		virtual ~wxsWindowEditor();

		/** Creating preview from given window */
		virtual void BuildPreview(wxsWidget* TopWidget);

		/** Killing previously created preview */
		virtual void KillCurrentPreview();

		/** Recreating current preview making everything look just like it will
		 *  be in real program
		 */
        inline void RecreatePreview() { BuildPreview(CurrentWidget); }

        /** Function notifying that there's need to recalculate current editor's
         *  sizers because given widget changed it's size
         */
        void PreviewReshaped();

        /** Getting top widget of current preview */
        wxsWidget* GetTopWidget() { return CurrentWidget; }

		/** Closing action will store code changes */
		virtual bool Close();
		
		/** Saving resource */
		virtual bool Save();

		/** Returns true if resource is modified, false otherwise */
		virtual bool GetModified();
		
		/** Set the resources's modification state to \c modified. */
		virtual void SetModified(bool modified);
		
    protected:

        /** Unbinding from resource - this function is killing preview objects */
        virtual void MyUnbind();

	private:

		/** Scrolled window, parent for preview */
        wxScrolledWindow* Scroll;

		/* Event handlers */
        void OnMouseClick(wxMouseEvent& event);
        void OnSelectWidget(wxsEvent& event);
        void OnUnselectWidget(wxsEvent& event);

        /** Root widget of currently edited window */
        wxsWidget* CurrentWidget;

        /** New layer used for dragging widgets */
        wxsDragWindow* DragWnd;
        
        DECLARE_EVENT_TABLE()
};


#endif // WXSWINDOWEDITOR_H
