/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include <globals.h>
#include "token.h"
#include <wx/intl.h>
#include <wx/tokenzr.h>

inline void SaveTokenIdxSetToFile(wxOutputStream* f,const TokenIdxSet& data)
{
    SaveIntToFile(f, (int)(data.size()));
    for (TokenIdxSet::iterator it = data.begin(); it != data.end(); it++)
    {
        int num = *it;
        SaveIntToFile(f, num);
    }
}

bool LoadTokenIdxSetFromFile(wxInputStream* f,TokenIdxSet* data)
{
    if(!data)
        return false;
    bool result = true;
    data->clear();
    int size = 0;
    do
    {
        if (!LoadIntFromFile(f, &size))
        {
            result = false;
            break;
        }
        int i,num = 0;
        for (i = 0; i < size; i++)
        {
            if(!LoadIntFromFile(f,&num))
            {
                result = false;
                break;
            }
            data->insert(num);
        }
    }while(false);
    return result;
}

Token::Token()
    :
    m_File(0),
    m_Line(0),
    m_ImplFile(0),
    m_ImplLine(0),
    m_ImplLineStart(0),
    m_ImplLineEnd(0),
    m_Scope(tsUndefined),
    m_TokenKind(tkUndefined),
    m_IsOperator(false),
    m_IsLocal(false),
    m_IsTemp(false),
    m_ParentIndex(-1),
    m_pUserData(0),
    m_pTree(0),
    m_Self(-1)
{
    m_Ticket = GetTokenTicket();
}

Token::Token(const wxString& name, unsigned int file, unsigned int line)
    : m_Name(name),
    m_File(file),
    m_Line(line),
    m_ImplFile(0),
    m_ImplLine(0),
    m_ImplLineStart(0),
    m_ImplLineEnd(0),
    m_Scope(tsUndefined),
    m_TokenKind(tkUndefined),
    m_IsOperator(false),
    m_IsLocal(false),
    m_IsTemp(false),
    m_ParentIndex(-1),
    m_pUserData(0),
    m_pTree(0),
    m_Self(-1)
{
    //ctor
    m_Ticket = GetTokenTicket();

}

Token::~Token()
{
    //dtor
}

unsigned long Token::GetTokenTicket()
{
    static wxCriticalSection s_TicketProtection;
    static unsigned long ticket = 256; // Reserve some space for the class browser
    wxCriticalSectionLocker lock(s_TicketProtection);
    return ticket++;
}

wxString Token::GetParentName()
{
    wxString parentname = _T("");
    wxCriticalSectionLocker* lock = 0;
    if(m_pTree)
        lock = new wxCriticalSectionLocker(s_MutexProtection);
    Token* parent = GetParentToken();
    if(parent)
        parentname = parent->m_Name;
    if(lock)
        delete lock;
    return parentname;
}

wxString Token::DisplayName() const
{
//    wxString result(_T(""));
//    wxString parentname = GetParentName();
//    if (!parentname.IsEmpty())
//        result << parentname << _T("::");
    wxString result = GetNamespace();
    result << m_Name << m_Args;
    if (!m_Type.IsEmpty())
        result << _T(" : ") << m_Type;
    return result;
}

Token* Token::GetParentToken()
{
    Token* the_token = 0;
    if(!m_pTree)
        return 0;
    the_token = m_pTree->at(m_ParentIndex);
    return the_token;
}

wxString Token::GetFilename() const
{
    if(!m_pTree)
        return wxString(_T(""));
    return m_pTree->GetFilename(m_File);
}

wxString Token::GetImplFilename() const
{
    if(!m_pTree)
        return wxString(_T(""));
    return m_pTree->GetFilename(m_ImplFile);
}

bool Token::MatchesFiles(const TokenFilesSet& files)
{
    if(!files.size())
        return true;
    if(!m_File && !m_ImplFile)
        return true;
    if((m_File && files.count(m_File)) || (m_ImplFile && files.count(m_ImplFile)))
        return true;
    return false;
}

