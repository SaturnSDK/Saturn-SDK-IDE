#include <sdk_precomp.h>
#include "scriptingmanager.h"
#include "cbexception.h"
#include "manager.h"
#include "messagemanager.h"
#include "configmanager.h"
#include "scriptingcall.h"
#include "as/bindings/scriptbindings.h"
#include <wx/msgdlg.h>
#include <wx/file.h>

static wxString s_Errors;

class asCOutputStream : public asIOutputStream
{
    public:
        void Write(const char *text)
        {
            s_Errors << text;
        }
};

//statics
static asCOutputStream asOut;
static ScriptingManager* pScripting = 0;
ScriptingManager* ScriptingManager::Get()
{
    if (!pScripting)
        pScripting = new ScriptingManager;
    return pScripting;
}

void ScriptingManager::Free()
{
    assert(pScripting);
    delete pScripting;
    pScripting = 0;
}

ScriptingManager::ScriptingManager()
{
    //ctor
	m_pEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if (!m_pEngine)
        cbThrow(_("Can't create scripting engine!"));

    m_pEngine->SetCommonMessageStream(&asOut);

    // register types
    RegisterBindings(m_pEngine);
}

ScriptingManager::~ScriptingManager()
{
    //dtor
	m_pEngine->Release();
}

wxString ScriptingManager::GetErrorDescription(int error)
{
    if (error >= 0) return _T("SUCCESS");

    switch (error)
    {
        case asERROR: return _T("ERROR");
        case asCONTEXT_ACTIVE: return _T("CONTEXT_ACTIVE");
        case asCONTEXT_NOT_FINISHED: return _T("CONTEXT_NOT_FINISHED");
        case asCONTEXT_NOT_PREPARED: return _T("CONTEXT_NOT_PREPARED");
        case asINVALID_ARG: return _T("INVALID_ARG");
        case asNO_FUNCTION: return _T("NO_FUNCTION");
        case asNOT_SUPPORTED: return _T("NOT_SUPPORTED");
        case asINVALID_NAME: return _T("INVALID_NAME");
        case asNAME_TAKEN: return _T("NAME_TAKEN");
        case asINVALID_DECLARATION: return _T("INVALID_DECLARATION");
        case asINVALID_OBJECT: return _T("INVALID_OBJECT");
        case asINVALID_TYPE: return _T("INVALID_TYPE");
        case asALREADY_REGISTERED: return _T("ALREADY_REGISTERED");
        case asMULTIPLE_FUNCTIONS: return _T("MULTIPLE_FUNCTIONS");
        case asNO_MODULE: return _T("NO_MODULE");
        case asNO_GLOBAL_VAR: return _T("NO_GLOBAL_VAR");
        case asINVALID_CONFIGURATION: return _T("INVALID_CONFIGURATION");
        case asINVALID_INTERFACE: return _T("INVALID_INTERFACE");
        case asCANT_BIND_ALL_FUNCTIONS: return _T("CANT_BIND_ALL_FUNCTIONS");
        case asLOWER_ARRAY_DIMENSION_NOT_REGISTERED: return _T("LOWER_ARRAY_DIMENSION_NOT_REGISTERED");
        case asWRONG_CONFIG_GROUP: return _T("WRONG_CONFIG_GROUP");
        case asCONFIG_GROUP_IS_IN_USE: return _T("CONFIG_GROUP_IS_IN_USE");
        default: return _T("NeverHere");
    }
    return _T("NeverHere");
}

bool ScriptingManager::DoLoadScript(const wxString& filename, wxString& script)
{
//    LOGSTREAM << _T("Trying to open script: ") << filename << '\n';
    wxLogNull ln;
    wxFile file(filename);
    if (!file.IsOpened())
        return false;
    int len = file.Length();
    if(!len)
    {
        file.Close();
        return false;
    }
    script.Clear();
    wxChar* buff = script.GetWriteBuf(len); // GetWriteBuf already handles the extra '\0'.
    file.Read((void*)buff, len);
    file.Close();
    script.UngetWriteBuf();
    return true;
}

int ScriptingManager::LoadScript(const wxString& filename, const wxString& module)
{
    wxString script;
    // try to load as-passed
    if (!DoLoadScript(filename, script))
    {
        // try in <data_path>/scripts/
        if (!DoLoadScript(ConfigManager::Get()->Read(_T("/data_path")) + _T("/scripts/") + filename, script))
        {
//            wxMessageBox(_("Can't open script ") + filename, _("Error"), wxICON_ERROR);
            return -1;
        }
    }

    s_Errors.Clear();

    // build script
	m_pEngine->AddScriptSection(module.c_str(), filename.c_str(), script.c_str(), script.Length(), 0, false);
	m_pEngine->Build(module.c_str());

    // locate and run "int main()"
	int funcID = FindFunctionByDeclaration("int main()", module);
	Executor<int> exec(funcID);
	int ret = exec.Call();

    // display errors (if any)
    if (!s_Errors.IsEmpty())
        wxMessageBox(s_Errors, _("Script error"), wxICON_ERROR);

	return ret;
}

int ScriptingManager::FindFunctionByDeclaration(const wxString& decl, const wxString& module)
{
	return m_pEngine->GetFunctionIDByDecl(module.c_str(), decl);
}

int ScriptingManager::FindFunctionByName(const wxString& name, const wxString& module)
{
	return m_pEngine->GetFunctionIDByName(module.c_str(), name);
}
