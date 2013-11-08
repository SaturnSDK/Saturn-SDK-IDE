/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef TOKENTREE_H
#define TOKENTREE_H

#include <wx/string.h>
#include <wx/thread.h>

#include <vector>
#include <deque>

#include "token.h"
#include "searchtree.h"

enum FileParsingStatus
{ fpsNotParsed = 0, fpsAssigned, fpsBeingParsed, fpsDone };

typedef std::deque<int>                                          TokenIdxList;
typedef std::vector<Token*>                                      TokenList;
typedef SearchTree<TokenIdxSet>                                  TokenSearchTree;
typedef BasicSearchTree                                          TokenFilenameMap;
typedef std::map< size_t, TokenIdxSet,       std::less<size_t> > TokenFileMap;
typedef std::map< size_t, FileParsingStatus, std::less<size_t> > TokenFileStatusMap;
typedef std::map< int, wxString >                                TokenIdxStringMap;

extern wxMutex s_TokenTreeMutex;

class TokenTree
{
    friend class CCDebugInfo;
    friend class CCTest;
    friend class CCTestFrame;
public:

    TokenTree();
    virtual ~TokenTree();

    // STL compatibility functions
    void                  clear();
    inline Token* operator[](int idx)       { return GetTokenAt(idx); }
    inline Token*         at(int idx)       { return GetTokenAt(idx); }
    inline const Token *  at(int idx) const { return GetTokenAt(idx); }
    size_t                size();
    size_t                realsize();
    inline bool           empty()           { return size()==0;       }
    int                   insert(Token* newToken);
    int                   insert(int loc, Token* newToken);
    int                   erase(int loc);
    void                  erase(Token* oldToken);

    inline void Clear()                     { clear();                }

    // Token specific functions
    void   RecalcFreeList();

    // This will convert the Token's ancestor string to it's IDs
    void   RecalcInheritanceChain(Token* token);

    int    TokenExists(const wxString& name, int parent, short int kindMask);
    int    TokenExists(const wxString& name, const wxString& baseArgs, int parent, TokenKind kind);
    int    TokenExists(const wxString& name, const TokenIdxSet& parents, short int kindMask);
    int    TokenExists(const wxString& name, const wxString& baseArgs, const TokenIdxSet& parents, TokenKind kind);
    size_t FindMatches(const wxString& query, TokenIdxSet& result, bool caseSensitive, bool is_prefix, TokenKind kindMask = tkUndefined);
    size_t FindTokensInFile(const wxString& filename, TokenIdxSet& result, short int kindMask);
    void   RemoveFile(const wxString& filename);
    void   RemoveFile(int fileIndex);

    // Protected access to internal lists / maps
    const TokenList*     GetTokens() const                        { return &m_Tokens;              }
    const TokenIdxSet*   GetGlobalNameSpaces() const              { return &m_GlobalNameSpaces;    }
    const TokenFileMap*  GetFilesMap() const                      { return &m_FileMap;            }
    const TokenIdxSet*   GetTokensBelongToFile(size_t fileIdx) const
    {
      TokenFileMap::const_iterator it = m_FileMap.find(fileIdx);
      return (it == m_FileMap.end() ? 0 : &(it->second));
    }
    const TokenFileSet* GetFilesToBeReparsed() const                   { return &m_FilesToBeReparsed;   }

    size_t       GetFileMapSize() const                                { return m_FileMap.size();             }
    void         InsertTokenBelongToFile(size_t fileIdx, int tokenIdx) { m_FileMap[fileIdx].insert(tokenIdx); }
    void         EraseFileMapInFileMap(size_t fileIdx)                 { m_FileMap.erase(fileIdx);            }

    size_t       GetFileStatusCountForIndex(size_t fileIdx) const      { return m_FileStatusMap.count(fileIdx);  }
    void         EraseFileStatusByIndex(size_t fileIdx)                { m_FileStatusMap.erase(fileIdx);         }

    void         EraseFilesToBeReparsedByIndex(size_t fileIdx)         { m_FilesToBeReparsed.erase(fileIdx);   }

