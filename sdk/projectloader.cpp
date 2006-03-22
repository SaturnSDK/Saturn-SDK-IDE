/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/confbase.h>
    #include <wx/fileconf.h>
    #include <wx/intl.h>
    #include <wx/filename.h>
    #include <wx/msgdlg.h>
    #include <wx/log.h>
    #include "manager.h"
    #include "projectmanager.h"
    #include "messagemanager.h"
    #include "macrosmanager.h"
    #include "cbproject.h"
    #include "compilerfactory.h"
    #include "globals.h"
#endif

#include "projectloader.h"
#include "projectloader_hooks.h"

ProjectLoader::ProjectLoader(cbProject* project)
    : m_pProject(project),
    m_Upgraded(false),
    m_OpenDirty(false)
{
	//ctor
}

ProjectLoader::~ProjectLoader()
{
	//dtor
}

bool ProjectLoader::Open(const wxString& filename)
{
    MessageManager* pMsg = Manager::Get()->GetMessageManager();
    if (!pMsg)
        return false;

    wxStopWatch sw;
    pMsg->DebugLog(_T("Loading project file..."));
    TiXmlDocument doc(filename.mb_str());
    if (!doc.LoadFile())
        return false;

    pMsg->DebugLog(_T("Parsing project file..."));
    TiXmlElement* root;
    TiXmlElement* proj;

    root = doc.FirstChildElement("CodeBlocks_project_file");
    if (!root)
    {
        // old tag
        root = doc.FirstChildElement("Code::Blocks_project_file");
        if (!root)
        {
            pMsg->DebugLog(_T("Not a valid Code::Blocks project file..."));
            return false;
        }
    }
    proj = root->FirstChildElement("Project");
    if (!proj)
    {
        pMsg->DebugLog(_T("No 'Project' element in file..."));
        return false;
    }

    m_IsPre_1_2 = false; // flag for some changed defaults in version 1.2
    TiXmlElement* version = root->FirstChildElement("FileVersion");
    if (version)
    {
        int major = PROJECT_FILE_VERSION_MAJOR;
        int minor = PROJECT_FILE_VERSION_MINOR;
        version->QueryIntAttribute("major", &major);
        version->QueryIntAttribute("minor", &minor);
        if (major < 1 ||
            (major == 1 && minor < 2))
        {
            // pre-1.2
            pMsg->DebugLog(_T("Project version is %d.%d. Defaults have changed since then..."), major, minor);
            m_IsPre_1_2 = true;
        }
        else if (major >= PROJECT_FILE_VERSION_MAJOR && minor > PROJECT_FILE_VERSION_MINOR)
        {
            pMsg->DebugLog(_T("Project version is > %d.%d. Trying to load..."), PROJECT_FILE_VERSION_MAJOR, PROJECT_FILE_VERSION_MINOR);
            cbMessageBox(_("This project file was saved with a newer version of Code::Blocks.\n"
                            "Will try to load, but you should make sure all the settings were loaded correctly..."),
                            _("Warning"),
                            wxICON_WARNING);
        }
    }

    DoProjectOptions(proj);
    DoBuild(proj);
    DoCompilerOptions(proj);
    DoResourceCompilerOptions(proj);
    DoLinkerOptions(proj);
    DoIncludesOptions(proj);
    DoLibsOptions(proj);
    DoExtraCommands(proj);
    DoUnits(proj);

    // as a last step, run all hooked callbacks
    TiXmlElement* node = proj->FirstChildElement("Extensions");
    if (node)
    {
        ProjectLoaderHooks::CallHooks(m_pProject, node, true);
    }

    if (!version)
    {
        // pre 1.1 version
        ConvertVersion_Pre_1_1();
        // format changed also:
        // removed <IncludeDirs> and <LibDirs> elements and added them as child elements
        // in <Compiler> and <Linker> elements respectively
        // so set m_Upgraded to true, irrespectively of libs detection...
        m_Upgraded = true;
    }
    else
    {
        // do something important based on version
//        wxString major = version->Attribute("major");
//        wxString minor = version->Attribute("minor");
    }

    pMsg->DebugLog(wxString(_T("Done loading project in ")) << wxString::Format(_T("%d"), (int) sw.Time()) << _T("ms"));
    return true;
}

void ProjectLoader::ConvertVersion_Pre_1_1()
{
    // ask to detect linker libraries and move them to the new
    // CompileOptionsBase linker libs container
    wxString msg;
    msg.Printf(_("Project \"%s\" was saved with an earlier version of Code::Blocks.\n"
                "In the current version, link libraries are treated separately from linker options.\n"
                "Do you want to auto-detect the libraries \"%s\" is using and configure it accordingly?"),
                m_pProject->GetTitle().c_str(),
                m_pProject->GetTitle().c_str());
    if (cbMessageBox(msg, _("Question"), wxICON_QUESTION | wxYES_NO) == wxID_YES)
    {
        // project first
        ConvertLibraries(m_pProject);

        for (int i = 0; i < m_pProject->GetBuildTargetsCount(); ++i)
        {
            ConvertLibraries(m_pProject->GetBuildTarget(i));
            m_Upgraded = true;
        }
    }
}

