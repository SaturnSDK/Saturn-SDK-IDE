#include <sdk_precomp.h>
#include "scriptbindings.h"
#include "sc_wxstring.h"

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
    // register wxString in script
    Register_wxString(engine);

    // register types
    Register_Project(engine);
    Register_ProjectManager(engine);

    // register global functions
    engine->RegisterGlobalFunction("void ShowMessage(const wxString& in)", asFUNCTION(gShowMessage), asCALL_CDECL);
    engine->RegisterGlobalFunction("void Log(const wxString& in)", asFUNCTION(gDebugLog), asCALL_CDECL);
    engine->RegisterGlobalFunction("void SetAcceleratorFor(const wxString& in, const wxString& in)", asFUNCTION(gSetAcceleratorFor), asCALL_CDECL);
}

//------------------------------------------------------------------------------
// Project
//------------------------------------------------------------------------------
void Register_Project(asIScriptEngine* engine)
{
    engine->RegisterObjectType("Project", 0, asOBJ_CLASS);
    ADD_DUMMY_REFCOUNT(engine, Project);

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
    engine->RegisterObjectType("ProjectManagerClass", 0, asOBJ_CLASS);

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
