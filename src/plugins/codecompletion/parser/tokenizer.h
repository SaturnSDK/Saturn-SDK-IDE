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

/** @brief This is just a simple lexer class
  *
  * A Tokenizer do the lexical analysis on an buffer. The buffer is a wxString either open from a file
  * or a wxString already in memory. The most public interface is the two member function:
  * GetToken() and PeekToken(). The first one just eat a token string, and the later one do a "look ahead"
  * on the next token string, thus, the peeked string will be buffered to the next GetToken() call, this can
  * improve the performance.
  * Also, Tokenizer class do some kind of handling Marcro on the buffer, see
  * member-function MacroReplace(). Furtherly, more functions like "conditional preprocessor" handling can
  * be added.
  */
class Tokenizer
{
public:
    /** Tokenizer constructor.
     * @param filename the file to be opened.
     */
    Tokenizer(const wxString& filename = wxEmptyString);
    Tokenizer(TokensTree* tokensTree, const wxString& filename = wxEmptyString);

    /** ParserThread destructor.*/
    ~Tokenizer();

    /** Initialize the buffer by opening a file through a loader. */
    bool Init(const wxString& filename = wxEmptyString, LoaderBase* loader = 0);
    /** Initialize the buffer by directly use a wxString reference. */
    bool InitFromBuffer(const wxString& buffer);

    /** Consume and return the current token string. */
    wxString GetToken();
    /** Do a "look ahead", and return the next token string. */
    wxString PeekToken();
    /** Undo the GetToken. */
    void     UngetToken();

    /** doc me */
    void SetTokenizerOption(bool wantPreprocessor)
    {
      m_TokenizerOptions.wantPreprocessor = wantPreprocessor;
    };

    /** Set the Tokenizer Skipping options. Eg, sometimes, we need to skip the statement after "=",
     * but sometimes, we should disable this options, see more details on TokenizerState.
     */
    void SetState(TokenizerState state)
    {
        m_State = state;
    };
    /** Return the Skipping options value, see TokenizerState for more details.*/
    TokenizerState GetState()
    {
        return m_State;
    }

    /** Checking the TokenizerState */
    bool IsSkippingUnwantedTokens() const
    {
        return (m_State == tsSkipUnWanted);
    };

    /** Return the opened files name */
    const wxString& GetFilename() const
    {
        return m_Filename;
    };

    /** Return the line number of the current token string */
    unsigned int GetLineNumber() const
    {
        return m_LineNumber;
    };

    /** Return the brace "{" level */
    unsigned int GetNestingLevel() const
    {
        return m_NestLevel;
    };

    /** Save the brace level, the parser might need to ignore the nesting level in some cases */
    void SaveNestingLevel()
    {
        m_SavedNestingLevel = m_NestLevel;
    };

    /** Restore the brace level */
    void RestoreNestingLevel()
    {
        m_NestLevel = m_SavedNestingLevel;
    };

    /** If the buffer is correctly loaded, this function return true. */
    bool IsOK() const
    {
        return m_IsOK;
    };

    /** return the string from the current position to the end of current line, in most case, this
     * function is used in handling #define, use with care outside this class!
     */
    wxString ReadToEOL(bool nestBraces = true, bool stripComment = false); // use with care outside this class!

    /** // the argument must be one of: ( [ < { */
    wxString ReadBlock(const wxChar& leftBrace);

    /** Skip fron the current position to the end of line.
     * @param nestBraces if true, we should still couting the brace levels in this function.
     */
    bool SkipToEOL(bool nestBraces = true); // use with care outside this class!

    /** Skip to then end of the C++ style comemnt */
    bool SkipToInlineCommentEnd();

    /** Add one Replacement rules, this is just a simple way of handling preprocessor (macro) replacement.
     * the rule composite of two strings. if the first string has found in
     * the Tokenizer, the it will retunrn the second string instead. We have more replacement rulse to
     * expand this sinple replacement.
     * for replace the "_GLIBCXX_BEGIN_NAMESPACE(std)" to  "namespace std {"
     * we can use: Tokenizer::SetReplacementString(_T("_GLIBCXX_BEGIN_NAMESPACE"), _T("+namespace"));
     * see more details in CodeCompletion::LoadTokenReplacements() function body.
     * @param from the matching key string
     * @param to the mathing value
     */
    static void SetReplacementString(const wxString& from, const wxString& to)
    {
        s_Replacements[from] = to;
    };
    /** Remove a replacement rule */
    static void RemoveReplacementString(const wxString& from)
    {
        wxStringHashMap::iterator it = s_Replacements.find(from);
        if (it != s_Replacements.end())
            s_Replacements.erase(it);
    };
    /** return the macro replacement map */
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

    /** Check wether the Tokenizer reached the end of the buffer (file) */
    bool IsEOF() const
    {
        return m_TokenIndex >= m_BufferLen;
    };
    /** return true if it is Not the end of buffer */
    bool NotEOF() const
    {
        return m_TokenIndex < m_BufferLen;
    };

protected:
    /** Initialize some member variables */
    void BaseInit();