void ProjectLoader::ConvertLibraries(CompileTargetBase* object)
{
    wxArrayString linkerOpts = object->GetLinkerOptions();
    wxArrayString linkLibs = object->GetLinkLibs();

    wxString compilerId = object->GetCompilerID();
    Compiler* compiler = CompilerFactory::GetCompiler(compilerId);
    wxString linkLib = compiler->GetSwitches().linkLibs;
    wxString libExt = compiler->GetSwitches().libExtension;
    size_t libExtLen = libExt.Length();

    size_t i = 0;
    while (i < linkerOpts.GetCount())
    {
        wxString opt = linkerOpts[i];
        if (!linkLib.IsEmpty() && opt.StartsWith(linkLib))
        {
            opt.Remove(0, 2);
            wxString ext = compiler->GetSwitches().libExtension;
            if (!ext.IsEmpty())
                ext = _T(".") + ext;
            linkLibs.Add(compiler->GetSwitches().libPrefix + opt + ext);
            linkerOpts.RemoveAt(i, 1);
        }
        else if (opt.Length() > libExtLen && opt.Right(libExtLen) == libExt)
        {
            linkLibs.Add(opt);
            linkerOpts.RemoveAt(i, 1);
        }
        else
            ++i;
    }

    object->SetLinkerOptions(linkerOpts);
    object->SetLinkLibs(linkLibs);
}

void ProjectLoader::DoMakeCommands(TiXmlElement* parentNode, CompileTargetBase* target)
{
    if (!parentNode)
        return; // no options

    TiXmlElement* node;

    node = parentNode->FirstChildElement("Build");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcBuild, cbC2U(node->Attribute("command")));

    node = parentNode->FirstChildElement("CompileFile");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcCompileFile, cbC2U(node->Attribute("command")));

    node = parentNode->FirstChildElement("Clean");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcClean, cbC2U(node->Attribute("command")));

    node = parentNode->FirstChildElement("DistClean");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcDistClean, cbC2U(node->Attribute("command")));
}

void ProjectLoader::DoProjectOptions(TiXmlElement* parentNode)
{
    TiXmlElement* node = parentNode->FirstChildElement("Option");
    if (!node)
        return; // no options

    wxString title;
    wxString makefile;
    bool makefile_custom = false;
    int defaultTarget = 0;
    int activeTarget = -1;
    wxString compilerId = _T("gcc");
    PCHMode pch_mode = m_IsPre_1_2 ? pchSourceDir : pchObjectDir;

    // loop through all options
    while (node)
    {
        if (node->Attribute("title"))
            title = cbC2U(node->Attribute("title"));

        else if (node->Attribute("makefile")) // there is only one attribute per option, so "else" is a safe optimisation
            makefile = cbC2U(node->Attribute("makefile"));

        else if (node->Attribute("makefile_is_custom"))
            makefile_custom = strncmp(node->Attribute("makefile_is_custom"), "1", 1) == 0;

        else if (node->Attribute("default_target"))
            defaultTarget = atoi(node->Attribute("default_target"));

        else if (node->Attribute("compiler"))
            compilerId = GetValidCompilerID(cbC2U(node->Attribute("compiler")), _T("the project"));

        else if (node->Attribute("pch_mode"))
            pch_mode = (PCHMode)atoi(node->Attribute("pch_mode"));

        node = node->NextSiblingElement("Option");
    }

    m_pProject->SetTitle(title);
    m_pProject->SetMakefile(makefile);
    m_pProject->SetMakefileCustom(makefile_custom);
    m_pProject->SetDefaultExecuteTargetIndex(defaultTarget);
    m_pProject->SetActiveBuildTarget(activeTarget);
    m_pProject->SetCompilerID(compilerId);
    m_pProject->SetModeForPCH(pch_mode);

    DoMakeCommands(parentNode->FirstChildElement("MakeCommands"), m_pProject);
}

void ProjectLoader::DoBuild(TiXmlElement* parentNode)
{
    TiXmlElement* node = parentNode->FirstChildElement("Build");
    while (node)
    {
        TiXmlElement* opt = node->FirstChildElement("Script");
        while (opt)
        {
            if (opt->Attribute("file"))
                m_pProject->AddBuildScript(cbC2U(opt->Attribute("file")));

            opt = opt->NextSiblingElement("Script");
        }

        DoBuildTarget(node);
        DoEnvironment(node, m_pProject);
        node = node->NextSiblingElement("Build");
    }
}

