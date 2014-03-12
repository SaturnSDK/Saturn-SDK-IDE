/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef NATIVEPARSER_H
#define NATIVEPARSER_H

#include "nativeparser_base.h"
#include "parser/parser.h"

#include <queue>
#include <map>

#include <wx/event.h>
#include <wx/hashmap.h> // TODO: replace with std::map

/** debug only variable, used to print the AI match related log message*/
extern bool s_DebugSmartSense;

extern const int g_EditorActivatedDelay;

// forward declaration
class cbEditor;
class EditorBase;
class cbProject;
class cbStyledTextCtrl;
class ClassBrowser;
class Compiler;
class Token;

WX_DECLARE_HASH_MAP(cbProject*, ParserBase*, wxPointerHash, wxPointerEqual, ParsersMap);
WX_DECLARE_HASH_MAP(cbProject*, wxString, wxPointerHash, wxPointerEqual, ParsersFilenameMap);

typedef std::map<cbProject*, wxArrayString> ProjectSearchDirsMap;

// TODO (ollydbg#1#), this class is dirty, I'm going to change its name like CursorLocation
/** Search location combination, a pointer to cbStyledTextCtrl and a filename is enough */
struct ccSearchData
{
    cbStyledTextCtrl* control;
    wxString          file;
};

/** Symbol browser tree showing option */
enum BrowserViewMode
{
    bvmRaw = 0,
    bvmInheritance
};

/** @brief NativeParser class is just like a manager class to control Parser objects.
 *
 * Normally, Each C::B Project (cbp) will have an associated Parser object.
 * In another mode, all C::B project belong to a C::B workspace share a single Parser object.
 * Nativeparser will manage all the Parser objects.
 */
class NativeParser : public wxEvtHandler, NativeParserBase
{
public:
    /** Constructor */
    NativeParser();

    /** Destructor */
    ~NativeParser();

    /** return a reference to the currently active Parser object */
    ParserBase& GetParser() { return *m_Parser; }

    /** return the Parser pointer corresponding to the input C::B project
     * @param project input C::B project pointer
     * @return a pointer to parser object
     */
    ParserBase* GetParserByProject(cbProject* project);

    /** return the Parser pointer associated with the input file
     * Internally this function first find the project containing the input file,
     * then return the Parser pointer by the project.
     * @param filename filename with full path.
     * @return Parser pointer
     */
    ParserBase* GetParserByFilename(const wxString& filename);

    /** return the C::B project associated with Parser pointer
     * @param parser Parser pointer
     * @return C::B Project pointer
     */
    cbProject* GetProjectByParser(ParserBase* parser);

    /** return the C::B project containing the filename
     * The function first try to match the filename in the active project, next to match other
     * projects opened, If the file exists in several projects, the first matched project will be returned.
     * @param filename input filename
     * @return project pointer containing the file
     */
    cbProject* GetProjectByFilename(const wxString& filename);

    /** return the C::B project containing the cbEditor pointer
     * @param editor Any valid cbEditor pointer
     * @return project pointer
     */
    cbProject* GetProjectByEditor(cbEditor* editor);

    /** Get current project by active editor or just return active project */
    cbProject* GetCurrentProject();

    /** Return true if use one Parser per whole workspace */
    bool IsParserPerWorkspace() const { return m_ParserPerWorkspace; }

    /** Return true if all the parser's batch-parse stages are finished, otherwise return false*/
    bool Done();

    /** Used to support Symbol browser and codecompletion UI
     *  Image list is used to initialize the symbol browser tree node image.
     */
    wxImageList* GetImageList() { return m_ImageList; }

    /** Returns the image assigned to a specific token for a symbol browser
     */
    int GetTokenKindImage(const Token* token);

    /** Get the implementation file path if the input is a header file. or Get the header file path
     * if the input is an implementation file.
     * Both the implementation file and header file can be in different directories.
     * @param filename input filename
     * @return corresponding file paths, in wxArrayString format
     */
    wxArrayString GetAllPathsByFilename(const wxString& filename);

    /** Add the paths to path array, and this will be used in GetAllPathsByFilename() function.
     *  internally, all the folder paths were recorded in UNIX format.
     */
    void AddPaths(wxArrayString& dirs, const wxString& path, bool hasExt);

    // the functions below are handling and managing Parser object

    /** Dynamically allocate a Parser object for the input C::B project
     * @param project C::B project
     * @return Parser pointer of the project.
     */
    ParserBase* CreateParser(cbProject* project);

    /** delete the Parser object for the input project
     * @param project C::B project.
     * @return true if success.
     */
    bool DeleteParser(cbProject* project);