wxString Token::GetNamespace() const
{
    const wxString dcolon(_T("::"));
    wxString res;
    Token* parentToken = m_pTree->at(m_ParentIndex);
    while (parentToken)
    {
        res.Prepend(dcolon);
        res.Prepend(parentToken->m_Name);
        parentToken = parentToken->GetParentToken();
    }
    return res;
}

void Token::AddChild(int child)
{
    if (child >= 0)
        m_Children.insert(child);
}

bool Token::InheritsFrom(int idx) const
{
    if (idx < 0 || !m_pTree)
        return false;
    Token* token = m_pTree->at(idx);
    if(!token)
        return false;

    for (TokenIdxSet::iterator it = m_DirectAncestors.begin(); it != m_DirectAncestors.end(); it++)
    {
        int idx2 = *it;
        Token* ancestor = m_pTree->at(idx2);
        if(!ancestor)
            continue;
        if (ancestor == token || ancestor->InheritsFrom(idx))  // ##### is this intended?
            return true;
    }
    return false;
}

wxString Token::GetTokenKindString() const
{
    switch (m_TokenKind)
    {
        case tkClass: return _T("class");
        case tkNamespace: return _T("namespace");
        case tkTypedef: return _T("typedef");
        case tkEnum: return _T("enum");
        case tkEnumerator: return _T("enumerator");
        case tkFunction: return _T("function");
        case tkConstructor: return _T("constructor");
        case tkDestructor: return _T("destructor");
        case tkPreprocessor: return _T("preprocessor");
        case tkVariable: return _T("variable");
        default: return wxEmptyString; // tkUndefined
    }
}

wxString Token::GetTokenScopeString() const
{
    switch (m_Scope)
    {
        case tsPrivate: return _T("private");
        case tsProtected: return _T("protected");
        case tsPublic: return _T("public");
        default: return wxEmptyString;
    }
}

bool Token::SerializeIn(wxInputStream* f)
{
    bool result = true;
    do
    {
        if (!LoadIntFromFile(f, (int*)&m_Self))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_ParentIndex))
        {
            result = false;
            break;
        }
        if(m_ParentIndex < 0)
            m_ParentIndex = -1;

        if (!LoadStringFromFile(f, m_Type))
        {
            result = false;
            break;
        }
        if (!LoadStringFromFile(f, m_ActualType))
        {
            result = false;
            break;
        }
        if (!LoadStringFromFile(f, m_Name))
        {
            result = false;
            break;
        }
        if (!LoadStringFromFile(f, m_Args))
        {
            result = false;
            break;
        }
        if (!LoadStringFromFile(f, m_AncestorsString))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_File))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_Line))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_ImplFile))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_ImplLine))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_Scope))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_TokenKind))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_IsOperator))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_IsLocal))
        {
            result = false;
            break;
        }
        if(!LoadTokenIdxSetFromFile(f, &m_Ancestors))
        {
            result = false;
            break;
        }
        if(!LoadTokenIdxSetFromFile(f, &m_Children))
        {
            result = false;
            break;
        }
        if(!LoadTokenIdxSetFromFile(f, &m_Descendants))
        {
            result = false;
            break;
        }
    }while(false);
    return result;
}

bool Token::SerializeOut(wxOutputStream* f)
{
    SaveIntToFile(f, m_Self);
    SaveIntToFile(f, m_ParentIndex);
    SaveStringToFile(f, m_Type);
    SaveStringToFile(f, m_ActualType);
    SaveStringToFile(f, m_Name);
    SaveStringToFile(f, m_Args);
    SaveStringToFile(f, m_AncestorsString);
    SaveIntToFile(f, m_File);
    SaveIntToFile(f, m_Line);
    SaveIntToFile(f, m_ImplFile);
    SaveIntToFile(f, m_ImplLine);
    SaveIntToFile(f, m_Scope);
    SaveIntToFile(f, m_TokenKind);
    SaveIntToFile(f, m_IsOperator ? 1 : 0);
    SaveIntToFile(f, m_IsLocal ? 1 : 0);

    SaveTokenIdxSetToFile(f, m_Ancestors);
    SaveTokenIdxSetToFile(f, m_Children);
    SaveTokenIdxSetToFile(f, m_Descendants);
    return true;
}

