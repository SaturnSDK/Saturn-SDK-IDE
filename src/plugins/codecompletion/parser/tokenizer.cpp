/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>

#include <cctype>
#include <wx/utils.h>
#include <wx/file.h>
#include <wx/msgdlg.h>

#include "expression.h"
#include "globals.h"
#include "logmanager.h"
#include "manager.h"
#include "tokenizer.h"

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

    return NotEOF();
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
						SkipToEOL(true);
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

    return NotEOF();
}

wxString Tokenizer::ReadToEOL(bool nestBraces, bool stripUnneeded)
{
    if (stripUnneeded)
    {
        TRACE(_T("%s : line=%d, CurrentChar='%c', PreviousChar='%c', NextChar='%c', nestBrace(%d)"),
              wxString(__PRETTY_FUNCTION__, wxConvUTF8).wc_str(), m_LineNumber, CurrentChar(),
              PreviousChar(), NextChar(), nestBraces ? 1 : 0);

        static const size_t maxBufferLen = 1024;
        wxChar buffer[maxBufferLen + 2];
        wxChar* p = buffer;
        wxString str;

        for (;;)
        {
            while (NotEOF() && CurrentChar() != _T('\n'))
            {
                while (SkipComment())
                    ;

                const wxChar ch = CurrentChar();
                if (ch == _T('\n'))
                    break;

                if (ch == _T(' ') && (p == buffer || *(p - 1) == ch))
                {
                    MoveToNextChar();
                    continue;
                }

                *p = ch;
                ++p;

                if (p >= buffer + maxBufferLen)
                {
                    str.Append(buffer, p - buffer);
                    p = buffer;
                }

                if (nestBraces)
                {
                    if (ch == _T('{'))
                        ++m_NestLevel;
                    else if (ch == _T('}'))
                        --m_NestLevel;
                }

                MoveToNextChar();
            }

            if (IsEOF() || !IsBackslashBeforeEOL())
                break;
            else
            {
                while (*(--p) <= _T(' ') && p > buffer)
                    ;
                MoveToNextChar();
            }
        }

        while (*(p - 1) <= _T(' ') && --p > buffer)
            ;
        str.Append(buffer, p - buffer);

        TRACE(_T("ReadToEOL(): (END) We are now at line %d, CurrentChar='%c', PreviousChar='%c', NextChar='%c'"),
              m_LineNumber, CurrentChar(), PreviousChar(), NextChar());
        TRACE(F(_T("ReadToEOL(): %s"), str.wx_str()));

        return str;
    }
    else
    {
        const unsigned int idx = m_TokenIndex;
        SkipToEOL(nestBraces);
        return m_Buffer.Mid(idx, m_TokenIndex - idx);
    }
}

void Tokenizer::ReadToEOL(wxArrayString& tokens)
{
    // need to force the tokenizer skip raw expression
    const TokenizerState oldState = m_State;
    m_State = tsReadRawExpression;

    const unsigned int undoIndex = m_TokenIndex;
    const unsigned int undoLine = m_LineNumber;
    SkipToEOL(false);
    const unsigned int endIndex = m_TokenIndex;
    m_TokenIndex = undoIndex;
    m_LineNumber = undoLine;

    int level = 0;
    wxArrayString tmp;

    while (m_TokenIndex < endIndex)
    {
        while (SkipComment())
            ;
        wxString token = DoGetToken();
        if (token[0] < _T(' ') || token == _T("\\"))
            continue;

        if (token[0] == _T('('))
            ++level;

        if (level == 0)
        {
            if (tmp.IsEmpty())
            {
                if (!token.Trim().IsEmpty())
                    tokens.Add(token);
            }
            else
            {
                wxString blockStr;
                for (size_t i = 0; i < tmp.GetCount(); ++i)
                    blockStr += tmp[i];
                tokens.Add(blockStr.Trim());
                tmp.Clear();
            }
        }
        else
            tmp.Add(token);

        if (token[0] == _T(')'))
            --level;
    }

    if (!tmp.IsEmpty())
    {
        if (level == 0)
        {
            wxString blockStr;
            for (size_t i = 0; i < tmp.GetCount(); ++i)
                blockStr += tmp[i];
            tokens.Add(blockStr.Trim());
        }
        else
        {
            for (size_t i = 0; i < tmp.GetCount(); ++i)
            {
                if (!tmp[i].Trim().IsEmpty())
                    tokens.Add(tmp[i]);
            }
        }
    }

    m_State = oldState;
}

