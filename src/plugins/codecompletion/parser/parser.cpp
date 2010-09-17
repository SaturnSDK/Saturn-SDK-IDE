/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include <wx/app.h>
#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <wx/intl.h>
#include <wx/progdlg.h>
#include "parser.h"

#include "../classbrowser.h"
#include "../classbrowserbuilderthread.h"

#include <configmanager.h>
#include <logmanager.h>
#include <editormanager.h>
#include <manager.h>
#include <globals.h>
#include <infowindow.h>
#include <cbproject.h>

#include <queue>

#ifndef CB_PRECOMP
    #include "editorbase.h"
#endif

#define CC_PARSER_DEBUG_OUTPUT 0

#if CC_PARSER_DEBUG_OUTPUT
    #define TRACE(format, args...)\
    Manager::Get()->GetLogManager()->DebugLog(F( format , ## args))
#else
    #define TRACE(format, args...)
#endif

static const char CACHE_MAGIC[] = "CCCACHE_1_3";
static const int batch_timer_delay = 300;
static const int reparse_timer_delay = 100;

int PARSER_START = wxNewId();
int PARSER_END = wxNewId();
int TIMER_ID = wxNewId();
int BATCH_TIMER_ID = wxNewId();

static Parser* s_CurrentParser = 0;
static wxCriticalSection s_ParseCritical;

BEGIN_EVENT_TABLE(Parser, wxEvtHandler)
END_EVENT_TABLE()

class AddParseThread : public cbThreadedTask
{
public:
    AddParseThread(Parser* parser) : m_Parent(parser)
    {}

    int Execute()
    {
        // Add up-front headers
        m_Parent->m_IsUpFront = true;
        for (size_t i = 0; !TestDestroy() && i < m_Parent->m_UpFrontHeaders.GetCount(); ++i)
            m_Parent->Parse(m_Parent->m_UpFrontHeaders[i]);
        m_Parent->m_IsUpFront = false;

        // Add all other files
        for (size_t i = 0; !TestDestroy() && i < m_Parent->m_BatchParseFiles.GetCount(); ++i)
            m_Parent->Parse(m_Parent->m_BatchParseFiles[i]);

        {
            wxCriticalSectionLocker locker(s_ParseCritical);
            m_Parent->m_UpFrontHeaders.Clear();
            m_Parent->m_BatchParseFiles.Clear();
            m_Parent->m_IsParsing = true;
        }

        return 0;
    }

private:
    Parser* m_Parent;
};

Parser::Parser(wxEvtHandler* parent) :
    m_pParent(parent),
    m_UsingCache(false),
    m_Pool(this, wxNewId(), 1), // in the meanwhile it'll have to be forced to 1
    m_IsUpFront(false),
    m_NeedsReparse(false),
    m_IsFirstBatch(false),
    m_IsParsing(false),
    m_Timer(this, TIMER_ID),
    m_BatchTimer(this, BATCH_TIMER_ID),
    m_StopWatchRunning(false),
    m_LastStopWatchTime(0),
    m_IgnoreThreadEvents(false),
    m_IsBatchParseDone(false),
    m_ParsingType(ptCreateParser)
{
    m_pTokensTree = new(std::nothrow) TokensTree;
    m_pTempTokensTree = new(std::nothrow) TokensTree;
    ReadOptions();
    ConnectEvents();
}

Parser::~Parser()
{
    // 1. Lock tokens tree
    wxCriticalSectionLocker locker(s_TokensTreeCritical);

    // 2. Process event
    Manager::ProcessPendingEvents();

    // 3. Disconnect events
    DisconnectEvents();

    // 4. Let's OnAllThreadsDone can not process event
    m_IgnoreThreadEvents = true;

    // 5. Abort all thread
    TerminateAllThreads();

    // 6. Reset current parser
    if (s_CurrentParser == this)
        s_CurrentParser = NULL;

    // 7. Free memory
    delete m_pTempTokensTree;
    delete m_pTokensTree;
}

void Parser::ConnectEvents()
{
    Connect(-1, -1, cbEVT_THREADTASK_ALLDONE,
            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
            &Parser::OnAllThreadsDone);
    Connect(TIMER_ID, -1, wxEVT_TIMER,
            (wxObjectEventFunction)(wxEventFunction)(wxTimerEventFunction)
            &Parser::OnTimer);
    Connect(BATCH_TIMER_ID, -1, wxEVT_TIMER,
            (wxObjectEventFunction)(wxEventFunction)(wxTimerEventFunction)
            &Parser::OnBatchTimer);
}

void Parser::DisconnectEvents()
{
    Disconnect(-1, BATCH_TIMER_ID, wxEVT_TIMER);
    Disconnect(-1, TIMER_ID, wxEVT_TIMER);
    Disconnect(-1, -1, cbEVT_THREADTASK_ALLDONE);
}

void Parser::ReadOptions()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    // one-time default settings change: upgrade everyone
    bool force_all_on = !cfg->ReadBool(_T("/parser_defaults_changed"), false);
    if (force_all_on)
    {
        cfg->Write(_T("/parser_defaults_changed"), (bool)true);

        cfg->Write(_T("/parser_follow_local_includes"), true);
        cfg->Write(_T("/parser_follow_global_includes"), true);
        cfg->Write(_T("/want_preprocessor"), true);
    }

    //m_Pool.SetConcurrentThreads(cfg->ReadInt(_T("/max_threads"), 1)); // Ignore it in the meanwhile

    m_Options.followLocalIncludes  = cfg->ReadBool(_T("/parser_follow_local_includes"), true);
    m_Options.followGlobalIncludes = cfg->ReadBool(_T("/parser_follow_global_includes"), true);
    m_Options.caseSensitive        = cfg->ReadBool(_T("/case_sensitive"), false);
    m_Options.useSmartSense        = cfg->ReadBool(_T("/use_SmartSense"), true);
    m_Options.whileTyping          = cfg->ReadBool(_T("/while_typing"), true);
    m_Options.wantPreprocessor     = cfg->ReadBool(_T("/want_preprocessor"), true);

    m_BrowserOptions.showInheritance = cfg->ReadBool(_T("/browser_show_inheritance"), false);
    m_BrowserOptions.expandNS        = cfg->ReadBool(_T("/browser_expand_ns"), false);
    m_BrowserOptions.treeMembers     = cfg->ReadBool(_T("/browser_tree_members"), true);
    m_BrowserOptions.displayFilter   = (BrowserDisplayFilter)cfg->ReadInt(_T("/browser_display_filter"), bdfFile);
    m_BrowserOptions.sortType        = (BrowserSortType)cfg->ReadInt(_T("/browser_sort_type"), bstKind);
}

void Parser::WriteOptions()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    cfg->Write(_T("/max_threads"),                   (int)GetMaxThreads());
    cfg->Write(_T("/parser_follow_local_includes"),  m_Options.followLocalIncludes);
    cfg->Write(_T("/parser_follow_global_includes"), m_Options.followGlobalIncludes);
    cfg->Write(_T("/case_sensitive"),                m_Options.caseSensitive);
    cfg->Write(_T("/use_SmartSense"),                m_Options.useSmartSense);
    cfg->Write(_T("/while_typing"),                  m_Options.whileTyping);
    cfg->Write(_T("/want_preprocessor"),             m_Options.wantPreprocessor);

    cfg->Write(_T("/browser_show_inheritance"),      m_BrowserOptions.showInheritance);
    cfg->Write(_T("/browser_expand_ns"),             m_BrowserOptions.expandNS);
    cfg->Write(_T("/browser_tree_members"),          m_BrowserOptions.treeMembers);
    cfg->Write(_T("/browser_display_filter"),        m_BrowserOptions.displayFilter);
    cfg->Write(_T("/browser_sort_type"),             m_BrowserOptions.sortType);
}

