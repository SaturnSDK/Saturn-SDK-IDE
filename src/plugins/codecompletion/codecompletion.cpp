/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include "codecompletion.h"
#include <wx/mimetype.h>
#include <wx/filename.h>
#include <wx/regex.h>
#include <wx/xrc/xmlres.h>
#include <wx/fs_zip.h>
#include <wx/msgdlg.h>
#include <wx/utils.h>
#include <wx/choice.h>
#include <wx/choicdlg.h>
#include <manager.h>
#include <configmanager.h>
#include <logmanager.h>
#include <projectmanager.h>
#include <editormanager.h>
#include <editorcolourset.h>
#include <sdk_events.h>
#include <incrementalselectlistdlg.h>
#include "insertclassmethoddlg.h"
#include "ccoptionsdlg.h"
#include "ccoptionsprjdlg.h"
#include "parser/parser.h"
#include "parser/tokenizer.h"
#include "selectincludefile.h"
#include "globals.h"
#include "cbstyledtextctrl.h"


#include "editor_hooks.h"
#include "cbeditor.h"
#include "multiselectdlg.h"
#include <wx/wxscintilla.h>
#include <wx/tipwin.h>
#include <wx/tokenzr.h>

#include <set> // for handling unique items in some places
#include <algorithm>
#include <iterator>

using namespace std;

// this auto-registers the plugin
namespace
{
    PluginRegistrant<CodeCompletion> reg(_T("CodeCompletion"));
}

// empty bitmap for use as C++ keywords icon in code-completion list
/* XPM */
static const char * cpp_keyword_xpm[] = {
"16 16 2 1",
"     c None",
".    c #04049B",
"                ",
"  .......       ",
" .........      ",
" ..     ..      ",
"..              ",
"..   ..     ..  ",
"..   ..     ..  ",
".. ...... ......",
".. ...... ......",
"..   ..     ..  ",
"..   ..     ..  ",
"..      ..      ",
"...     ..      ",
" .........      ",
"  .......       ",
"                "};

// bitmap for other-than-C++ keywords
// it's pretty nice actually :)
/* XPM */
static const char * unknown_keyword_xpm[] = {
"16 16 7 1",
"     c None",
".    c #FF8800",
"+    c #FF8D0B",
"@    c #FF9115",
"#    c #FFA948",
"$    c #FFC686",
"%    c #FFFFFF",
"                ",
"                ",
"      ....      ",
"    ........    ",
"   ..+@+.....   ",
"   .+#$#+....   ",
"  ..@$%$@.....  ",
"  ..+#$#+.....  ",
"  ...+@+......  ",
"  ............  ",
"   ..........   ",
"   ..........   ",
"    ........    ",
"      ....      ",
"                ",
"                "};

// menu IDS
// just because we don't know other plugins' used identifiers,
// we use wxNewId() to generate a guaranteed unique ID ;), instead of enum
// (don't forget that, especially in a plugin)
int idMenuCodeComplete = wxNewId();
int idMenuShowCallTip = wxNewId();
int idMenuGotoFunction = wxNewId();
int idMenuGotoPrevFunction = wxNewId();
int idMenuGotoNextFunction = wxNewId();
int idViewClassBrowser = wxNewId();
int idEditorSubMenu = wxNewId();
int idClassMethod = wxNewId();
int idUnimplementedClassMethods = wxNewId();
int idGotoDeclaration = wxNewId();
int idGotoImplementation = wxNewId();
int idOpenIncludeFile = wxNewId();
int idStartParsingProjects = wxNewId();
int idCodeCompleteTimer = wxNewId();
int idFunctionsParsingTimer = wxNewId();

// milliseconds
#define EDITOR_AND_LINE_INTERVAL 150

BEGIN_EVENT_TABLE(CodeCompletion, cbCodeCompletionPlugin)
    EVT_UPDATE_UI_RANGE(idMenuCodeComplete, idViewClassBrowser, CodeCompletion::OnUpdateUI)

    EVT_MENU(idMenuCodeComplete, CodeCompletion::OnCodeComplete)
    EVT_MENU(idMenuShowCallTip, CodeCompletion::OnShowCallTip)
    EVT_MENU(idMenuGotoFunction, CodeCompletion::OnGotoFunction)
    EVT_MENU(idMenuGotoPrevFunction, CodeCompletion::OnGotoPrevFunction)
    EVT_MENU(idMenuGotoNextFunction, CodeCompletion::OnGotoNextFunction)
    EVT_MENU(idClassMethod, CodeCompletion::OnClassMethod)
    EVT_MENU(idUnimplementedClassMethods, CodeCompletion::OnUnimplementedClassMethods)
    EVT_MENU(idGotoDeclaration, CodeCompletion::OnGotoDeclaration)
    EVT_MENU(idGotoImplementation, CodeCompletion::OnGotoDeclaration)
    EVT_MENU(idOpenIncludeFile, CodeCompletion::OnOpenIncludeFile)

    EVT_MENU(idViewClassBrowser, CodeCompletion::OnViewClassBrowser)

    EVT_TIMER(idCodeCompleteTimer, CodeCompletion::OnCodeCompleteTimer)
    EVT_TIMER(idFunctionsParsingTimer, CodeCompletion::OnStartParsingFunctions)

//    EVT_CHOICE(XRCID("chcCodeCompletionScope"),  CodeCompletion::OnScope)
    EVT_CHOICE(XRCID("chcCodeCompletionFunction"),  CodeCompletion::OnFunction)

    EVT_MENU(PARSER_END, CodeCompletion::OnParserEnd)

END_EVENT_TABLE()

CodeCompletion::CodeCompletion() :
    m_EditorHookId(0),
    m_timerCodeCompletion(this, idCodeCompleteTimer),
    m_pCodeCompletionLastEditor(0),
    m_ActiveCalltipsNest(0),
    m_IsAutoPopup(false),
    m_ToolbarChanged(true),
    m_CurrentLine(0),
    m_LastFile(wxEmptyString),
    m_FunctionsParsingTimer(this, idFunctionsParsingTimer)
{
    if(!Manager::LoadResource(_T("codecompletion.zip")))
    {
        NotifyMissingFile(_T("codecompletion.zip"));
    }
}

CodeCompletion::~CodeCompletion()
{
}

void CodeCompletion::LoadTokenReplacements()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    ConfigManagerContainer::StringToStringMap& repl = Tokenizer::GetTokenReplacementsMap();
    repl.clear();

    if (!cfg->Exists(_T("token_replacements")))
    {
        // first run; add default replacements
        Tokenizer::SetReplacementString(_T("_GLIBCXX_STD"), _T("std"));
    }
    else
        cfg->Read(_T("token_replacements"), &repl);
}

void CodeCompletion::SaveTokenReplacements()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    const ConfigManagerContainer::StringToStringMap& repl = Tokenizer::GetTokenReplacementsMap();
    cfg->Write(_T("token_replacements"), repl);
}

cbConfigurationPanel* CodeCompletion::GetConfigurationPanel(wxWindow* parent)
{
    CCOptionsDlg* dlg = new CCOptionsDlg(parent, &m_NativeParsers, this);
    return dlg;
}

cbConfigurationPanel* CodeCompletion::GetProjectConfigurationPanel(wxWindow* parent, cbProject* project)
{
    return new CCOptionsProjectDlg(parent, project, &m_NativeParsers);
}

int CodeCompletion::Configure()
{
//    CCOptionsDlg dlg(Manager::Get()->GetAppWindow());
//    if (dlg.ShowModal() == wxID_OK)
//    {
//        m_NativeParsers.RereadParserOptions();
//    }
    return 0;
}

void CodeCompletion::RereadOptions()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    m_LexerKeywordsToInclude[0] = cfg->ReadBool(_T("/lexer_keywords_set1"), true);
    m_LexerKeywordsToInclude[1] = cfg->ReadBool(_T("/lexer_keywords_set2"), true);
    m_LexerKeywordsToInclude[2] = cfg->ReadBool(_T("/lexer_keywords_set3"), false);
    m_LexerKeywordsToInclude[3] = cfg->ReadBool(_T("/lexer_keywords_set4"), false);
    m_LexerKeywordsToInclude[4] = cfg->ReadBool(_T("/lexer_keywords_set5"), false);
    m_LexerKeywordsToInclude[5] = cfg->ReadBool(_T("/lexer_keywords_set6"), false);
    m_LexerKeywordsToInclude[6] = cfg->ReadBool(_T("/lexer_keywords_set7"), false);
    m_LexerKeywordsToInclude[7] = cfg->ReadBool(_T("/lexer_keywords_set8"), false);
    m_LexerKeywordsToInclude[8] = cfg->ReadBool(_T("/lexer_keywords_set9"), false);
}

