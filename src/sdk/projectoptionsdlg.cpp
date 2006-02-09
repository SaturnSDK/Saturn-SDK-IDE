/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "manager.h"
    #include "pluginmanager.h"
    #include "projectmanager.h"
    #include "compilerfactory.h"
    #include "globals.h"

    #include <wx/xrc/xmlres.h>
    #include <wx/treectrl.h>
    #include <wx/spinctrl.h>
    #include <wx/button.h>
#endif

#include <wx/radiobox.h>

#include "projectoptionsdlg.h" // class's header file
#include "editarrayorderdlg.h"
#include "editarrayfiledlg.h"
#include "externaldepsdlg.h"
#include "annoyingdialog.h"
#include "angelscript.h"

BEGIN_EVENT_TABLE(ProjectOptionsDlg, wxDialog)
    EVT_UPDATE_UI( -1,                                 ProjectOptionsDlg::OnUpdateUI)
	EVT_BUTTON(    XRCID("btnOK"),                     ProjectOptionsDlg::OnOK)
	EVT_BUTTON(    XRCID("btnProjectBuildOptions"),    ProjectOptionsDlg::OnProjectBuildOptionsClick)
	EVT_BUTTON(    XRCID("btnProjectDeps"),            ProjectOptionsDlg::OnProjectDepsClick)
	EVT_BUTTON(    XRCID("btnTargetBuildOptions"),     ProjectOptionsDlg::OnTargetBuildOptionsClick)
	EVT_LISTBOX_DCLICK(XRCID("lstBuildTarget"),        ProjectOptionsDlg::OnTargetBuildOptionsClick)
	EVT_BUTTON(    XRCID("btnBuildOrder"),             ProjectOptionsDlg::OnBuildOrderClick)
	EVT_BUTTON(    XRCID("btnAddBuildTarget"),         ProjectOptionsDlg::OnAddBuildTargetClick)
	EVT_BUTTON(    XRCID("btnEditBuildTarget"),        ProjectOptionsDlg::OnEditBuildTargetClick)
	EVT_BUTTON(    XRCID("btnCopyBuildTarget"),        ProjectOptionsDlg::OnCopyBuildTargetClick)
	EVT_BUTTON(    XRCID("btnDelBuildTarget"),         ProjectOptionsDlg::OnRemoveBuildTargetClick)
	EVT_BUTTON(    XRCID("btnBrowseOutputFilename"),   ProjectOptionsDlg::OnBrowseOutputFilenameClick)
	EVT_BUTTON(    XRCID("btnBrowseWorkingDir"),       ProjectOptionsDlg::OnBrowseDirClick)
	EVT_BUTTON(    XRCID("btnBrowseObjectDir"),        ProjectOptionsDlg::OnBrowseDirClick)
	EVT_BUTTON(    XRCID("btnExternalDeps"),           ProjectOptionsDlg::OnEditDepsClick)
	EVT_BUTTON(    XRCID("btnExportTarget"),           ProjectOptionsDlg::OnExportTargetClick)
	EVT_LISTBOX_DCLICK(XRCID("lstFiles"),              ProjectOptionsDlg::OnFileOptionsClick)
	EVT_BUTTON(    XRCID("btnFileOptions"),            ProjectOptionsDlg::OnFileOptionsClick)
	EVT_BUTTON(    XRCID("btnToggleCheckmarks"),       ProjectOptionsDlg::OnFileToggleMarkClick)
	EVT_LISTBOX(   XRCID("lstBuildTarget"),            ProjectOptionsDlg::OnBuildTargetChanged)
	EVT_COMBOBOX(  XRCID("cmbProjectType"),            ProjectOptionsDlg::OnProjectTypeChanged)

    EVT_TREE_SEL_CHANGED(XRCID("tcOverview"),          ProjectOptionsDlg::OnScriptsOverviewSelChanged)
    EVT_BUTTON(XRCID("btnCheckScripts"),               ProjectOptionsDlg::OnCheckScripts)
    EVT_BUTTON(XRCID("btnAddPreScripts"),              ProjectOptionsDlg::OnAddScript)
    EVT_BUTTON(XRCID("btnRemovePreScripts"),           ProjectOptionsDlg::OnRemoveScript)
    EVT_SPIN_UP(XRCID("spnPreScripts"),                ProjectOptionsDlg::OnScriptMoveUp)
    EVT_SPIN_DOWN(XRCID("spnPreScripts"),              ProjectOptionsDlg::OnScriptMoveDown)
END_EVENT_TABLE()

