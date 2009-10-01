/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include "parsewatchvalue.h"

struct Token
{
    enum Type
    {
        Undefined,
        OpenBrace,
        CloseBrace,
        Equal,
        String,
        Comma
    };

    Token() : type(Undefined)
    {
    }
    Token(int start_, int end_, Type type_) :
        start(start_),
        end(end_),
        type(type_)
    {
    }

    bool operator == (Token const &t) const
    {
        return start == t.start && end == t.end && type == t.type;
    }
    wxString ExtractString(wxString const &s) const
    {
        assert(end <= static_cast<int>(s.length()));
        return s.substr(start, end - start);
    }

    void Trim(wxString const &s)
    {
        while(start < static_cast<int>(s.length())
              && (s[start] == wxT(' ') || s[start] == wxT('\t') || s[start] == wxT('\n')))
            start++;
        while(end > 0
              && (s[end - 1] == wxT(' ') || s[end - 1] == wxT('\t') || s[end - 1] == wxT('\n')))
            end--;
    }

    int start, end;
    Type type;
};

bool GetNextToken(wxString const &str, int pos, Token &token)
{
    while(pos < static_cast<int>(str.length())
          && (str[pos] == _T(' ') || str[pos] == _T('\t') || str[pos] == _T('\n')))
        ++pos;

    if(pos >= static_cast<int>(str.length()))
        return false;

    token.start = -1;
    bool in_quote = false;
    int open_angle_braces = 0;

    switch(str[pos])
    {
    case _T('='):
        token = Token(pos, pos + 1, Token::Equal);
        return true;
    case _T(','):
        token = Token(pos, pos + 1, Token::Comma);
        return true;
    case _T('{'):
        token = Token(pos, pos + 1, Token::OpenBrace);
        return true;
    case _T('}'):
        token = Token(pos, pos + 1, Token::CloseBrace);
        return true;

    case _T('"'):
        in_quote = true;
        token.type = Token::String;
        token.start = pos;
        break;
    case _T('<'):
        token.type = Token::String;
        token.start = pos;
        open_angle_braces = 1;
    default:
        token.type = Token::String;
        token.start = pos;
    }
    ++pos;

    bool escape_next = false;
    while(pos < static_cast<int>(str.length()))
    {
        if(open_angle_braces == 0)
        {
            if((str[pos] == _T(',') || str[pos] == _T('=') || str[pos] == _T('{')
                || str[pos] == _T('}'))
                && !in_quote)
            {
                token.end = pos;
                return true;
            }
            else if(str[pos] == _T('"') && in_quote && !escape_next)
            {
                token.end = pos + 1;
                return true;
            }
            else if(str[pos] == _T('\\'))
            {
                escape_next = true;
            }
            else
            {
                escape_next = false;
            }
            if(str[pos] == wxT('<'))
                open_angle_braces++;
        }
        else
        {
            if(str[pos] == wxT('<'))
                open_angle_braces++;
            else if(str[pos] == wxT('>'))
                --open_angle_braces;
        }
        ++pos;
    }

    if(in_quote)
    {
        token.end = -1;
        return false;
    }
    else
    {
        token.end = pos;
        return true;
    }
}

GDBWatch* AddChild(GDBWatch &parent, wxString const &full_value, Token &name)
{
    wxString const &str_name = name.ExtractString(full_value);
    cbWatch *old_child = parent.FindChild(str_name);
    GDBWatch *child;
    if(old_child)
        child = static_cast<GDBWatch*>(old_child);
    else
    {
        child = new GDBWatch(str_name);
        parent.AddChild(child);
    }
    child->MarkAsRemoved(false);
    return child;
}

GDBWatch* AddChild(GDBWatch &parent, wxString const &str_name)
{
    int index = parent.FindChildIndex(str_name);
    GDBWatch *child;
    if(index != -1)
        child = static_cast<GDBWatch*>(parent.GetChild(index));
    else
    {
        child = new GDBWatch(str_name);
        parent.AddChild(child);
        index = parent.GetChildCount();
    }
    child->MarkAsRemoved(false);
    return child;
}