void CodeCompletion::BuildMenu(wxMenuBar* menuBar)
{
    // if not attached, exit
    if (!IsAttached())
        return;

//    if (m_EditMenu)
//        return; // already set-up

    int pos = menuBar->FindMenu(_("&Edit"));
    if (pos != wxNOT_FOUND)
    {
        m_EditMenu = menuBar->GetMenu(pos);
        m_EditMenu->AppendSeparator();
        m_EditMenu->Append(idMenuCodeComplete, _("Complete code\tCtrl-Space"));
        m_EditMenu->Append(idMenuShowCallTip, _("Show call tip\tCtrl-Shift-Space"));
    }
    else
        Manager::Get()->GetLogManager()->DebugLog(_T("Could not find Edit menu!"));
    pos = menuBar->FindMenu(_("Sea&rch"));
    if (pos != wxNOT_FOUND)
    {
        m_SearchMenu = menuBar->GetMenu(pos);
        m_SearchMenu->Append(idMenuGotoFunction, _("Goto function...\tCtrl-Alt-G"));
        m_SearchMenu->Append(idMenuGotoPrevFunction, _("Goto previous function\tCtrl-PgUp"));
        m_SearchMenu->Append(idMenuGotoNextFunction, _("Goto next function\tCtrl-PgDn"));
    }
    else
        Manager::Get()->GetLogManager()->DebugLog(_T("Could not find Search menu!"));

    // add the classbrowser window in the "View" menu
    int idx = menuBar->FindMenu(_("View"));
    if (idx != wxNOT_FOUND)
    {
        m_ViewMenu = menuBar->GetMenu(idx);
        wxMenuItemList& items = m_ViewMenu->GetMenuItems();
        // find the first separator and insert before it
        for (size_t i = 0; i < items.GetCount(); ++i)
        {
            if (items[i]->IsSeparator())
            {
                m_ViewMenu->InsertCheckItem(i, idViewClassBrowser, _("Symbols browser"), _("Toggle displaying the symbols browser"));
                return;
            }
        }
        // not found, just append
        m_ViewMenu->AppendCheckItem(idViewClassBrowser, _("Symbols browser"), _("Toggle displaying the symbols browser"));
    }
}

void CodeCompletion::BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    // if not attached, exit
    if (!menu || !IsAttached() || !m_InitDone)
        return;

    if (type == mtEditorManager)
    {
        cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
        if (ed)
        {
            m_LastIncludeFileFrom = ed->GetFilename();
            cbStyledTextCtrl* control = ed->GetControl();
            int pos = control->GetCurrentPos();
            wxString line = control->GetLine(control->LineFromPosition(pos));

            wxRegEx reg(_T("^[ \t]*#[ \t]*include[ \t]+[\"<]([^\">]+)[\">]"));
            wxString inc;
            if (reg.Matches(line))
                inc = reg.GetMatch(line, 1);
            m_LastIncludeFile.Clear();
            if (!inc.IsEmpty())
            {
                wxString msg;
                msg.Printf(_("Open #include file: '%s'"), inc.c_str());
                menu->Insert(0, idOpenIncludeFile, msg);
                menu->Insert(1, wxID_SEPARATOR, wxEmptyString);
                m_LastIncludeFile = inc;
            }
            else // either #include or keyword-search
            {
                int ws = control->WordStartPosition(pos, true);
                int we = control->WordEndPosition(pos, true);
                wxString txt = control->GetTextRange(ws, we);
                m_LastKeyword.Clear();
                if (!txt.IsEmpty())
                {
                    wxString msg;
                    msg.Printf(_("Find declaration of: '%s'"), txt.c_str());
                    menu->Insert(0, idGotoDeclaration, msg);

                    msg.Printf(_("Find implementation of: '%s'"), txt.c_str());
                    menu->Insert(1, idGotoImplementation, msg);

                    menu->Insert(2, wxID_SEPARATOR, wxEmptyString);
                    m_LastKeyword = txt;
                }
            }
            int insertId = menu->FindItem(_("Insert"));
            if (insertId != wxNOT_FOUND)
            {
                wxMenuItem* insertMenu = menu->FindItem(insertId, NULL);
                if (insertMenu)
                {
                    wxMenu* subMenu = insertMenu->GetSubMenu();
                    if (subMenu)
                    {
                        subMenu->Append(idClassMethod, _("Class method declaration/implementation..."));
                        subMenu->Append(idUnimplementedClassMethods, _("All class methods without implementation..."));
                    }
                    else
                        Manager::Get()->GetLogManager()->DebugLog(_T("Could not find Insert menu 3!"));
                }
                else
                    Manager::Get()->GetLogManager()->DebugLog(_T("Could not find Insert menu 2!"));
            }
            else
                Manager::Get()->GetLogManager()->DebugLog(_T("Could not find Insert menu!"));
        }
    }
}

bool CodeCompletion::BuildToolBar(wxToolBar* toolBar)
{
    Manager::Get()->AddonToolBar(toolBar,_T("codecompletion_toolbar"));
    m_Function = XRCCTRL(*toolBar, "chcCodeCompletionFunction", wxChoice);
    m_Scope = XRCCTRL(*toolBar, "chcCodeCompletionScope", wxChoice);
    m_Scope->Disable();
    toolBar->Realize();
    toolBar->SetInitialSize();
    return true;
}

void CodeCompletion::OnAttach()
{
    m_PageIndex = -1;
    m_InitDone = false;
    m_EditMenu = 0;
    m_SearchMenu = 0;
    m_ViewMenu = 0;
    m_Function = 0;
    m_Scope = 0;
    m_ParsedProjects.clear();
    m_FunctionsScope.clear();
    m_NameSpaces.clear();
    m_AllFunctionsScopes.clear();
    m_ToolbarChanged = true; // by default

    m_LastFile = wxEmptyString;

    LoadTokenReplacements();
    RereadOptions();

    m_LastPosForCodeCompletion = -1;
    StartIdxNameSpaceInScope = -1;
    m_NativeParsers.SetNextHandler(this);

    m_NativeParsers.CreateClassBrowser();

    // hook to editors
    EditorHooks::HookFunctorBase* myhook = new EditorHooks::HookFunctor<CodeCompletion>(this, &CodeCompletion::EditorEventHook);
    m_EditorHookId = EditorHooks::RegisterHook(myhook);

    // register event sinks
    Manager* pm = Manager::Get();

    pm->RegisterEventSink(cbEVT_EDITOR_SAVE, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnReparseActiveEditor));
    pm->RegisterEventSink(cbEVT_EDITOR_OPEN, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnEditorOpen));
    pm->RegisterEventSink(cbEVT_EDITOR_ACTIVATED, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnEditorActivated));
    pm->RegisterEventSink(cbEVT_EDITOR_TOOLTIP, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnValueTooltip));
    pm->RegisterEventSink(cbEVT_EDITOR_CLOSE, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnEditorClosed));

    pm->RegisterEventSink(cbEVT_APP_STARTUP_DONE, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnAppDoneStartup));
    pm->RegisterEventSink(cbEVT_WORKSPACE_CHANGED, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnWorkspaceChanged));
    pm->RegisterEventSink(cbEVT_PROJECT_ACTIVATE, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnProjectActivated));
    pm->RegisterEventSink(cbEVT_PROJECT_CLOSE, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnProjectClosed));
    pm->RegisterEventSink(cbEVT_PROJECT_FILE_ADDED, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnProjectFileAdded));
    pm->RegisterEventSink(cbEVT_PROJECT_FILE_REMOVED, new cbEventFunctor<CodeCompletion, CodeBlocksEvent>(this, &CodeCompletion::OnProjectFileRemoved));
}

