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
#include "classbrowser.h"
#include "parser/parser.h"
#include <compilerfactory.h>
#include <projectloader_hooks.h>
#include <wx/regex.h>
#include <wx/log.h> // for wxSafeShowMessage()
#include <tinyxml/tinyxml.h>
#include "wx/wxFlatNotebook/wxFlatNotebook.h"
#include "cbstyledtextctrl.h"


#include <wx/wfstream.h>

#include <cctype>

bool s_DebugSmartSense = false;

BEGIN_EVENT_TABLE(NativeParser, wxEvtHandler)
//    EVT_MENU(THREAD_START, NativeParser::OnThreadStart)
//    EVT_MENU(THREAD_END, NativeParser::OnThreadEnd)
    EVT_MENU(PARSER_END, NativeParser::OnParserEnd)
END_EVENT_TABLE()

NativeParser::NativeParser()
    : m_Parser(this),
    m_EditorStartWord(-1),
    m_EditorEndWord(-1),
    m_CallTipCommas(0),
    m_pClassBrowser(0),
    m_GettingCalltips(false),
    m_ClassBrowserIsFloating(false),
    m_LastAISearchWasGlobal(false)
{
    //ctor

    // hook to project loading procedure
    ProjectLoaderHooks::HookFunctorBase* myhook = new ProjectLoaderHooks::HookFunctor<NativeParser>(this, &NativeParser::OnProjectLoadingHook);
    m_HookId = ProjectLoaderHooks::RegisterHook(myhook);
}

NativeParser::~NativeParser()
{
    ProjectLoaderHooks::UnregisterHook(m_HookId, true);

    RemoveClassBrowser();
    ClearParsers();
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
		node->Clear();
        for (size_t i = 0; i < pdirs.GetCount(); ++i)
        {
            TiXmlElement* path = node->InsertEndChild(TiXmlElement("search_path"))->ToElement();
            path->SetAttribute("add", cbU2C(pdirs[i]));
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
        }
        m_ClassBrowserIsFloating = isFloating;

		// Dreaded DDE-open bug related: do not touch unless for a good reason
//        m_pClassBrowser->SetParser(&m_Parser);
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
    if (m_pClassBrowser && m_Parser.Done() && !Manager::isappShuttingDown())
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Updating class browser..."));
//            m_pClassBrowser->SetParser(&m_Parser);
            m_pClassBrowser->UpdateView();
        Manager::Get()->GetLogManager()->DebugLog(_T("Class browser updated."));
    }
}

void NativeParser::RereadParserOptions()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));
    bool needsReparsing = false;

    ParserOptions opts = m_Parser.Options();
    m_Parser.ReadOptions();

    // disabled?
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
    if (opts.followLocalIncludes != m_Parser.Options().followLocalIncludes ||
        opts.followGlobalIncludes != m_Parser.Options().followGlobalIncludes ||
        opts.wantPreprocessor != m_Parser.Options().wantPreprocessor)
    {
        // important options changed... flag for reparsing
        needsReparsing = true;
    }

    if (needsReparsing && m_Parser.GetTokens()->size() > 0)
    {
        if (cbMessageBox(_("You changed some class parser options. Do you want to "
                        "reparse your projects now, using the new options?"),
                        _("Reparse?"),
                        wxYES_NO | wxICON_QUESTION) == wxID_YES)
        {
            ClearParsers();
            ProjectsArray* projects = Manager::Get()->GetProjectManager()->GetProjects();
            for (unsigned int i = 0; i < projects->GetCount(); ++i)
            {
                AddParser(projects->Item(i));
            }
            if (m_pClassBrowser)
                m_pClassBrowser->SetParser(&m_Parser);//m_Parsers[proj]);
        }
    }
    if (m_pClassBrowser)
        m_pClassBrowser->UpdateView();
}

void NativeParser::SetClassBrowserProject(cbProject* project)
{
    if (m_pClassBrowser)
        m_pClassBrowser->SetParser(&m_Parser);//m_Parsers[project]);
}

void NativeParser::SetCBViewMode(const BrowserViewMode& mode)
{
    m_Parser.ClassBrowserOptions().showInheritance = mode == bvmInheritance;
    UpdateClassBrowser();
}

void NativeParser::ClearParsers()
{
    if (m_pClassBrowser)
    {
        m_pClassBrowser->SetParser(0L);
        m_pClassBrowser->UpdateView();
    }
//    ProjectsArray* projects = Manager::Get()->GetProjectManager()->GetProjects();
//    for (size_t i = 0; i < projects->GetCount(); ++i)
//    {
//        cbProject* prj = projects->Item(i);
//        RemoveParser(prj, false);
//    }
    m_Parser.Clear();
}

