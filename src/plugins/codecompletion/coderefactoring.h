/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CODEREFACTORING_H
#define CODEREFACTORING_H

#include "nativeparser.h"

struct crSearchData
{
    int pos;
    int line;
    const wxString text;
    crSearchData(int pos, int line, const wxString& text) :
        pos(pos),
        line(line),
        text(text)
    {}
};

class CodeRefactoring
{
    typedef std::list<crSearchData> SearchDataList;
    typedef std::map<wxString, SearchDataList> SearchDataMap;

public:
    CodeRefactoring(NativeParser& np);
    virtual ~CodeRefactoring();

    void FindReferences();
    void RenameSymbols();

private:
    size_t SearchInFiles(cbProject* project, const wxString& targetText, bool onlyOpenFiles);
    size_t VerifyResult(cbProject* project, const TokenIdxSet& targetResult, const wxString& targetText);
    void Find(cbStyledTextCtrl* control, const wxString& file, const wxString& target);
    wxString GetSymbolUnderCursor();

    void DoFindReferences();
    void DoRenameSymbols(const wxString& targetText, const wxString& replaceText);

private:
    bool Parse();

private:
    NativeParser& m_NativeParser;
    SearchDataMap m_SearchDataMap;
};

#endif // CODEREFACTORING_H