void CodeCompletion::OnRelease(bool appShutDown)
{
    SaveTokenReplacements();

    // unregister hook
    // 'true' will delete the functor too
    EditorHooks::UnregisterHook(m_EditorHookId, true);

    m_NativeParsers.RemoveClassBrowser(appShutDown);
    m_NativeParsers.ClearParsers();
    m_ParsedProjects.clear();
    m_FunctionsScope.clear();
    m_NameSpaces.clear();
    m_AllFunctionsScopes.clear();
    m_ToolbarChanged = false;

/* TODO (mandrav#1#): Delete separator line too... */
    if (m_EditMenu)
    {
        m_EditMenu->Delete(idMenuCodeComplete);
        m_EditMenu->Delete(idMenuShowCallTip);
    }
    if (m_SearchMenu)
    {
        m_SearchMenu->Delete(idMenuGotoFunction);
        m_SearchMenu->Delete(idMenuGotoPrevFunction);
        m_SearchMenu->Delete(idMenuGotoNextFunction);
    }
}

static int SortCCList(const wxString& first, const wxString& second)
{
    const wxChar* a = first.c_str();
    const wxChar* b = second.c_str();
    while (*a && *b)
    {
        if (*a != *b)
        {
            if (*a == _T('?') && *b != _T('?'))
                return -1;
            else if (*a != _T('?') && *b == _T('?'))
                return 1;
            else if (*a == _T('?') && *b == _T('?'))
                return 0;
            if (*a == _T('_') && *b != _T('_'))
                return 1;
            else if (*a != _T('_') && *b == _T('_'))
                return -1;
            wxChar lowerA = wxTolower(*a);
            wxChar lowerB = wxTolower(*b);
            if (lowerA != lowerB)
                return lowerA - lowerB;
        }
        a++;
        b++;
    }
    // Either *a or *b is null
    return *a - *b;
}

int CodeCompletion::CodeComplete()
{
    if (!IsAttached() || !m_InitDone)
        return -1;

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if (!ed)
        return -3;

    FileType ft = FileTypeOf(ed->GetShortName());
//    if (ft != ftHeader && ft != ftSource) // only parse source/header files
//        return -4;

    Parser* parser = m_NativeParsers.FindParserFromEditor(ed);
    if (!parser)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Active editor has no associated parser ?!?"));
        return -4;
    }

//    wxBusyCursor busy;

    TokenIdxSet result;
    if (m_NativeParsers.MarkItemsByAI(result, parser->Options().useSmartSense) > 0 ||
        m_NativeParsers.LastAISearchWasGlobal()) // enter even if no match (code-complete C++ keywords)
    {
#ifdef DEBUG_CC_AI
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(F(_T("%d results"), result.size()));
#endif
        size_t max_match = cfg->ReadInt(_T("/max/matches"), 16384);
        if (result.size() <= max_match)
        {
#ifdef DEBUG_CC_AI
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(_T("Generating tokens list"));
#endif
            wxImageList* ilist = parser->GetImageList();
            ed->GetControl()->ClearRegisteredImages();
            bool caseSens = parser ? parser->Options().caseSensitive : false;
            wxArrayString items;
            int pos = ed->GetControl()->GetCurrentPos();
            int start = ed->GetControl()->WordStartPosition(pos, true);
            wxArrayInt already_registered;
            std::set< wxString, std::less<wxString> > unique_strings; // check against this before inserting a new string in the list
            items.Alloc(result.size());
            TokensTree* tokens = parser->GetTokens();
            for (TokenIdxSet::iterator it = result.begin(); it != result.end(); ++it)
            {
                Token* token = tokens->at(*it);
                if (!token || token->m_Name.IsEmpty())
                    continue;
                // check hashmap for unique_strings
                if (unique_strings.find(token->m_Name) != unique_strings.end())
                    continue;
                unique_strings.insert(token->m_Name);
                int iidx = parser->GetTokenKindImage(token);
                if (already_registered.Index(iidx) == wxNOT_FOUND)
                {
                    ed->GetControl()->RegisterImage(iidx, ilist->GetBitmap(iidx));
                    already_registered.Add(iidx);
                }
                wxString tmp;
                tmp << token->m_Name << wxString::Format(_T("?%d"), iidx);
                items.Add(tmp);

                if (token->m_TokenKind == tkNamespace && token->m_Aliases.size())
                {
                    for (size_t i = 0; i < token->m_Aliases.size(); ++i)
                    {
                        if (unique_strings.find(token->m_Aliases[i]) != unique_strings.end())
                            continue;
                        unique_strings.insert(token->m_Aliases[i]);
                        wxString tmp;
                        tmp << token->m_Aliases[i] << wxString::Format(_T("?%d"), iidx);
                        items.Add(tmp);
                    }
                }
            }

            if (m_NativeParsers.LastAISearchWasGlobal())
            {
                // empty or partial search phrase: add theme keywords in search list
#ifdef DEBUG_CC_AI
                if (s_DebugSmartSense)
                    Manager::Get()->GetLogManager()->DebugLog(_T("Last AI search was global: adding theme keywords in list"));
#endif
                EditorColourSet* theme = ed->GetColourSet();
                if (theme)
                {
                    wxString lastSearch = m_NativeParsers.LastAIGlobalSearch().Lower();
                    int iidx = ilist->GetImageCount();
                    bool isC = ft == ftHeader || ft == ftSource;
                    ed->GetControl()->RegisterImage(iidx, wxBitmap(isC ? cpp_keyword_xpm : unknown_keyword_xpm));
                    // theme keywords
                    HighlightLanguage lang = theme->GetLanguageForFilename(_T(".")+wxFileName(ed->GetFilename()).GetExt());
                    // the first two keyword sets are the primary and secondary keywords (for most lexers at least)
                    // but this is now configurable in global settings
                    for (int i = 0; i <= wxSCI_KEYWORDSET_MAX; ++i)
                    {
                        if (!m_LexerKeywordsToInclude[i])
                            continue;

                        wxString keywords = theme->GetKeywords(lang, i);
                        wxStringTokenizer tkz(keywords, _T(" \t\r\n"), wxTOKEN_STRTOK);
                        while (tkz.HasMoreTokens())
                        {
                            wxString kw = tkz.GetNextToken() + wxString::Format(_T("?%d"), iidx);
                            if (kw.Lower().StartsWith(lastSearch))
                                items.Add(kw);
                        }
                    }
                }
            }

#ifdef DEBUG_CC_AI
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(_T("0 results"));
#endif
            if (items.GetCount() == 0)
                return -2;

            if (caseSens)
                items.Sort();
            else
                items.Sort(SortCCList);
#ifdef DEBUG_CC_AI
            if (s_DebugSmartSense)
                Manager::Get()->GetLogManager()->DebugLog(_T("Done generating tokens list"));
#endif
            ed->GetControl()->AutoCompSetIgnoreCase(!caseSens);
            ed->GetControl()->AutoCompSetCancelAtStart(true);
            ed->GetControl()->AutoCompSetFillUps(cfg->Read(_T("/fillup_chars"), wxEmptyString));
            ed->GetControl()->AutoCompSetChooseSingle(m_IsAutoPopup ? false : cfg->ReadBool(_T("/auto_select_one"), false));
            ed->GetControl()->AutoCompSetAutoHide(true);
            ed->GetControl()->AutoCompSetDropRestOfWord(m_IsAutoPopup ? false : true);
            wxString final = GetStringFromArray(items, _T(" "));
            final.RemoveLast(); // remove last space
            ed->GetControl()->AutoCompShow(pos - start, final);
            return 0;
        }
        else if (!ed->GetControl()->CallTipActive())
        {
            wxString msg = _("Too many results.Please edit results' limit in code-completion options,\n"
                            "or type at least one more character to narrow the scope down...");
            ed->GetControl()->CallTipShow(ed->GetControl()->GetCurrentPos(), msg);
            return -2;
        }
    }
    else if (!ed->GetControl()->CallTipActive())
    {
#ifdef DEBUG_CC_AI
        if (s_DebugSmartSense)
            Manager::Get()->GetLogManager()->DebugLog(_T("0 results"));
#endif
        if (!parser->Done())
        {
            wxString msg = _("C++ Parser is still parsing files...");
            ed->GetControl()->CallTipShow(ed->GetControl()->GetCurrentPos(), msg);
        }
    }
    return -5;
}