    /** Do the job of lexer, both GetToken() and PeekToken will internally call this function */
    wxString DoGetToken();

    /** Read a file, and fill the m_Buffer */
    bool ReadFile();
    /** Check the current character is a C-Escape character in a string. */
    bool IsEscapedChar();

    /** Skip every character until we meet a ch */
    bool SkipToChar(const wxChar& ch);

    /** Skip every until we meet any characters in a wxChar Array,
     * @param supportNesting handling brace level in this function.
     * @param skipPreprocessor handling preprocessor directive.
     * @param skipAngleBrace if this value is false, we will not do a match of "< > "
     */
    bool SkipToOneOfChars(const wxChar* chars, bool supportNesting = false, bool skipPreprocessor = false, bool skipAngleBrace = true);

    /** Skip the blocks like <>, {}, [], () */
    bool SkipBlock(const wxChar& ch);

    /** skips comments, assignments, preprocessor etc. Eg, sometimes, it will skip the statement after
     * the "=" statement (depend on the TokenizerState value).
     */
    bool SkipUnwanted();

    /** Skip any "tab" "whitespace" */
    bool SkipWhiteSpace();

    /** Skip the C/C++ comment */
    bool SkipComment();

    /** Skip the "XXXX" or 'X' */
    bool SkipString();

    /** Move to the end of "XXXX" or 'X' */
    bool SkipToStringEnd(const wxChar& ch);

    /** Move to the next chracter in the buffer, amount defines the stpe (by default, it is one) */
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

    /** Return the current character indexed by m_TokenIndex in the m_Buffer */
    wxChar CurrentChar() const
    {
        if(m_TokenIndex < m_BufferLen)
            return m_Buffer.GetChar(m_TokenIndex);
        return 0;
    };

    /** Do both of the previous two functions */
    wxChar CurrentCharMoveNext()
    {
        wxChar c = CurrentChar();
        m_TokenIndex++;
        return c;
    };

    /** Return (peek)the next character */
    wxChar NextChar() const
    {
        if ((m_TokenIndex + 1) >= m_BufferLen) // m_TokenIndex + 1) < 0  can never be true
            return 0;

        return m_Buffer.GetChar(m_TokenIndex + 1);
    };

    /** Return (peek) the previous character */
    wxChar PreviousChar() const
    {
        if ( ((m_TokenIndex - 1) < 0) || (m_BufferLen==0) ) // (m_TokenIndex - 1) >= m_BufferLen can never be true
            return 0;

        return m_Buffer.GetChar(m_TokenIndex - 1);
    };

private:
    /** Check if a ch is in the wxChar array */
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

    /** This function is not used in the current code, it is replaced by MacroReplace() */
    inline const wxString& ThisOrReplacement(const wxString& str) const
    {
        wxStringHashMap::const_iterator it = s_Replacements.find(str);
        if (it != s_Replacements.end())
            return it->second;
        return str;
    };

    /** Check the previous char before EOL is a backslash */
    inline bool IsBackslashBeforeEOL()
    {
        wxChar last = PreviousChar();
        // if DOS line endings, we 've hit \r and we skip to \n...
        if (last == '\r')
        {
            if (m_TokenIndex - 2 >= 0)
                last = m_Buffer.GetChar(m_TokenIndex - 2);
            else
                last = _T('\0');
        }

        if(last == '\\')
            return true;
        else
            return false;
    }

    /** Do the Macro replacement according to the macro replacement rules */
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

    /** Tokenizer options specify the current skipping option */
    TokenizerOptions m_TokenizerOptions;
    TokensTree*      m_pTokensTree;

    /** File name */
    wxString         m_Filename;
    /** Buffer, all the lexial analysis is done on this buffer string */
    wxString         m_Buffer;
    /** Buffer length */
    unsigned int     m_BufferLen;

    /** These varialbes defined the current Token string attached information,
     * such as the token name, the line of the token, the current brace nest level
     */
    wxString         m_Token;
    unsigned int     m_TokenIndex;
    unsigned int     m_LineNumber;
    unsigned int     m_NestLevel; // keep track of block nesting { }
    unsigned int     m_SavedNestingLevel;

    /** Backuped the previous Token information */
    unsigned int     m_UndoTokenIndex;
    unsigned int     m_UndoLineNumber;
    unsigned int     m_UndoNestLevel;

    /** Peek token information */
    bool             m_PeekAvailable;
    wxString         m_PeekToken;
    unsigned int     m_PeekTokenIndex;
    unsigned int     m_PeekLineNumber;
    unsigned int     m_PeekNestLevel;

    /** bool variable specifies the buffer is ok */
    bool             m_IsOK;
    /** bool variable specifies whether the previous token is a "operator" */
    bool             m_IsOperator;
    /** Tokeniser state specifies the skipping option */
    TokenizerState   m_State;
    /** File loader pointer */
    LoaderBase*      m_pLoader;

    /** Calc Expression's result, true or false */
    std::stack<bool> m_ExpressionResult;

    /** Static member, this is a map to hold the replacement rules */
    static wxStringHashMap s_Replacements;
};

#endif // TOKENIZER_H


