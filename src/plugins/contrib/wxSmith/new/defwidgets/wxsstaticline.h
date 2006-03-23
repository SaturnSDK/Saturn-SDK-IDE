#ifndef WXSSTATICLINE_H
#define WXSSTATICLINE_H

#include "../wxswidget.h"

/** \brief Class for wxsStaticLine widget */

class wxsStaticLine: public wxsWidget
{
    public:

        /** \brief Widget's info, in public scope because it will be accessed
         *         from widget manager
         */
        static wxsItemInfo Info;

        /** \brief Ctor
         *
         * Ctor in final classes should have only one argument - pointer to
         * resource. That's because manager will have only one argument when
         * creating widget also.
         */
        wxsStaticLine(wxsWindowRes* Resource);

        /** \brief Function building creating code */
        virtual void BuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language);



    protected:
        /** \brief Function building preview */
        virtual wxObject* DoBuildPreview(wxWindow* Parent,bool Exact);

        /** \brief Function enumerating properties specific for this widget
         *         only. Other properties are added automatically.
         */
        virtual void EnumWidgetProperties(long Flags);

        /** \brief Function to allow widgets specified property update
         *
         */
//        virtual bool MyPropertiesUpdated(bool Validate,bool Correct);
    private:

};


#endif
