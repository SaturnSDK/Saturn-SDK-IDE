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

///Calculate the hash value for a wxString
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
            return size_t((128 ^ len) + *((unsigned int*)((const wxChar*)x + len - intWxChar)));
        else if (len >= shortWxChar)
            return size_t((256 ^ len) + *((unsigned short*)((const wxChar*)x + len - shortWxChar)));
        else
            return size_t((512 ^ len) + *((const wxChar*)x + len - 1));
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

///wxString->wxString hash map used to store the macro replacement rules
WX_DECLARE_HASH_MAP(wxString, wxString, HashForWxStringMap, EqualForWxStringMap, wxStringHashMap);

/// Enum defines the skip state of the Tokenizer
enum TokenizerState
{
    tsSkipEqual         = 0x0001,         //!< Skip the assignment statement
    tsSkipQuestion      = 0x0002,         //!< Skip the conditional evaluation statement
    tsSkipSubScrip      = 0x0004,         //!< Skip the array-subscript notation statement

    tsSingleAngleBrace  = 0x0008,         //!< Reserve angle braces
    tsReadRawExpression = 0x0010,         //!< Reserve every chars

    tsSkipNone          = 0x1000,         //!< Skip None
    // convenient masks
    tsSkipUnWanted      = tsSkipEqual    | tsSkipQuestion | tsSkipSubScrip,
    tsTemplateArgument  = tsSkipUnWanted | tsSingleAngleBrace
};

/// Enum categorizing C-preprocessor directives
enum PreprocessorType
{
    ptIf                = 0x0001,   //!< #if
    ptIfdef             = 0x0002,   //!< #ifdef
    ptIfndef            = 0x0003,   //!< #ifndef
    ptElif              = 0x0004,   //!< #elif
    ptElifdef           = 0x0005,   //!< #elifdef
    ptElifndef          = 0x0006,   //!< #elifndef
    ptElse              = 0x0007,   //!< #else
    ptEndif             = 0x0008,   //!< #endif
    ptOthers            = 0x0009    //!< #include, #define ...
};

/// Whether we need to handle C-preprocessor directives
struct TokenizerOptions
{
    bool wantPreprocessor;
    bool storeDocumentation;
};

/** @brief This is just a simple lexer class
 *
 * A Tokenizer does the lexical analysis on a buffer. The buffer is either a wxString loaded from a local source/header file
 * or a wxString already in memory(e.g. the scintilla text buffer). The most public interfaces are two member functions:
 * GetToken() and PeekToken().
 * The former one eats one token string from buffer, the later one does a "look ahead" on the buffer and return
 * the next token string(peeked string). The peeked string content will be buffered until the next GetToken() call,
 * thus performance can be improved.
 * Also, Tokenizer class does some kind of handling "Macro replacement" on the buffer to imitate the macro expansion in
 * C-preprocessor, see member-function ReplaceMacro() for details.
 * Further more, it handles some "conditional preprocessor directives"(like "#if xxx").
 */
class Tokenizer
{
public:
    /** Tokenizer constructor.
     * @param filename the file to be opened.
     */
    Tokenizer(TokenTree* tokenTree, const wxString& filename = wxEmptyString);

    /** Tokenizer destructor.*/
    ~Tokenizer();

    /** Initialize the buffer by opening a file through a loader. */
    bool Init(const wxString& filename = wxEmptyString, LoaderBase* loader = 0);

    /** Initialize the buffer by directly using a wxString reference.
     * @param initLineNumber the start line of the buffer, usually the parser try to parse a function
     * body, so the line information of each Token can be correct.
     */
    bool InitFromBuffer(const wxString& buffer, const wxString& fileOfBuffer = wxEmptyString,
                        size_t initLineNumber = 0);

    /** Consume and return the current token string. */
    wxString GetToken();

    /** Do a "look ahead", and return the next token string. */
    wxString PeekToken();

    /** Undo the GetToken. */
    void     UngetToken();

    /** Handle condition preprocessor and store documentation or not */
    void SetTokenizerOption(bool wantPreprocessor, bool storeDocumentation)
    {
        m_TokenizerOptions.wantPreprocessor = wantPreprocessor;
        m_TokenizerOptions.storeDocumentation = storeDocumentation;
    };

