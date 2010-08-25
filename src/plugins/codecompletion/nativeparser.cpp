/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>

#include "nativeparser.h"
#include "classbrowser.h"
#include "parser/parser.h"

#include <manager.h>
#include <configmanager.h>
#include <projectmanager.h>
#include <pluginmanager.h>
#include <logmanager.h>
#include <editormanager.h>
#include <macrosmanager.h>
#include <cbeditor.h>
#include <cbproject.h>
#include <cbexception.h>
#include <cbstyledtextctrl.h>
#include <compilerfactory.h>
#include <projectloader_hooks.h>
#include <tinyxml/tinyxml.h>

#include "cbauibook.h"
#include <wx/log.h> // for wxSafeShowMessage()
#include <wx/regex.h>
#include <wx/wfstream.h>
#include <wx/tokenzr.h>

#include <cctype>

#define CC_NATIVE_PARSER_DEBUG_OUTPUT 0

#if CC_NATIVE_PARSER_DEBUG_OUTPUT
    #define TRACE(format, args...)\
    Manager::Get()->GetLogManager()->DebugLog(F( format , ## args))
#else
    #define TRACE(format, args...)
#endif

int idTimerEditorActivated = wxNewId();

bool s_DebugSmartSense = false;
const wxString g_StartHereTitle = _("Start here");

BEGIN_EVENT_TABLE(NativeParser, wxEvtHandler)
//    EVT_MENU(THREAD_START, NativeParser::OnThreadStart)
//    EVT_MENU(THREAD_END, NativeParser::OnThreadEnd)
    EVT_MENU(PARSER_START, NativeParser::OnParserStart)
    EVT_MENU(PARSER_END, NativeParser::OnParserEnd)
    EVT_TIMER(idTimerEditorActivated, NativeParser::OnEditorActivatedTimer)
END_EVENT_TABLE()

NativeParser::NativeParser() :
    m_TempParser(this),
    m_pParser(&m_TempParser),
    m_EditorStartWord(-1),
    m_EditorEndWord(-1),
    m_TimerEditorActivated(this, idTimerEditorActivated),
    m_CallTipCommas(0),
    m_pClassBrowser(0),
    m_GettingCalltips(false),
    m_ClassBrowserIsFloating(false),
    m_LastAISearchWasGlobal(false),
    m_pImageList(0L)
{
    m_TemplateMap.clear();

    // hook to project loading procedure
    ProjectLoaderHooks::HookFunctorBase* myhook = new ProjectLoaderHooks::HookFunctor<NativeParser>(this, &NativeParser::OnProjectLoadingHook);
    m_HookId = ProjectLoaderHooks::RegisterHook(myhook);

    m_pImageList = new wxImageList(16, 16);
    wxBitmap bmp;
    wxString prefix;
    prefix = ConfigManager::GetDataFolder() + _T("/images/codecompletion/");
    // bitmaps must be added by order of PARSER_IMG_* consts
    bmp = cbLoadBitmap(prefix + _T("class_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("class.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS
    bmp = cbLoadBitmap(prefix + _T("class_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("class_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("class_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("ctor_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CTOR_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("ctor_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CTOR_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("ctor_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CTOR_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("dtor_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_DTOR_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("dtor_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_DTOR_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("dtor_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_DTOR_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("method_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNC_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("method_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNC_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("method_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNC_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("var_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VAR_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("var_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VAR_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("var_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VAR_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("preproc.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_PREPROCESSOR
    bmp = cbLoadBitmap(prefix + _T("enum.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUM
    bmp = cbLoadBitmap(prefix + _T("enum_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUM_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("enum_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUM_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("enum_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUM_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("enumerator.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUMERATOR
    bmp = cbLoadBitmap(prefix + _T("namespace.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_NAMESPACE
    bmp = cbLoadBitmap(prefix + _T("typedef.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF
    bmp = cbLoadBitmap(prefix + _T("typedef_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("typedef_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("typedef_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("symbols_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_SYMBOLS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("vars_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VARS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("funcs_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNCS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("enums_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUMS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("preproc_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_PREPROC_FOLDER
    bmp = cbLoadBitmap(prefix + _T("others_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_OTHERS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("typedefs_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF_FOLDER
    bmp = cbLoadBitmap(prefix + _T("macro.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO
    bmp = cbLoadBitmap(prefix + _T("macro_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("macro_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("macro_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("macro_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO_FOLDER
}

NativeParser::~NativeParser()
{
    ProjectLoaderHooks::UnregisterHook(m_HookId, true);
    ClearParsers();
    RemoveClassBrowser();
    delete m_pImageList;
}

void NativeParser::SetParser(Parser* parser)
{
    m_pParser = parser;
    if (m_pClassBrowser)
    {
        m_pClassBrowser->SetParser(parser);
        m_pClassBrowser->UpdateView();
    }
}

Parser* NativeParser::GetParserByProject(cbProject* project)
{
    for (ParserList::const_iterator it = m_ParserList.begin(); it != m_ParserList.end(); ++it)
    {
        if (it->first == project)
            return it->second;
    }

    return NULL;
}

Parser* NativeParser::GetParserByFilename(const wxString& filename)
{
    cbProject* project = GetProjectByFilename(filename);
    return GetParserByProject(project);
}

cbProject* NativeParser::GetProjectByParser(Parser* parser)
{
    for (ParserList::const_iterator it = m_ParserList.begin(); it != m_ParserList.end(); ++it)
    {
        if (it->second == parser)
            return it->first;
    }

    return NULL;
}

cbProject* NativeParser::GetProjectByFilename(const wxString& filename)
{
    cbProject* activeProject = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (activeProject)
    {
        Parser* parser = GetParserByProject(activeProject);
        if ((parser && parser->IsFileParsed(filename)) || activeProject->GetFileByFilename(filename, false, true))
            return activeProject;
        else
        {
            ProjectsArray* projArr = Manager::Get()->GetProjectManager()->GetProjects();
            for (size_t i = 0; i < projArr->GetCount(); ++i)
            {
                cbProject* project = projArr->Item(i);
                if (!project || project == activeProject)
                    continue;

                parser = GetParserByProject(project);
                if ((parser && parser->IsFileParsed(filename)) || project->GetFileByFilename(filename, false, true))
                    return project;
            }
        }
    }

    return NULL;
}

void NativeParser::SetTokenKindImage(int kind, const wxBitmap& bitmap, const wxBitmap& mask)
{
    if (kind < PARSER_IMG_MIN || kind > PARSER_IMG_MAX)
        return;
#ifdef __WXMSW__
    m_pImageList->Replace(kind, bitmap, mask);
#endif
}

void NativeParser::SetTokenKindImage(int kind, const wxBitmap& bitmap, const wxColour& maskColour)
{
    if (kind < PARSER_IMG_MIN || kind > PARSER_IMG_MAX)
        return;
    m_pImageList->Replace(kind, bitmap);//, maskColour);
}

void NativeParser::SetTokenKindImage(int kind, const wxIcon& icon)
{
    if (kind < PARSER_IMG_MIN || kind > PARSER_IMG_MAX)
        return;
    m_pImageList->Replace(kind, icon);
}

int NativeParser::GetTokenKindImage(Token* token)
{
    if (!token)
        return PARSER_IMG_NONE;

    switch (token->m_TokenKind)
    {
        case tkPreprocessor:      return PARSER_IMG_PREPROCESSOR;

        case tkEnum:
            switch (token->m_Scope)
            {
                case tsPublic:    return PARSER_IMG_ENUM_PUBLIC;
                case tsProtected: return PARSER_IMG_ENUM_PROTECTED;
                case tsPrivate:   return PARSER_IMG_ENUM_PRIVATE;
                default:          return PARSER_IMG_ENUM;
            }

        case tkEnumerator:        return PARSER_IMG_ENUMERATOR;

        case tkClass:
            switch (token->m_Scope)
            {
                case tsPublic:    return PARSER_IMG_CLASS_PUBLIC;
                case tsProtected: return PARSER_IMG_CLASS_PROTECTED;
                case tsPrivate:   return PARSER_IMG_CLASS_PRIVATE;
                default:          return PARSER_IMG_CLASS_PUBLIC;
            }

        case tkNamespace:         return PARSER_IMG_NAMESPACE;

        case tkTypedef:
            switch (token->m_Scope)
            {
                case tsPublic:    return PARSER_IMG_TYPEDEF_PUBLIC;
                case tsProtected: return PARSER_IMG_TYPEDEF_PROTECTED;
                case tsPrivate:   return PARSER_IMG_TYPEDEF_PRIVATE;
                default:          return PARSER_IMG_TYPEDEF;
            }

        case tkMacro:
            switch (token->m_Scope)
            {
                case tsPublic:    return PARSER_IMG_MACRO_PUBLIC;
                case tsProtected: return PARSER_IMG_MACRO_PROTECTED;
                case tsPrivate:   return PARSER_IMG_MACRO_PRIVATE;
                default:          return PARSER_IMG_MACRO;
            }

        case tkConstructor:
            switch (token->m_Scope)
            {
                case tsProtected: return PARSER_IMG_CTOR_PROTECTED;
                case tsPrivate:   return PARSER_IMG_CTOR_PRIVATE;
                default:          return PARSER_IMG_CTOR_PUBLIC;
            }

        case tkDestructor:
            switch (token->m_Scope)
            {
                case tsProtected: return PARSER_IMG_DTOR_PROTECTED;
                case tsPrivate:   return PARSER_IMG_DTOR_PRIVATE;
                default:          return PARSER_IMG_DTOR_PUBLIC;
            }

        case tkFunction:
            switch (token->m_Scope)
            {
                case tsProtected: return PARSER_IMG_FUNC_PROTECTED;
                case tsPrivate:   return PARSER_IMG_FUNC_PRIVATE;
                default:          return PARSER_IMG_FUNC_PUBLIC;
            }

        case tkVariable:
            switch (token->m_Scope)
            {
                case tsProtected: return PARSER_IMG_VAR_PROTECTED;
                case tsPrivate:   return PARSER_IMG_VAR_PRIVATE;
                default:          return PARSER_IMG_VAR_PUBLIC;
            }

        default:                  return PARSER_IMG_NONE;
    }
}

void NativeParser::OnProjectLoadingHook(cbProject* project, TiXmlElement* elem, bool loading)
{
    if (loading)
    {
        // Hook called when loading project file.
        wxArrayString& pdirs = GetProjectSearchDirs(project);

        TiXmlElement* CCConf = elem->FirstChildElement("code_completion");
        if (CCConf)
        {
            TiXmlElement* pathsElem = CCConf->FirstChildElement("search_path");
            while (pathsElem)
            {
                if (pathsElem->Attribute("add"))
                {
                    wxString dir = cbC2U(pathsElem->Attribute("add"));
                    if (pdirs.Index(dir) == wxNOT_FOUND)
                        pdirs.Add(dir);
                }

                pathsElem = pathsElem->NextSiblingElement("search_path");
            }
        }
    }
    else
    {
        // Hook called when saving project file.
        wxArrayString& pdirs = GetProjectSearchDirs(project);

        // since rev4332, the project keeps a copy of the <Extensions> element
        // and re-uses it when saving the project (so to avoid losing entries in it
        // if plugins that use that element are not loaded atm).
        // so, instead of blindly inserting the element, we must first check it's
        // not already there (and if it is, clear its contents)
        TiXmlElement* node = elem->FirstChildElement("code_completion");
        if (!node)
            node = elem->InsertEndChild(TiXmlElement("code_completion"))->ToElement();
        if (node)
        {
            node->Clear();
            for (size_t i = 0; i < pdirs.GetCount(); ++i)
            {
                TiXmlElement* path = node->InsertEndChild(TiXmlElement("search_path"))->ToElement();
                if (path) path->SetAttribute("add", cbU2C(pdirs[i]));
            }
        }
    }
}

void NativeParser::CreateClassBrowser()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));
    if (!m_pClassBrowser && cfg->ReadBool(_T("/use_symbols_browser"), true))
    {
        bool isFloating = cfg->ReadBool(_T("/as_floating_window"), false);

        if (!isFloating)
        {
            // make this a tab in projectmanager notebook
            m_pClassBrowser = new ClassBrowser(Manager::Get()->GetProjectManager()->GetNotebook(), this);
            Manager::Get()->GetProjectManager()->GetNotebook()->AddPage(m_pClassBrowser, _("Symbols"));
            m_pClassBrowser->UpdateSash();
        }
        else
        {
            m_pClassBrowser = new ClassBrowser(Manager::Get()->GetAppWindow(), this);

            // make this a free floating/docking window
            CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);

            evt.name = _T("SymbolsBrowser");
            evt.title = _("Symbols browser");
            evt.pWindow = m_pClassBrowser;
            evt.dockSide = CodeBlocksDockEvent::dsRight;
            evt.desiredSize.Set(200, 250);
            evt.floatingSize.Set(200, 250);
            evt.minimumSize.Set(150, 150);
            evt.shown = true;
            evt.hideable = true;
            Manager::Get()->ProcessEvent(evt);
            m_pClassBrowser->UpdateSash();
        }
        m_ClassBrowserIsFloating = isFloating;

        // Dreaded DDE-open bug related: do not touch unless for a good reason
        // TODO by Loaden ? what's bug? I test it, it's works well now.
        m_pClassBrowser->SetParser(m_pParser);
    }
}

void NativeParser::RemoveClassBrowser(bool appShutDown)
{
    if (m_pClassBrowser)
    {
        if (!m_ClassBrowserIsFloating)
        {
            int idx = Manager::Get()->GetProjectManager()->GetNotebook()->GetPageIndex(m_pClassBrowser);
            if (idx != -1)
                Manager::Get()->GetProjectManager()->GetNotebook()->RemovePage(idx);
        }
        else if (m_ClassBrowserIsFloating)
        {
            CodeBlocksDockEvent evt(cbEVT_REMOVE_DOCK_WINDOW);
            evt.pWindow = m_pClassBrowser;
            Manager::Get()->ProcessEvent(evt);
        }
        m_pClassBrowser->Destroy();
    }
    m_pClassBrowser = 0L;
}

void NativeParser::UpdateClassBrowser()
{
    if (m_pClassBrowser && m_pParser->Done() && !Manager::isappShuttingDown())
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Updating class browser..."));
        m_pClassBrowser->UpdateView();
        Manager::Get()->GetLogManager()->DebugLog(_T("Class browser updated."));
    }
}

void NativeParser::RereadParserOptions()
{
    // disabled?
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));
    if (cfg->ReadBool(_T("/use_symbols_browser"), true))
    {
        if (!m_pClassBrowser)
        {
            CreateClassBrowser();
            UpdateClassBrowser();
        }
        // change class-browser docking settings
        else if (m_ClassBrowserIsFloating != cfg->ReadBool(_T("/as_floating_window"), false))
        {
            RemoveClassBrowser();
            CreateClassBrowser();
            // force re-update
            UpdateClassBrowser();
        }
    }
    else if (!cfg->ReadBool(_T("/use_symbols_browser"), true) && m_pClassBrowser)
        RemoveClassBrowser();

    // reparse if settings changed
    ParserOptions opts = m_pParser->Options();
    m_pParser->ReadOptions();
    if (opts.followLocalIncludes != m_pParser->Options().followLocalIncludes ||
        opts.followGlobalIncludes != m_pParser->Options().followGlobalIncludes ||
        opts.wantPreprocessor != m_pParser->Options().wantPreprocessor)
    {
        // important options changed... flag for reparsing
        if (cbMessageBox(_("You changed some class parser options. Do you want to "
                        "reparse your projects now, using the new options?"),
                        _("Reparse?"),
                        wxYES_NO | wxICON_QUESTION) == wxID_YES)
        {
            ClearParsers();
            ForceReparseActiveProject();
            return;
        }
    }

    RemoveObsoleteParsers();

    if (m_pClassBrowser)
        m_pClassBrowser->UpdateView();
}

void NativeParser::SetCBViewMode(const BrowserViewMode& mode)
{
    m_pParser->ClassBrowserOptions().showInheritance = (mode == bvmInheritance) ? true : false;
    UpdateClassBrowser();
}

void NativeParser::ClearParsers()
{
    SetParser(&m_TempParser);

    for (ParserList::const_iterator it = m_ParserList.begin(); it != m_ParserList.end(); ++it)
    {
        delete it->second;
        if (!Manager::IsAppShuttingDown())
        {
            wxString log(F(_("Delete parser for project '%s'!"), it->first
                           ? it->first->GetTitle().wx_str()
                           : _T("*NONE*")));
            Manager::Get()->GetLogManager()->Log(log);
            Manager::Get()->GetLogManager()->DebugLog(log);
        }
    }

    m_ParserList.clear();
}

bool NativeParser::AddCompilerDirs(cbProject* project, Parser* parser)
{
    if (!project || !parser)
        return false;

    wxString base = project->GetBasePath();
    parser->AddIncludeDir(base); // add project's base path
    TRACE(_T("AddCompilerDirs() : Adding project base dir to parser: ") + base);

    Compiler* compiler = CompilerFactory::GetCompiler(project->GetCompilerID());

    // so we can access post-processed project's search dirs
    if (compiler)
        compiler->Init(project);

    // get project include dirs
    for (unsigned int i = 0; i < project->GetIncludeDirs().GetCount(); ++i)
    {
        wxString out = project->GetIncludeDirs()[i];
        Manager::Get()->GetMacrosManager()->ReplaceMacros(out);
        wxFileName dir(out);
        if (NormalizePath(dir, base))
        {
            parser->AddIncludeDir(dir.GetFullPath());
            TRACE(_T("AddCompilerDirs() : Adding project dir to parser: ") + dir.GetFullPath());
        }
        else
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"), out.wx_str(), base.wx_str()));
    }

    // alloc array for target's compilers and project compiler
    int nCompilers = 1 + project->GetBuildTargetsCount();
    Compiler** Compilers = new(std::nothrow) Compiler* [nCompilers];
    if (!Compilers)
        return false;

    memset(Compilers, 0, sizeof(Compiler*) * nCompilers);
    nCompilers = 0; // reset , use as insert index in the next for loop

    // get targets include dirs
    for (int i = 0; i < project->GetBuildTargetsCount(); ++i)
    {
        ProjectBuildTarget* target = project->GetBuildTarget(i);
        if (target && target->SupportsCurrentPlatform())
        {
            if (compiler)
            {
                // post-processed search dirs (from build scripts)
                for (unsigned int ti = 0; ti < compiler->GetCompilerSearchDirs(target).GetCount(); ++ti)
                {
                    wxString out = compiler->GetCompilerSearchDirs(target)[ti];
                    wxFileName dir(out);
                    if (NormalizePath(dir, base))
                    {
                        parser->AddIncludeDir(dir.GetFullPath());
                        TRACE(_T("AddCompilerDirs() : Adding compiler target dir to parser: ") + dir.GetFullPath());
                    }
                    else
                        Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"), out.wx_str(), base.wx_str()));
                }
            }

            // apply target vars
//            target->GetCustomVars().ApplyVarsToEnvironment();
            for (unsigned int ti = 0; ti < target->GetIncludeDirs().GetCount(); ++ti)
            {
                wxString out = target->GetIncludeDirs()[ti];
                Manager::Get()->GetMacrosManager()->ReplaceMacros(out);
                wxFileName dir(out);
                if (NormalizePath(dir, base))
                {
                    parser->AddIncludeDir(dir.GetFullPath());
                    TRACE(_T("AddCompilerDirs() : Adding target dir to parser: ") + dir.GetFullPath());
                }
                else
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"), out.wx_str(), base.wx_str()));
            }
            // get the compiler
            wxString CompilerIndex = target->GetCompilerID();
            Compiler* tgtCompiler = CompilerFactory::GetCompiler(CompilerIndex);
            if (tgtCompiler)
            {
                Compilers[nCompilers] = tgtCompiler;
                ++nCompilers;
            }
        } // if (target)
    } // end loop over the targets

    // add the project compiler to the array of compilers
    if (compiler)
    { // note it might be possible that this compiler is already in the list
        // no need to worry since the compiler list of the parser will filter out duplicate
        // entries in the include dir list
        Compilers[nCompilers++] = compiler;
    }

    // add compiler include dirs
    for (int idxCompiler = 0; idxCompiler < nCompilers; ++idxCompiler)
    {
        const wxArrayString& dirs = (Compilers[idxCompiler])->GetIncludeDirs();
        for (unsigned int i = 0; i < dirs.GetCount(); ++i)
        {
//            Manager::Get()->GetLogManager()->Log(mltDevDebug, "Adding %s", dirs[i].c_str());
            wxString out = dirs[i];
            Manager::Get()->GetMacrosManager()->ReplaceMacros(out);
            wxFileName dir(out);
            if (NormalizePath(dir,base))
            {
                parser->AddIncludeDir(dir.GetFullPath());
                TRACE(_T("AddCompilerDirs() : Adding compiler dir to parser: ") + dir.GetFullPath());
            }
            else
                Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"), out.wx_str(), base.wx_str()));
        }

        // find out which compiler, if gnu, do the special trick
        // to find it's internal include paths
        // but do only once per C::B session, thus cache for later calls
        wxString CompilerID = (Compilers[idxCompiler])->GetID();
        if (CompilerID == _T("gcc"))
        {
            const wxArrayString& gccDirs = GetGCCCompilerDirs(((Compilers[idxCompiler])->GetPrograms()).CPP);
            TRACE(_T("Adding %d cached gcc dirs to parser..."), gccDirs.GetCount());
            for (size_t i = 0; i < gccDirs.GetCount(); ++i)
            {
                parser->AddIncludeDir(gccDirs[i]);
                TRACE(_T("AddCompilerDirs() : Adding cached compiler dir to parser: ") + gccDirs[i]);
            }
        }
    } // end of while loop over the found compilers

    if (!nCompilers)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("No compilers found!"));
    }
    delete [] Compilers;
    return true;
}

bool NativeParser::AddDefaultCompilerDirs(Parser* parser)
{
    if (!parser)
        return false;

    const wxString compilerId = CompilerFactory::GetDefaultCompilerID();
    const Compiler* compiler = CompilerFactory::GetCompiler(compilerId);
    const wxArrayString& dirs = compiler->GetIncludeDirs();
    for (unsigned int i = 0; i < dirs.GetCount(); ++i)
    {
        wxString dir = dirs[i];
        Manager::Get()->GetMacrosManager()->ReplaceMacros(dir);
        parser->AddIncludeDir(dir);
        TRACE(_T("AddDefaultCompilerDirs() : Adding compiler dir to parser: ") + dir);
    }

    if (compilerId == _T("gcc"))
    {
        const wxArrayString& gccDirs = GetGCCCompilerDirs(compiler->GetPrograms().CPP);
        TRACE(_T("Adding %d cached gcc dirs to parser..."), gccDirs.GetCount());
        for (size_t i = 0; i < gccDirs.GetCount(); ++i)
        {
            parser->AddIncludeDir(gccDirs[i]);
            TRACE(_T("AddDefaultCompilerDirs() : Adding cached compiler dir to parser: ") + gccDirs[i]);
        }
    }
    return true;
}

bool NativeParser::AddCompilerPredefinedMacros(cbProject* project, Parser* parser)
{
    if (!parser || !parser->Options().wantPreprocessor)
        return false;

	wxString defs;
	wxString compilerId;

	if (project)
        compilerId = project->GetCompilerID();
    else
        compilerId = CompilerFactory::GetDefaultCompilerID(); // for single file parser (non project)

	// gcc
	if (compilerId == _T("gcc"))
	{
        static wxString gccDefs;
        if (gccDefs.IsEmpty())
        {
#ifdef __WXMSW__
            wxString cmd(_T("cpp -dM -E nul"));
#else
            wxString cmd(_T("cpp -dM -E /dev/null"));
#endif
            wxArrayString output;
            if (wxExecute(cmd, output, wxEXEC_SYNC | wxEXEC_NODISABLE) == -1)
                return false;

            for (size_t i = 0; i < output.Count(); ++i)
                gccDefs += output[i] + _T("\n");
        }

        defs = gccDefs;
	}

	// vc
	else if (compilerId.StartsWith(_T("msvc")))
	{
	    static wxString vcDefs;
	    if (vcDefs.IsEmpty())
        {
            Compiler* compiler = CompilerFactory::GetCompiler(compilerId);
            wxString cmd = compiler->GetMasterPath() + _T("\\bin\\") + compiler->GetPrograms().C;
            Manager::Get()->GetMacrosManager()->ReplaceMacros(cmd);

            wxArrayString output, error;
            if (wxExecute(cmd, output, error, wxEXEC_SYNC | wxEXEC_NODISABLE) == -1 || error.IsEmpty())
                return false;

            wxString str = error[0];
            wxString tmp(_T("Microsoft (R) "));
            int pos = str.Find(tmp);
            if (pos != wxNOT_FOUND)
            {
                wxString bit = str.Mid(pos + tmp.Length(), 2);
                if (bit == _T("32"))
                    defs += _T("#define _WIN32") _T("\n");
                else if (bit == _T("64"))
                    defs += _T("#define _WIN64") _T("\n");
            }

            tmp = _T("Compiler Version ");
            pos = str.Find(tmp);
            if (pos != wxNOT_FOUND)
            {
                wxString ver = str.Mid(pos + tmp.Length(), 4);
                pos = ver.Find(_T('.'));
                if (pos != wxNOT_FOUND)
                {
                    ver[pos] = ver[pos + 1];
                    ver[pos + 1] = _T('0');
                    defs += _T("#define _MSC_VER ") + ver;
                }
            }
        }

        defs = vcDefs;
	}

	TRACE(_T("Add compiler predefined preprocessor macros:\n%s"), defs.wx_str());
	parser->AddPredefinedMacros(defs, false);
	return true;
}

bool NativeParser::AddProjectDefinedMacros(cbProject* project, Parser* parser)
{
    if (!parser)
        return false;

    wxString compilerId = project->GetCompilerID();
    wxString param;
    if (compilerId == _T("gcc"))
        param = _T("-D");
    else if (compilerId.StartsWith(_T("msvc")))
        param = _T("/D");

    if (param.IsEmpty())
        return false;

    wxString defs;
    wxArrayString opts = project->GetCompilerOptions();
    ProjectBuildTarget* target = project->GetBuildTarget(project->GetActiveBuildTarget());
    if (target != NULL)
    {
        wxArrayString targetOpts = target->GetCompilerOptions();
        for (size_t i = 0; i < targetOpts.GetCount(); ++i)
            opts.Add(targetOpts[i]);
    }

    for (size_t i = 0; i < opts.GetCount(); ++i)
    {
        wxString def = opts[i];
        Manager::Get()->GetMacrosManager()->ReplaceMacros(def);
        if (!def.StartsWith(param))
            continue;

        def = def.Right(def.Length() - param.Length());
        int pos = def.Find(_T('='));
        if (pos != wxNOT_FOUND)
            def[pos] = _T(' ');

        defs += _T("#define ") + def + _T("\n");
    }

	TRACE(_T("Add project and current buildtarget defined preprocessor macros:\n%s"), defs.wx_str());
	parser->AddPredefinedMacros(defs, true);
	return true;
}

const wxArrayString& NativeParser::GetGCCCompilerDirs(const wxString &cpp_compiler)
{
    // keep the gcc compiler path's once if found accross C::B session
    // makes opening workspaces a *lot* faster by avoiding endless calls to the compiler
    static wxArrayString gcc_compiler_dirs;
    if (!gcc_compiler_dirs.IsEmpty())
        return gcc_compiler_dirs;

    // for starters , only do this for gnu compiler
    //Manager::Get()->GetLogManager()->DebugLog(_T("CompilerID ") + CompilerID);
    //
    //   windows: mingw32-g++ -v -E -x c++ nul
    //   linux  : g++ -v -E -x c++ /dev/null
    // do the trick only for c++, not needed then for C (since this is a subset of C++)


    // let's construct the command
    // use a null file handler
    // both works fine in Windows and linux

#ifdef __WXMSW__
    wxString Command = cpp_compiler + _T(" -v -E -x c++ nul");
#else
    wxString Command = cpp_compiler + _T(" -v -E -x c++ /dev/null");
#endif

    // action time  (everything shows up on the error stream
    wxArrayString Output, Errors;
    wxExecute(Command, Output, Errors, wxEXEC_NODISABLE);

    // start from "#include <...>", and the path followed
    // let's hope this does not change too quickly, otherwise we need
    // to adjust our search code (for several versions ...)
    bool start = false;
    for (size_t idxCount = 0; idxCount < Errors.GetCount(); ++idxCount)
    {
        wxString path = Errors[idxCount].Trim(true).Trim(false);
        if (!start)
        {
            if (!path.StartsWith(_T("#include <...>")))
                continue;
            path = Errors[++idxCount].Trim(true).Trim(false);
            start = true;
        }

        wxFileName fname(path, wxEmptyString);
        fname.Normalize();
        fname.SetVolume(fname.GetVolume().MakeUpper());
        if (!fname.DirExists())
            break;

        Manager::Get()->GetLogManager()->DebugLog(_T("Caching GCC dir: ") + fname.GetPath());
        gcc_compiler_dirs.Add(fname.GetPath());
    }

    return gcc_compiler_dirs;
}

wxArrayString& NativeParser::GetProjectSearchDirs(cbProject* project)
{
    ProjectSearchDirsMap::iterator it;
    it = m_ProjectSearchDirsMap.find(project);
    if (it == m_ProjectSearchDirsMap.end())
        it = m_ProjectSearchDirsMap.insert(m_ProjectSearchDirsMap.end(), std::make_pair(project, wxArrayString()));

    return it->second;
}

bool NativeParser::CreateParser(cbProject* project)
{
    if (GetParserByProject(project))
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Parser has been in existence!"));
        return false;
    }

    Parser* parser = new(std::nothrow) Parser(this);
    if (!parser)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Failed to create parser instances!"));
        return false;
    }

    m_ParserList.push_back(std::make_pair(project, parser));
    StartCompleteParsing(project, parser);

    if (m_pParser == &m_TempParser)
        SetParser(parser);

    wxString log(F(_("Create new parser for project '%s'"), project
                   ? project->GetTitle().wx_str()
                   : _T("*NONE*")));
    Manager::Get()->GetLogManager()->Log(log);
    Manager::Get()->GetLogManager()->DebugLog(log);

    RemoveObsoleteParsers();

    return true;
}

bool NativeParser::DeleteParser(cbProject* project)
{
    ParserList::iterator it = m_ParserList.begin();
    for (; it != m_ParserList.end(); ++it)
    {
        if (it->first == project)
            break;
    }

    if (it == m_ParserList.end())
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Deleted parser does not exist!"));
        return false;
    }

    if (it->second == m_pParser)
        SetParser(&m_TempParser);

    delete it->second;
    m_ParserList.erase(it);

    wxString log(F(_("Delete parser for project '%s'!"), project
                   ? project->GetTitle().wx_str()
                   : _T("*NONE*")));
    Manager::Get()->GetLogManager()->Log(log);
    Manager::Get()->GetLogManager()->DebugLog(log);

    return true;
}

bool NativeParser::SwitchParser(cbProject* project, Parser* parser)
{
    if (parser == m_pParser || GetParserByProject(project) != parser)
        return false;

    SetParser(parser);
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Switch parser to project '%s'"), project
                                                ? project->GetTitle().wx_str()
                                                : _T("*NONE*")));
    return true;
}

bool NativeParser::ReparseFile(cbProject* project, const wxString& filename)
{
    Parser* parser = GetParserByProject(project);
    if (!parser)
        return false;

    return parser->Reparse(filename);
}

bool NativeParser::AddFileToParser(cbProject* project, const wxString& filename)
{
    Parser* parser = GetParserByProject(project);
    if (!parser)
        return false;

    return parser->AddFile(filename);
}

bool NativeParser::RemoveFileFromParser(cbProject* project, const wxString& filename)
{
    Parser* parser = GetParserByProject(project);
    if (!parser)
        return false;

    return parser->RemoveFile(filename);
}

void NativeParser::StartCompleteParsing(cbProject* project, Parser* parser)
{
    if (!project)
    {
        AddCompilerPredefinedMacros(project, parser);
        return;
    }

    AddCompilerDirs(project, parser);
    AddCompilerPredefinedMacros(project, parser);
    AddProjectDefinedMacros(project, parser);

    // add per-project dirs
    wxArrayString& pdirs = GetProjectSearchDirs(project);
    wxString base = project->GetBasePath();
    for (size_t i = 0; i < pdirs.GetCount(); ++i)
    {
        wxString path = pdirs[i];
        Manager::Get()->GetMacrosManager()->ReplaceMacros(path);
        wxFileName dir(path);

        if (NormalizePath(dir, base))
            parser->AddIncludeDir(dir.GetFullPath());
        else
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"), path.wx_str(), base.wx_str()));

    }

    wxArrayString fronts;
    wxArrayString headers;
    wxArrayString sources;

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));
    const wxString def_fronts = _T("<cstddef>, <w32api.h>, <wx/defs.h>, <wx/dlimpexp.h>, <wx/toplevel.h>, ")
                                _T("\"pch.h\", \"sdk.h\", \"stdafx.h\"");
    wxString up_fronts = cfg->Read(_T("/up_front_headers"), def_fronts);
    if (!up_fronts.StartsWith(def_fronts))
    {
        wxStringTokenizer def(def_fronts, _T(","));
        wxArrayString defs;
        while (def.HasMoreTokens())
            defs.Add(def.GetNextToken().Trim(false).Trim(true));

        wxStringTokenizer user(up_fronts, _T(","));
        wxArrayString users;
        while (user.HasMoreTokens())
            users.Add(user.GetNextToken().Trim(false).Trim(true));

        up_fronts = def_fronts;
        for (size_t i = 0; i < users.GetCount(); ++i)
        {
            if (!users[i].IsEmpty() && defs.Index(users[i], false) == wxNOT_FOUND)
                up_fronts.Append(_T(", ") + users[i]);
        }

        cfg->Write(_T("/up_front_headers"), up_fronts);
    }

    wxStringTokenizer tkz(up_fronts, _T(","));

    typedef std::map<int, wxString> FrontMap;
    FrontMap frontMap;
    FrontMap frontTempMap;
    int frontCnt = 0;
    while (tkz.HasMoreTokens())
    {
        wxString token = tkz.GetNextToken().Trim(false).Trim(true);
        if (token.Len() <= 2) // "" or <>
            continue;

        if (   parser->Options().followLocalIncludes
            && token[0] == _T('"')
            && token[token.Len() - 1] == _T('"') )
        {
            frontTempMap[++frontCnt] = token.SubString(1, token.Len() - 2).Trim(false).Trim(true);
        }
        else if (   parser->Options().followLocalIncludes
                 && token[0] == _T('<')
                 && token[token.Len() - 1] == _T('>') )
        {
            token = token.SubString(1, token.Len() - 2).Trim(false).Trim(true);
            wxArrayString finds = parser->FindFileInIncludeDirs(token);
            for (size_t i = 0; i < finds.GetCount(); ++i)
                frontMap[++frontCnt] = finds[i];
        }
    }

    // parse header files first
    for (int i = 0; i < project->GetFilesCount(); ++i)
    {
        ProjectFile* pf = project->GetFile(i);
        FileType ft = FileTypeOf(pf->relativeFilename);
        if (ft == ftHeader) // parse header files
        {
            bool isUpFrontFile = false;
            for (FrontMap::iterator it = frontTempMap.begin(); it != frontTempMap.end(); ++it)
            {
                if (it->second.IsSameAs(pf->file.GetFullName(), false))
                {
                    isUpFrontFile = true;
                    frontMap[it->first] = pf->file.GetFullPath();
                    frontTempMap.erase(it);
                    break;
                }
            }

            if (!isUpFrontFile)
                headers.Add(pf->file.GetFullPath());
        }
        else if (ft == ftSource) // parse source files
        {
            sources.Add(pf->file.GetFullPath());
        }
    }

    for (FrontMap::const_iterator it = frontMap.begin(); it != frontMap.end(); ++it)
        fronts.Add(it->second);

    Manager::Get()->GetLogManager()->DebugLog(_T("Passing list of files to batch-parser."));

    // prepare parsing
    parser->PrepareParsing();

    // parse up-front files
    if (!fronts.IsEmpty())
    {
        for (size_t i = 0; i < fronts.GetCount(); ++i)
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Header to parse up-front: '%s'"), fronts[i].wx_str()));

        Manager::Get()->GetLogManager()->DebugLog(F(_T("Add up-front parsing %d file(s) for project '%s'..."),                                                        fronts.GetCount(), project->GetTitle().wx_str()));
        parser->AddBatchParse(fronts, true);
    }

    if (!headers.IsEmpty() || !sources.IsEmpty())
    {
        Manager::Get()->GetLogManager()->DebugLog(F(_T("Add batch-parsing %d file(s) for project '%s'..."),                                                        headers.GetCount() + sources.GetCount(), project->GetTitle().wx_str()));
        parser->AddBatchParse(headers);
        parser->AddBatchParse(sources);
    }

    // start parsing
    parser->StartParsing();
}

void NativeParser::ForceReparseActiveProject()
{
    cbProject* curProject = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (curProject)
    {
        DeleteParser(curProject);
        CreateParser(curProject);
    }
}

// UNUSED
bool NativeParser::LoadCachedData(cbProject* project)
{
    if (!project || !m_pParser)
        return false;

    wxFileName projectCache = project->GetFilename();
    projectCache.SetExt(_T("cbCache"));

    wxFile f(projectCache.GetFullPath(), wxFile::read);
    if (!f.IsOpened())
        return false;

    // read cache file
    Manager::Get()->GetLogManager()->DebugLog(_T("Using parser's existing cache: ") + projectCache.GetFullPath());
    bool ret = false;
    try
    {
        wxFileInputStream fs(f);
        wxBufferedInputStream fb(fs);

        ret = m_pParser->ReadFromCache(&fb);
    }
    catch (cbException& ex)
    {
        ex.ShowErrorMessage(true);
        ret = false;
    }
    catch (...)
    {
        // eat it
        wxSafeShowMessage(_("Exception thrown!"),_("ERROR"));
        ret = false;
    }
    if (!ret)
        Manager::Get()->GetLogManager()->DebugLog(_T("Error reading Cache! Re-parsing from scratch."));
    return ret;
}

// UNUSED
bool NativeParser::SaveCachedData(const wxString& projectFilename)
{
    bool result = false;

    wxFileName projectCache = projectFilename;
    projectCache.SetExt(_T("cbCache"));

    wxFile f(projectCache.GetFullPath(), wxFile::write);
    if (!f.IsOpened())
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Failed updating parser's cache: ") + projectCache.GetFullPath());
        return false;
    }

    Manager::Get()->GetLogManager()->DebugLog(_T("Updating parser's cache: ") + projectCache.GetFullPath());

    // write cache file
    wxFileOutputStream fs(f);
    {
        wxBufferedOutputStream fb(fs);
        result = m_pParser->WriteToCache(&fb);
    }
    return result;
}

bool NativeParser::ParseFunctionArguments(cbEditor* ed, int caretPos)
{
    if (!ed || !m_pParser->Done())
        return false;

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("ParseFunctionArguments() Parse function arguments"));

    TokenIdxSet proc_result;
    if (FindCurrentFunctionToken(ed, proc_result, caretPos) != 0)
    {
        for (TokenIdxSet::iterator it = proc_result.begin(); it != proc_result.end(); ++it)
        {
            Token* token = m_pParser->GetTokens()->at(*it);
            if (!token)
                continue;

            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(_T("ParseFunctionArguments() + Function match: ") + token->m_Name);

            if (!token->m_Args.IsEmpty() && !token->m_Args.Matches(_T("()")))
            {
                wxString buffer = token->m_Args;
                // Now we have something like "(int my_int, const TheClass* my_class, float f)"
                buffer.Remove(0, 1);              // remove (
                buffer.RemoveLast();              // remove )
                // Now we have                "int my_int, const TheClass* my_class, float f"
                buffer.Replace(_T(","), _T(";")); // replace commas with semi-colons
                // Now we have                "int my_int; const TheClass* my_class; float f"
                buffer << _T(';');                // aid parser ;)
                // Finally we have            "int my_int; const TheClass* my_class; float f;"
                buffer.Trim();

                if (s_DebugSmartSense)
                {
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("ParseFunctionArguments() Parsing arguments: \"%s\""), buffer.wx_str()));
                }

                if (!buffer.IsEmpty() && !m_pParser->ParseBuffer(buffer, false, false, true))
                {
                    if (s_DebugSmartSense)
                        Manager::Get()->GetLogManager()->DebugLog(_T("ParseFunctionArguments() Error parsing arguments."));
                }
            }
        }
        return true;
    }
    else
    {
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(_T("ParseFunctionArguments() Could not determine current function's namespace..."));
    }
    return false;
}

bool NativeParser::ParseLocalBlock(cbEditor* ed, int caretPos)
{
    if (!ed || !m_pParser->Done())
        return false;

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("ParseLocalBlock() Parse local block"));

    int blockStart = FindCurrentFunctionStart(ed, 0, 0, caretPos);
    if (blockStart != -1)
    {
        ++blockStart; // skip {
        int blockEnd = caretPos == -1 ? ed->GetControl()->GetCurrentPos() : caretPos;
        if (blockEnd < 0 || blockEnd > ed->GetControl()->GetLength())
        {
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(F(_T("ParseLocalBlock() ERROR blockEnd=%d and edLength=%d?!"), blockEnd, ed->GetControl()->GetLength()));
            return false;
        }

        if (blockStart >= blockEnd)
            blockStart = blockEnd;

        wxString buffer = ed->GetControl()->GetTextRange(blockStart, blockEnd);
        buffer.Trim();
        if (!buffer.IsEmpty() && !m_pParser->ParseBuffer(buffer, false, false, true))
        {
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(_T("ParseLocalBlock() ERROR parsing block:\n") + buffer);
        }
        else
        {
            if (s_DebugSmartSense)
            {
                Manager::Get()->GetLogManager()->DebugLog(F(_T("ParseLocalBlock() Block:\n%s"), buffer.wx_str()));
                Manager::Get()->GetLogManager()->DebugLog(_T("ParseLocalBlock() Local tokens:"));
                for (size_t i = 0; i < m_pParser->GetTokens()->size(); ++i)
                {
                    Token* t = m_pParser->GetTokens()->at(i);
                    if (t && t->m_IsTemp)
                    {
                       Manager::Get()->GetLogManager()->DebugLog(
                            _T("ParseLocalBlock() + ") + t->DisplayName() + _T(" parent = ") + t->GetParentName()  );
                    }

                }
            }
            return true;
        }
    }
    else
    {
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(_T("ParseLocalBlock() Could not determine current block start..."));
    }
    return false;
}

bool NativeParser::ParseUsingNamespace(cbEditor* ed, TokenIdxSet& search_scope, int caretPos)
{
    if (!ed)
        return false;

    TokensTree* tree = m_pParser->GetTokens();

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("ParseUsingNamespace() Parse file scope for \"using namespace\""));

    wxArrayString ns;
    int pos = caretPos == -1 ? ed->GetControl()->GetCurrentPos() : caretPos;
    if (pos < 0 || pos > ed->GetControl()->GetLength())
        return false;

    // Get the buffer from begin of the editor to the current caret position
    wxString buffer = ed->GetControl()->GetTextRange(0, pos);
    m_pParser->ParseBufferForUsingNamespace(buffer, ns);

    for (size_t i = 0; i < ns.GetCount(); ++i)
    {
        std::queue<ParserComponent> components;
        BreakUpComponents(ns[i], components);

        int parentIdx = -1;
        while (!components.empty())
        {
            ParserComponent pc = components.front();
            components.pop();

            int id = tree->TokenExists(pc.component, parentIdx, tkNamespace);
            if (id == -1)
            {
                parentIdx = -1;
                break;
            }
            parentIdx = id;
        }

        if (s_DebugSmartSense && parentIdx != -1)
        {
            Token* token = tree->at(parentIdx);
            Manager::Get()->GetLogManager()->DebugLog(F(_T("ParseUsingNamespace() Found %s%s"), token->GetNamespace().wx_str(), token->m_Name.wx_str()));
        }
        search_scope.insert(parentIdx);
    }

    return true;
}

// Here, we collect the "using namespace XXXX" directives
// Also, we locate the current caret in which function, then, add the function parameters to Token trie
// Also, the variables in the function body( local block ) was add to the Token trie
size_t NativeParser::MarkItemsByAI(TokenIdxSet& result, bool reallyUseAI, bool noPartialMatch, bool caseSensitive, int caretPos)
{

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("MarkItemsByAI()")));

    result.clear();

    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return 0;

    if (!m_pParser->Done())
        Manager::Get()->GetLogManager()->DebugLog(_T("C++ Parser is still parsing files..."));
    else
    {
        // remove old temporaries
        m_pParser->GetTokens()->FreeTemporaries();
        m_pParser->GetTempTokens()->Clear();

        // find "using namespace" directives in the file
        TokenIdxSet search_scope;
        ParseUsingNamespace(ed, search_scope, caretPos);

        // parse function's arguments
        ParseFunctionArguments(ed, caretPos);

        // parse current code block (from the start of function up to the cursor)
        ParseLocalBlock(ed, caretPos);

        if (!reallyUseAI)
        {
            // all tokens, no AI whatsoever
            TokensTree* tokens = m_pParser->GetTokens();
            for (size_t i = 0; i < tokens->size(); ++i)
                result.insert(i);
            return result.size();
        }
        // we have correctly collected all the tokens, so we will do the artificial intelligence search
        return AI(result, ed, wxEmptyString, noPartialMatch, caseSensitive, &search_scope, caretPos);
    }
    return 0;
}

const wxString& NativeParser::GetCodeCompletionItems()
{
    m_CCItems.Clear();

    TokenIdxSet result;
    int count = MarkItemsByAI(result); // if m_pParser == NULL, the count will be set to zero.
    if (count)
    {
        TokensTree* tokens = m_pParser->GetTokens();
        for (TokenIdxSet::iterator it = result.begin(); it != result.end(); ++it)
        {
            Token* token = tokens->at(*it);
            if (!token)
                continue;
            if (!m_CCItems.IsEmpty())
                 m_CCItems << _T(";");
            m_CCItems << token->m_Name << token->m_Args;//" " << token->m_Filename << ":" << token->m_Line;
        }
    }

    return m_CCItems;
}

// set start and end to the calltip highlight region, based on m_CallTipCommas (calculated in GetCallTips())
void NativeParser::GetCallTipHighlight(const wxString& calltip, int* start, int* end)
{
    int pos = 1; // skip opening parenthesis
    int nest = 0;
    int commas = 0;
    *start = 0;
    *end = 0;
    while (true)
    {
        wxChar c = calltip.GetChar(pos++);
        if (c == '\0')
            break;
        else if (c == '(')
            ++nest;
        else if (c == ')')
            --nest;
        else if (c == ',' && nest == 0)
        {
            ++commas;
            if (commas == m_CallTipCommas)
            {
                if (m_CallTipCommas == 0)
                {
                    *start = 1;
                    *end = pos - 1;
                    break;
                }
                else
                    *start = pos;
            }
            else if (commas == m_CallTipCommas + 1)
            {
                *end = pos; // already incremented
                break;
            }
        }
    }
    if (*end == 0)
        *end = calltip.Length() - 1;
}

// count commas in lineText (nesting parentheses)
int NativeParser::CountCommas(const wxString& lineText, int start)
{
    int commas = 0;
    int nest = 0;
    while (true)
    {
        wxChar c = lineText.GetChar(start++);
        if (c == '\0')
            break;
        else if (c == '(')
            ++nest;
        else if (c == ')')
            --nest;
        else if (c == ',' && nest == 0)
            ++commas;
    }
    return commas;
}

const wxArrayString& NativeParser::GetCallTips(int chars_per_line)
{
    m_CallTips.Clear();
    int end = 0;
    int commas = 0;
    wxString lineText = _T("");
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    do
    {
        if (!ed || !m_pParser->Done())
            break;

        int line = ed->GetControl()->GetCurrentLine();
        lineText = ed->GetControl()->GetLine(line);
        end = ed->GetControl()->GetCurrentPos() - ed->GetControl()->PositionFromLine(line);
        int nest = 0;
        while (end > 0)
        {
            --end;
            if (lineText.GetChar(end) == ')')
                --nest;
            else if (lineText.GetChar(end) == '(')
            {
                ++nest;
                if (nest > 0)
                {
                    // count commas (nesting parentheses again) to see how far we 're in arguments
                    commas = CountCommas(lineText, end + 1);
                    break;
                }
            }
        }
        if (!end)
            break;
        lineText.Remove(end).Trim(false);
        if (lineText.StartsWith(_T("::")))
            lineText = lineText.Right(lineText.Length() - 2);
//        Manager::Get()->GetLogManager()->DebugLog(_T("Sending \"%s\" for call-tip"), lineText.c_str());

        TokensTree* tokens = m_pParser->GetTokens();
        tokens->FreeTemporaries();

        TokenIdxSet search_scope;
        ParseUsingNamespace(ed, search_scope);
        ParseFunctionArguments(ed);
        ParseLocalBlock(ed);

        m_GettingCalltips = true;

        Token* tk = tokens->at(tokens->TokenExists(lineText, -1, tkPreprocessor));
        if (tk != NULL)
            lineText = tk->m_Type;

        TokenIdxSet result;
        if (!AI(result, ed, lineText, true, true, &search_scope))
            break;

        for (TokenIdxSet::iterator it = result.begin(); it != result.end(); ++it)
        {
            Token* token = tokens->at(*it);
            if (!token)
                continue;
            if (token->m_Args != _T("()"))
            {
                wxString s;
                BreakUpInLines(s, token->m_Args, chars_per_line);
                m_CallTips.Add(s);
            }
            else if (token->m_TokenKind == tkTypedef && token->m_ActualType.Contains(_T("(")))
                m_CallTips.Add(token->m_ActualType); // typedef'd function pointer
        }
    } while (false);

    m_GettingCalltips = false;
    m_CallTipCommas = commas;

    TRACE(_T("GetCallTips() : m_CallTipCommas=%d"), m_CallTipCommas);

    return m_CallTips;
}

// helper funcs

void NativeParser::BreakUpInLines(wxString& str, const wxString& original_str, int chars_per_line)
{
    if (chars_per_line == -1 || original_str.Length() <= (size_t)chars_per_line)
    {
        str = original_str;
        return;
    }

    // break it up in lines
    size_t pos = 0;
    size_t copy_start = 0;
    int last_comma = -1;
    int last_space = -1;
    while (pos < original_str.Length())
    {
        wxChar c = original_str.GetChar(pos);

        if      (c == _T(','))
            last_comma = pos;
        else if (   (c == _T(' '))
                 || (c == _T('\t')) )
            last_space = pos;

        if (pos % chars_per_line == 0 && last_comma != -1)
        {
            str << original_str.Mid(copy_start, last_comma - copy_start + 1);
            str << _T('\n');
            copy_start = last_comma + 1;
        }
        else if (pos == original_str.Length() - 1)
            str << original_str.Mid(copy_start); // rest of the string
        ++pos;
    }
}

// convenient static funcs for fast access and improved readability

static bool InsideToken(int startAt, const wxString& line)
{
    return (   (startAt >= 0)
            && ((size_t)startAt < line.Len())
            && (   (wxIsalnum(line.GetChar(startAt)))
                || (line.GetChar(startAt) == '_') ) );
}
static int BeginOfToken(int startAt, const wxString& line)
{
    while (   (startAt >= 0)
           && ((size_t)startAt < line.Len())
           && (   (wxIsalnum(line.GetChar(startAt)))
               || (line.GetChar(startAt) == '_') ) )
        --startAt;
    return startAt;
}
static int BeforeToken(int startAt, const wxString& line)
{
    if (   (startAt > 0)
        && ((size_t)startAt < line.Len() + 1)
        && (   (wxIsalnum(line.GetChar(startAt - 1)))
            || (line.GetChar(startAt - 1) == '_') ) )
        --startAt;
    return startAt;
}
static bool IsOperatorEnd(int startAt, const wxString& line)
{
    return (   (startAt > 0)
            && ((size_t)startAt < line.Len())
            && (   (   (line.GetChar(startAt) == '>')
                    && (line.GetChar(startAt - 1) == '-') )
                || (   (line.GetChar(startAt) == ':')
                    && (line.GetChar(startAt - 1) == ':') ) ) );
}
static bool IsOperatorBegin(int startAt, const wxString& line)
{
    return (   (startAt >= 0)
            && ((size_t)startAt < line.Len())
            && (   (   (line.GetChar(startAt ) == '-')
                    && (line.GetChar(startAt + 1) == '>') )
                || (   (line.GetChar(startAt) == ':')
                    && (line.GetChar(startAt + 1) == ':') ) ) );
}
static bool IsOperatorDot(int startAt, const wxString& line)
{
    return (   (startAt >= 0)
            && ((size_t)startAt < line.Len())
            && (line.GetChar(startAt) == '.') );
}
static int BeforeWhitespace(int startAt, const wxString& line)
{
    while (   (startAt >= 0)
           && ((size_t)startAt < line.Len())
           && (   (line.GetChar(startAt) == ' ')
               || (line.GetChar(startAt) == '\t') ) )
        --startAt;
    return startAt;
}
static int AfterWhitespace(int startAt, const wxString& line)
{
    if (startAt < 0)
        startAt = 0;
    while (   ((size_t)startAt < line.Len())
           && (   (line.GetChar(startAt) == ' ')
               || (line.GetChar(startAt) == '\t') ) )
        ++startAt;
    return startAt;
}
static bool IsOpeningBracket(int startAt, const wxString& line)
{
    return (   ((size_t)startAt < line.Len())
            && (   (line.GetChar(startAt) == '(')
                || (line.GetChar(startAt) == '[') ) );
}
static bool IsClosingBracket(int startAt, const wxString& line)
{
    return (   (startAt >= 0)
            && (   (line.GetChar(startAt) == ')')
                || (line.GetChar(startAt) == ']') ) );
}

unsigned int NativeParser::FindCCTokenStart(const wxString& line)
{
    // Careful: startAt can become negative, so it's defined as integer here!
    int startAt = line.Len() - 1;
    int nest    = 0;

    bool repeat = true;
    while (repeat)
    {
        repeat = false;
        // Go back to the beginning of the function/variable (token)
        startAt = BeginOfToken(startAt, line);

        // Check for [Class]. ('.' pressed)
        if (IsOperatorDot(startAt, line))
        {
            --startAt;
            repeat = true; // yes -> repeat.
        }
        // Check for [Class]-> ('>' pressed)
        // Check for [Class]:: (':' pressed)
        else if (IsOperatorEnd(startAt, line))
        {
            startAt -= 2;
            repeat = true; // yes -> repeat.
        }

        if (repeat)
        {
            // now we're just before the "." or "->" or "::"
            // skip any whitespace
            startAt = BeforeWhitespace(startAt, line);

            // check for function/array/cast ()
            if (IsClosingBracket(startAt, line))
            {
                ++nest;
                while (   (--startAt >= 0)
                       && (nest != 0) )
                {
                    #if wxCHECK_VERSION(2, 9, 0)
                    switch (line.GetChar(startAt).GetValue())
                    #else
                    switch (line.GetChar(startAt))
                    #endif
                    {
                        case ']':
                        case ')': ++nest; --startAt; break;

                        case '[':
                        case '(': --nest; --startAt; break;

                    }

                    startAt = BeforeWhitespace(startAt, line);

                    if (IsClosingBracket(startAt, line))
                        ++nest;
                }

                startAt = BeforeToken(startAt, line);
            }
        }
    }
    ++startAt;

    startAt = AfterWhitespace(startAt, line);


    TRACE(_T("FindCCTokenStart() : Starting at %d \"%s\""), startAt, line.Mid(startAt).c_str());

    return startAt;
}

wxString NativeParser::GetNextCCToken(const wxString& line, unsigned int& startAt, bool& is_function)
{
    wxString res;
    int nest = 0;

    if (   (startAt < line.Len())
        && (line.GetChar(startAt) == '(') )
    {
        while (   (startAt < line.Len())
               && (   (line.GetChar(startAt) == '*')
                   || (line.GetChar(startAt) == '&')
                   || (line.GetChar(startAt) == '(') ) )
        {
            if (line.GetChar(startAt) == '(')
                ++nest;
            ++startAt;
        }
    }

    TRACE(_T("GetNextCCToken() : at %d (%c): res=%s"), startAt, line.GetChar(startAt), res.c_str());

    while (InsideToken(startAt, line))
    {
        res << line.GetChar(startAt);
        ++startAt;
    }
    while (   (nest > 0)
           && (startAt < line.Len()) )
    {
        if (line.GetChar(startAt) == ')')
            --nest;
        ++startAt;
    }

    TRACE(_T("GetNextCCToken() : Done nest: at %d (%c): res=%s"), startAt, line.GetChar(startAt), res.c_str());

    startAt = AfterWhitespace(startAt, line);
    if (IsOpeningBracket(startAt, line))
    {
        is_function = line.GetChar(startAt) == '(';

        ++nest;
        while (   (startAt < line.Len()-1)
               && (nest != 0) )
        {
            ++startAt;
            #if wxCHECK_VERSION(2, 9, 0)
            switch (line.GetChar(startAt).GetValue())
            #else
            switch (line.GetChar(startAt))
            #endif
            {
                case ']':
                case ')': --nest; ++startAt; break;

                case '[':
                case '(': ++nest; ++startAt; break;
            }

            startAt = AfterWhitespace(startAt, line);

            if (IsOpeningBracket(startAt, line))
                ++nest;
        }
    }
    if (IsOperatorBegin(startAt, line))
        ++startAt;

    TRACE(_T("GetNextCCToken() : Return at %d (%c): res=%s"), startAt, line.GetChar(startAt), res.c_str());

    return res;
}

wxString NativeParser::GetCCToken(wxString& line, ParserTokenType& tokenType)
{
    // line contains a string on the following form:
    // "    char* mychar = SomeNamespace::m_SomeVar.SomeMeth"
    // first we locate the first non-space char starting from the *end*:
    //
    // "    char* mychar = SomeNamespace::m_SomeVar.SomeMeth"
    //                     ^
    // then we remove everything before it.
    // after it, what we do here, is (by this example) return "SomeNamespace"
    // *and* modify line to become:
    // m_SomeVar.SomeMeth
    // so that if we 're called again with the (modified) line,
    // we 'll return "m_SomeVar" and modify line (again) to become:
    // SomeMeth
    // and so on and so forth until we return an empty string...
    // NOTE: if we find () args or [] arrays in our way, we skip them (done in GetNextCCToken)...

    tokenType = pttSearchText;
    if (line.IsEmpty())
        return wxEmptyString;

    bool is_function = false;
    unsigned int startAt = FindCCTokenStart(line);
    wxString res = GetNextCCToken(line, startAt, is_function);

    TRACE(_T("GetCCToken() : FindCCTokenStart returned %d \"%s\""), startAt, line.c_str());
    TRACE(_T("GetCCToken() : GetNextCCToken returned %d \"%s\""), startAt, res.c_str());


    if (startAt == line.Len())
        line.Clear();
    else
    {
        // skip whitespace
        startAt = AfterWhitespace(startAt, line);

        // Check for [Class]. ('.' pressed)
        if (IsOperatorDot(startAt, line))
        {
            tokenType = pttClass;
            line.Remove(0, startAt + 1);
        }
        // Check for [Class]-> ('>' pressed)
        // Check for [Class]:: (':' pressed)
        else if (IsOperatorEnd(startAt, line))
        {
            if (line.GetChar(startAt) == ':')
                tokenType = pttNamespace;
            else
                tokenType = pttClass;
            line.Remove(0, startAt + 1);
        }
        else
            line.Clear();
    }

    TRACE(_T("GetCCToken() : Left \"%s\""), line.c_str());

    if (is_function)
        tokenType = pttFunction;
    return res;
}

// Start an Artificial Intelligence (!) sequence to gather all the matching tokens..
// The actual AI is in FindAIMatches() below...
size_t NativeParser::AI(TokenIdxSet& result,
                        cbEditor* editor,
                        const wxString& lineText,
                        bool noPartialMatch,
                        bool caseSensitive,
                        TokenIdxSet* search_scope,
                        int caretPos)
{
    m_LastAISearchWasGlobal = false;
    m_LastAIGlobalSearch.Clear();

    int pos = caretPos == -1 ? editor->GetControl()->GetCurrentPos() : caretPos;
    if (pos < 0 || pos > editor->GetControl()->GetLength())
        return 0;

    m_EditorStartWord = editor->GetControl()->WordStartPosition(pos, true);
    m_EditorEndWord   = pos; //editor->GetControl()->WordEndPosition(pos, true);
    int line = editor->GetControl()->LineFromPosition(pos);

    wxString searchtext;

    TokensTree* tree = m_pParser->GetTokens();
    if (!tree)
        return 0;

    // Get the actual search text, such as "objA.m_aaa.m_bbb"
    wxString actual_search;
    int col;
    wxString tabwidth;
    tabwidth.Pad(editor->GetControl()->GetTabWidth(), ' ');
    if (lineText.IsEmpty())
    {
        actual_search = editor->GetControl()->GetLine(line);
        col = editor->GetControl()->GetColumn(pos);
        // replace tabs in line by equal-count spaces because col is in spaces!
        actual_search.Replace(_T("\t"), tabwidth);
        actual_search.Remove(col);
        actual_search.Trim();
    }
    else
    {
        actual_search = lineText;
        col = actual_search.Length() - 1;
    }

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("AI() AI enter, actual_search: \"%s\""), actual_search.wx_str()));


    static ProjectFile* cached_project           = 0;
    static cbEditor*    cached_editor            = 0;
    static int          cached_editor_start_word = 0;
    static wxString     cached_search;
    static size_t       cached_results_count     = 0;

    // early-out opportunity
    // if the user starts typing a token that in our last search had 0 results,
    // and we see that he's continuing typing for that same token,
    // don't even bother to search
    if (   cached_project           == editor->GetProjectFile()
        && cached_editor            == editor
        && cached_editor_start_word == m_EditorStartWord
        && cached_results_count     == 0
        && actual_search.StartsWith(cached_search) )
    {
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(_T("AI() AI leave, Aborting search: last attempt returned 0 results"));

        // but set m_LastAISearchWasGlobal correctly so C++ keywords can be shown
        std::queue<ParserComponent> components;
        BreakUpComponents(actual_search, components);
        m_LastAISearchWasGlobal = components.size() <= 1;
        if (!components.empty())
            m_LastAIGlobalSearch = components.front().component;
        return 0;
    }

    // Do the whole job here
    if (s_DebugSmartSense)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("AI() ========================================================="));
        Manager::Get()->GetLogManager()->DebugLog(F(_T("AI() Doing AI for '%s':"), actual_search.wx_str()));
    }

    // find current function's namespace so we can include local scope's tokens
    // we ' ll get the function's token (all matches) and add its parent namespace
    TokenIdxSet scope_result;
    TokenIdxSet proc_result;
    if (FindCurrentFunctionToken(editor, proc_result) != 0)
    {
        for (TokenIdxSet::iterator it = proc_result.begin(); it != proc_result.end(); ++it)
        {
            Token* token = m_pParser->GetTokens()->at(*it);
            if (!token)
                continue;
            scope_result.insert(token->m_ParentIndex);

            if (s_DebugSmartSense)
            {
                Token* parent = m_pParser->GetTokens()->at(token->m_ParentIndex);
                Manager::Get()->GetLogManager()->DebugLog(_T("AI() Adding search namespace: ") + (parent ? parent->m_Name : _T("Global namespace")));
            }
        }
    }

    // add additional search scopes???
    // for example, we are here:
    /*  void ClassA::FunctionB(int paraC){
            m_aaa
    */
    // then, ClassA should be added as a search_scope, the global scope should be added too.

    // if search_scope is already defined, then, add scope_result to search_scope
    // otherwise we just set search_scope as scope_result
    if (!search_scope)
        search_scope = &scope_result;
    else
    {
        // add scopes
        for (TokenIdxSet::iterator it = scope_result.begin(); it != scope_result.end(); ++it)
            search_scope->insert(*it);
    }

    // remove non-namespace/class tokens
    TokenIdxSet::iterator it = search_scope->begin();
    while (it != search_scope->end())
    {
        Token* token = tree->at(*it);
        if (!token || !(token->m_TokenKind & (tkNamespace | tkClass | tkTypedef)))
        {
            TokenIdxSet::iterator it2 = it;
            ++it;
            search_scope->erase(it2);
        }
        else
            ++it;
    }

    //alwayser search the global scope.
    search_scope->insert(-1);

    // find all other matches
    std::queue<ParserComponent> components;
    BreakUpComponents(actual_search, components);

    m_LastAISearchWasGlobal = components.size() <= 1;
    if (!components.empty())
        m_LastAIGlobalSearch = components.front().component;

    ResolveExpression(components, *search_scope, result, caseSensitive, noPartialMatch);

    cached_editor = editor;
    if (result.size() || (m_EditorEndWord - m_EditorStartWord))
        cached_editor_start_word = m_EditorStartWord;

    cached_search = actual_search;
    cached_results_count = result.size();

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("AI() AI leave, returned %d results"),result.size()));

    return result.size();
}

// Breaks up the phrase for code-completion.
// Suppose the user has invoked code-completion in this piece of code:
//
//   Ogre::Root::getSingleton().|
//
// This function will break this up into an std::queue (FIFO) containing
// the following items (top is first-out):
//
// Ogre             [pttNamespace]
// Root             [pttClass]
// getSingleton     [pttFunction]
// (empty space)    [pttSearchText]
//
// It also classifies each component as a pttClass, pttNamespace, pttFunction, pttSearchText
size_t NativeParser::BreakUpComponents(const wxString& actual, std::queue<ParserComponent>& components)
{
    ParserTokenType tokenType;
    wxString tmp = actual;

    // break up components of phrase
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("BreakUpComponents() Breaking up '%s'"), tmp.wx_str()));

    while (true)
    {
        wxString tok = GetCCToken(tmp, tokenType);

        ParserComponent pc;
        pc.component = tok;
        pc.token_type = tokenType;

        // Debug smart sense: output the component's name and type.
        if (s_DebugSmartSense)
        {
            wxString tokenTypeString;
            switch (tokenType)
            {
                case (pttFunction):
                {   tokenTypeString = _T("Function");   break; }
                case (pttClass):
                {   tokenTypeString = _T("Class");      break; }
                case (pttNamespace):
                {   tokenTypeString = _T("Namespace");  break; }
                case (pttSearchText):
                {   tokenTypeString = _T("SearchText"); break; }
                default:
                {   tokenTypeString = _T("Undefined");         }
            }
            Manager::Get()->GetLogManager()->DebugLog(F(_T("BreakUpComponents() Found component: '%s' (%s)"), tok.wx_str(), tokenTypeString.wx_str()));
        }

        // Support global namespace like ::MessageBoxA
        // Break up into "", type is pttNameSpace and "MessageBoxA", type is pttSearchText.
        // for pttNameSpace  type, if its text (tok) is empty -> ignore this component.
        // for pttSearchText type, don't do this because for ss:: we need this, too.
        if (!tok.IsEmpty() || (tokenType == pttSearchText && components.size() != 0))
        {
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(F(_T("BreakUpComponents() Adding component: '%s'."), tok.wx_str()));
            components.push(pc);
        }

        if (tokenType == pttSearchText)
            break;
    }

    return 0;
}

// Here's the meat of code-completion :)
// This function decides most of what gets included in the auto-completion list
// presented to the user.
// It's called recursively for each component of the std::queue argument.
// for example: objA.objB.function()
// components is a queue of:  'objA'  'objB' 'function'. we deal with objA firstly.
size_t NativeParser::FindAIMatches(std::queue<ParserComponent> components,
                                   TokenIdxSet& result,
                                   int parentTokenIdx,
                                   bool noPartialMatch,
                                   bool caseSensitive,
                                   bool use_inheritance,
                                   short int kindMask,
                                   TokenIdxSet* search_scope)
{
    if (components.empty())
        return 0;

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("FindAIMatches() ----- FindAIMatches - enter -----"));

    TokensTree* tree = m_pParser->GetTokens();

    // pop top component
    ParserComponent parser_component = components.front();
    components.pop();

    // handle the special keyword "this".
    if ((parentTokenIdx != -1) && (parser_component.component == _T("this")))
    {
        // this will make the AI behave like it's the previous scope (or the current if no previous scope)

        // move on please, nothing to see here...
        return FindAIMatches(components, result, parentTokenIdx, noPartialMatch, caseSensitive, use_inheritance, kindMask, search_scope);
    }

    // we 'll only add tokens in the result set if we get matches for the last token
    bool isLastComponent = components.empty();
    wxString searchtext = parser_component.component;

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("FindAIMatches() Search for %s, isLast = %d"),
                                                    searchtext.wx_str(),
                                                    isLastComponent?1:0));

    // get a set of matches for the current token
    TokenIdxSet local_result;
    GenerateResultSet(tree, searchtext, parentTokenIdx, local_result,
                      (caseSensitive || !isLastComponent),
                      (isLastComponent && !noPartialMatch), kindMask);

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("FindAIMatches() Looping %d results"), local_result.size()));

    // loop all matches, and recurse
    for (TokenIdxSet::iterator it = local_result.begin(); it != local_result.end(); it++)
    {
        int id = *it;
        Token* token = tree->at(id);

        // sanity check
        if (!token)
        {
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(_T("FindAIMatches() Token is NULL?!"));
            continue;
        }

        // ignore operators
        if (token->m_IsOperator)
            continue;

        // enums children (enumerators), are added by default
        if (token->m_TokenKind == tkEnum)
        {
            // insert enum type
            result.insert(id);

            // insert enumerators
            for (TokenIdxSet::iterator it2 = token->m_Children.begin(); it2 != token->m_Children.end(); it2++)
                result.insert(*it2);

            continue; // done with this token
        }

        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(F(_T("FindAIMatches() Match: '%s' (ID='%d') : type='%s'"), token->m_Name.wx_str(), id, token->m_ActualType.wx_str()));


        // is the token a function or variable (i.e. is not a type)
        if (    !searchtext.IsEmpty()
             && (parser_component.token_type != pttSearchText)
             /* || m_GettingCalltips */ // DISABLED! (crash in some cases) this allows calltips for typedef'd function pointers
             && !token->m_ActualType.IsEmpty() )
        {
            // the token is not a type
            // find its type's ID and use this as parent instead of (*it)
            TokenIdxSet type_result;
            std::queue<ParserComponent> type_components;
            wxString actual = token->m_ActualType;

            // TODO: ignore builtin types (void, int, etc)
            BreakUpComponents(actual, type_components);
            // the parent to search under is a bit troubling, because of namespaces
            // what we 'll do is search under current parent and traverse up the parentship
            // until we find a result, or reach -1...

            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(F(_T("FindAIMatches() Looking for type: '%s' (%d components)"), actual.wx_str(), type_components.size()));

            // search under all search-scope namespaces too
            TokenIdxSet temp_search_scope;
            if (search_scope)
                temp_search_scope = *search_scope;

            // add grand-parent as search scope (if none defined)
            // this helps with namespaces when the token's type doesn't contain
            // namespace info. In that case (with the code here) we 're searching in
            // the parent's namespace too
            if (parentTokenIdx != -1)
            {
                Token* parent = tree->at(parentTokenIdx);
                if (parent && parent->GetParentToken())
                {
                    temp_search_scope.insert(parent->GetParentToken()->GetSelf());
                    if (s_DebugSmartSense)
                        Manager::Get()->GetLogManager()->DebugLog(_T("FindAIMatches() Implicit search scope added:") + parent->GetParentToken()->m_Name);
                }
            }

            TokenIdxSet::iterator itsearch;
            itsearch = temp_search_scope.begin();
            while (!search_scope || itsearch != temp_search_scope.end())
            {
                Token* parent = tree->at(*itsearch);

                if (s_DebugSmartSense)
#if wxCHECK_VERSION(2, 9, 0)
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("FindAIMatches() Now looking under '%s'"),
                                                                parent ? parent->m_Name.wx_str() : _("Global namespace").wx_str()));
#else
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("FindAIMatches() Now looking under '%s'"),
                                                                parent ? parent->m_Name.wx_str() : _("Global namespace")));