// *** TokensTree ***

TokensTree::TokensTree():
m_modified(false)
{
    m_Tokens.clear();
    m_FilenamesMap.clear();
    m_FilesMap.clear();
    m_FilesStatus.clear();
    m_FreeTokens.clear();
    m_FilesToBeReparsed.clear();
    m_TopNameSpaces.clear();
    m_GlobalNameSpace.clear();

}


TokensTree::~TokensTree()
{
    clear();
}

void TokensTree::clear()
{
    m_Tree.clear();
    m_FilenamesMap.clear();
    m_FilesMap.clear();
    m_FilesStatus.clear();
    m_FilesToBeReparsed.clear();
    m_FreeTokens.clear();
    m_TopNameSpaces.clear();
    m_GlobalNameSpace.clear();

    size_t i;
    for(i = 0;i < m_Tokens.size(); i++)
    {
        Token* token = m_Tokens[i];
        if(token)
            delete token;
    }
    m_Tokens.clear();
}

size_t TokensTree::size()
{
    return m_Tokens.size();
}

size_t TokensTree::realsize()
{
    if(m_Tokens.size() <= m_FreeTokens.size())
        return 0;
    return m_Tokens.size() - m_FreeTokens.size();
}

int TokensTree::insert(Token* newToken)
{
    if(!newToken)
        return -1;
    return AddToken(newToken, -1);
}

int TokensTree::insert(int loc, Token* newToken)
{
    if(!newToken)
        return -1;

    return AddToken(newToken, loc);
}

int TokensTree::erase(int loc)
{
    if(!m_Tokens[loc])
        return 0;
    RemoveToken(loc);
    return 1;
}

void TokensTree::erase(Token* oldToken)
{
    RemoveToken(oldToken);
}

int TokensTree::TokenExists(const wxString& name, int parent, short int kindMask)
{
    int idx = m_Tree.GetItemNo(name);
    if(!idx)
        return -1;
    TokenIdxSet::iterator it;
    TokenIdxSet& curlist = m_Tree.GetItemAtPos(idx);
    int result = -1;
    for(it = curlist.begin(); it != curlist.end(); it++)
    {
        result = *it;
        if(result < 0 || (size_t)result >= m_Tokens.size())
            continue;
        Token* curtoken = m_Tokens[result];
        if(!curtoken)
            continue;
        if((parent<0 || curtoken->m_ParentIndex == parent) && curtoken->m_TokenKind & kindMask)
            return result;
    }
    return -1;
}

size_t TokensTree::FindMatches(const wxString& s,TokenIdxSet& result,bool caseSensitive,bool is_prefix, int kindMask)
{
    set<size_t> lists;
    result.clear();
    int numitems = m_Tree.FindMatches(s,lists,caseSensitive,is_prefix);
    if(!numitems)
        return 0;
    TokenIdxSet* curset;
    set<size_t>::iterator it;
    TokenIdxSet::iterator it2;
    for(it = lists.begin(); it != lists.end(); it++)
    {
        curset = &(m_Tree.GetItemAtPos(*it));
        for(it2 = curset->begin();it2 != curset->end(); it2++)
        {
            if (kindMask == 0xffff || (at(*it)->m_TokenKind & kindMask))
                result.insert(*it2);
        }
    }
    return result.size();
}

size_t TokensTree::FindTokensInFile(const wxString& file, TokenIdxSet& result, short int kindMask)
{
    result.clear();

    // get file idx
    if (!m_FilenamesMap.HasItem(file))
        return 0;
    int idx = m_FilenamesMap.GetItemNo(file);

    // now get the tokens set matching this file idx
    TokenFilesMap::iterator itf = m_FilesMap.find(idx);
    if (itf == m_FilesMap.end())
        return 0;
    TokenIdxSet& tokens = itf->second;

    // loop all results and add to final result set after filtering on token kind
    for (TokenIdxSet::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        Token* token = at(*it);
        if (kindMask & token->m_TokenKind)
            result.insert(*it);
    }
    return result.size();
}

