#ifndef WXSITEMRES_H
#define WXSITEMRES_H

#include "wxwidgetsres.h"

class wxsItem;

/** \brief Base class for resources using item as root element
 *
 * This class implements most of functions in wxsResource and wxWidgetsRes,
 * only need to write this function: \code virtual wxString OnGetAppBuildingCode() \endcode
 */
class wxsItemRes: public wxWidgetsRes
{
    DECLARE_CLASS(wxsItemRes)
    public:

        /** \brief Available edit modes for item resources */
        enum EditMode { File, Source, Mixed };

        /** \brief Ctor */
        wxsItemRes(wxsProject* Owner,const wxString& ResourceType,bool CanBeMain);

        /** \brief Ctor for external resource
         *  \param FileName name of XRC file
         *  \param Object Xml node with XRC resource
         */
        wxsItemRes(const wxString& FileName,const TiXmlElement* XrcElem,const wxString& ResourceType);

        /** \brief Dctor */
        virtual ~wxsItemRes();

        /** \brief Creating new resource and building files if necessarry */
        virtual bool CreateNewResource(
            const wxString& Class,
            const wxString& Src,
            bool GenSrc,
            const wxString& Hdr,
            bool GenHdr,
            const wxString& Xrc,
            bool GenXrc);

        /** \brief Getting root item of this resource */
        inline wxsItem* GetRootItem() { return m_RootItem; }

        /** \brief Building properties filter for item properties based on resource config */
        unsigned long GetPropertiesFilter();

        /** \brief Function for resource change notification */
        void NotifyChange(wxsItem* Item);

        /** \brief Getting name of resource class (equivalent to resource name) */
        inline const wxString& GetClassName() { return GetResourceName(); }

        /** \brief Getting current edit mode */
        EditMode GetEditMode();

    protected:

        /** \brief This function should load xrc resource from file
         *
         * Default implementation uses LoadObject function
         */
        virtual void OnBuildXrcLoadingCode(wxsCodingLang Language,wxString& Code);

        virtual wxsEditor* OnCreateEditor();
        virtual bool OnReadConfig(const TiXmlElement* Node);
        virtual bool OnWriteConfig(TiXmlElement* Node);
        virtual bool OnCanHandleFile(const wxString& FileName);
        virtual wxString OnGetDeclarationFile();
        virtual bool OnGetUsingXRC();
        virtual bool OnGetCanBeMain();

    private:

        /** \brief Rebuilding sources for this resource */
        void RebuildSourceCode();

        /** \brief Rebuilding XRC file managed by this resource */
        void RebuildXrcFile();

        // Functions used by RebuildSourceCode
        void BuildVariablesCode(wxsCodingLang Lang,wxString& LocalCode, wxString& GlobalCode);
        void BuildVariablesCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& LocalCode, wxString& GlobalCode);
        void BuildCreatingCode(wxsCodingLang Lang,wxString& Code);
        void BuildXrcLoadingCode(wxsCodingLang Lang,wxString& Code);
        void BuildXrcItemsFetchingCode(wxsCodingLang Lang,wxString& Code);
        void BuildXrcItemsFetchingCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& Code);
        void BuildEventHandlersCode(wxsCodingLang Lang,wxString& Code);
        void BuildEventHandlersCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& Code);
        void BuildIdentifiersCode(wxsCodingLang Lang,wxString& Code,wxString& IdInitCode);
        void BuildIdsArrayReq(wxsItem* Item,wxArrayString& Array);
        void BuildIncludesCode(wxsCodingLang Lang,wxString& LocalIncludes,wxString& GlobalIncludes);
        void BuildHeadersReq(wxsCodingLang Lang,wxsItem* Item,wxArrayString& LocalHeaders,wxArrayString& GlobalHeaders);

        wxString m_WxsFileName;
        wxString m_SrcFileName;
        wxString m_HdrFileName;
        wxString m_XrcFileName;
        bool     m_CanBeMain;
        wxsItem* m_RootItem;
};

#endif