bool ParseGDBWatchValue(GDBWatch &watch, wxString const &value, int &start, int length)
{
    watch.SetDebugValue(value);
    watch.MarkChildsAsRemoved();

    int position = start;
    Token token, token_name, token_value;
    bool skip_comma = false;
    bool last_was_closing_brace = false;
    int added_children = 0;
    int token_real_end = 0;
    while(GetNextToken(value, position, token))
    {
//        if(length > 0 && token.start >= start + length - 1)
//            break;
        token_real_end = token.end;
        token.Trim(value);
        wxString dbg = token.ExtractString(value);
        switch (token.type)
        {
        case Token::String:
            if(token_name.type == Token::Undefined)
                token_name = token;
            else if(token_value.type == Token::Undefined)
                token_value = token;
            else
                return false;
            last_was_closing_brace = false;
            break;
        case Token::Equal:
            last_was_closing_brace = false;
            break;
        case Token::Comma:
            last_was_closing_brace = false;
            if(skip_comma)
                skip_comma = false;
            else
            {
                if(token_name.type != Token::Undefined)
                {
                    if(token_value.type != Token::Undefined)
                    {
                        GDBWatch *child = AddChild(watch, value, token_name);
                        child->SetValue(token_value.ExtractString(value));
                    }
                    else
                    {
                        int start = watch.IsArray() ? watch.GetArrayStart() : 0;
                        GDBWatch *child = AddChild(watch, wxString::Format(wxT("[%d]"), start + added_children));
                        child->SetValue(token_name.ExtractString(value));
                    }
                    token_name.type = token_value.type = Token::Undefined;
                    added_children++;
                }
                else
                    return false;
            }
            break;
        case Token::OpenBrace:
            {
                if(token_name.type == Token::Undefined)
                    return false;
                GDBWatch *child = AddChild(watch, value, token_name);
                position = token_real_end;
                added_children++;
                if(!ParseGDBWatchValue(*child, value, position, 0))
                    return false;
                token_real_end = position;
                token_name.type = token_value.type = Token::Undefined;
                skip_comma = true;
                last_was_closing_brace = true;
            }
            break;
        case Token::CloseBrace:
            if (!last_was_closing_brace)
            {
                if (token_name.type != Token::Undefined)
                {
                    if (token_value.type != Token::Undefined)
                    {
                        GDBWatch *child = AddChild(watch, value, token_name);
                        child->SetValue(token_value.ExtractString(value));
                    }
                    else
                    {
                        int start = watch.IsArray() ? watch.GetArrayStart() : 0;
                        GDBWatch *child = AddChild(watch, wxString::Format(wxT("[%d]"), start + added_children));
                        child->SetValue(token_name.ExtractString(value));
                    }
                    token_name.type = token_value.type = Token::Undefined;
                    added_children++;
                }
                else
                    return false;
            }

            start = token_real_end;
            return true;
        default:
            return false;
        }

        position = token_real_end;
        if(length > 0 && position >= start + length)
            break;
    }

    start = position + 1;
    if(token_name.type != Token::Undefined)
    {
        if(token_value.type != Token::Undefined)
        {
            GDBWatch *child = AddChild(watch, value, token_name);
            child->SetValue(token_value.ExtractString(value));
        }
        else
        {
            int start = watch.IsArray() ? watch.GetArrayStart() : 0;
            GDBWatch *child = AddChild(watch, wxString::Format(wxT("[%d]"), start + added_children));
            child->SetValue(token_name.ExtractString(value));
        }
    }

    return true;
}
bool ParseGDBWatchValue(GDBWatch &watch, wxString const &value)
{
    if(value.empty())
    {
        watch.SetValue(value);
        return true;
    }

    // Try to find the first brace.
    // If the watch is for a reference the brace is not at position = 0
    wxString::size_type start = value.find(wxT('{'));

    if(start != wxString::npos && value[value.length() - 1] == wxT('}'))
    {
        int t_start = start + 1;
        bool result = ParseGDBWatchValue(watch, value, t_start, value.length() - 2);
        if(result)
        {
            if(start > 0)
                watch.SetValue(value.substr(0, start));
            watch.RemoveChildren(true);
        }
        return result;
    }
    else
    {
        watch.SetValue(value);
        watch.RemoveChildren();
        return true;
    }
    return false;
}