// class constructor
ProjectOptionsDlg::ProjectOptionsDlg(wxWindow* parent, cbProject* project)
    : m_Project(project),
    m_Current_Sel(-1),
	m_pCompiler(0L)
{
	wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgProjectOptions"));

    wxCheckListBox* list = XRCCTRL(*this, "lstFiles", wxCheckListBox);
    int count = m_Project->GetFilesCount();
    for (int i = 0; i < count; ++i)
    {
        ProjectFile* pf = m_Project->GetFile(i);
        list->Append(pf->relativeFilename);
    }

    // general
	XRCCTRL(*this, "txtProjectName", wxTextCtrl)->SetValue(m_Project->GetTitle());
	XRCCTRL(*this, "txtProjectFilename", wxStaticText)->SetLabel(m_Project->GetFilename());
	XRCCTRL(*this, "txtProjectMakefile", wxTextCtrl)->SetValue(m_Project->GetMakefile());
	XRCCTRL(*this, "chkCustomMakefile", wxCheckBox)->SetValue(m_Project->IsMakefileCustom());
	XRCCTRL(*this, "rbPCHStrategy", wxRadioBox)->SetSelection((int)m_Project->GetModeForPCH());

    FillBuildTargets();

	PluginsArray plugins = Manager::Get()->GetPluginManager()->GetCompilerOffers();
	if (plugins.GetCount())
		m_pCompiler = (cbCompilerPlugin*)plugins[0];

    // scripts
    BuildScriptsTree();
}

// class destructor
ProjectOptionsDlg::~ProjectOptionsDlg()
{
	// insert your code here
}

void ProjectOptionsDlg::BuildScriptsTree()
{
	wxTreeCtrl* tc = XRCCTRL(*this, "tcOverview", wxTreeCtrl);
	tc->DeleteAllItems();

	wxTreeItemId root = tc->AddRoot(m_Project->GetTitle());
    for (int x = 0; x < m_Project->GetBuildTargetsCount(); ++x)
    {
        ProjectBuildTarget* target = m_Project->GetBuildTarget(x);
        tc->AppendItem(root, target->GetTitle());
    }
    tc->Expand(root);
    tc->SelectItem(root);
    FillScripts();
}

void ProjectOptionsDlg::FillScripts()
{
	wxTreeCtrl* tc = XRCCTRL(*this, "tcOverview", wxTreeCtrl);
	wxTreeItemId sel = tc->GetSelection();
    CompileOptionsBase* base = sel == tc->GetRootItem()
                                ? static_cast<CompileOptionsBase*>(m_Project)
                                : static_cast<CompileOptionsBase*>(m_Project->GetBuildTarget(tc->GetItemText(sel)));

    wxListBox* lstScripts = XRCCTRL(*this, "lstPreScripts", wxListBox);
    lstScripts->Clear();
    for (size_t i = 0; i < base->GetBuildScripts().GetCount(); ++i)
        lstScripts->Append(base->GetBuildScripts().Item(i));
}

void ProjectOptionsDlg::FillBuildTargets()
{
    // add build targets
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
    lstTargets->Clear();
    for (int i = 0; i < m_Project->GetBuildTargetsCount(); ++i)
        lstTargets->Append(m_Project->GetBuildTarget(i)->GetTitle());
    lstTargets->SetSelection(0);
    DoTargetChange();
}