void NativeParser::AddCompilerDirs(Parser* parser, cbProject* project)
{
    if (!parser)
        return;

    // do not clean include dirs: we use a single parser for the whole workspace
//    parser->ClearIncludeDirs();
    wxString base = project->GetBasePath();
    parser->AddIncludeDir(base); // add project's base path

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
        if(NormalizePath(dir,base))
        {
            parser->AddIncludeDir(dir.GetFullPath());
//            Manager::Get()->GetLogManager()->DebugLog(_T("Parser prj dir: ") + dir.GetFullPath());
        }
        else
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"),out.c_str(),base.c_str()));

    }

    // alloc array for target's compilers and project compiler
    int nCompilers = 1 + project->GetBuildTargetsCount();
    Compiler** Compilers = new Compiler* [nCompilers];
    memset(Compilers, 0, sizeof(Compiler*) * nCompilers);
    nCompilers = 0; // reset , use as insert index in the next for loop

    // get targets include dirs
    for (int i = 0; i < project->GetBuildTargetsCount(); ++i)
    {
        ProjectBuildTarget* target = project->GetBuildTarget(i);
        if (target)
        {
            if (compiler)
            {
                // post-processed search dirs (from build scripts)
                for (unsigned int ti = 0; ti < compiler->GetCompilerSearchDirs(target).GetCount(); ++ti)
                {
                    wxString out = compiler->GetCompilerSearchDirs(target)[ti];
                    wxFileName dir(out);
                    if(NormalizePath(dir,base))
                    {
                        parser->AddIncludeDir(dir.GetFullPath());
//                    Manager::Get()->GetLogManager()->DebugLog(_T("Parser tgt dir: ") + dir.GetFullPath());
                    }
                    else
                        Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"),out.c_str(),base.c_str()));
                }
            }

            // apply target vars
//            target->GetCustomVars().ApplyVarsToEnvironment();
            for (unsigned int ti = 0; ti < target->GetIncludeDirs().GetCount(); ++ti)
            {
                wxString out = target->GetIncludeDirs()[ti];
                Manager::Get()->GetMacrosManager()->ReplaceMacros(out);
                wxFileName dir(out);
                if(NormalizePath(dir,base))
                {
                    parser->AddIncludeDir(dir.GetFullPath());
//                    Manager::Get()->GetLogManager()->DebugLog(_T("Parser tgt dir: ") + dir.GetFullPath());
                }
                else
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"),out.c_str(),base.c_str()));
            }
            // get the compiler
            wxString CompilerIndex = target->GetCompilerID();
            Compiler* myc = CompilerFactory::GetCompiler(CompilerIndex);
            if (myc)
            {
                Compilers[nCompilers] = myc;
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

    // keep the gcc compiler path's once if found accross C::B session
    // makes opening workspaces a *lot* faster by avoiding endless calls to the compiler
    static wxArrayString gcc_compiler_dirs;

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
//                Manager::Get()->GetLogManager()->DebugLog(_T("Parser cmp dir: ") + dir.GetFullPath());
            }
            else
                Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"),out.c_str(),base.c_str()));
        }

        // find out which compiler, if gnu, do the special trick
        // to find it's internal include paths
        // but do only once per C::B session, thus cache for later calls
        wxString CompilerID = (Compilers[idxCompiler])->GetID();
        if(CompilerID == _T("gcc"))
        {
          if (gcc_compiler_dirs.IsEmpty())
          {
            Manager::Get()->GetLogManager()->DebugLog(_T("Caching internal gcc dirs for adding to parser..."));
            gcc_compiler_dirs = GetGCCCompilerDirs(((Compilers[idxCompiler])->GetPrograms()).CPP, base);
          }

          // Manager::Get()->GetLogManager()->DebugLog(_T("Adding %d cached gcc dirs to parser..."), gcc_compiler_dirs.GetCount());
          for (size_t i=0; i<gcc_compiler_dirs.GetCount(); i++)
            parser->AddIncludeDir(gcc_compiler_dirs[i]);
        }
    } // end of while loop over the found compilers
    if(!nCompilers)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("No compilers found!"));
    }
    delete [] Compilers;
} // end of AddCompilerDirs

wxArrayString NativeParser::GetGCCCompilerDirs(const wxString &cpp_compiler, const wxString &base)
{
    wxArrayString gcc_compiler_dirs;

    // for starters , only do this for gnu compiler
//    Manager::Get()->GetLogManager()->DebugLog(_T("CompilerID ") + CompilerID);
    //    wxString Command("mingw32-g++ -v -E -x c++ - < nul");
    // specifying "< nul", does not seem to work
    // workaround : create a dummy file (let's hope it does not exist)
    // do the trick only for c++, not needed then for C (since this is a subset of C++)
    wxString DummyFileName = wxFileName::CreateTempFileName(_T("Dummy_z4hsdkl9nf7ba3L9nv41"));
    if(!DummyFileName.IsEmpty())
    {
        // let's construct the command
        wxString Command = cpp_compiler + _T(" -v -E -x c++ ") + DummyFileName;
        // action time  (everything shows up on the error stream
        wxArrayString Output, Errors;
        wxExecute(Command, Output, Errors, wxEXEC_NODISABLE);
        int nCount = Errors.GetCount();
        // the include dir (1 per line) show up between the lines
        // #include <...> search starts here:
        // End of search list
        //   let's hope this does not change too quickly, otherwise we need
        // to adjust our search code (for several versions ...)
        bool bStart = false;
        for(int idxCount = 0; idxCount < nCount; ++idxCount)
        {
            if (!bStart && Errors[idxCount] == _("#include <...> search starts here:"))
            {
                bStart = true;
            }
            else if (bStart && Errors[idxCount] == _("End of search list."))
            {
                bStart = false; // could jump out of for loop if we want
            }
            else if (bStart)
            {
//                Manager::Get()->GetLogManager()->DebugLog("include dir " + Errors[idxCount]);
                // get rid of the leading space (more general : any whitespace)in front
                wxRegEx reg(_T("^[ \t]*(.*)"));
                if(reg.Matches(Errors[idxCount]))
                {
                    wxString out = reg.GetMatch(Errors[idxCount], 1);
                    if(!out.IsEmpty())
                    {
                        wxFileName dir(out);
                        if (NormalizePath(dir,base))
                        {
                            Manager::Get()->GetLogManager()->DebugLog(_T("Caching GCC dir: ") + dir.GetFullPath());
                            gcc_compiler_dirs.Add(dir.GetFullPath());
                        }
                        else
                            Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"),out.c_str(),base.c_str()));
                    }
                }
            }
        } // end for : idx : idxCount
        // clean up our temp file
        ::wxRemoveFile(DummyFileName);
    } // Dummy is open

    return gcc_compiler_dirs;
}

