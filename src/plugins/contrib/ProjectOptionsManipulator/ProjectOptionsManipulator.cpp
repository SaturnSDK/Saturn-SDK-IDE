#include "ProjectOptionsManipulator.h"

#include <cbproject.h>
#include <globals.h> // cbMessageBox
#include <manager.h>
#include <projectmanager.h>

#include "ProjectOptionsManipulatorDlg.h"
#include "ProjectOptionsManipulatorResultDlg.h"

// Register the plugin
namespace
{
  PluginRegistrant<ProjectOptionsManipulator> reg(_T("ProjectOptionsManipulator"));
  long ID_PROJECT_OPTIONS_DLG        = wxNewId();
  long ID_PROJECT_OPTIONS_RESULT_DLG = wxNewId();
};

// Identifiers for settings dialog
BEGIN_EVENT_TABLE(ProjectOptionsManipulator, cbPlugin)
END_EVENT_TABLE()

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

ProjectOptionsManipulator::ProjectOptionsManipulator() :
  m_Dlg(NULL)
{
  //ctor
}// ProjectOptionsManipulator

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

ProjectOptionsManipulator::~ProjectOptionsManipulator()
{
  //dtor
}//~ProjectOptionsManipulator

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void ProjectOptionsManipulator::OnAttach()
{
  // do whatever initialization you need for your plugin
  // NOTE: after this function, the inherited member variable
  // m_IsAttached will be TRUE...
  // You should check for it in other functions, because if it
  // is FALSE, it means that the application did *not* "load"
  // (see: does not need) this plugin...

  m_Dlg = new ProjectOptionsManipulatorDlg( Manager::Get()->GetAppWindow(), ID_PROJECT_OPTIONS_DLG );
}// OnAttach

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void ProjectOptionsManipulator::OnRelease(bool WXUNUSED(appShutDown))
{
  // do de-initialization for your plugin
  // if appShutDown is false, the plugin is unloaded because Code::Blocks is being shut down,
  // which means you must not use any of the SDK Managers
  // NOTE: after this function, the inherited member variable
  // m_IsAttached will be FALSE...

  if (m_Dlg)
    m_Dlg->Destroy();
}// OnRelease

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

int ProjectOptionsManipulator::Execute()
{
  if ( !IsAttached() || !m_Dlg )
    return -1;

  wxArrayString result;
  if ( m_Dlg->ShowModal()==wxID_OK )
  {
    if      ( m_Dlg->GetScanForWorkspace() )
    {
      if ( !OperateWorkspace(result) )
      {
        cbMessageBox(_("Processing options for workspace failed!"), _("Error"),
                     wxICON_ERROR, Manager::Get()->GetAppWindow());
        return -1;
      }
    }
    else if ( m_Dlg->GetScanForProject()   )
    {
      if ( !OperateProject( m_Dlg->GetProjectIdx(), result ) )
      {
        cbMessageBox(_("Processing options for project failed!"), _("Error"),
                     wxICON_ERROR, Manager::Get()->GetAppWindow());
        return -1;
      }
    }
  }

  if ( !result.IsEmpty() )
  {
    ProjectOptionsManipulatorResultDlg dlg( Manager::Get()->GetAppWindow(),ID_PROJECT_OPTIONS_RESULT_DLG );
    dlg.ApplyResult(result);
    dlg.ShowModal();
  }

  return 0;
}// Execute

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