    /** Set the Tokenizer skipping options. E.g. sometimes, we need to skip the statement after "=",
     * but sometimes, we should disable this options, see more details on TokenizerState.
     */
    void SetState(TokenizerState state)
    {
        m_State = state;
    };

    /** Return the skipping options value, see TokenizerState for more details.*/
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

    /** Return the brace "{" level.
     * the value will increase by one when we meet a "{", decrease by one when we meet a "}".
     */
    unsigned int GetNestingLevel() const
    {
        return m_NestLevel;
    };

    /** Save the brace "{" level, the parser might need to ignore the nesting level in some cases */
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
     * @param nestBraces true if you still need to count the '{' and '}' levels
     * @param stripUnneeded true if you are going to remove comments and compression spaces(two or
     * more spaces should become one space)
     */
    wxString ReadToEOL(bool nestBraces = true, bool stripUnneeded = true);

    /** Read and format between (), stored in 'str'
     * @param trimFirst, some macro definition are mixed with comments, like below
     */
     // #define AAA  /*args*/ (x) x
     // so, we just eat the embedded C style comments, and get "(x) x"
    void ReadParentheses(wxString& str, bool trimFirst);

    void ReadParentheses(wxString& str);

    /** Skip from the current position to the end of line.
     * @param nestBraces if true, we should still counting the brace levels in this function.
     */
    bool SkipToEOL(bool nestBraces = true); // use with care outside this class!

    /** Skip to then end of the C++ style comment */
    bool SkipToInlineCommentEnd();

    /** Add one "replacement rule", this is just a simple way of defining a macro definition.
     * The rule composites two strings: the key string and the value string.
     * When the Tokenizer gets an identifier kind string, it is lookuped in the
     * replacement rules map, if it matches one rule, the rule's value string will be returned instead..
     * Other rules are some function like macro definition,
     * E.g. to replace the "_GLIBCXX_BEGIN_NAMESPACE(std)" to  "namespace std {"
     * We can use: Tokenizer::SetReplacementString(_T("_GLIBCXX_BEGIN_NAMESPACE"), _T("+namespace"));
     * See more details in CodeCompletion::LoadTokenReplacements() function.
     * @param from the matching key string
     * @param to the matching value string
     */
    static void SetReplacementString(const wxString& from, const wxString& to)
    {
        s_Replacements[from] = to;
    };

    /** Remove a macro replacement rule */
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
            s_Replacements[it->first] = it->second;
    }

    /** Check whether the Tokenizer reached the end of the buffer (file) */
    bool IsEOF() const
    {
        return m_TokenIndex >= m_BufferLen;
    }

    /** return true if it is Not the end of buffer */
    bool NotEOF() const
    {
        return m_TokenIndex < m_BufferLen;
    }

    /** Backward buffer replacement for re-parsing
     *
     * @param target the new text going to take place on the m_Buffer
     * @param updatePeekToken do we need to update the m_PeekToken after the replacement?
     *
     * http://forums.codeblocks.org/index.php/topic,13384.msg90391.html#msg90391
     *
     * Macro expansion is just replace some characters in the m_Buffer.
     *
     * xxxxxxxxxAAAA(u,v)yyyyyyyyy
     *          ^---m_TokenIndex
     * For example, the above is a wxChar Array m_Buffer, then "AAAA(u,v)" need to do a Macro
     * expansion to some other text. So, we just do a "backward" text replace, so that, after
     * replacement, The last replacement char was ")" in "AAAA(u,v)" (We say it as an entry point),
     * so the text becomes:
     *
     * xxxNNNNNNNNNNNNNNNyyyyyyyyy
     *    ^---m_TokenIndex
     * Note that "NNNNNNNNNNNN" is some macro expansion text. then the m_TokenIndex was moved
     * backward to the beginning of the text.
     * if the macro expansion result text is small enough, then m_Buffer's length do not need to
     * change.
     * The situation when our m_Buffer's length need to be change is that the macro expansion text
     * is too long, so the buffer before "entry point" can not hold the new text, this way,
     * m_Buffer's length will adjusted. like below:
     * NNNNNNNNNNNNNNNNNNNNNNyyyyyyyyy
     * ^---m_TokenIndex
     */
    bool ReplaceBufferText(const wxString& target, bool updatePeekToken = true);

    /** Get actual context for macro, then replace buffer for re-parsing
     *  @param tk the macro definition, this is usually happens we want to expand a function like
     *  macro, since a variable like macro just did a simple text replacement.
     */
    bool ReplaceFunctionLikeMacro(const Token* tk, bool updatePeekToken = true);

    /** Get first token position in buffer */
    int GetFirstTokenPosition(const wxString& buffer, const wxString& target)
    {
        return GetFirstTokenPosition(buffer.GetData(), buffer.Len(), target.GetData(), target.Len());
    }

    /** find the sub-string key in the whole buffer, return the first position of the key
     *  @param buffer the content of the string
     *  @param bufferLen length of the string
     *  @param key the search key(sub-string)
     *  @param keyLen the search key length
     */
    int GetFirstTokenPosition(const wxChar* buffer, const size_t bufferLen,
                              const wxChar* key, const size_t keyLen);

    /** KMP find, get the first position, if find nothing, return -1 */
    int KMP_Find(const wxChar* text, const wxChar* pattern, const int patternLen);

    void SetLastTokenIdx(int tokenIdx);

