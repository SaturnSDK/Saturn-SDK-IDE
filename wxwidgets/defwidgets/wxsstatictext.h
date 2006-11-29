#ifndef WXSSTATICTEXT_H
#define WXSSTATICTEXT_H

#include "../wxswidget.h"

/** \brief Class for wxsStaticText widget */
class wxsStaticText: public wxsWidget
{
    public:

        wxsStaticText(wxsItemResData* Data);

    private:

        virtual void      OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language);
        virtual wxObject* OnBuildPreview(wxWindow* Parent,bool Exact,bool Store);
        virtual void      OnEnumWidgetProperties(long Flags);
        virtual void      OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language);

        wxString Label;
};

#endif
