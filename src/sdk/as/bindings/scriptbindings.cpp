#include <sdk_precomp.h>
#include "scriptbindings.h"
#include "sc_wxstring.h"
#include "sc_wxarraystring.h"

// In Code::Blocks nothing is refcounted.
// In order to use objects as handles (aka pointers), we must provide
// these two dummy functions.
#define ADD_DUMMY_REFCOUNT(engine,api) \
    engine->RegisterObjectBehaviour(#api, asBEHAVE_ADDREF, "void f()", asFUNCTION(DummyAddRef), asCALL_CDECL_OBJLAST); \
    engine->RegisterObjectBehaviour(#api, asBEHAVE_RELEASE, "void f()", asFUNCTION(DummyRelease), asCALL_CDECL_OBJLAST)

void DummyAddRef(cbProject& p){}
void DummyRelease(cbProject& p){}

//------------------------------------------------------------------------------
// Forwards
//------------------------------------------------------------------------------
void Register_ProjectFile(asIScriptEngine* engine);
void Register_ProjectBuildTarget(asIScriptEngine* engine);
void Register_Project(asIScriptEngine* engine);
void Register_ProjectManager(asIScriptEngine* engine);

//------------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------------
void gShowMessage(const wxString& msg){ wxMessageBox(msg, _("Script message")); }
void gDebugLog(const wxString& msg){ LOGSTREAM << msg << '\n'; }
void gSetAcceleratorFor(const wxString& m, const wxString& s)
{
    wxMenuBar* bar = Manager::Get()->GetAppWindow()->GetMenuBar();
    if (!bar) return;

    wxString sm = m.BeforeFirst(_T('|'));
    wxString si = m.AfterFirst(_T('|'));
    int id = bar->FindMenuItem(sm, si);
    if (id == wxNOT_FOUND) return;

    wxMenuItem* item = bar->FindItem(id);
    if (!item) return;

    item->SetText(item->GetText().BeforeFirst('\t') + '\t' + s);
}

//------------------------------------------------------------------------------
// Actual registration
//------------------------------------------------------------------------------
void RegisterBindings(asIScriptEngine* engine)
{
    // register wx types in script
    Register_wxString(engine);
    Register_wxArrayString(engine);

    // register types
    engine->RegisterObjectType("ProjectFile", 0, asOBJ_CLASS);
    ADD_DUMMY_REFCOUNT(engine, ProjectFile);
    engine->RegisterObjectType("BuildTarget", 0, asOBJ_CLASS);
    ADD_DUMMY_REFCOUNT(engine, BuildTarget);
    engine->RegisterObjectType("Project", 0, asOBJ_CLASS);
    ADD_DUMMY_REFCOUNT(engine, Project);
    engine->RegisterObjectType("ProjectManagerClass", 0, asOBJ_CLASS);

    // register member functions
    Register_ProjectFile(engine);
    Register_ProjectBuildTarget(engine);
    Register_Project(engine);
    Register_ProjectManager(engine);

    // register global functions
    engine->RegisterGlobalFunction("void ShowMessage(const wxString& in)", asFUNCTION(gShowMessage), asCALL_CDECL);
    engine->RegisterGlobalFunction("void Log(const wxString& in)", asFUNCTION(gDebugLog), asCALL_CDECL);
    engine->RegisterGlobalFunction("void SetAcceleratorFor(const wxString& in, const wxString& in)", asFUNCTION(gSetAcceleratorFor), asCALL_CDECL);
}

//------------------------------------------------------------------------------
// CompileOptionsBase
//------------------------------------------------------------------------------
template <class T> void Register_CompileOptionsBase(asIScriptEngine* engine, const wxString& classname)
{
    engine->RegisterObjectMethod(classname, "void SetBuildConfiguration(int)", asMETHOD(T, SetBuildConfiguration), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetLinkerOptions(const wxArrayString& in)", asMETHOD(T, SetLinkerOptions), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetLinkLibs(const wxArrayString& in)", asMETHOD(T, SetLinkLibs), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetCompilerOptions(const wxArrayString& in)", asMETHOD(T, SetCompilerOptions), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetIncludeDirs(const wxArrayString& in)", asMETHOD(T, SetIncludeDirs), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetResourceIncludeDirs(const wxArrayString& in)", asMETHOD(T, SetResourceIncludeDirs), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetLibDirs(const wxArrayString& in)", asMETHOD(T, SetLibDirs), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetCommandsBeforeBuild(const wxArrayString& in)", asMETHOD(T, SetCommandsBeforeBuild), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetCommandsAfterBuild(const wxArrayString& in)", asMETHOD(T, SetCommandsAfterBuild), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "int& GetBuildConfiguration()", asMETHOD(T, GetBuildConfiguration), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxArrayString& GetLinkerOptions()", asMETHOD(T, GetLinkerOptions), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxArrayString& GetLinkLibs()", asMETHOD(T, GetLinkLibs), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxArrayString& GetCompilerOptions()", asMETHOD(T, GetCompilerOptions), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxArrayString& GetIncludeDirs()", asMETHOD(T, GetIncludeDirs), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxArrayString& GetResourceIncludeDirs()", asMETHOD(T, GetResourceIncludeDirs), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxArrayString& GetLibDirs()", asMETHOD(T, GetLibDirs), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxArrayString& GetCommandsBeforeBuild()", asMETHOD(T, GetCommandsBeforeBuild), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxArrayString& GetCommandsAfterBuild()", asMETHOD(T, GetCommandsAfterBuild), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "bool GetModified()", asMETHOD(T, GetModified), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetModified(bool)", asMETHOD(T, SetModified), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void AddLinkerOption(const wxString& in)", asMETHOD(T, AddLinkerOption), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void AddLinkLib(const wxString& in)", asMETHOD(T, AddLinkLib), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void AddCompilerOption(const wxString& in)", asMETHOD(T, AddCompilerOption), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void AddIncludeDir(const wxString& in)", asMETHOD(T, AddIncludeDir), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void AddResourceIncludeDir(const wxString& in)", asMETHOD(T, AddResourceIncludeDir), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void AddLibDir(const wxString& in)", asMETHOD(T, AddLibDir), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void AddCommandsBeforeBuild(const wxString& in)", asMETHOD(T, AddCommandsBeforeBuild), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void AddCommandsAfterBuild(const wxString& in)", asMETHOD(T, AddCommandsAfterBuild), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "bool GetAlwaysRunPreBuildSteps()", asMETHOD(T, GetAlwaysRunPreBuildSteps), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "bool GetAlwaysRunPostBuildSteps()", asMETHOD(T, GetAlwaysRunPostBuildSteps), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetAlwaysRunPreBuildSteps(bool)", asMETHOD(T, SetAlwaysRunPreBuildSteps), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetAlwaysRunPostBuildSteps(bool)", asMETHOD(T, SetAlwaysRunPostBuildSteps), asCALL_THISCALL);
}

//------------------------------------------------------------------------------
// CompileTargetBase
//------------------------------------------------------------------------------
template <class T> void Register_CompileTargetBase(asIScriptEngine* engine, const wxString& classname)
{
    // add CompileOptionsBase methods/properties
    Register_CompileOptionsBase<T>(engine, classname);

    engine->RegisterObjectMethod(classname, "wxString& GetFilename()", asMETHOD(T, GetFilename), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString& GetTitle()", asMETHOD(T, GetTitle), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetTitle(const wxString& in)", asMETHOD(T, SetTitle), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetOutputFilename(const wxString& in)", asMETHOD(T, SetOutputFilename), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetWorkingDir(const wxString& in)", asMETHOD(T, SetWorkingDir), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetObjectOutput(const wxString& in)", asMETHOD(T, SetObjectOutput), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetDepsOutput(const wxString& in)", asMETHOD(T, SetDepsOutput), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "int GetOptionRelation(int)", asMETHOD(T, GetOptionRelation), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetOptionRelation(int,int)", asMETHOD(T, SetOptionRelation), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString GetWorkingDir()", asMETHOD(T, GetWorkingDir), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString GetObjectOutput()", asMETHOD(T, GetObjectOutput), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString GetDepsOutput()", asMETHOD(T, GetDepsOutput), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString GetOutputFilename()", asMETHOD(T, GetOutputFilename), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString SuggestOutputFilename()", asMETHOD(T, SuggestOutputFilename), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString GetExecutableFilename()", asMETHOD(T, GetExecutableFilename), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString GetDynamicLibFilename()", asMETHOD(T, GetDynamicLibFilename), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString GetStaticLibFilename()", asMETHOD(T, GetStaticLibFilename), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString GetBasePath()", asMETHOD(T, GetBasePath), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetTargetType(int)", asMETHOD(T, SetTargetType), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "int& GetTargetType()", asMETHOD(T, GetTargetType), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString& GetExecutionParameters()", asMETHOD(T, GetExecutionParameters), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetExecutionParameters(const wxString& in)", asMETHOD(T, SetExecutionParameters), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "wxString& GetHostApplication()", asMETHOD(T, GetHostApplication), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetHostApplication(const wxString& in)", asMETHOD(T, SetHostApplication), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "void SetCompilerIndex(int)", asMETHOD(T, SetCompilerIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod(classname, "int GetCompilerIndex()", asMETHOD(T, GetCompilerIndex), asCALL_THISCALL);
}

//------------------------------------------------------------------------------
// ProjectFile
//------------------------------------------------------------------------------
void Register_ProjectFile(asIScriptEngine* engine)
{
    engine->RegisterObjectMethod("ProjectFile", "void AddBuildTarget(const wxString& in)", asMETHOD(ProjectFile, AddBuildTarget), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectFile", "void RenameBuildTarget(const wxString& in, const wxString& in)", asMETHOD(ProjectFile, RenameBuildTarget), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectFile", "void RemoveBuildTarget(const wxString& in)", asMETHOD(ProjectFile, RemoveBuildTarget), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectFile", "void ClearBreakpoints()", asMETHOD(ProjectFile, ClearBreakpoints), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectFile", "void SetBreakpoint(int)", asMETHOD(ProjectFile, SetBreakpoint), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectFile", "void RemoveBreakpoint(int)", asMETHOD(ProjectFile, RemoveBreakpoint), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectFile", "void ToggleBreakpoint(int)", asMETHOD(ProjectFile, ToggleBreakpoint), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectFile", "wxString GetBaseName()", asMETHOD(ProjectFile, GetBaseName), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectFile", "wxString& GetObjName()", asMETHOD(ProjectFile, GetObjName), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectFile", "void SetObjName(const wxString& in)", asMETHOD(ProjectFile, SetObjName), asCALL_THISCALL);

    engine->RegisterObjectProperty("ProjectFile", "Project@ project", offsetof(ProjectFile, project));
    engine->RegisterObjectProperty("ProjectFile", "wxString relativeFilename", offsetof(ProjectFile, relativeFilename));
    engine->RegisterObjectProperty("ProjectFile", "wxString relativeToCommonTopLevelPath", offsetof(ProjectFile, relativeToCommonTopLevelPath));
    engine->RegisterObjectProperty("ProjectFile", "bool compile", offsetof(ProjectFile, compile));
    engine->RegisterObjectProperty("ProjectFile", "bool link", offsetof(ProjectFile, link));
    engine->RegisterObjectProperty("ProjectFile", "uint16 weight", offsetof(ProjectFile, weight));
    engine->RegisterObjectProperty("ProjectFile", "wxString buildCommand", offsetof(ProjectFile, buildCommand));
    engine->RegisterObjectProperty("ProjectFile", "bool useCustomBuildCommand", offsetof(ProjectFile, useCustomBuildCommand));
    engine->RegisterObjectProperty("ProjectFile", "bool autoDeps", offsetof(ProjectFile, autoDeps));
    engine->RegisterObjectProperty("ProjectFile", "wxString customDeps", offsetof(ProjectFile, customDeps));
    engine->RegisterObjectProperty("ProjectFile", "wxString compilerVar", offsetof(ProjectFile, compilerVar));
}

//------------------------------------------------------------------------------
// ProjectBuildTarget
//------------------------------------------------------------------------------
void Register_ProjectBuildTarget(asIScriptEngine* engine)
{
    // add CompileTargetBase methods/properties
    Register_CompileTargetBase<ProjectBuildTarget>(engine, "BuildTarget");

    engine->RegisterObjectMethod("BuildTarget", "Project@ GetParentProject()", asMETHOD(ProjectBuildTarget, GetParentProject), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "wxString GetFullTitle()", asMETHOD(ProjectBuildTarget, GetFullTitle), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "wxString& GetExternalDeps()", asMETHOD(ProjectBuildTarget, GetExternalDeps), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "void SetExternalDeps(const wxString& in)", asMETHOD(ProjectBuildTarget, SetExternalDeps), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "void SetAdditionalOutputFiles(const wxString& in)", asMETHOD(ProjectBuildTarget, SetAdditionalOutputFiles), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "wxString& GetAdditionalOutputFiles()", asMETHOD(ProjectBuildTarget, GetAdditionalOutputFiles), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "bool GetIncludeInTargetAll()", asMETHOD(ProjectBuildTarget, GetIncludeInTargetAll), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "void SetIncludeInTargetAll(bool)", asMETHOD(ProjectBuildTarget, SetIncludeInTargetAll), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "bool GetCreateDefFile()", asMETHOD(ProjectBuildTarget, GetCreateDefFile), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "void SetCreateDefFile(bool)", asMETHOD(ProjectBuildTarget, SetCreateDefFile), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "bool GetCreateStaticLib()", asMETHOD(ProjectBuildTarget, GetCreateStaticLib), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "void SetCreateStaticLib(bool)", asMETHOD(ProjectBuildTarget, SetCreateStaticLib), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "bool GetUseConsoleRunner()", asMETHOD(ProjectBuildTarget, GetUseConsoleRunner), asCALL_THISCALL);
    engine->RegisterObjectMethod("BuildTarget", "void SetUseConsoleRunner(bool)", asMETHOD(ProjectBuildTarget, SetUseConsoleRunner), asCALL_THISCALL);
}

//------------------------------------------------------------------------------
// Project
//------------------------------------------------------------------------------
void Register_Project(asIScriptEngine* engine)
{
    // add CompileTargetBase methods/properties
    Register_CompileTargetBase<cbProject>(engine, "Project");

    engine->RegisterObjectMethod("Project", "bool GetModified()", asMETHOD(cbProject, GetModified), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "void SetModified(bool)", asMETHOD(cbProject, SetModified), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "void SetMakefile(const wxString& in)", asMETHOD(cbProject, SetMakefile), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "wxString& GetMakefile()", asMETHOD(cbProject, GetMakefile), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "void SetMakefileCustom(bool)", asMETHOD(cbProject, SetMakefileCustom), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "bool IsMakefileCustom()", asMETHOD(cbProject, IsMakefileCustom), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "bool CloseAllFiles()", asMETHOD(cbProject, CloseAllFiles), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "bool SaveAllFiles()", asMETHOD(cbProject, SaveAllFiles), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "bool Save()", asMETHOD(cbProject, Save), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "bool SaveAs()", asMETHOD(cbProject, SaveAs), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "bool SaveLayout()", asMETHOD(cbProject, SaveLayout), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "bool LoadLayout()", asMETHOD(cbProject, LoadLayout), asCALL_THISCALL);
    engine->RegisterObjectMethod("Project", "bool ShowOptions()", asMETHOD(cbProject, ShowOptions), asCALL_THISCALL);
}

//------------------------------------------------------------------------------
// ProjectManager
//------------------------------------------------------------------------------
void Register_ProjectManager(asIScriptEngine* engine)
{
    engine->RegisterObjectMethod("ProjectManagerClass", "Project@ GetActiveProject()", asMETHOD(ProjectManager, GetActiveProject), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "void SetProject(Project@, bool)", asMETHOD(ProjectManager, SetProject), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool LoadWorkspace(const wxString& in)", asMETHOD(ProjectManager, LoadWorkspace), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool SaveWorkspace()", asMETHOD(ProjectManager, SaveWorkspace), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool SaveWorkspaceAs(const wxString& in)", asMETHOD(ProjectManager, SaveWorkspaceAs), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool CloseWorkspaceAs()", asMETHOD(ProjectManager, CloseWorkspace), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "Project@ LoadProject(const wxString& in)", asMETHOD(ProjectManager, LoadProject), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool SaveProject(Project@)", asMETHOD(ProjectManager, SaveProject), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool SaveProjectAs(Project@)", asMETHOD(ProjectManager, SaveProjectAs), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool SaveActiveProject()", asMETHOD(ProjectManager, SaveActiveProject), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool SaveActiveProjectAs()", asMETHOD(ProjectManager, SaveActiveProjectAs), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool SaveAllProjects()", asMETHOD(ProjectManager, SaveAllProjects), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool CloseProject(Project@)", asMETHOD(ProjectManager, CloseProject), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool CloseActiveProject()", asMETHOD(ProjectManager, CloseActiveProject), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "bool CloseAllProjects()", asMETHOD(ProjectManager, CloseAllProjects), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "Project@ NewProject()", asMETHOD(ProjectManager, NewProject), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "int AddFileToProject(const wxString& in, Project@, int)", asMETHODPR(ProjectManager, AddFileToProject, (const wxString&,cbProject*,int), int), asCALL_THISCALL);
    engine->RegisterObjectMethod("ProjectManagerClass", "int AskForBuildTargetIndex(Project@)", asMETHOD(ProjectManager, AskForBuildTargetIndex), asCALL_THISCALL);

    // actually bind ProjectManager's instance
    engine->RegisterGlobalProperty("ProjectManagerClass ProjectManager", Manager::Get()->GetProjectManager());
}