protected:
    /** Initialize some member variables */
    void BaseInit();

    /** Do the actual lexical analysis, both GetToken() and PeekToken() will internally call this
     * function. It just move the m_TokenIndex one step forward, and return a lexeme before the
     * m_TokenIndex.
     */
    wxString DoGetToken();

    /** Read a file, and fill the m_Buffer */
    bool ReadFile();

    /** Check the current character is a C-Escape character in a string. */
    bool IsEscapedChar();

    /** Skip character until we meet a ch */
    bool SkipToChar(const wxChar& ch);

    /** Skip characters until we meet any characters in a wxChar Array.
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

    /** Skip any "tab" "white-space" */
    bool SkipWhiteSpace();

    /** Skip the C/C++ comment */
    bool SkipComment();

    /** Skip the string literal(enclosed in double quotes) or character literal(enclosed in single quotes).*/
    bool SkipString();

    /** Move to the end of string literal or character literal */
    bool SkipToStringEnd(const wxChar& ch);

    /** Move to the next character in the buffer, amount defines the steps (by default, it is one) */
    bool MoveToNextChar(const unsigned int amount = 1);

    /** Return the current character indexed by m_TokenIndex in the m_Buffer */
    wxChar CurrentChar() const
    {
        if(m_TokenIndex < m_BufferLen)
            return m_Buffer.GetChar(m_TokenIndex);
        return 0;
    };

    /** Do the previous two functions sequentially */
    wxChar CurrentCharMoveNext()
    {
        wxChar c = CurrentChar();
        m_TokenIndex++;
        return c;
    };

    /** Return (peek) the next character */
    wxChar NextChar() const
    {
        if ((m_TokenIndex + 1) >= m_BufferLen) // m_TokenIndex + 1) < 0  can never be true
            return 0;

        return m_Buffer.GetChar(m_TokenIndex + 1);
    };

    /** Return (peek) the previous character */
    wxChar PreviousChar() const
    {
        if (m_TokenIndex > 0 && m_BufferLen > 0) // m_TokenIndex > m_BufferLen can never be true
            return m_Buffer.GetChar(m_TokenIndex - 1);

        return 0;
    };