wxArrayString& NativeParser::GetProjectSearchDirs(cbProject* project)
{
    ProjectSearchDirsMap::iterator it;
    it = m_ProjectSearchDirsMap.find(project);
    if (it == m_ProjectSearchDirsMap.end())
    {
        it = m_ProjectSearchDirsMap.insert(m_ProjectSearchDirsMap.end(), std::make_pair(project, wxArrayString()));
    }
    return it->second;
}

void NativeParser::AddParser(cbProject* project, bool useCache)
{
    if (!project)
        return;

    Manager::Get()->GetLogManager()->DebugLog(F(_T("Add project %s in parsing queue"), project->GetTitle().c_str()));
    Parser* parser = &m_Parser;//new Parser(this);
    AddCompilerDirs(parser, project);

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
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Error normalizing path: '%s' from '%s'"), path.c_str(), base.c_str()));
    }

    wxArrayString files;

    // parse header files first
    for (int i = 0; i < project->GetFilesCount(); ++i)
    {
        ProjectFile* pf = project->GetFile(i);
        FileType ft = FileTypeOf(pf->relativeFilename);
        if (ft == ftHeader) // only parse header files
        {
            files.Add(pf->file.GetFullPath());
        }
    }
    // next, parse source files
    for (int i = 0; i < project->GetFilesCount(); ++i)
    {
        ProjectFile* pf = project->GetFile(i);
        FileType ft = FileTypeOf(pf->relativeFilename);
        if (ft == ftSource) // only parse source files
        {
            files.Add(pf->file.GetFullPath());
        }
    }
    if (!files.IsEmpty())
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Passing list of files to parse"));
        parser->BatchParse(files);
    }
}

void NativeParser::RemoveParser(cbProject* project, bool useCache)
{
    if (Manager::Get()->GetProjectManager()->GetProjects()->GetCount() == 0)
    {
        m_Parser.Clear();
        UpdateClassBrowser();
        return;
    }
    if (!project)
        return;
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Removing project %s from parsed projects"), project->GetTitle().c_str()));
    for (int i = 0; i < project->GetFilesCount(); ++i)
    {
        ProjectFile* pf = project->GetFile(i);
        m_Parser.RemoveFile(pf->file.GetFullPath());
    }
    UpdateClassBrowser();
}

void NativeParser::AddFileToParser(cbProject* project, const wxString& filename)
{
    m_Parser.Parse(filename, true);
}

void NativeParser::RemoveFileFromParser(cbProject* project, const wxString& filename)
{
    m_Parser.RemoveFile(filename);
}

// NOTE: it actually forces reparsing of workspace
void NativeParser::ForceReparseActiveProject()
{
	m_Parser.Clear();
	UpdateClassBrowser();

	ProjectsArray* projects = Manager::Get()->GetProjectManager()->GetProjects();
	for (size_t i = 0; i < projects->GetCount(); ++i)
	{
		AddParser(projects->Item(i), false);
	}
}

Parser* NativeParser::FindParserFromEditor(EditorBase* editor)
{
    return &m_Parser;
}

Parser* NativeParser::FindParserFromActiveEditor()
{
    return &m_Parser;
}

Parser* NativeParser::FindParserFromActiveProject()
{
    return &m_Parser;
}

Parser* NativeParser::FindParserFromProject(cbProject* project)
{
    return &m_Parser;
}

// UNUSED
bool NativeParser::LoadCachedData(Parser* parser, cbProject* project)
{
    if (!parser || !project)
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

        ret = parser->ReadFromCache(&fb);
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
    if(!ret)
        Manager::Get()->GetLogManager()->DebugLog(_T("Error reading Cache! Re-parsing from scratch."));
//    else
//        DisplayStatus(parser, project);
    return ret;
}

// UNUSED
bool NativeParser::SaveCachedData(Parser* parser, const wxString& projectFilename)
{
    bool result = false;
    if (!parser)
        return false;

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
        result = parser->WriteToCache(&fb);
    }
    return result;
}

void NativeParser::DisplayStatus(Parser* parser)
{
    if (!parser)
        return;
    long int tim = parser->LastParseTime();
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Parsing stage done (%d total parsed files, %d tokens in %d minute(s), %d.%d seconds)."),
                    parser->GetFilesCount(),
                    parser->GetTokens()->realsize(),
                    (tim / 60000),
                    ((tim / 1000) %60),
                    tim % 1000));
}