bool Parser::CacheNeedsUpdate()
{
    if (m_UsingCache)
    {
        wxCriticalSectionLocker locker(s_TokensTreeCritical);
        return m_pTokensTree->m_Modified;
    }
    return true;
}

unsigned int Parser::GetFilesCount()
{
    wxCriticalSectionLocker locker(s_TokensTreeCritical);
    return m_pTokensTree->m_FilesMap.size();
}

bool Parser::Done()
{
    wxCriticalSectionLocker locker(s_ParseCritical);
    bool done =    m_UpFrontHeaders.IsEmpty()
                && m_SystemUpFrontHeaders.IsEmpty()
                && m_BatchParseFiles.IsEmpty()
                && m_PoolTask.empty()
                && m_Pool.Done();
    return done;
}

Token* Parser::FindTokenByName(const wxString& name, bool globalsOnly, short int kindMask)
{
    wxCriticalSectionLocker locker(s_TokensTreeCritical);
    int result = m_pTokensTree->TokenExists(name, -1, kindMask);
    return m_pTokensTree->at(result);
}

Token* Parser::FindChildTokenByName(Token* parent, const wxString& name, bool useInheritance, short int kindMask)
{
    if (!parent)
        return FindTokenByName(name, false, kindMask);

    Token* result = 0;
    {
        wxCriticalSectionLocker locker(s_TokensTreeCritical);
        result = m_pTokensTree->at(m_pTokensTree->TokenExists(name, parent->GetSelf(), kindMask));
    }

    if (!result && useInheritance)
    {
        // no reason for a critical section here:
        // it will only recurse to itself.
        // the critical section above is sufficient
        TokenIdxSet::iterator it;
        m_pTokensTree->RecalcInheritanceChain(parent);
        for (it = parent->m_DirectAncestors.begin(); it != parent->m_DirectAncestors.end(); ++it)
        {
            Token* ancestor = m_pTokensTree->at(*it);
            result = FindChildTokenByName(ancestor, name, true, kindMask);
            if (result)
                break;
        }
    }
    return result;
}