void ProjectOptionsDlg::DoTargetChange()
{
    DoBeforeTargetChange();

    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);

    if (lstTargets->GetSelection() == -1)
        lstTargets->SetSelection(0);
	ProjectBuildTarget* target = m_Project->GetBuildTarget(lstTargets->GetSelection());
	if (!target)
		return;

	XRCCTRL(*this, "chkBuildThisTarget", wxCheckBox)->SetValue(target->GetIncludeInTargetAll());
	XRCCTRL(*this, "chkCreateDefFile", wxCheckBox)->SetValue(target->GetCreateDefFile());
	XRCCTRL(*this, "chkCreateStaticLib", wxCheckBox)->SetValue(target->GetCreateStaticLib());

	// global project options
	wxComboBox* cmb = XRCCTRL(*this, "cmbProjectType", wxComboBox);
	wxCheckBox* chkCR = XRCCTRL(*this, "chkUseConsoleRunner", wxCheckBox);
    wxTextCtrl* txt = XRCCTRL(*this, "txtOutputFilename", wxTextCtrl);
    wxTextCtrl* txtW = XRCCTRL(*this, "txtWorkingDir", wxTextCtrl);
    wxTextCtrl* txtO = XRCCTRL(*this, "txtObjectDir", wxTextCtrl);
    wxButton* browse = XRCCTRL(*this, "btnBrowseOutputFilename", wxButton);
    wxButton* browseW = XRCCTRL(*this, "btnBrowseWorkingDir", wxButton);
    wxButton* browseO = XRCCTRL(*this, "btnBrowseObjectDir", wxButton);
    XRCCTRL(*this, "chkCreateDefFile", wxCheckBox)->Enable(target->GetTargetType() == ttStaticLib ||
                                                            target->GetTargetType() == ttDynamicLib);
    chkCR->Enable(false);
    XRCCTRL(*this, "chkCreateStaticLib", wxCheckBox)->Enable(target->GetTargetType() == ttDynamicLib);
    if (cmb && chkCR && txt && browse)
    {
        cmb->SetSelection(target->GetTargetType());
//        Compiler* compiler = CompilerFactory::Compilers[target->GetCompilerIndex()];
        switch ((TargetType)cmb->GetSelection())
        {
            case ttConsoleOnly:
                chkCR->Enable(true);
                chkCR->SetValue(target->GetUseConsoleRunner());
                // purposely fall-through

            case ttExecutable:
            case ttDynamicLib:
            case ttStaticLib:
                txt->SetValue(target->GetOutputFilename());
                txt->Enable(true);
                txtW->SetValue(target->GetWorkingDir());
                txtW->Enable((TargetType)cmb->GetSelection() == ttExecutable ||
                            (TargetType)cmb->GetSelection() == ttConsoleOnly ||
                            (TargetType)cmb->GetSelection() == ttDynamicLib);
                txtO->SetValue(target->GetObjectOutput());
                txtO->Enable(true);
                browse->Enable(true);
                browseW->Enable((TargetType)cmb->GetSelection() == ttExecutable ||
                                (TargetType)cmb->GetSelection() == ttConsoleOnly ||
                                (TargetType)cmb->GetSelection() == ttDynamicLib);
                browseO->Enable(true);
                break;

            default: // for commands-only targets
                txt->SetValue(_T(""));
                txt->Enable(false);
                txtW->SetValue(_T(""));
                txtW->Enable(false);
                txtO->SetValue(_T(""));
                txtO->Enable(false);
                browse->Enable(false);
                browseW->Enable(false);
                browseO->Enable(false);
                break;
        }
    }

	// files options
	wxCheckListBox* list = XRCCTRL(*this, "lstFiles", wxCheckListBox);
	int count = list->GetCount();
	for (int i = 0; i < count; ++i)
	{
		ProjectFile* pf = m_Project->GetFile(i);
		if (!pf)
			break;

		bool doit = pf->buildTargets.Index(target->GetTitle()) >= 0;
		list->Check(i, doit);
	}

    // update currently selected target
	m_Current_Sel = lstTargets->GetSelection();
}

void ProjectOptionsDlg::DoBeforeTargetChange(bool force)
{
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);

    // if no previously selected target, exit
    if (m_Current_Sel == -1)
        return;

    if (force || lstTargets->GetSelection() != m_Current_Sel)
    {
        // selected another build target
        // save changes to the previously selected target
        wxArrayString array;
		ProjectBuildTarget* target = m_Project->GetBuildTarget(m_Current_Sel);
		if (!target)
			return;

        target->SetUseConsoleRunner(XRCCTRL(*this, "chkUseConsoleRunner", wxCheckBox)->GetValue());
		target->SetIncludeInTargetAll(XRCCTRL(*this, "chkBuildThisTarget", wxCheckBox)->GetValue());
        target->SetCreateDefFile(XRCCTRL(*this, "chkCreateDefFile", wxCheckBox)->GetValue());
        target->SetCreateStaticLib(XRCCTRL(*this, "chkCreateStaticLib", wxCheckBox)->GetValue());

		// global project options
		target->SetTargetType(TargetType(XRCCTRL(*this, "cmbProjectType", wxComboBox)->GetSelection()));
		wxFileName fname(XRCCTRL(*this, "txtOutputFilename", wxTextCtrl)->GetValue());
//		fname.Normalize(wxPATH_NORM_ALL & ~wxPATH_NORM_CASE, m_Project->GetBasePath());
//		fname.MakeRelativeTo(m_Project->GetBasePath());
		target->SetOutputFilename(fname.GetFullPath());

		fname.Assign(XRCCTRL(*this, "txtWorkingDir", wxTextCtrl)->GetValue());
//		fname.Normalize(wxPATH_NORM_ALL & ~wxPATH_NORM_CASE, m_Project->GetBasePath());
//		fname.MakeRelativeTo(m_Project->GetBasePath());
        target->SetWorkingDir(fname.GetFullPath());

		fname.Assign(XRCCTRL(*this, "txtObjectDir", wxTextCtrl)->GetValue());
//		fname.Normalize(wxPATH_NORM_ALL & ~wxPATH_NORM_CASE, m_Project->GetBasePath());
//		fname.MakeRelativeTo(m_Project->GetBasePath());
		target->SetObjectOutput(fname.GetFullPath());

		// files options
		wxCheckListBox* list = XRCCTRL(*this, "lstFiles", wxCheckListBox);
		int count = list->GetCount();
		for (int i = 0; i < count; ++i)
		{
			ProjectFile* pf = m_Project->GetFile(i);
			if (!pf)
				break;

			if (list->IsChecked(i))
				pf->AddBuildTarget(target->GetTitle());
			else
				pf->RemoveBuildTarget(target->GetTitle());
		}
	}
}

