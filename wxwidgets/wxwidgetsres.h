#ifndef WXWIDGETSRES_H
#define WXWIDGETSRES_H

#include "../wxsresource.h"

/** \brief Base class for all resources used inside wxWidgets */
class wxWidgetsRes: public wxsResource
{
    DECLARE_CLASS(wxWidgetsRes)
    public:

        /** \brief Ctor */
        wxWidgetsRes(wxsProject* Owner,const wxString& ResourceName,const wxString& ResourceType,wxsCodingLang Language=wxsCPP):
            wxsResource(Owner,ResourceName,ResourceType,_T("wxWidgets"),Language)
        {}

        /** \brief Checking if this resource does use XRC */
        virtual bool OnGetUsingXRC() = 0;

        /** \brief Checking if this resource can be main resource in app */
        virtual bool OnGetCanBeMain() = 0;

};

#endif