bool NativeParser::ParseFunctionArguments(cbEditor* ed, int caretPos)
{
    if (!ed)
        return false;

    Parser* parser = FindParserFromEditor(ed);
    if (!parser)
        return false;

    if (!parser->Done())
        return false;

#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("Parse function arguments"));
#endif

    TokenIdxSet proc_result;
    if (FindCurrentFunctionToken(ed, proc_result, caretPos) != 0)
    {
        for (TokenIdxSet::iterator it = proc_result.begin(); it != proc_result.end(); ++it)
        {
            Token* token = parser->GetTokens()->at(*it);
            if (!token)
                continue;

#ifdef DEBUG_CC_AI
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(_T(" + Function match: ") + token->m_Name);
#endif

            if (!token->m_Args.IsEmpty() && !token->m_Args.Matches(_T("()")))
            {
                wxString buffer = token->m_Args;
                buffer.Remove(0, 1); // remove (
                buffer.RemoveLast(); // remove )
                buffer.Replace(_T(","), _T(";")); // replace commas with semi-colons
                buffer << _T(';'); // aid parser ;)
                buffer.Trim();
#ifdef DEBUG_CC_AI
                if (s_DebugSmartSense)
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("Parsing arguments: \"%s\""), buffer.c_str()));
#endif
                if (!buffer.IsEmpty() && !parser->ParseBuffer(buffer, false, false, true))
                {
#ifdef DEBUG_CC_AI
                    if (s_DebugSmartSense)
                        Manager::Get()->GetLogManager()->DebugLog(_T("ERROR parsing arguments"));
#endif
                }
            }
        }
        return true;
    }
    else
    {
#ifdef DEBUG_CC_AI
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(_T("Could not determine current function's namespace..."));
#endif
    }
    return false;
}

bool NativeParser::ParseLocalBlock(cbEditor* ed, int caretPos)
{
    if (!ed)
        return false;

    Parser* parser = FindParserFromEditor(ed);
    if (!parser)
        return false;

    if (!parser->Done())
        return false;

#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("Parse local block"));
#endif

    int blockStart = FindCurrentFunctionStart(ed, 0, 0, caretPos);
    if (blockStart != -1)
    {
        ++blockStart; // skip {
        int blockEnd = caretPos == -1 ? ed->GetControl()->GetCurrentPos() : caretPos;
		if (blockEnd < 0 || blockEnd >= ed->GetControl()->GetLength())
			return false;

        if (blockStart >= blockEnd)
            blockStart = blockEnd;

        wxString buffer = ed->GetControl()->GetTextRange(blockStart, blockEnd);
        buffer.Trim();
        if (!buffer.IsEmpty() && !parser->ParseBuffer(buffer, false, false, true))
        {
#ifdef DEBUG_CC_AI
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(_T("ERROR parsing block:\n") + buffer);
#endif
        }
        else
        {
#ifdef DEBUG_CC_AI
            if (s_DebugSmartSense)
            {
//				Manager::Get()->GetLogManager()->DebugLog(F(_T("Block:\n%s"), buffer.c_str()));
                Manager::Get()->GetLogManager()->DebugLog(_T("Local tokens:"));
                for (size_t i = 0; i < parser->GetTokens()->size(); ++i)
                {
                    Token* t = parser->GetTokens()->at(i);
                    if (t && t->m_IsTemp)
                        Manager::Get()->GetLogManager()->DebugLog(_T(" + ") + t->DisplayName());
                }
            }
#endif
            return true;
        }
    }
    else
    {
#ifdef DEBUG_CC_AI
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(_T("Could not determine current block start..."));
#endif
    }
    return false;
}

bool NativeParser::ParseUsingNamespace(cbEditor* ed, TokenIdxSet& search_scope, int caretPos)
{
    if (!ed)
        return false;

    Parser* parser = FindParserFromEditor(ed);
    if (!parser)
        return false;
    TokensTree* tree = parser->GetTokens();

#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("Parse file scope for \"using namespace\""));
#endif

    wxArrayString ns;
    int pos = caretPos == -1 ? ed->GetControl()->GetCurrentPos() : caretPos;
    if (pos < 0 || pos >= ed->GetControl()->GetLength())
		return false;
    wxString buffer = ed->GetControl()->GetTextRange(0, pos);
    parser->ParseBufferForUsingNamespace(buffer, ns);

    for (size_t i = 0; i < ns.GetCount(); ++i)
    {
        std::queue<ParserComponent> components;
        BreakUpComponents(parser, ns[i], components);

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
#ifdef DEBUG_CC_AI
        if (s_DebugSmartSense && parentIdx != -1)
        {
            Token* token = tree->at(parentIdx);
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Found %s%s"), token->GetNamespace().c_str(), token->m_Name.c_str()));
        }
#endif
        search_scope.insert(parentIdx);
    }

    return true;
}

size_t NativeParser::MarkItemsByAI(TokenIdxSet& result, bool reallyUseAI, bool noPartialMatch, bool caseSensitive, int caretPos)
{
    result.clear();

    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return 0;

    Parser* parser = FindParserFromEditor(ed);
    if (!parser)
        return 0;

    if (!parser->Done())
        Manager::Get()->GetLogManager()->DebugLog(_T("C++ Parser is still parsing files..."));
    else
    {
        // remove old temporaries
        parser->GetTokens()->FreeTemporaries();
        parser->GetTempTokens()->Clear();

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
            TokensTree* tokens = parser->GetTokens();
            for (size_t i = 0; i < tokens->size(); ++i)
                result.insert(i);
            return result.size();
        }

        return AI(result, ed, parser, wxEmptyString, noPartialMatch, caseSensitive, &search_scope, caretPos);
    }
    return 0;
}