// events

void ProjectOptionsDlg::OnProjectTypeChanged(wxCommandEvent& event)
{
    ProjectBuildTarget* target = m_Project->GetBuildTarget(m_Current_Sel);
    if (!target)
        return;
	wxComboBox* cmb = XRCCTRL(*this, "cmbProjectType", wxComboBox);
    wxTextCtrl* txt = XRCCTRL(*this, "txtOutputFilename", wxTextCtrl);
    wxTextCtrl* txtW = XRCCTRL(*this, "txtWorkingDir", wxTextCtrl);
    wxTextCtrl* txtO = XRCCTRL(*this, "txtObjectDir", wxTextCtrl);
    wxButton* browse = XRCCTRL(*this, "btnBrowseOutputFilename", wxButton);
    wxButton* browseW = XRCCTRL(*this, "btnBrowseWorkingDir", wxButton);
    wxButton* browseO = XRCCTRL(*this, "btnBrowseObjectDir", wxButton);
    if (!cmb || !txt || !browse)
        return;

    XRCCTRL(*this, "chkCreateDefFile", wxCheckBox)->Enable(cmb->GetSelection() == ttStaticLib ||
                                                            cmb->GetSelection() == ttDynamicLib);
    XRCCTRL(*this, "chkCreateStaticLib", wxCheckBox)->Enable(cmb->GetSelection() == ttDynamicLib);

    txt->Enable(true);
    txtW->SetValue(target->GetWorkingDir());
    txtW->Enable((TargetType)cmb->GetSelection() == ttExecutable ||
                (TargetType)cmb->GetSelection() == ttConsoleOnly ||
                (TargetType)cmb->GetSelection() == ttDynamicLib);
    txtO->Enable(true);
    txtO->SetValue(target->GetObjectOutput());
    browse->Enable(true);
    browseW->Enable((TargetType)cmb->GetSelection() == ttExecutable ||
                    (TargetType)cmb->GetSelection() == ttConsoleOnly ||
                    (TargetType)cmb->GetSelection() == ttDynamicLib);
    browseO->Enable(true);

    Compiler* compiler = CompilerFactory::GetCompiler(target->GetCompilerID());

    wxFileName fname = target->GetOutputFilename();
    wxString name = fname.GetName();
    wxString ext = fname.GetExt();
    wxString libext = compiler->GetSwitches().libExtension;
    wxString libpre = compiler->GetSwitches().libPrefix;
    switch ((TargetType)cmb->GetSelection())
    {
        case ttConsoleOnly:
        case ttExecutable:
            if (ext != EXECUTABLE_EXT)
                fname.SetExt(EXECUTABLE_EXT);
            if (!libpre.IsEmpty() && name.StartsWith(libpre))
            {
                name.Remove(0, libpre.Length());
                fname.SetName(name);
            }
            txt->SetValue(fname.GetFullPath());
            break;
        case ttDynamicLib:
            if (ext != DYNAMICLIB_EXT)
                fname.SetExt(DYNAMICLIB_EXT);
            if (!libpre.IsEmpty() && name.StartsWith(libpre))
            {
                name.Remove(0, libpre.Length());
                fname.SetName(name);
            }
            txt->SetValue(fname.GetFullPath());
            break;
        case ttStaticLib:
            if (ext != libext)
                fname.SetExt(libext);
            if (!libpre.IsEmpty() && !name.StartsWith(libpre))
            {
                name.Prepend(libpre);
                fname.SetName(name);
            }
            txt->SetValue(fname.GetFullPath());
            break;
        case ttCommandsOnly:
            txt->SetValue(_T(""));
            txtW->SetValue(_T(""));
            txtO->SetValue(_T(""));
            txt->Enable(false);
            txtW->Enable(false);
            txtO->Enable(false);
            browse->Enable(false);
            browseW->Enable(false);
            browseO->Enable(false);
            break;
    }
}

void ProjectOptionsDlg::OnBuildTargetChanged(wxCommandEvent& event)
{
    DoTargetChange();
    CodeBlocksEvent e(cbEVT_PROJECT_TARGETS_MODIFIED);
    Manager::Get()->GetPluginManager()->NotifyPlugins(e);
}

void ProjectOptionsDlg::OnBuildOrderClick(wxCommandEvent& event)
{
    wxArrayString array;
    for (int i = 0; i < m_Project->GetBuildTargetsCount(); ++i)
        array.Add(m_Project->GetBuildTarget(i)->GetTitle());

    EditArrayOrderDlg dlg(this, array);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        DoBeforeTargetChange(); // save changes in current target
        m_Project->ReOrderTargets(dlg.GetArray());
        m_Current_Sel = -1; // force no "save changes" for next call
        FillBuildTargets();
    }
    CodeBlocksEvent e(cbEVT_PROJECT_TARGETS_MODIFIED);
    Manager::Get()->GetPluginManager()->NotifyPlugins(e);
}