    /** Single file re-parse.
     * This was happening when you add a single file to project, or a file was modified.
     * @param project C::B project
     * @param filename filename with full patch in the C::B project
     */
    bool ReparseFile(cbProject* project, const wxString& filename);

    /** New file was added to the C::B project, so this will cause a re-parse on the new added file.
     * @param project C::B project
     * @param filename filename with full path in the C::B project
     */
    bool AddFileToParser(cbProject* project, const wxString& filename, ParserBase* parser = nullptr);

    /** remove a file from C::B project and Parser
     * @param project C::B project
     * @param filename filename with full patch in the C::B project
     */
    bool RemoveFileFromParser(cbProject* project, const wxString& filename);

    /** when user changes the CC option, we should re-read the option */
    void RereadParserOptions();

    /** re-parse the active Parser (the project associated with m_Parser member variable */
    void ReparseCurrentProject();

    /** re-parse the project select by context menu in projects management panel */
    void ReparseSelectedProject();

    /** collect tokens where a code suggestion list can be shown
     * @param searchData input variable, search location
     * @param result output variable, containing all matching result token indexes
     * @param reallyUseAI true means the context scope information should be considered,
     *        false if only do a plain word match
     * @param isPrefix partially match which result all the Tokens' name with the same prefix,
              otherwise use full-text match
     * @param caseSensitive case sensitive or not
     * @param caretPos Where the current caret locates, -1 means we use the current caret position.
     * @return the matching Token count
     */
    size_t MarkItemsByAI(ccSearchData* searchData, TokenIdxSet& result, bool reallyUseAI = true,
                         bool isPrefix = true, bool caseSensitive = false, int caretPos = -1);

    /** the same as before, but we don't specify the searchData information, so it will use the active
     *  editor and current caret information.
     */
    size_t MarkItemsByAI(TokenIdxSet& result, bool reallyUseAI = true, bool isPrefix = true,
                         bool caseSensitive = false, int caretPos = -1);

    /** Call tips are when you mouse pointer hover some statement and show the information of statement below caret.
     * these tips information could be:
     * the prototypes information of the current function,
     * the type information of the variable...
     *
     * @param items array to store result in.
     * @param typedCommas how much comma characters the user has typed in the current line before the cursor.
     * @return position index of the start of the arguments
     */
    int GetCallTips(wxArrayString& items, int& typedCommas, int pos = wxNOT_FOUND);

    /** Word start position in the editor
     * @return position index
     */
    int GetEditorStartWord() const { return m_EditorStartWord; }

    /** Word end position in the editor
     * @return  position index
     */
    int GetEditorEndWord() const { return m_EditorEndWord; }

    /** project search path is used for auto completion for #include <> */
    wxArrayString& GetProjectSearchDirs(cbProject* project);

    // The function below is used to manage symbols browser
    /** return active class browser pointer*/
    ClassBrowser* GetClassBrowser() const { return m_ClassBrowser; }

    /** create the class browser */
    void CreateClassBrowser();

    /** remove the class browser */
    void RemoveClassBrowser(bool appShutDown = false);

    /** update the class browser tree*/
    void UpdateClassBrowser();

protected:
    /** When a Parser is created, we need a full parsing stage including:
     * 1, parse the priority header files firstly.
     * 2, parse all the other project files.
     */
    bool DoFullParsing(cbProject* project, ParserBase* parser);

    /** Switch parser object according the current active editor and filename */
    bool SwitchParser(cbProject* project, ParserBase* parser);

    /** Set a new Parser as the active Parser
     * Set the active parser pointer (m_Parser member variable)
     * update the ClassBrowser's Parser pointer
     * re-fresh the symbol browser tree.
     */
    void SetParser(ParserBase* parser);

    /** Clear all Parser object*/
    void ClearParsers();

    /** Remove all the obsolete Parser object
     * if the number exceeds the limited number (can be set in the CC's option), then all the
     * obsolete parser will be removed.
     */
    void RemoveObsoleteParsers();

    /** Get cbProject and Parser pointer, according to the current active editor*/
    std::pair<cbProject*, ParserBase*> GetParserInfoByCurrentEditor();

    /** Used to support Symbol browser and codecompletion UI
     *  Image list is used to initialize the symbol browser tree node image.
     */
    void SetTokenKindImage(int kind, const wxBitmap& bitmap, const wxBitmap& mask = wxNullBitmap);
    void SetTokenKindImage(int kind, const wxBitmap& bitmap, const wxColour& maskColour);
    void SetTokenKindImage(int kind, const wxIcon& icon);

    /** set the class browser view mode*/
    void SetCBViewMode(const BrowserViewMode& mode);

private:
    friend class CodeCompletion;

    /** Read project CC options when a C::B project is loading */
    void OnProjectLoadingHook(cbProject* project, TiXmlElement* elem, bool loading);