void ProjectLoader::DoBuildTarget(TiXmlElement* parentNode)
{
    TiXmlElement* node = parentNode->FirstChildElement("Target");
    if (!node)
        return; // no options

    while (node)
    {
        ProjectBuildTarget* target = 0L;
        wxString title = cbC2U(node->Attribute("title"));
        if (!title.IsEmpty())
            target = m_pProject->AddBuildTarget(title);

        if (target)
        {
            Manager::Get()->GetMessageManager()->DebugLog(_T("Loading target %s"), title.c_str());
            DoBuildTargetOptions(node, target);
            DoCompilerOptions(node, target);
            DoResourceCompilerOptions(node, target);
            DoLinkerOptions(node, target);
            DoIncludesOptions(node, target);
            DoLibsOptions(node, target);
            DoExtraCommands(node, target);
            DoEnvironment(node, target);
        }

        node = node->NextSiblingElement("Target");
    }
}

void ProjectLoader::DoBuildTargetOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("Option");
    if (!node)
        return; // no options

    bool use_console_runner = true;
    wxString output;
    wxString working_dir;
    wxString obj_output;
    wxString deps_output;
    wxString deps;
    wxString added;
    int type = -1;
    wxString compilerId = m_pProject->GetCompilerID();
    wxString parameters;
    wxString hostApplication;
    bool includeInTargetAll = m_IsPre_1_2 ? true : false;
    bool createStaticLib = false;
    bool createDefFile = false;
    int projectCompilerOptionsRelation = 3;
    int projectLinkerOptionsRelation = 3;
    int projectIncludeDirsRelation = 3;
    int projectLibDirsRelation = 3;
    int projectResIncludeDirsRelation = 3;

    while (node)
    {
        if (node->Attribute("use_console_runner"))
            use_console_runner = strncmp(node->Attribute("use_console_runner"), "0", 1) != 0;

        else if (node->Attribute("output"))
            output = UnixFilename(cbC2U(node->Attribute("output")));

        else if (node->Attribute("working_dir"))
            working_dir = UnixFilename(cbC2U(node->Attribute("working_dir")));

        else if (node->Attribute("object_output"))
            obj_output = UnixFilename(cbC2U(node->Attribute("object_output")));

        else if (node->Attribute("deps_output"))
            deps_output = UnixFilename(cbC2U(node->Attribute("deps_output")));

        else if (node->Attribute("external_deps"))
            deps = UnixFilename(cbC2U(node->Attribute("external_deps")));

        else if (node->Attribute("additional_output"))
            added = UnixFilename(cbC2U(node->Attribute("additional_output")));

        else if (node->Attribute("type"))
            type = atoi(node->Attribute("type"));

        else if (node->Attribute("compiler"))
            compilerId = GetValidCompilerID(cbC2U(node->Attribute("compiler")), target->GetTitle());

        else if (node->Attribute("parameters"))
            parameters = cbC2U(node->Attribute("parameters"));

        else if (node->Attribute("host_application"))
            hostApplication = UnixFilename(cbC2U(node->Attribute("host_application")));

        else if (node->Attribute("includeInTargetAll"))
            includeInTargetAll = atoi(node->Attribute("includeInTargetAll")) != 0;

        else if (node->Attribute("createDefFile"))
            createDefFile = atoi(node->Attribute("createDefFile")) != 0;

        else if (node->Attribute("createStaticLib"))
            createStaticLib = atoi(node->Attribute("createStaticLib")) != 0;

        else if (node->Attribute("projectCompilerOptionsRelation"))
            projectCompilerOptionsRelation = atoi(node->Attribute("projectCompilerOptionsRelation"));

        else if (node->Attribute("projectLinkerOptionsRelation"))
            projectLinkerOptionsRelation = atoi(node->Attribute("projectLinkerOptionsRelation"));

        else if (node->Attribute("projectIncludeDirsRelation"))
            projectIncludeDirsRelation = atoi(node->Attribute("projectIncludeDirsRelation"));

        else if (node->Attribute("projectLibDirsRelation"))
            projectLibDirsRelation = atoi(node->Attribute("projectLibDirsRelation"));

        else if (node->Attribute("projectResourceIncludeDirsRelation"))
        {
            projectResIncludeDirsRelation = atoi(node->Attribute("projectResourceIncludeDirsRelation"));
            // there used to be a bug in this setting and it might have a negative or very big number
            // detect this case and set as default
            if (projectResIncludeDirsRelation < 0 || projectResIncludeDirsRelation >= ortLast)
                projectResIncludeDirsRelation = 3;
        }

        node = node->NextSiblingElement("Option");
    }

    node = parentNode->FirstChildElement("Script");
    while (node)
    {
        if (node->Attribute("file"))
            target->AddBuildScript(UnixFilename(cbC2U(node->Attribute("file"))));

        node = node->NextSiblingElement("Script");
    }

    if (type != -1)
    {
        target->SetTargetType((TargetType)type); // type *must* come before output filename!
        target->SetOutputFilename(output); // because if no filename defined, one will be suggested based on target type...
        target->SetUseConsoleRunner(use_console_runner);
        if (!working_dir.IsEmpty())
            target->SetWorkingDir(working_dir);
        if (!obj_output.IsEmpty())
            target->SetObjectOutput(obj_output);
        if (!deps_output.IsEmpty())
            target->SetDepsOutput(deps_output);
        target->SetExternalDeps(deps);
        target->SetAdditionalOutputFiles(added);
        target->SetCompilerID(compilerId);
        target->SetExecutionParameters(parameters);
        target->SetHostApplication(hostApplication);
        target->SetIncludeInTargetAll(includeInTargetAll);
        target->SetCreateDefFile(createDefFile);
        target->SetCreateStaticLib(createStaticLib);
        target->SetOptionRelation(ortCompilerOptions, (OptionsRelation)projectCompilerOptionsRelation);
        target->SetOptionRelation(ortLinkerOptions, (OptionsRelation)projectLinkerOptionsRelation);
        target->SetOptionRelation(ortIncludeDirs, (OptionsRelation)projectIncludeDirsRelation);
        target->SetOptionRelation(ortLibDirs, (OptionsRelation)projectLibDirsRelation);
        target->SetOptionRelation(ortResDirs, (OptionsRelation)projectResIncludeDirsRelation);

        DoMakeCommands(parentNode->FirstChildElement("MakeCommands"), target);
    }
}

