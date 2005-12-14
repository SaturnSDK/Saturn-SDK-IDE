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
* $Id$
* $Date$
*/

#include <sdk.h>
#include "tokenizer.h"
#include <wx/utils.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include "../../../sdk/manager.h"
#include <cctype>


namespace TokenizerConsts
{
const wxString colon(_T(":"));
const wxString colon_colon(_T("::"));
const wxString operator_str(_T("operator"));
const wxString include_str(_T("#include"));
const wxString hash(_T("#"));
const wxString tabcrlf(_T("\t\n\r"));
};

Tokenizer::Tokenizer(const wxString& filename)
	: m_Filename(filename),
	m_BufferLen(0),
	m_NestLevel(0),
	m_UndoNestLevel(0),
	m_TokenIndex(0),
	m_UndoTokenIndex(0),
	m_LineNumber(1),
	m_UndoLineNumber(1),
	m_IsOK(false),
	m_IsOperator(false)
{
	//ctor
	m_Options.wantPreprocessor = false;
	if (!m_Filename.IsEmpty())
		Init(m_Filename);
}

Tokenizer::~Tokenizer()
{
	//dtor
}

bool Tokenizer::Init(const wxString& filename)
{
	BaseInit();
	if (filename.IsEmpty())
	{
		if (m_Filename.IsEmpty())
		{
			wxMessageBox(_T("Tokenizer::Init() called without filename..."));
			return false;
		}
	}
	else
		m_Filename = filename;

	if (!wxFileExists(m_Filename))
		return false;

	if (!ReadFile())
	{
		wxMessageBox(_T("File ") + filename + _T(" does not exist..."));
		return false;
	}

	if (!m_BufferLen)
	{
		//wxMessageBox("File is empty!");
		return false;
	}

	m_IsOK = true;
	return true;
}

bool Tokenizer::InitFromBuffer(const wxString& buffer)
{
	BaseInit();
	m_Buffer = buffer;
	m_BufferLen = buffer.Length();
	m_IsOK = true;
	m_Filename.Clear();
	return true;
}

void Tokenizer::BaseInit()
{
	m_TokenIndex = 0;
	m_UndoTokenIndex = 0;
	m_LineNumber = 1;
	m_UndoLineNumber = 1;
	m_NestLevel = 0;
	m_UndoNestLevel = 0;
	m_IsOperator = false;
	m_BufferLen = 0;
	m_LastWasPreprocessor = false;
	m_LastPreprocessor.Clear();
	m_Buffer.Clear();
	m_IsOK = false;
}

bool Tokenizer::ReadFile()
{
    if (!wxFileExists(m_Filename))
        return false;

    // open file
    wxFile file(m_Filename);

    if (!cbRead(file,m_Buffer))
        return false;
	m_BufferLen = m_Buffer.Length();

    return true;
}

bool Tokenizer::SkipWhiteSpace()
{
    // skip spaces, tabs, etc.
    while (CurrentChar() <= _T(' ') && MoveToNextChar()) // don't check EOF when MoveToNextChar already does, also replace replace isspace() which calls msvcrt.dll
        ;             									 // with a dirty hack:  CurrentChar() <= ' ' is "good enough" here
    if (unlikely(IsEOF()))
        return false;
    return true;
}

bool Tokenizer::SkipToChar(const wxChar& ch)
{
    // skip everything until we find ch
    while(true)
    {
        while (CurrentChar() != ch && MoveToNextChar())  // don't check EOF when MoveToNextChar already does
            ;

        if (unlikely(IsEOF()))
            return false;

        if (PreviousChar() != '\\')
            break;
        else
        {
            // check for "\\"
            if (m_TokenIndex - 2 >= 0 && m_Buffer.GetChar(m_TokenIndex - 2) == '\\')
                break;
        }
        MoveToNextChar();
    }
    return true;
}


bool Tokenizer::SkipToOneOfChars(const char* chars, bool supportNesting)
{
	// skip everything until we find any one of chars
	while (1)
	{
		while (NotEOF() && !CharInString(CurrentChar(), chars))
		{
			if (CurrentChar() == '"' || CurrentChar() == '\'')
			{
				// this is the case that match is inside a string!
				char ch = CurrentChar();
				MoveToNextChar();
				SkipToChar(ch);
			}
			MoveToNextChar();
			if (supportNesting)
			{
				if (CurrentChar() == '{')
					SkipBlock('{');
			}
		}
		if (PreviousChar() != '\\')
			break;
		else
		{
			// check for "\\"
			if (m_TokenIndex - 2 >= 0 && m_Buffer.GetChar(m_TokenIndex - 2) == '\\')
				break;
		}
		MoveToNextChar();
	}
	if (IsEOF())
		return false;
	return true;
}

