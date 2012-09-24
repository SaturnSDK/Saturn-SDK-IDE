/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6125 $
 * $Id: scriptbindings.cpp 6125 2010-01-31 03:56:32Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/scripting/bindings/scriptbindings.cpp $
 */

#include <sdk_precomp.h>
#ifndef CB_PRECOMP
    #include <settings.h>
    #include <manager.h>
    #include <logmanager.h>
    #include <configmanager.h>
    #include <editormanager.h>
    #include <projectmanager.h>
    #include <macrosmanager.h>
    #include <compilerfactory.h>
    #include <cbproject.h>
    #include <cbeditor.h>
    #include <globals.h>
#endif
#ifndef CB_FOR_CONSOLE
    #include "cbstyledtextctrl.h"
#endif // #ifndef CB_FOR_CONSOLE

#include "scriptbindings_base.h"
#include <cbexception.h>
#include "sc_base_types.h"

namespace ScriptBindings
{
    extern void Register_Constants();
    extern void Register_Base_Globals();
    extern void Register_wxTypes();
    extern void Register_Base_IO();

    SQInteger ConfigManager_Read(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 3)
        {
            wxString key = *SqPlus::GetInstance<wxString,false>(v, 2);
            if (sa.GetType(3) == OT_INTEGER)
                return sa.Return((SQInteger)Manager::Get()->GetConfigManager(_T("scripts"))->ReadInt(key, sa.GetInt(3)));
            else if (sa.GetType(3) == OT_BOOL)
                return sa.Return(Manager::Get()->GetConfigManager(_T("scripts"))->ReadBool(key, sa.GetBool(3)));
            else if (sa.GetType(3) == OT_FLOAT)
                return sa.Return((float)Manager::Get()->GetConfigManager(_T("scripts"))->ReadDouble(key, sa.GetFloat(3)));
            else
            {
                wxString val = *SqPlus::GetInstance<wxString,false>(v, 3);
                wxString ret = Manager::Get()->GetConfigManager(_T("scripts"))->Read(key, val);
                return SqPlus::ReturnCopy(v, ret);
            }
        }
        return sa.ThrowError("Invalid arguments to \"ConfigManager::Read\"");
    }
    SQInteger ConfigManager_Write(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 3)
        {
            wxString key = *SqPlus::GetInstance<wxString,false>(v, 2);
            if (sa.GetType(3) == OT_INTEGER)
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, (int)sa.GetInt(3));
                return SQ_OK;
            }
            else if (sa.GetType(3) == OT_BOOL)
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, (bool)sa.GetBool(3));
                return SQ_OK;
            }
            else if (sa.GetType(3) == OT_FLOAT)
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, sa.GetFloat(3));
                return SQ_OK;
            }
            else
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, *SqPlus::GetInstance<wxString,false>(v, 3));
                return SQ_OK;
            }
        }
        else if (paramCount == 4)
        {
            wxString key = *SqPlus::GetInstance<wxString,false>(v, 2);
            wxString val = *SqPlus::GetInstance<wxString,false>(v, 3);
            if (sa.GetType(4) == OT_BOOL)
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, val, sa.GetBool(4));
                return SQ_OK;
            }
        }
        return sa.ThrowError("Invalid arguments to \"ConfigManager::Write\"");
    }
    SQInteger cbProject_RemoveFile(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            cbProject* prj = SqPlus::GetInstance<cbProject,false>(v, 1);
            if (sa.GetType(2) == OT_INTEGER)
                return sa.ThrowError("Invalid arguments to \"cbProject::RemoveFile\"");
            else
                return sa.Return(prj->RemoveFile(SqPlus::GetInstance<ProjectFile,false>(v, 2)));
        }
        return sa.ThrowError("Invalid arguments to \"cbProject::RemoveFile\"");
    }
    SQInteger cbProject_AddFile(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount >= 3)
        {
            cbProject* prj = SqPlus::GetInstance<cbProject,false>(v, 1);
            wxString str = *SqPlus::GetInstance<wxString,false>(v, 3);
            bool b1 = paramCount >= 4 ? sa.GetBool(4) : true;
            bool b2 = paramCount >= 5 ? sa.GetBool(5) : true;
            int i = paramCount == 6 ? sa.GetInt(6) : 50;
            ProjectFile* pf = 0;
            if (sa.GetType(2) == OT_INTEGER)
                pf = prj->AddFile(sa.GetInt(2), str, b1, b2, i);
            else
                pf = prj->AddFile(*SqPlus::GetInstance<wxString,false>(v, 2), str, b1, b2, i);
            SqPlus::Push(v, pf);
            return 1;
        }
        return sa.ThrowError("Invalid arguments to \"cbProject::AddFile\"");
    }
    SQInteger cbProject_GetBuildTarget(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            cbProject* prj = SqPlus::GetInstance<cbProject,false>(v, 1);
            ProjectBuildTarget* bt = 0;
            if (sa.GetType(2) == OT_INTEGER)
                bt = prj->GetBuildTarget(sa.GetInt(2));
            else
                bt = prj->GetBuildTarget(*SqPlus::GetInstance<wxString,false>(v, 2));
            SqPlus::Push(v, bt);
            return 1;
        }
        return sa.ThrowError("Invalid arguments to \"cbProject::GetBuildTarget\"");
    }

    SQInteger ProjectManager_GetProjectCount(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int count = sa.GetParamCount();
        if (count != 1)
            return sa.ThrowError("Invalid arguments to \"ProjectManager::GetProjectCount\"");
        else
        {
            ProjectManager *manager = SqPlus::GetInstance<ProjectManager, false>(v, 1);
            int project_count = manager->GetProjects()->GetCount();
            return sa.Return((SQInteger)project_count);
        }
    }

    SQInteger ProjectManager_GetProject(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int count = sa.GetParamCount();
        if (count != 2)
            return sa.ThrowError("Invalid arguments to \"ProjectManager::GetProject\"");
        else
        {
            ProjectManager *manager = SqPlus::GetInstance<ProjectManager, false>(v, 1);
            int index = sa.GetInt(2);
            int project_count = manager->GetProjects()->GetCount();
            if(index >= project_count)
                return sa.ThrowError("Index out of bounds in \"ProjectManager::GetProject\"");
            else
            {
                cbProject *project = (*manager->GetProjects())[index];
                SqPlus::Push(v, project);
                return 1;
            }
        }
    }

    SQInteger CompilerFactory_GetCompilerIndex(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
            return sa.Return((SQInteger)CompilerFactory::GetCompilerIndex(*SqPlus::GetInstance<wxString,false>(v, 2)));
        return sa.ThrowError("Invalid arguments to \"CompilerFactory::GetCompilerIndex\"");
    }

    void RegisterBaseBindings()
    {
        if (!SquirrelVM::GetVMPtr())
            cbThrow(_T("Scripting engine not initialized!?"));

        Register_wxTypes();
        Register_Constants();
        Register_Base_Globals();
        Register_Base_IO(); // IO is enabled, but just for harmless functions

        SqPlus::SQClassDef<ConfigManager>("ConfigManager").
                staticFuncVarArgs(&ConfigManager_Read, "Read", "*").
                staticFuncVarArgs(&ConfigManager_Write, "Write", "*");

        SqPlus::SQClassDef<ProjectFile>("ProjectFile").
                func(&ProjectFile::AddBuildTarget, "AddBuildTarget").
                func(&ProjectFile::RenameBuildTarget, "RenameBuildTarget").
                func(&ProjectFile::RemoveBuildTarget, "RemoveBuildTarget").
                func(&ProjectFile::GetBaseName, "GetBaseName").
                func(&ProjectFile::GetObjName, "GetObjName").
                func(&ProjectFile::SetObjName, "SetObjName").
                func(&ProjectFile::GetParentProject, "GetParentProject").
                func(&ProjectFile::SetUseCustomBuildCommand, "SetUseCustomBuildCommand").
                func(&ProjectFile::SetCustomBuildCommand, "SetCustomBuildCommand").
                func(&ProjectFile::GetUseCustomBuildCommand, "GetUseCustomBuildCommand").
                func(&ProjectFile::GetCustomBuildCommand, "GetCustomBuildCommand").
                var(&ProjectFile::file, "file").
                var(&ProjectFile::relativeFilename, "relativeFilename").
                var(&ProjectFile::relativeToCommonTopLevelPath, "relativeToCommonTopLevelPath").
                var(&ProjectFile::compile, "compile").
                var(&ProjectFile::link, "link").
                var(&ProjectFile::weight, "weight").
                var(&ProjectFile::compilerVar, "compilerVar").
                var(&ProjectFile::buildTargets, "buildTargets");

        SqPlus::SQClassDef<CompileOptionsBase>("CompileOptionsBase").
                func(&CompileOptionsBase::AddPlatform, "AddPlatform").
                func(&CompileOptionsBase::RemovePlatform, "RemovePlatform").
                func(&CompileOptionsBase::SetPlatforms, "SetPlatforms").
                func(&CompileOptionsBase::GetPlatforms, "GetPlatforms").
                func(&CompileOptionsBase::SupportsCurrentPlatform, "SupportsCurrentPlatform").
                func(&CompileOptionsBase::SetLinkerOptions, "SetLinkerOptions").
                func(&CompileOptionsBase::SetLinkLibs, "SetLinkLibs").
                func(&CompileOptionsBase::SetCompilerOptions, "SetCompilerOptions").
                func(&CompileOptionsBase::SetIncludeDirs, "SetIncludeDirs").
                func(&CompileOptionsBase::SetResourceIncludeDirs, "SetResourceIncludeDirs").
                func(&CompileOptionsBase::SetLibDirs, "SetLibDirs").
                func(&CompileOptionsBase::SetCommandsBeforeBuild, "SetCommandsBeforeBuild").
                func(&CompileOptionsBase::SetCommandsAfterBuild, "SetCommandsAfterBuild").
                func(&CompileOptionsBase::GetLinkerOptions, "GetLinkerOptions").
                func(&CompileOptionsBase::GetLinkLibs, "GetLinkLibs").
                func(&CompileOptionsBase::GetCompilerOptions, "GetCompilerOptions").
                func(&CompileOptionsBase::GetIncludeDirs, "GetIncludeDirs").
                func(&CompileOptionsBase::GetResourceIncludeDirs, "GetResourceIncludeDirs").
                func(&CompileOptionsBase::GetLibDirs, "GetLibDirs").
                func(&CompileOptionsBase::GetCommandsBeforeBuild, "GetCommandsBeforeBuild").
                func(&CompileOptionsBase::GetCommandsAfterBuild, "GetCommandsAfterBuild").
                func(&CompileOptionsBase::GetModified, "GetModified").
                func(&CompileOptionsBase::SetModified, "SetModified").
                func(&CompileOptionsBase::AddLinkerOption, "AddLinkerOption").
                func(&CompileOptionsBase::AddLinkLib, "AddLinkLib").
                func(&CompileOptionsBase::AddCompilerOption, "AddCompilerOption").
                func(&CompileOptionsBase::AddIncludeDir, "AddIncludeDir").
                func(&CompileOptionsBase::AddResourceIncludeDir, "AddResourceIncludeDir").
                func(&CompileOptionsBase::AddLibDir, "AddLibDir").
                func(&CompileOptionsBase::AddCommandsBeforeBuild, "AddCommandsBeforeBuild").
                func(&CompileOptionsBase::AddCommandsAfterBuild, "AddCommandsAfterBuild").
                func(&CompileOptionsBase::RemoveLinkerOption, "RemoveLinkerOption").
                func(&CompileOptionsBase::RemoveLinkLib, "RemoveLinkLib").
                func(&CompileOptionsBase::RemoveCompilerOption, "RemoveCompilerOption").
                func(&CompileOptionsBase::RemoveIncludeDir, "RemoveIncludeDir").
                func(&CompileOptionsBase::RemoveResourceIncludeDir, "RemoveResourceIncludeDir").
                func(&CompileOptionsBase::RemoveLibDir, "RemoveLibDir").
                func(&CompileOptionsBase::RemoveCommandsBeforeBuild, "RemoveCommandsBeforeBuild").
                func(&CompileOptionsBase::RemoveCommandsAfterBuild, "RemoveCommandsAfterBuild").
                func(&CompileOptionsBase::GetAlwaysRunPostBuildSteps, "GetAlwaysRunPostBuildSteps").
                func(&CompileOptionsBase::SetAlwaysRunPostBuildSteps, "SetAlwaysRunPostBuildSteps").
                func(&CompileOptionsBase::SetBuildScripts, "SetBuildScripts").
                func(&CompileOptionsBase::GetBuildScripts, "GetBuildScripts").
                func(&CompileOptionsBase::AddBuildScript, "AddBuildScript").
                func(&CompileOptionsBase::RemoveBuildScript, "RemoveBuildScript").
                func(&CompileOptionsBase::SetVar, "SetVar").
                func(&CompileOptionsBase::GetVar, "GetVar").
                func(&CompileOptionsBase::UnsetVar, "UnsetVar").
                func(&CompileOptionsBase::UnsetAllVars, "UnsetAllVars");

        SqPlus::SQClassDef<CompileTargetBase>("CompileTargetBase", "CompileOptionsBase").
                func(&CompileTargetBase::SetTargetFilenameGenerationPolicy, "SetTargetFilenameGenerationPolicy").
//                func(&CompileTargetBase::GetTargetFilenameGenerationPolicy, "GetTargetFilenameGenerationPolicy"). // not exposed because its args are non-const references
                func(&CompileTargetBase::GetFilename, "GetFilename").
                func(&CompileTargetBase::GetTitle, "GetTitle").
                func(&CompileTargetBase::SetTitle, "SetTitle").
                func(&CompileTargetBase::SetOutputFilename, "SetOutputFilename").
                func(&CompileTargetBase::SetWorkingDir, "SetWorkingDir").
                func(&CompileTargetBase::SetObjectOutput, "SetObjectOutput").
                func(&CompileTargetBase::SetDepsOutput, "SetDepsOutput").
                func(&CompileTargetBase::GetOptionRelation, "GetOptionRelation").
                func(&CompileTargetBase::SetOptionRelation, "SetOptionRelation").
                func(&CompileTargetBase::GetWorkingDir, "GetWorkingDir").
                func(&CompileTargetBase::GetObjectOutput, "GetObjectOutput").
                func(&CompileTargetBase::GetDepsOutput, "GetDepsOutput").
                func(&CompileTargetBase::GetOutputFilename, "GetOutputFilename").
                func(&CompileTargetBase::SuggestOutputFilename, "SuggestOutputFilename").
                func(&CompileTargetBase::GetExecutableFilename, "GetExecutableFilename").
                func(&CompileTargetBase::GetDynamicLibFilename, "GetDynamicLibFilename").
                func(&CompileTargetBase::GetDynamicLibDefFilename, "GetDynamicLibDefFilename").
                func(&CompileTargetBase::GetStaticLibFilename, "GetStaticLibFilename").
                func(&CompileTargetBase::GetBasePath, "GetBasePath").
                func(&CompileTargetBase::SetTargetType, "SetTargetType").
                func(&CompileTargetBase::GetTargetType, "GetTargetType").
                func(&CompileTargetBase::GetExecutionParameters, "GetExecutionParameters").
                func(&CompileTargetBase::SetExecutionParameters, "SetExecutionParameters").
                func(&CompileTargetBase::GetHostApplication, "GetHostApplication").
                func(&CompileTargetBase::SetHostApplication, "SetHostApplication").
                func(&CompileTargetBase::SetCompilerID, "SetCompilerID").
                func(&CompileTargetBase::GetCompilerID, "GetCompilerID").
                func(&CompileTargetBase::GetMakeCommandFor, "GetMakeCommandFor").
                func(&CompileTargetBase::SetMakeCommandFor, "SetMakeCommandFor").
                func(&CompileTargetBase::MakeCommandsModified, "MakeCommandsModified");

        SqPlus::SQClassDef<ProjectBuildTarget>("ProjectBuildTarget", "CompileTargetBase").
                func(&ProjectBuildTarget::GetParentProject, "GetParentProject").
                func(&ProjectBuildTarget::GetFullTitle, "GetFullTitle").
                func(&ProjectBuildTarget::GetExternalDeps, "GetExternalDeps").
                func(&ProjectBuildTarget::SetExternalDeps, "SetExternalDeps").
                func(&ProjectBuildTarget::SetAdditionalOutputFiles, "SetAdditionalOutputFiles").
                func(&ProjectBuildTarget::GetAdditionalOutputFiles, "GetAdditionalOutputFiles").
                func(&ProjectBuildTarget::GetIncludeInTargetAll, "GetIncludeInTargetAll").
                func(&ProjectBuildTarget::SetIncludeInTargetAll, "SetIncludeInTargetAll").
                func(&ProjectBuildTarget::GetCreateDefFile, "GetCreateDefFile").
                func(&ProjectBuildTarget::SetCreateDefFile, "SetCreateDefFile").
                func(&ProjectBuildTarget::GetCreateStaticLib, "GetCreateStaticLib").
                func(&ProjectBuildTarget::SetCreateStaticLib, "SetCreateStaticLib").
                func(&ProjectBuildTarget::GetUseConsoleRunner, "GetUseConsoleRunner").
                func(&ProjectBuildTarget::SetUseConsoleRunner, "SetUseConsoleRunner").
                func(&ProjectBuildTarget::GetFilesCount, "GetFilesCount").
                func(&ProjectBuildTarget::GetFile, "GetFile");

        SqPlus::SQClassDef<cbProject>("cbProject", "CompileTargetBase").
                func(&cbProject::GetModified, "GetModified").
                func(&cbProject::SetModified, "SetModified").
                func(&cbProject::GetMakefile, "GetMakefile").
                func(&cbProject::SetMakefile, "SetMakefile").
                func(&cbProject::IsMakefileCustom, "IsMakefileCustom").
                func(&cbProject::SetMakefileCustom, "SetMakefileCustom").
                func(&cbProject::GetCommonTopLevelPath, "GetCommonTopLevelPath").
                func(&cbProject::GetFilesCount, "GetFilesCount").
                func(&cbProject::GetFile, "GetFile").
                func(&cbProject::GetFileByFilename, "GetFileByFilename").
                staticFuncVarArgs(&cbProject_RemoveFile, "RemoveFile", "*").
                staticFuncVarArgs(&cbProject_AddFile, "AddFile", "*").
                func(&cbProject::GetBuildTargetsCount, "GetBuildTargetsCount").
                staticFuncVarArgs(&cbProject_GetBuildTarget, "GetBuildTarget", "*").
                func(&cbProject::AddBuildTarget, "AddBuildTarget").
                func(&cbProject::BuildTargetValid, "BuildTargetValid").
                func(&cbProject::GetFirstValidBuildTargetName, "GetFirstValidBuildTargetName").
                func(&cbProject::SetDefaultExecuteTarget, "SetDefaultExecuteTarget").
                func(&cbProject::GetDefaultExecuteTarget, "GetDefaultExecuteTarget").
                func(&cbProject::SetActiveBuildTarget, "SetActiveBuildTarget").
                func(&cbProject::GetActiveBuildTarget, "GetActiveBuildTarget").
                func(&cbProject::GetCurrentlyCompilingTarget, "GetCurrentlyCompilingTarget").
                func(&cbProject::SetCurrentlyCompilingTarget, "SetCurrentlyCompilingTarget").
                func(&cbProject::GetModeForPCH, "GetModeForPCH").
                func(&cbProject::SetModeForPCH, "SetModeForPCH").
                func(&cbProject::SetExtendedObjectNamesGeneration, "SetExtendedObjectNamesGeneration").
                func(&cbProject::GetExtendedObjectNamesGeneration, "GetExtendedObjectNamesGeneration").
                func(&cbProject::AddToExtensions, "AddToExtensions").
                func(&cbProject::DefineVirtualBuildTarget, "DefineVirtualBuildTarget").
                func(&cbProject::HasVirtualBuildTarget, "HasVirtualBuildTarget").
                func(&cbProject::RemoveVirtualBuildTarget, "RemoveVirtualBuildTarget").
                func(&cbProject::GetVirtualBuildTargets, "GetVirtualBuildTargets").
                func(&cbProject::GetVirtualBuildTargetGroup, "GetVirtualBuildTargetGroup").
                func(&cbProject::GetExpandedVirtualBuildTargetGroup, "GetExpandedVirtualBuildTargetGroup").
                func(&cbProject::CanAddToVirtualBuildTarget, "CanAddToVirtualBuildTarget").
                func(&cbProject::SetTitle, "SetTitle");


        SqPlus::SQClassDef<ProjectManager>("ProjectManager").
                func(&ProjectManager::GetDefaultPath, "GetDefaultPath").
                func(&ProjectManager::SetDefaultPath, "SetDefaultPath").
                func(&ProjectManager::GetActiveProject, "GetActiveProject").
                staticFuncVarArgs(&ProjectManager_GetProjectCount, "GetProjectCount", "*").
                staticFuncVarArgs(&ProjectManager_GetProject, "GetProject", "*").
                func(&ProjectManager::SetProject, "SetProject").
                func(&ProjectManager::LoadWorkspace, "LoadWorkspace").
                func(&ProjectManager::IsOpen, "IsOpen").
                func(&ProjectManager::LoadProject, "LoadProject").
                func(&ProjectManager::CloseProject, "CloseProject").
                func(&ProjectManager::CloseActiveProject, "CloseActiveProject").
                func(&ProjectManager::CloseAllProjects, "CloseAllProjects").
                func(&ProjectManager::NewProject, "NewProject").
                func(&ProjectManager::AddProjectDependency, "AddProjectDependency").
                func(&ProjectManager::RemoveProjectDependency, "RemoveProjectDependency").
                func(&ProjectManager::ClearProjectDependencies, "ClearProjectDependencies").
                func(&ProjectManager::RemoveProjectFromAllDependencies, "RemoveProjectFromAllDependencies");

        SqPlus::SQClassDef<UserVariableManager>("UserVariableManager").
                func(&UserVariableManager::Exists, "Exists");


        typedef bool(*CF_INHERITSFROM)(const wxString&, const wxString&); // CompilerInheritsFrom

        SqPlus::SQClassDef<CompilerFactory>("CompilerFactory").
                staticFunc(&CompilerFactory::IsValidCompilerID, "IsValidCompilerID").
                staticFuncVarArgs(&CompilerFactory_GetCompilerIndex, "GetCompilerIndex", "*").
                staticFunc(&CompilerFactory::GetDefaultCompilerID, "GetDefaultCompilerID").
                staticFunc(&CompilerFactory::GetCompilerVersionString, "GetCompilerVersionString").
                staticFunc<CF_INHERITSFROM>(&CompilerFactory::CompilerInheritsFrom, "CompilerInheritsFrom");

        SqPlus::SQClassDef<PluginInfo>("PluginInfo").
            emptyCtor().
            var(&PluginInfo::name, "name").
            var(&PluginInfo::title, "title").
            var(&PluginInfo::version, "version").
            var(&PluginInfo::description, "description").
            var(&PluginInfo::author, "author").
            var(&PluginInfo::authorEmail, "authorEmail").
            var(&PluginInfo::authorWebsite, "authorWebsite").
            var(&PluginInfo::thanksTo, "thanksTo").
            var(&PluginInfo::license, "license");

        SqPlus::SQClassDef<FileTreeData>("FileTreeData").
            func(&FileTreeData::GetKind, "GetKind").
            func(&FileTreeData::GetProject, "GetProject").
            func(&FileTreeData::GetFileIndex, "GetFileIndex").
            func(&FileTreeData::GetProjectFile, "GetProjectFile").
            func(&FileTreeData::GetFolder, "GetFolder").
            func(&FileTreeData::SetKind, "SetKind").
            func(&FileTreeData::SetProject, "SetProject").
            func(&FileTreeData::SetFileIndex, "SetFileIndex").
            func(&FileTreeData::SetProjectFile, "SetProjectFile").
            func(&FileTreeData::SetFolder, "SetFolder");

    }
} // namespace ScriptBindings
