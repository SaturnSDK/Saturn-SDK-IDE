#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/confbase.h>
    #include <wx/fileconf.h>
    #include <wx/msgdlg.h>
    #include <wx/intl.h>
    #include <wx/filename.h>
    #include <wx/txtstrm.h>
    #include <wx/dynarray.h>

    #include "manager.h"
    #include "projectmanager.h"
    #include "messagemanager.h"
    #include "cbproject.h"
    #include "globals.h"
    #include "compilerfactory.h"
    #include "compiler.h"
    #include <wx/wfstream.h>
    #include <wx/log.h>
#endif

#include <wx/stream.h>

#include "importers_globals.h"
#include "msvcloader.h"
#include "multiselectdlg.h"


MSVCLoader::MSVCLoader(cbProject* project)
    : m_pProject(project),
    m_ConvertSwitches(true)
{
	//ctor
}

MSVCLoader::~MSVCLoader()
{
	//dtor
}

bool MSVCLoader::Open(const wxString& filename)
{
 /* NOTE (mandrav#1#): not necessary to ask for switches conversion... */
    m_ConvertSwitches = m_pProject->GetCompilerIndex() == 0; // GCC

   m_Filename = filename;
    if (!ReadConfigurations())
        return false;

    // the file is read, now process it
    Manager::Get()->GetMessageManager()->DebugLog(_("Importing MSVC project: %s"), filename.c_str());

    // delete all targets of the project (we 'll create new ones from the imported configurations)
    while (m_pProject->GetBuildTargetsCount())
        m_pProject->RemoveBuildTarget(0);

    wxArrayInt selected_indices;
    if (ImportersGlobals::ImportAllTargets)
    {
        // don't ask; just fill selected_indices with all indices
        for (size_t i = 0; i < m_Configurations.GetCount(); ++i)
            selected_indices.Add(i);
    }
    else
    {
        // ask the user to select a configuration - multiple choice ;)
        MultiSelectDlg dlg(0, m_Configurations, true, _("Select configurations to import:"), m_Filename.GetName());
        if (dlg.ShowModal() == wxID_CANCEL)
        {
            Manager::Get()->GetMessageManager()->DebugLog(_T("Canceled..."));
            return false;
        }
        selected_indices = dlg.GetSelectedIndices();
    }

    // create all selected targets
    for (size_t i = 0; i < selected_indices.GetCount(); ++i)
    {
        if (!ParseConfiguration(selected_indices[i]))
            return false;
    }

    m_pProject->SetTitle(m_Filename.GetName());
    return ParseSourceFiles();
}

bool MSVCLoader::Save(const wxString& filename)
{
    // no support to save MSVC projects
    return false;
}

bool MSVCLoader::ReadConfigurations()
{
    m_Configurations.Clear();
    m_ConfigurationsLineIndex.Clear();
    m_BeginTargetLine = -1;

    wxFileInputStream file(m_Filename.GetFullPath());
    if (!file.Ok())
        return false; // error opening file???

    wxArrayString comps;
    wxTextInputStream input(file);

    int currentLine = 0;
    while (!file.Eof())
    {
        wxString line = input.ReadLine();
        ++currentLine;
        line.Trim(true);
        line.Trim(false);
        int size = -1;
        if (line.StartsWith(_T("# TARGTYPE")))
        {
          	// # TARGTYPE "Win32 (x86) Application" 0x0103
            int idx = line.Find(' ', true);
            if (idx != -1)
            {
            	TargetType type;
                wxString targtype = line.Mid(12, idx-1-12);
                wxString projcode = line.Mid(idx+3, 4);
                if      (projcode.Matches(_T("0101"))) type = ttExecutable;
                else if (projcode.Matches(_T("0102"))) type = ttDynamicLib;
                else if (projcode.Matches(_T("0103"))) type = ttConsoleOnly;
                else if (projcode.Matches(_T("0104"))) type = ttStaticLib;
                else if (projcode.Matches(_T("010a"))) type = ttCommandsOnly;
                else {
                  type = ttCommandsOnly;
                  Manager::Get()->GetMessageManager()->DebugLog(_("unrecognized target type"));
                }

                //Manager::Get()->GetMessageManager()->DebugLog(_("TargType '%s' is %d"), targtype.c_str(), type);
                m_TargType[targtype] = type;
            }
            continue;
        }
        else if (line.StartsWith(_T("!MESSAGE \""))) {
        //  !MESSAGE "anothertest - Win32 Release" (based on "Win32 (x86) Application")
            int pos;
            pos = line.Find('\"');
            line.Remove(0, pos+1);
            pos = line.Find('\"');
            wxArrayString projectTarget = GetArrayFromString(line.Left(pos), _T("-"));
            wxString target = projectTarget[1];
            if (projectTarget.GetCount() != 2) {
                Manager::Get()->GetMessageManager()->DebugLog(_("ERROR: bad target format"));
                return false;
            }
            line.Remove(0, pos+1);
            pos = line.Find('\"');
            line.Remove(0, pos+1);
            pos = line.Find('\"');
            wxString basedon = line.Left(pos);
            TargetType type = ttCommandsOnly;
            HashTargetType::iterator it = m_TargType.find(basedon);
            if (it != m_TargType.end()) type = it->second;
            else {
                Manager::Get()->GetMessageManager()->DebugLog(_("ERROR: target type not found"));
                return false;
            }
            m_TargetBasedOn[target] = type;
            //Manager::Get()->GetMessageManager()->DebugLog(_("Target '%s' type %d"), target.c_str(), type);
        }
        else if (line.StartsWith(_T("!IF  \"$(CFG)\" ==")))
            size = 16;
        else if (line.StartsWith(_T("!ELSEIF  \"$(CFG)\" ==")))
            size = 20;
        else if (line == _T("# Begin Target"))
        {
            // done
            m_BeginTargetLine = currentLine;
            break;
        }
        if (size != -1)
        {
            // read configuration name
            line.Remove(0, size);
            line.Trim(true);
            line.Trim(false);
            wxString tmp = RemoveQuotes(line);
            // remove the project name part, i.e "anothertest - "
            int idx = tmp.Find('-');
            if (idx != -1) {
              tmp.Remove(0, idx+1);
              tmp.Trim(false);
            }
            if (m_Configurations.Index(tmp) == wxNOT_FOUND)
            {
                m_Configurations.Add(tmp);
                m_ConfigurationsLineIndex.Add(currentLine);
                Manager::Get()->GetMessageManager()->DebugLog(_T("Detected configuration '%s' at line %d"), tmp.c_str(), currentLine);
            }
        }
    }
    return true;
}