void CodeCompletion::CodeCompleteIncludes()
{
    if (!IsAttached() || !m_InitDone)
        return;

    cbProject* pPrj = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (!pPrj)
        return;

    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if (!ed)
        return;

    Parser* parser = m_NativeParsers.FindParserFromActiveEditor();
    bool caseSens = parser ? parser->Options().caseSensitive : false;

    FileType ft = FileTypeOf(ed->GetShortName());
    if ( ft != ftHeader && ft != ftSource) // only parse source/header files
        return;

    int pos = ed->GetControl()->GetCurrentPos();
    int lineStartPos = ed->GetControl()->PositionFromLine(ed->GetControl()->GetCurrentLine());
    wxString line = ed->GetControl()->GetLine(ed->GetControl()->GetCurrentLine());
    //Manager::Get()->GetLogManager()->DebugLog("#include cc for \"%s\"", line.c_str());
    line.Trim();
    if (line.IsEmpty() || !line.StartsWith(_T("#include")))
        return;

    // find opening quote (" or <)
    int idx = pos - lineStartPos;
    int found = -1;
    wxString filename;
    while (idx > 0)
    {
        wxChar c = line.GetChar(idx);
        if (c == _T('>'))
            return; // the quote is closed already...
        else if (c == _T('"') || c == _T('<'))
        {
            if (found != -1)
                return; // the already found quote was the closing one...
            found = idx + 1;
        }
        else if (c != _T(' ') && c != _T('\t') && !found)
            filename << c;
        --idx;
    }

    // take care: found might point at the end of the string (out of bounds)
    // in this case: #include "(<-code-completion at this point)
    //Manager::Get()->GetLogManager()->DebugLog("#include using \"%s\" (starting at %d)", filename.c_str(), found);
    if (found == -1)
        return;

    // fill a list of matching project files
    wxArrayString files;
    for (int i = 0; i < pPrj->GetFilesCount(); ++i)
    {
        ProjectFile* pf = pPrj->GetFile(i);
        if (pf && FileTypeOf(pf->relativeFilename) == ftHeader)
        {
            wxFileName fname(pf->relativeFilename);
            if (files.Index(pf->relativeFilename) == wxNOT_FOUND)
                files.Add(pf->relativeFilename);
            if (files.Index(fname.GetFullName()) == wxNOT_FOUND)
                files.Add(fname.GetFullName());
        }
    }

    if (files.GetCount() != 0)
    {
        files.Sort();
        ed->GetControl()->AutoCompSetIgnoreCase(caseSens);
        ed->GetControl()->AutoCompShow(pos - lineStartPos - found, GetStringFromArray(files, _T(" ")));
    }
}

wxArrayString CodeCompletion::GetCallTips()
{
    if (!IsAttached() || !m_InitDone)
    {
        wxArrayString items;
        return items;
    }
    return m_NativeParsers.GetCallTips(-1);
}

void CodeCompletion::ShowCallTip()
{
    if (!IsAttached() || !m_InitDone)
        return;

    if (!Manager::Get()->GetEditorManager())
        return;

    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return;

    // calculate the size of the calltips window
    int pos = ed->GetControl()->GetCurrentPos();
    wxPoint p = ed->GetControl()->PointFromPosition(pos); // relative point
    int pixelWidthPerChar = ed->GetControl()->TextWidth(wxSCI_STYLE_LINENUMBER, _T("W"));
    int maxCalltipLineSizeInChars = (ed->GetSize().x - p.x) / pixelWidthPerChar;
    if (maxCalltipLineSizeInChars < 64)
    {
        // if less than a threshold in chars, recalculate the starting position (instead of shrinking it even more)
        p.x -= (64 - maxCalltipLineSizeInChars) * pixelWidthPerChar;
        // but if it goes out of range, continue shrinking
        if (p.x >= 0)
        {
            maxCalltipLineSizeInChars = 64;
            pos = ed->GetControl()->PositionFromPoint(p);
        }
        // else, out of range
    }

    int start = 0;
    int end = 0;
    int count = 0;
    int commas = m_NativeParsers.GetCallTipCommas(); // how many commas has the user typed so far?
    wxArrayString items = m_NativeParsers.GetCallTips(maxCalltipLineSizeInChars);
    std::set< wxString, std::less<wxString> > unique_tips; // check against this before inserting a new tip in the list
    wxString definition;
    for (unsigned int i = 0; i < items.GetCount(); ++i)
    {
        // allow only unique, non-empty items with equal or more commas than what the user has already typed
        if (unique_tips.find(items[i]) == unique_tips.end() && // unique
            !items[i].IsEmpty() && // non-empty
            commas <= m_NativeParsers.CountCommas(items[i], 1)) // commas satisfied
        {
            unique_tips.insert(items[i]);
            if (count != 0)
                definition << _T('\n'); // add new-line, except for the first line
            definition << items[i];
            m_NativeParsers.GetCallTipHighlight(items[i], &start, &end);
            ++count;
        }
    }
    if (!definition.IsEmpty())
        ed->GetControl()->CallTipShow(pos, definition);
//    Manager::Get()->GetLogManager()->DebugLog(_T("start=%d, end=%d"), start, end);
    // only highlight current argument if only one calltip (scintilla doesn't support multiple highlighting ranges in calltips)
    ed->GetControl()->CallTipSetHighlight(count == 1 ? start : 0, count == 1 ? end : 0);
}

int CodeCompletion::DoClassMethodDeclImpl()
{
    if (!IsAttached() || !m_InitDone)
        return -1;

    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if (!ed)
        return -3;

    FileType ft = FileTypeOf(ed->GetShortName());
    if ( ft != ftHeader && ft != ftSource) // only parse source/header files
        return -4;

    Parser* parser = m_NativeParsers.FindParserFromActiveEditor();
    if (!parser)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Active editor has no associated parser ?!?"));
        return -4;
    }

    wxString filename = ed->GetFilename();
    InsertClassMethodDlg dlg(Manager::Get()->GetAppWindow(), parser, filename);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        int pos = ed->GetControl()->GetCurrentPos();
        int line = ed->GetControl()->LineFromPosition(pos);
        ed->GetControl()->GotoPos(ed->GetControl()->PositionFromLine(line));

        wxArrayString result = dlg.GetCode();
        for (unsigned int i = 0; i < result.GetCount(); ++i)
        {
            pos = ed->GetControl()->GetCurrentPos();
            line = ed->GetControl()->LineFromPosition(pos);
            wxString str = ed->GetLineIndentString(line - 1) + result[i];
            ed->GetControl()->SetTargetStart(pos);
            ed->GetControl()->SetTargetEnd(pos);
            ed->GetControl()->ReplaceTarget(str);
            ed->GetControl()->GotoPos(pos + str.Length());// - 3);
        }
        return 0;
    }

    return -5;
}

int CodeCompletion::DoAllMethodsImpl()
{
    if (!IsAttached() || !m_InitDone)
        return -1;

    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if (!ed)
        return -3;

    FileType ft = FileTypeOf(ed->GetShortName());
    if ( ft != ftHeader && ft != ftSource) // only parse source/header files
        return -4;

    Parser* parser = m_NativeParsers.FindParserFromActiveEditor();
    if (!parser)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("Active editor has no associated parser ?!?"));
        return -4;
    }

    // mask for filenames (include only classes declared in filename.*)
    wxString filename = UnixFilename(ed->GetFilename().BeforeLast(_T('.')));
    filename << _T('.');

    TokensTree* tree = parser->GetTokens();

    // get all filenames' indices matching our mask
    std::set<size_t> result;
    tree->m_FilenamesMap.FindMatches(filename, result, true, true);
    if (result.empty())
    {
        cbMessageBox(_("File not in parser's database: ") + filename + _T('*'), _("Warning"), wxICON_WARNING);
        return -5;
    }

    // loop matching files, loop tokens in file and get list of un-implemented functions
    wxArrayString arr; // for selection (keeps strings)
    wxArrayInt arrint; // for selection (keeps indices)
    for (std::set<size_t>::iterator itf = result.begin(); itf != result.end(); ++itf)
    {
        TokenIdxSet& tokens = tree->m_FilesMap[*itf];
        // loop tokens in file
        for (TokenIdxSet::iterator its = tokens.begin(); its != tokens.end(); ++its)
        {
            Token* token = tree->at(*its);
            if (token && // valid token
                (token->m_TokenKind & (tkFunction | tkConstructor | tkDestructor)) && // is method
                token->m_ImplLine == 0) // is un-implemented
            {
                arr.Add(token->DisplayName());
                arrint.Add(*its);
            }
        }
    }

    if (arr.empty())
    {
        cbMessageBox(_("No classes declared or no un-implemented class methods found in ") + filename + _T('*'), _("Warning"), wxICON_WARNING);
        return -5;
    }

    // select tokens
    MultiSelectDlg dlg(Manager::Get()->GetAppWindow(), arr, true);
    if (dlg.ShowModal() == wxID_OK)
    {
        cbStyledTextCtrl* control = ed->GetControl();
        int pos = control->GetCurrentPos();
        int line = control->LineFromPosition(pos);
        control->GotoPos(control->PositionFromLine(line));

        wxArrayInt indices = dlg.GetSelectedIndices();
        for (size_t i = 0; i < indices.GetCount(); ++i)
        {
            Token* token = tree->at(arrint[indices[i]]);
            if (!token)
                continue;

            pos = control->GetCurrentPos();
            line = control->LineFromPosition(pos);

            // actual code generation
            wxString str;
            str << ed->GetLineIndentString(line - 1);
            str << _T("/** @brief ") << token->m_Name << _T("\n  *\n  * @todo: document this function\n  */\n");
            str << token->m_Type << _T(" ") << token->GetParentName() << _T("::") << token->m_Name << token->m_Args;
            if(token->m_IsConst)
            {
                str << _T(" const");
            }
            str << _T("\n{\n\n}\n\n");

            // add code in editor
            control->SetTargetStart(pos);
            control->SetTargetEnd(pos);
            control->ReplaceTarget(str);
            control->GotoPos(pos + str.Length());
        }
        return 0;
    }

    return -5;
}