#endif
                do
                {
                    // types are searched as whole words, case sensitive and only classes/namespaces
                    if (FindAIMatches(type_components,
                                      type_result,
                                      parent ? parent->GetSelf() : -1,
                                      true,
                                      false,
                                      false,
                                      tkClass | tkNamespace | tkTypedef | tkEnum,
                                      &temp_search_scope) != 0)
                        break;
                    if (!parent)
                        break;
                    parent = tree->at(parent->m_ParentIndex);
                } while (true);
                ++itsearch;
            }

            // we got all possible types (hopefully should be just one)
            if (!type_result.empty())
            {
                // this is the first result
                id = *(type_result.begin());
                if (type_result.size() > 1)
                {
                    // if we have more than one result, recurse for all of them
                    TokenIdxSet::iterator it = type_result.begin();
                    ++it;
                    while (it != type_result.end())
                    {
                        std::queue<ParserComponent> lcomp = components;
                        FindAIMatches(lcomp, result, *it, noPartialMatch, caseSensitive, use_inheritance, kindMask, search_scope);
                        ++it;
                    }
                }

                if (s_DebugSmartSense)
                {
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("FindAIMatches() Type: '%s' (%d)"), tree->at(id)->m_Name.wx_str(), id));
                    if (type_result.size() > 1)
                        Manager::Get()->GetLogManager()->DebugLog(F(_T("FindAIMatches() Multiple types matched for '%s': %d results"),
                                                                    token->m_ActualType.wx_str(),
                                                                    type_result.size()));
                }
            }
            else if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(F(_T("FindAIMatches() No types matched '%s'."), token->m_ActualType.wx_str()));
        }

        // if no more components, add to result set
        if (isLastComponent)
            result.insert(id);
        // else recurse this function using id as a parent
        else
            FindAIMatches(components, result, id, noPartialMatch, caseSensitive, use_inheritance, kindMask, search_scope);
    }

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("FindAIMatches() ----- FindAIMatches - leave -----"));

    return result.size();
}

