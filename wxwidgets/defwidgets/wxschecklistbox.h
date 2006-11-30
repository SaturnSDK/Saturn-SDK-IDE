#ifndef WXSCHECKLISTBOX_H
#define WXSCHECKLISTBOX_H

#include "../wxswidget.h"

/** \brief Class for wxsCheckListBox widget */
class wxsCheckListBox: public wxsWidget
{
    public:

        wxsCheckListBox(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language);
        virtual wxObject* OnBuildPreview(wxWindow* Parent,bool Exact,bool Store);
        virtual void OnEnumWidgetProperties(long Flags);
        virtual void OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language);

        wxArrayString ArrayChoices;
        wxArrayBool ArrayChecks;
};

#endif
