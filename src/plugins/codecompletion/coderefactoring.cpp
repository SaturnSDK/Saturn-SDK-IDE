/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <sdk.h>
#include "coderefactoring.h"
#include "nativeparser.h"
#include "cbstyledtextctrl.h"
#include "cbproject.h"
#include "encodingdetector.h"
#include "searchresultslog.h"

#include <wx/progdlg.h>

CodeRefactoring::CodeRefactoring(NativeParser& np) :
    m_NativeParser(np)
{
}

CodeRefactoring::~CodeRefactoring()
{
}

wxString CodeRefactoring::GetSymbolUnderCursor()
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor)
        return wxEmptyString;

    if (!m_NativeParser.GetParser()->Done())
    {
        cbMessageBox(_("C++ Parser is still parsing files..."), _("Code Refactoring"), wxOK | wxICON_WARNING);
        return wxEmptyString;
    }

    const int pos = editor->GetControl()->GetCurrentPos();
    const int start = editor->GetControl()->WordStartPosition(pos, true);
    const int end = editor->GetControl()->WordEndPosition(pos, true);
    return editor->GetControl()->GetTextRange(start, end);
}

bool CodeRefactoring::Parse()
{
    cbEditor* editor = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!editor)
        return false;

    const wxString targetText = GetSymbolUnderCursor();
    if (targetText.IsEmpty())
        return false;

    TokenIdxSet targetResult;
    m_NativeParser.MarkItemsByAI(targetResult, m_NativeParser.GetParser()->Options().useSmartSense);
    m_NativeParser.RemoveInvalid(targetResult, targetText);
    if (targetResult.empty())
    {
        cbMessageBox(_("Symbol not found under cursor!"), _("Code Refactoring"), wxOK | wxICON_WARNING);
        return false;
    }

    cbProject* project = m_NativeParser.GetProjectByFilename(editor->GetFilename());
    const int ret = cbMessageBox(_("Only search open files? Select \"No\" search the project!"),
                                 _("Code Refactoring"), wxYES_NO);
    size_t count = SearchInFiles(project, targetText, ret == wxID_YES);
    if (count)
        count = VerifyResult(project, targetResult, targetText);

    return !!count;
}

void CodeRefactoring::FindReferences()
{
    if (Parse())
        DoFindReferences();
}

void CodeRefactoring::RenameSymbols()
{
    const wxString targetText = GetSymbolUnderCursor();
    if (targetText.IsEmpty())
        return;

    wxString replaceText = wxGetTextFromUser(_("Rename symbols under cursor"),
                                             _("Code Refactoring"),
                                             targetText,
                                             Manager::Get()->GetAppWindow());
    if (!replaceText.IsEmpty() && replaceText != targetText && Parse())
        DoRenameSymbols(targetText, replaceText);
}

size_t CodeRefactoring::SearchInFiles(cbProject* project, const wxString& targetText, bool onlyOpenFiles)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor)
        return 0;

    m_SearchDataMap.clear();
    wxArrayString files;

    if (onlyOpenFiles)
    {
        for (int i = 0; i < edMan->GetEditorsCount(); ++i)
            files.Add(edMan->GetEditor(i)->GetFilename());
    }
    else
    {
        if (!project)
            files.Add(editor->GetFilename());
        else
        {
            // fill the search list with all the project files
            for (int i = 0; i < project->GetFilesCount(); ++i)
            {
                ProjectFile* pf = project->GetFile(i);
                FileType ft = CCFileTypeOf(pf->relativeFilename);
                if (ft != ftOther)
                    files.Add(pf->file.GetFullPath());
            }
        }
    }

    if (files.IsEmpty())
        return 0;

    // now that list is filled, we'll search
    cbStyledTextCtrl* control = new cbStyledTextCtrl(editor->GetParent(), wxID_ANY, wxDefaultPosition, wxSize(0, 0));
    control->Show(false);

    // let's create a progress dialog because it might take some time depending on the files count
    wxProgressDialog* progress = new wxProgressDialog(_("Code Refactoring"),
                                                      _("Please wait while searching inside the project..."),
                                                      files.GetCount(),
                                                      Manager::Get()->GetAppWindow(),
                                                      wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT);
    PlaceWindow(progress);

    for (size_t i = 0; i < files.GetCount(); ++i)
    {
        // update the progress bar
        if (!progress->Update(i))
            break; // user pressed "Cancel"

        // check if the file is already opened in built-in editor and do search in it
        cbEditor* ed = edMan->IsBuiltinOpen(files[i]);
        if (ed)
            control->SetText(ed->GetControl()->GetText());
        else // else load the file in the control
        {
            EncodingDetector detector(files[i]);
            if (!detector.IsOK())
                continue; // failed
            control->SetText(detector.GetWxStr());
        }

        Find(control, files[i], targetText);
    }

    delete control; // done with it
    delete progress; // done here too

    return m_SearchDataMap.size();
}

