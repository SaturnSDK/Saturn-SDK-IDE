#ifndef PROJECTFILEOPTIONSDLG_H
#define PROJECTFILEOPTIONSDLG_H

#include <wx/dialog.h>
#include "settings.h"

class ProjectFile;

class ProjectFileOptionsDlg : public wxDialog
{
	public:
		ProjectFileOptionsDlg(wxWindow* parent, ProjectFile* pf);
		~ProjectFileOptionsDlg();
		void OnCompilerCombo(wxCommandEvent& event);
		void OnUpdateUI(wxUpdateUIEvent& event);
		void EndModal(int retCode);
	private:
        void FillCompilers();
        void UpdateBuildCommand();
        void SaveBuildCommandSelection();

		ProjectFile* m_ProjectFile;
		int m_LastBuildStageCompilerSel;

		DECLARE_EVENT_TABLE()
};

#endif // PROJECTFILEOPTIONSDLG_H