void ProjectOptionsDlg::OnProjectDepsClick(wxCommandEvent& event)
{
    Manager::Get()->GetProjectManager()->ConfigureProjectDependencies(m_Project);
}

void ProjectOptionsDlg::OnProjectBuildOptionsClick(wxCommandEvent& event)
{
	if (m_pCompiler)
		m_pCompiler->Configure(m_Project);
}

void ProjectOptionsDlg::OnTargetBuildOptionsClick(wxCommandEvent& event)
{
	if (m_pCompiler)
	{
		wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
		int targetIdx = lstTargets->GetSelection();

		ProjectBuildTarget* target = m_Project->GetBuildTarget(targetIdx);
		if (target)
			m_pCompiler->Configure(m_Project, target);
	}
}

void ProjectOptionsDlg::OnAddBuildTargetClick(wxCommandEvent& event)
{
    wxString targetName = wxGetTextFromUser(_("Enter the new build target name:"),
                                            _("New build target"));
    if (targetName.IsEmpty())
        return;
    if (m_Project->GetBuildTarget(targetName))
    {
        wxMessageDialog(this, _("A target with this name already exists in this project!"),
                                _("Error"),
                                wxOK | wxCENTRE | wxICON_ERROR);
        return;
    }

    ProjectBuildTarget* target = m_Project->AddBuildTarget(targetName);
    if (!target)
    {
        wxMessageDialog(this, _("The new target could not be added..."),
                                _("Error"),
                                wxOK | wxCENTRE | wxICON_ERROR);
        return;
    }

    // add target to targets combo
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
    lstTargets->Append(targetName);
    lstTargets->SetSelection(lstTargets->GetCount() - 1);
    DoTargetChange();
    BuildScriptsTree();
    CodeBlocksEvent e(cbEVT_PROJECT_TARGETS_MODIFIED);
    Manager::Get()->GetPluginManager()->NotifyPlugins(e);
}

void ProjectOptionsDlg::OnEditBuildTargetClick(wxCommandEvent& event)
{
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
    int targetIdx = lstTargets->GetSelection();

    ProjectBuildTarget* target = m_Project->GetBuildTarget(targetIdx);
    if (!target)
    {
        wxMessageDialog(this, _("Could not locate target..."),
                                _("Error"),
                                wxOK | wxCENTRE | wxICON_ERROR);
        return;
    }

    wxString oldTargetName = target->GetTitle();
    wxString newTargetName = wxGetTextFromUser(_("Change the build target name:"),
                                               _("Rename build target"),
                                              oldTargetName);
    if (newTargetName.IsEmpty())
        return;
    m_Project->RenameBuildTarget(targetIdx, newTargetName);
    lstTargets->SetString(targetIdx, newTargetName);
    lstTargets->SetSelection(targetIdx);
    BuildScriptsTree();
    CodeBlocksEvent e(cbEVT_PROJECT_TARGETS_MODIFIED);
    Manager::Get()->GetPluginManager()->NotifyPlugins(e);
}

void ProjectOptionsDlg::OnCopyBuildTargetClick(wxCommandEvent& event)
{
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
    int targetIdx = lstTargets->GetSelection();

    ProjectBuildTarget* target = m_Project->GetBuildTarget(targetIdx);
    if (!target)
    {
        wxMessageDialog(this, _("Could not locate target..."),
                                _("Error"),
                                wxOK | wxCENTRE | wxICON_ERROR);
        return;
    }

    wxString newTargetName = wxGetTextFromUser(_("Enter the duplicated build target's name:"),
                                               _("Duplicate build target"),
                                              _("Copy of ") + target->GetTitle());
    if (newTargetName.IsEmpty())
        return;
    if (!m_Project->DuplicateBuildTarget(targetIdx, newTargetName))
    {
        wxMessageBox(_("Failed to duplicate this build target..."), _("Error"), wxICON_ERROR);
        return;
    }

    // add target to targets combo
    lstTargets->Append(newTargetName);
    lstTargets->SetSelection(lstTargets->GetCount() - 1);
    DoTargetChange();
    BuildScriptsTree();
    CodeBlocksEvent e(cbEVT_PROJECT_TARGETS_MODIFIED);
    Manager::Get()->GetPluginManager()->NotifyPlugins(e);
}

void ProjectOptionsDlg::OnRemoveBuildTargetClick(wxCommandEvent& event)
{
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
    int targetIdx = lstTargets->GetSelection();

    wxString caption;
    caption.Printf(_("Are you sure you want to delete build target \"%s\"?"), lstTargets->GetStringSelection().c_str());
    wxMessageDialog dlg(this,
                        caption,
                        _("Confirmation"),
                        wxYES_NO | wxNO_DEFAULT | wxCENTRE | wxICON_QUESTION);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_NO)
        return;

    // the target name in the units is changed by the project...
    m_Project->RemoveBuildTarget(targetIdx);
    lstTargets->Delete(targetIdx);
    lstTargets->SetSelection(targetIdx);
    m_Current_Sel = -1;
    DoTargetChange();
    BuildScriptsTree();
    CodeBlocksEvent e(cbEVT_PROJECT_TARGETS_MODIFIED);
    Manager::Get()->GetPluginManager()->NotifyPlugins(e);
}