int TokensTree::AddToken(Token* newToken,int forceidx)
{
    if(!newToken)
        return -1;

    const wxString& name = newToken->m_Name;
    static TokenIdxSet tmp_tokens = TokenIdxSet();
    // tmp_tokens.clear();

    // Insert the token's name and the token in the (inserted?) list
    size_t idx2 = m_Tree.AddItem(name,tmp_tokens,false);
    TokenIdxSet& curlist = m_Tree.GetItemAtPos(idx2);

    int newitem = AddTokenToList(newToken,forceidx);
    curlist.insert(newitem);
    m_FilesMap[newToken->m_File].insert(newitem);

    // Add Token (if applicable) to the namespaces indexes
    if(newToken->m_ParentIndex < 0)
    {
        newToken->m_ParentIndex = -1;
        m_GlobalNameSpace.insert(newitem);
        if(newToken->m_TokenKind == tkNamespace)
            m_TopNameSpaces.insert(newitem);
    }

    // All done!
    return newitem;
}

void TokensTree::RemoveToken(int idx)
{
    if(idx<0 || (size_t)idx >= m_Tokens.size())
        return;
    RemoveToken(m_Tokens[idx]);
}

void TokensTree::RemoveToken(Token* oldToken)
{
    if(!oldToken)
        return;
    int idx = oldToken->m_Self;
    if(m_Tokens[idx]!=oldToken)
        return;

    // Step 1: Detach token from its parent

    Token* parentToken = 0;
    if((size_t)(oldToken->m_ParentIndex) >= m_Tokens.size())
        oldToken->m_ParentIndex = -1;
    if(oldToken->m_ParentIndex >= 0)
        parentToken = m_Tokens[oldToken->m_ParentIndex];
    if(parentToken)
        parentToken->m_Children.erase(idx);

    TokenIdxSet nodes;
    TokenIdxSet::iterator it;

    // Step 2: Detach token from its ancestors
    nodes = (oldToken->m_DirectAncestors);
    for(it = nodes.begin();it!=nodes.end(); it++)
    {
        int ancestoridx = *it;
        if(ancestoridx < 0 || (size_t)ancestoridx >= m_Tokens.size())
            continue;
        Token* ancestor = m_Tokens[ancestoridx];
        if(ancestor)
            ancestor->m_Descendants.erase(idx);
    }
    oldToken->m_Ancestors.clear();
    oldToken->m_DirectAncestors.clear();

    // Step 3: Remove children
    nodes = (oldToken->m_Children); // Copy the list to avoid interference
    for(it = nodes.begin();it!=nodes.end(); it++)
        RemoveToken(*it);
    // m_Children SHOULD be empty by now - but clear anyway.
    oldToken->m_Children.clear();

    // Step 4: Remove descendants
    nodes = oldToken->m_Descendants; // Copy the list to avoid interference
    for(it = nodes.begin();it!=nodes.end(); it++)
        RemoveToken(*it);
    // m_Descendants SHOULD be empty by now - but clear anyway.
    oldToken->m_Descendants.clear();

    // Step 5: Detach token from the SearchTrees
    int idx2 = m_Tree.GetItemNo(oldToken->m_Name);
    if(idx2)
    {
        TokenIdxSet& curlist = m_Tree.GetItemAtPos(idx2);
        curlist.erase(idx);
    }

    // Now, from the global namespace (if applicable)
    if(oldToken->m_ParentIndex == -1)
    {
        m_GlobalNameSpace.erase(idx);
        m_TopNameSpaces.erase(idx);
    }

    // Step 6: Finally, remove it from the list.
    RemoveTokenFromList(idx);
}

