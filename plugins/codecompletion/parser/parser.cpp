/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include <sdk.h>
#include <wx/log.h>
#include <wx/app.h>
#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <wx/intl.h>
#include <wx/progdlg.h>
#include "parser.h"
#include "../classbrowser.h"
#ifndef STANDALONE
	#include <configmanager.h>
	#include <messagemanager.h>
	#include <editormanager.h>
	#include <manager.h>
	#include <globals.h>
#endif // STANDALONE

static const char CACHE_MAGIC[] = "CCCACHE_1_3";
static const int batch_timer_delay = 500;
static const int reparse_timer_delay = 250;

int PARSER_END = wxNewId();
static int idPool = wxNewId();
int TIMER_ID = wxNewId();
int BATCH_TIMER_ID = wxNewId();

BEGIN_EVENT_TABLE(Parser, wxEvtHandler)
END_EVENT_TABLE()

Parser::Parser(wxEvtHandler* parent)
	: m_MaxThreadsCount(8),
	m_pParent(parent)
#ifndef STANDALONE
	,m_pImageList(0L),
#endif
    m_UsingCache(false),
    m_Pool(this, idPool,1),
    m_pTokens(0),
    m_pTempTokens(0),
    m_NeedsReparse(false),
    m_IsBatch(false),
    m_pClassBrowser(0),
    m_TreeBuildingStatus(0),
    m_TreeBuildingTokenIdx(0),
    m_timer(this, TIMER_ID),
    m_batchtimer(this,BATCH_TIMER_ID),
    m_StopWatchRunning(false),
    m_LastStopWatchTime(0),
    m_IgnoreThreadEvents(false),
    m_ShuttingDown(false)
{
    m_pTokens = new TokensTree;
    m_pTempTokens = new TokensTree;
    m_LocalFiles.clear();
    m_GlobalIncludes.clear();
	ReadOptions();
#ifndef STANDALONE
	m_pImageList = new wxImageList(16, 16);
	wxBitmap bmp;
	wxString prefix;
    prefix = ConfigManager::GetDataFolder() + _T("/images/codecompletion/");
    // bitmaps must be added by order of PARSER_IMG_* consts
    bmp.LoadFile(prefix + _T("class_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS_FOLDER
    bmp.LoadFile(prefix + _T("class.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS
    bmp.LoadFile(prefix + _T("ctor_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CTOR_PRIVATE
    bmp.LoadFile(prefix + _T("ctor_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CTOR_PROTECTED
    bmp.LoadFile(prefix + _T("ctor_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CTOR_PUBLIC
    bmp.LoadFile(prefix + _T("dtor_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_DTOR_PRIVATE
    bmp.LoadFile(prefix + _T("dtor_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_DTOR_PROTECTED
    bmp.LoadFile(prefix + _T("dtor_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_DTOR_PUBLIC
    bmp.LoadFile(prefix + _T("method_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNC_PRIVATE
    bmp.LoadFile(prefix + _T("method_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNC_PRIVATE
    bmp.LoadFile(prefix + _T("method_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNC_PUBLIC
    bmp.LoadFile(prefix + _T("var_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VAR_PRIVATE
    bmp.LoadFile(prefix + _T("var_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VAR_PROTECTED
    bmp.LoadFile(prefix + _T("var_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VAR_PUBLIC
    bmp.LoadFile(prefix + _T("preproc.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_PREPROCESSOR
    bmp.LoadFile(prefix + _T("enum.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUM
    bmp.LoadFile(prefix + _T("enumerator.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUMERATOR
    bmp.LoadFile(prefix + _T("namespace.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_NAMESPACE
    bmp.LoadFile(prefix + _T("symbols_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_SYMBOLS_FOLDER
    bmp.LoadFile(prefix + _T("enums_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUMS_FOLDER
    bmp.LoadFile(prefix + _T("preproc_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_PREPROC_FOLDER
    bmp.LoadFile(prefix + _T("others_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_OTHERS_FOLDER
#endif // STANDALONE
    ConnectEvents();
}

Parser::~Parser()
{
    m_ShuttingDown = true;
    if(m_pClassBrowser && m_pClassBrowser->GetParserPtr() == this)
        m_pClassBrowser->UnlinkParser();
    m_TreeBuildingStatus = 0;
    m_pClassBrowser = NULL;

	Clear(); // Clear also disconnects the events
#ifndef STANDALONE
	delete m_pImageList;
	m_pImageList = NULL;

    delete m_pTempTokens;
    m_pTempTokens = NULL;
	delete m_pTokens;
	m_pTokens = NULL;
#endif // STANDALONE
}

void Parser::ConnectEvents()
{
    Connect(-1,-1,cbEVT_THREADTASK_ALLDONE,
            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
            &Parser::OnAllThreadsDone);
    Connect(-1,TIMER_ID, wxEVT_TIMER,(wxObjectEventFunction)
            (wxEventFunction)(wxTimerEventFunction)&Parser::OnTimer);
    Connect(-1,BATCH_TIMER_ID,wxEVT_TIMER,(wxObjectEventFunction)
            (wxEventFunction)(wxTimerEventFunction)&Parser::OnBatchTimer);
}

void Parser::DisconnectEvents()
{
    Disconnect(-1,BATCH_TIMER_ID,wxEVT_TIMER);
    Disconnect(-1,TIMER_ID, wxEVT_TIMER);
    Disconnect(-1,-1,cbEVT_THREADTASK_ALLDONE);
}

void Parser::ReadOptions()
{
#ifdef STANDALONE
	m_Options.followLocalIncludes = true;
	m_Options.followGlobalIncludes = false;
	m_Options.caseSensitive = true;
	m_Options.wantPreprocessor = false;
	m_Options.useSmartSense = true;
	m_BrowserOptions.showInheritance = false;
	m_BrowserOptions.viewFlat = false;
	m_BrowserOptions.showAllSymbols = false;
#else // !STANDALONE
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));
	m_MaxThreadsCount = cfg->ReadInt(_T("/max_threads"), wxThread::GetCPUCount());
	m_Options.followLocalIncludes = cfg->ReadBool(_T("/parser_follow_local_includes"), false);
	m_Options.followGlobalIncludes = cfg->ReadBool(_T("/parser_follow_global_includes"), false);
	m_Options.caseSensitive = cfg->ReadBool(_T("/case_sensitive"), false);
	m_Options.useSmartSense = cfg->ReadBool(_T("/use_SmartSense"), true);
	m_Options.wantPreprocessor = cfg->ReadBool(_T("/want_preprocessor"), false);
	m_BrowserOptions.showInheritance = cfg->ReadBool(_T("/browser_show_inheritance"), false);
	m_BrowserOptions.viewFlat = cfg->ReadBool(_T("/browser_view_flat"), false);
	m_BrowserOptions.showAllSymbols = cfg->ReadBool(_T("/show_all_symbols"), false);
#endif // STANDALONE
}

void Parser::WriteOptions()
{
#ifndef STANDALONE
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));
	cfg->Write(_T("/max_threads"), (int)m_MaxThreadsCount);
	cfg->Write(_T("/parser_follow_local_includes"), m_Options.followLocalIncludes);
	cfg->Write(_T("/parser_follow_global_includes"), m_Options.followGlobalIncludes);
	cfg->Write(_T("/case_sensitive"), m_Options.caseSensitive);
	cfg->Write(_T("/use_SmartSense"), m_Options.useSmartSense);
	cfg->Write(_T("/want_preprocessor"), m_Options.wantPreprocessor);
	cfg->Write(_T("/browser_show_inheritance"), m_BrowserOptions.showInheritance);
	cfg->Write(_T("/show_all_symbols"), m_BrowserOptions.showAllSymbols);
	cfg->Write(_T("/browser_view_flat"), m_BrowserOptions.viewFlat);
#endif // STANDALONE
}

bool Parser::CacheNeedsUpdate()
{
    if (m_UsingCache)
    {
        wxCriticalSectionLocker lock(s_MutexProtection);
        return m_pTokens->m_modified;
    }
    return true;
}

unsigned int Parser::GetFilesCount()
{
	wxCriticalSectionLocker lock(s_MutexProtection);
	return m_pTokens->m_FilesMap.size();
}

bool Parser::Done()
{
    wxCriticalSectionLocker lock(s_mutexListProtection);
	return m_Pool.Done();
}

#ifndef STANDALONE
void Parser::SetTokenKindImage(int kind, const wxBitmap& bitmap, const wxBitmap& mask)
{
	if (kind < PARSER_IMG_MIN || kind > PARSER_IMG_MAX)
		return;
#ifdef __WXMSW__
	m_pImageList->Replace(kind, bitmap, mask);
#endif
}

void Parser::SetTokenKindImage(int kind, const wxBitmap& bitmap, const wxColour& maskColor)
{
	if (kind < PARSER_IMG_MIN || kind > PARSER_IMG_MAX)
		return;
	m_pImageList->Replace(kind, bitmap);//, maskColor);
}

void Parser::SetTokenKindImage(int kind, const wxIcon& icon)
{
	if (kind < PARSER_IMG_MIN || kind > PARSER_IMG_MAX)
		return;
	m_pImageList->Replace(kind, icon);
}

int Parser::GetTokenKindImage(Token* token)
{
    if (!token)
        return PARSER_IMG_NONE;

	switch (token->m_TokenKind)
	{
		case tkPreprocessor: return PARSER_IMG_PREPROCESSOR;

		case tkEnum: return PARSER_IMG_ENUM;

		case tkEnumerator: return PARSER_IMG_ENUMERATOR;

		case tkClass: return PARSER_IMG_CLASS;

		case tkNamespace: return PARSER_IMG_NAMESPACE;

		case tkConstructor:
			switch (token->m_Scope)
			{
				case tsProtected: return PARSER_IMG_CTOR_PROTECTED;
				case tsPrivate: return PARSER_IMG_CTOR_PRIVATE;
				default: return PARSER_IMG_CTOR_PUBLIC;
			}

		case tkDestructor:
			switch (token->m_Scope)
			{
				case tsProtected: return PARSER_IMG_DTOR_PROTECTED;
				case tsPrivate: return PARSER_IMG_DTOR_PRIVATE;
				default: return PARSER_IMG_DTOR_PUBLIC;
			}

		case tkFunction:
			switch (token->m_Scope)
			{
				case tsProtected: return PARSER_IMG_FUNC_PROTECTED;
				case tsPrivate: return PARSER_IMG_FUNC_PRIVATE;
				default: return PARSER_IMG_FUNC_PUBLIC;
			}

		case tkVariable:
			switch (token->m_Scope)
			{
				case tsProtected: return PARSER_IMG_VAR_PROTECTED;
				case tsPrivate: return PARSER_IMG_VAR_PRIVATE;
				default: return PARSER_IMG_VAR_PUBLIC;
			}

        default: return PARSER_IMG_NONE;
    }
}
#endif // STANDALONE

Token* Parser::FindTokenByName(const wxString& name, bool globalsOnly, short int kindMask) const
{
    wxCriticalSectionLocker lock(s_MutexProtection);
    int result = m_pTokens->TokenExists(name,-1,kindMask);
    return m_pTokens->at(result);
}

Token* Parser::FindChildTokenByName(Token* parent, const wxString& name, bool useInheritance, short int kindMask) const
{
	if (!parent)
		return FindTokenByName(name, false, kindMask);
    Token* result = 0;
    wxCriticalSectionLocker *lock = 0;
    {
        lock = new wxCriticalSectionLocker(s_MutexProtection);
        result = m_pTokens->at(m_pTokens->TokenExists(name,parent->GetSelf(),kindMask));
        delete lock;
    }
    if(!result && useInheritance)
    {
        lock = new wxCriticalSectionLocker(s_MutexProtection);
        TokenIdxSet::iterator it;
        for(it = parent->m_Ancestors.begin();it != parent->m_Ancestors.end();++it)
        {
            Token* ancestor = m_pTokens->at(*it);
            result = FindChildTokenByName(ancestor, name, true, kindMask);
            if(result)
                break;
        }
        delete lock;
    }
    return result;
}

size_t Parser::FindMatches(const wxString& s,TokenList& result,bool caseSensitive,bool is_prefix,bool markedonly)
{
    result.clear();
    TokenIdxSet tmpresult;
    wxCriticalSectionLocker lock(s_MutexProtection);
    if(!m_pTokens->FindMatches(s,tmpresult,caseSensitive,is_prefix))
        return 0;

    TokenIdxSet::iterator it;
    for(it = tmpresult.begin();it!=tmpresult.end();++it)
    {
        Token* token = m_pTokens->at(*it);
        if(token && (!markedonly || token->m_Bool))
        result.push_back(token);
    }
    return result.size();
}

void Parser::LinkInheritance(bool tempsOnly)
{
	wxCriticalSectionLocker lock(s_MutexProtection);
	(tempsOnly ? m_pTempTokens :  m_pTokens)->RecalcData();
}

bool Parser::ParseBuffer(const wxString& buffer, bool isLocal, bool bufferSkipBlocks)
{
	ParserThreadOptions opts;
	opts.wantPreprocessor = m_Options.wantPreprocessor;
	opts.followLocalIncludes = m_Options.followLocalIncludes;
	opts.followGlobalIncludes = m_Options.followGlobalIncludes;
	opts.useBuffer = true;
	opts.bufferSkipBlocks = bufferSkipBlocks;
	return Parse(buffer, isLocal, opts);
}

void Parser::BatchParse(const wxArrayString& filenames)
{
    m_batchtimer.Stop();
    m_IsBatch = true;
    m_Pool.BatchBegin();
    for (unsigned int i = 0; i < filenames.GetCount(); ++i)
        Parse(filenames[i]);

    // Allow future parses to take place in this same run
    m_batchtimer.Start(batch_timer_delay,wxTIMER_ONE_SHOT);
}

bool Parser::Parse(const wxString& filename, bool isLocal)
{
	ParserThreadOptions opts;
	opts.wantPreprocessor = m_Options.wantPreprocessor;
	opts.useBuffer = false;
	opts.bufferSkipBlocks = false;
	opts.followLocalIncludes = m_Options.followLocalIncludes;
	opts.followGlobalIncludes = m_Options.followGlobalIncludes;
	return Parse(UnixFilename(filename), isLocal, opts);
}

bool Parser::Parse(const wxString& bufferOrFilename, bool isLocal, ParserThreadOptions& opts)
{
	wxString buffOrFile = bufferOrFilename;
    bool result = false;
    do
    {
        if(!opts.useBuffer)
        {
            wxCriticalSectionLocker lock(s_MutexProtection);
            bool canparse = !m_pTokens->IsFileParsed(buffOrFile);
            if(canparse)
                canparse = m_pTokens->ReserveFileForParsing(buffOrFile,true) != 0;
            if (!canparse)
                break;
        }

        ParserThread* thread = new ParserThread(this,
                                                buffOrFile,
                                                isLocal,
                                                opts,
                                                m_pTokens);//(opts.useBuffer ? m_pTempTokens : m_pTokens));
        if (opts.useBuffer)
        {
            result = thread->Parse();
            LinkInheritance(true);
            delete thread;
            break;
        }

        bool use_timer = m_batchtimer.IsRunning();
        if(!m_IsBatch && wxThread::IsMain())
        {
            use_timer = true;
            m_IsBatch = true;
            m_Pool.BatchBegin();
        }
//        Manager::Get()->GetMessageManager()->DebugLog(_T("parsing %s"),buffOrFile.c_str());
        if(m_IgnoreThreadEvents)
            m_IgnoreThreadEvents = false;
        #ifdef CODECOMPLETION_PROFILING
        StartStopWatch();
        m_batchtimer.Stop();
        thread->Parse();
        #else
        m_Pool.AddTask(thread, true);
        #endif

        // For every parse, reset the countdown to -batch_timer_delay.
        // This will give us a tolerance period before the next parse job is queued.
        if(use_timer)
            m_batchtimer.Start(batch_timer_delay,wxTIMER_ONE_SHOT);
        result = true;
    }while(false);
    return result;
}

bool Parser::ParseBufferForFunctions(const wxString& buffer)
{
	ParserThreadOptions opts;
	opts.wantPreprocessor = m_Options.wantPreprocessor;
	opts.useBuffer = false;
	opts.bufferSkipBlocks = false;
	ParserThread* thread = new ParserThread(this,
											wxEmptyString,
											false,
											opts,
											m_pTempTokens);
	return thread->ParseBufferForFunctions(buffer);
}

bool Parser::RemoveFile(const wxString& filename)
{
	if(!Done())
        return false; // Can't alter the tokens tree if parsing has not finished
    bool result = false;
	wxString file = UnixFilename(filename);
	{
        wxCriticalSectionLocker lock(s_MutexProtection);
        size_t index = m_pTokens->GetFileIndex(file);
        result = m_pTokens->m_FilesStatus.count(index);

        m_pTokens->RemoveFile(filename);
        m_pTokens->m_FilesMap.erase(index);
        m_pTokens->m_FilesStatus.erase(index);
        m_pTokens->m_FilesToBeReparsed.erase(index);
        m_pTokens->m_modified = true;
	}
	return result;
}

bool Parser::Reparse(const wxString& filename, bool isLocal)
{
	if (!Done())
		return false; // if still parsing, exit with error
	wxString file = UnixFilename(filename);
	if(isLocal)
        m_LocalFiles.insert(filename);
    else
        m_LocalFiles.erase(filename);
	{
	    wxCriticalSectionLocker lock(s_MutexProtection);
	    m_pTokens->FlagFileForReparsing(file);
	}
    m_NeedsReparse = true;
    m_timer.Start(reparse_timer_delay,wxTIMER_ONE_SHOT);
    return true;
}

void Parser::Clear()
{
    DisconnectEvents();
	if(m_ShuttingDown)
	{
        Manager::Get()->GetMessageManager()->DebugLog(_("Terminating threads..."));
	}
	TerminateAllThreads(); //
	if(m_ShuttingDown)
	{
        Manager::Get()->GetMessageManager()->DebugLog(_("Done."));
	}

    Manager::ProcessPendingEvents();

    m_IncludeDirs.Clear();
    m_pTokens->clear();
    m_pTempTokens->clear();

    m_LocalFiles.clear();
    m_GlobalIncludes.clear();

	if(!m_ShuttingDown)
	{
        Manager::ProcessPendingEvents();
        ConnectEvents();
	}

	m_UsingCache = false;
}

bool Parser::ReadFromCache(wxInputStream* f)
{
    bool result = false;
    wxCriticalSectionLocker lock(s_MutexProtection);

    char CACHE_MAGIC_READ[] = "           ";
    m_pTokens->clear(); // Clear data

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
                if(!LoadIntFromFile(f,&idx)) // Filename index
                    break;
                if(idx != i)
                    break;
                if(!LoadStringFromFile(f,file)) // Filename data
                    break;
                if(!idx)
                    file.Clear();
                if(file.IsEmpty())
                    idx = 0;
                m_pTokens->m_FilenamesMap.insert(file);
                actual_fcount++;
            }
            result = (actual_fcount == fcount);
            if(!result)
                break;
            if(tcount)
                m_pTokens->m_Tokens.resize(tcount,0);
            // Tokens
            for (i = 0; i < tcount && !f->Eof(); ++i)
            {
                token = 0;
                if (!LoadIntFromFile(f, &nonempty_token))
                break;
                if(nonempty_token != 0)
                {
                    token = new Token();
                    if (!token->SerializeIn(f))
                    {
                        delete token;
                        token = 0;
                        break;
                    }
                    m_pTokens->insert(i,token);
                }
                ++actual_tcount;
            }
            if(actual_tcount != tcount)
                break;
            m_pTokens->RecalcFreeList();
            result = true;
        }while(false);

    }
    while(false);

//  End loading process

    if(result)
        m_UsingCache = true;
    else
        m_pTokens->clear();
    m_pTokens->m_modified = false;
    return result;
}

bool Parser::WriteToCache(wxOutputStream* f)
{
    bool result = false;
    wxCriticalSectionLocker lock(s_MutexProtection);
//  Begin saving process

    size_t tcount = m_pTokens->m_Tokens.size();
    size_t fcount = m_pTokens->m_FilenamesMap.size();
    size_t i = 0;

    // write cache magic
    f->Write(CACHE_MAGIC, sizeof(CACHE_MAGIC));

    SaveIntToFile(f, fcount); // num parsed files
    SaveIntToFile(f, tcount); // num tokens

    // Filenames
    for(i = 0; i < fcount; ++i)
    {
        SaveIntToFile(f,i);
        SaveStringToFile(f,m_pTokens->m_FilenamesMap.GetString(i));
    }

    // Tokens

    for (i = 0; i < tcount; ++i)
    {
        // Manager::Get()->GetMessageManager()->DebugLog(_("Token #%d, offset %d"),i,f->TellO());
        Token* token = m_pTokens->at(i);
        SaveIntToFile(f,(token!=0) ? 1 : 0);
        if(token)
            token->SerializeOut(f);
    }

    result = true;

    if(result)
        m_pTokens->m_modified = false;
    return result;
//  End saving process
    return result;
}

void Parser::TerminateAllThreads()
{
    m_IgnoreThreadEvents = true;
    m_Pool.AbortAllTasks();
}

void Parser::AddIncludeDir(const wxString& file)
{
	if(m_IncludeDirs.Index(file) == wxNOT_FOUND)
	{
//    	Manager::Get()->GetMessageManager()->DebugLog(_("Adding %s"), file.c_str());
		m_IncludeDirs.Add(file);
	}
} // end of AddIncludeDir

wxString Parser::FindFirstFileInIncludeDirs(const wxString& file)
{
    wxString FirstFound = m_GlobalIncludes.GetItem(file);
    if(FirstFound.IsEmpty())
    {
        wxArrayString FoundSet = FindFileInIncludeDirs(file,true);
        if(FoundSet.GetCount())
        {
            FirstFound = UnixFilename(FoundSet[0]);
            m_GlobalIncludes.AddItem(file,FirstFound);
        }
    }
    return FirstFound;
}

wxArrayString Parser::FindFileInIncludeDirs(const wxString& file,bool firstonly)
{
	wxArrayString FoundSet;
	for(size_t idxSearch = 0; idxSearch < m_IncludeDirs.GetCount(); ++idxSearch)
	{
		wxString base = m_IncludeDirs[idxSearch];
		wxFileName tmp = file;
		NormalizePath(tmp,base);
		wxString fullname = tmp.GetFullPath();
		if(wxFileExists(fullname))
		{
			FoundSet.Add(fullname);
			if(firstonly)
                break;
		}
	} // end for : idx : idxSearch
//	Manager::Get()->GetMessageManager()->DebugLog(_("Searching %s"), file.c_str());
//	Manager::Get()->GetMessageManager()->DebugLog(_("Found %d"), FoundSet.GetCount());
	return FoundSet;
} // end of FindFileInIncludeDirs

void Parser::OnAllThreadsDone(CodeBlocksEvent& event)
{
    if(m_IgnoreThreadEvents)
        return;
    EndStopWatch();
    LinkInheritance(false);
    wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, PARSER_END);
    evt.SetClientData(this);
	wxPostEvent(m_pParent, evt);
}

wxString Parser::GetFullFileName(const wxString& src,const wxString& tgt, bool isGlobal)
{
    wxCriticalSectionLocker lock(s_mutexListProtection);
    wxString fullname(_T("")); // Initialize with Empty String
    if(isGlobal)
        fullname = FindFirstFileInIncludeDirs(tgt);
    else // local files are more tricky, since they depend on two filenames
    {
        wxFileName fname(tgt);
        wxFileName source(src);
        if(NormalizePath(fname,source.GetPath(wxPATH_GET_VOLUME)))
        {
            fullname = fname.GetFullPath();
            if(!wxFileExists(fullname))
                fullname.Clear();
        }
    }
    return fullname;
}

void Parser::OnParseFile(const wxString& filename,int flags)
{
    if(m_IgnoreThreadEvents)
        return;
	if ((flags == 0 && !m_Options.followLocalIncludes) ||
		(flags == 1 && !m_Options.followGlobalIncludes))
		return;
	if (filename.IsEmpty())
        return;
    Parse(filename, flags == 0); // isLocal = (flags==0)
}

void Parser::BuildTree(wxTreeCtrl& tree)
{
	if (!Done())
		return;

	tree.Freeze();
    tree.DeleteAllItems();
    TokenFilesSet currset;
    currset.clear();
    Token* token = 0;
#ifndef STANDALONE
	tree.SetImageList(m_pImageList);

    wxString fname(_T(""));
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        fname = ed->GetFilename().BeforeLast(_T('.'));

    // "mark" tokens based on scope
    bool fnameEmpty = fname.IsEmpty();
    fname.Append(_T('.'));
    if(!fnameEmpty && !m_BrowserOptions.showAllSymbols)
    {
        for(size_t i = 1; i < m_pTokens->m_FilenamesMap.size(); ++i)
        {
            if(m_pTokens->m_FilenamesMap.GetString(i).StartsWith(fname))
                currset.insert(i);
        }
    }

#endif

	m_RootNode = tree.AddRoot(_("Symbols"), PARSER_IMG_SYMBOLS_FOLDER);
	if (m_BrowserOptions.viewFlat)
	{
        TokenIdxSet::iterator it,it_end;
        it = m_pTokens->m_GlobalNameSpace.begin();
        it_end = m_pTokens->m_GlobalNameSpace.end();

        for(;it != it_end;++it)
        {
            token = m_pTokens->at(*it);
            if(!token || !token->m_IsLocal || token->m_ParentIndex!=-1 || !token->MatchesFiles(currset))
                continue;
            AddTreeNode(tree, m_RootNode, token);
        }
		tree.SortChildren(m_RootNode);
	}
	else
	{
        wxTreeItemId globalNS = tree.AppendItem(m_RootNode, _("Global namespace"), PARSER_IMG_NAMESPACE);
        AddTreeNamespace(tree, globalNS, 0,currset);
        BuildTreeNamespace(tree, m_RootNode, 0,currset);
	}

	tree.Expand(m_RootNode);
	tree.Thaw();
	// wxString memdump = m_pTokens->m_Tree.Serialize();
	// Manager::Get()->GetMessageManager()->DebugLog(memdump);
}

void Parser::BuildTreeNamespace(wxTreeCtrl& tree, const wxTreeItemId& parentNode, Token* parent, const TokenFilesSet& currset)
{
	TokenIdxSet::iterator it,it_end;
	int parentidx;
	if(!parent)
	{
        it = m_pTokens->m_TopNameSpaces.begin();
        it_end = m_pTokens->m_TopNameSpaces.end();
        parentidx = -1;
	}
    else
    {
        it = parent->m_Children.begin();
        it_end = parent->m_Children.end();
        parentidx = parent->GetSelf();
    }

	for(;it != it_end; it++)
	{
	    Token* token = m_pTokens->at(*it);
	    if(!token || /* !token->m_Bool || */ !token->m_IsLocal || token->m_TokenKind != tkNamespace)
            continue;
        if(currset.size() && !token->MatchesFiles(currset))
            continue;
        ClassTreeData* ctd = new ClassTreeData(token);
        wxTreeItemId newNS = tree.AppendItem(parentNode, token->m_Name, PARSER_IMG_NAMESPACE, -1, ctd);
        BuildTreeNamespace(tree, newNS, token, currset);
        AddTreeNamespace(tree, newNS, token, currset);
	}
    tree.SortChildren(parentNode);
}

void Parser::AddTreeNamespace(wxTreeCtrl& tree, const wxTreeItemId& parentNode, Token* parent,const TokenFilesSet& currset)
{
	TokenIdxSet::iterator it,it_end;
	int parentidx;
	if(!parent)
	{
        it = m_pTokens->m_GlobalNameSpace.begin();
        it_end = m_pTokens->m_GlobalNameSpace.end();
        parentidx = -1;
	}
    else
    {
        it = parent->m_Children.begin();
        it_end = parent->m_Children.end();
        parentidx = parent->GetSelf();
    }

	bool has_classes = false,has_enums = false,has_preprocessor = false,has_others = false;
	wxTreeItemId node_classes;
	wxTreeItemId node_enums;
	wxTreeItemId node_preprocessor;
	wxTreeItemId node_others;
	wxTreeItemId* curnode = 0;

	for(;it != it_end; it++)
	{
	    Token* token = m_pTokens->at(*it);
	    if(!token || /* !token->m_Bool || */ !token->m_IsLocal)
            continue;
        if(currset.size() && !token->MatchesFiles(currset))
            continue;

        switch(token->m_TokenKind)
        {
            case tkClass:
                    if(!has_classes)
                    {
                        has_classes = true;
                        node_classes = tree.AppendItem(parentNode, _("Classes"), PARSER_IMG_CLASS_FOLDER);
                    }
                    curnode = &node_classes;
                    break;
            case tkEnum:
                    if(!has_enums)
                    {
                        has_enums = true;
                        node_enums = tree.AppendItem(parentNode, _("Enums"), PARSER_IMG_ENUMS_FOLDER);
                    }
                    curnode = &node_enums;
                    break;
            case tkPreprocessor:
                    if(!has_preprocessor)
                    {
                        has_preprocessor = true;
                        node_preprocessor = tree.AppendItem(parentNode, _("Preprocessor"), PARSER_IMG_PREPROC_FOLDER);
                    }
                    curnode = &node_preprocessor;
                    break;
            case tkEnumerator:
            case tkFunction:
            case tkVariable:
            case tkUndefined:
                    if(!has_others)
                    {
                        has_others = true;
                        node_others = tree.AppendItem(parentNode, _("Others"), PARSER_IMG_OTHERS_FOLDER);
                    }
                    curnode = &node_others;
                    break;
            default:curnode = 0;
        }
        if(curnode)
            AddTreeNode(tree, *curnode, token);
	}
    if(has_classes)
        tree.SortChildren(node_classes);
    if(has_enums)
        tree.SortChildren(node_enums);
    if(has_preprocessor)
        tree.SortChildren(node_preprocessor);
    if(has_others)
        tree.SortChildren(node_others);
}

void Parser::AddTreeNode(wxTreeCtrl& tree, const wxTreeItemId& parentNode, Token* token, bool childrenOnly)
{
    if (!token)
        return;
	ClassTreeData* ctd = new ClassTreeData(token);
	int image = -1;
#ifndef STANDALONE
	image = GetTokenKindImage(token);
#endif
	wxString str = token->m_Name + token->m_Args;
	if (!token->m_ActualType.IsEmpty())
		 str = str + _T(" : ") + token->m_ActualType;
	wxTreeItemId node = childrenOnly ? parentNode : tree.AppendItem(parentNode, str, image, -1, ctd);

	// add children
	TokenIdxSet::iterator it;
	for(it=token->m_Children.begin();it!=token->m_Children.end();++it)
	{
	    AddTreeNode(tree, node, m_pTokens->at(*it));
	}

	if (!m_BrowserOptions.showInheritance || (token->m_TokenKind != tkClass && token->m_TokenKind != tkNamespace))
		return;
	// add ancestor's children
	for(it=token->m_Ancestors.begin();it!=token->m_Ancestors.end();++it)
	{
	    AddTreeNode(tree, node, m_pTokens->at(*it),true);
	}

    tree.SortChildren(node);
}

void Parser::AbortBuildingTree()
{
    if(m_TreeBuildingStatus == 2)
    {
        if(m_pClassBrowser && m_pClassBrowser->GetParserPtr() == this && m_pClassBrowser->GetTree())
        {
            m_pClassBrowser->GetTree()->DeleteAllItems();
            m_pClassBrowser->GetTree()->Thaw();
        }
    }
    m_TreeBuildingStatus = 0;
}

void Parser::StartStopWatch()
{
    if(!m_StopWatchRunning)
    {
        m_StopWatchRunning = true;
        m_StopWatch.Start();
    }
}

void Parser::EndStopWatch()
{
    if(m_StopWatchRunning)
    {
        m_StopWatch.Pause();
        m_StopWatchRunning = false;
        m_LastStopWatchTime = m_StopWatch.Time();
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
#ifndef CODECOMPLETION_PROFILING
    if(m_IsBatch)
    {
        m_IsBatch = false;
        StartStopWatch();
        m_Pool.BatchEnd();
    }
#else
    EndStopWatch();
    if(m_LastStopWatchTime > batch_timer_delay)
        m_LastStopWatchTime -= batch_timer_delay;
    else
        m_LastStopWatchTime = 0;
    CodeBlocksEvent evt;
    OnAllThreadsDone(evt);
#endif
}

bool Parser::ReparseModifiedFiles()
{
    if(!m_NeedsReparse || !m_Pool.Done())
        return false;
    Manager::Get()->GetMessageManager()->DebugLog(_("Reparsing saved files..."));
    m_NeedsReparse = false;
    int numfiles = 0;
    vector<wxString> files_list;
    {
        wxCriticalSectionLocker lock(s_MutexProtection);
        TokenFilesSet::iterator it;
        for(it = m_pTokens->m_FilesToBeReparsed.begin(); it != m_pTokens->m_FilesToBeReparsed.end(); ++it)
        {
            m_pTokens->RemoveFile(*it);
            files_list.push_back(m_pTokens->m_FilenamesMap.GetString(*it));
            ++numfiles;
        }
    }
    if(!numfiles)
        return true;
    for(size_t i = 0; i < files_list.size();++i)
    {
        Parse(files_list[i],m_LocalFiles.count(files_list[i]));
    }
    return true;
}