void ProjectOptionsDlg::OnEditDepsClick(wxCommandEvent& event)
{
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
	ProjectBuildTarget* target = m_Project->GetBuildTarget(lstTargets->GetSelection());
	if (!target)
		return;

	ExternalDepsDlg dlg(this, m_Project, target);
    PlaceWindow(&dlg);
	dlg.ShowModal();
}

void ProjectOptionsDlg::OnExportTargetClick(wxCommandEvent& event)
{
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
	ProjectBuildTarget* target = m_Project->GetBuildTarget(lstTargets->GetSelection());
	if (!target)
        return;

    AnnoyingDialog dlg(_("Create project from target confirmation"),
                        _("This project will be saved before exporting the build target.\n"
                        "Are you sure you want to export the selected "
                        "build target to a new project?"),
                        wxART_QUESTION,
                        AnnoyingDialog::YES_NO,
                        wxID_YES);
    if (dlg.ShowModal() == wxID_YES)
    {
        if (m_Project->ExportTargetAsProject(target->GetTitle()))
            wxMessageBox(_("New project created succesfully!"), _("Information"), wxICON_INFORMATION);
    }
}

void ProjectOptionsDlg::OnBrowseDirClick(wxCommandEvent& event)
{
    wxTextCtrl* targettext = 0;
    if (event.GetId() == XRCID("btnBrowseWorkingDir"))
        targettext = XRCCTRL(*this, "txtWorkingDir", wxTextCtrl);
    else if (event.GetId() == XRCID("btnBrowseObjectDir"))
        targettext = XRCCTRL(*this, "txtObjectDir", wxTextCtrl);
    else
        return;

    wxFileName fname(targettext->GetValue() + wxFileName::GetPathSeparator());
    fname.Normalize(wxPATH_NORM_ALL & ~wxPATH_NORM_CASE, m_Project->GetBasePath());

    wxString path = ChooseDirectory(this,
                                    _("Select directory"),
                                    fname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR),
                                    m_Project->GetBasePath(),
                                    true,
                                    true);
    if (path.IsEmpty())
        return;

    fname.Assign(path);
    targettext->SetValue(path);
}

void ProjectOptionsDlg::OnBrowseOutputFilenameClick(wxCommandEvent& event)
{
    wxFileName fname;
    fname.Assign(XRCCTRL(*this, "txtOutputFilename", wxTextCtrl)->GetValue());
    fname.Normalize(wxPATH_NORM_ALL & ~wxPATH_NORM_CASE, m_Project->GetBasePath());
    wxFileDialog dlg(this,
                    _("Select output filename"),
                    fname.GetPath(),
                    fname.GetFullName(),
                    ALL_FILES_FILTER,
                    wxSAVE | wxOVERWRITE_PROMPT);

    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
        return;
    fname.Assign(dlg.GetPath());
    fname.MakeRelativeTo(m_Project->GetBasePath());
    XRCCTRL(*this, "txtOutputFilename", wxTextCtrl)->SetValue(fname.GetFullPath());
}

void ProjectOptionsDlg::OnFileOptionsClick(wxCommandEvent& event)
{
    wxCheckListBox* list = XRCCTRL(*this, "lstFiles", wxCheckListBox);

    if (list->GetSelection() >= 0)
	{
		// show file options dialog
		ProjectFile* pf = m_Project->GetFile(list->GetSelection());
		pf->ShowOptions(this);
	}
}

void ProjectOptionsDlg::OnFileToggleMarkClick(wxCommandEvent& event)
{
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
    int targetIdx = lstTargets->GetSelection();
    ProjectBuildTarget* target = m_Project->GetBuildTarget(targetIdx);

    wxCheckListBox* list = XRCCTRL(*this, "lstFiles", wxCheckListBox);
    for (int i = 0; i < list->GetCount(); ++i)
	{
		ProjectFile* pf = m_Project->GetFile(i);
		list->Check(i, !list->IsChecked(i));
        if (list->IsChecked(i))
            pf->AddBuildTarget(target->GetTitle());
        else
            pf->RemoveBuildTarget(target->GetTitle());
	}
}

void ProjectOptionsDlg::OnScriptsOverviewSelChanged(wxTreeEvent& event)
{
    FillScripts();
}

bool ProjectOptionsDlg::IsScriptValid(const wxString& script)
{
    int r = Manager::Get()->GetScriptingManager()->LoadScript(m_Project->GetBasePath() + wxFILE_SEP_PATH + script, _T("test_module"), false);
    Manager::Get()->GetScriptingManager()->GetEngine()->Discard("test_module");
    return r == 0;
}