int TokensTree::AddTokenToList(Token* newToken,int forceidx)
{
    int result = -1;
    if(!newToken)
        return -1;
    if(forceidx >= 0) // Reading from Cache?
    {
        if((size_t)forceidx >= m_Tokens.size())
        {
            int max = 250*((forceidx + 250) / 250);
            m_Tokens.resize((max),0); // fill next 250 items with null-values
        }
        m_Tokens[forceidx] = newToken;
        result = forceidx;
    }
    else // For Realtime Parsing
    {
        if(m_FreeTokens.size())
        {
            result = m_FreeTokens[m_FreeTokens.size() - 1];
            m_FreeTokens.pop_back();
            m_Tokens[result] = newToken;
        }
        else
        {
            result = m_Tokens.size();
            m_Tokens.push_back(newToken);
        }
    }

    newToken->m_pTree = this;
    newToken->m_Self = result;
    // Clean up extra string memory

    newToken->m_Type.Shrink();
    newToken->m_Name.Shrink();
    newToken->m_Args.Shrink();
    newToken->m_AncestorsString.Shrink();

    return result;
}

void TokensTree::RemoveTokenFromList(int idx)
{
    if(idx < 0 || (size_t)idx >= m_Tokens.size())
        return;
    Token* oldToken = m_Tokens[idx];
    if(oldToken)
    {
        m_Tokens[idx] = 0;
        m_FreeTokens.push_back(idx);
        m_FilesToBeReparsed.insert(oldToken->m_File);
        delete oldToken;
    }
}

void TokensTree::RemoveFile(const wxString& filename)
{
    int index = GetFileIndex(filename);
    RemoveFile(index);
}

void TokensTree::RemoveFile(int index)
{
    if(index <=0)
        return;
    TokenIdxSet& the_list = m_FilesMap[index];
    TokenIdxSet::iterator it;
    for(it = the_list.begin(); it != the_list.end();it++)
    {
        int idx = *it;
        if(idx < 0 || (size_t)idx > m_Tokens.size())
            continue;
        Token* the_token = at(idx);
        if(!the_token)
            continue;

        // do not remove token lightly...
        // only if both its decl filename and impl filename are either empty or match this file
        // if one of those filenames do not match the above criteria
        // just clear the relevant info, leaving the token where it is...

        bool match1 = the_token->m_File == 0 || (int)the_token->m_File == index;
        bool match2 = the_token->m_ImplFile == 0 || (int)the_token->m_ImplFile == index;
        if (match1 && match2)
            RemoveToken(the_token); // safe to remove the token
        else
        {
            // do not remove token, just clear the matching info
            if (match1)
            {
                the_token->m_File = 0;
                the_token->m_Line = 0;
            }
            else if (match2)
            {
                the_token->m_ImplFile = 0;
                the_token->m_ImplLine = 0;
            }
        }
    }
    the_list.clear();
}

void TokensTree::FreeTemporaries()
{
    int i;
    for(i = m_Tokens.size() -1;i >= 0;i--)
    {
        Token* token = m_Tokens[i];
        if (token && token->m_IsTemp)
            RemoveToken(token);
    }
}

void TokensTree::RecalcFreeList()
{
    m_FreeTokens.clear();
    int i;
    for(i = m_Tokens.size() -1;i >= 0;i--)
    {
        if(!m_Tokens[i])
            m_FreeTokens.push_back(i);
    }
}