    /** Start an Artificial Intelligence search algorithm to gather all the matching tokens.
     * The actual AI is in FindAIMatches() below.
     * @param result output parameter.
     * @param searchData cbEditor information.
     * @param lineText current statement.
     * @param isPrefix if true, then the result contains all the tokens whose name is a prefix of current lineText.
     * @param caseSensitive true is case sensitive is enabled on the match.
     * @param search_scope it is the "parent token" where we match the "search-key".
     * @param caretPos use current caret position if it is -1.
     * @return match token number
     */
    size_t AI(TokenIdxSet& result,
              ccSearchData* searchData,
              const wxString& lineText = wxEmptyString,
              bool isPrefix = false,
              bool caseSensitive = false,
              TokenIdxSet* search_scope = 0,
              int caretPos = -1);

    /** return all the tokens matching the current function(hopefully, just one)
     * @param editor editor pointer
     * @param result output result containing all the Token index
     * @param caretPos -1 if the current caret position is used.
     * @return number of result Tokens
     */
    size_t FindCurrentFunctionToken(ccSearchData* searchData, TokenIdxSet& result, int caretPos = -1);

    /** returns the editor's position where the current function scope starts.
     * optionally, returns the function's namespace (ends in double-colon ::) and name and token
     * @param searchData search data struct pointer
     * @param nameSpace get the namespace modifier
     * @param procName get the function name
     * @param functionToken get the token of current function
     * @param caretPos caret position in cbeditor
     * @return current function line number
     */
    int FindCurrentFunctionStart(ccSearchData* searchData,
                                 wxString*     nameSpace = 0L,
                                 wxString*     procName = 0L,
                                 int*          functionIndex = 0L,
                                 int           caretPos = -1);

    /** helper function for statement parsing*/
    bool SkipWhitespaceForward(cbEditor* editor, int& pos);

    /** helper function for statement parsing*/
    bool SkipWhitespaceBackward(cbEditor* editor, int& pos);

    /** used in CodeCompletion suggestion list to boost the performance, we use a caches*/
    bool LastAISearchWasGlobal() const { return m_LastAISearchWasGlobal; }

    /** The same as above*/
    const wxString& LastAIGlobalSearch() const { return m_LastAIGlobalSearch; }

    /** collect the using namespace directive in the editor specified by searchData
     * @param searchData search location
     * @param search_scope resulting tokens collection
     * @param caretPos caret position, if not specified, we use the current caret position
     */
    bool ParseUsingNamespace(ccSearchData* searchData, TokenIdxSet& search_scope, int caretPos = -1);

    /** collect the using namespace directive in the buffer specified by searchData
     * @param buffer code to parse
     * @param search_scope resulting tokens collection
     * @param bufferSkipBlocks skip brace sets { }
     */
    bool ParseBufferForUsingNamespace(const wxString& buffer, TokenIdxSet& search_scope, bool bufferSkipBlocks = true);

    /** collect function argument, add them to the token tree (as temporary tokens)
     * @param searchData search location
     * @param caretPos caret position, if not specified, we use the current caret position
     */
    bool ParseFunctionArguments(ccSearchData* searchData, int caretPos = -1);

    /** parses from the start of function up to the cursor, this is used to collect local variables.
     * @param searchData search location
     * @param search_scope resulting tokens collection of local using namespace
     * @param caretPos caret position, if not specified, we use the current caret position
     */
    bool ParseLocalBlock(ccSearchData* searchData, TokenIdxSet& search_scope, int caretPos = -1);

    /** collect the header file search directories, those dirs include:
     *  1, project's base dir, e.g. if you cbp file was c:/bbb/aaa.cbp, then c:/bbb is added.
     *  2, project's setting search dirs, for a wx project, then c:/wxWidgets2.8.12/include is added.
     *  3, a project may has some targets, so add search dirs for those targets
     *  4, compiler's own search path, like: c:/mingw/include
     */
    bool AddCompilerDirs(cbProject* project, ParserBase* parser);

    /** collect compiler specific predefined preprocessor definition, this is usually run a special
     * compiler command, like GCC -dM for gcc.
     * @return true if there are some macro definition added, else it is false
     */
    bool AddCompilerPredefinedMacros(cbProject* project, ParserBase* parser);

    /** collect GCC compiler predefined preprocessor definition */
    bool AddCompilerPredefinedMacrosGCC(const wxString& compilerId, cbProject* project, wxString& defs);

    /** lookup GCC compiler -std=XXX option */
    wxString GetCompilerStandardGCC(Compiler* compiler, cbProject* project);