inline bool MatchText(const wxString& text, const wxString& search, bool caseSens, bool isPrefix)
{
    if (isPrefix && search.IsEmpty())
        return true;
    if (!isPrefix)
        return text.CompareTo(search, caseSens ? wxString::exact : wxString::ignoreCase) == 0;
    // isPrefix == true
    if (caseSens)
        return text.StartsWith(search);
    return text.Upper().StartsWith(search.Upper());
}

inline bool MatchType(TokenKind kind, short int kindMask)
{
    return kind & kindMask;
}

size_t NativeParser::GenerateResultSet(TokensTree*     tree,
                                       const wxString& search,
                                       int             parentIdx,
                                       TokenIdxSet&    result,
                                       bool            caseSens,
                                       bool            isPrefix,
                                       short int       kindMask)
{
    if (!tree)
        return 0;

    Token* parent = tree->at(parentIdx);
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_("GenerateResultSet() search '%s', parent='%s (id:%d, type:%s), isPrefix=%d'"),
                                                    search.wx_str(),
#if wxCHECK_VERSION(2, 9, 0)
                                                    parent ? parent->m_Name.wx_str() : _("Global namespace").wx_str(),
#else
                                                    parent ? parent->m_Name.wx_str() : _("Global namespace"),
#endif
                                                    parent ? parent->GetSelf() : 0,
                                                    parent ? parent->GetTokenKindString().wx_str():0,
                                                    isPrefix ? 1 : 0));

    // parent == null means we are searching in the global scope
    if (parent)
    {
        // we got a parent; add its children
        for (TokenIdxSet::iterator it = parent->m_Children.begin(); it != parent->m_Children.end(); ++it)
        {
            Token* token = tree->at(*it);
            if (token && MatchType(token->m_TokenKind, kindMask))
            {
                if (MatchText(token->m_Name, search, caseSens, isPrefix))
                    result.insert(*it);
                else if (token && token->m_TokenKind == tkNamespace && token->m_Aliases.size()) // handle namespace aliases
                {
                    for (size_t i = 0; i < token->m_Aliases.size(); ++i)
                    {
                        if (MatchText(token->m_Aliases[i], search, caseSens, isPrefix))
                        {
                            result.insert(*it);
                            // break; ?
                        }
                    }
                }
                else if (token && token->m_TokenKind == tkEnum) // check enumerators for match too
                    GenerateResultSet(tree, search, *it, result, caseSens, isPrefix, kindMask);
            }
        }
        // now go up the inheritance chain and add all ancestors' children too
        tree->RecalcInheritanceChain(parent);
        for (TokenIdxSet::iterator it = parent->m_Ancestors.begin(); it != parent->m_Ancestors.end(); ++it)
        {
            Token* ancestor = tree->at(*it);
            if (!ancestor)
                continue;
            for (TokenIdxSet::iterator it2 = ancestor->m_Children.begin(); it2 != ancestor->m_Children.end(); ++it2)
            {
                Token* token = tree->at(*it2);
                if (token && MatchType(token->m_TokenKind, kindMask))
                {
                    if (MatchText(token->m_Name, search, caseSens, isPrefix))
                        result.insert(*it2);
                    else if (token && token->m_TokenKind == tkNamespace && token->m_Aliases.size()) // handle namespace aliases
                    {
                        for (size_t i = 0; i < token->m_Aliases.size(); ++i)
                        {
                            if (MatchText(token->m_Aliases[i], search, caseSens, isPrefix))
                            {
                                result.insert(*it2);
                                // break; ?
                            }
                        }
                    }
                    else if (token && token->m_TokenKind == tkEnum) // check enumerators for match too
                        GenerateResultSet(tree, search, *it2, result, caseSens, isPrefix, kindMask);
                }
            }
        }
    }
    else
    {
        // all global tokens
        for (TokenList::iterator it = tree->m_Tokens.begin(); it != tree->m_Tokens.end(); ++it)
        {
            Token* token = *it;
            if (token && token->m_ParentIndex == -1)
            {
                if (token && MatchType(token->m_TokenKind, kindMask))
                {
                    if (MatchText(token->m_Name, search, caseSens, isPrefix))
                        result.insert(token->GetSelf());
                    else if (token && token->m_TokenKind == tkNamespace && token->m_Aliases.size()) // handle namespace aliases
                    {
                        for (size_t i = 0; i < token->m_Aliases.size(); ++i)
                        {
                            if (MatchText(token->m_Aliases[i], search, caseSens, isPrefix))
                            {
                                result.insert(token->GetSelf());
                                // break; ?
                            }
                        }
                    }
                    else if (token && token->m_TokenKind == tkEnum) // check enumerators for match too
                        GenerateResultSet(tree, search, token->GetSelf(), result, caseSens, isPrefix, kindMask);
                }
            }
        }
    }

    // done
    return result.size();
}