void CodeCompletion::DoCodeComplete()
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if (!ed)
        return;

    int style = ed->GetControl()->GetStyleAt(ed->GetControl()->GetCurrentPos());
//    Manager::Get()->GetLogManager()->DebugLog(_T("Style at %d is %d"), ed->GetControl()->GetCurrentPos(), style);
//    Manager::Get()->GetLogManager()->DebugLog(_T("wxSCI_C_PREPROCESSOR is %d"), wxSCI_C_PREPROCESSOR);
    if (style == wxSCI_C_PREPROCESSOR)
    {
        CodeCompleteIncludes();
        return;
    }

    if (style != wxSCI_C_DEFAULT && style != wxSCI_C_OPERATOR && style != wxSCI_C_IDENTIFIER)
        return;

    CodeComplete();
}

void CodeCompletion::DoInsertCodeCompleteToken(wxString tokName)
{
    // remove arguments
    int pos = tokName.Find(_T("("));
    if (pos != wxNOT_FOUND)
        tokName.Remove(pos);

    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if (!ed)
        return;

    int end = ed->GetControl()->GetCurrentPos() > m_NativeParsers.GetEditorEndWord() ? ed->GetControl()->GetCurrentPos() : m_NativeParsers.GetEditorEndWord();
    ed->GetControl()->SetSelection(m_NativeParsers.GetEditorStartWord(), end);
    ed->GetControl()->ReplaceSelection(_T(""));
    ed->GetControl()->InsertText(m_NativeParsers.GetEditorStartWord(), tokName);
    ed->GetControl()->GotoPos(m_NativeParsers.GetEditorStartWord() + tokName.Length());
}

// events

void CodeCompletion::OnViewClassBrowser(wxCommandEvent& event)
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));
    if (!cfg->ReadBool(_T("/use_symbols_browser"), true))
    {
        cbMessageBox(_("The symbols browser is disabled in code-completion options.\n"
                        "Please enable it there first..."), _("Information"), wxICON_INFORMATION);
        return;
    }
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = (wxWindow*)m_NativeParsers.GetClassBrowser();
    Manager::Get()->ProcessEvent(evt);
}

void CodeCompletion::OnAppDoneStartup(CodeBlocksEvent& event)
{
    // Let the app startup before parsing
    // This is to prevent the Splash Screen from delaying so much. By adding the
    // timer, the splash screen is closed and Code::Blocks doesn't take so long
    // in starting.
    m_InitDone = true;

	// Dreaded DDE-open bug related: do not touch the following lines unless for a good reason
    
    // parse any projects opened through DDE or the command-line
    ParseActiveProjects();
    ProjectManager* prjMan = Manager::Get()->GetProjectManager();
	m_NativeParsers.SetClassBrowserProject(prjMan->GetActiveProject());

    event.Skip();
}

void CodeCompletion::OnCodeCompleteTimer(wxTimerEvent& event)
{
    if (Manager::Get()->GetEditorManager()->FindPageFromEditor(m_pCodeCompletionLastEditor) == -1)
        return; // editor is invalid (prbably closed already)

    // ask for code-completion *only* if the editor is still after the "." or "->" operator
    if (m_pCodeCompletionLastEditor->GetControl()->GetCurrentPos() == m_LastPosForCodeCompletion)
    {
        DoCodeComplete();
        m_LastPosForCodeCompletion = -1; // reset it
    }
}

void CodeCompletion::ParseActiveProjects()
{
    // parse all active projects
    m_InitDone = false;
    ProjectManager* prjMan = Manager::Get()->GetProjectManager();
    for (unsigned int i = 0; i < prjMan->GetProjects()->GetCount(); ++i)
    {
        cbProject* curproject = prjMan->GetProjects()->Item(i);
        if(m_ParsedProjects.find(curproject)==m_ParsedProjects.end())
        {
            m_ParsedProjects.insert(curproject);
            m_NativeParsers.AddParser(curproject);
        }
    }
    m_InitDone = true;
}

void CodeCompletion::OnWorkspaceChanged(CodeBlocksEvent& event)
{
    // EVT_WORKSPACE_CHANGED is a powerful event, it's sent after any project
    // has finished loading or closing. It's the *LAST* event to be sent when
    // the workspace has been changed, and it's not sent if the application is
    // shutting down. So it's the ideal time to parse files and update your
    // widgets.
    if (IsAttached() && m_InitDone)
    {
        // Parse the projects
        ParseActiveProjects();
        // Update the Function toolbar
        ParseFunctionsAndFillToolbar();
        // Update the class browser
        ProjectManager* prjMan = Manager::Get()->GetProjectManager();
        m_NativeParsers.SetClassBrowserProject(prjMan->GetActiveProject());
    }
    event.Skip();
}

void CodeCompletion::OnProjectActivated(CodeBlocksEvent& event)
{
    // The Class browser shouldn't be updated if we're in the middle of loading/closing
    // a project/workspace, because the class browser would need to be updated again.
    // So we need to update it with the EVT_WORKSPACE_CHANGED event, which gets
    // triggered after everything's finished loading/closing.

    if (!ProjectManager::IsBusy() && IsAttached() && m_InitDone)
        m_NativeParsers.SetClassBrowserProject(event.GetProject());
    event.Skip();
}

void CodeCompletion::OnProjectClosed(CodeBlocksEvent& event)
{
    // After this, the Class Browser needs to be updated. It will happen
    // when we receive the next EVT_PROJECT_ACTIVATED event.
    if (IsAttached() && m_InitDone)
    {
        m_ParsedProjects.erase(event.GetProject());
        m_NativeParsers.RemoveParser(event.GetProject());
    }
    event.Skip();
}

void CodeCompletion::OnProjectFileAdded(CodeBlocksEvent& event)
{
    if (IsAttached() && m_InitDone)
        m_NativeParsers.AddFileToParser(event.GetProject(), event.GetString());
    event.Skip();
}

void CodeCompletion::OnProjectFileRemoved(CodeBlocksEvent& event)
{
    if (IsAttached() && m_InitDone)
        m_NativeParsers.RemoveFileFromParser(event.GetProject(), event.GetString());
    event.Skip();
}

void CodeCompletion::OnUserListSelection(CodeBlocksEvent& event)
{
    if (IsAttached() && m_InitDone)
    {
        wxString tokName = event.GetString();
        DoInsertCodeCompleteToken(event.GetString());
    }

    event.Skip();
}

void CodeCompletion::OnReparseActiveEditor(CodeBlocksEvent& event)
{
    if (!ProjectManager::IsBusy() && IsAttached() && m_InitDone)
    {
        EditorBase* ed = event.GetEditor();
        if (!ed)
            return;
        Parser* parser = m_NativeParsers.FindParserFromEditor(ed);
        if (!parser)
            return;
        parser->Reparse(ed->GetFilename());
        ParseFunctionsAndFillToolbar(true);
    }
    event.Skip();
}

// compare method for the sort algorithm for our FunctionScope struct
bool LessFunctionScope(const CodeCompletion::FunctionScope& fs1, const CodeCompletion::FunctionScope& fs2)
{
    if(fs1.Name == fs2.Name)
    {
        return fs1.StartLine < fs2.StartLine;
    }
    else
    {
        return fs1.Name < fs2.Name;
    }
} // end of LessFunctionScope