const wxString& NativeParser::GetCodeCompletionItems()
{
    m_CCItems.Clear();

    Parser* parser = FindParserFromActiveEditor();
    if (!parser)
        return m_CCItems;

    TokenIdxSet result;
    int count = MarkItemsByAI(result);
    if (count)
    {
        TokensTree* tokens = parser->GetTokens();
        for (TokenIdxSet::iterator it = result.begin(); it != result.end(); ++it)
        {
            Token* token = tokens->at(*it);
            if(!token)
                continue;
            if (!m_CCItems.IsEmpty())
                 m_CCItems << _T(";");
            m_CCItems << token->m_Name << token->m_Args;//" " << token->m_Filename << ":" << token->m_Line;
        }
    }

    return m_CCItems;
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

const wxArrayString& NativeParser::GetCallTips(int chars_per_line)
{
    m_CallTips.Clear();
    Parser* parser = 0;
    int end = 0;
    int commas = 0;
    wxString lineText = _T("");
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    wxCriticalSectionLocker* lock = 0;
    do
    {
        if(!ed)
            break;
        parser = FindParserFromActiveEditor();
        if(!parser)
            break;
        if(!parser->Done())
            break;
        int line = ed->GetControl()->GetCurrentLine();
        lineText = ed->GetControl()->GetLine(line);
        end = lineText.Length();
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
        lineText.Remove(end);
//        Manager::Get()->GetLogManager()->DebugLog(_T("Sending \"%s\" for call-tip"), lineText.c_str());

        TokensTree* tokens = parser->GetTokens();
        TokenIdxSet result;
        lock = new wxCriticalSectionLocker(s_MutexProtection);

        tokens->FreeTemporaries();

        TokenIdxSet search_scope;
        ParseUsingNamespace(ed, search_scope);
        ParseFunctionArguments(ed);
        ParseLocalBlock(ed);

        m_GettingCalltips = true;
        if (!AI(result, ed, parser, lineText, true, true, &search_scope))
            break;
        for (TokenIdxSet::iterator it = result.begin(); it != result.end(); ++it)
        {
            Token* token = tokens->at(*it);
            if(!token)
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
    }while(false);
    if(lock)
        delete lock;
    m_GettingCalltips = false;
    m_CallTipCommas = commas;
//    Manager::Get()->GetLogManager()->DebugLog(_T("m_CallTipCommas=%d"), m_CallTipCommas);
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

        if (c == _T(','))
            last_comma = pos;
        else if (c == _T(' ') || c == _T('\t'))
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

unsigned int NativeParser::FindCCTokenStart(const wxString& line)
{
    int x = line.Length() - 1;
    int nest = 0;

    bool repeat = true;
    while (repeat)
    {
        repeat = false;
        while (x >= 0 && (wxIsalnum(line.GetChar(x)) || line.GetChar(x) == '_'))
            --x;

        if (x > 0 &&
            (line.GetChar(x) == '>' && line.GetChar(x - 1) == '-') ||
            (line.GetChar(x) == ':' && line.GetChar(x - 1) == ':'))
        {
            x -= 2;
            repeat = true;
        }
        else if (x >= 0 && line.GetChar(x) == '.')
        {
            --x;
            repeat = true;
        }

        if (repeat)
        {
            // now we 're just before the "." or "->" or "::"
            // skip any whitespace
            while (x >= 0 && (line.GetChar(x) == ' ' || line.GetChar(x) == '\t'))
                --x;

            // check for function/array/cast ()
            if (x >= 0 && (line.GetChar(x) == ')' || line.GetChar(x) == ']'))
            {
                ++nest;
                while (--x >= 0 && nest != 0)
                {
                    switch (line.GetChar(x))
                    {
                        case ']':
                        case ')': ++nest; break;

                        case '[':
                        case '(': --nest; break;
                    }
                }
                if (x > 0 && (wxIsalnum(line.GetChar(x - 1)) || line.GetChar(x - 1) == '_'))
                    --x;
            }
        }
    }
    ++x;

    if (x < 0)
        x = 0;

    while (line.GetChar(x) == ' ' || line.GetChar(x) == '\t')
        ++x;

    //Manager::Get()->GetLogManager()->DebugLog("Starting at %d \"%s\"", x, line.Mid(x).c_str());
    return x;
}

wxString NativeParser::GetNextCCToken(const wxString& line, unsigned int& startAt, bool& is_function)
{
    wxString res;
    int nest = 0;

    if (startAt < line.Length() && line.GetChar(startAt) == '(')
    {
        while (startAt < line.Length() &&
                (line.GetChar(startAt) == '*' ||
                line.GetChar(startAt) == '&' ||
                line.GetChar(startAt) == '('))
        {
            if (line.GetChar(startAt) == '(')
                ++nest;
            ++startAt;
        }
    }

//    Manager::Get()->GetLogManager()->DebugLog(_T("at %d (%c): res=%s"), startAt, line.GetChar(startAt), res.c_str());
    while (startAt < line.Length() && (wxIsalnum(line.GetChar(startAt)) || line.GetChar(startAt) == '_'))
    {
        res << line.GetChar(startAt);
        ++startAt;
    }

    while (nest > 0 && startAt < line.Length())
    {
        if (line.GetChar(startAt) == ')')
            --nest;
        ++startAt;
    }
    //Manager::Get()->GetLogManager()->DebugLog("Done nest: at %d (%c): res=%s", startAt, line.GetChar(startAt), res.c_str());

    if (startAt < line.Length() && (line.GetChar(startAt) == '(' || line.GetChar(startAt) == '['))
    {
        is_function = line.GetChar(startAt) == '(';

        ++nest;
        while (startAt < line.Length() - 1 && nest != 0)
        {
            ++startAt;
            switch (line.GetChar(startAt))
            {
                case ']':
                case ')': --nest; break;

                case '[':
                case '(': ++nest; break;
            }
        }
        ++startAt;
    }

    //Manager::Get()->GetLogManager()->DebugLog("Return at %d (%c): res=%s", startAt, line.GetChar(startAt), res.c_str());
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
    unsigned int x = FindCCTokenStart(line);
    wxString res = GetNextCCToken(line, x, is_function);
//    Manager::Get()->GetLogManager()->DebugLog(_T("FindCCTokenStart returned %d \"%s\""), x, line.c_str());
//    Manager::Get()->GetLogManager()->DebugLog(_T("GetNextCCToken returned %d \"%s\""), x, res.c_str());

    if (x == line.Length())
        line.Clear();
    else
    {
        // skip whitespace
        while (line.GetChar(x) == ' ' || line.GetChar(x) == '\t')
            ++x;

        if (line.GetChar(x) == '.')
        {
            tokenType = pttClass;
            line.Remove(0, x + 1);
        }
        else if ((x < line.Length() - 1 && line.GetChar(x) == '-' && line.GetChar(x + 1) == '>') ||
            (x < line.Length() - 1 && line.GetChar(x) == ':' && line.GetChar(x + 1) == ':'))
        {
            if (line.GetChar(x) == ':')
                tokenType = pttNamespace;
            else
                tokenType = pttClass;
            line.Remove(0, x + 2);
        }
        else
            line.Clear();
    }
//    Manager::Get()->GetLogManager()->DebugLog(_T("Left \"%s\""), line.c_str());

    if (is_function)
        tokenType = pttFunction;
    return res;
}

// Start an Artificial Intelligence (!) sequence to gather all the matching tokens..
// The actual AI is in FindAIMatches() below...
size_t NativeParser::AI(TokenIdxSet& result,
                        cbEditor* editor,
                        Parser* parser,
                        const wxString& lineText,
                        bool noPartialMatch,
                        bool caseSensitive,
                        TokenIdxSet* search_scope,
                        int caretPos)
{
    m_LastAISearchWasGlobal = false;
    m_LastAIGlobalSearch.Clear();

    int pos = caretPos == -1 ? editor->GetControl()->GetCurrentPos() : caretPos;
    if (pos < 0 || pos >= editor->GetControl()->GetLength())
		return 0;
    m_EditorStartWord = editor->GetControl()->WordStartPosition(pos, true);
    m_EditorEndWord = pos;//editor->GetControl()->WordEndPosition(pos, true);
    int line = editor->GetControl()->LineFromPosition(pos);

    wxString searchtext;
    //Manager::Get()->GetLogManager()->DebugLog("********* START **********");

    TokensTree* tree = parser->GetTokens();
    if(!tree)
        return 0;
//    Token* parentToken = 0L;
    wxString actual;
    int col;
    wxString tabwidth;
    tabwidth.Pad(editor->GetControl()->GetTabWidth(), ' ');
    if (lineText.IsEmpty())
    {
        actual = editor->GetControl()->GetLine(line);
        col = editor->GetControl()->GetColumn(pos);
        // replace tabs in line by equal-count spaces because col is in spaces!
        actual.Replace(_T("\t"), tabwidth);
        actual.Remove(col);
        actual.Trim();
    }
    else
    {
        actual = lineText;
        col = actual.Length() - 1;
    }

    static cbEditor* cached_editor = 0;
    static int cached_editor_start_word = 0;
    static wxString cached_search;
    static size_t cached_results_count = 0;

    // early-out opportunity
    // if the user starts typing a token that in our last search had 0 results,
    // and we see that he's continuing typing for that same token,
    // don't even bother to search
    if (cached_editor == editor &&
        cached_editor_start_word == m_EditorStartWord &&
        cached_results_count == 0 &&
        actual.StartsWith(cached_search))
    {
#ifdef DEBUG_CC_AI
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(_T("Aborting search: last attempt returned 0 results"));
#endif
        // but set m_LastAISearchWasGlobal correctly so C++ keywords can be shown
        std::queue<ParserComponent> components;
        BreakUpComponents(parser, actual, components);
        m_LastAISearchWasGlobal = components.size() <= 1;
        if (!components.empty())
            m_LastAIGlobalSearch = components.front().component;
        return 0;
    }

#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("========================================================="));
        Manager::Get()->GetLogManager()->DebugLog(F(_T("Doing AI for '%s':"), actual.c_str()));
    }
#endif

    // find current function's namespace so we can include local scope's tokens
    // we ' ll get the function's token (all matches) and add its parent namespace
    TokenIdxSet scope_result;
    TokenIdxSet proc_result;
    if (FindCurrentFunctionToken(editor, proc_result) != 0)
    {
        for (TokenIdxSet::iterator it = proc_result.begin(); it != proc_result.end(); ++it)
        {
            Token* token = parser->GetTokens()->at(*it);
            if (!token)
                continue;
            scope_result.insert(token->m_ParentIndex);
#ifdef DEBUG_CC_AI
            if (s_DebugSmartSense)
            {
                Token* parent = parser->GetTokens()->at(token->m_ParentIndex);
                Manager::Get()->GetLogManager()->DebugLog(_T("Adding search namespace: ") + (parent ? parent->m_Name : _T("Global namespace")));
            }
#endif
        }
    }


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

    // always add scope -1 (i.e. global namespace)
    search_scope->insert(-1);

    // find all other matches
    std::queue<ParserComponent> components;
    BreakUpComponents(parser, actual, components);

    m_LastAISearchWasGlobal = components.size() <= 1;
    if (!components.empty())
        m_LastAIGlobalSearch = components.front().component;

    // actually find all matches in selected namespaces
    for (TokenIdxSet::iterator it = search_scope->begin(); it != search_scope->end(); ++it)
    {
#ifdef DEBUG_CC_AI
        if (s_DebugSmartSense)
        {
            Token* scopeToken = tree->at(*it);
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Parent scope: '%s' (%d)"), scopeToken ? scopeToken->m_Name.c_str() : _T("Global namespace"), *it));
        }
#endif
        FindAIMatches(parser, components, result, *it, noPartialMatch, caseSensitive, true, 0xffff, search_scope);
    }

    cached_editor = editor;
    cached_editor_start_word = m_EditorStartWord;
    cached_search = actual;
    cached_results_count = result.size();

    return result.size();
}

// Breaks up the phrase for code-completion.
// Suppose the user has invokde code-completion in this piece of code:
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
size_t NativeParser::BreakUpComponents(Parser* parser, const wxString& actual, std::queue<ParserComponent>& components)
{
    ParserTokenType tokenType;
    wxString tmp = actual;

    // break up components of phrase
    while (true)
    {
        wxString tok = GetCCToken(tmp, tokenType);

        ParserComponent pc;
        pc.component = tok;
        pc.token_type = tokenType;
        components.push(pc);

        if (tokenType == pttSearchText)
            break;
    }

    return 0;
}

// Here's the meat of code-completion :)
// This function decides most of what gets included in the auto-completion list
// presented to the user.
// It's called recursively for each component of the std::queue argument.
size_t NativeParser::FindAIMatches(Parser* parser,
                                std::queue<ParserComponent> components,
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

#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("FindAIMatches - enter"));
#endif
    TokensTree* tree = parser->GetTokens();

    // pop top component
    ParserComponent parser_component = components.front();
    components.pop();

    // handle the special keyword "this".
    if (parentTokenIdx != -1 && parser_component.component == _T("this"))
    {
        // this will make the AI behave like it's the previous scope (or the current if no previous scope)

        // move on please, nothing to see here...
        return FindAIMatches(parser, components, result, parentTokenIdx, noPartialMatch, caseSensitive, use_inheritance, kindMask, search_scope);
    }

    // we 'll only add tokens in the result set if we get matches for the last token
    bool is_last = components.empty();
    wxString searchtext = parser_component.component;
#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("Search for ") + searchtext);
#endif

    // get a set of matches for the current token
    TokenIdxSet local_result;
    GenerateResultSet(tree, searchtext, parentTokenIdx, local_result, caseSensitive || !is_last, is_last && !noPartialMatch, kindMask);
//    tree->FindMatches(searchtext, local_result, caseSensitive || !is_last, is_last && !noPartialMatch);
#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("Looping %d results"), local_result.size()));
#endif

    // loop all matches, and recurse
    for (TokenIdxSet::iterator it = local_result.begin(); it != local_result.end(); it++)
    {
        int id = *it;
        Token* token = tree->at(id);

        // sanity check
        if (!token)
            continue;
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

#ifdef DEBUG_CC_AI
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Match: '%s' (%d) : '%s'"), token->m_Name.c_str(), id, token->m_ActualType.c_str()));
#endif

        // is the token a function or variable (i.e. is not a type)
        if (!searchtext.IsEmpty() &&
            (parser_component.token_type != pttSearchText /*||
            m_GettingCalltips*/) && // DISABLED! (crash in some cases) this allows calltips for typedef'd function pointers
            !token->m_ActualType.IsEmpty())
        {
            // the token is not a type
            // find its type's ID and use this as parent instead of (*it)
            TokenIdxSet type_result;
            std::queue<ParserComponent> type_components;
            wxString actual = token->m_ActualType;
            // TODO: ignore builtin types (void, int, etc)
            BreakUpComponents(parser, actual, type_components);
            // the parent to search under is a bit troubling, because of namespaces
            // what we 'll do is search under current parent and traverse up the parentship
            // until we find a result, or reach -1...
#ifdef DEBUG_CC_AI
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(F(_T("Looking for type: '%s' (%d components)"), actual.c_str(), type_components.size()));
#endif
            // search under all search-scope namespaces too
            TokenIdxSet temp_search_scope;
            if (search_scope)
                temp_search_scope = *search_scope;
            TokenIdxSet::iterator itsearch;
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
#ifdef DEBUG_CC_AI
                    if (s_DebugSmartSense)
                        Manager::Get()->GetLogManager()->DebugLog(_T("Implicit search scope added: ") + parent->GetParentToken()->m_Name);
#endif
                }
            }
            itsearch = temp_search_scope.begin();
            while (!search_scope || itsearch != temp_search_scope.end())
            {
                Token* parent = tree->at(*itsearch);
#ifdef DEBUG_CC_AI
                if (s_DebugSmartSense)
                    Manager::Get()->GetLogManager()->DebugLog(F(_T(" : looking under '%s'"), parent ? parent->m_Name.c_str() : _T("Global namespace")));
#endif
                do
                {
                    // types are searched as whole words, case-sensitive and only classes/namespaces
                    if (FindAIMatches(parser, type_components, type_result, parent ? parent->GetSelf() : -1, true, false, false, tkClass | tkNamespace | tkTypedef | tkEnum, &temp_search_scope) != 0)
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
                        FindAIMatches(parser, lcomp, result, *it, noPartialMatch, caseSensitive, use_inheritance, kindMask, search_scope);
                        ++it;
                    }
                }
#ifdef DEBUG_CC_AI
                if (s_DebugSmartSense)
                {
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("Type: '%s' (%d)"), tree->at(id)->m_Name.c_str(), id));
                    if (type_result.size() > 1)
                        Manager::Get()->GetLogManager()->DebugLog(F(_T("Multiple types matched for '%s': %d results"), token->m_ActualType.c_str(), type_result.size()));
                }
#endif
            }
        }

        // if no more components, add to result set
        if (is_last)
            result.insert(id);
        // else recurse this function using id as a parent
        else
            FindAIMatches(parser, components, result, id, noPartialMatch, caseSensitive, use_inheritance, kindMask, search_scope);
    }
