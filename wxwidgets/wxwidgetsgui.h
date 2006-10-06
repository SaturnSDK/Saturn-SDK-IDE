#ifndef WXWIDGETSGUI_H
#define WXWIDGETSGUI_H

#include "../wxsgui.h"
#include "../wxscodinglang.h"

/** \brief Class responsible for operations on wxWidgets-based projects */
class wxWidgetsGUI : public wxsGUI
{
    public:
        /** \brief Ctor */
        wxWidgetsGUI(wxsProject* Project);

        /** \brief Dctor */
        virtual ~wxWidgetsGUI();

    private:

        virtual cbConfigurationPanel* OnBuildConfigurationPanel(wxWindow* Parent);
        virtual void OnRebuildApplicationCode();
        virtual bool OnCheckIfApplicationManaged();
        virtual bool OnCreateApplicationBinding();
        virtual void OnReadConfig(TiXmlElement* element);
        virtual void OnWriteConfig(TiXmlElement* element);
        bool IsAppSourceManaged(const wxString& FileName);
        bool ScanForApp(ProjectFile* File);
        bool AddSmithToApp(const wxString& RelativeFileName);
        bool AskForNewApp();
        wxString GetAppClassName(const wxString& Source);
        inline bool IsWhite(wxChar Ch);
        inline bool IsWhite(const wxString& Str,int Pos);
        inline bool Match(const wxString& Str,int Pos,wxChar Ch);

        wxString m_AppFile;               ///< \brief Source file defining application
        wxArrayString m_LoadedResources;  ///< \brief List of automatically loaded resource files
        wxString m_MainResource;          ///< \brief Resource used by default
        bool m_CallInitAll;               ///< \brief True if wxXmlResource::Get()->InitAllHandlers() should be called while initialization
        bool m_CallInitAllNecessary;      ///< \brief True if should call wxXmlResource::Get()->InitAllHandlers() only when necessary
        wxsCodingLang m_AppLanguage;      ///< \brief Coding language for main app file

        friend class wxWidgetsGUIConfigPanel;
};

#endif