size_t Parser::FindMatches(const wxString& s, TokenList& result, bool caseSensitive, bool is_prefix)
{
    result.clear();
    TokenIdxSet tmpresult;
    wxCriticalSectionLocker locker(s_TokensTreeCritical);
    if (!m_pTokensTree->FindMatches(s, tmpresult, caseSensitive, is_prefix))
        return 0;

    TokenIdxSet::iterator it;
    for (it = tmpresult.begin(); it != tmpresult.end(); ++it)
    {
        Token* token = m_pTokensTree->at(*it);
        if (token)
        result.push_back(token);
    }
    return result.size();
}

size_t Parser::FindMatches(const wxString& s, TokenIdxSet& result, bool caseSensitive, bool is_prefix)
{
    result.clear();
    TokenIdxSet tmpresult;
    wxCriticalSectionLocker locker(s_TokensTreeCritical);
    if (!m_pTokensTree->FindMatches(s, tmpresult, caseSensitive, is_prefix))
        return 0;

    TokenIdxSet::iterator it;
    for (it = tmpresult.begin(); it != tmpresult.end(); ++it)
    {
        Token* token = m_pTokensTree->at(*it);
        if (token)
        //result.push_back(token);
        result.insert(*it);
    }
    return result.size();
}

void Parser::LinkInheritance(bool tempsOnly)
{
    wxCriticalSectionLocker locker(s_TokensTreeCritical);
    (tempsOnly ? m_pTempTokensTree :  m_pTokensTree)->RecalcData();
}

void Parser::MarkFileTokensAsLocal(const wxString& filename, bool local, void* userData)
{
    wxCriticalSectionLocker locker(s_TokensTreeCritical);
    m_pTokensTree->MarkFileTokensAsLocal(filename, local, userData);
}

bool Parser::ParseBuffer(const wxString& buffer, bool isLocal, bool bufferSkipBlocks, bool isTemp)
{
    ParserThreadOptions opts;
    opts.wantPreprocessor     = m_Options.wantPreprocessor;
    opts.followLocalIncludes  = m_Options.followLocalIncludes;
    opts.followGlobalIncludes = m_Options.followGlobalIncludes;
    opts.useBuffer            = true;
    opts.isTemp               = isTemp;
    opts.bufferSkipBlocks     = bufferSkipBlocks;
    opts.handleFunctions      = false;

    return Parse(buffer, isLocal, opts);
}

void Parser::PrepareParsing()
{
    m_IsFirstBatch = true;
}

void Parser::AddUpFrontHeaders(const wxString& filename, bool systemHeaderFile)
{
    wxCriticalSectionLocker locker(s_ParseCritical);

    // Do up-front parse in sub thread
    m_UpFrontHeaders.Add(filename);

    // Save system up-front headers, when all task is over, we need reparse it!
    if (systemHeaderFile)
        m_SystemUpFrontHeaders.Add(filename);
}

void Parser::AddBatchParse(const wxArrayString& filenames)
{
    wxCriticalSectionLocker locker(s_ParseCritical);

    if (m_BatchParseFiles.IsEmpty())
        m_BatchParseFiles = filenames;
    else
    {
        m_BatchParseFiles.Alloc(m_BatchParseFiles.GetCount() + filenames.GetCount());
        for (size_t i = 0; i < filenames.GetCount(); ++i)
            m_BatchParseFiles.Add(filenames[i]);
    }
}

