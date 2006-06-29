#ifndef NEWFROMTEMPLATEDLG_H
#define NEWFROMTEMPLATEDLG_H

#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include "projecttemplateloader.h"
#include "settings.h"
#include "pluginmanager.h"

WX_DEFINE_ARRAY(ProjectTemplateLoader*, ProjectTemplateArray);

class NewFromTemplateDlg : public wxDialog
{
	public:
		NewFromTemplateDlg(const wxArrayString& user_templates);
		virtual ~NewFromTemplateDlg();

		ProjectTemplateLoader* GetTemplate(){ return m_Template; }
		cbWizardPlugin* GetWizard(){ return m_pWizard; }
		int GetWizardIndex(){ return m_WizardIndex; }
		cbWizardPlugin* NewFromTemplateDlg::GetSelectedTemplate();
		bool SelectedUserTemplate();
		wxString GetSelectedUserTemplate();
	protected:
		void BuildCategories();
		void BuildList();
		void FillTemplate(ProjectTemplateLoader* pt);
		void ClearList();
		void OnListSelection(wxListEvent& event);
		void OnListRightClick(wxListEvent& event);
		void OnCategoryChanged(wxCommandEvent& event);
		void OnEditScript(wxCommandEvent& event);
		void OnEditGlobalScript(wxCommandEvent& event);
		void OnUpdateUI(wxUpdateUIEvent& event);

		void EditScript(const wxString& relativeFilename);
	private:
		ProjectTemplateLoader* m_Template;
		cbWizardPlugin* m_pWizard;
		int m_WizardIndex;
		wxImageList m_ImageList;
		PluginsArray m_Wizards;
		DECLARE_EVENT_TABLE()
};

#endif // NEWFROMTEMPLATEDLG_H