void Tokenizer::ReadParentheses(wxString& str)
{
    static const size_t maxBufferLen = 1024;
    wxChar buffer[maxBufferLen + 2];
    wxChar* p = buffer;
    int level = 0;

    while (NotEOF())
    {
        while (SkipComment())
            ;
        wxChar ch = CurrentChar();

        while (ch == _T('#')) // do not use if
        {
            const PreprocessorType type = GetPreprocessorType();
            if (type == ptOthers)
                break;
            HandleConditionPreprocessor(type);
            ch = CurrentChar();
        }

        const unsigned int startIndex = m_TokenIndex;

        switch(ch)
        {
        case _T('('):
            {
                ++level;
                *p = ch;
                ++p;
            }
            break;

        case _T(')'):
            {
                --level;
                *p = ch;
                ++p;
            }
            break;

        case _T('\''):
        case _T('"'):
            {
                MoveToNextChar();
                SkipToStringEnd(ch);
                MoveToNextChar();
                const size_t writeLen = m_TokenIndex - startIndex;
                const size_t usedLen = p - buffer;
                if (usedLen + writeLen > maxBufferLen)
                {
                    if (p != buffer)
                    {
                        str.Append(buffer, usedLen);
                        p = buffer;
                    }

                    str.Append(&m_Buffer[startIndex], writeLen);
                }
                else
                {
                    memcpy((void*)p, (void*)(&m_Buffer[startIndex]), writeLen * sizeof(wxChar));
                    p += writeLen;
                }

                continue;
            }
            break;

        case _T(','):
            {
                if (*(p - 1) == _T(' '))
                    --p;

                *p = _T(',');
                *++p = _T(' ');
                ++p;
            }
            break;

        case _T('*'):
            {
                if (*(p - 1) == _T(' '))
                    --p;

                *p = _T('*');
                *++p = _T(' ');
                ++p;
            }
            break;

        case _T('='):
            {
                if (*(p - 1) == _T(' '))
                {
                    *p = _T('=');
                    *++p = _T(' ');
                    ++p;
                }
                else
                {
                    switch (*(p - 1))
                    {
                    case _T('='):
                    case _T('!'):
                    case _T('>'):
                    case _T('<'):
                        {
                            *p = _T('=');
                            *++p = _T(' ');
                            ++p;
                        }

                    default:
                        {
                            *p = _T(' ');
                            *++p = _T('=');
                            *++p = _T(' ');
                            ++p;
                        }
                    }
                }
            }
            break;

        case _T(' '):
            {
                if (*(p - 1) != _T(' '))
                {
                    *p = _T(' ');
                    ++p;
                }
            }
            break;

        case _T('\r'):
        case _T('\n'):
        case _T('\t'):
            break;

        default:
            {
                *p = ch;
                ++p;
            }
            break;
        }

        if (p >= buffer + maxBufferLen)
        {
            str.Append(buffer, p - buffer);
            p = buffer;
        }

        MoveToNextChar();

        if (level == 0)
            break;
    }

    if (p != buffer)
        str.Append(buffer, p - buffer);
    TRACE(_T("ReadParentheses(): %s, line=%d"), str.wx_str(), m_LineNumber);
}

bool Tokenizer::SkipToEOL(bool nestBraces)
{
    TRACE(_T("%s : line=%d, CurrentChar='%c', PreviousChar='%c', NextChar='%c', nestBrace(%d)"),
          wxString(__PRETTY_FUNCTION__, wxConvUTF8).wc_str(), m_LineNumber, CurrentChar(),
          PreviousChar(), NextChar(), nestBraces ? 1 : 0);

    // skip everything until we find EOL
    for (;;)
    {
        while (NotEOF() && CurrentChar() != '\n')
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

            MoveToNextChar();
        }

        if (IsEOF() || !IsBackslashBeforeEOL())
            break;
        else
            MoveToNextChar();
    }

    TRACE(_T("SkipToEOL(): (END) We are now at line %d, CurrentChar='%c', PreviousChar='%c', NextChar='%c'"),
          m_LineNumber, CurrentChar(), PreviousChar(), NextChar());

    return NotEOF();
}