void Parser::AddParse(const wxString& filename)
{
    wxCriticalSectionLocker locker(s_ParseCritical);
    m_BatchParseFiles.Add(filename);
}

void Parser::StartParsing(bool delay)
{
    // Allow future parses to take place in this same run
    if (!m_IsParsing)
        m_BatchTimer.Start(delay ? batch_timer_delay : 1, wxTIMER_ONE_SHOT);
    else
        Manager::Get()->GetLogManager()->DebugLog(_T("Start parsing failed!"));
}

bool Parser::Parse(const wxString& filename, bool isLocal, LoaderBase* loader)
{
    ParserThreadOptions opts;
    opts.wantPreprocessor      = m_Options.wantPreprocessor;
    opts.useBuffer             = false;
    opts.bufferSkipBlocks      = false;
    opts.bufferSkipOuterBlocks = false;
    opts.followLocalIncludes   = m_Options.followLocalIncludes;
    opts.followGlobalIncludes  = m_Options.followGlobalIncludes;
    opts.loader                = loader; // maybe 0 at this point

    return Parse(UnixFilename(filename), isLocal, opts);
}

bool Parser::Parse(const wxString& bufferOrFilename, bool isLocal, ParserThreadOptions& opts)
{
    bool result = false;
    do
    {
        if (!opts.useBuffer)
        {
            wxCriticalSectionLocker locker(s_TokensTreeCritical);

            bool canparse = !m_pTokensTree->IsFileParsed(bufferOrFilename);
            if (canparse)
                canparse = m_pTokensTree->ReserveFileForParsing(bufferOrFilename, true) != 0;

            if (!canparse)
            {
               if (opts.loader) // if a loader is already open at this point, the caller must clean it up
                   Manager::Get()->GetLogManager()->DebugLog(_T("Parse() : CodeCompletion Plugin: FileLoader memory leak likely while loading file ")+bufferOrFilename);
               break;
            }

            if (!opts.loader) // this should always be true (memory will leak if a loader has already been initialized before this point)
                opts.loader = Manager::Get()->GetFileManager()->Load(bufferOrFilename, m_NeedsReparse);
        }

        TRACE(_T("Parse() : Creating task for: %s"), bufferOrFilename.wx_str());
        ParserThread* thread = new(std::nothrow) ParserThread(this, bufferOrFilename, isLocal, opts, m_pTokensTree);
        if (!thread)
            break;

        bool doParseNow = opts.useBuffer;
#if CC_PARSER_PROFILE_TEST
        doParseNow = true;
#endif
        if (doParseNow)
        {
            result = thread->Parse();
            LinkInheritance(true);
            delete thread;
            break;
        }

        TRACE(_T("Parse() : Parsing %s"), bufferOrFilename.wx_str());

        if (!m_IsUpFront)
        {
            TRACE(_T("Parse() : Parallel Parsing %s"), bufferOrFilename.wx_str());

            // Add a task for all project files
            if (m_IsFirstBatch)
            {
                wxCriticalSectionLocker locker(s_ParseCritical);
                m_IsFirstBatch = false;
                m_PoolTask.push(PTVector());
            }

            if (!m_IsParsing && m_PoolTask.empty())
            {
                Manager::Get()->GetLogManager()->DebugLog(_T("Parse file failed!"));
                break;
            }

            if (m_IsParsing)
                m_Pool.AddTask(thread, true);
            else
            {
                wxCriticalSectionLocker locker(s_ParseCritical);
                m_PoolTask.back().push_back(thread);
            }
        }
        else if (m_IsUpFront)
        {
            if (isLocal) // Parsing up-front files
            {
                TRACE(_T("Parse() : Parsing up-front header, %s"), bufferOrFilename.wx_str());
                result = thread->Parse();
                delete thread;
                break;
            }
            else // Add task when parsing up-front files
            {
                TRACE(_T("Parse() : Add task for up-front header, %s"), bufferOrFilename.wx_str());
                wxCriticalSectionLocker locker(s_ParseCritical);
                m_PoolTask.push(PTVector());
                m_PoolTask.back().push_back(thread);
            }
        }

        result = true;
    }
    while (false);

    return result;
}

bool Parser::ParseBufferForFunctions(const wxString& buffer)
{
    ParserThreadOptions opts;
    opts.wantPreprocessor = m_Options.wantPreprocessor;
    opts.useBuffer        = true;
    opts.bufferSkipBlocks = true;
    opts.handleFunctions  = true;

    ParserThread thread(this,
                        buffer,
                        false,
                        opts,
                        m_pTempTokensTree);

    return thread.Parse();
}