void ProjectLoader::DoCompilerOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("Compiler");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = cbC2U(child->Attribute("option"));
        wxString dir = cbC2U(child->Attribute("directory"));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddCompilerOption(option);
            else
                m_pProject->AddCompilerOption(option);
        }
        if (!dir.IsEmpty())
        {
            if (target)
                target->AddIncludeDir(dir);
            else
                m_pProject->AddIncludeDir(dir);
        }

        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoResourceCompilerOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("ResourceCompiler");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString dir = cbC2U(child->Attribute("directory"));
        if (!dir.IsEmpty())
        {
            if (target)
                target->AddResourceIncludeDir(dir);
            else
                m_pProject->AddResourceIncludeDir(dir);
        }

        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoLinkerOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("Linker");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = cbC2U(child->Attribute("option"));
        wxString dir = UnixFilename(cbC2U(child->Attribute("directory")));
        wxString lib = UnixFilename(cbC2U(child->Attribute("library")));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddLinkerOption(option);
            else
                m_pProject->AddLinkerOption(option);
        }
        if (!lib.IsEmpty())
        {
            if (target)
                target->AddLinkLib(lib);
            else
                m_pProject->AddLinkLib(lib);
        }
        if (!dir.IsEmpty())
        {
            if (target)
                target->AddLibDir(dir);
            else
                m_pProject->AddLibDir(dir);
        }

        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoIncludesOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("IncludeDirs");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = UnixFilename(cbC2U(child->Attribute("option")));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddIncludeDir(option);
            else
                m_pProject->AddIncludeDir(option);
        }

        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoLibsOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("LibDirs");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = cbC2U(child->Attribute("option"));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddLibDir(option);
            else
                m_pProject->AddLibDir(option);
        }

        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoExtraCommands(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("ExtraCommands");
    while (node)
    {
        CompileOptionsBase* base = target ? target : (CompileOptionsBase*)m_pProject;
        TiXmlElement* child = node->FirstChildElement("Mode");
        while (child)
        {
            wxString mode = cbC2U(child->Attribute("after"));
            if (mode == _T("always"))
                base->SetAlwaysRunPostBuildSteps(true);

            child = child->NextSiblingElement("Mode");
        }

        child = node->FirstChildElement("Add");
        while (child)
        {
            wxString before;
            wxString after;

            if (child->Attribute("before"))
                before = cbC2U(child->Attribute("before"));
            if (child->Attribute("after"))
                after = cbC2U(child->Attribute("after"));

            if (!before.IsEmpty())
                base->AddCommandsBeforeBuild(before);
            if (!after.IsEmpty())
                base->AddCommandsAfterBuild(after);

            child = child->NextSiblingElement("Add");
        }
        node = node->NextSiblingElement("ExtraCommands");
    }
}

void ProjectLoader::DoEnvironment(TiXmlElement* parentNode, CompileOptionsBase* base)
{
	if (!base)
        return;
    TiXmlElement* node = parentNode->FirstChildElement("Environment");
    while (node)
    {
        TiXmlElement* child = node->FirstChildElement("Variable");
        while (child)
        {
            wxString name = cbC2U(child->Attribute("name"));
            wxString value = cbC2U(child->Attribute("value"));
            if (!name.IsEmpty())
            	base->SetVar(name, UnixFilename(value));

            child = child->NextSiblingElement("Variable");
        }
        node = node->NextSiblingElement("Environment");
    }
}