bool MSVCLoader::ParseConfiguration(int index)
{
    wxFileInputStream file(m_Filename.GetFullPath());
    if (!file.Ok())
        return false; // error opening file???

    // create new target
    ProjectBuildTarget* bt = m_pProject->AddBuildTarget(m_Configurations[index]);
    if (!bt)
        return false;
    bt->SetCompilerIndex(m_pProject->GetCompilerIndex());
    m_Type = ttCommandsOnly;
    HashTargetType::iterator it = m_TargetBasedOn.find(m_Configurations[index]);
    if (it != m_TargetBasedOn.end()) m_Type = it->second;
    else Manager::Get()->GetMessageManager()->DebugLog(_T("ERROR: could not find the target type of %s"), m_Configurations[index].c_str());
    bt->SetTargetType(m_Type);
    bt->SetOutputFilename(bt->SuggestOutputFilename());

    wxTextInputStream input(file);

    // go to the configuration's line
    int currentLine = 0;
    while (!file.Eof() && currentLine <= m_ConfigurationsLineIndex[index])
    {
        input.ReadLine();
        ++currentLine;
    }

    // start parsing the configuration
    while (!file.Eof())
    {
        wxString line = input.ReadLine();
        line.Trim(true);
        line.Trim(false);

        // we want empty lines (skipped) or lines starting with #
        // if we encounter a line starting with !, we break out of here
        if (line.GetChar(0) == '!')
            break;
        if (line.IsEmpty() || line.GetChar(0) != '#')
            continue;

//        if (line.StartsWith("# PROP BASE Output_Dir "))
        if (line.StartsWith(_T("# PROP Output_Dir ")))
        {
            line.Remove(0, 18);
            line.Trim(true);
            line.Trim(false);
            wxString tmp = RemoveQuotes(line);
            if (!line.IsEmpty())
            {
                wxFileName out = bt->GetOutputFilename();
                out.SetPath(tmp); // out could be a full path name and not only a relative one !
                if (out.IsRelative())
                    out.MakeAbsolute(m_Filename.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
                bt->SetOutputFilename(out.GetFullPath());
            }
        }
//        else if (line.StartsWith("# PROP BASE Intermediate_Dir "))
        else if (line.StartsWith(_T("# PROP Intermediate_Dir ")))
        {
            line.Remove(0, 24);
            line.Trim(true);
            line.Trim(false);
            wxString tmp = RemoveQuotes(line);
            if (!line.IsEmpty())
            {
                bt->SetObjectOutput(tmp);
            }
        }
        else if (line.StartsWith(_T("# ADD BASE CPP ")))
        {
            line.Remove(0, 15);
            line.Trim(true);
            line.Trim(false);
            ProcessCompilerOptions(bt, line);
        }
        else if (line.StartsWith(_T("# ADD CPP ")))
        {
            line.Remove(0, 10);
            line.Trim(true);
            line.Trim(false);
            ProcessCompilerOptions(bt, line);
        }
        else if (line.StartsWith(_T("# ADD BASE LIB32 ")))
        {
            line.Remove(0, 17);
            line.Trim(true);
            line.Trim(false);
            ProcessLinkerOptions(bt, line);
        }
        else if (line.StartsWith(_T("# ADD LIB32 ")))
        {
            line.Remove(0, 12);
            line.Trim(true);
            line.Trim(false);
            ProcessLinkerOptions(bt, line);
        }
    }
    return true;
}

bool MSVCLoader::ParseSourceFiles()
{
    wxFileInputStream file(m_Filename.GetFullPath());
    if (!file.Ok())
        return false; // error opening file???

    wxTextInputStream input(file);

    // go to the begining of source files
    int currentLine = 0;
    while (!file.Eof() && currentLine < m_BeginTargetLine)
    {
        input.ReadLine();
        ++currentLine;
    }

    while (!file.Eof())
    {
        wxString line = input.ReadLine();
        line.Trim(true);
        line.Trim(false);

        // we 're only interested in lines starting with SOURCE=
        if (!line.StartsWith(_T("SOURCE=")))
            continue;

        line.Remove(0, 7);
        line.Trim(true);
        line.Trim(false);

        ProjectFile* pf = m_pProject->AddFile(0, RemoveQuotes(line));
        if (pf)
        {
            // add it to all configurations, not just the first
            for (int i = 1; i < m_pProject->GetBuildTargetsCount(); ++i)
                pf->AddBuildTarget(m_pProject->GetBuildTarget(i)->GetTitle());
        }
    }
    return true;
}

void MSVCLoader::ProcessCompilerOptions(ProjectBuildTarget* target, const wxString& opts)
{
    wxArrayString array;
    array = GetArrayFromString(opts, _T(" "));

    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
        wxString opt = array[i];
        opt.Trim();

        if (m_ConvertSwitches)
        {
            if (opt.Matches(_T("/D")))
                target->AddCompilerOption(_T("-D") + RemoveQuotes(array[++i]));
            else if (opt.Matches(_T("/U")))
                target->AddCompilerOption(_T("-U") + RemoveQuotes(array[++i]));
            else if (opt.Matches(_T("/Zi")) || opt.Matches(_T("/ZI")))
                target->AddCompilerOption(_T("-g"));
            else if (opt.Matches(_T("/I")))
                target->AddIncludeDir(RemoveQuotes(array[++i]));
            else if (opt.Matches(_T("/W0")))
                target->AddCompilerOption(_T("-w"));
            else if (opt.Matches(_T("/O1")) ||
                    opt.Matches(_T("/O2")) ||
                    opt.Matches(_T("/O3")))
                target->AddCompilerOption(_T("-O2"));
            else if (opt.Matches(_T("/W1")) ||
                    opt.Matches(_T("/W2")) ||
                    opt.Matches(_T("/W3")))
                target->AddCompilerOption(_T("-W"));
            else if (opt.Matches(_T("/W4")))
                target->AddCompilerOption(_T("-Wall"));
            else if (opt.Matches(_T("/WX")))
                target->AddCompilerOption(_T("-Werror"));
            else if (opt.Matches(_T("/GX")))
                target->AddCompilerOption(_T("-fexceptions"));
            else if (opt.Matches(_T("/Ob0")))
                target->AddCompilerOption(_T("-fno-inline"));
            else if (opt.Matches(_T("/Ob2")))
                target->AddCompilerOption(_T("-finline-functions"));
            else if (opt.Matches(_T("/Oy")))
                target->AddCompilerOption(_T("-fomit-frame-pointer"));
            else if (opt.Matches(_T("/GB")))
                target->AddCompilerOption(_T("-mcpu=pentiumpro -D_M_IX86=500"));
            else if (opt.Matches(_T("/G6")))
                target->AddCompilerOption(_T("-mcpu=pentiumpro -D_M_IX86=600"));
            else if (opt.Matches(_T("/G5")))
                target->AddCompilerOption(_T("-mcpu=pentium -D_M_IX86=500"));
            else if (opt.Matches(_T("/G4")))
                target->AddCompilerOption(_T("-mcpu=i486 -D_M_IX86=400"));
            else if (opt.Matches(_T("/G3")))
                target->AddCompilerOption(_T("-mcpu=i386 -D_M_IX86=300"));
            else if (opt.Matches(_T("/Za")))
                target->AddCompilerOption(_T("-ansi"));
            else if (opt.Matches(_T("/Zp1")))
                target->AddCompilerOption(_T("-fpack-struct"));
            else if (opt.Matches(_T("/nologo")))
            {
                // do nothing (ignore silently)
            }
            else if (opt.Matches(_T("/c"))) {} // do nothing
            //else Manager::Get()->GetMessageManager()->DebugLog("Unhandled compiler option: " + opt);
        }
        else // !m_ConvertSwitches
        {
            // only differentiate includes and definitions
            if (opt.Matches(_T("/I")))
                target->AddIncludeDir(RemoveQuotes(array[++i]));
            else if (opt.Matches(_T("/D")))
                target->AddCompilerOption(_T("/D") + RemoveQuotes(array[++i]));
            else if (opt.Matches(_T("/U")))
                target->AddCompilerOption(_T("/U") + RemoveQuotes(array[++i]));
            else if (opt.StartsWith(_T("/Yu")))
                Manager::Get()->GetMessageManager()->DebugLog(_("Ignoring precompiled headers option (/Yu)"));
            else if (opt.Matches(_T("/c")) || opt.Matches(_T("/nologo"))) {} // do nothing
            else
                target->AddCompilerOption(opt);
        }
    }
}