size_t NativeParser::ResolveActualType(wxString searchText, const TokenIdxSet& searchScope, TokenIdxSet& result)
{
    // break up the search text for next analysis.
    std::queue<ParserComponent> typeComponents;
    BreakUpComponents(searchText, typeComponents);
    if (!typeComponents.empty())
    {

        TokenIdxSet initialScope;
        if (!searchScope.empty())
            initialScope = searchScope;
        else
            initialScope.insert(-1);
        while (typeComponents.size() > 0)
        {
            TokenIdxSet initialResult;
            ParserComponent component = typeComponents.front();
            typeComponents.pop();
            wxString actualTypeStr = component.component;
            GenerateResultSet(actualTypeStr, initialScope, initialResult, true, false, 0xFFFF);
            if (initialResult.size() > 0)
            {
                initialScope.clear();
                for (TokenIdxSet::iterator it = initialResult.begin(); it != initialResult.end(); ++it)
                {
                    // TODO (blueshake#1#): eclimate the variable/function
                    initialScope.insert(*it);
                }
            }
            else
            {
                initialScope.clear();
                break;
            }

        }
        if (initialScope.size() > 0)
        {
            result = initialScope;
        }
    }

    return (result.size()>0 ? result.size() : 0);
}

size_t NativeParser::ResolveExpression(std::queue<ParserComponent> components, const TokenIdxSet& searchScope, TokenIdxSet& result, bool isCaseSense, bool IsPrefix)
{
    m_TemplateMap.clear();
    static ParserComponent lastComponent;
    if (components.empty())
        return 0;

    TokensTree* tree = m_pParser->GetTokens();
    if (!tree)
        return 0;

    TokenIdxSet initialScope;
    if (!searchScope.empty())
        initialScope = searchScope;
    else
        initialScope.insert(-1);

    while (components.size() > 0)
    {
        TokenIdxSet initialResult;
        ParserComponent subComponent = components.front();
        components.pop();
        bool isLastComponent = components.empty();
        wxString searchText = subComponent.component;
        if (searchText == _T("this"))
        {
            initialScope.erase(-1);
            TokenIdxSet tempInitialScope = initialScope;
            for (TokenIdxSet::iterator it=tempInitialScope.begin(); it!=tempInitialScope.end(); ++it)
            {
                Token* token = tree->at(*it);
                if (token && (token->m_TokenKind !=tkClass))
                {
                    initialScope.erase(*it);
                }
            }
            if (initialScope.size() > 0)
                continue;
            else
                break;//error happened.
        }

        if (s_DebugSmartSense)
        {
            Manager::Get()->GetLogManager()->DebugLog(F(_T("ResolveExpression() search scope is %d result."), initialScope.size()));
            for (TokenIdxSet::iterator tt=initialScope.begin(); tt != initialScope.end(); ++tt)
            Manager::Get()->GetLogManager()->DebugLog(F(_T("search scope: %d"), (*tt)));
        }

        GenerateResultSet(searchText, initialScope, initialResult, (isCaseSense || !isLastComponent), (!IsPrefix && isLastComponent));
        // now we should clear the initialScope.
        initialScope.clear();

        //-------------------------------------

        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(F(_T("ResolveExpression() Looping %d result."), initialResult.size()));

        //------------------------------------
        if (initialResult.size() > 0)
        {
            //loop all matches.
            for (TokenIdxSet::iterator it=initialResult.begin(); it!=initialResult.end(); ++it)
            {
                size_t id = (*it);
                Token* token = tree->at(id);

                if (!token)
                {
                    if (s_DebugSmartSense)
                        Manager::Get()->GetLogManager()->DebugLog(F(_T("ResolveExpression() token is NULL?!")));

                    continue;
                }

                // TODO: we should deal with operators carefully.
                // it should work for class::/namespace::
                if (token->m_IsOperator && (lastComponent.token_type!=pttNamespace))
                    continue;

                //------------------------------

                if (s_DebugSmartSense)
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("ResolvExpression() Match:'%s(ID=%d) : type='%s'"), token->m_Name.wx_str(), id, token->m_ActualType.wx_str()));

                //------------------------------
                // recond the template map message here. hope it will work.
                // wxString tkname = token->m_Name;
                // wxArrayString tks = token->m_TemplateType;
                if (!token->m_TemplateMap.empty())
                    m_TemplateMap = token->m_TemplateMap;
                // handle it if the token is a function/variable(i.e. is not a type)
                if (   !searchText.IsEmpty()
                    && (subComponent.token_type != pttSearchText)
                    && !token->m_ActualType.IsEmpty())
                {
                    TokenIdxSet autualTypeResult;
                    wxString actualTypeStr = token->m_ActualType;

                    TokenIdxSet actualTypeScope;
                    if (searchScope.empty())
                        actualTypeScope.insert(-1);
                    else
                    {
                        for (TokenIdxSet::iterator pScope=searchScope.begin(); pScope!=searchScope.end(); ++pScope)
                        {
                            actualTypeScope.insert(*pScope);
                            //we need to pScope's parent scope too.
                            if ((*pScope) != -1)
                            {
                                Token* parent = tree->at(*pScope)->GetParentToken();
                                while(true)
                                {
                                    if (!parent)
                                        break;
                                    actualTypeScope.insert(parent->GetSelf());
                                    parent = parent->GetParentToken();

                                }
                            }
                        }
                        //now add the current token's parent scope;
                        Token* currentTokenParent = token->GetParentToken();
                        while(true)
                        {
                            if (!currentTokenParent)
                                break;
                            actualTypeScope.insert(currentTokenParent->GetSelf());
                            currentTokenParent = currentTokenParent->GetParentToken();

                        }
                    }

                    //now get the tokens of variable/function.
                    TokenIdxSet actualTypeResult;
                    ResolveActualType(actualTypeStr, actualTypeScope, actualTypeResult);
                    if (actualTypeResult.size() > 0)
                    {
                        for (TokenIdxSet::iterator it2=actualTypeResult.begin(); it2!=actualTypeResult.end(); ++it2)
                        {
                            initialScope.insert(*it2);
                            //and we need to add the template argument alias too.
                            Token* typeToken = tree->at(*it2);
                            if (typeToken && typeToken->m_TokenKind == tkTypedef && !typeToken->m_TemplateAlias.IsEmpty())
                            {
                                actualTypeStr = typeToken->m_TemplateAlias;
                                map<wxString, wxString>::iterator it = m_TemplateMap.find(actualTypeStr);
                                if (it != m_TemplateMap.end())
                                {
                                    actualTypeStr = it->second;
                                    ResolveActualType(actualTypeStr, actualTypeScope, actualTypeResult);
                                    if (actualTypeResult.size() > 0)
                                    {
                                        for (TokenIdxSet::iterator it3=actualTypeResult.begin(); it3!=actualTypeResult.end(); ++it3)
                                            initialScope.insert(*it3);
                                    }
                                }
                            }

                        }
                    }
                    else // ok ,we search template container to check if type is template formal.
                    {
                        map<wxString, wxString>::iterator it = m_TemplateMap.find(actualTypeStr);
                        if (it != m_TemplateMap.end())
                        {
                            actualTypeStr = it->second;
                            ResolveActualType(actualTypeStr, actualTypeScope, actualTypeResult);
                            if (actualTypeResult.size() > 0)
                            {
                                for (TokenIdxSet::iterator it2=actualTypeResult.begin(); it2!=actualTypeResult.end(); ++it2)
                                    initialScope.insert(*it2);
                            }
                        }
                    }
                    continue;

                }

                initialScope.insert(id);
            }
        }
        else
        {
            initialScope.clear();
            break;
        }
        if (subComponent.token_type != pttSearchText)
            lastComponent = subComponent;
    }


    if (initialScope.size() > 0)
        result = initialScope;
    return (result.size()>0 ? result.size() : 0);
}