#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("FindAIMatches - leave"));
#endif
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

size_t NativeParser::GenerateResultSet(TokensTree* tree,
                                        const wxString& search,
                                        int parentIdx,
                                        TokenIdxSet& result,
                                        bool caseSens,
                                        bool isPrefix,
                                        short int kindMask)
{
    if (!tree)
        return 0;

    Token* parent = tree->at(parentIdx);
#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("GenerateResultSet: search '%s', parent='%s'"), search.c_str(), parent ? parent->m_Name.c_str() : _T("Global namespace")));
#endif
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
        return -1;

    static cbEditor* s_LastEditor = 0;
    static int s_LastLine = -1;
    static int s_LastResult = -1;
    static wxString s_LastNS;
    static wxString s_LastPROC;

    // cache last result for optimization
    int pos = caretPos == -1 ? control->GetCurrentPos() : caretPos;
    if (pos < 0 || pos >= control->GetLength())
		return -1;
    int line = control->LineFromPosition(pos) + 1;
    if (line == s_LastLine && editor == s_LastEditor)
    {
        if (nameSpace) *nameSpace = s_LastNS;
        if (procName) *procName = s_LastPROC;
        return s_LastResult;
    }
    s_LastEditor = editor;
    s_LastLine = line;

    Parser parser(this);
    parser.ParseBufferForFunctions(control->GetTextRange(0, pos));

    wxArrayString funcs;
    TokensTree* tmptree = parser.GetTempTokens();

    // look for implementation functions that enclose our current line number
    for(size_t i = 0; i < tmptree->size();i++)
    {
        Token* token = tmptree->at(i);
        if (token && (token->m_TokenKind == tkFunction || token->m_TokenKind == tkConstructor || token->m_TokenKind == tkDestructor))
        {
            // found a function; check its bounds
            if (token->m_ImplLineStart <= (size_t)line && token->m_ImplLineEnd >= (size_t)line)
            {
                // got it :)
#ifdef DEBUG_CC_AI
                if (s_DebugSmartSense)
                    Manager::Get()->GetLogManager()->DebugLog(F(_T("Current function: %s (at line %d)"), token->DisplayName().c_str(), token->m_ImplLine));
#endif
                s_LastNS = token->GetNamespace();
                s_LastPROC = token->m_Name;
                s_LastResult = control->PositionFromLine(token->m_ImplLine - 1);

                // locate function's opening brace
                while (s_LastResult < control->GetTextLength())
                {
                    wxChar ch = control->GetCharAt(s_LastResult);
                    if (ch == _T('{'))
                        break;
                    else if (ch == 0)
                        return -1;
                    ++s_LastResult;
                }

                if (nameSpace) *nameSpace = s_LastNS;
                if (procName) *procName = s_LastPROC;

                return s_LastResult;
            }
        }
    }
#ifdef DEBUG_CC_AI
    if (s_DebugSmartSense)
        Manager::Get()->GetLogManager()->DebugLog(_T("Can't determine current function..."));
#endif
    s_LastResult = -1;
    return -1;
}

