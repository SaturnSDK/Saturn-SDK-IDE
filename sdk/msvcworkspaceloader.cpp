#include <wx/stream.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/dynarray.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

#include "msvcworkspaceloader.h"

#include "globals.h"
#include "importers_globals.h"
#include "manager.h"
#include "messagemanager.h"
#include "projectmanager.h"
#include "compilerfactory.h"
#include "compiler.h"

MSVCWorkspaceLoader::MSVCWorkspaceLoader()
{
	//ctor
}

MSVCWorkspaceLoader::~MSVCWorkspaceLoader()
{
	//dtor
}

bool MSVCWorkspaceLoader::Open(const wxString& filename)
{
    bool askForCompiler = false;
    bool askForTargets = false;
    switch (wxMessageBox(_("Do you want the imported projects to use the default compiler?\n"
                        "(If you answer No, you will be asked for each and every project"
                        " which compiler to use...)"), _("Question"), wxICON_QUESTION | wxYES_NO | wxCANCEL))
    {
        case wxYES: askForCompiler = false; break;
        case wxNO: askForCompiler = true; break;
        case wxCANCEL: return false;
    }
    switch (wxMessageBox(_("Do you want to import all configurations (e.g. Debug/Release) from the "
                        "imported projects?\n"
                        "(If you answer No, you will be asked for each and every project"
                        " which configurations to import...)"), _("Question"), wxICON_QUESTION | wxYES_NO | wxCANCEL))
    {
        case wxYES: askForTargets = false; break;
        case wxNO: askForTargets = true; break;
        case wxCANCEL: return false;
    }

    wxFileInputStream file(filename);
    if (!file.Ok())
        return false; // error opening file???

    wxArrayString comps;
    wxTextInputStream input(file);

    // read "header"
    if (!file.Eof())
    {
        wxString line = input.ReadLine();
        if (line.IsEmpty())
        {
            Manager::Get()->GetMessageManager()->DebugLog(_("Unsupported format."));
            return false;
        }
        comps = GetArrayFromString(line, _T(","));
        line = comps[0];
        line.Trim(true);
        line.Trim(false);
        if (line != _T("Microsoft Developer Studio Workspace File"))
        {
            Manager::Get()->GetMessageManager()->DebugLog(_("Unsupported format."));
            return false;
        }
        line = comps.GetCount() > 1 ? comps[1] : wxString(wxEmptyString);
        line.Trim(true);
        line.Trim(false);
        if (line != _T("Format Version 6.00"))
            Manager::Get()->GetMessageManager()->DebugLog(_("Format not recognized. Will try to parse though..."));
    }

    ImportersGlobals::UseDefaultCompiler = !askForCompiler;
    ImportersGlobals::ImportAllTargets = !askForTargets;

    int count = 0;
    cbProject* project = 0;
    wxFileName wfname = filename;
    wfname.Normalize();
    Manager::Get()->GetMessageManager()->DebugLog(_("Workspace dir: %s"), wfname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR).c_str());
    while (!file.Eof())
    {
        wxString line = input.ReadLine();

        line.Trim(true);
        line.Trim(false);

        // example wanted line:
        //Project: "Demo_BSP"=.\Samples\BSP\scripts\Demo_BSP.dsp - Package Owner=<4>
        if (line.StartsWith(_T("Project:"))) {
          line.Remove(0, 8); // remove "Project:"
          // now we need to find the equal sign (=) that separates the
          // project title from the filename, and the minus sign (-)
          // that separates the filename from junk info - at least to this importer ;)
          int equal = line.Find(_T('='));
          int minus = line.Find(_T('-'), true); // search from end

          if (equal == -1 || minus == -1)
            continue;

          // read project title and trim quotes
          wxString prjTitle = line.Left(equal);
          prjTitle.Trim(true);
          prjTitle.Trim(false);
          if (prjTitle.IsEmpty())
            continue;
          if (prjTitle.GetChar(0) == _T('\"'))
          {
            prjTitle.Truncate(prjTitle.Length() - 1);
            prjTitle.Remove(0, 1);
          }

          // read project filename and trim quotes
          ++equal;
          wxString prjFile = line.Mid(equal, minus - equal);
          prjFile.Trim(true);
          prjFile.Trim(false);
          if (prjFile.IsEmpty())
            continue;
          if (prjFile.GetChar(0) == _T('\"'))
          {
            prjFile.Truncate(prjFile.Length() - 1);
            prjFile.Remove(0, 1);
          }

          ++count;
          wxFileName fname(prjFile);
          fname.Normalize(wxPATH_NORM_ALL, wfname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR), wxPATH_NATIVE);
          Manager::Get()->GetMessageManager()->DebugLog(_("Found project '%s' in '%s'"), prjTitle.c_str(), fname.GetFullPath().c_str());
          project = Manager::Get()->GetProjectManager()->LoadProject(fname.GetFullPath());
          if (project) registerProject(project->GetTitle(), project);
        }
        /*
        * exemple wanted line:
        * Project_Dep_Name VstSDK
        * and add the dependency/link of the VstSDK project to the current project
        * be carefull, the dependent projects could not have already been read, so we have to remember them
        */
        else if (line.StartsWith(_T("Project_Dep_Name"))) {
          line.Remove(0, 16);
          line.Trim(false);
          if (project) addDependency(project->GetTitle(), line);
        }
    }

    updateProjects();
    ImportersGlobals::ResetDefaults();

    m_Title = wxFileName(filename).GetName() + _(" workspace");
    return count != 0;
}

bool MSVCWorkspaceLoader::Save(const wxString& title, const wxString& filename)
{
    // no support for saving workspace files (.dsw)
    return false;
}