size_t NativeParser::GenerateResultSet(wxString search,
                                       const TokenIdxSet& ptrParentID,
                                       TokenIdxSet&    result,
                                       bool            caseSens,
                                       bool            isPrefix,
                                       short int       kindMask)
{
    TokensTree* tree = m_pParser->GetTokens();
    if (!tree)
        return 0;

    if (search.IsEmpty())
    {
        for (TokenIdxSet::iterator ptr = ptrParentID.begin(); ptr != ptrParentID.end(); ++ptr)
        {
            size_t parentIdx = (*ptr);
            Token* parent = m_pParser->GetTokens()->at(parentIdx);
            if (parent)
            {
                for (TokenIdxSet::iterator it = parent->m_Children.begin(); it != parent->m_Children.end(); ++it)
                {
                    Token* token = m_pParser->GetTokens()->at(*it);
                    if (token)
                        result.insert(*it);
                }
                tree->RecalcInheritanceChain(parent);
                for (TokenIdxSet::iterator it = parent->m_Ancestors.begin(); it != parent->m_Ancestors.end(); ++it)
                {
                    Token* ancestor = m_pParser->GetTokens()->at(*it);
                    if (!ancestor)
                        continue;
                    for (TokenIdxSet::iterator it2 = ancestor->m_Children.begin(); it2 != ancestor->m_Children.end(); ++it2)
                    {
                        Token* token = m_pParser->GetTokens()->at(*it2);
                        if (token)
                        {
                            result.insert(*it2);
                        }
                    }
                }
            }
        }

    }
    else
    {
        TokenIdxSet tempResult;
        // we use FindMatches to get the items from tree directly and eclimate the
        //items which are not under the search scope.
        size_t resultCount = m_pParser->FindMatches(search, tempResult, caseSens, isPrefix);
        //if (m_pParser->FindMatches(search, tempResult, caseSens, isPrefix))
        if (resultCount > 0)
        {
//            Manager::Get()->GetLogManager()->DebugLog(F(_T("Find %d result from the tree."), resultCount));
            //get the tokens under the search scope.
            for (TokenIdxSet::iterator ptr = ptrParentID.begin(); ptr != ptrParentID.end(); ++ptr)
            {
                //to make it clear, parentIdx stand for search scope.
                // (*it) stand for matched item id.
                int parentIdx = (*ptr);
                for (TokenIdxSet::iterator it = tempResult.begin(); it != tempResult.end(); ++it)
                {
                    Token* token = tree->at(*it);
                    if (token && (token->m_ParentIndex == parentIdx))
                        result.insert(*it);

                    //if the matched item id is under the search scope's ancestor scope.
                    //we need to add them too.
                    Token* tokenParent = tree->at(token->m_ParentIndex);//get the matched item's parent token.
                    if (tokenParent)
                    {
                        tree->RecalcInheritanceChain(tokenParent);

                        //match the ancestor scope,add them
                        //(*it2) should be the search scope ancestor's id(search scope)
                        for (TokenIdxSet::iterator it2=tokenParent->m_Descendants.begin(); it2!=tokenParent->m_Descendants.end(); ++it2)
                        {
                            if (parentIdx == (*it2)) //matched
                                result.insert(*it);
                        }
                        //if the search scope is global,and the token's parent token kind is tkEnum ,we add them too.
                        if (-1==parentIdx && tokenParent->m_TokenKind==tkEnum)
                            result.insert(*it);
                    }

                }

            }

        }
        else
        {
            //we need to handle namespace aliases too.I hope we can find a good to do this.
            //TODO: handle template class here.
            if (ptrParentID.count(-1))
            {
                for (TokenList::iterator it = tree->m_Tokens.begin(); it != tree->m_Tokens.end(); ++it)
                {
                    Token* token = (*it);
                    if (token && token->m_TokenKind == tkNamespace && token->m_Aliases.size())
                    {
                        for (size_t i = 0; i < token->m_Aliases.size(); ++i)
                        {
                            if (token->m_Aliases[i] == search)
                            {
                                result.insert(token->GetSelf());
                                // break; ?
                            }
                        }
                    }
                }
            }
        }
    }

    return result.size();
}