    // Parsing related functions
    size_t         InsertFileOrGetIndex(const wxString& filename);
    size_t         GetFileMatches(const wxString& filename, std::set<size_t>& result, bool caseSensitive, bool is_prefix);
    size_t         GetFileIndex(const wxString& filename);
    const wxString GetFilename(size_t fileIdx) const;
    size_t         ReserveFileForParsing(const wxString& filename, bool preliminary = false);
    void           FlagFileForReparsing(const wxString& filename);
    void           FlagFileAsParsed(const wxString& filename);
    bool           IsFileParsed(const wxString& filename);

    void MarkFileTokensAsLocal(const wxString& filename, bool local = true, void* userData = 0);
    void MarkFileTokensAsLocal(size_t fileIdx, bool local = true, void* userData = 0);

    // Documentation related functions
    void AppendDocumentation(int tokenIdx, const wxString& doc);
    wxString GetDocumentation(int tokenIdx);

    size_t        m_TokenTicketCount;

protected:
    Token*        GetTokenAt(int idx);
    Token const * GetTokenAt(int idx) const;
    int           AddToken(Token* newToken, int fileIdx);

    void          RemoveToken(int idx);
    void          RemoveToken(Token* oldToken);

    int           AddTokenToList(Token* newToken, int forceidx);
    void          RemoveTokenFromList(int idx);

    void RecalcFullInheritance(int parentIdx, TokenIdxSet& result); // called by RecalcData

    /** Check all the children belong this token should be removed
      * @param token the checked token pointer
      * @param fileIdx file index the token belongs to
      * @return if true, we can safely remove the token
      */
    bool CheckChildRemove(const Token* token, int fileIdx);

    /** This is a string->TokenIndexSet map. E.g. we have a class Token named "AAA", also, we can
     *  have a function Token named "AAA", they are  different Tokens, but they share the same name.
     *  So we may have an tree point "AAA" -> <30, 40> map in the TokenSearchTree. Note here 30 and
     *  40 are the indexes (slots) in the m_Tokens vector, which holds the Tokens pointers. In-fact,
     * "AAA" -> <30, 40> is not mapped directly, it has a indirect map like "AAA" -> 16 -> <30, 40>,
     *  the middle number 16 here is the tree point item index in the BasicSearchTree, as we have
     *  a std::vector< TokenIndexSet > in the SearchTree, the 16 is the index to fetch the value
     *  <30,40> in the vector
     */
	TokenSearchTree   m_Tree;

    /** Contains the pointers to all the Token instances, it is just a std::vector<Token*>, the
     *  suggest way to access a Token instance is by first get its index in the m_Tokens, then get
     *  its address by m_Tokens[index], the reason we have such indirect access is that there are
     *  many Tokens which can reference each other, it is much safe using index instead of raw
     *  pointers.
     */
	TokenList         m_Tokens;

	/** List of all the deleted (and available) tokens. When more and more Tokens were allocated,
	 *  their address was recorded in the m_Tokens, m_Tokens grows larger, but if we delete some
	 *  Tokens, the will have some empty slots in the std::vector<Token*>, we need to reuse those
	 *  slots, here m_FreeTokens is to hold those empty slots for reuse, so we don't waste empty
	 *  slots in the m_Tokens */
	TokenIdxList      m_FreeTokens;

    /** List of tokens belonging to the global namespace */
    TokenIdxSet       m_TopNameSpaces;
    TokenIdxSet       m_GlobalNameSpaces;

    TokenFilenameMap    m_FilenameMap;         /** Map: file names -> file indices */
    TokenFileMap        m_FileMap;             /** Map: file indices -> sets of TokenIndexes */
    TokenFileStatusMap  m_FileStatusMap;       /** Map: file indices -> status */
    TokenFileSet        m_FilesToBeReparsed;   /** Set: file indices */

    TokenIdxStringMap   m_TokenDocumentationMap; /** Map: token index -> documentation */
};

#endif // TOKENTREE_H