bool Tokenizer::SkipToEOL()
{
	// skip everything until we find EOL
	while (1)
	{
		while (NotEOF() && CurrentChar() != '\n')
			MoveToNextChar();
		wxChar last = PreviousChar();
		// if DOS line endings, we 've hit \r and we skip to \n...
		if (last == '\r')
			last = m_Buffer.GetChar(m_TokenIndex - 2);
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
	int count = 1; // counter for nested blocks (xxx())
	while (NotEOF())
	{
		if (CurrentChar() == '"' || CurrentChar() == '\'')
		{
			// this is the case that match is inside a string!
			char ch = CurrentChar();
			MoveToNextChar();
			SkipToChar(ch);
			MoveToNextChar();
		}
		if (CurrentChar() == ch)
			++count;
		else if (CurrentChar() == match)
			--count;
		MoveToNextChar();
		if (count == 0)
			break;
	}
	if (IsEOF())
		return false;
	return true;
}

bool Tokenizer::SkipUnwanted()
{
	while (CurrentChar() == '#' ||
			(!m_IsOperator && CurrentChar() == '=') ||
			(!m_IsOperator && CurrentChar() == '[') ||
			CurrentChar() == '?' ||
			CurrentChar() == '/' && (NextChar() == '/' || NextChar() == '*') )
	{
		bool skipPreprocessor = false; // used for #include
		while (m_Buffer.Mid(m_TokenIndex, 2) == _T("//") ||
				m_Buffer.Mid(m_TokenIndex, 2) == _T("/*"))
		{
			// C/C++ style comments
			bool cstyle = NextChar() == '*';
			MoveToNextChar(2);
			//MoveToNextChar();
			while (1)
			{
				if (!cstyle)
				{
					if (!SkipToEOL())
						return false;
					MoveToNextChar();
					break;
				}
				else
				{
					if (SkipToChar('/'))
					{
						if (PreviousChar() == '*')
						{
							MoveToNextChar();
							break;
						}
						MoveToNextChar();
					}
					else
						return false;
				}
			}
			if (IsEOF())
				return false;
			if (!SkipWhiteSpace())
				return false;
		}

		while (CurrentChar() == '#')
		{
			// preprocessor directives
			// we only care for #include and #define, for now
			unsigned int backupIdx = m_TokenIndex;
			MoveToNextChar();
			SkipWhiteSpace();
			if ((CurrentChar() == 'i' && NextChar() == 'n') || // in(clude)
				(m_Options.wantPreprocessor && CurrentChar() == 'd' && NextChar() == 'e')) // de(fine)
			{
				// ok, we have something like #in(clude)
				m_LastWasPreprocessor = true;
				m_LastPreprocessor.Clear();
				m_TokenIndex = backupIdx; // keep #
				skipPreprocessor = true;
				break;
			}
			else
			{
				// skip the rest for now...
				SkipToEOL();
				if (!SkipWhiteSpace())
					return false;
			}
			if (skipPreprocessor)
				break;
		}

		while (CurrentChar() == '[')
		{
			// array subscripts
			// skip them for now...
			SkipBlock('[');
			if (!SkipWhiteSpace())
				return false;
		}

		while (CurrentChar() == '=')
		{
			// skip assignments
			// TODO: what happens with operators?
			if (!SkipToOneOfChars(",;}", true))
				return false;
		}

		while (CurrentChar() == '?')
		{
			// skip "condition ? true : false"
			// TODO: what happens with operators?
			if (!SkipToOneOfChars(";}"))
				return false;
		}
		if (skipPreprocessor)
			break;
	}
	return true;
}

wxString Tokenizer::GetToken()
{
	m_UndoTokenIndex = m_TokenIndex;
	m_UndoLineNumber = m_LineNumber;
	m_UndoNestLevel = m_NestLevel;
	return DoGetToken();
}

wxString Tokenizer::PeekToken()
{
	unsigned int undoTokenIndex = m_TokenIndex;
	unsigned int undoLineNumber = m_LineNumber;
	unsigned int undoNestLevel = m_NestLevel;
	wxString peek = DoGetToken();
	m_TokenIndex = undoTokenIndex;
	m_LineNumber = undoLineNumber;
	m_NestLevel = undoNestLevel;
	return peek;
}

void Tokenizer::UngetToken()
{
	m_TokenIndex = m_UndoTokenIndex;
	m_LineNumber = m_UndoLineNumber;
	m_NestLevel = m_UndoNestLevel;
}

wxString Tokenizer::DoGetToken()
{
	if (unlikely(IsEOF()))
		return wxEmptyString;

	if (!SkipWhiteSpace())
		return wxEmptyString;

	if (!SkipUnwanted())
		return wxEmptyString;

	int start = m_TokenIndex;
	wxString m_Str;

	if (CurrentChar() == '_' || isalpha(CurrentChar()))
	{
		// keywords, identifiers, etc.

		// operator== is cheaper than isalnum, also MoveToNextChar already includes IsEOF
		while (  ( CurrentChar() == '_' ||
				   isalnum(CurrentChar()) ) && MoveToNextChar()  )
		;

		if (unlikely(IsEOF()))
			return wxEmptyString;
		m_Str = m_Buffer.Mid(start, m_TokenIndex - start);
		m_IsOperator = m_Str.IsSameAs(TokenizerConsts::operator_str);
	}
	else if (isdigit(CurrentChar()))
	{
		// numbers
		while (NotEOF() && CharInString(CurrentChar(), "0123456789.abcdefABCDEFXxLl"))
			MoveToNextChar();
		if (IsEOF())
			return wxEmptyString;
		m_Str = m_Buffer.Mid(start, m_TokenIndex - start);
		m_IsOperator = false;
	}
	else if (CurrentChar() == '"' ||
			CurrentChar() == '\'')
	{
		// string, char, etc.
		wxChar match = CurrentChar();
		MoveToNextChar();  // skip starting ' or "
		if (!SkipToChar(match))
			return wxEmptyString;
		MoveToNextChar(); // skip ending ' or "
		m_Str = m_Buffer.Mid(start, m_TokenIndex - start);
	}
	else if (CurrentChar() == ':')
	{
		if (NextChar() == ':')
		{
			MoveToNextChar();
			MoveToNextChar();
			m_Str.assign(TokenizerConsts::colon_colon); // this only copies a pointer, but operator= allocates memory and does a memcpy!
		}
		else
		{
			MoveToNextChar();
			m_Str.assign(TokenizerConsts::colon);
		}
	}
	else if (CurrentChar() == '(')
	{
		m_IsOperator = false;
		// skip blocks () []
		if (!SkipBlock(CurrentChar()))
			return wxEmptyString;
		wxString tmp = m_Buffer.Mid(start, m_TokenIndex - start);
//		tmp.Replace(_T("\t"), _T(" ")); // replace tabs with spaces
//		tmp.Replace(_T("\n"), _T(" ")); // replace LF with spaces
//		tmp.Replace(_T("\r"), _T(" ")); // replace CR with spaces
		{ // this is much faster:
			size_t i;
			while((i = tmp.find_first_of(TokenizerConsts::tabcrlf)) != wxString::npos)
				tmp[i] = _T(' ');
		}
		// fix-up arguments (remove excessive spaces/tabs/newlines)
		for (unsigned int i = 0; i < tmp.Length() - 1; ++i)
		{
			if (tmp.GetChar(i) == '/' && tmp.GetChar(i + 1) == '*')
			{
				// skip C comments
				i += 2;
				while (i < tmp.Length() - 1)
				{
					if (tmp.GetChar(i) == '*' && tmp.GetChar(i + 1) == '/')
						break;
					++i;
				}
				if (i >= tmp.Length() - 1 || tmp.GetChar(i + 1) != '/')
					continue; // we failed...
				i += 2;
			}
			else if (tmp.GetChar(i) == '=')
			{
				// skip default assignments
				++i;
				int level = 0; // nesting parenthesis
				while (i < tmp.Length())
				{
					if (tmp.GetChar(i) == '(')
						++level;
					else if (tmp.GetChar(i) == ')')
						--level;
					if ((tmp.GetChar(i) == ',' && level == 0) ||
						(tmp.GetChar(i) == ')' && level < 0))
						break;
					++i;
				}
                if (i < tmp.Length() && tmp.GetChar(i) == ',')
                    --i;
				continue; // we are done here
			}

			if (i < tmp.Length() - 1 && tmp.GetChar(i) == ' ' && tmp.GetChar(i + 1) == ' ')
				continue; // skip excessive spaces
			m_Str << tmp.GetChar(i);
		}
		m_Str << _T(')'); // add closing parenthesis (see "i < tmp.Length() - 1" in previous "for")
//		m_Str.Replace(_T("  "), _T(" ")); // replace two-spaces with single-space (introduced if it skipped comments or assignments)
//		m_Str.Replace(_T("( "), _T("("));
//		m_Str.Replace(_T(" )"), _T(")"));
		//Str.Replace is massive overkill here since it has to allocate one new block per replacement
        CompactSpaces(m_Str);
	}
	else
	{
		if (CurrentChar() == '{')
			++m_NestLevel;
		else if (CurrentChar() == '}')
			--m_NestLevel;
		m_Str = CurrentChar();
		MoveToNextChar();
	}

	if (m_LastWasPreprocessor && !m_Str.IsSameAs(_T("#")) && !m_LastPreprocessor.IsSameAs(_T("#")))
	{
		if (!m_LastPreprocessor.IsSameAs(TokenizerConsts::include_str))
		{
			// except for #include, all other preprocessor directives need only
			// one word exactly after the directive, e.g. #define THIS_WORD
			SkipToEOL();
		}
		m_LastWasPreprocessor = false;
		m_LastPreprocessor.Clear();
	}

	if (m_LastWasPreprocessor)
		m_LastPreprocessor << m_Str;

	return m_Str;
}