// Decides if the token belongs to its parent or one of its ancestors
bool NativeParser::BelongsToParentOrItsAncestors(TokensTree* tree, Token* token, int parentIdx, bool use_inheritance)
{
    // sanity check
    if (!tree || !token)
        return false;

    if (token->m_ParentIndex == parentIdx)
        return true; // direct child of parent (matches globals too)

    if (token->m_ParentIndex == -1)
        return false; // global

    if (!use_inheritance)
        return false;

    // no parent token? no ancestors...
    Token* parentToken = tree->at(parentIdx);
    if (!parentToken)
        return false;

    // now search up the ancestors list
    tree->RecalcInheritanceChain(parentToken);
    return parentToken->m_Ancestors.find(token->m_ParentIndex) != parentToken->m_Ancestors.end();
}

bool NativeParser::SkipWhitespaceForward(cbEditor* editor, int& pos)
{
    if (!editor)
        return false;
    wxChar ch = editor->GetControl()->GetCharAt(pos);
    int len = editor->GetControl()->GetLength() - 1;
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
    {
        while (pos < len && (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'))
        {
            ++pos;
            ch = editor->GetControl()->GetCharAt(pos);
        }
        return true;
    }
    return false;
}

bool NativeParser::SkipWhitespaceBackward(cbEditor* editor, int& pos)
{
    if (!editor)
        return false;
    wxChar ch = editor->GetControl()->GetCharAt(pos);
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
    {
        while (pos > 0 && (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'))
        {
            --pos;
            ch = editor->GetControl()->GetCharAt(pos);
        }
        return true;
    }
    return false;
}

// returns current function's position (not line) in the editor
int NativeParser::FindCurrentFunctionStart(cbEditor* editor, wxString* nameSpace, wxString* procName, int caretPos)
{
    cbStyledTextCtrl* control = editor->GetControl();
    if (!control)
    {
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(_T("FindCurrentFunctionStart() Cannot access control."));
        return -1;
    }

    static ProjectFile* s_LastProject =  0;
    static cbEditor*    s_LastEditor  =  0;
    static int          s_LastLine    = -1;
    static int          s_LastResult  = -1;
    static wxString     s_LastNS;
    static wxString     s_LastPROC;

    // cache last result for optimization
    int pos = caretPos == -1 ? control->GetCurrentPos() : caretPos;
    if ((pos < 0) || (pos > control->GetLength()))
    {
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(F(_T("FindCurrentFunctionStart() Cannot determine position. caretPos=%d, control=%d"),
                                                        caretPos, control->GetCurrentPos()));
        return -1;
    }

    int line = control->LineFromPosition(pos) + 1;
    if (   (line == s_LastLine)
        && ( (editor == s_LastEditor) && (!editor->GetModified()) )
        && (editor->GetProjectFile()==s_LastProject) )
    {
        if (nameSpace) *nameSpace = s_LastNS;
        if (procName)  *procName  = s_LastPROC;

        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(F(_T("FindCurrentFunctionStart() Cached namespace='%s', cached proc='%s' (returning %d)"),
                                                        s_LastNS.wx_str(), s_LastPROC.wx_str(), s_LastResult));

        return s_LastResult;
    }

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("FindCurrentFunctionStart() Looking for tokens in '%s'"),
                                                    editor->GetFilename().wx_str()));
    s_LastProject = editor->GetProjectFile();
    s_LastEditor  = editor;
    s_LastLine    = line;

    TokenIdxSet result;
    size_t num_results = m_pParser->FindTokensInFile(editor->GetFilename(), result, tkFunction|tkConstructor|tkDestructor);
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("FindCurrentFunctionStart() Found %d results"), num_results));

    TokensTree* tree = m_pParser->GetTokens();
    for (TokenIdxSet::iterator it = result.begin(); it != result.end(); ++it)
    {
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(_T("FindCurrentFunctionStart() (Next) Iteration..."));
        Token* token = tree->at(*it);
        if (token)
        {
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(F(_T("FindCurrentFunctionStart() Iterating: tN='%s', tF='%s', tStart=%d, tEnd=%d"),
                                                            token->DisplayName().wx_str(), token->GetFilename().wx_str(), token->m_ImplLineStart, token->m_ImplLineEnd));
            // found a matching function; check its bounds
            if (token->m_ImplLineStart <= (size_t)line && token->m_ImplLineEnd >= (size_t)line)
            {
                // got it :)
                if (s_DebugSmartSense)
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("FindCurrentFunctionStart() Current function: '%s' (at line %d)"),
                                                                token->DisplayName().wx_str(),
                                                                token->m_ImplLine));

                s_LastNS     = token->GetNamespace();
                s_LastPROC   = token->m_Name;
                s_LastResult = control->PositionFromLine(token->m_ImplLine - 1);

                // locate function's opening brace
                while (s_LastResult < control->GetTextLength())
                {
                    wxChar ch = control->GetCharAt(s_LastResult);
                    if (ch == _T('{'))
                        break;
                    else if (ch == 0)
                    {
                        if (s_DebugSmartSense)
                            Manager::Get()->GetLogManager()->DebugLog(_T("FindCurrentFunctionStart() Can't determine functions opening brace..."));
                        return -1;
                    }

                    ++s_LastResult;
                }

                if (nameSpace) *nameSpace = s_LastNS;
                if (procName)  *procName  = s_LastPROC;

                if (s_DebugSmartSense)
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("FindCurrentFunctionStart() Namespace='%s', proc='%s' (returning %d)"),
                                                                s_LastNS.wx_str(), s_LastPROC.wx_str(), s_LastResult));
                return s_LastResult;
            }
            else if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(F(_T("FindCurrentFunctionStart() Function out of bounds: tN='%s', tF='%s', tStart=%d, tEnd=%d, line=%d (size_t)line=%d"),
                                                            token->DisplayName().wx_str(), token->GetFilename().wx_str(), token->m_ImplLineStart, token->m_ImplLineEnd, line, (size_t)line));
        }
    }

    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("FindCurrentFunctionStart() Can't determine current function..."));

    s_LastResult = -1;
    return -1;
}

