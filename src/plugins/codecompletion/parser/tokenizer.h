/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <wx/string.h>
#include <wx/hashmap.h>
#include <configmanager.h>
#include <filemanager.h>
#include "token.h"

#include <stack>

class HashForWxStringMap
{
public:
    HashForWxStringMap() {}
    unsigned long operator()(const wxString& x) const
    {
        const size_t len = x.length();
        const size_t intWxChar = sizeof(unsigned int) / sizeof(wxChar);
        const size_t shortWxChar = sizeof(unsigned short) / sizeof(wxChar);
        if (len >= intWxChar)
            return size_t((128 ^ len) + *((unsigned int*)(x.c_str() + len - intWxChar)));
        else if (len >= shortWxChar)
            return size_t((256 ^ len) + *((unsigned short*)(x.c_str() + len - shortWxChar)));
        else
            return size_t((512 ^ len) + *(x.c_str() + len - 1));
    }
    HashForWxStringMap& operator=(const HashForWxStringMap&) { return *this; }
};

class EqualForWxStringMap
{
public:
    EqualForWxStringMap() { }
    bool operator()(const wxString& a, const wxString& b) const { return a == b; }
    EqualForWxStringMap& operator=(const EqualForWxStringMap&) { return *this; }
};

WX_DECLARE_HASH_MAP(wxString, wxString, HashForWxStringMap, EqualForWxStringMap, wxStringHashMap);

enum TokenizerState
{
    tsSkipEqual        = 0x0001,
    tsSkipQuestion     = 0x0002,
    tsSkipSubScrip     = 0x0004,
    tsSingleAngleBrace = 0x0008,
    tsReadRawExpression= 0x0010,

    tsSkipNone         = 0x1000,
    // convenient masks
    tsSkipUnWanted     = tsSkipEqual    | tsSkipQuestion | tsSkipSubScrip,
    tsTemplateArgument = tsSkipUnWanted | tsSingleAngleBrace
};


struct TokenizerOptions
{
    bool wantPreprocessor;
};

class Tokenizer
{
public:
    Tokenizer(TokensTree* tokensTree, const wxString& filename = wxEmptyString);
    ~Tokenizer();

    bool Init(const wxString& filename = wxEmptyString, LoaderBase* loader = 0);
    bool InitFromBuffer(const wxString& buffer);


    wxString GetToken();
    wxString PeekToken();
    void     UngetToken();

    void SetTokenizerOption(bool wantPreprocessor)
    {
      m_TokenizerOptions.wantPreprocessor = wantPreprocessor;
    };

    void SetState(TokenizerState state)
    {
        m_State = state;
    };

    TokenizerState GetState()
    {
        return m_State;
    }

    bool IsSkippingUnwantedTokens() const
    {
        return (m_State == tsSkipUnWanted);
    };

    const wxString& GetFilename() const
    {
        return m_Filename;
    };

    unsigned int GetLineNumber() const
    {
        return m_LineNumber;
    };

    unsigned int GetNestingLevel() const
    {
        return m_NestLevel;
    };

    void SaveNestingLevel() // the parser might need to ignore the nesting level in some cases
    {
        m_SavedNestingLevel = m_NestLevel;
    };

    void RestoreNestingLevel()
    {
        m_NestLevel = m_SavedNestingLevel;
    };

    bool IsOK() const
    {
        return m_IsOK;
    };

    wxString ReadToEOL(bool nestBraces = true, bool stripComment = false); // use with care outside this class!
    wxString ReadBlock(const wxChar& leftBrace); // the argument must be one of: ( [ < {

    bool SkipToEOL(bool nestBraces = true, bool skippingComment = false); // use with care outside this class!

    static void SetReplacementString(const wxString& from, const wxString& to)
    {
        s_Replacements[from] = to;
    };

    static void RemoveReplacementString(const wxString& from)
    {
        wxStringHashMap::iterator it = s_Replacements.find(from);
        if (it != s_Replacements.end())
            s_Replacements.erase(it);
    };
    
    static wxStringHashMap& GetTokenReplacementsMap()
    {
        return s_Replacements;
    }
    
    static void ConvertToHashReplacementMap(const ConfigManagerContainer::StringToStringMap& map)
    {
        ConfigManagerContainer::StringToStringMap::const_iterator it = map.begin();
        for (; it != map.end(); it++)
        {
            s_Replacements[it->first] = it->second;
        }
    }

    bool IsEOF() const
    {
        return m_TokenIndex >= m_BufferLen;
    };