bool ProjectOptionsDlg::DoCheckScripts(CompileTargetBase* base)
{
    const wxArrayString& scripts = base->GetBuildScripts();
    for (size_t i = 0; i < scripts.GetCount(); ++i)
    {
        if (!IsScriptValid(scripts[i]))
        {
            wxString msg;
            msg << _("Invalid build script: ") + scripts[i] << _T('\n');
            msg << _("First seen in: ") + base->GetTitle() << _T('\n');
            wxMessageBox(msg, _("Error"), wxICON_ERROR);
            return false;
        }
    }
    return true;
}

void ProjectOptionsDlg::OnCheckScripts(wxCommandEvent& event)
{
    if (!DoCheckScripts(m_Project))
        return;

    for (int i = 0; i < m_Project->GetBuildTargetsCount(); ++i)
    {
        if (!DoCheckScripts(m_Project->GetBuildTarget(i)))
            return;
    }

    wxMessageBox(_("All scripts seem to be valid!"), _("Information"), wxICON_INFORMATION);
}

void ProjectOptionsDlg::OnAddScript(wxCommandEvent& event)
{
    wxListBox* ctrl = XRCCTRL(*this, "lstPreScripts", wxListBox);
    if (!ctrl)
        return;

    wxFileName fname;
    fname.Assign(ctrl->GetStringSelection());
    fname.Normalize(wxPATH_NORM_ALL & ~wxPATH_NORM_CASE, m_Project->GetBasePath());
    wxFileDialog dlg(this,
                    _("Select script file"),
                    fname.GetPath(),
                    fname.GetFullName(),
                    _("Script files (*.script)|*.script"),
                    wxOPEN);

    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
        return;
    fname.Assign(dlg.GetPath());
    fname.MakeRelativeTo(m_Project->GetBasePath());
    ctrl->Append(fname.GetFullPath());
    ctrl->SetSelection(ctrl->GetCount());

	wxTreeCtrl* tc = XRCCTRL(*this, "tcOverview", wxTreeCtrl);
	wxTreeItemId sel = tc->GetSelection();
    CompileOptionsBase* base = sel == tc->GetRootItem()
                                ? static_cast<CompileOptionsBase*>(m_Project)
                                : static_cast<CompileOptionsBase*>(m_Project->GetBuildTarget(tc->GetItemText(sel)));
    base->AddBuildScript(fname.GetFullPath());
}

void ProjectOptionsDlg::OnRemoveScript(wxCommandEvent& event)
{
    wxListBox* ctrl = XRCCTRL(*this, "lstPreScripts", wxListBox);
    if (!ctrl || ctrl->GetSelection() == -1)
        return;
    wxString script = ctrl->GetStringSelection();
    if (script.IsEmpty())
        return;

	wxTreeCtrl* tc = XRCCTRL(*this, "tcOverview", wxTreeCtrl);
	wxTreeItemId sel = tc->GetSelection();
    CompileOptionsBase* base = sel == tc->GetRootItem()
                                ? static_cast<CompileOptionsBase*>(m_Project)
                                : static_cast<CompileOptionsBase*>(m_Project->GetBuildTarget(tc->GetItemText(sel)));
    base->RemoveBuildScript(script);
    int isel = ctrl->GetSelection();
    ctrl->Delete(isel);
    ctrl->SetSelection(isel < ctrl->GetCount() ? isel : --isel );
}

void ProjectOptionsDlg::OnScriptMoveUp(wxSpinEvent& event)
{
    wxListBox* ctrl = XRCCTRL(*this, "lstPreScripts", wxListBox);
    if (!ctrl || ctrl->GetSelection() <= 0)
        return;

	wxTreeCtrl* tc = XRCCTRL(*this, "tcOverview", wxTreeCtrl);
	wxTreeItemId sel = tc->GetSelection();
    CompileOptionsBase* base = sel == tc->GetRootItem()
                                ? static_cast<CompileOptionsBase*>(m_Project)
                                : static_cast<CompileOptionsBase*>(m_Project->GetBuildTarget(tc->GetItemText(sel)));

    int isel = ctrl->GetSelection();
    wxString ssel = ctrl->GetStringSelection();
    wxArrayString scripts = base->GetBuildScripts();
    scripts.RemoveAt(isel);
    ctrl->Delete(isel);
    --isel;
    ctrl->Insert(ssel, isel);
    scripts.Insert(ssel, isel);
    ctrl->SetSelection(isel);
    base->SetBuildScripts(scripts);
}