size_t NativeParser::FindCurrentFunctionToken(cbEditor* editor, TokenIdxSet& result, int caretPos)
{
    if (!editor || !m_pParser->Done())
        return 0;

    TokenIdxSet scope_result;
    wxString procName;
    wxString scopeName;
    FindCurrentFunctionStart(editor, &scopeName, &procName, caretPos);

    if (procName.IsEmpty())
        return 0;

    // add current scope
    if (!scopeName.IsEmpty())
    {
        // _namespace ends with double-colon (::). remove it
        scopeName.RemoveLast();
        scopeName.RemoveLast();

        // search for namespace
        std::queue<ParserComponent> ns;
        BreakUpComponents(scopeName, ns);
        FindAIMatches(ns, scope_result, -1, true, true, false, tkNamespace | tkClass | tkTypedef);
    }

    // if no scope, use global scope
    if (scope_result.empty())
        scope_result.insert(-1);

    for (TokenIdxSet::iterator it = scope_result.begin(); it != scope_result.end(); ++it)
    {
        GenerateResultSet(m_pParser->GetTokens(), procName, *it, result, true, false, tkFunction | tkConstructor | tkDestructor);
    }

    return result.size();
}

// events

void NativeParser::OnThreadStart(wxCommandEvent& event)
{
//     nothing for now
}