void MSVCLoader::ProcessLinkerOptions(ProjectBuildTarget* target, const wxString& opts)
{
    wxArrayString array;
    array = GetArrayFromString(opts, _T(" "));

    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
        wxString opt = array[i];
        opt.Trim();

        if (m_ConvertSwitches)
        {
            if (opt.StartsWith(_T("/libpath:")))
            {
                opt.Remove(0, 9);
                target->AddLibDir(RemoveQuotes(opt));
            }
            else if (opt.StartsWith(_T("/base:")))
            {
                opt.Remove(0, 6);
                target->AddLinkerOption(_T("--image-base ") + RemoveQuotes(opt));
            }
            else if (opt.StartsWith(_T("/implib:")))
            {
                opt.Remove(0, 8);
                target->AddLinkerOption(_T("--implib ") + RemoveQuotes(opt));
            }
            else if (opt.StartsWith(_T("/map:")))
            {
                opt.Remove(0, 5);
                target->AddLinkerOption(_T("-Map ") + RemoveQuotes(opt) + _T(".map"));
            }
            else if (opt.Matches(_T("/nologo")))
            {
                // do nothing (ignore silently)
            }
            else if (opt.StartsWith(_T("/out:")))
            {
                // do nothing; it is handled below, in common options
            }
            else
                Manager::Get()->GetMessageManager()->DebugLog(_("Unknown linker option: " + opt));
        }
        else // !m_ConvertSwitches
        {
            if (opt.StartsWith(_T("/libpath:")))
            {
                opt.Remove(0, 9);
                target->AddLibDir(RemoveQuotes(opt));
            }
            else if (opt.Matches(_T("/nologo"))) {} // ignore silently
            else
            {
                // don't add linking lib (added below, in common options)
                int idx = opt.Find(_T(".lib"));
                if (idx == -1)
                    target->AddLinkerOption(opt);
            }
        }

        // common options
        if (!opt.StartsWith(_T("/")))
        {
            // probably linking lib
            int idx = opt.Find(_T(".lib"));
            if (idx != -1)
            {
                opt.Remove(idx);
                target->AddLinkLib(opt);
            }
        }
        else if (opt.StartsWith(_T("/out:")))
        {
            opt.Remove(0, 5);
            opt = RemoveQuotes(opt);
            if (m_Type == ttStaticLib)
            {
              // convert lib filename based on compiler
/* NOTE (mandrav#1#): I think I should move this code somewhere more accessible...
I need it here and there... */
                wxFileName orig = target->GetOutputFilename();
                wxFileName newf = opt;
                if (newf.IsRelative())
                    newf.MakeAbsolute(m_pProject->GetBasePath());
                Compiler* compiler = CompilerFactory::Compilers[m_pProject->GetCompilerIndex()];
                newf.SetExt(compiler->GetSwitches().libExtension);
                wxString name = newf.GetName();
                wxString prefix = compiler->GetSwitches().libPrefix;
                if (!prefix.IsEmpty() && !name.StartsWith(prefix))
                    newf.SetName(prefix + name);
                target->SetOutputFilename(newf.GetFullPath());
            }
            else
                target->SetOutputFilename(opt);
        }
    }
}

wxString MSVCLoader::RemoveQuotes(const wxString& src)
{
    wxString res = src;
    if (res.StartsWith(_T("\"")))
    {
        res.Remove(0, 1);
        res.Remove(res.Length() - 1);
    }
//    Manager::Get()->GetMessageManager()->DebugLog("Removing quotes: %s --> %s", src.c_str(), res.c_str());
    return res;
}