// help method in finding the namespace position in the vector for the namespace containing the current line
int CodeCompletion::NameSpacePosition() const
{
    int retValue = -1; // -1 : not found
    for(unsigned int idxNs = 0; idxNs < m_NameSpaces.size(); ++idxNs)
    {
            const NameSpace Ns = m_NameSpaces[idxNs];
            if (Ns.StartLine <= m_CurrentLine && Ns.EndLine >= m_CurrentLine)
            {    // got one, maybe there might be a btter fitting namespace (embedded namespaces)
                // so keep on looking
                retValue = static_cast<int>(idxNs);
            }
            else if (Ns.StartLine > m_CurrentLine)
            {    // past the curent line, stop searching
            }
    } // end for : idx : idxNs
    return retValue;
} // end of NameSpacePosition

// help method in finding the function position in the vector for the function containing the current line
int CodeCompletion::FunctionPosition() const
{
    int retValue = -1; // -1 : not found
    for (unsigned int idxFn = 0; idxFn < m_FunctionsScope.size(); ++idxFn)
    {
        const FunctionScope fs = m_FunctionsScope[idxFn];
        if (fs.StartLine <= m_CurrentLine && fs.EndLine >= m_CurrentLine)
        {    // got it :)
            retValue = static_cast<int>(idxFn);
            break;
        }
    }
    return retValue;
}

void CodeCompletion::GotoFunctionPrevNext(bool next /* = false */)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if (!ed)
        return;

    int current_line = ed->GetControl()->GetCurrentLine();

    // search previous/next function from current line, default: previous
    int line = -1;  // -1 : not found
    if (m_FunctionsScope.size())
    {
        unsigned int best_func = 0;
        bool found_best_func   = false;
        for (unsigned int idx_func=0; idx_func<m_FunctionsScope.size(); ++idx_func)
        {
            int best_func_line  = m_FunctionsScope[best_func].StartLine;
            int func_start_line = m_FunctionsScope[idx_func].StartLine;
            if (next)
            {
                if         (best_func_line  > current_line)     // candidate: is after current line
                {
                    if (   (func_start_line > current_line  )   // another candidate
                        && (func_start_line < best_func_line) ) // decide which is more near
                    { best_func = idx_func; found_best_func = true; }
                }
                else if    (func_start_line > current_line)     // candidate: is after current line
                {     best_func = idx_func; found_best_func = true; }
            }
            else // prev
            {
                if         (best_func_line  < current_line)     // candidate: is before current line
                {
                    if (   (func_start_line < current_line  )   // another candidate
                        && (func_start_line > best_func_line) ) // decide which is more near
                    { best_func = idx_func; found_best_func = true; }
                }
                else if    (func_start_line < current_line)     // candidate: is before current line
                {     best_func = idx_func; found_best_func = true; }
            }
        }

        if      (found_best_func)
        { line = m_FunctionsScope[best_func].StartLine; }
        else if ( next && m_FunctionsScope[best_func].StartLine>current_line)
        { line = m_FunctionsScope[best_func].StartLine; }
        else if (!next && m_FunctionsScope[best_func].StartLine<current_line)
        { line = m_FunctionsScope[best_func].StartLine; }
    }

    if (line != -1)
    {
        ed->GotoLine(line);
        ed->SetFocus();
    }
}

void CodeCompletion::ParseFunctionsAndFillToolbar(bool force)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    if(!edMan) // Closing the app?
        return;
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if(!ed)
    {
        if (m_Function)
            m_Function->Clear();
        if (m_Scope)
            m_Scope->Clear();
        return;
    }
    wxString filename = ed->GetFilename();
    if(filename.IsEmpty())
        return;
    FunctionsScopePerFile* funcdata = &(m_AllFunctionsScopes[filename]);

    // *** Part 1: Parse the file (if needed) ***
    if(force || !funcdata->parsed)
    {
        m_FunctionsParsingTimer.Stop();
        funcdata->m_FunctionsScope.clear();
        funcdata->m_NameSpaces.clear();
        funcdata->parsed = true;

        Parser parser(this);
        TokensTree* tmptree = parser.GetTempTokens();
        tmptree->clear();
        parser.ParseBufferForFunctions(ed->GetControl()->GetText());

        for(size_t i = 0; i < tmptree->size(); ++i)
        {
            const Token* token = tmptree->at(i);
            if (token && (token->m_TokenKind == tkFunction || token->m_TokenKind == tkConstructor || token->m_TokenKind == tkDestructor)
                && token->m_ImplLine != 0)
            {
                FunctionScope func;
                func.StartLine = token->m_ImplLine - 1;
                func.EndLine = token->m_ImplLineEnd - 1;
                func.Scope = token->GetNamespace();
                wxString result = token->m_Name;
                result << token->m_Args;
                if (!token->m_Type.IsEmpty())
                    result << _T(" : ") << token->m_Type;
                func.Name = result;
                funcdata->m_FunctionsScope.push_back(func);
            }
            else if(token && token->m_TokenKind == tkNamespace)
            {
                NameSpace Ns;
                Ns.StartLine = token->m_ImplLine - 1;
                Ns.EndLine = token->m_ImplLineEnd - 1;
                Ns.Name = token->m_Name;
                funcdata->m_NameSpaces.push_back(Ns);
    //        Manager::Get()->GetLogManager()->DebugLog(_T("namespace ") + token->m_Name);
    //       wxString Log;
    //        Log.Printf(_("start %d and end %d"), token->m_ImplLine, token->m_ImplLineEnd);
    //        Manager::Get()->GetLogManager()->DebugLog(Log);
            }
        }
        // sort the vector
        sort(funcdata->m_FunctionsScope.begin(), funcdata->m_FunctionsScope.end(), LessFunctionScope);
        m_ToolbarChanged = true;
    }
    // *** Part 2: Fill the toolbar ***

    m_FunctionsScope = funcdata->m_FunctionsScope;
    m_NameSpaces = funcdata->m_NameSpaces;

    // Does the toolbar need a refresh?
    if(m_ToolbarChanged || m_LastFile!=filename)
    {

        // Update the last editor and changed flag...
        m_ToolbarChanged = false;
        m_LastFile = filename;
        // ...and refresh the toolbars.
        m_Function->Clear();
        m_Scope->Clear();

        // add to the choice controls
        for(unsigned int idxFn = 0; idxFn < m_FunctionsScope.size(); ++idxFn)
        {
            const FunctionScope fs = m_FunctionsScope[idxFn];
            m_Function->Append(fs.Name);
            m_Scope->Append(fs.Scope);
        } // end for : idx : idxFn
        // add namespaces to the scope
        StartIdxNameSpaceInScope = m_FunctionsScope.size();
        for(unsigned int idxNs = 0; idxNs < m_NameSpaces.size(); ++idxNs)
        {
            const NameSpace Ns = m_NameSpaces[idxNs];
            m_Scope->Append(Ns.Name);
        } // end for : idx : idxNs
    }

    // Finally, find the current function and update
    m_CurrentLine = ed->GetControl()->GetCurrentLine();
    int sel = FunctionPosition();
    if(sel != -1)
    {
        m_Function->SetSelection(sel);
        m_Scope->SetSelection(sel);
    }
    else
    {
        m_Function->SetSelection(wxNOT_FOUND);
        // TO DO : set scope correctly
        int NsSel = NameSpacePosition();
        if(NsSel != -1)
        {
            m_Scope->SetSelection(NsSel + StartIdxNameSpaceInScope);
        }
        else
        {
            m_Scope->SetSelection(wxNOT_FOUND);
        }
    }
} // end of ParseFunctionsAndFillToolbar

void CodeCompletion::OnEditorOpen(CodeBlocksEvent& event)
{
    if(!Manager::IsAppShuttingDown() && IsAttached() && m_InitDone)
    {
        cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinEditor(event.GetEditor());
        if(ed)
        {
            wxString filename = ed->GetFilename();
            // wxString s_tmplog = _T("CC: OnEditorOpen... Filename: ");
            // s_tmplog = s_tmplog + filename;
            // Manager::Get()->GetLogManager()->DebugLog(s_tmplog);
            FunctionsScopePerFile* funcdata = &(m_AllFunctionsScopes[filename]);
            funcdata->parsed = false;
        }
        if(!ProjectManager::IsBusy())
            m_FunctionsParsingTimer.Start(50, wxTIMER_ONE_SHOT);
    }
    event.Skip();
}

