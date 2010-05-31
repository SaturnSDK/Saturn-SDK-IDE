/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include "tokenizer.h"
#include <wx/utils.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include "manager.h"
#include <cctype>
#include <globals.h>
#include "logmanager.h"
#include "expression.h"

#define TOKENIZER_DEBUG_OUTPUT 0

#ifdef PARSER_TEST
    extern void ParserTrace(const wxChar* format, ...);
    #define TRACE(format, args...)\
    ParserTrace(format , ## args)
#else
#if TOKENIZER_DEBUG_OUTPUT
    #define TRACE(format, args...)\
    Manager::Get()->GetLogManager()->DebugLog(F( format , ## args))
#else
    #define TRACE(format, args...)
#endif
#endif

namespace TokenizerConsts
{
    const wxString colon        (_T(":"));
    const wxString colon_colon  (_T("::"));
    const wxString kw_if        (_T("if"));
    const wxString kw_ifdef     (_T("ifdef"));
    const wxString kw_ifndef    (_T("ifndef"));
    const wxString kw_elif      (_T("elif"));
    const wxString kw_elifdef   (_T("elifdef"));
    const wxString kw_elifndef  (_T("elifndef"));
    const wxString kw_else      (_T("else"));
    const wxString kw_el        (_T("el"));
    const wxString kw_endif     (_T("endif"));
    const wxString hash         (_T("#"));
    const wxString tabcrlf      (_T("\t\n\r"));
};

// static
wxStringHashMap Tokenizer::s_Replacements;

Tokenizer::Tokenizer(TokensTree* tokensTree, const wxString& filename)
    : m_pTokensTree(tokensTree),
    m_Filename(filename),
    m_BufferLen(0),
    m_Token(_T("")),
    m_TokenIndex(0),
    m_LineNumber(1),
    m_NestLevel(0),
    m_SavedNestingLevel(0),
    m_UndoTokenIndex(0),
    m_UndoLineNumber(1),
    m_UndoNestLevel(0),
    m_PeekAvailable(false),
    m_PeekToken(_T("")),
    m_PeekTokenIndex(0),
    m_PeekLineNumber(0),
    m_PeekNestLevel(0),
    m_IsOK(false),
    m_IsOperator(false),
    m_State(tsSkipUnWanted),
    m_pLoader(0)
{
    m_TokenizerOptions.wantPreprocessor = false;
    if (!m_Filename.IsEmpty())
        Init(m_Filename);
}

Tokenizer::~Tokenizer()
{
}

bool Tokenizer::Init(const wxString& filename, LoaderBase* loader)
{
    m_pLoader = loader;
    BaseInit();
    if (filename.IsEmpty())
    {
        if (m_Filename.IsEmpty())
        {
            TRACE(_T("Init() : Called without filename."));
            return false;
        }
    }
    else
    {
        m_Filename = filename;
        TRACE(_T("Init() : m_Filename='%s'"), m_Filename.wx_str());
    }

    if (!wxFileExists(m_Filename))
    {
        TRACE(_T("Init() : File '%s' does not exist."), m_Filename.wx_str());
        return false;
    }

    if (!ReadFile())
    {
        TRACE(_T("Init() : File '%s' could not be read."), m_Filename.wx_str());
        return false;
    }

    if (!m_BufferLen)
    {
        TRACE(_T("Init() : File '%s' is empty."), m_Filename.wx_str());
        return false;
    }

    m_IsOK = true;
    return true;
}

bool Tokenizer::InitFromBuffer(const wxString& buffer)
{
    BaseInit();
    m_BufferLen = buffer.Length();
    m_Buffer.Alloc(m_BufferLen + 1); // + 1 => sentinel
    m_Buffer = buffer;
    m_Buffer += _T(' ');
    m_IsOK = true;
    m_Filename.Clear();
    return true;
}

void Tokenizer::BaseInit()
{
    m_BufferLen         = 0;
    m_TokenIndex        = 0;
    m_LineNumber        = 1;
    m_NestLevel         = 0;
    m_SavedNestingLevel = 0;
    m_UndoTokenIndex    = 0;
    m_UndoLineNumber    = 1;
    m_UndoNestLevel     = 0;
    m_PeekTokenIndex    = 0;
    m_PeekLineNumber    = 0;
    m_PeekNestLevel     = 0;
    m_IsOK              = false;
    m_IsOperator        = false;
    m_Buffer.Clear();
}

bool Tokenizer::ReadFile()
{
    bool success = false;
    wxString fileName = wxEmptyString;
    if (m_pLoader)
    {
        fileName = m_pLoader->FileName();
        char* data  = m_pLoader->GetData();
        m_BufferLen = m_pLoader->GetLength();

        // the following code is faster than DetectEncodingAndConvert()
//        DetectEncodingAndConvert(data, m_Buffer);

        // same code as in cbC2U() but with the addition of the string length (3rd param in unicode version)
        // and the fallback encoding conversion
#if wxUSE_UNICODE
        m_Buffer = wxString(data, wxConvUTF8, m_BufferLen + 1); // + 1 => sentinel
        if (m_Buffer.Length() == 0)
        {
            // could not read as utf-8 encoding, try iso8859-1
            m_Buffer = wxString(data, wxConvISO8859_1, m_BufferLen + 1); // + 1 => sentinel
        }
#else
        m_Buffer = wxString(data, m_BufferLen + 1); // + 1 => sentinel
#endif

        success = (data != 0);
    }
    else
    {
        if (!wxFileExists(m_Filename))
            return false;

        // open file
        wxFile file(m_Filename);

        if (!cbRead(file, m_Buffer))
            return false;

        fileName = m_Filename;
        success = true;
    }

//    size_t replacements  = m_Buffer.Replace(_T("_GLIBCXX_BEGIN_NESTED_NAMESPACE(std, _GLIBCXX_STD_D)"), _T("namespace std {"),       true);
//           replacements += m_Buffer.Replace(_T("_GLIBCXX_BEGIN_NESTED_NAMESPACE(std, _GLIBCXX_STD_P)"), _T("namespace std {"),       true);
//           replacements += m_Buffer.Replace(_T("_GLIBCXX_END_NESTED_NAMESPACE"),                        _T("}"),                     true);
//           replacements += m_Buffer.Replace(_T("_GLIBCXX_BEGIN_NAMESPACE_TR1"),                         _T("namespace tr1 {"),       true);
//           // The following must be before replacing "_GLIBCXX_END_NAMESPACE"!!!
//           replacements += m_Buffer.Replace(_T("_GLIBCXX_END_NAMESPACE_TR1"),                           _T("}"),                     true);
//           replacements += m_Buffer.Replace(_T("_GLIBCXX_BEGIN_NAMESPACE(__gnu_cxx)"),                  _T("namespace __gnu_cxx {"), true);
//           replacements += m_Buffer.Replace(_T("_GLIBCXX_BEGIN_NAMESPACE(std)"),                        _T("namespace std {"),       true);
//           replacements += m_Buffer.Replace(_T("_GLIBCXX_END_NAMESPACE"),                               _T("}"),                     true);
//
//    if (replacements) TRACE(_T("Did %d replacements in buffer of '%s'."), replacements, fileName.wx_str());

    m_BufferLen = m_Buffer.Length();

    // add 'sentinel' to the end of the string (not counted to the length of the string)
    m_Buffer += _T(' ');

    return success;
}

// Behavior consistent with SkipComment
bool Tokenizer::SkipWhiteSpace()
{
    if (CurrentChar() > _T(' ') || IsEOF())
        return false;

    // skip spaces, tabs, etc.
    // don't check EOF when MoveToNextChar already does, also replace isspace() which calls msvcrt.dll
    // with a dirty hack: CurrentChar() <= ' ' is "good enough" here
    while (CurrentChar() <= _T(' ') && MoveToNextChar())
        ;

    return true;
}

// only be called when we are in a C-string,
// To check whether the current charactor is the real end of C-string
// See SkipToStringEnd() for more details
bool Tokenizer::IsEscapedChar()
{
    // Easy: If previous char is not a backslash, too than it's surely escape'd
    if (PreviousChar() != '\\')
        return true;
    else
    {
        // check for multiple backslashes, e.g. "\\"
        unsigned int numBackslash = 2; // for sure we have at least two at this point
        while (   ((m_TokenIndex - numBackslash) >= 0)
               && ((m_TokenIndex - numBackslash) <= m_BufferLen)
               && (m_Buffer.GetChar(m_TokenIndex - numBackslash) == '\\') )
            ++numBackslash; // another one...

        if ( (numBackslash%2) == 1) // number of backslashes (including current char) is odd
            return true;            // eg: "\""
        else                        // number of backslashes (including current char) is even
            return false;           // eg: "\\""
    }
    return false;
}

// expect we are not in a C-string
bool Tokenizer::SkipToChar(const wxChar& ch)
{
    // skip everything until we find ch

    while (CurrentChar() != ch && MoveToNextChar())  // don't check EOF when MoveToNextChar already does
        ;

    if (IsEOF())
        return false;

    return true;
}

//  For example: X"ABCDEFG\"HIJKLMN"Y
//  We are now at A, and would skip to Y
//  The double quote before H is a "C-escaped-character", We shouldn't quite from that
bool Tokenizer::SkipToStringEnd(const wxChar& ch)
{
    while (true)
    {
        while (CurrentChar() != ch && MoveToNextChar()) // don't check EOF when MoveToNextChar already does
            ;

        if (IsEOF())
            return false;

        if (IsEscapedChar()) break;

        MoveToNextChar();
    }
    return true;
}

// return true if we really skip a string, that means m_TokenIndex has changed.
bool Tokenizer::SkipString()
{
    if (IsEOF())
        return false;

    const wxChar ch = CurrentChar();
    if (ch == _T('"') || ch == _T('\''))
    {
        // this is the case that match is inside a string!
        MoveToNextChar();
        SkipToStringEnd(ch);
        MoveToNextChar();
        return true;
    }

    return false;
}

// expect we are not in a C-string.
bool Tokenizer::SkipToOneOfChars(const wxChar* chars, bool supportNesting, bool skipPreprocessor, bool skipAngleBrace)
{
    while (NotEOF() && !CharInString(CurrentChar(), chars))
    {
        MoveToNextChar();

        while (SkipString() || SkipComment())
            ;

        // use 'while' here to cater for consecutive blocks to skip (e.g. sometemplate<foo>(bar)
        // must skip <foo> and immediately after (bar))
        // because if we don't, the next block won't be skipped ((bar) in the example) leading to weird
        // parsing results
        bool done = false;
        while (supportNesting && !done)
        {
            switch (CurrentChar())
            {
                case '#':
					if (skipPreprocessor)
						SkipToEOL(true, true);
					else
						done = true;
					break;
                case '{': SkipBlock('{'); break;
                case '(': SkipBlock('('); break;
                case '[': SkipBlock('['); break;
                case '<': // don't skip if << operator
                    if (skipAngleBrace)
                    {
                        if (NextChar() == '<')
                            MoveToNextChar(2); // skip it and also the next '<' or the next '<' leads to a SkipBlock('<');
                        else
                            SkipBlock('<');
                        break;
                    }

                default: done = true; break;
            }
        }

    }

    if (IsEOF())
        return false;

    return true;
}

wxString Tokenizer::ReadToEOL(bool nestBraces, bool stripComment)
{
    if (stripComment)
    {
        wxString str;
        while (true)
        {
            while (NotEOF() && CurrentChar() != _T('\n'))
            {
                while (SkipComment())
                    ;

                const wxChar ch = CurrentChar();
                if (ch == _T('\n'))
                    break;

                str.Append(ch);
                if (nestBraces)
                {
                    if (ch == _T('{'))
                        ++m_NestLevel;
                    else if (ch == _T('}'))
                        --m_NestLevel;
                }
                MoveToNextChar();
            }

            wxChar last = PreviousChar();
            // if DOS line endings, we 've hit \r and we skip to \n...
            if (last == _T('\r'))
            {
                if (m_TokenIndex - 2 >= 0)
                    last = m_Buffer.GetChar(m_TokenIndex - 2);
                else
                    last = _T('\0');
            }
            if (IsEOF() || last != _T('\\'))
                break;
            else
            {
                str.Trim();
                str.RemoveLast();
                MoveToNextChar();
            }
        }
        return str;
    }
    else
    {
        unsigned int idx = m_TokenIndex;
        SkipToEOL(nestBraces);
        return m_Buffer.Mid(idx, m_TokenIndex - idx);
    }
}


wxString Tokenizer::ReadBlock(const wxChar& leftBrace)
{
    wxString str;
    int nestLevel = 0;

    wxChar rightBrace;
    if (leftBrace == _T('('))
        rightBrace = _T(')');
    else if (leftBrace == _T('{'))
        rightBrace = _T('}');
    else if (leftBrace == _T('['))
        rightBrace = _T(']');
    else if (leftBrace == _T('<'))
        rightBrace = _T('>');
    else
        return wxEmptyString;

    while (NotEOF())
    {
        while (SkipComment())
            ;

        wxChar ch = CurrentChar();
        if (ch == leftBrace)
        {
            ++nestLevel;
            str.Trim(true);
        }
        else if (ch == rightBrace)
        {
            --nestLevel;
            str.Trim(true);
        }

        unsigned int index = m_TokenIndex;
        while (ch == _T('#'))
        {
            if (!HandleConditionPreprocessor())
                break;
            ch = CurrentChar();
        }

        if (ch == _T('\'') || ch == _T('"'))
        {
            unsigned int start = m_TokenIndex;
            MoveToNextChar();
            SkipToStringEnd(ch);
            MoveToNextChar();
            str.Append(m_Buffer.Mid(start, m_TokenIndex - start));
        }
        else if (ch == _T(','))
        {
            str.Trim(true);
            str.Append(_T(", "));
        }
        else if (ch == _T('='))
        {
            str.Trim(true);
            const wxChar last = str.Last();
            if (!(last == _T('=') || last == _T('!') || last == _T('>') || last == _T('<')))
                str.Append(_T(' '));
            str.Append(_T("= "));
        }
        else if ((ch > _T(' ')) || (ch == _T(' ') && str.Last() != _T(' ')))
            str.Append(ch);

        if (index == m_TokenIndex)
            MoveToNextChar();

        if (nestLevel == 0)
            break;
    }

    return str;
}

bool Tokenizer::SkipToEOL(bool nestBraces, bool skippingComment)
{
    // skip everything until we find EOL
    while (1)
    {
        while (NotEOF() && CurrentChar() != '\n')
        {
            if (!skippingComment)
            {
                if (CurrentChar() == '/' && NextChar() == '*')
                {
                    SkipComment();
                    if (CurrentChar() == _T('\n'))
                        break;
                }

                if (nestBraces && CurrentChar() == _T('{'))
                    ++m_NestLevel;
                else if (nestBraces && CurrentChar() == _T('}'))
                    --m_NestLevel;
            }
            MoveToNextChar();
        }
        wxChar last = PreviousChar();
        // if DOS line endings, we 've hit \r and we skip to \n...
        if (last == '\r')
        {
            if (m_TokenIndex - 2 >= 0)
                last = m_Buffer.GetChar(m_TokenIndex - 2);
            else
                last = _T('\0');
        }
        if (IsEOF() || last != '\\')
            break;
        else
            MoveToNextChar();
    }
    if (IsEOF())
        return false;
    return true;
}

bool Tokenizer::SkipBlock(const wxChar& ch)
{
    // skip blocks () [] {} <>
    wxChar match;
    switch (ch)
    {
        case '(': match = ')'; break;
        case '[': match = ']'; break;
        case '{': match = '}'; break;
        case '<': match = '>'; break;
        default : return false;
    }

    MoveToNextChar();
    int nestLevel = 1; // counter for nested blocks (xxx())
    while (NotEOF())
    {

        while (SkipWhiteSpace() || SkipString() || SkipComment())
            ;

        if (CurrentChar() == ch)
            ++nestLevel;
        else if (CurrentChar() == match)
            --nestLevel;

        MoveToNextChar();

        if (nestLevel == 0)
            break;
    }

    if (IsEOF())
        return false;
    return true;
}

// if we really move forward, return true, which means we have the new m_TokenIndex
// if we stay here, return false
bool Tokenizer::SkipComment()
{
    if (IsEOF())
        return false;

    bool cstyle;            // C or C++ style comments

    //check the comment prompt
    if (CurrentChar() == '/')
    {
        if      (NextChar() == '*')
            cstyle = true;
        else if (NextChar() == '/')
            cstyle = false;
        else
            return false; // Not a comment, return false;
    }
    else
        return false;     // Not a comment, return false;

    MoveToNextChar(2);    // Skip the comment prompt

    // Here, we are in the comment body
    while (true)
    {
        if (cstyle)      // C style comment
        {
            SkipToChar('/');
            if (PreviousChar() == '*') // end of a C style comment
            {
                MoveToNextChar();
                break;
            }
            if (!MoveToNextChar())
                break;
        }
        else             // C++ style comment
        {
            SkipToEOL(false, true); // nestBrace = false, skipComment = true
            MoveToNextChar();
            break;
        }
    }

    return true;
}

bool Tokenizer::SkipUnwanted()
{
    while (SkipWhiteSpace() || SkipComment())
        ;

    unsigned int startIndex = m_TokenIndex;
    wxChar c = CurrentChar();

    while (c == _T('#'))
    {
        if (!HandleConditionPreprocessor())
            break;
        startIndex = m_TokenIndex;
        c = CurrentChar();
    }

    // skip [XXX][YYY]
    if (m_State&tsSkipSubScrip)
    {
        while (c == _T('[') )
        {
            SkipBlock('[');
            SkipWhiteSpace();
            if (IsEOF())
                return false;
            c = CurrentChar();
        }
    }

    // skip the following = or ?
    if (m_State&tsSkipEqual)
    {
        if (c == _T('='))
        {
            if (!SkipToOneOfChars(_T(",;}"), true, true, false))
                return false;
        }
    }
    else if (m_State&tsSkipQuestion)
    {
        if (c == _T('?'))
        {
            if (!SkipToOneOfChars(_T(";}"), false, true))
                return false;
        }
    }

    // skip the following white space and comments
    while (SkipWhiteSpace() || SkipComment())
        ;

    if (startIndex != m_TokenIndex)
    {
        c = CurrentChar();
        while (c == _T('#'))
        {
            if (!HandleConditionPreprocessor())
                break;
            c = CurrentChar();
        }
    }

    return NotEOF();
}

wxString Tokenizer::GetToken()
{
    m_UndoTokenIndex = m_TokenIndex;
    m_UndoLineNumber = m_LineNumber;
    m_UndoNestLevel  = m_NestLevel;

    if (m_PeekAvailable)
    {
        m_TokenIndex = m_PeekTokenIndex;
        m_LineNumber = m_PeekLineNumber;
        m_NestLevel  = m_PeekNestLevel;
        m_Token      = m_PeekToken;
    }
    else
    {
        if (!SkipUnwanted())
            m_Token = wxEmptyString;
        else
            m_Token = DoGetToken();
    }

    m_PeekAvailable = false;

    return m_Token;
}

wxString Tokenizer::PeekToken()
{
    if (!m_PeekAvailable)
    {
        m_PeekAvailable = true;

        unsigned int savedTokenIndex = m_TokenIndex;
        unsigned int savedLineNumber = m_LineNumber;
        unsigned int savedNestLevel  = m_NestLevel;

        if (!SkipUnwanted())
            m_PeekToken = wxEmptyString;
        else
            m_PeekToken = DoGetToken();

        m_PeekTokenIndex             = m_TokenIndex;
        m_PeekLineNumber             = m_LineNumber;
        m_PeekNestLevel              = m_NestLevel;

        m_TokenIndex                 = savedTokenIndex;
        m_LineNumber                 = savedLineNumber;
        m_NestLevel                  = savedNestLevel;

    }
    return m_PeekToken;
}

void Tokenizer::UngetToken()
{
    m_PeekTokenIndex = m_TokenIndex;
    m_PeekLineNumber = m_LineNumber;
    m_PeekNestLevel  = m_NestLevel;
    m_TokenIndex     = m_UndoTokenIndex;
    m_LineNumber     = m_UndoLineNumber;
    m_NestLevel      = m_UndoNestLevel;
    m_PeekToken      = m_Token;
    m_PeekAvailable  = true;
}

wxString Tokenizer::DoGetToken()
{
    int start = m_TokenIndex;
    bool needReplace = false;

    wxString str;
    wxChar   c = CurrentChar();

    if (c == '_' || wxIsalpha(c))
    {
        // keywords, identifiers, etc.

        // operator== is cheaper than wxIsalnum, also MoveToNextChar already includes IsEOF
        while (    ( (c == '_') || (wxIsalnum(c)) )
               &&  MoveToNextChar() )
            c = CurrentChar(); // repeat

        if (IsEOF())
            return wxEmptyString;

        needReplace = true;
        str = m_Buffer.Mid(start, m_TokenIndex - start);
    }
#ifdef __WXMSW__ // This is a Windows only bug!
// fetch non-English characters, see more details in: http://forums.codeblocks.org/index.php/topic,11387.0.html
    else if (c == 178 || c == 179 || c == 185)
    {
        str = c;
        MoveToNextChar();
    }
#endif
    else if (wxIsdigit(c))
    {
        // numbers
        while (NotEOF() && CharInString(CurrentChar(), _T("0123456789.abcdefABCDEFXxLl")))
            MoveToNextChar();

        if (IsEOF())
            return wxEmptyString;

        str = m_Buffer.Mid(start, m_TokenIndex - start);
        m_IsOperator = false;
    }
    else if ( (c == '"') || (c == '\'') )
    {
        SkipString();
        //Now, we are after the end of the C-string, so return the whole string as a token.
        str = m_Buffer.Mid(start, m_TokenIndex - start);
    }
    else if (c == ':')
    {
        if (NextChar() == ':')
        {
            MoveToNextChar();
            MoveToNextChar();
            // this only copies a pointer, but operator= allocates memory and does a memcpy!
            str.assign(TokenizerConsts::colon_colon);
        }
        else
        {
            MoveToNextChar();
            str.assign(TokenizerConsts::colon);
        }
    }
    else if (c == '<')
    {
        if (m_State&tsSingleAngleBrace)
        {
            if ( !SkipToOneOfChars(  _T(">"), true, true)   )
                return wxEmptyString;
            MoveToNextChar();
            str= m_Buffer.Mid(start, m_TokenIndex - start);
        }
        else
        {
            str = c;
            MoveToNextChar();
        }
    }
    else if (c == '(')
    {
        m_IsOperator = false;

        if (m_State & tsReadRawExpression)
        {
            str = c;
            MoveToNextChar();
        }
        else
        {
            str = ReadBlock(c);
        }
    }
    else
    {
        if      (c == '{')
            ++m_NestLevel;
        else if (c == '}')
            --m_NestLevel;

        str = c;
        MoveToNextChar();
    }

    if (needReplace)
        return MacroReplace(str);

    return str;
}

wxString Tokenizer::MacroReplace(const wxString str)
{
    wxStringHashMap::const_iterator it = s_Replacements.find(str);

    if (it != s_Replacements.end())
    {
        // match one!
        wxString key   = it->first;
        wxString value = it->second;
        TRACE(_T("MacroReplace() : Replacing '%s' with '%s' (file='%s')."), key.wx_str(), value.wx_str(), m_Filename.wx_str());
        if (value[0]=='+' && CurrentChar()=='(')
        {
            unsigned int start = m_TokenIndex;
            m_Buffer[start] = ' ';
            bool fillSpace = false;
            while (m_Buffer[start]!=')')
            {
                if (m_Buffer[start]==',')
                    fillSpace = true;

                if (fillSpace==true)
                    m_Buffer[start]=' ';

                start++;
            }
            m_Buffer[start] = '{';
            return value.Remove(0,1);
        }
        else if (value[0] == '-')
        {
            unsigned int lenKey = key.Len();
            value = value.Remove(0,1);
            unsigned int lenValue = value.Len();

            for (unsigned int i=1; i<=lenKey; i++)
            {
                if (i < lenValue)
                    m_Buffer[m_TokenIndex-i] = value[lenValue-i];
                else
                    m_Buffer[m_TokenIndex-i] = ' ';
            }

            int firstSpace = value.First(' ');
            // adjust m_TokenIndex
            m_TokenIndex = m_TokenIndex - lenValue + firstSpace;

            return value.Mid(0,firstSpace);
        }
        else
            return value;
    }

    return str;
}

void Tokenizer::GetPreprocessorValue(const wxString& token, bool& found, long& value)
{
    Token* tk = NULL;
    if (m_pTokensTree != NULL)
        tk = m_pTokensTree->at(m_pTokensTree->TokenExists(token, -1, tkPreprocessor));

    if (tk != NULL)
    {
        found = true;
        if (tk->m_Type.IsEmpty())
            return;

        if (wxIsdigit(tk->m_Type[0]))
            tk->m_Type.ToLong(&value, tk->m_Type.StartsWith(_T("0x")) ? 16 : 10);
        else if (tk->m_Type != token)
            GetPreprocessorValue(tk->m_Type, found, value);
    }
}

bool Tokenizer::CalcConditionExpression()
{
    // need to force the tokenizer skip raw expression
    TokenizerState oldState = m_State;
    m_State = tsReadRawExpression;

    const unsigned int undoIndex = m_TokenIndex;
    const unsigned int undoLine = m_LineNumber;
    SkipToEOL(false, true);
    const unsigned int expEndIndex = m_TokenIndex;
    m_TokenIndex = undoIndex;
    m_LineNumber = undoLine;

    Expression exp;
    while (m_TokenIndex < expEndIndex)
    {
        while (SkipComment())
            ;
        wxString token = DoGetToken();
        if (token.IsEmpty() || token == _T("\\") || token == _T("defined") || token[0] <= _T(' '))
            continue;

        if (token.Len() > 1 && !wxIsdigit(token[0]))
        {
            bool found = false; // Must be initialized to false
            long value = -1; // Must be initialized to -1
            GetPreprocessorValue(token, found, value);
            if (found)
                token.Printf(_T("%d"), (value == -1) ? 1 : value);
            else
                token = _T("0");
        }

        exp.AddToInfixExpression(token);
    }

    // reset tokenizer's functionality
    m_State = oldState;

    exp.ConvertInfixToPostfix();
    if (exp.CalcPostfix())
    {
        bool result = exp.GetStatus() && exp.GetResult();
        TRACE(_T("CalcConditionExpression() : exp.GetStatus() : %d, exp.GetResult() : %d"), exp.GetStatus(), exp.GetResult());
        return result;
    }

    return true;
}

bool Tokenizer::IsMacroDefined()
{
    while (SkipWhiteSpace() || SkipComment())
        ;
    const wxString token = DoGetToken();
    bool found = false; // Must be initialized to false
    long value = -1; // Must be initialized to -1
    GetPreprocessorValue(token, found, value);
    SkipToEOL(false, true);
    return found;
}


void Tokenizer::SkipToNextConditionPreprocessor()
{
    do
    {
        wxChar ch = CurrentChar();
        if (ch == _T('\'') || ch == _T('"') || ch == _T('/') || ch <= _T(' '))
        {
            while (SkipWhiteSpace() || SkipString() || SkipComment())
                ;
            ch = CurrentChar();
        }

        if (ch == _T('#'))
        {
            const unsigned int undoIndex = m_TokenIndex;
            const unsigned int undoLine = m_LineNumber;

            MoveToNextChar();
            while (SkipWhiteSpace() || SkipComment())
                ;

            const wxChar current = CurrentChar();
            const wxChar next = NextChar();

            // #if
            if (current == _T('i') && next == _T('f'))
                SkipToEndConditionPreprocessor();

            // #else #elif #elifdef #elifndef #endif
            else if (current == _T('e') && (next == _T('l') || next == _T('n')))
            {
                m_TokenIndex = undoIndex;
                m_LineNumber = undoLine;
                break;
            }
        }
    }
    while (MoveToNextChar());
}

void Tokenizer::SkipToEndConditionPreprocessor()
{
    do
    {
        wxChar ch = CurrentChar();
        if (ch == _T('\'') || ch == _T('"') || ch == _T('/') || ch <= _T(' '))
        {
            while (SkipWhiteSpace() || SkipString() || SkipComment())
                ;
            ch = CurrentChar();
        }

        if (ch == _T('#'))
        {
            MoveToNextChar();
            while (SkipWhiteSpace() || SkipComment())
                ;

            const wxChar current = CurrentChar();
            const wxChar next = NextChar();

            // #if
            if (current == _T('i') && next == _T('f'))
                SkipToEndConditionPreprocessor();

            // #endif
            else if (current == _T('e') && next == _T('n'))
            {
                SkipToEOL(false);
                break;
            }
        }
    }
    while (MoveToNextChar());
}

bool Tokenizer::HandleConditionPreprocessor()
{
#ifdef PARSER_TEST
    if (PreviousChar() != _T('#'))
        TRACE(_T("HandleConditionPreprocessor() : Error handling!"));
#endif

    const unsigned int undoIndex = m_TokenIndex;
    const unsigned int undoLine = m_LineNumber;

    MoveToNextChar();
    while (SkipWhiteSpace() || SkipComment())
        ;

    // ONLY FOR TEST!
//    if (m_Filename.Contains(_T("exception"))) // Line: 38
//    {
//        m_Filename.Len();
//    }
//    else if (m_Filename.Contains(_T("yvals.h"))) // Line: 79
//    {
//        m_Filename.Len();
//    }

    const wxString token = DoGetToken();

    // #if #ifdef #ifndef
    if (token.StartsWith(TokenizerConsts::kw_if))
    {
        bool result = false;
        if (token == TokenizerConsts::kw_if)
        {
            result = CalcConditionExpression();
            m_ExpressionResult.push(result);
        }
        else
        {
            const bool found = IsMacroDefined();
            result = (token == TokenizerConsts::kw_ifdef) ? found : !found;
            m_ExpressionResult.push(result);
        }

        if (!result)
           SkipToNextConditionPreprocessor();
    }

    // #elif #elifdef #elifndef
    else if (token.StartsWith(TokenizerConsts::kw_elif))
    {
        bool result = false;
        if (!m_ExpressionResult.empty() && !m_ExpressionResult.top())
        {
            if (token == TokenizerConsts::kw_elif)
                result = CalcConditionExpression();
            else
            {
                const bool found = IsMacroDefined();
                result = (token == TokenizerConsts::kw_elifdef) ? found : !found;
            }
        }

        if (result)
            m_ExpressionResult.top() = true;
        else
            SkipToNextConditionPreprocessor();
    }

    // #else
    else if (token==TokenizerConsts::kw_else)
    {
        if (!m_ExpressionResult.empty() && !m_ExpressionResult.top())
            SkipToEOL(false, true);
        else
            SkipToEndConditionPreprocessor();
    }

    // #endif
    else if (token==TokenizerConsts::kw_endif)
    {
        SkipToEOL(false, true);
        if (!m_ExpressionResult.empty())
            m_ExpressionResult.pop();
    }

    // #include, #define ...
    else
    {
        m_TokenIndex = undoIndex;
        m_LineNumber = undoLine;
        TRACE(_T("HandleConditionPreprocessor() : Skip Non-conditional expression blocks"));
        return false;
    }

    return true;
}