bool Tokenizer::SkipToInlineCommentEnd()
{
    TRACE(_T("%s : line=%d, CurrentChar='%c', PreviousChar='%c', NextChar='%c'"),
          wxString(__PRETTY_FUNCTION__, wxConvUTF8).wc_str(), m_LineNumber, CurrentChar(),
          PreviousChar(), NextChar());

    // skip everything until we find EOL
    while (true)
    {
        SkipToChar('\n');
        if (IsEOF() || !IsBackslashBeforeEOL())
            break;
        else
            MoveToNextChar();
    }

    TRACE(_T("SkipToInlineCommentEnd(): (END) We are now at line %d, CurrentChar='%c', PreviousChar='%c',")
          _T(" NextChar='%c'"), m_LineNumber, CurrentChar(), PreviousChar(), NextChar());

    return NotEOF();
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

    return NotEOF();
}

// if we really move forward, return true, which means we have the new m_TokenIndex
// if we stay here, return false
bool Tokenizer::SkipComment()
{
    //TRACE(_T(Tokenizer::SkipComment() Start from line = %d"), m_LineNumber);
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
            TRACE(_T("Tokenizer::SkipComment() , Need to call SkipToEOL() here at line = %d"), m_LineNumber);

            SkipToInlineCommentEnd();
            break;
        }
    }

    return true;
}

bool Tokenizer::SkipUnwanted()
{
    while (SkipWhiteSpace() || SkipComment())
        ;

    wxChar c = CurrentChar();
    const unsigned int startIndex = m_TokenIndex;

    if (c == _T('#'))
    {
        const PreprocessorType type = GetPreprocessorType();
        if (type != ptOthers)
        {
            HandleConditionPreprocessor(type);
            c = CurrentChar();
        }
    }

    // skip [XXX][YYY]
    if (m_State & tsSkipSubScrip)
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
    if (m_State & tsSkipEqual)
    {
        if (c == _T('='))
        {
            if (!SkipToOneOfChars(_T(",;}"), true, true, false))
                return false;
        }
    }
    else if (m_State & tsSkipQuestion)
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

    if (startIndex != m_TokenIndex && CurrentChar() == _T('#'))
        return SkipUnwanted();

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
        if (SkipUnwanted())
            m_Token = DoGetToken();
        else
            m_Token.Clear();
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

        if (SkipUnwanted())
            m_PeekToken = DoGetToken();
        else
            m_PeekToken.Clear();

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
            ReadParentheses(str);
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
        MacroReplace(str);

    return str;
}

void Tokenizer::MacroReplace(wxString& str)
{
    wxStringHashMap::const_iterator it = s_Replacements.find(str);
    if (it == s_Replacements.end())
        return;

    TRACE(_T("MacroReplace() : Replacing '%s' with '%s' (file='%s', line='%d')."), it->first.wx_str(),
          it->second.wx_str(), m_Filename.wx_str(), m_LineNumber);

    if (it->second.IsEmpty())
    {
        SkipUnwanted();
        str = DoGetToken();
    }
    else if (it->second[0] == _T('+'))
    {
        SkipUnwanted();
        DoGetToken(); // eat (...)
        ReplaceBufferForReparse(&it->second[1]);
        str = DoGetToken();
    }
    else if (it->second[0] == _T('-'))
    {
        wxString end(&it->second[1]);
        if (end.IsEmpty())
            return;

        while (NotEOF())
        {
            while (SkipComment() && SkipWhiteSpace())
                ;
            if (CurrentChar() == end[0])
            {
                if (DoGetToken() == end)
                    break;
            }
            else
                MoveToNextChar();
        }

        // eat ()
        SkipUnwanted();
        str = DoGetToken();
        if (str[0] == _T('('))
        {
            SkipUnwanted();
            str = DoGetToken();
        }
    }
    else
    {
        ReplaceBufferForReparse(it->second);
        str = DoGetToken();
    }
}

