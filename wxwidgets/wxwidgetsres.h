#ifndef WXWIDGETSRES_H
#define WXWIDGETSRES_H

#include "../wxsresource.h"

/** \brief Base class for all resources used inside wxWidgets */
class wxWidgetsRes: public wxsResource
{
    DECLARE_CLASS(wxWidgetsRes)
    public:

        /** \brief Ctor */
        wxWidgetsRes(wxsProject* Owner,const wxString& ResourceType):
            wxsResource(Owner,ResourceType,_T("wxWidgets"))
        {}

        /** \brief Checking if this resource does use XRC */
        virtual bool OnGetUsingXRC() = 0;

        /** \brief Checking if this resource can be main resource in app */
        virtual bool OnGetCanBeMain() = 0;

        /** \brief Function creating new resource */
        virtual bool OnCreateNewResource(
            const wxString& Class,
            const wxString& Src, bool GenSrc,
            const wxString& Hdr, bool GenHdr,
            const wxString& Xrc, bool GenXrc ) = 0;
};

#endif