bool Parser::ParseBufferForNamespaces(const wxString& buffer, NameSpaceVec& result)
{
	ParserThreadOptions opts;
	opts.useBuffer = true;

	ParserThread thread(this,
						wxEmptyString,
						true,
						opts,
						m_pTempTokensTree);

	return thread.ParseBufferForNamespaces(buffer, result);
}

bool Parser::ParseBufferForUsingNamespace(const wxString& buffer, wxArrayString& result)
{
    ParserThreadOptions opts;

    ParserThread thread(this,
                        wxEmptyString,
                        false,
                        opts,
                        m_pTempTokensTree);

    return thread.ParseBufferForUsingNamespace(buffer, result);
}

bool Parser::RemoveFile(const wxString& filename)
{
    if (!Done())
        return false; // Can't alter the tokens tree if parsing has not finished

    bool result = false;
    wxString file = UnixFilename(filename);
    {
        wxCriticalSectionLocker locker(s_TokensTreeCritical);
        size_t index = m_pTokensTree->GetFileIndex(file);
        result = m_pTokensTree->m_FilesStatus.count(index);

        m_pTokensTree->RemoveFile(filename);
        m_pTokensTree->m_FilesMap.erase(index);
        m_pTokensTree->m_FilesStatus.erase(index);
        m_pTokensTree->m_FilesToBeReparsed.erase(index);
        m_pTokensTree->m_Modified = true;
    }

    return result;
}

bool Parser::AddFile(const wxString& filename, bool isLocal)
{
    wxString file = UnixFilename(filename);
    if (IsFileParsed(file))
        return false;

    const bool done = Done();
    if (done)
        PrepareParsing();

    if (m_ParsingType == ptUndefined)
        m_ParsingType = ptAddFileToParser;

    AddParse(file);

    if (done)
        StartParsing(false);

    return true;
}

bool Parser::Reparse(const wxString& filename, bool isLocal)
{
    if (!Done())
        return false; // if still parsing, exit with error

    if (m_Timer.IsRunning())
        m_Timer.Stop();

    wxString file = UnixFilename(filename);
    if (isLocal)
        m_LocalFiles.insert(filename);
    else
        m_LocalFiles.erase(filename);

    {
        wxCriticalSectionLocker locker(s_TokensTreeCritical);
        m_pTokensTree->FlagFileForReparsing(file);
    }

    m_NeedsReparse = true;
    m_Timer.Start(reparse_timer_delay, wxTIMER_ONE_SHOT);

    return true;
}

bool Parser::ReadFromCache(wxInputStream* f)
{
    bool result = false;
    wxCriticalSectionLocker locker(s_TokensTreeCritical);

    char CACHE_MAGIC_READ[] = "           ";
    m_pTokensTree->clear(); // Clear data

    // File format is like this:
    //
    // CACHE_MAGIC
    // Number of parsed files
    // Number of tokens
    // Parsed files
    // Tokens
    // EOF

//  Begin loading process
    do
    {

        // keep a backup of include dirs
        if (f->Read(CACHE_MAGIC_READ, sizeof(CACHE_MAGIC_READ)).LastRead() != sizeof(CACHE_MAGIC_READ) ||
            strncmp(CACHE_MAGIC, CACHE_MAGIC_READ, sizeof(CACHE_MAGIC_READ) != 0))
            break;
        int fcount = 0, actual_fcount = 0;
        int tcount = 0, actual_tcount = 0;
        int idx;
        if (!LoadIntFromFile(f, &fcount))
            break;
        if (!LoadIntFromFile(f, &tcount))
            break;
        if (fcount < 0)
            break;
        if (tcount < 0)
            break;

        wxString file;
        int nonempty_token = 0;
        Token* token = 0;
        do // do while-false block
        {
            // Filenames
            int i;
            for (i = 0; i < fcount && !f->Eof(); ++i)
            {
                if (!LoadIntFromFile(f,&idx)) // Filename index
                    break;
                if (idx != i)
                    break;
                if (!LoadStringFromFile(f,file)) // Filename data
                    break;
                if (!idx)
                    file.Clear();
                if (file.IsEmpty())
                    idx = 0;
                m_pTokensTree->m_FilenamesMap.insert(file);
                actual_fcount++;
            }
            result = (actual_fcount == fcount);
            if (!result)
                break;
            if (tcount)
                m_pTokensTree->m_Tokens.resize(tcount,0);
            // Tokens
            for (i = 0; i < tcount && !f->Eof(); ++i)
            {
                token = 0;
                if (!LoadIntFromFile(f, &nonempty_token))
                break;
                if (nonempty_token != 0)
                {
                    token = new(std::nothrow) Token(wxEmptyString, 0, 0, ++m_pTokensTree->m_TokenTicketCount);
                    if (!token)
                    {
                        --m_pTokensTree->m_TokenTicketCount;
                        return false;
                    }
                    if (!token->SerializeIn(f))
                    {
                        delete token;
                        --m_pTokensTree->m_TokenTicketCount;
                        token = 0;
                        break;
                    }
                    m_pTokensTree->insert(i,token);
                }
                ++actual_tcount;
            }
            if (actual_tcount != tcount)
                break;
            m_pTokensTree->RecalcFreeList();
            result = true;
        } while(false);

    } while(false);

//  End loading process

    if (result)
        m_UsingCache = true;
    else
        m_pTokensTree->clear();

    m_pTokensTree->m_Modified = false;

    return result;
}