size_t NativeParser::FindCurrentFunctionToken(cbEditor* editor, TokenIdxSet& result, int caretPos)
{
    if (!editor)
        return 0;

    Parser* parser = FindParserFromEditor(editor);
    if (!parser)
        return 0;

    if (!parser->Done())
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
        BreakUpComponents(parser, scopeName, ns);
        FindAIMatches(parser, ns, scope_result, -1, true, true, false, tkNamespace | tkClass | tkTypedef);
    }

    // if no scope, use global scope
    if (scope_result.empty())
        scope_result.insert(-1);

    for (TokenIdxSet::iterator it = scope_result.begin(); it != scope_result.end(); ++it)
    {
        GenerateResultSet(parser->GetTokens(), procName, *it, result, true, false, tkFunction | tkConstructor | tkDestructor);
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

void NativeParser::OnParserEnd(wxCommandEvent& event)
{
    Parser* parser = (Parser*)event.GetClientData();
    if (parser)// && parser->Done())
    {
        // inheritance post-step
        parser->LinkInheritance(false);

        // also, mark all workspace files as local
        ProjectsArray* projects = Manager::Get()->GetProjectManager()->GetProjects();
        for (size_t i = 0; i < projects->GetCount(); ++i) // for all projects
        {
            cbProject* prj = projects->Item(i);
            for (int x = 0; x < prj->GetFilesCount(); ++x) // for all files in project
            {
                ProjectFile* pf = prj->GetFile(x);
                if (!pf)
                    continue;
                parser->MarkFileTokensAsLocal(pf->file.GetFullPath(), true, prj);
            }
        }

        DisplayStatus(parser);
        UpdateClassBrowser();
    }

    event.Skip();
}

void NativeParser::OnEditorActivated(EditorBase* editor)
{
    if (!m_pClassBrowser)
        return;
    cbEditor* ed = editor && editor->IsBuiltinEditor() ? static_cast<cbEditor*>(editor) : 0;
    if (ed)
    {
        Parser* parser = FindParserFromEditor(ed);
        if (parser && parser->ClassBrowserOptions().displayFilter == bdfFile)
        {
            m_pClassBrowser->UpdateView();
        }
    }
}