void TokensTree::RecalcData()
{
//    Manager::Get()->GetLogManager()->DebugLog(_T("Calculating full inheritance tree"));

    // first loop to convert ancestors string to token indices for each token
    for (size_t i = 0; i < size(); ++i)
    {
        Token* token = at(i);
        if (!token)
            continue;

        if (!(token->m_TokenKind & (tkClass | tkTypedef | tkEnum)))
            continue;
        if (token->m_AncestorsString.IsEmpty())
            continue;
        // only local symbols might change inheritance
//        if (!token->m_IsLocal)
//            continue;

        token->m_DirectAncestors.clear();
        token->m_Ancestors.clear();
//        Manager::Get()->GetLogManager()->DebugLog(_T(" : '%s'"), token->m_Name.c_str());

        //Manager::Get()->GetLogManager()->DebugLog("Token %s, Ancestors %s", token->m_Name.c_str(), token->m_AncestorsString.c_str());
        wxStringTokenizer tkz(token->m_AncestorsString, _T(","));
        while (tkz.HasMoreTokens())
        {
            wxString ancestor = tkz.GetNextToken();
            if (ancestor.IsEmpty() || ancestor == token->m_Name)
                continue;
//            Manager::Get()->GetLogManager()->DebugLog(_T("Ancestor %s"), ancestor.c_str());
            // ancestors might contain namespaces, e.g. NS::Ancestor
            if (ancestor.Find(_T("::")) != wxNOT_FOUND)
            {
                Token* ancestorToken = 0;
                wxStringTokenizer anctkz(ancestor, _T("::"));
                while (anctkz.HasMoreTokens())
                {
                    wxString ns = anctkz.GetNextToken();
                    if (!ns.IsEmpty())
                    {
                        int ancestorIdx = TokenExists(ns, ancestorToken ? ancestorToken->GetSelf() : -1, tkNamespace | tkClass | tkTypedef);
                        ancestorToken = at(ancestorIdx);
//                        ancestorToken = token->HasChildToken(ns, tkNamespace | tkClass);
                        if (!ancestorToken) // unresolved
                            break;
                    }
                }
                if (ancestorToken && ancestorToken != token && ancestorToken->m_TokenKind == tkClass)// && !ancestorToken->m_IsTypedef)
                {
//                    Manager::Get()->GetLogManager()->DebugLog(_T("Resolved to %s"), ancestorToken->m_Name.c_str());
                    token->m_Ancestors.insert(ancestorToken->GetSelf());
                    ancestorToken->m_Descendants.insert(i);
//                    Manager::Get()->GetLogManager()->DebugLog(_T("   + '%s'"), ancestorToken->m_Name.c_str());
                }
//                else
//                    Manager::Get()->GetLogManager()->DebugLog(_T("   ! '%s' (unresolved)"), ancestor.c_str());
            }
            else // no namespaces in ancestor
            {
                // accept multiple matches for inheritance
                TokenIdxSet result;
                FindMatches(ancestor, result, true, false);
                for (TokenIdxSet::iterator it = result.begin(); it != result.end(); it++)
                {
                    Token* ancestorToken = at(*it);
                    // only classes take part in inheritance
                    if (ancestorToken && ancestorToken != token && (ancestorToken->m_TokenKind == tkClass || ancestorToken->m_TokenKind == tkEnum))// && !ancestorToken->m_IsTypedef)
                    {
                        token->m_Ancestors.insert(*it);
                        ancestorToken->m_Descendants.insert(i);
//                        Manager::Get()->GetLogManager()->DebugLog(_T("   + '%s'"), ancestorToken->m_Name.c_str());
                    }
                }
//                if (result.empty())
//                    Manager::Get()->GetLogManager()->DebugLog(_T("   ! '%s' (unresolved)"), ancestor.c_str());
            }
        }

        token->m_DirectAncestors = token->m_Ancestors;

        if (!token->m_IsLocal) // global symbols are linked once
        {
            //Manager::Get()->GetLogManager()->DebugLog("Removing ancestor string from %s", token->m_Name.c_str(), token->m_Name.c_str());
            token->m_AncestorsString.Clear();
        }
    }

    // second loop to calculate full inheritance for each token
    for (size_t i = 0; i < size(); ++i)
    {
        Token* token = at(i);
        if (!token)
            continue;

        if (!(token->m_TokenKind & (tkClass | tkTypedef | tkEnum)))
            continue;

        // recalc
        TokenIdxSet result;
        for (TokenIdxSet::iterator it = token->m_Ancestors.begin(); it != token->m_Ancestors.end(); it++)
            RecalcFullInheritance(*it, result);

        // now, add the resulting set to ancestors set
        for (TokenIdxSet::iterator it = result.begin(); it != result.end(); it++)
        {
            Token* ancestor = at(*it);
            if (ancestor)
            {
                token->m_Ancestors.insert(*it);
                ancestor->m_Descendants.insert(i);
            }
        }

//        // debug loop
//        Manager::Get()->GetLogManager()->DebugLog(_T("Ancestors for %s:"),token->m_Name.c_str());
//        for (TokenIdxSet::iterator it = token->m_Ancestors.begin(); it != token->m_Ancestors.end(); it++)
//            Manager::Get()->GetLogManager()->DebugLog(_T(" + %s"), at(*it)->m_Name.c_str());
    }
//    Manager::Get()->GetLogManager()->DebugLog(_T("Full inheritance calculated."));
}