bool Parser::WriteToCache(wxOutputStream* f)
{
    bool result = false;
    wxCriticalSectionLocker locker(s_TokensTreeCritical);
//  Begin saving process

    size_t tcount = m_pTokensTree->m_Tokens.size();
    size_t fcount = m_pTokensTree->m_FilenamesMap.size();
    size_t i = 0;

    // write cache magic
    f->Write(CACHE_MAGIC, sizeof(CACHE_MAGIC));

    SaveIntToFile(f, fcount); // num parsed files
    SaveIntToFile(f, tcount); // num tokens

    // Filenames
    for (i = 0; i < fcount; ++i)
    {
        SaveIntToFile(f,i);
        SaveStringToFile(f,m_pTokensTree->m_FilenamesMap.GetString(i));
    }

    // Tokens

    for (i = 0; i < tcount; ++i)
    {
        TRACE(_T("WriteToCache() : Token #%d, offset %d"),i,f->TellO());
        Token* token = m_pTokensTree->at(i);
        SaveIntToFile(f,(token!=0) ? 1 : 0);
        if (token)
            token->SerializeOut(f);
    }

    result = true;

    if (result)
        m_pTokensTree->m_Modified = false;

//  End saving process
    return result;
}

void Parser::TerminateAllThreads()
{
    m_Pool.AbortAllTasks();
    wxMilliSleep(10);
    while (!m_Pool.Done())
        wxMilliSleep(10);

    while (!m_PoolTask.empty())
    {
        PTVector& v = m_PoolTask.front();
        for (PTVector::const_iterator it = v.begin(); it != v.end(); ++it)
            delete *it;
        m_PoolTask.pop();
    }
}

void Parser::AddIncludeDir(const wxString& file)
{
    wxString base = file;
    if (base.Last() == wxFILE_SEP_PATH)
        base.RemoveLast();

    if (m_IncludeDirs.Index(base) == wxNOT_FOUND)
    {
        TRACE(_T("AddIncludeDir() : Adding %s"), base.wx_str());
        m_IncludeDirs.Add(base);
    }
}

void Parser::AddPredefinedMacros(const wxString& defs, bool isLocal)
{
    ParserThreadOptions opts;
    opts.wantPreprocessor = m_Options.wantPreprocessor;
    opts.followLocalIncludes = m_Options.followLocalIncludes;
    opts.followGlobalIncludes = m_Options.followGlobalIncludes;
    opts.useBuffer = true;
    opts.isTemp = false;
    opts.bufferSkipBlocks = false;
    opts.handleFunctions = false;
    Parse(defs, isLocal, opts);
}

wxString Parser::FindFirstFileInIncludeDirs(const wxString& file)
{
    wxString FirstFound = m_GlobalIncludes.GetItem(file);
    if (FirstFound.IsEmpty())
    {
        wxArrayString FoundSet = FindFileInIncludeDirs(file,true);
        if (FoundSet.GetCount())
        {
            FirstFound = UnixFilename(FoundSet[0]);
            m_GlobalIncludes.AddItem(file, FirstFound);
        }
    }
    return FirstFound;
}

