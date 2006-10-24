#ifndef WXSDIALOGRES_H
#define WXSDIALOGRES_H

#include "wxwidgetsres.h"

/** \brief Class responsible for managing dialog resource */
class wxsDialogRes: public wxWidgetsRes
{
    public:

        /** \brief Ctor */
        wxsDialogRes(wxsProject* Owner);

        /** \brief Dctor */
        virtual ~wxsDialogRes();

    protected:

        virtual wxsEditor* OnCreateEditor();
        virtual bool OnReadConfig(const TiXmlElement* Node);
        virtual bool OnWriteConfig(TiXmlElement* Node);
        virtual bool OnCanHandleFile(const wxString& FileName);
        virtual wxString OnGetDeclarationFile();
        virtual wxString OnGetAppBuildingCode();
        virtual bool OnGetUsingXRC();
        virtual bool OnGetCanBeMain();

    private:

        wxString m_WxsFileName;
        wxString m_SrcFileName;
        wxString m_HdrFileName;
        wxString m_XrcFileName;
};

#endif