    /** lookup GCC compiler -std=XXX option for specific GCC options*/
    wxString GetCompilerUsingStandardGCC(const wxArrayString& compilerOptions);

    /** collect VC compiler predefined preprocessor definition */
    bool AddCompilerPredefinedMacrosVC(const wxString& compilerId, wxString& defs);

    /** collect project (user) defined preprocessor definition, such as for wxWidgets project, the
     * macro may have "#define wxUSE_UNICODE" defined in its project file.
     * @return true if there are some macro definition added, else it is false
     */
    bool AddProjectDefinedMacros(cbProject* project, ParserBase* parser);

    /** Collect the default compiler include file search paths. called by AddCompilerDirs() function*/
    const wxArrayString& GetGCCCompilerDirs(const wxString &cpp_compiler);

    /** Add the collected default GCC compiler include file search paths to a parser */
    void AddGCCCompilerDirs(const wxString& masterPath, const wxString& compilerCpp, ParserBase* parser);

    /** Add a list of directories to the parser's search directories, normalise to "base" path, if "base" is not empty. Replaces macros. */
    void AddIncludeDirsToParser(const wxArrayString& dirs, const wxString& base, ParserBase* parser);

    /** Event handler when the batch parse starts, print some log information */
    void OnParserStart(wxCommandEvent& event);

    /** Event handler when the batch parse finishes, print some log information, check  whether the active editor
     * belong to the current parser, if not, do a parser switch */
    void OnParserEnd(wxCommandEvent& event);

    /** If use one parser per whole workspace, we need parse all project one by one */
    void OnParsingOneByOneTimer(wxTimerEvent& event);

    /** Event handler when an editor activate, *NONE* project is handled here */
    void OnEditorActivated(EditorBase* editor);

    /** Event handler when an editor closed, if it is the last editor belong to *NONE* project, then
     *  the *NONE* Parser will be removed
     */
    void OnEditorClosed(EditorBase* editor);

    /** Init cc search member variables */
    void InitCCSearchVariables();

    /** Add one project to the common parser in one parser for the whole workspace mode
     * @return true means there are some thing (macro and files) need to parse, otherwise it is false
     */
    bool AddProjectToParser(cbProject* project);

    /** Remove cbp from the common parser, this only happens in one parser for whole workspace mode
     * when a parser is removed from the workspace, we should remove the project from the parser
     */
    bool RemoveProjectFromParser(cbProject* project);

private:
    typedef std::pair<cbProject*, ParserBase*> ProjectParserPair;
    typedef std::list<ProjectParserPair>       ParserList;

    /** a list holing all the cbp->parser pairs, if in one parser per project mode, there are many
     * many pairs in this list. In one parser per workspace mode, there is only one pair, and the
     * m_ParserList.begin()->second is the common parser for all the projects in workspace.
     */
    ParserList                   m_ParserList;
    /** a temp parser object pointer*/
    ParserBase*                  m_TempParser;
    /** active parser object pointer*/
    ParserBase*                  m_Parser;

    /** a delay timer to parser every project in sequence*/
    wxTimer                      m_TimerParsingOneByOne;
    /** symbol browser window*/
    ClassBrowser*                m_ClassBrowser;
    /** if true, which means m_ClassBrowser is floating (not docked)*/
    bool                         m_ClassBrowserIsFloating;

    /** a map: project pointer -> C/C++ parser search paths for this project */
    ProjectSearchDirsMap         m_ProjectSearchDirsMap;
    int                          m_HookId;    //!< project loader hook ID
    wxImageList*                 m_ImageList; //!< Images for class browser

    /// all the files which opened, but does not belong to any cbp
    wxArrayString                m_StandaloneFiles;
    /**  if true, which means the parser hold tokens of the whole workspace's project, if false
     * then one parser per a cbp
     */
    bool                         m_ParserPerWorkspace;
    /// only used when m_ParserPerWorkspace is true, and holds all the cbps for the common parser
    std::set<cbProject*>         m_ParsedProjects;

    /* CC Search Member Variables => START */
    int               m_EditorStartWord;
    int               m_EditorEndWord;
    wxString          m_LastAIGlobalSearch;    //!< same case like above, it holds the search string
    bool              m_LastAISearchWasGlobal; //!< true if the phrase for code-completion is empty or partial text (i.e. no . -> or :: operators)
    cbStyledTextCtrl* m_LastControl;
    wxString          m_LastFile;
    int               m_LastFunctionIndex;
    int               m_LastFuncTokenIdx;      //!< saved the function token's index, for remove all local variable
    int               m_LastLine;
    wxString          m_LastNamespace;
    wxString          m_LastPROC;
    int               m_LastResult;
    /* CC Search Member Variables => END */
};

#endif // NATIVEPARSER_H