wxArrayString Parser::FindFileInIncludeDirs(const wxString& file, bool firstonly)
{
    wxArrayString FoundSet;
    for (size_t idxSearch = 0; idxSearch < m_IncludeDirs.GetCount(); ++idxSearch)
    {
        wxString base = m_IncludeDirs[idxSearch];
        wxFileName tmp = file;
        NormalizePath(tmp,base);
        wxString fullname = tmp.GetFullPath();
        if (wxFileExists(fullname))
        {
            FoundSet.Add(fullname);
            if (firstonly)
                break;
        }
    }

    TRACE(_T("FindFileInIncludeDirs() : Searching %s"), file.wx_str());
    TRACE(_T("FindFileInIncludeDirs() : Found %d"), FoundSet.GetCount());

    return FoundSet;
}

void Parser::OnAllThreadsDone(CodeBlocksEvent& event)
{
    if (m_IgnoreThreadEvents || !m_IsParsing)
        return;

    // Do next task
    if (!m_PoolTask.empty())
        m_BatchTimer.Start(1, wxTIMER_ONE_SHOT);

#if !CC_PARSER_PROFILE_TEST
    // Reparse system up-front headers
    else if (!m_SystemUpFrontHeaders.IsEmpty())
    {
        // Part.1 Set m_IsParsing to false
        m_IsParsing = false;

        // Part.2 Remove all up-front headers in token tree
        for (size_t i = 0; i < m_SystemUpFrontHeaders.GetCount(); ++i)
            RemoveFile(m_SystemUpFrontHeaders[i]);

        // Part.3 Prepare parsing
        PrepareParsing();

        // Part.4 Reparse system up-front headers
        AddBatchParse(m_SystemUpFrontHeaders);

        // Part.5 Clear
        m_SystemUpFrontHeaders.Clear();

        // Part.6 Start parsing
        StartParsing();
    }
#endif

    // Finish all task, then we need post a Parse_END event
    else
    {
        m_NeedsReparse = false;
        m_IsParsing = false;
        m_IsBatchParseDone = true;
        EndStopWatch();
        ProcessParserEvent(m_ParsingType, PARSER_END);
        m_ParsingType = ptUndefined;
        s_CurrentParser = NULL;
    }
}

wxString Parser::GetFullFileName(const wxString& src, const wxString& tgt, bool isGlobal)
{
    wxString fullname;
    if (isGlobal)
    {
        fullname = FindFirstFileInIncludeDirs(tgt);
        if (fullname.IsEmpty())
        {
            // not found; check this case:
            //
            // we had entered the previous file like this: #include <gl/gl.h>
            // and it now does this: #include "glext.h"
            // glext.h was correctly not found above but we can now search
            // for gl/glext.h.
            // if we still not find it, it's not there. A compilation error
            // is imminent (well, almost - I guess the compiler knows a little better ;).
            wxString base = wxFileName(src).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
            fullname = FindFirstFileInIncludeDirs(base + tgt);
        }
    }

    // NOTE: isGlobal is always true. The following code never executes...

    else // local files are more tricky, since they depend on two filenames
    {
        wxFileName fname(tgt);
        wxFileName source(src);
        if (NormalizePath(fname,source.GetPath(wxPATH_GET_VOLUME)))
        {
            fullname = fname.GetFullPath();
            if (!wxFileExists(fullname))
                fullname.Clear();
        }
    }

    return fullname;
}

void Parser::DoParseFile(const wxString& filename, bool isGlobal)
{
    if (m_IgnoreThreadEvents)
        return;

    if (   (!isGlobal && !m_Options.followLocalIncludes)
        || ( isGlobal && !m_Options.followGlobalIncludes) )
        return;

    if (filename.IsEmpty())
        return;

    Parse(filename, !isGlobal);
}

void Parser::StartStopWatch()
{
    if (!m_StopWatchRunning)
    {
        m_StopWatchRunning = true;
        m_StopWatch.Start();
    }
}

void Parser::EndStopWatch()
{
    if (m_StopWatchRunning)
    {
        m_StopWatch.Pause();
        m_StopWatchRunning = false;
        if (m_IsBatchParseDone)
            m_LastStopWatchTime = m_StopWatch.Time();
        else
            m_LastStopWatchTime += m_StopWatch.Time();
    }
}

long Parser::EllapsedTime()
{
    return m_StopWatchRunning ? m_StopWatch.Time() : 0;
}

long Parser::LastParseTime()
{
    return m_LastStopWatchTime;
}

void Parser::OnTimer(wxTimerEvent& event)
{
    ReparseModifiedFiles();
    event.Skip();
}