bool Tokenizer::CalcConditionExpression()
{
    // need to force the tokenizer skip raw expression
    const TokenizerState oldState = m_State;
    m_State = tsReadRawExpression;

    const unsigned int undoIndex = m_TokenIndex;
    const unsigned int undoLine = m_LineNumber;
    SkipToEOL(false);
    const unsigned int expEndIndex = m_TokenIndex;
    m_TokenIndex = undoIndex;
    m_LineNumber = undoLine;

    Expression exp;
    while (m_TokenIndex < expEndIndex)
    {
        while (SkipComment())
            ;
        wxString token = DoGetToken();
        if (token[0] <= _T(' ') || token == _T("defined") || token == _T("\\"))
            continue;

        if (token.Len() > 1 && !wxIsdigit(token[0]))
        {
            const int id = m_pTokensTree->TokenExists(token, -1, tkPreprocessor);
            if (id != -1)
            {
                Token* tk = m_pTokensTree->at(id);
                if (tk)
                {
                    if (tk->m_Type.IsEmpty() || tk->m_Type == token)
                    {
                        exp.AddToInfixExpression(_T("1"));
                        continue;
                    }
                    else if (wxIsdigit(tk->m_Type[0]))
                        token = tk->m_Type;
                    else
                    {
                        ReplaceBufferForReparse(tk->m_Type);
                        continue;
                    }
                }
            }
            else
            {
                exp.AddToInfixExpression(_T("0"));
                continue;
            }
        }

        if (!token.StartsWith(_T("0x")))
            exp.AddToInfixExpression(token);
        else
        {
            long value;
            if (token.ToLong(&value, 16))
                exp.AddToInfixExpression(wxString::Format(_T("%d"), value));
            else
                exp.AddToInfixExpression(_T("0"));
        }
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
    int id = m_pTokensTree->TokenExists(DoGetToken(), -1, tkPreprocessor);
    SkipToEOL(false);
    return (id != -1);
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

PreprocessorType Tokenizer::GetPreprocessorType()
{
    const unsigned int undoIndex = m_TokenIndex;
    const unsigned int undoLine = m_LineNumber;

    MoveToNextChar();
    while (SkipWhiteSpace() || SkipComment())
        ;

    const wxString token = DoGetToken();

    switch (token.Len())
    {
    case 2:
        if (token == TokenizerConsts::kw_if)
            return ptIf;
        break;

    case 4:
        if (token == TokenizerConsts::kw_else)
            return ptElse;
        else if (token == TokenizerConsts::kw_elif)
            return ptElif;
        break;

    case 5:
        if (token == TokenizerConsts::kw_ifdef)
            return ptIfdef;
        else if (token == TokenizerConsts::kw_endif)
            return ptEndif;
        break;

    case 6:
        if (token == TokenizerConsts::kw_ifndef)
            return ptIfndef;
        break;

    case 7:
        if (token == TokenizerConsts::kw_elifdef)
            return ptElifdef;
        break;

    case 8:
        if (token == TokenizerConsts::kw_elifndef)
            return ptElifndef;
        break;
    }

    m_TokenIndex = undoIndex;
    m_LineNumber = undoLine;
    return ptOthers;
}

void Tokenizer::HandleConditionPreprocessor(const PreprocessorType type)
{
    switch (type)
    {
    case ptIf:
        {
            TRACE(_T("HandleConditionPreprocessor() : #if at line = %d"), m_LineNumber);
            bool result = CalcConditionExpression();
            m_ExpressionResult.push(result);
            if (!result)
               SkipToNextConditionPreprocessor();
        }
        break;

    case ptIfdef:
        {
            TRACE(_T("HandleConditionPreprocessor() : #ifdef at line = %d"), m_LineNumber);
            bool result = IsMacroDefined();
            m_ExpressionResult.push(result);
            if (!result)
               SkipToNextConditionPreprocessor();
        }
        break;

    case ptIfndef:
        {
            TRACE(_T("HandleConditionPreprocessor() : #ifndef at line = %d"), m_LineNumber);
            bool result = !IsMacroDefined();
            m_ExpressionResult.push(result);
            if (!result)
               SkipToNextConditionPreprocessor();
        }
        break;

    case ptElif:
        {
            TRACE(_T("HandleConditionPreprocessor() : #elif at line = %d"), m_LineNumber);
            bool result = false;
            if (!m_ExpressionResult.empty() && !m_ExpressionResult.top())
                result = CalcConditionExpression();
            if (result)
                m_ExpressionResult.top() = true;
            else
                SkipToNextConditionPreprocessor();
        }
        break;

    case ptElifdef:
        {
            TRACE(_T("HandleConditionPreprocessor() : #elifdef at line = %d"), m_LineNumber);
            bool result = false;
            if (!m_ExpressionResult.empty() && !m_ExpressionResult.top())
                result = IsMacroDefined();
            if (result)
                m_ExpressionResult.top() = true;
            else
                SkipToNextConditionPreprocessor();
        }
        break;

    case ptElifndef:
        {
            TRACE(_T("HandleConditionPreprocessor() : #elifndef at line = %d"), m_LineNumber);
            bool result = false;
            if (!m_ExpressionResult.empty() && !m_ExpressionResult.top())
                result = !IsMacroDefined();
            if (result)
                m_ExpressionResult.top() = true;
            else
                SkipToNextConditionPreprocessor();
        }
        break;

    case ptElse:
        {
            TRACE(_T("HandleConditionPreprocessor() : #else at line = %d"), m_LineNumber);
            if (!m_ExpressionResult.empty() && !m_ExpressionResult.top())
                SkipToEOL(false);
            else
                SkipToEndConditionPreprocessor();
        }
        break;

    case ptEndif:
        {
            TRACE(_T("HandleConditionPreprocessor() : #endif at line = %d"), m_LineNumber);
            SkipToEOL(false);
            if (!m_ExpressionResult.empty())
                m_ExpressionResult.pop();
        }
        break;

    case ptOthers:
        break;
    }
}

void Tokenizer::SpliteMacroActualArgument(wxArrayString& results)
{
    const unsigned int savedTokenIndex = m_TokenIndex;
    UngetToken();
    m_PeekAvailable = false; // as the peek is already parsed, we need to reparse the peek string

    if (CurrentChar() != _T('('))
        return;

    const TokenizerState oldState = m_State;
    m_State = tsSkipNone;

    MoveToNextChar(); // Skip the '('

    wxString testStr;
    wxString piece;
    while (m_TokenIndex < savedTokenIndex)
    {
        testStr = DoGetToken();
        if (testStr == _T(","))
        {
            results.Add(piece);
            piece.Clear();
        }
        else if (testStr == _T(")"))
        {
            results.Add(piece);
            break; // if it is ')' or other chracters, we break, we hopefully it is a ')'
        }
        else
            piece << testStr << _T(" ");
    }

    m_State = oldState;
}

void Tokenizer::ReplaceBufferForReparse(const wxString& target)
{
    wxString buffer(target);
    if (buffer.IsEmpty())
        return;

    // Keep all in one line
    for (size_t i = 0; i < buffer.Len(); ++i)
    {
        switch (buffer.GetChar(i))
        {
        case _T('\\'):
        case _T('\r'):
        case _T('\n'):
            buffer.SetChar(i, _T(' '));
        }
    }

    // Increase memory
    const size_t bufLen = buffer.Len();
    if (m_TokenIndex < bufLen)
    {
        const size_t diffLen = bufLen - m_TokenIndex;
        m_Buffer.insert(0, wxString(_T(' '), diffLen));
        m_BufferLen += diffLen;
        m_TokenIndex += diffLen;
        m_UndoTokenIndex += diffLen;
        m_PeekTokenIndex += diffLen;
    }

    // Replacement back
    wxChar* p = const_cast<wxChar*>(m_Buffer.GetData()) + m_TokenIndex - bufLen;
    TRACE(_T("ReplaceBufferForReparse() : <FROM>:%s<TO>:%s"), wxString(p, bufLen).wx_str(), buffer.wx_str());
    memcpy(p, buffer.GetData(), bufLen * sizeof(wxChar));

    // Fix token index
    m_TokenIndex -= bufLen;
}