private:
    /** Check if a ch matches any characters in the wxChar array */
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

    /** Check the previous char before EOL is a backslash, call this function in the condition that
     * the CurrentChar is '\n', here we have two cases:
     * ......\\\r\n......
     *            ^--current char, this is DOS style EOL
     * ......\\\n......
     *          ^--current char, this is Linux style EOL
     */
    inline bool IsBackslashBeforeEOL()
    {
        wxChar last = PreviousChar();
        // if DOS line endings, we have hit \r and we skip to \n...
        if (last == _T('\r') && m_TokenIndex >= 2)
            return m_Buffer.GetChar(m_TokenIndex - 2) == _T('\\');
        return last == _T('\\');
    }

    /** Do the Macro replacement according to the macro replacement rules */
    void ReplaceMacro(wxString& str);

    /** Judge what is the first block
      * It will call 'SkipToEOL(false, true)' before returned.
      */
    bool CalcConditionExpression();

    /** If the macro is defined, return true
      * It will call 'SkipToEOL(false, true)' before returned.
      */
    bool IsMacroDefined();

    /** Skip to the next conditional preprocessor directive branch. */
    void SkipToNextConditionPreprocessor();

    /** Skip to the #endif conditional preprocessor directive. */
    void SkipToEndConditionPreprocessor();

    /** Get current conditional preprocessor type */
    PreprocessorType GetPreprocessorType();

    /** handle the preprocessor directive:
      * #ifdef XXX or #endif or #if or #elif or...
      * If conditional preprocessor handles correctly, return true, otherwise return false.
      */
    void HandleConditionPreprocessor(const PreprocessorType type);

    /** Split the actual macro arguments, and store them in results*/
    void SplitArguments(wxArrayString& results);

    /** Get the text after macro expansion
     * @param tk the macro definition token, usually a function like macro definition
     * @param expandedText is an output variable string
     */
    bool GetMacroExpendedText(const Token* tk, wxString& expandedText);

    /** Just for KMP find */
    void KMP_GetNextVal(const wxChar* pattern, int next[]);

    /** Tokenizer options specify the skipping option */
    TokenizerOptions     m_TokenizerOptions;
    TokenTree*           m_TokenTree;

    /** Filename of the buffer */
    wxString             m_Filename;
    /** File index, useful when parsing documentation; \sa SkipComment */
    int                  m_FileIdx;
    /** Buffer content, all the lexical analysis is done on this */
    wxString             m_Buffer;
    /** Buffer length */
    unsigned int         m_BufferLen;

    /** These variables define the current token string and its auxiliary information,
     * such as the token name, the line number of the token, the current brace nest level.
     */
    wxString             m_Token;                //!< token name
    /** when parsing a buffer
     * ....... namespace std { int a; .......
     *                      ^ --- m_TokenIndex, m_Token = "std"
     * m_TokenIndex is always point to the next character of a valid token, in the above example,
     * it is the space after "std".
     */
    unsigned int         m_TokenIndex;           //!< index offset in buffer
    unsigned int         m_LineNumber;           //!< line offset in buffer
    unsigned int         m_NestLevel;            //!< keep track of block nesting { }
    unsigned int         m_SavedNestingLevel;

    /** Backup the previous Token information */
    unsigned int         m_UndoTokenIndex;
    unsigned int         m_UndoLineNumber;
    unsigned int         m_UndoNestLevel;

    /** Peek token information */
    bool                 m_PeekAvailable;
    wxString             m_PeekToken;
    unsigned int         m_PeekTokenIndex;
    unsigned int         m_PeekLineNumber;
    unsigned int         m_PeekNestLevel;

    /** bool variable specifies whether the buffer is ready for parsing */
    bool                 m_IsOK;
    /** Tokeniser state specifies the skipping option */
    TokenizerState       m_State;
    /** File loader */
    LoaderBase*          m_Loader;

    /** Calculate Expression's result, stack for Shunting-yard algorithm */
    std::stack<bool>     m_ExpressionResult;

    /** Whether we are in replace buffer parsing, avoid recursive calling */
    bool                 m_IsReplaceParsing;

    /** Save the remaining length, after the first replace buffer, this is the length which replacement
     * does not touch. (from the beginning of the m_Buffer)
     */
    size_t               m_FirstRemainingLength;

    /** Save the repeat replace buffer count if currently in replace parsing */
    size_t               m_RepeatReplaceCount;

    /** Static member, this is a hash map storing all macro replacement rules */
    static wxStringHashMap s_Replacements;

    wxString             m_NextTokenDoc;
    int                  m_LastTokenIdx;
};

#endif // TOKENIZER_H