void ProjectOptionsDlg::OnScriptMoveDown(wxSpinEvent& event)
{
    wxListBox* ctrl = XRCCTRL(*this, "lstPreScripts", wxListBox);
    if (!ctrl || ctrl->GetSelection() == ctrl->GetCount() - 1)
        return;

	wxTreeCtrl* tc = XRCCTRL(*this, "tcOverview", wxTreeCtrl);
	wxTreeItemId sel = tc->GetSelection();
    CompileOptionsBase* base = sel == tc->GetRootItem()
                                ? static_cast<CompileOptionsBase*>(m_Project)
                                : static_cast<CompileOptionsBase*>(m_Project->GetBuildTarget(tc->GetItemText(sel)));

    int isel = ctrl->GetSelection();
    wxString ssel = ctrl->GetStringSelection();
    wxArrayString scripts = base->GetBuildScripts();
    scripts.RemoveAt(isel);
    ctrl->Delete(isel);
    ++isel;
    ctrl->Insert(ssel, isel);
    scripts.Insert(ssel, isel);
    ctrl->SetSelection(isel);
    base->SetBuildScripts(scripts);
}

void ProjectOptionsDlg::OnUpdateUI(wxUpdateUIEvent& event)
{
    wxListBox* lstTargets = XRCCTRL(*this, "lstBuildTarget", wxListBox);
    wxCheckListBox* list = XRCCTRL(*this, "lstFiles", wxCheckListBox);

    bool filesEn = list->GetSelection() >= 0;
    bool en = lstTargets->GetSelection() >= 0;

    // files options
	XRCCTRL(*this, "btnFileOptions", wxButton)->Enable(filesEn);

    // target options
    XRCCTRL(*this, "btnBuildOrder", wxButton)->Enable(lstTargets->GetCount() > 1);
    XRCCTRL(*this, "btnEditBuildTarget", wxButton)->Enable(en);
    XRCCTRL(*this, "btnCopyBuildTarget", wxButton)->Enable(en);
    XRCCTRL(*this, "btnDelBuildTarget", wxButton)->Enable(en && lstTargets->GetCount() > 1);

	// edit project/target build options
    XRCCTRL(*this, "btnProjectBuildOptions", wxButton)->Enable(m_pCompiler);
    XRCCTRL(*this, "btnTargetBuildOptions", wxButton)->Enable(m_pCompiler && en);
    XRCCTRL(*this, "btnExportTarget", wxButton)->Enable(en);

    // disable some stuff if using a custom makefile
    bool customMake = XRCCTRL(*this, "chkCustomMakefile", wxCheckBox)->GetValue();
    XRCCTRL(*this, "rbPCHStrategy", wxRadioBox)->Enable(!customMake);
    XRCCTRL(*this, "txtObjectDir", wxTextCtrl)->Enable(!customMake && en);
    XRCCTRL(*this, "txtObjectDir", wxTextCtrl)->Enable(!customMake && en);
    XRCCTRL(*this, "btnBrowseObjectDir", wxButton)->Enable(!customMake && en);
    XRCCTRL(*this, "btnToggleCheckmarks", wxButton)->Enable(!customMake && en);
    list->Enable(!customMake);

    // scripts page
	wxTreeCtrl* tc = XRCCTRL(*this, "tcOverview", wxTreeCtrl);
	tc->Enable(!customMake);
	bool scrsel = !customMake && tc->GetSelection().IsOk();

    wxListBox* lstPreScripts = XRCCTRL(*this, "lstPreScripts", wxListBox);
    lstPreScripts->Enable(scrsel);
    bool presel = lstPreScripts->GetSelection() != -1;
    XRCCTRL(*this, "btnAddPreScripts", wxButton)->Enable(scrsel);
    XRCCTRL(*this, "btnRemovePreScripts", wxButton)->Enable(scrsel && presel);
    XRCCTRL(*this, "spnPreScripts", wxSpinButton)->Enable(scrsel && presel && lstPreScripts->GetCount() > 1);
    XRCCTRL(*this, "btnCheckScripts", wxSpinButton)->Enable(!customMake);
}

void ProjectOptionsDlg::OnOK(wxCommandEvent& event)
{

	m_Project->SetTitle(XRCCTRL(*this, "txtProjectName", wxTextCtrl)->GetValue());
	m_Project->RenameInTree(m_Project->GetTitle());
	m_Project->SetMakefile(XRCCTRL(*this, "txtProjectMakefile", wxTextCtrl)->GetValue());
	m_Project->SetMakefileCustom(XRCCTRL(*this, "chkCustomMakefile", wxCheckBox)->GetValue());
	m_Project->SetTargetType(TargetType(XRCCTRL(*this, "cmbProjectType", wxComboBox)->GetSelection()));
	m_Project->SetModeForPCH((PCHMode)XRCCTRL(*this, "rbPCHStrategy", wxRadioBox)->GetSelection());

#if 1
    if (m_Current_Sel == -1)
        m_Current_Sel = 0; // force update of global options
    DoBeforeTargetChange(true);
#endif

    EndModal(wxID_OK);
}