bool ProjectOptionsManipulator::OperateWorkspace(wxArrayString& result)
{
  ProjectsArray* pa = Manager::Get()->GetProjectManager()->GetProjects();
  bool success = true;
  if (pa)
  {
    for (size_t i=0; i<pa->GetCount(); ++i)
      success &= OperateProject( pa->Item(i), result );
  }

  return success;
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

bool ProjectOptionsManipulator::OperateProject(size_t prj_idx, wxArrayString& result)
{
  ProjectsArray* pa = Manager::Get()->GetProjectManager()->GetProjects();
  bool success = true;
  if (pa)
    success &= OperateProject( pa->Item(prj_idx), result );

  return success;
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

bool ProjectOptionsManipulator::OperateProject(cbProject* prj, wxArrayString& result)
{
  if (!prj) return false;

  const wxString opt = m_Dlg->GetOption();
  if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eLinker) )
    ProcessLinkerOptions(prj, opt, result);

  if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eCompiler) )
    ProcessCompilerOptions(prj, opt, result);

  return true;
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void ProjectOptionsManipulator::ProcessLinkerOptions(cbProject* prj, const wxString& opt, wxArrayString& result)
{
  ProjectOptionsManipulatorDlg::EProjectScanOption scan_opt = m_Dlg->GetScanOption();
  switch (scan_opt)
  {
    case ProjectOptionsManipulatorDlg::eSearch:
    case ProjectOptionsManipulatorDlg::eSearchNot:
    {
      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eProject) )
      {
        const wxArrayString lo = prj->GetLinkerOptions();
        int idx = lo.Index(opt);
        if (idx!=wxNOT_FOUND)
        {
          result.Add(wxString::Format(wxT("Project '%s': Contains linker option '%s'."),
                                   prj->GetTitle().wx_str(), opt.wx_str()));
        }
      }

      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eTarget) )
      {
        for (int i=0; i<prj->GetBuildTargetsCount(); ++i)
        {
          ProjectBuildTarget* tgt = prj->GetBuildTarget(i);
          if (tgt)
          {
            const wxArrayString lo = tgt->GetLinkerOptions();
            int idx = lo.Index(opt);
            if (idx!=wxNOT_FOUND)
            {
              result.Add(wxString::Format(wxT("Project '%s', target '%s': Contains linker option '%s'."),
                                      prj->GetTitle().wx_str(), tgt->GetTitle().wx_str(), opt.wx_str()));
            }
          }
        }
      }
    }
    break;

    case ProjectOptionsManipulatorDlg::eRemove:
    {
      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eProject) )
        prj->RemoveLinkerOption(opt);

      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eTarget) )
      {
        for (int i=0; i<prj->GetBuildTargetsCount(); ++i)
        {
          ProjectBuildTarget* tgt = prj->GetBuildTarget(i);
          if (tgt) tgt->RemoveLinkerOption(opt);
        }
      }
    }
    break;

    case ProjectOptionsManipulatorDlg::eAdd:
    {
      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eProject) )
        prj->AddLinkerOption(opt);

      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eTarget) )
      {
        for (int i=0; i<prj->GetBuildTargetsCount(); ++i)
        {
          ProjectBuildTarget* tgt = prj->GetBuildTarget(i);
          if (tgt) tgt->AddLinkerOption(opt);
        }
      }
    }
    break;

    default:
    break;
  }
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void ProjectOptionsManipulator::ProcessCompilerOptions(cbProject* prj, const wxString& opt, wxArrayString& result)
{
  ProjectOptionsManipulatorDlg::EProjectScanOption scan_opt = m_Dlg->GetScanOption();
  switch (scan_opt)
  {
    case ProjectOptionsManipulatorDlg::eSearch:
    case ProjectOptionsManipulatorDlg::eSearchNot:
    {
      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eProject) )
      {
        const wxArrayString co = prj->GetCompilerOptions();
        int idx = co.Index(opt);
        if (idx==wxNOT_FOUND && scan_opt==ProjectOptionsManipulatorDlg::eSearchNot)
        {
          result.Add(wxString::Format(wxT("Project '%s': Does not contain compiler option '%s'."),
                                   prj->GetTitle().wx_str(), opt.wx_str()));
        }
        else if (idx!=wxNOT_FOUND && scan_opt==ProjectOptionsManipulatorDlg::eSearch)
        {
          result.Add(wxString::Format(wxT("Project '%s': Contains compiler option '%s'."),
                                   prj->GetTitle().wx_str(), opt.wx_str()));
        }
      }

      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eTarget) )
      {
        for (int i=0; i<prj->GetBuildTargetsCount(); ++i)
        {
          ProjectBuildTarget* tgt = prj->GetBuildTarget(i);
          if (tgt)
          {
            const wxArrayString co = tgt->GetCompilerOptions();
            int idx = co.Index(opt);
            if (idx==wxNOT_FOUND && scan_opt==ProjectOptionsManipulatorDlg::eSearchNot)
            {
              result.Add(wxString::Format(wxT("Project '%s', target '%s': Does not contain compiler option '%s'."),
                                      prj->GetTitle().wx_str(), tgt->GetTitle().wx_str(), opt.wx_str()));
            }
            else if (idx!=wxNOT_FOUND && scan_opt==ProjectOptionsManipulatorDlg::eSearch)
            {
              result.Add(wxString::Format(wxT("Project '%s', target '%s': Contains compiler option '%s'."),
                                      prj->GetTitle().wx_str(), tgt->GetTitle().wx_str(), opt.wx_str()));
            }
          }
        }
      }
    }
    break;

    case ProjectOptionsManipulatorDlg::eRemove:
    {
      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eProject) )
        prj->RemoveCompilerOption(opt);

      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eTarget) )
      {
        for (int i=0; i<prj->GetBuildTargetsCount(); ++i)
        {
          ProjectBuildTarget* tgt = prj->GetBuildTarget(i);
          if (tgt) tgt->RemoveCompilerOption(opt);
        }
      }
    }
    break;

    case ProjectOptionsManipulatorDlg::eAdd:
    {
      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eProject) )
        prj->AddCompilerOption(opt);

      if ( m_Dlg->GetOptionActive(ProjectOptionsManipulatorDlg::eTarget) )
      {
        for (int i=0; i<prj->GetBuildTargetsCount(); ++i)
        {
          ProjectBuildTarget* tgt = prj->GetBuildTarget(i);
          if (tgt) tgt->AddCompilerOption(opt);
        }
      }
    }
    break;

    default:
    break;
  }
}