void ProjectLoader::DoUnits(TiXmlElement* parentNode)
{
    Manager::Get()->GetMessageManager()->DebugLog(_T("Loading project files..."));
    int count = 0;
    TiXmlElement* unit = parentNode->FirstChildElement("Unit");
    while (unit)
    {
        wxString filename = cbC2U(unit->Attribute("filename"));
        if (!filename.IsEmpty())
        {
            ProjectFile* file = m_pProject->AddFile(-1, UnixFilename(filename));
            if (!file)
                Manager::Get()->GetMessageManager()->DebugLog(_T("Can't load file '%s'"), filename.c_str());
            else
            {
                ++count;
                DoUnitOptions(unit, file);
            }
        }

        unit = unit->NextSiblingElement("Unit");
    }
    Manager::Get()->GetMessageManager()->DebugLog(_T("%d files loaded"), count);
}

void ProjectLoader::DoUnitOptions(TiXmlElement* parentNode, ProjectFile* file)
{
    int tempval = 0;
    bool foundCompile = false;
    bool foundLink = false;
    bool foundCompilerVar = false;

    Compiler* compiler = CompilerFactory::GetCompiler(m_pProject->GetCompilerID());

    TiXmlElement* node = parentNode->FirstChildElement("Option");
    while (node)
    {
        if (node->Attribute("compilerVar"))
        {
            file->compilerVar = cbC2U(node->Attribute("compilerVar"));
            foundCompilerVar = true;
        }
        //
        if (node->QueryIntAttribute("compile", &tempval) == TIXML_SUCCESS)
        {
            file->compile = tempval != 0;
            foundCompile = true;
        }
        //
        if (node->QueryIntAttribute("link", &tempval) == TIXML_SUCCESS)
        {
            file->link = tempval != 0;
            foundLink = true;
        }
        //
        if (node->QueryIntAttribute("weight", &tempval) == TIXML_SUCCESS)
            file->weight = tempval;
        //
        if (node->Attribute("buildCommand") && node->Attribute("compiler"))
        {
            wxString cmp = cbC2U(node->Attribute("compiler"));
            wxString tmp = cbC2U(node->Attribute("buildCommand"));
            if (!cmp.IsEmpty() && !tmp.IsEmpty())
            {
                tmp.Replace(_T("\\n"), _T("\n"));
                file->customBuild[cmp].buildCommand = tmp;
                if (node->QueryIntAttribute("use", &tempval) == TIXML_SUCCESS)
                    file->customBuild[cmp].useCustomBuildCommand = tempval != 0;
            }
        }
        //
        if (node->Attribute("objectName"))
        {
            wxFileName objName(cbC2U(node->Attribute("objectName")));
            FileType ft = FileTypeOf(file->relativeFilename);
            if (ft != ftResource && ft != ftResourceBin)
            {
                if (objName.GetExt() != compiler->GetSwitches().objectExtension)
                    file->SetObjName(UnixFilename(file->relativeFilename));
            }
        }
        //
        if (node->Attribute("target"))
            file->AddBuildTarget(cbC2U(node->Attribute("target")));

        node = node->NextSiblingElement("Option");
    }

    // make sure the "compile" and "link" flags are honored
    if (!foundCompile)
        file->compile = true;
    if (!foundLink)
        file->link = true;
    if (!foundCompilerVar)
        file->compilerVar = _T("CPP");
}

// convenience function, used in Save()
TiXmlElement* ProjectLoader::AddElement(TiXmlElement* parent, const char* name, const char* attr, const wxString& attribute)
{
    TiXmlNode* node = parent->InsertEndChild(TiXmlElement(name));
    TiXmlElement* elem = static_cast<TiXmlElement*>(node);
    if (attr && strlen(attr))
        elem->SetAttribute(attr, cbU2C(attribute));
    return elem;
}

// convenience function, used in Save()
TiXmlElement* ProjectLoader::AddElement(TiXmlElement* parent, const char* name, const char* attr, int attribute)
{
    TiXmlNode* node = parent->InsertEndChild(TiXmlElement(name));
    TiXmlElement* elem = static_cast<TiXmlElement*>(node);
    if (attr&& strlen(attr))
        elem->SetAttribute(attr, attribute);
    return elem;
}

// convenience function, used in Save()
void ProjectLoader::AddArrayOfElements(TiXmlElement* parent, const char* name, const char* attr, const wxArrayString& array)
{
    if (!array.GetCount())
        return;
    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
        if (array[i].IsEmpty())
            continue;
        AddElement(parent, name, attr, array[i]);
    }
}

