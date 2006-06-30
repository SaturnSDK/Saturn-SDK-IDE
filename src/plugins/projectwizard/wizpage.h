#ifndef WIZPAGE_H
#define WIZPAGE_H

#include <wx/string.h>
#include <wx/wizard.h>
#include <wx/panel.h>
#include <wx/xrc/xmlres.h>

class ProjectPathPanel;
class CompilerPanel;
class LanguagePanel;
class FilePathPanel;

class WizPageBase : public wxWizardPageSimple
{
	public:
		WizPageBase(const wxString& pageName, wxWizard* parent, const wxBitmap& bitmap = wxNullBitmap);
		~WizPageBase();
	    virtual void OnPageChanging(wxWizardEvent& event);
	    virtual void OnPageChanged(wxWizardEvent& event);
    protected:
        wxString m_PageName;
    private:
		DECLARE_EVENT_TABLE()
};

class WizPage : public WizPageBase
{
	public:
		WizPage(const wxString& panelName, wxWizard* parent, const wxBitmap& bitmap = wxNullBitmap);
		~WizPage();
	    void OnButton(wxCommandEvent& event);
    protected:
    private:
		DECLARE_EVENT_TABLE()
};

class WizIntroPanel : public WizPageBase
{
	public:
		WizIntroPanel(const wxString& intro_msg, wxWizard* parent, const wxBitmap& bitmap = wxNullBitmap);
		~WizIntroPanel();
    protected:
    private:
};

class WizFilePathPanel : public WizPageBase
{
	public:
		WizFilePathPanel(bool showHeaderGuard, wxWizard* parent, const wxBitmap& bitmap = wxNullBitmap);
		~WizFilePathPanel();

        wxString GetFilename(){ return m_Filename; }
        wxString GetHeaderGuard(){ return m_HeaderGuard; }
        bool GetAddToProject(){ return m_AddToProject; }
        int GetTargetIndex(){ return m_TargetIndex; }
        void SetFilePathSelectionFilter(const wxString& filter);

	    void OnPageChanging(wxWizardEvent& event);
    protected:
        FilePathPanel* m_pFilePathPanel;
        wxString m_Filename;
        wxString m_HeaderGuard;
        bool m_AddToProject;
        int m_TargetIndex;
    private:
};

class WizProjectPathPanel : public WizPageBase
{
	public:
		WizProjectPathPanel(wxWizard* parent, const wxBitmap& bitmap = wxNullBitmap);
		~WizProjectPathPanel();

		wxString GetPath();
		wxString GetName();
		wxString GetFullFileName();
		wxString GetTitle();

	    void OnButton(wxCommandEvent& event);
	    virtual void OnPageChanging(wxWizardEvent& event);
	    virtual void OnPageChanged(wxWizardEvent& event);
    protected:
        ProjectPathPanel* m_pProjectPathPanel;
    private:
		DECLARE_EVENT_TABLE()
};

class WizCompilerPanel : public WizPageBase
{
	public:
		WizCompilerPanel(const wxString& compilerID, const wxString& validCompilerIDs, wxWizard* parent, const wxBitmap& bitmap = wxNullBitmap,
                        bool allowCompilerChange = true, bool allowConfigChange = true);
		~WizCompilerPanel();

		wxString GetCompilerID();
		bool GetWantDebug();
        wxString GetDebugName();
        wxString GetDebugOutputDir();
        wxString GetDebugObjectOutputDir();
		bool GetWantRelease();
        wxString GetReleaseName();
        wxString GetReleaseOutputDir();
        wxString GetReleaseObjectOutputDir();

	    void OnPageChanging(wxWizardEvent& event);
    protected:
        CompilerPanel* m_pCompilerPanel;
    private:
};

class WizLanguagePanel : public WizPageBase
{
	public:
		WizLanguagePanel(const wxArrayString& langs, int defLang, wxWizard* parent, const wxBitmap& bitmap = wxNullBitmap);
		~WizLanguagePanel();

        int GetLanguage();
        void SetLanguage(int lang);
    protected:
        LanguagePanel* m_pLanguagePanel;
    private:
};

#endif // WIZPAGE_H