// caches the inheritance info for each token (recursive function)
void TokensTree::RecalcFullInheritance(int parentIdx, TokenIdxSet& result)
{
    // no parent token? no ancestors...
    if (parentIdx == -1)
        return;

    // no parent token? no ancestors...
    Token* ancestor = at(parentIdx);
    if (!ancestor)
        return;

    // only classes take part in inheritance
    if (!(ancestor->m_TokenKind & (tkClass | tkTypedef)))
        return;
//    Manager::Get()->GetLogManager()->DebugLog(_T("Anc: '%s'"), ancestor->m_Name.c_str());

    // for all its ancestors
    for (TokenIdxSet::iterator it = ancestor->m_Ancestors.begin(); it != ancestor->m_Ancestors.end(); it++)
    {
        if (*it != -1 && // not global scope
            *it != parentIdx && // not the same token (avoid infinite loop)
            result.find(*it) == result.end()) // not already in the set (avoid infinite loop)
        {
            // add it to the set
            result.insert(*it);
            // and recurse for its ancestors
            RecalcFullInheritance(*it, result);
        }
    }
}

Token* TokensTree::GetTokenAt(int idx)
{
    if(idx < 0 || (size_t)idx >= m_Tokens.size())
        return 0;
    return m_Tokens[idx];
}

size_t TokensTree::GetFileIndex(const wxString& filename)
{
    size_t result = m_FilenamesMap.insert(filename);
    // Insert does not alter the tree if the filename is already found.
    return result;
}

const wxString TokensTree::GetFilename(size_t idx) const
{
    wxString result = m_FilenamesMap.GetString(idx);
    return result;
}

bool TokensTree::IsFileParsed(const wxString& filename)
{
    size_t index = GetFileIndex(filename);
    bool parsed = (m_FilesMap.count(index) &&
                   m_FilesStatus[index]!=fpsNotParsed &&
                  !m_FilesToBeReparsed.count(index)
                  );
    return parsed;
}

void TokensTree::MarkFileTokensAsLocal(const wxString& filename, bool local, void* userData)
{
    MarkFileTokensAsLocal(GetFileIndex(filename), local, userData);
}

void TokensTree::MarkFileTokensAsLocal(size_t file, bool local, void* userData)
{
    if (file == 0)
        return;

    TokenIdxSet& tokens = m_FilesMap[file];
    for (TokenIdxSet::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        Token* token = m_Tokens.at(*it);
        if (token)
        {
            token->m_IsLocal = local;
            token->m_pUserData = userData;
        }
    }
}

size_t TokensTree::ReserveFileForParsing(const wxString& filename,bool preliminary)
{
    size_t index = GetFileIndex(filename);
    if(m_FilesToBeReparsed.count(index) &&
       (!m_FilesStatus.count(index) || m_FilesStatus[index]==fpsDone))
    {
        RemoveFile(filename);
        m_FilesToBeReparsed.erase(index);
        m_FilesStatus[index]=fpsNotParsed;
    }
    if(m_FilesStatus.count(index))
    {
        FileParsingStatus status = m_FilesStatus[index];
        if(preliminary)
        {
            if(status >= fpsAssigned)
                return 0; // Already assigned
        }
        else
        {
            if(status > fpsAssigned)
                return 0; // No parsing needed
        }
    }
    m_FilesToBeReparsed.erase(index);
    m_FilesStatus[index]=preliminary ? fpsAssigned : fpsBeingParsed; // Reserve file
    return index;
}

void TokensTree::FlagFileForReparsing(const wxString& filename)
{
    m_FilesToBeReparsed.insert(GetFileIndex(filename));
}

void TokensTree::FlagFileAsParsed(const wxString& filename)
{
    m_FilesStatus[GetFileIndex(filename)]=fpsDone;
}