// convenience function, used in Save()
void ProjectLoader::SaveEnvironment(TiXmlElement* parent, CompileOptionsBase* base)
{
    if (!base)
        return;
    const StringHash& v = base->GetAllVars();
    if (v.empty())
        return;
    TiXmlElement* node = AddElement(parent, "Environment", 0, 0);
    for (StringHash::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        TiXmlElement* elem = AddElement(node, "Variable", "name", it->first);
        elem->SetAttribute("value", cbU2C(it->second));
    }
}

bool ProjectLoader::Save(const wxString& filename)
{
    if (ExportTargetAsProject(filename, wxEmptyString))
    {
        m_pProject->SetModified(false);
        return true;
    }
    return false;
}

bool ProjectLoader::ExportTargetAsProject(const wxString& filename, const wxString& onlyTarget)
{
    const char* ROOT_TAG = "CodeBlocks_project_file";

    TiXmlDocument doc;
    doc.SetCondenseWhiteSpace(false);
    doc.InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", "yes"));
    TiXmlElement* rootnode = static_cast<TiXmlElement*>(doc.InsertEndChild(TiXmlElement(ROOT_TAG)));
    if (!rootnode)
        return false;

    Compiler* compiler = CompilerFactory::GetCompiler(m_pProject->GetCompilerID());

    rootnode->InsertEndChild(TiXmlElement("FileVersion"));
    rootnode->FirstChildElement("FileVersion")->SetAttribute("major", PROJECT_FILE_VERSION_MAJOR);
    rootnode->FirstChildElement("FileVersion")->SetAttribute("minor", PROJECT_FILE_VERSION_MINOR);

    rootnode->InsertEndChild(TiXmlElement("Project"));
    TiXmlElement* prjnode = rootnode->FirstChildElement("Project");

    AddElement(prjnode, "Option", "title", m_pProject->GetTitle());
    if (m_pProject->GetMakefile() != _T("Makefile"))
        AddElement(prjnode, "Option", "makefile", m_pProject->GetMakefile());
    if (m_pProject->IsMakefileCustom())
        AddElement(prjnode, "Option", "makefile_is_custom", 1);
    if (m_pProject->GetModeForPCH() != pchObjectDir)
        AddElement(prjnode, "Option", "pch_mode", (int)m_pProject->GetModeForPCH());
    if (m_pProject->GetDefaultExecuteTargetIndex() != 0)
        AddElement(prjnode, "Option", "default_target", m_pProject->GetDefaultExecuteTargetIndex());
    AddElement(prjnode, "Option", "compiler", m_pProject->GetCompilerID());

    if (m_pProject->MakeCommandsModified())
    {
        TiXmlElement* makenode = AddElement(prjnode, "MakeCommands", 0, 0);
        AddElement(makenode, "Build", "command", m_pProject->GetMakeCommandFor(mcBuild));
        AddElement(makenode, "CompileFile", "command", m_pProject->GetMakeCommandFor(mcCompileFile));
        AddElement(makenode, "Clean", "command", m_pProject->GetMakeCommandFor(mcClean));
        AddElement(makenode, "DistClean", "command", m_pProject->GetMakeCommandFor(mcDistClean));
    }

    prjnode->InsertEndChild(TiXmlElement("Build"));
    TiXmlElement* buildnode = prjnode->FirstChildElement("Build");

    for (size_t x = 0; x < m_pProject->GetBuildScripts().GetCount(); ++x)
    {
        AddElement(buildnode, "Script", "file", m_pProject->GetBuildScripts().Item(x));
    }

    // now decide which target we 're exporting.
    // remember that if onlyTarget is empty, we export all targets (i.e. normal save).
    ProjectBuildTarget* onlytgt = m_pProject->GetBuildTarget(onlyTarget);

    for (int i = 0; i < m_pProject->GetBuildTargetsCount(); ++i)
    {
        ProjectBuildTarget* target = m_pProject->GetBuildTarget(i);
        if (!target)
            break;

        // skip every target except the desired one
        if (onlytgt && onlytgt != target)
            continue;

        TiXmlElement* tgtnode = AddElement(buildnode, "Target", "title", target->GetTitle());
        if (target->GetTargetType() != ttCommandsOnly)
        {
            AddElement(tgtnode, "Option", "output", target->GetOutputFilename());
            if (target->GetWorkingDir() != _T("."))
                AddElement(tgtnode, "Option", "working_dir", target->GetWorkingDir());
            if (target->GetObjectOutput() != _T(".objs"))
                AddElement(tgtnode, "Option", "object_output", target->GetObjectOutput());
            if (target->GetDepsOutput() != _T(".deps"))
                AddElement(tgtnode, "Option", "deps_output", target->GetDepsOutput());
        }
        if (!target->GetExternalDeps().IsEmpty())
            AddElement(tgtnode, "Option", "external_deps", target->GetExternalDeps());
        if (!target->GetAdditionalOutputFiles().IsEmpty())
            AddElement(tgtnode, "Option", "additional_output", target->GetAdditionalOutputFiles());
        AddElement(tgtnode, "Option", "type", target->GetTargetType());
        AddElement(tgtnode, "Option", "compiler", target->GetCompilerID());
        if (target->GetTargetType() == ttConsoleOnly && !target->GetUseConsoleRunner())
            AddElement(tgtnode, "Option", "use_console_runner", 0);
        if (!target->GetExecutionParameters().IsEmpty())
            AddElement(tgtnode, "Option", "parameters", target->GetExecutionParameters());
        if (!target->GetHostApplication().IsEmpty())
            AddElement(tgtnode, "Option", "host_application", target->GetHostApplication());
        if (target->GetIncludeInTargetAll())
            AddElement(tgtnode, "Option", "includeInTargetAll", 1);
        if ((target->GetTargetType() == ttStaticLib || target->GetTargetType() == ttDynamicLib) && target->GetCreateDefFile())
            AddElement(tgtnode, "Option", "createDefFile", 1);
        if (target->GetTargetType() == ttDynamicLib && target->GetCreateStaticLib())
            AddElement(tgtnode, "Option", "createStaticLib", 1);
        if (target->GetOptionRelation(ortCompilerOptions) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectCompilerOptionsRelation", target->GetOptionRelation(ortCompilerOptions));
        if (target->GetOptionRelation(ortLinkerOptions) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectLinkerOptionsRelation", target->GetOptionRelation(ortLinkerOptions));
        if (target->GetOptionRelation(ortIncludeDirs) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectIncludeDirsRelation", target->GetOptionRelation(ortIncludeDirs));
        if (target->GetOptionRelation(ortResDirs) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectResourceIncludeDirsRelation", target->GetOptionRelation(ortResDirs));
        if (target->GetOptionRelation(ortLibDirs) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectLibDirsRelation", target->GetOptionRelation(ortLibDirs));

        for (size_t x = 0; x < target->GetBuildScripts().GetCount(); ++x)
        {
            AddElement(tgtnode, "Script", "file", target->GetBuildScripts().Item(x));
        }

        TiXmlElement* node = AddElement(tgtnode, "Compiler", 0, 0);
        AddArrayOfElements(node, "Add", "option", target->GetCompilerOptions());
        AddArrayOfElements(node, "Add", "directory", target->GetIncludeDirs());
        if (node->NoChildren())
            tgtnode->RemoveChild(node);

        node = AddElement(tgtnode, "ResourceCompiler", 0, 0);
        AddArrayOfElements(node, "Add", "directory", target->GetResourceIncludeDirs());
        if (node->NoChildren())
            tgtnode->RemoveChild(node);

        node = AddElement(tgtnode, "Linker", 0, 0);
        AddArrayOfElements(node, "Add", "option", target->GetLinkerOptions());
        AddArrayOfElements(node, "Add", "library", target->GetLinkLibs());
        AddArrayOfElements(node, "Add", "directory", target->GetLibDirs());
        if (node->NoChildren())
            tgtnode->RemoveChild(node);

        node = AddElement(tgtnode, "ExtraCommands", 0, 0);
        AddArrayOfElements(node, "Add", "before", target->GetCommandsBeforeBuild());
        AddArrayOfElements(node, "Add", "after", target->GetCommandsAfterBuild());
        if (node->NoChildren())
            tgtnode->RemoveChild(node);
        else
        {
            if (target->GetAlwaysRunPostBuildSteps())
                AddElement(node, "Mode", "after", wxString(_T("always")));
        }

        SaveEnvironment(tgtnode, target);

        if (target->MakeCommandsModified())
        {
            TiXmlElement* makenode = AddElement(tgtnode, "MakeCommands", 0, 0);
            AddElement(makenode, "Build", "command", target->GetMakeCommandFor(mcBuild));
            AddElement(makenode, "CompileFile", "command", target->GetMakeCommandFor(mcCompileFile));
            AddElement(makenode, "Clean", "command", target->GetMakeCommandFor(mcClean));
            AddElement(makenode, "DistClean", "command", target->GetMakeCommandFor(mcDistClean));
        }
    }

    SaveEnvironment(buildnode, m_pProject);

    TiXmlElement* node = AddElement(prjnode, "Compiler", 0, 0);
    AddArrayOfElements(node, "Add", "option", m_pProject->GetCompilerOptions());
    AddArrayOfElements(node, "Add", "directory", m_pProject->GetIncludeDirs());
    if (node->NoChildren())
        prjnode->RemoveChild(node);

    node = AddElement(prjnode, "ResourceCompiler", 0, 0);
    AddArrayOfElements(node, "Add", "directory", m_pProject->GetResourceIncludeDirs());
    if (node->NoChildren())
        prjnode->RemoveChild(node);

    node = AddElement(prjnode, "Linker", 0, 0);
    AddArrayOfElements(node, "Add", "option", m_pProject->GetLinkerOptions());
    AddArrayOfElements(node, "Add", "library", m_pProject->GetLinkLibs());
    AddArrayOfElements(node, "Add", "directory", m_pProject->GetLibDirs());
    if (node->NoChildren())
        prjnode->RemoveChild(node);

    node = AddElement(prjnode, "ExtraCommands", 0, 0);
    AddArrayOfElements(node, "Add", "before", m_pProject->GetCommandsBeforeBuild());
    AddArrayOfElements(node, "Add", "after", m_pProject->GetCommandsAfterBuild());
    if (node->NoChildren())
        prjnode->RemoveChild(node);
    else
    {
        if (m_pProject->GetAlwaysRunPostBuildSteps())
            AddElement(node, "Mode", "after", wxString(_T("always")));
    }

    int count = m_pProject->GetFilesCount();
    for (int i = 0; i < count; ++i)
    {
        ProjectFile* f = m_pProject->GetFile(i);

        // do not save project files that do not belong in the target we 're exporting
        if (onlytgt && !onlytgt->GetFilesList().Find(f))
            continue;

        TiXmlElement* unitnode = AddElement(prjnode, "Unit", "filename", f->relativeFilename);
        AddElement(unitnode, "Option", "compilerVar", f->compilerVar);
        if (!f->compile)
            AddElement(unitnode, "Option", "compile", 0);
        if (!f->link)
            AddElement(unitnode, "Option", "link", 0);
        if (f->weight != 50)
            AddElement(unitnode, "Option", "weight", f->weight);

        // loop and save custom build commands
        for (pfCustomBuildMap::iterator it = f->customBuild.begin(); it != f->customBuild.end(); ++it)
        {
            pfCustomBuild& pfcb = it->second;
            if (!pfcb.buildCommand.IsEmpty())
            {
                wxString tmp = pfcb.buildCommand;
                tmp.Replace(_T("\n"), _T("\\n"));
                TiXmlElement* elem = AddElement(unitnode, "Option", "compiler", it->first);
                elem->SetAttribute("use", pfcb.useCustomBuildCommand ? "1" : "0");
                elem->SetAttribute("buildCommand", cbU2C(tmp));
            }
        }

        if (!f->GetObjName().IsEmpty())
        {
            wxFileName tmp(f->GetObjName());
            if (FileTypeOf(f->relativeFilename) != ftHeader &&
                tmp.GetExt() != compiler->GetSwitches().objectExtension)
            {
                AddElement(unitnode, "Option", "objectName", f->GetObjName());
            }
        }
        for (unsigned int x = 0; x < f->buildTargets.GetCount(); ++x)
            AddElement(unitnode, "Option", "target", f->buildTargets[x]);
    }

    // as a last step, run all hooked callbacks
    if (ProjectLoaderHooks::HasRegisteredHooks())
    {
        TiXmlElement* node = AddElement(prjnode, "Extensions", "", wxEmptyString);
        if (node)
        {
            ProjectLoaderHooks::CallHooks(m_pProject, node, false);
        }
    }

    return cbSaveTinyXMLDocument(&doc, filename);
}

wxString ProjectLoader::GetValidCompilerID(const wxString& proposal, const wxString& scope)
{
    if (CompilerFactory::GetCompiler(proposal))
        return proposal;

    m_OpenDirty = true;

    // check the map; maybe we asked the user before
    CompilerSubstitutes::iterator it = m_CompilerSubstitutes.find(proposal);
    if (it != m_CompilerSubstitutes.end())
        return it->second;

    Compiler* compiler = 0;

    // if compiler is a number, then this is an older version of the project file
    // propose the same compiler by index
    if (!proposal.IsEmpty())
    {
        long int idx = -1;
        proposal.ToLong(&idx);
        compiler = CompilerFactory::GetCompiler(idx);
    }

    if (!compiler)
    {
        wxString msg;
        msg.Printf(_("The defined compiler for %s cannot be located. "
                    "Please choose the compiler you want to use:"), scope.c_str());
        compiler = CompilerFactory::SelectCompilerUI(msg);
    }

    if (!compiler)
    {
        cbMessageBox(_("Setting to default compiler..."), _("Warning"), wxICON_WARNING);
        return CompilerFactory::GetDefaultCompilerID();
    }

    // finally, keep the user selection in the map so we don't ask him again
    m_CompilerSubstitutes[proposal] = compiler->GetID();
    return compiler->GetID();
}