void CodeCompletion::OnEditorActivated(CodeBlocksEvent& event)
{
    if (!ProjectManager::IsBusy() && IsAttached() && m_InitDone)
    {
        EditorBase* eb = event.GetEditor();
        m_NativeParsers.OnEditorActivated(eb);
        ParseFunctionsAndFillToolbar();
    }

    event.Skip();
}

void CodeCompletion::OnEditorClosed(CodeBlocksEvent& event)
{
    EditorManager* edm = Manager::Get()->GetEditorManager();

    // we need to clear CC toolbar only when we are closing last editor
    // in other situations OnEditorActivated does this job
    if(edm->GetEditorsCount() == 1)
    {
        // clear toolbar when closing last editor
        m_Function->Clear();
        cbEditor* ed = edm->GetBuiltinEditor(event.GetEditor());
        wxString filename(wxEmptyString);
        if(ed)
            filename = ed->GetFilename();

        m_AllFunctionsScopes[filename].m_FunctionsScope.clear();
        m_AllFunctionsScopes[filename].m_NameSpaces.clear();
        m_AllFunctionsScopes[filename].parsed = false;
    }

    event.Skip();
}

void CodeCompletion::OnStartParsingFunctions(wxTimerEvent& event)
{
    if(!ProjectManager::IsBusy())
    {
        ParseFunctionsAndFillToolbar();
    }
}

void CodeCompletion::OnValueTooltip(CodeBlocksEvent& event)
{
    event.Skip();

    if (IsAttached() && m_InitDone)
    {
        if (!Manager::Get()->GetConfigManager(_T("code_completion"))->ReadBool(_T("eval_tooltip"), true))
            return;

        EditorBase* base = event.GetEditor();
        cbEditor* ed = base && base->IsBuiltinEditor() ? static_cast<cbEditor*>(base) : 0;
        if (!ed)
            return;

        if (ed->GetControl()->CallTipActive())
            ed->GetControl()->CallTipCancel();
//        Manager::Get()->GetLogManager()->DebugLog(F(_T("CodeCompletion::OnValueTooltip: %p"), ed));
        /* NOTE: The following 2 lines of codes can fix [Bug #11785].
        *       The solution may not the best one and it requires the editor
        *       to have the focus (even if C::B has the focus) in order to pop-up the tooltip. */
        if (wxWindow::FindFocus() != static_cast<wxWindow*>(ed->GetControl()))
            return;

        // ignore comments, strings, preprocesor, etc
        int style = event.GetInt();
        if (style != wxSCI_C_DEFAULT && style != wxSCI_C_OPERATOR && style != wxSCI_C_IDENTIFIER)
            return;

        int pos = ed->GetControl()->PositionFromPointClose(event.GetX(), event.GetY());
        if (pos < 0 || pos >= ed->GetControl()->GetLength())
            return;
        int endOfWord = ed->GetControl()->WordEndPosition(pos, true);

        Parser* parser = m_NativeParsers.FindParserFromEditor(ed);
        if (parser)
        {
            TokenIdxSet result;
            if (m_NativeParsers.MarkItemsByAI(result, true, true, true, endOfWord))
            {
                wxString msg;
                int count = 0;
                for (TokenIdxSet::iterator it = result.begin(); it != result.end(); ++it)
                {
                    Token* token = parser->GetTokens()->at(*it);
                    if (token)
                    {
                        msg << token->DisplayName() << _T("\n");
                        ++count;
                        if (count > 32) // allow max 32 matches (else something is definitely wrong)
                        {
                            msg.Clear();
                            break;
                        }
                    }
                }
                if (!msg.IsEmpty())
                {
                    msg.RemoveLast(); // last \n
                    ed->GetControl()->CallTipShow(pos, msg);
//                    Manager::Get()->GetLogManager()->DebugLog(F(msg));
                }
            }
        }
    }
}

void CodeCompletion::OnUpdateUI(wxUpdateUIEvent& event)
{
    bool hasEd = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor() != 0;
    if (m_EditMenu)
    {
        m_EditMenu->Enable(idMenuCodeComplete, hasEd);
        m_EditMenu->Enable(idMenuShowCallTip, hasEd);
    }

    if (m_SearchMenu)
    {
        m_SearchMenu->Enable(idMenuGotoFunction,     hasEd);
        m_SearchMenu->Enable(idMenuGotoPrevFunction, hasEd);
        m_SearchMenu->Enable(idMenuGotoNextFunction, hasEd);
    }

    if (m_ViewMenu)
    {
        bool isVis = IsWindowReallyShown((wxWindow*)m_NativeParsers.GetClassBrowser());
        m_ViewMenu->Check(idViewClassBrowser, isVis);
    }

    // must do...
    event.Skip();
}

void CodeCompletion::OnCodeComplete(wxCommandEvent& event)
{
    if (!Manager::Get()->GetConfigManager(_T("code_completion"))->ReadBool(_T("/use_code_completion"), true))
        return;
    if (IsAttached() && m_InitDone)
        DoCodeComplete();
    event.Skip();
}

void CodeCompletion::OnShowCallTip(wxCommandEvent& event)
{
    if (IsAttached() && m_InitDone)
        ShowCallTip();
    event.Skip();
}

void CodeCompletion::OnGotoFunction(wxCommandEvent& event)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if (!ed)
        return;

    Parser parser(this);
    parser.ParseBufferForFunctions(ed->GetControl()->GetText());

    wxArrayString funcs;
    TokensTree* tmptree = parser.GetTempTokens();

    if (!tmptree->size())
    {
        cbMessageBox(_("No functions parsed in this file..."));
        return;
    }
    wxArrayString tokens;
    SearchTree<Token*> tmpsearch;
    tokens.Clear();
    for(size_t i = 0; i < tmptree->size();i++)
    {
        Token* token = tmptree->at(i);
        if (token && (token->m_TokenKind == tkFunction || token->m_TokenKind == tkConstructor || token->m_TokenKind == tkDestructor))
        {
            tokens.Add(token->DisplayName());
            tmpsearch.AddItem(token->DisplayName(),token);
        }
    }
    IncrementalSelectListDlg dlg(Manager::Get()->GetAppWindow(), tokens, _("Select function..."), _("Please select function to go to:"));
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        wxString sel = dlg.GetStringSelection();
        Token* token = tmpsearch.GetItem(sel);
        if(token)
        {
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Token found at line %d"), token->m_Line));
            ed->GotoLine(token->m_Line - 1);
        }
    }
}

void CodeCompletion::OnGotoPrevFunction(wxCommandEvent& event)
{
    GotoFunctionPrevNext(); // prev function
}

void CodeCompletion::OnGotoNextFunction(wxCommandEvent& event)
{
    GotoFunctionPrevNext(true); // next function
}

void CodeCompletion::OnClassMethod(wxCommandEvent& event)
{
    DoClassMethodDeclImpl();
}

void CodeCompletion::OnUnimplementedClassMethods(wxCommandEvent& event)
{
    DoAllMethodsImpl();
}

void CodeCompletion::OnGotoDeclaration(wxCommandEvent& event)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();

    wxString txt = m_LastKeyword;