    bool NotEOF() const
    {
        return m_TokenIndex < m_BufferLen;
    };

protected:
    void BaseInit();
    wxString DoGetToken();
    bool ReadFile();
    bool IsEscapedChar();
    bool SkipToChar(const wxChar& ch);
    bool SkipToOneOfChars(const wxChar* chars, bool supportNesting = false, bool skipPreprocessor = false, bool skipAngleBrace = true);
    bool SkipBlock(const wxChar& ch);
    bool SkipUnwanted(); // skips whitespace, comments, assignments etc.
    bool SkipWhiteSpace();
    bool SkipComment();
    bool SkipString();
    bool SkipToStringEnd(const wxChar& ch);

    bool MoveToNextChar(const unsigned int amount = 1)
    {
        assert(amount);
        if(amount == 1) // compiler will dead-strip this
        {
            ++m_TokenIndex;
            if (IsEOF())
            {
                m_TokenIndex = m_BufferLen;
                return false;
            }

            if (CurrentChar() == _T('\n'))
                ++m_LineNumber;
            return true;
        }
        else
        {
            m_TokenIndex += amount;
            if (IsEOF())
            {
                m_TokenIndex = m_BufferLen;
                return false;
            }

            if (CurrentChar() == _T('\n'))
                ++m_LineNumber;
            return true;
        }
    };

    wxChar CurrentChar() const
    {
        if(m_TokenIndex < m_BufferLen)
            return m_Buffer.GetChar(m_TokenIndex);
        return 0;
    };

    wxChar CurrentCharMoveNext()
    {
        wxChar c = CurrentChar();
        m_TokenIndex++;
        return c;
    };

    wxChar NextChar() const
    {
        if ((m_TokenIndex + 1) >= m_BufferLen) // m_TokenIndex + 1) < 0  can never be true
            return 0;

        return m_Buffer.GetChar(m_TokenIndex + 1);
    };

    wxChar PreviousChar() const
    {
        if ( ((m_TokenIndex - 1) < 0) || (m_BufferLen==0) ) // (m_TokenIndex - 1) >= m_BufferLen can never be true
            return 0;

        return m_Buffer.GetChar(m_TokenIndex - 1);
    };

private:
    inline bool CharInString(const wxChar ch, const wxChar* chars) const
    {
        int len = wxStrlen(chars);
        for (int i = 0; i < len; ++i)
        {
            if (ch == chars[i])
                return true;
        }
        return false;
    };

    inline const wxString& ThisOrReplacement(const wxString& str) const
    {
        wxStringHashMap::const_iterator it = s_Replacements.find(str);
        if (it != s_Replacements.end())
            return it->second;
        return str;
    };

    wxString MacroReplace(const wxString str);

    /** Get the value of pre-processor
      * @param 'found' MUST BE Initialized to false, 'value' MUST BE Initialized to -1
      * @return finded or not)
      */
    void GetPreprocessorValue(const wxString& token, bool& found, long& value);

    /** Judge what is the first block
      * If call this function, it will call 'SkipToEOL(false, true)' final.
      */
    bool CalcConditionExpression();

    /** If the macro defined, return true
      * If call this function, it will call 'SkipToEOL(false, true)' final.
      */
    bool IsMacroDefined();

    /** Skip to next condition preprocessor, To mark #else #elif #endif as the end
      */
    void SkipToNextConditionPreprocessor();

    /** Skip to end condition preprocessor, To mark #endif as the end
      */
    void SkipToEndConditionPreprocessor();

    /** handle the proprocessor directive:
      * #ifdef XXX or #endif or #if or #elif or...
      * If handled condition preprocessor, return true; if Un-condition preprocessor, return false
      */
    bool HandleConditionPreprocessor();

    TokenizerOptions m_TokenizerOptions;
    TokensTree*      m_pTokensTree;
    wxString         m_Filename;
    wxString         m_Buffer;
    unsigned int     m_BufferLen;

    wxString         m_Token;
    unsigned int     m_TokenIndex;
    unsigned int     m_LineNumber;
    unsigned int     m_NestLevel; // keep track of block nesting { }
    unsigned int     m_SavedNestingLevel;

    unsigned int     m_UndoTokenIndex;
    unsigned int     m_UndoLineNumber;
    unsigned int     m_UndoNestLevel;

    bool             m_PeekAvailable;
    wxString         m_PeekToken;
    unsigned int     m_PeekTokenIndex;
    unsigned int     m_PeekLineNumber;
    unsigned int     m_PeekNestLevel;

    bool             m_IsOK;
    bool             m_IsOperator;

    TokenizerState   m_State;

    LoaderBase*      m_pLoader;

    /** Calc Expression's result, true or false */
    std::stack<bool> m_ExpressionResult;

    static wxStringHashMap s_Replacements;
};

#endif // TOKENIZER_H