void Parser::OnBatchTimer(wxTimerEvent& event)
{
    if (!m_StopWatchRunning)
        StartStopWatch();

    // Add parse by child thread
    if (!m_UpFrontHeaders.IsEmpty() || !m_BatchParseFiles.IsEmpty())
    {
        do
        {
            // Current batch parser is already exists
            if (s_CurrentParser && s_CurrentParser != this)
                break;

            AddParseThread* thread = new(std::nothrow) AddParseThread(this);
            if (!thread)
                break;

            // Have not any batch parsing
            if (!s_CurrentParser)
            {
                s_CurrentParser = this;
                m_StopWatch.Start(); // reset timer
                ProcessParserEvent(m_ParsingType, PARSER_START);
            }

            m_Pool.AddTask(thread, true);
            return;
        }
        while (false);

        m_BatchTimer.Start(1000, wxTIMER_ONE_SHOT);
        return;
    }

    // Setting parse flag when all task added
    else if (!m_IsParsing)
        m_IsParsing = true;

    if (!m_PoolTask.empty())
    {
        m_Pool.BatchBegin();

        PTVector& v = m_PoolTask.front();
        for (PTVector::const_iterator it = v.begin(); it != v.end(); ++it)
            m_Pool.AddTask(*it, true);
        m_PoolTask.pop();

        m_Pool.BatchEnd();
    }
    else
    {
        ProcessParserEvent(ptUndefined, PARSER_START, _T("No files for batch parsing"));
        CodeBlocksEvent evt;
        evt.SetEventObject(this);
        OnAllThreadsDone(evt);
    }
}

bool Parser::ReparseModifiedFiles()
{
    if (!m_NeedsReparse || !m_Pool.Done())
        return false;

    std::queue<wxString> files_list;
    {
        wxCriticalSectionLocker locker(s_TokensTreeCritical);
        TokenFilesSet::iterator it;

        // loop two times so that we reparse modified *header* files first
        // because they usually hold definitions which need to exist
        // when we parse the normal source files...
        for (it = m_pTokensTree->m_FilesToBeReparsed.begin(); it != m_pTokensTree->m_FilesToBeReparsed.end(); ++it)
        {
            wxString filename = m_pTokensTree->m_FilenamesMap.GetString(*it);
            if (FileTypeOf(filename) == ftSource) // ignore source files (*.cpp etc)
                continue;
            files_list.push(filename);
            m_pTokensTree->RemoveFile(*it);
        }
        for (it = m_pTokensTree->m_FilesToBeReparsed.begin(); it != m_pTokensTree->m_FilesToBeReparsed.end(); ++it)
        {
            wxString filename = m_pTokensTree->m_FilenamesMap.GetString(*it);
            if (FileTypeOf(filename) != ftSource) // ignore non-source files (*.h etc)
                continue;
            files_list.push(filename);
            m_pTokensTree->RemoveFile(*it);
        }
    }

    const bool done = Done();
    if (done)
        PrepareParsing();

    while (!files_list.empty())
    {
        wxString& filename = files_list.front();
        AddParse(filename);
        files_list.pop();
    }

    if (m_ParsingType == ptUndefined)
        m_ParsingType = ptReparseFile;

    if (done)
        StartParsing(false);

    return true;
}

size_t Parser::FindTokensInFile(const wxString& fileName, TokenIdxSet& result, short int kindMask)
{
    result.clear();
    wxString file = UnixFilename(fileName);
    TokenIdxSet tmpresult;

    wxCriticalSectionLocker locker(s_TokensTreeCritical);
    TRACE(_T("Parser::FindTokensInFile() : Searching for file '%s' in tokens tree..."), file.wx_str());
    if ( !m_pTokensTree->FindTokensInFile(file, tmpresult, kindMask) )
        return 0;

    for (TokenIdxSet::iterator it = tmpresult.begin(); it != tmpresult.end(); ++it)
    {
        Token* token = m_pTokensTree->at(*it);
        if (token)
            result.insert(*it);
    }
    return result.size();
}

bool Parser::IsFileParsed(const wxString& filename)
{
    wxCriticalSectionLocker locker(s_TokensTreeCritical);
    return m_pTokensTree->IsFileParsed(UnixFilename(filename));
}

void Parser::ProcessParserEvent(ParsingType type, int id, const wxString& info)
{
    if (m_IgnoreThreadEvents)
        return;

    wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, id);
    evt.SetClientData(this);
    evt.SetInt(type);
    evt.SetString(info);
    m_pParent->ProcessEvent(evt);
}