//    Manager::Get()->GetLogManager()->DebugLog(_T("Go to decl for '%s'"), txt.c_str());

    Parser* parser = m_NativeParsers.FindParserFromActiveEditor();
    if (!parser)
        parser = m_NativeParsers.FindParserFromActiveProject(); // get parser of active project, then
    if (!parser)
        return;

    // get the matching set
    Token* token = 0;
    TokenIdxSet result;
    parser->GetTokens()->FindMatches(txt, result, true, false);

    // one match
    if (result.size() == 1)
    {
        token = parser->GetTokens()->at(*(result.begin()));
    }
    // if more than one match, display a selection dialog
    else if (result.size() > 1)
    {
        // TODO: we could parse the line containing the text so
        // if namespaces were included, we could limit the results (and be more accurate)
        wxArrayString selections;
        wxArrayInt int_selections;
        for (TokenIdxSet::iterator it = result.begin(); it != result.end(); ++it)
        {
            Token* sel = parser->GetTokens()->at(*it);
            if (sel)
            {
                // only match tokens that have filename info
                if ((event.GetId() == idGotoImplementation && !sel->GetImplFilename().IsEmpty()) ||
                    (event.GetId() == idGotoDeclaration && !sel->GetFilename().IsEmpty()))
                {
                    selections.Add(sel->DisplayName());
                    int_selections.Add(*it);
                }
            }
        }
        if (selections.GetCount() > 1)
        {
            int sel = wxGetSingleChoiceIndex(_("Please make a selection:"), _("Multiple matches"), selections);
            if (sel == -1)
                return;
            token = parser->GetTokens()->at(int_selections[sel]);
        }
        else if (selections.GetCount() == 1)
        {    // number of selections can be < result.size() due to the if tests, so in case we fall
            // back on 1 entry no need to show a selection
            token = parser->GetTokens()->at(int_selections[0]);
        }
    }

    // do we have a token?
    if (token)
    {
        if(event.GetId() == idGotoImplementation)
        {
            cbEditor* ed = edMan->Open(token->GetImplFilename());
            if (ed)
                ed->GotoLine(token->m_ImplLine - 1);
            else
                cbMessageBox(wxString::Format(_("Implementation not found: %s"), txt.c_str()), _("Warning"), wxICON_WARNING);
        }
        else
        {
            cbEditor* ed = edMan->Open(token->GetFilename());
            if (ed)
                ed->GotoLine(token->m_Line - 1);
            else
                cbMessageBox(wxString::Format(_("Declaration not found: %s"), txt.c_str()), _("Warning"), wxICON_WARNING);
        }
    }
    else
        cbMessageBox(wxString::Format(_("Not found: %s"), txt.c_str()), _("Warning"), wxICON_WARNING);
}

void CodeCompletion::OnOpenIncludeFile(wxCommandEvent& event)
{
    Parser* parser = m_NativeParsers.FindParserFromActiveEditor();
    if (!parser)
    {
        parser = m_NativeParsers.FindParserFromActiveProject(); // get parser of active project, then
    }

    if (parser)
    {
        // search in all parser's include dirs
        wxString tmp;
        wxArrayString FoundSet = parser->FindFileInIncludeDirs(m_LastIncludeFile);
        if(FoundSet.GetCount() > static_cast<size_t>(1))
        {    // more then 1 hit : let the user choose
            SelectIncludeFile Dialog(Manager::Get()->GetAppWindow());
            Dialog.AddListEntries(FoundSet);
            PlaceWindow(&Dialog);
            if(Dialog.ShowModal() == wxID_OK)
            {
              tmp = Dialog.GetIncludeFile();
            }
            else
                return; // user cancelled the dialog...
        }
        else if(FoundSet.GetCount())
        {
            tmp = FoundSet[0];
        }

        if (!tmp.IsEmpty())
        {
            EditorManager* edMan = Manager::Get()->GetEditorManager();
            edMan->Open(tmp);
            return;
        }
    }

    // look in the same dir as the source file
    wxFileName fname = m_LastIncludeFile;
    fname.Assign(wxFileName(m_LastIncludeFileFrom).GetPath(wxPATH_GET_SEPARATOR) + m_LastIncludeFile);
    if (wxFileExists(fname.GetFullPath()))
    {
        EditorManager* edMan = Manager::Get()->GetEditorManager();
        edMan->Open(fname.GetFullPath());
        return;
    }

    cbMessageBox(wxString::Format(_("Not found: %s"), m_LastIncludeFile.c_str()), _("Warning"), wxICON_WARNING);
} // end of OnOpenIncludeFile

void CodeCompletion::EditorEventHook(cbEditor* editor, wxScintillaEvent& event)
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    if (!IsAttached() ||
        !m_InitDone ||
        !cfg->ReadBool(_T("/use_code_completion"), true))
    {
        event.Skip();
        return;
    }
    cbStyledTextCtrl* control = editor->GetControl();

//    if (event.GetEventType() == wxEVT_SCI_CHARADDED)
//        Manager::Get()->GetLogManager()->DebugLog(_T("wxEVT_SCI_CHARADDED"));
//    else if (event.GetEventType() == wxEVT_SCI_CHANGE)
//        Manager::Get()->GetLogManager()->DebugLog(_T("wxEVT_SCI_CHANGE"));
//    else if (event.GetEventType() == wxEVT_SCI_KEY)
//        Manager::Get()->GetLogManager()->DebugLog(_T("wxEVT_SCI_KEY"));
//    else if (event.GetEventType() == wxEVT_SCI_MODIFIED)
//        Manager::Get()->GetLogManager()->DebugLog(_T("wxEVT_SCI_MODIFIED"));

    if (event.GetEventType() == wxEVT_SCI_CHARADDED &&
        !control->AutoCompActive()) // not already active autocompletion
    {
        // a character was just added in the editor
        m_timerCodeCompletion.Stop();
        wxChar ch = event.GetKey();
        int pos = control->GetCurrentPos();
        int wordstart = control->WordStartPosition(pos, true);

        // if more than two chars have been typed, invoke CC
        int autoCCchars = cfg->ReadInt(_T("/auto_launch_chars"), 4);
        bool autoCC = cfg->ReadBool(_T("/auto_launch"), true) &&
                    pos - wordstart >= autoCCchars;

        // update calltip highlight while we type
        if (control->CallTipActive())
            ShowCallTip();

        // start calltip
        if (ch == _T('('))
        {
            if (control->CallTipActive())
                ++m_ActiveCalltipsNest;
            ShowCallTip();
        }

        // end calltip
        else if (ch == _T(')'))
        {
            // cancel any active calltip
            control->CallTipCancel();
            if (m_ActiveCalltipsNest > 0)
            {
                --m_ActiveCalltipsNest;
                ShowCallTip();
            }
        }

        else if (autoCC ||
            (ch == _T('"')) || // this and the next one are for #include's completion
            (ch == _T('<')) ||
            (ch == _T('.')) ||
            // we use -2 because the char has already been added and Pos is ahead of it...
            ((ch == _T('>')) && (control->GetCharAt(pos - 2) == _T('-'))) ||
            ((ch == _T(':')) && (control->GetCharAt(pos - 2) == _T(':'))))
        {
            int style = control->GetStyleAt(pos);
            //Manager::Get()->GetLogManager()->DebugLog(_T("Style at %d is %d (char '%c')"), pos, style, ch);
            if (ch == _T('"') || ch == _T('<'))
            {
                if (style != wxSCI_C_PREPROCESSOR)
                {
                    event.Skip();
                    return;
                }
            }
            else
            {
                if (style != wxSCI_C_DEFAULT && style != wxSCI_C_OPERATOR && style != wxSCI_C_IDENTIFIER)
                {
                    event.Skip();
                    return;
                }
            }

            int timerDelay = cfg->ReadInt(_T("/cc_delay"), 500);
            if (autoCC || timerDelay == 0)
            {
                if (autoCC)
                    m_IsAutoPopup = true;
                DoCodeComplete();
                if (autoCC)
                    m_IsAutoPopup = false;
            }
            else
            {
                m_LastPosForCodeCompletion = pos;
                m_pCodeCompletionLastEditor = editor;
                m_timerCodeCompletion.Start(timerDelay, wxTIMER_ONE_SHOT);
            }
        }
    }

    if( control->GetCurrentLine() != m_CurrentLine)
    {
        m_CurrentLine = control->GetCurrentLine();
        int sel = FunctionPosition();
        if(sel != -1 && sel != m_Function->GetSelection())
        {
            m_Function->SetSelection(sel);
            m_Scope->SetSelection(sel);
        }
        else if(sel == -1)
        {
            m_Function->SetSelection(wxNOT_FOUND);
            // TO DO : set scope correctly
            int NsSel = NameSpacePosition();
            if(NsSel != -1)
            {
                m_Scope->SetSelection(NsSel + StartIdxNameSpaceInScope);
            }
            else
            {
                m_Scope->SetSelection(wxNOT_FOUND);
            }
        }
    }

    // allow others to handle this event
    event.Skip();
}

void CodeCompletion::OnFunction(wxCommandEvent& /*event*/)
{
    int sel = m_Function->GetSelection();
    if(sel != -1 && sel < static_cast<int>(m_FunctionsScope.size()))
    {
        int Line = m_FunctionsScope[sel].StartLine;
        EditorManager* edMan = Manager::Get()->GetEditorManager();
        cbEditor* ed = edMan->GetBuiltinActiveEditor();
        if (!ed)
            return;
        ed->GotoLine(Line);
        m_Scope->SetSelection(sel);
        ed->SetFocus();
    }
}

void CodeCompletion::OnParserEnd(wxCommandEvent& event)
{
    // nothing for now
}