size_t CodeRefactoring::VerifyResult(cbProject* project, const TokenIdxSet& targetResult,
                                     const wxString& targetText)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor)
        return 0;

    // now that list is filled, we'll search
    cbStyledTextCtrl* control = new cbStyledTextCtrl(editor->GetParent(), wxID_ANY, wxDefaultPosition, wxSize(0, 0));
    control->Show(false);

    size_t totalCount = 0;
    for (SearchDataMap::iterator it = m_SearchDataMap.begin(); it != m_SearchDataMap.end(); ++it)
        totalCount += it->second.size();

    // let's create a progress dialog because it might take some time depending on the files count
    wxProgressDialog* progress = new wxProgressDialog(_("Code Refactoring"),
                                                      _("Please wait while verifying result..."),
                                                      totalCount,
                                                      Manager::Get()->GetAppWindow(),
                                                      wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT);
    PlaceWindow(progress);

    size_t task = totalCount;
    TokenIdxSet result;
    bool userBreak = false;
    for (SearchDataMap::iterator it = m_SearchDataMap.begin(); it != m_SearchDataMap.end();)
    {
        // check if the file is already opened in built-in editor and do search in it
        cbEditor* ed = edMan->IsBuiltinOpen(it->first);
        if (ed)
            control->SetText(ed->GetControl()->GetText());
        else // else load the file in the control
        {
            EncodingDetector detector(it->first);
            if (!detector.IsOK())
            {
                task -= it->second.size();
                continue; // failed
            }
            control->SetText(detector.GetWxStr());
        }

        ccSearchData searchData = { control, it->first };

        for (SearchDataList::iterator itList = it->second.begin(); itList != it->second.end();)
        {
            // update the progress bar
            if (!progress->Update(totalCount - (--task)))
            {
                userBreak = true;
                break; // user pressed "Cancel"
            }

            // e.g. void |Test(...
            // we *must* goto pos as "void T|est(..."
            control->GotoPos(itList->pos + 1);

            m_NativeParser.MarkItemsByAI(&searchData, result, m_NativeParser.GetParser()->Options().useSmartSense);
            m_NativeParser.RemoveInvalid(result, targetText);

            TokenIdxSet::iterator intersect = std::find_first_of(targetResult.begin(), targetResult.end(),
                                                                 result.begin(), result.end());
            if (intersect == targetResult.end())
                it->second.erase(itList++);
            else
                ++itList;
        }

        if (it->second.empty())
            m_SearchDataMap.erase(it++);
        else
            ++it;

        if (userBreak)
            break;
    }

    delete control; // done with it
    delete progress; // done here too

    return m_SearchDataMap.size();
}

void CodeRefactoring::Find(cbStyledTextCtrl* control, const wxString& file, const wxString& target)
{
    const int end = control->GetLength();
    int start = 0;
    int pos = 0;

    for (;;)
    {
        int lengthFound;
        pos = control->FindText(start, end, target, wxSCI_FIND_WHOLEWORD | wxSCI_FIND_MATCHCASE, &lengthFound);
        if (pos != wxSCI_INVALID_POSITION)
        {
            start = pos + lengthFound;

            // TODO (Loaden) not work?
            const int style = control->GetStyleAt(pos); // always been zero?
            if (control->IsString(style) || control->IsComment(style))
                continue;

            int line = control->LineFromPosition(pos);
            wxString text = control->GetLine(line).Trim(true).Trim(false);
            m_SearchDataMap[file].push_back(crSearchData(pos, line, text));
        }
        else
            break;
    }
}

void CodeRefactoring::DoFindReferences()
{
    cbEditor* editor = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!editor)
        return;

    SearchResultsLog* searchLog = Manager::Get()->GetEditorManager()->GetSearchResultLogger();

    const wxString focusFile = editor->GetFilename();
    const int focusLine = editor->GetControl()->GetCurrentLine();
    wxFileName fn(focusFile);
    const wxString basePath(fn.GetPath());
    size_t index = 0;
    size_t focusIndex = 0;

    searchLog->Clear();
    searchLog->SetBasePath(basePath);

    for (SearchDataMap::iterator it = m_SearchDataMap.begin(); it != m_SearchDataMap.end(); ++it)
    {
        for (SearchDataList::iterator itList = it->second.begin(); itList != it->second.end(); ++itList)
        {
            if (it->first == focusFile && itList->line == focusLine)
                focusIndex = index;

            wxArrayString values;
            wxFileName curFn(it->first);
            curFn.MakeRelativeTo(basePath);
            values.Add(curFn.GetFullPath());
            values.Add(wxString::Format(_T("%d"), itList->line + 1));
            values.Add(itList->text);
            searchLog->Append(values, Logger::info);

            ++index;
        }
    }

    if (Manager::Get()->GetConfigManager(_T("message_manager"))->ReadBool(_T("/auto_show_search"), true))
    {
        CodeBlocksLogEvent evtSwitch(cbEVT_SWITCH_TO_LOG_WINDOW, searchLog);
        CodeBlocksLogEvent evtShow(cbEVT_SHOW_LOG_MANAGER);
        Manager::Get()->ProcessEvent(evtSwitch);
        Manager::Get()->ProcessEvent(evtShow);
    }

    searchLog->FocusEntry(focusIndex);
}

void CodeRefactoring::DoRenameSymbols(const wxString& targetText, const wxString& replaceText)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor)
        return;

    cbProject* project = m_NativeParser.GetProjectByFilename(editor->GetFilename());

    for (SearchDataMap::iterator it = m_SearchDataMap.begin(); it != m_SearchDataMap.end(); ++it)
    {
        // check if the file is already opened in built-in editor and do search in it
        cbEditor* ed = edMan->IsBuiltinOpen(it->first);
        if (!ed)
        {
            ProjectFile* pf = project ? project->GetFileByFilename(it->first) : 0;
            ed = edMan->Open(it->first, it->second.front().pos, pf);
        }

        cbStyledTextCtrl* control = ed->GetControl();
        control->BeginUndoAction();

        for (SearchDataList::reverse_iterator rIter = it->second.rbegin(); rIter != it->second.rend(); ++rIter)
        {
            control->SetTargetStart(rIter->pos);
            control->SetTargetEnd(rIter->pos + targetText.Len());
            control->ReplaceTarget(replaceText);
        }

        control->EndUndoAction();
    }
}