void NativeParser::OnThreadEnd(wxCommandEvent& event)
{
//     nothing for now
}

void NativeParser::OnParserStart(wxCommandEvent& event)
{
    Parser* parser = static_cast<Parser*>(event.GetClientData());
    cbProject* project = GetProjectByParser(parser);

    switch (static_cast<ParsingType>(event.GetInt()))
    {
    case ptCreateParser:
        Manager::Get()->GetLogManager()->DebugLog(F(_("Starting batch parsing for project '%s'..."), project
                                                    ? project->GetTitle().wx_str()
                                                    : _T("*NONE*")));
        {
            EditorBase* editor = Manager::Get()->GetEditorManager()->GetActiveEditor();
            if (!editor || editor->GetFilename() == g_StartHereTitle)
                break;

            cbProject* edPrject = GetProjectByFilename(editor->GetFilename());
            if (edPrject && edPrject != project)
            {
                Parser* parser = GetParserByProject(edPrject);
                if (!parser)
                {
                    if (CreateParser(edPrject))
                        parser = GetParserByProject(edPrject);
                }

                if (parser)
                    SetParser(parser);
            }
        }
        break;

    case ptAddFileToParser:
        break;

    case ptReparseFile:
        break;

    case ptUndefined:
        if (event.GetString().IsEmpty())
            Manager::Get()->GetLogManager()->DebugLog(F(_("Batch parsing error in project '%s'"), project
                                                        ? project->GetTitle().wx_str()
                                                        : _T("*NONE*")));
        else
            Manager::Get()->GetLogManager()->DebugLog(F(_("%s in project '%s'"), event.GetString().wx_str(), project
                                                        ? project->GetTitle().wx_str()
                                                        : _T("*NONE*")));
        return;
    }

    event.Skip();
}

void NativeParser::OnParserEnd(wxCommandEvent& event)
{
    Parser* parser = static_cast<Parser*>(event.GetClientData());
    cbProject* project = GetProjectByParser(parser);

    const ParsingType type = static_cast<ParsingType>(event.GetInt());
    switch (type)
    {
    case ptCreateParser:
        {
            wxString log(F(_("Project '%s' parsing stage done!"), project
                           ? project->GetTitle().wx_str()
                           : _T("*NONE*")));
            Manager::Get()->GetLogManager()->Log(log);
            Manager::Get()->GetLogManager()->DebugLog(log);
            CC_PROFILE_TIMER_LOG();
        }
        break;

    case ptAddFileToParser:
        Manager::Get()->GetLogManager()->DebugLog(F(_T("Add files to parser for project '%s'"), project
                                                    ? project->GetTitle().wx_str()
                                                    : _T("*NONE*")));
        break;

    case ptReparseFile:
        Manager::Get()->GetLogManager()->DebugLog(F(_T("Reparsing modified files for project '%s'"), project
                                                    ? project->GetTitle().wx_str()
                                                    : _T("*NONE*")));
        break;

    case ptUndefined:
        Manager::Get()->GetLogManager()->DebugLog(F(_T("Error parser end handle for project '%s'"), project
                                                    ? project->GetTitle().wx_str()
                                                    : _T("*NONE*")));
        return;
    }

    // also, mark all project files as local
    if (   project
        && (type == ptCreateParser || type == ptAddFileToParser) )
    {
        for (int i = 0; i < project->GetFilesCount(); ++i)
        {
            ProjectFile* pf = project->GetFile(i);
            if (!pf)
                continue;
            parser->MarkFileTokensAsLocal(pf->file.GetFullPath(), true, project);
        }
    }

    long tim = parser->LastParseTime();
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Project '%s' parsing stage done (%d total parsed files, ")
                                                _T("%d tokens in %d minute(s), %d.%03d seconds)."),
                    project ? project->GetTitle().wx_str() : _T("*NONE*"),
                    parser->GetFilesCount(),
                    parser->GetTokens()->realsize(),
                    (tim / 60000),
                    ((tim / 1000) %60),
                    tim % 1000));

    UpdateClassBrowser();

    event.Skip();
}

void NativeParser::OnEditorActivatedTimer(wxTimerEvent& event)
{
    if (m_LastActivatedFile == g_StartHereTitle || m_LastActivatedFile.IsEmpty())
    {
        SetParser(&m_TempParser);
        return;
    }

    cbProject* project = GetProjectByFilename(m_LastActivatedFile);
    const int pos = m_StandaloneFiles.Index(m_LastActivatedFile);
    if (project && pos != wxNOT_FOUND)
    {
        m_StandaloneFiles.RemoveAt(pos);
        if (m_StandaloneFiles.IsEmpty())
            DeleteParser(NULL);
        else
            RemoveFileFromParser(NULL, m_LastActivatedFile);
    }

    Parser* parser = GetParserByProject(project);
    if (!parser)
    {
        if (CreateParser(project))
        {
            parser = GetParserByProject(project);
            if (!project)
            {
                parser->AddFile(m_LastActivatedFile);
                m_StandaloneFiles.Add(m_LastActivatedFile);
            }
        }
        else
            parser = &m_TempParser;
    }
    else if (!project)
    {
        wxFileName file(m_LastActivatedFile);
        if (parser->IsFileParsed(m_LastActivatedFile))
        {
            // Need update parser include dir, for classbrowser
            wxArrayString incDirs = parser->GetIncludeDirs();
            if (!incDirs.IsEmpty() && incDirs.Item(0) != file.GetPath())
                incDirs.Item(0) = file.GetPath();
            ReparseFile(project, m_LastActivatedFile);
        }
        else
        {
            m_StandaloneFiles.Add(m_LastActivatedFile);
            parser->ClearIncludeDirs();
            parser->AddIncludeDir(file.GetPath());
            AddDefaultCompilerDirs(parser);
            AddFileToParser(NULL, m_LastActivatedFile);
        }
    }

    if (parser != m_pParser)
        SwitchParser(project, parser);

    if (m_pClassBrowser && parser->ClassBrowserOptions().displayFilter == bdfFile)
    {
        // check header and implementation file swap, if yes, don't need to rebuild browser tree
        m_pClassBrowser->UpdateView(true);
    }
}

void NativeParser::OnEditorActivated(EditorBase* editor)
{
    if (m_LastActivatedFile != editor->GetFilename())
    {
        if (m_TimerEditorActivated.IsRunning())
            m_TimerEditorActivated.Stop();

        m_LastActivatedFile = editor->GetFilename();
        m_TimerEditorActivated.Start(500, wxTIMER_ONE_SHOT);
    }
}

void NativeParser::OnEditorClosed(EditorBase* editor)
{
    if (m_TimerEditorActivated.IsRunning())
        m_TimerEditorActivated.Stop();

    wxString filename = editor->GetFilename();
    if (filename == g_StartHereTitle)
        return;

    const int pos = m_StandaloneFiles.Index(filename);
    if (pos != wxNOT_FOUND)
    {
        m_StandaloneFiles.RemoveAt(pos);
        if (m_StandaloneFiles.IsEmpty())
            DeleteParser(NULL);
        else
            RemoveFileFromParser(NULL, filename);
    }
}

size_t NativeParser::RemoveObsoleteParsers()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));
    const size_t maxParsers = cfg->ReadInt(_T("/max_parsers"), 5);
    size_t removedCount = 0;

    while (m_ParserList.size() > maxParsers)
    {
        if (DeleteParser(m_ParserList.front().first))
        {
            m_ParserList.pop_front();
            ++removedCount;
        }
        else
            break;
    }

    if (removedCount)
    {
        wxString log(F(_("Removed obsolete parser count is %d"), removedCount));
        Manager::Get()->GetLogManager()->Log(log);
        Manager::Get()->GetLogManager()->DebugLog(log);
    }

    return removedCount;
}

class ParserDirTraverser : public wxDirTraverser
{
public:
    ParserDirTraverser(const wxString& excludePath, wxArrayString& files) :
        m_ExcludeDir(excludePath),
        m_Files(files)
    {}

    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        wxString ext = filename.AfterLast(_T('.')).Lower();
        if (ext.StartsWith(_T("h")) || ext.StartsWith(_T("c")) || ext.IsEmpty())
            m_Files.Add(filename);
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        if (dirname == m_ExcludeDir)
            return wxDIR_IGNORE;
        if (m_Files.GetCount() == 1)
            return wxDIR_STOP;
        m_Files.Clear();
        return wxDIR_CONTINUE;
    }

private:
    const wxString& m_ExcludeDir;
    wxArrayString& m_Files;
};

wxArrayString NativeParser::GetAllPathsByFilename(const wxString& filename)
{
    wxArrayString dirs;
    const wxFileName fn(filename);
    const wxString filespec = fn.HasExt() ? fn.GetName() + _T(".*") : fn.GetName();

    wxDir dir(fn.GetPath());
    if (!dir.IsOpened())
        return wxArrayString();

    wxArrayString files;
    ParserDirTraverser traverser(wxEmptyString, files);
    dir.Traverse(traverser, filespec, wxDIR_FILES);
    if (files.GetCount() == 1)
    {
        cbProject* project = GetProjectByParser(m_pParser);
        if (project)
        {
            const wxString prjPath = project->GetCommonTopLevelPath();
            wxString priorityPath;
            if (fn.HasExt() && (fn.GetExt().StartsWith(_T("h")) || fn.GetExt().StartsWith(_T("c"))))
            {
                wxFileName priFn(prjPath);
                priFn.AppendDir(fn.GetExt().StartsWith(_T("h")) ? _T("sdk") : _T("include"));
                if (priFn.DirExists())
                {
                    priorityPath = priFn.GetFullPath();
                    wxDir priorityDir(priorityPath);
                    if (priorityDir.IsOpened())
                    {
                        wxArrayString others;
                        ParserDirTraverser traverser(wxEmptyString, others);
                        priorityDir.Traverse(traverser, filespec, wxDIR_FILES | wxDIR_DIRS);
                        if (others.GetCount() == 1)
                            AddPaths(dirs, others[0], fn.HasExt());
                    }
                }
            }

            if (dirs.IsEmpty())
            {
                wxDir prjDir(prjPath);
                if (prjDir.IsOpened())
                {
                    wxArrayString others;
                    ParserDirTraverser traverser(priorityPath, others);
                    prjDir.Traverse(traverser, filespec, wxDIR_FILES | wxDIR_DIRS);
                    if (others.GetCount() == 1)
                        AddPaths(dirs, others[0], fn.HasExt());
                }
            }
        }
    }

    if (!files.IsEmpty())
        AddPaths(dirs, files[0], fn.HasExt());

    return dirs;
}


void NativeParser::AddPaths(wxArrayString& dirs, const wxString& path, bool hasExt)
{
    wxString s;
    if (hasExt)
        s = UnixFilename(path.BeforeLast(_T('.'))) + _T(".");
    else
        s = UnixFilename(path);

    if (dirs.Index(s, false) == wxNOT_FOUND)
        dirs.Add(s);
}
