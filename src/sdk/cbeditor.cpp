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

#include "sdk_precomp.h"
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/fontutil.h>
#include <wx/notebook.h>

#include "cbeditor.h" // class's header file
#include "globals.h"
#include "sdk_events.h"
#include "projectbuildtarget.h" // for ProjectFile*
#include "editorcolorset.h"
#include "manager.h"
#include "configmanager.h"
#include "pluginmanager.h"
#include "editormanager.h"
#include "messagemanager.h"
#include "cbplugin.h"
#include "cbeditorprintout.h"


const wxString EDITOR_MODIFIED(_T("*"));

#define BOOKMARK_MARKER		0
#define BOOKMARK_STYLE 		wxSCI_MARK_ARROW

#define BREAKPOINT_MARKER	1
#define BREAKPOINT_STYLE 	wxSCI_MARK_CIRCLE

#define DEBUG_MARKER		5
#define DEBUG_STYLE 		wxSCI_MARK_ARROW

#define ERROR_LINE			4

BEGIN_EVENT_TABLE(cbStyledTextCtrl, wxScintilla)
	EVT_CONTEXT_MENU(cbStyledTextCtrl::OnContextMenu)
END_EVENT_TABLE()

cbStyledTextCtrl::cbStyledTextCtrl(wxWindow* pParent, int id, const wxPoint& pos, const wxSize& size, long style)
	: wxScintilla(pParent, id, pos, size, style),
	m_pParent(pParent)
{
	//ctor
}

cbStyledTextCtrl::~cbStyledTextCtrl()
{
	//dtor
}

// events

void cbStyledTextCtrl::OnContextMenu(wxContextMenuEvent& event)
{
	if (m_pParent)
		((cbEditor*)m_pParent)->DisplayContextMenu(event.GetPosition());
}

/* This struct holds private data for the cbEditor class.
 * It's a paradigm to avoid rebuilding the entire project (as cbEditor is a basic dependency)
 * for just adding a private var or method.
 * What happens is that we 've declared a cbEditorInternalData* private member in cbEditor
 * and define it in the .cpp file (here). Effectively, this means that we can now add/remove
 * elements from cbEditorInternalData without needing to rebuild the project :)
 * The cbEditor::m_pData is the variable to use in code. It's the very first thing
 * constructed and the very last destructed.
 *
 * So, if we want to add a new private member in cbEditor, we add it here instead
 * and access it with m_pData->
 * e.g. m_pData->lastPosForCodeCompletion
 * and of course you can add member functions here ;)
 *
 * cbEditorInternalData also contains a pointer to the owner cbEditor (named m_pOwner).
 * Using m_pOwner the struct's member functions can access cbEditor functions
 * (even private ones - it's a friend).
 *
 * The same logic should be used all around the project's classes, gradually.
 */
struct cbEditorInternalData
{
    cbEditorInternalData(cbEditor* owner)
        : m_pOwner(owner),
        lastPosForCodeCompletion(0),
        m_strip_trailing_spaces(true),
        m_ensure_final_line_end(false),
        m_ensure_consistent_line_ends(true),
        m_LastMarginMenuLine(-1),
        m_LastDebugLine(-1)
    {}
    cbEditor* m_pOwner;

    // add member vars/funcs below
    wxString GetEOLString() const
    {
        wxString eolstring;
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        switch (control->GetEOLMode())
        {
            case wxSCI_EOL_LF:
                eolstring = _T("\n");
                break;
            case wxSCI_EOL_CR:
                eolstring = _T("\r");
                break;
            default:
                eolstring = _T("\r\n");
        }
        return eolstring;
    }
    // funcs
    /** Get the last non-whitespace character before position */
    wxChar GetLastNonWhitespaceChar(int position = -1)
    {
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        if (position == -1)
            position = control->GetCurrentPos();

        int count = 0; // Used to count the number of blank lines
        bool foundlf = false; // For the rare case of CR's without LF's
        while (position)
        {
            wxChar c = control->GetCharAt(--position);
            int style = control->GetStyleAt(position);
            bool inComment = style == wxSCI_C_COMMENT ||
                            style == wxSCI_C_COMMENTDOC ||
                            style == wxSCI_C_COMMENTDOCKEYWORD ||
                            style == wxSCI_C_COMMENTDOCKEYWORDERROR ||
                            style == wxSCI_C_COMMENTLINE ||
                            style == wxSCI_C_COMMENTLINEDOC;
            if (c == _T('\n'))
            {
            	count++;
            	foundlf = true;
            }
            else if (c == _T('\r') && !foundlf)
            	count++;
            else
                foundlf = false;
            if (count > 1) return 0; // Don't over-indent
            if (!inComment && c != _T(' ') && c != _T('\t') && c != _T('\n') && c != _T('\r'))
                return c;
        }
        return 0;
    }

    int FindBlockStart(int position, wxChar blockStart, wxChar blockEnd, bool skipNested = true)
    {
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        int lvl = 0;
        wxChar b = control->GetCharAt(position);
        while (b)
        {
            if (b == blockEnd)
                ++lvl;
            else if (b == blockStart)
            {
                if (lvl == 0)
                    return position;
                --lvl;
            }
            --position;
            b = control->GetCharAt(position);
        }
        return -1;
    }

    /** Strip Trailing Blanks before saving */
    void StripTrailingSpaces()
    {
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        // The following code was adapted from the SciTE sourcecode

        int maxLines = control->GetLineCount();
        for (int line = 0; line < maxLines; line++) {
            int lineStart = control->PositionFromLine(line);
            int lineEnd = control->GetLineEndPosition(line);
            int i = lineEnd-1;
            wxChar ch = (wxChar)(control->GetCharAt(i));
            while ((i >= lineStart) && ((ch == _T(' ')) || (ch == _T('\t'))))
            {
                i--;
                ch = (wxChar)(control->GetCharAt(i));
            }
            if (i < (lineEnd-1)) {
                control->SetTargetStart(i+1);
                control->SetTargetEnd(lineEnd);
                control->ReplaceTarget(_T(""));
            }
        }
    }

    /** Add extra blank line to the file */
    void EnsureFinalLineEnd()
    {
    	cbStyledTextCtrl* control = m_pOwner->GetControl();
        // The following code was adapted from the SciTE sourcecode
        int maxLines = control->GetLineCount();
        int enddoc = control->PositionFromLine(maxLines);
        if(maxLines <= 1 || enddoc > control->PositionFromLine(maxLines-1))
            control->InsertText(enddoc,GetEOLString());
    }

    /** Make sure all the lines end with the same EOL mode */
    void EnsureConsistentLineEnds()
    {
    	cbStyledTextCtrl* control = m_pOwner->GetControl();
        // The following code was adapted from the SciTE sourcecode
    	control->ConvertEOLs(control->GetEOLMode());
    }

    //vars
    int lastPosForCodeCompletion;
    bool m_strip_trailing_spaces;
    bool m_ensure_final_line_end;
    bool m_ensure_consistent_line_ends;

    int m_LastMarginMenuLine;
    int m_LastDebugLine;

};
////////////////////////////////////////////////////////////////////////////////

const int idEmptyMenu = wxNewId();
const int idEdit = wxNewId();
const int idUndo = wxNewId();
const int idRedo = wxNewId();
const int idCut = wxNewId();
const int idCopy = wxNewId();
const int idPaste = wxNewId();
const int idDelete = wxNewId();
const int idSelectAll = wxNewId();
const int idSwapHeaderSource = wxNewId();
const int idBookmarks = wxNewId();
const int idBookmarksToggle = wxNewId();
const int idBookmarksPrevious = wxNewId();
const int idBookmarksNext = wxNewId();
const int idFolding = wxNewId();
const int idFoldingFoldAll = wxNewId();
const int idFoldingUnfoldAll = wxNewId();
const int idFoldingToggleAll = wxNewId();
const int idFoldingFoldCurrent = wxNewId();
const int idFoldingUnfoldCurrent = wxNewId();
const int idFoldingToggleCurrent = wxNewId();
const int idInsert = wxNewId();
const int idConfigureEditor = wxNewId();
const int idProperties = wxNewId();

const int idBookmarkAdd = wxNewId();
const int idBookmarkRemove = wxNewId();

const int idBreakpointAdd = wxNewId();
const int idBreakpointEdit = wxNewId();
const int idBreakpointRemove = wxNewId();

BEGIN_EVENT_TABLE(cbEditor, EditorBase)
    EVT_CLOSE(cbEditor::OnClose)
    EVT_TIMER(-1, cbEditor::OnTimer)
    // we got dynamic events; look in CreateEditor()

	EVT_MENU(idUndo, cbEditor::OnContextMenuEntry)
	EVT_MENU(idRedo, cbEditor::OnContextMenuEntry)
	EVT_MENU(idCut, cbEditor::OnContextMenuEntry)
	EVT_MENU(idCopy, cbEditor::OnContextMenuEntry)
	EVT_MENU(idPaste, cbEditor::OnContextMenuEntry)
	EVT_MENU(idDelete, cbEditor::OnContextMenuEntry)
	EVT_MENU(idSelectAll, cbEditor::OnContextMenuEntry)
	EVT_MENU(idSwapHeaderSource, cbEditor::OnContextMenuEntry)
	EVT_MENU(idBookmarksToggle, cbEditor::OnContextMenuEntry)
	EVT_MENU(idBookmarksPrevious, cbEditor::OnContextMenuEntry)
	EVT_MENU(idBookmarksNext, cbEditor::OnContextMenuEntry)
	EVT_MENU(idFoldingFoldAll, cbEditor::OnContextMenuEntry)
	EVT_MENU(idFoldingUnfoldAll, cbEditor::OnContextMenuEntry)
	EVT_MENU(idFoldingToggleAll, cbEditor::OnContextMenuEntry)
	EVT_MENU(idFoldingFoldCurrent, cbEditor::OnContextMenuEntry)
	EVT_MENU(idFoldingUnfoldCurrent, cbEditor::OnContextMenuEntry)
	EVT_MENU(idFoldingToggleCurrent, cbEditor::OnContextMenuEntry)
	EVT_MENU(idConfigureEditor, cbEditor::OnContextMenuEntry)
	EVT_MENU(idProperties, cbEditor::OnContextMenuEntry)
	EVT_MENU(idBookmarkAdd, cbEditor::OnContextMenuEntry)
	EVT_MENU(idBookmarkRemove, cbEditor::OnContextMenuEntry)
	EVT_MENU(idBreakpointAdd, cbEditor::OnContextMenuEntry)
	EVT_MENU(idBreakpointEdit, cbEditor::OnContextMenuEntry)
	EVT_MENU(idBreakpointRemove, cbEditor::OnContextMenuEntry)
END_EVENT_TABLE()

// class constructor
cbEditor::cbEditor(wxWindow* parent, const wxString& filename, EditorColorSet* theme)
    : EditorBase(parent, filename),
	m_pControl(0L),
	m_Modified(false),
	m_Index(-1),
	m_pProjectFile(0L),
	m_pTheme(theme),
	m_ActiveCalltipsNest(0)
{
    // first thing to do!
    // if we add more constructors in the future, don't forget to set this!
    m_pData = new cbEditorInternalData(this);
    m_IsBuiltinEditor = true;

    m_timerWait.SetOwner(this);

    InitFilename(filename);
//    Manager::Get()->GetMessageManager()->DebugLog(_("ctor: Filename=%s\nShort=%s"), m_Filename.c_str(), m_Shortname.c_str());

	CreateEditor();
	SetEditorStyle();
	m_IsOK = Open();

    // if !m_IsOK then it's a new file, so set the modified flag ON
    if (!m_IsOK && filename.IsEmpty())
    {
        SetModified(true);
    }
}

// class destructor
cbEditor::~cbEditor()
{
	NotifyPlugins(cbEVT_EDITOR_CLOSE, 0, m_Filename);
	UpdateProjectFile();
	if (m_pControl)
	{
		if (m_pProjectFile)
			m_pProjectFile->editorOpen = false;
        m_pControl->Destroy();
        m_pControl = 0;
	}
	delete m_pData;
}

void cbEditor::NotifyPlugins(wxEventType type, int intArg, const wxString& strArg, int xArg, int yArg)
{
    if (!Manager::Get()->GetPluginManager())
        return; // no plugin manager! app shuting down?
	CodeBlocksEvent event(type);
	event.SetEditor(this);
	event.SetInt(intArg);
	event.SetString(strArg);
	event.SetX(xArg);
	event.SetY(yArg);
	//wxPostEvent(Manager::Get()->GetAppWindow(), event);
	Manager::Get()->GetPluginManager()->NotifyPlugins(event);
}

bool cbEditor::GetModified()
{
    return m_Modified || m_pControl->GetModify();
}

void cbEditor::SetModified(bool modified)
{
    if (modified != m_Modified)
    {
        m_Modified = modified;
        if (m_Modified)
            SetEditorTitle(EDITOR_MODIFIED + m_Shortname);
        else
            SetEditorTitle(m_Shortname);
        Manager::Get()->GetEditorManager()->RefreshOpenedFilesTree();
    }
    // visual state
    if (m_pProjectFile)
        m_pProjectFile->SetFileState(m_pControl->GetReadOnly() ? fvsReadOnly : (m_Modified ? fvsModified : fvsNormal));
}

void cbEditor::SetEditorTitle(const wxString& title)
{
    SetTitle(title);
}

void cbEditor::SetProjectFile(ProjectFile* project_file, bool preserve_modified)
{
	if (m_pProjectFile == project_file)
		return; // we 've been here before ;)

	bool wasmodified = false;
	if(preserve_modified)
        wasmodified = GetModified();

	m_pProjectFile = project_file;
	if (m_pProjectFile)
	{
        // update our filename
        m_Filename = UnixFilename(project_file->file.GetFullPath());

		m_pControl->GotoPos(m_pProjectFile->editorPos);
		m_pControl->ScrollToLine(m_pProjectFile->editorTopLine);
		m_pControl->ScrollToColumn(0);

		m_pProjectFile->editorOpen = true;

		if (Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/tab_text_relative"), true))
            m_Shortname = m_pProjectFile->relativeToCommonTopLevelPath;
        else
            m_Shortname = m_pProjectFile->file.GetFullName();
		SetEditorTitle(m_Shortname);
	}

    if (!wxFileExists(m_Filename))
        m_pProjectFile->SetFileState(fvsMissing);
    else if (!wxFile::Access(m_Filename.c_str(), wxFile::write)) // readonly
        m_pProjectFile->SetFileState(fvsReadOnly);

#if 0
	wxString dbg;
	dbg << _T("[ed] Filename: ") << GetFilename() << _T('\n');
	dbg << _T("[ed] Short name: ") << GetShortName() << _T('\n');
	dbg << _T("[ed] Modified: ") << GetModified() << _T('\n');
	dbg << _T("[ed] Project: ") << ((m_pProjectFile && m_pProjectFile->project) ? m_pProjectFile->project->GetTitle() : _T("unknown")) << _T('\n');
	dbg << _T("[ed] Project file: ") << (m_pProjectFile ? m_pProjectFile->relativeFilename : _T("unknown")) << _T('\n');
	Manager::Get()->GetMessageManager()->DebugLog(dbg);
#endif
    if(preserve_modified)
        SetModified(wasmodified);
}

void cbEditor::UpdateProjectFile()
{
	if (m_pControl && m_pProjectFile)
	{
		m_pProjectFile->editorPos = m_pControl->GetCurrentPos();
		m_pProjectFile->editorTopLine = m_pControl->GetFirstVisibleLine();
		m_pProjectFile->editorOpen = true;
	}
}

void cbEditor::CreateEditor()
{
	m_ID = wxNewId();

    wxBoxSizer* bs = new wxBoxSizer(wxVERTICAL);
    m_pControl = new cbStyledTextCtrl(this, m_ID, wxPoint(0, 0), GetSize());
	m_pControl->UsePopUp(false);
    bs->Add(m_pControl, 1, wxEXPAND);
    SetSizer(bs);
    SetAutoLayout(true);

    // dynamic events
    Connect( m_ID,  -1, wxEVT_SCI_MARGINCLICK,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &cbEditor::OnMarginClick );
    Connect( m_ID,  -1, wxEVT_SCI_UPDATEUI,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &cbEditor::OnEditorUpdateUI );
    Connect( m_ID,  -1, wxEVT_SCI_CHANGE,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &cbEditor::OnEditorChange );
    Connect( m_ID,  -1, wxEVT_SCI_CHARADDED,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &cbEditor::OnEditorCharAdded );
    Connect( m_ID,  -1, wxEVT_SCI_DWELLSTART,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &cbEditor::OnEditorDwellStart );
    Connect( m_ID,  -1, wxEVT_SCI_DWELLEND,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &cbEditor::OnEditorDwellEnd );
    Connect( m_ID,  -1, wxEVT_SCI_USERLISTSELECTION,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &cbEditor::OnUserListSelection );
}

wxColour cbEditor::GetOptionColour(const wxString& option, const wxColour _default)
{
    return Manager::Get()->GetConfigManager(_T("editor"))->ReadColour(option, _default);
}

void cbEditor::SetEditorStyle()
{
    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("editor"));

    wxFont font(8, wxMODERN, wxNORMAL, wxNORMAL);
    wxString fontstring = mgr->Read(_T("/font"), wxEmptyString);
    int eolmode = wxSCI_EOL_CRLF;
    if (!fontstring.IsEmpty())
    {
        wxNativeFontInfo nfi;
        nfi.FromString(fontstring);
        font.SetNativeFontInfo(nfi);
    }

    // update the tab text based on preferences
    if (m_pProjectFile)
    {
		if (mgr->ReadBool(_T("/tab_text_relative"), true))
            m_Shortname = m_pProjectFile->relativeToCommonTopLevelPath;
        else
            m_Shortname = m_pProjectFile->file.GetFullName();
		SetEditorTitle(m_Shortname);
    }

	m_pControl->SetMouseDwellTime(1000);

	m_pControl->SetCaretLineVisible(mgr->ReadBool(_T("/highlight_caret_line"), true));
	m_pControl->SetCaretLineBackground(GetOptionColour(_T("/highlight_caret_line_color"), wxColour(0xFF, 0xFF, 0x00)));

    m_pControl->SetUseTabs(mgr->ReadBool(_T("/use_tab"), false));
	m_pControl->SetIndentationGuides(mgr->ReadBool(_T("/show_indent_guides"), false));
    m_pControl->SetTabIndents(mgr->ReadBool(_T("/tab_indents"), true));
    m_pControl->SetBackSpaceUnIndents(mgr->ReadBool(_T("/backspace_unindents"), true));
    m_pControl->SetWrapMode(mgr->ReadBool(_T("/word_wrap"), false));
    m_pControl->SetViewEOL(mgr->ReadBool(_T("/show_eol"), false));
    m_pControl->SetViewWhiteSpace(mgr->ReadBool(_T("/view_whitespace"), false));
	//gutter
    m_pControl->SetEdgeMode(mgr->ReadInt(_T("/gutter/mode"), 1));
    m_pControl->SetEdgeColour(GetOptionColour(_T("/gutter/color"), *wxLIGHT_GREY));
    m_pControl->SetEdgeColumn(mgr->ReadInt(_T("/gutter/column"), 80));

    m_pControl->StyleSetFont(wxSCI_STYLE_DEFAULT, font);
    m_pControl->StyleClearAll();

    m_pControl->SetTabWidth(mgr->ReadInt(_T("/tab_size"), 4));

    // line numbering
   	if (mgr->ReadBool(_T("/show_line_numbers"), false))
    {
	    m_pControl->SetMarginType(0, wxSCI_MARGIN_NUMBER);
    	m_pControl->SetMarginWidth(0, 48);
    }
	else
		m_pControl->SetMarginWidth(0, 0);

    // margin for bookmarks, breakpoints etc.
	// FIXME: how to display a mark with an offset???
	m_pControl->SetMarginWidth(1, 16);
    m_pControl->SetMarginType(1, wxSCI_MARGIN_SYMBOL);
    m_pControl->SetMarginSensitive(1, 1);
    m_pControl->SetMarginMask(1, (1 << BOOKMARK_MARKER) |
                                 (1 << BREAKPOINT_MARKER) |
                                 (1 << DEBUG_MARKER));
	m_pControl->MarkerDefine(BOOKMARK_MARKER, BOOKMARK_STYLE);
	m_pControl->MarkerSetBackground(BOOKMARK_MARKER, wxColour(0xA0, 0xA0, 0xFF));
	m_pControl->MarkerDefine(BREAKPOINT_MARKER, BREAKPOINT_STYLE);
	m_pControl->MarkerSetBackground(BREAKPOINT_MARKER, wxColour(0xFF, 0x00, 0x00));
	m_pControl->MarkerDefine(DEBUG_MARKER, DEBUG_STYLE);
	m_pControl->MarkerSetBackground(DEBUG_MARKER, wxColour(0xFF, 0xFF, 0x00));

    // EOL properties
    m_pData->m_strip_trailing_spaces = mgr->ReadBool(_T("/eol/strip_trailing_spaces"), true);
    m_pData->m_ensure_final_line_end = mgr->ReadBool(_T("/eol/ensure_final_line_end"), false);
    m_pData->m_ensure_consistent_line_ends = mgr->ReadBool(_T("/eol/ensure_consistent_line_ends"), false);

#ifdef __WXMSW__
	#define DEFAULT_EOL 0 //CR&LF
#else
	#define DEFAULT_EOL 2 //LF
	// TODO: find out the macro to discover if it is a MAC and change it to CR (I think)
#endif
    switch (mgr->ReadInt(_T("/eol/eolmode"), (int)DEFAULT_EOL))
    {
    	case 0:
        default:
            eolmode = wxSCI_EOL_CRLF;
        break;
        case 1:
            eolmode = wxSCI_EOL_CR;
        break;
        case 2:
            eolmode = wxSCI_EOL_LF;
    }
    m_pControl->SetEOLMode(eolmode);

    // folding margin
    m_pControl->SetProperty(_T("fold"), mgr->ReadBool(_T("/folding/show_folds"), true) ? _T("1") : _T("0"));
    m_pControl->SetProperty(_T("fold.html"), mgr->ReadBool(_T("/folding/fold_xml"), true) ? _T("1") : _T("0"));
    m_pControl->SetProperty(_T("fold.comment"), mgr->ReadBool(_T("/folding/fold_comments"), false) ? _T("1") : _T("0"));
    m_pControl->SetProperty(_T("fold.compact"), _T("0"));
    m_pControl->SetProperty(_T("fold.preprocessor"), mgr->ReadBool(_T("/folding/fold_preprocessor"), false) ? _T("1") : _T("0"));
   	if (mgr->ReadBool(_T("/folding/show_folds"), true))
    {
        m_pControl->SetFoldFlags(16);
        m_pControl->SetMarginType(2, wxSCI_MARGIN_SYMBOL);
        m_pControl->SetMarginWidth(2, 16);
        m_pControl->SetMarginMask(2, wxSCI_MASK_FOLDERS);
        m_pControl->SetMarginSensitive(2, 1);

        m_pControl->MarkerDefine(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_BOXMINUS);
        m_pControl->MarkerSetForeground(wxSCI_MARKNUM_FOLDEROPEN, wxColour(0xff, 0xff, 0xff));
        m_pControl->MarkerSetBackground(wxSCI_MARKNUM_FOLDEROPEN, wxColour(0x80, 0x80, 0x80));
        m_pControl->MarkerDefine(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_BOXPLUS);
        m_pControl->MarkerSetForeground(wxSCI_MARKNUM_FOLDER, wxColour(0xff, 0xff, 0xff));
        m_pControl->MarkerSetBackground(wxSCI_MARKNUM_FOLDER, wxColour(0x80, 0x80, 0x80));
        m_pControl->MarkerDefine(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_VLINE);
        m_pControl->MarkerSetForeground(wxSCI_MARKNUM_FOLDERSUB, wxColour(0xff, 0xff, 0xff));
        m_pControl->MarkerSetBackground(wxSCI_MARKNUM_FOLDERSUB, wxColour(0x80, 0x80, 0x80));
        m_pControl->MarkerDefine(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_LCORNER);
        m_pControl->MarkerSetForeground(wxSCI_MARKNUM_FOLDERTAIL, wxColour(0xff, 0xff, 0xff));
        m_pControl->MarkerSetBackground(wxSCI_MARKNUM_FOLDERTAIL, wxColour(0x80, 0x80, 0x80));
        m_pControl->MarkerDefine(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_BOXPLUSCONNECTED);
        m_pControl->MarkerSetForeground(wxSCI_MARKNUM_FOLDEREND, wxColour(0xff, 0xff, 0xff));
        m_pControl->MarkerSetBackground(wxSCI_MARKNUM_FOLDEREND, wxColour(0x80, 0x80, 0x80));
        m_pControl->MarkerDefine(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_BOXMINUSCONNECTED);
        m_pControl->MarkerSetForeground(wxSCI_MARKNUM_FOLDEROPENMID, wxColour(0xff, 0xff, 0xff));
        m_pControl->MarkerSetBackground(wxSCI_MARKNUM_FOLDEROPENMID, wxColour(0x80, 0x80, 0x80));
        m_pControl->MarkerDefine(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_TCORNER);
        m_pControl->MarkerSetForeground(wxSCI_MARKNUM_FOLDERMIDTAIL, wxColour(0xff, 0xff, 0xff));
        m_pControl->MarkerSetBackground(wxSCI_MARKNUM_FOLDERMIDTAIL, wxColour(0x80, 0x80, 0x80));
	}
	else
		m_pControl->SetMarginWidth(2, 0);

	if (m_pTheme)
		m_pTheme->Apply(this);
}

void cbEditor::SetColorSet(EditorColorSet* theme)
{
	m_pTheme = theme;
	if (m_pTheme)
		m_pTheme->Apply(this);
}

bool cbEditor::Reload()
{
    // keep current pos
    int pos = m_pControl ? m_pControl->GetCurrentPos() : 0;

    // call open
    if (!Open())
        return false;

    // return (if possible) to old pos
    if (m_pControl)
        m_pControl->GotoPos(pos);

    return true;
}

void cbEditor::Touch()
{
	m_LastModified = wxDateTime::Now();
}

bool cbEditor::Open()
{
    if (m_pProjectFile)
    {
        if (!wxFileExists(m_Filename))
            m_pProjectFile->SetFileState(fvsMissing);
        else if (!wxFile::Access(m_Filename.c_str(), wxFile::write)) // readonly
            m_pProjectFile->SetFileState(fvsReadOnly);
    }

    if (!wxFileExists(m_Filename))
        return false;

    // open file
    m_pControl->SetReadOnly(false);
    wxString st;

    m_pControl->ClearAll();
    wxFile file(m_Filename);

    if (!file.IsOpened())
        return false;

    m_pControl->InsertText(0, cbReadFileContents(file));
    m_pControl->EmptyUndoBuffer();

    // mark the file read-only, if applicable
    bool read_only = !wxFile::Access(m_Filename.c_str(), wxFile::write);
    m_pControl->SetReadOnly(read_only);
    // if editor is read-only, override bg color for *all* styles...
    if(m_pTheme)
        m_pTheme->Apply(this); //Apply default theme 1st
    if (read_only)
    {
        for (int i = 0; i < wxSCI_STYLE_MAX; ++i)
            m_pControl->StyleSetBackground(i, wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    }

	if (Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/folding/fold_all_on_open"), false))
		FoldAll();

    wxFileName fname(m_Filename);
    m_LastModified = fname.GetModificationTime();

    SetModified(false);
	NotifyPlugins(cbEVT_EDITOR_OPEN);
    return true;
}

bool cbEditor::Save()
{
	if (!GetModified())
		return true;

    // one undo action for all modifications in this context
    // (angled braces added for clarity)
    m_pControl->BeginUndoAction();
    {
        if(m_pData->m_strip_trailing_spaces)
            m_pData->StripTrailingSpaces();
        if(m_pData->m_ensure_consistent_line_ends)
            m_pData->EnsureConsistentLineEnds();
        if(m_pData->m_ensure_final_line_end)
            m_pData->EnsureFinalLineEnd();
    }
    m_pControl->EndUndoAction();

    if (!m_IsOK)
    {
        return SaveAs();
    }

    if(!cbSaveToFile(m_Filename, m_pControl->GetText()))
        return false; // failed; file is read-only?

    wxFileName fname(m_Filename);
    m_LastModified = fname.GetModificationTime();

    m_IsOK = true;

    m_pControl->SetSavePoint();
    SetModified(false);

	NotifyPlugins(cbEVT_EDITOR_SAVE);
    return true;
}

bool cbEditor::SaveAs()
{
    wxFileName fname;
    fname.Assign(m_Filename);
    wxFileDialog* dlg = new wxFileDialog(this,
                            _("Save file"),
                            fname.GetPath(),
                            fname.GetFullName(),
                            SOURCE_FILES_DIALOG_FILTER,
                            wxSAVE | wxOVERWRITE_PROMPT);
    dlg->SetFilterIndex(7);// all files;

    if (dlg->ShowModal() != wxID_OK)
        return false;
    m_Filename = dlg->GetPath();
    LOGSTREAM << m_Filename << _T('\n');
    fname.Assign(m_Filename);
    m_Shortname = fname.GetFullName();
	SetEditorTitle(m_Shortname);
    //Manager::Get()->GetMessageManager()->Log(mltDevDebug, "Filename=%s\nShort=%s", m_Filename.c_str(), m_Shortname.c_str());
    m_IsOK = true;
    SetModified(true);
	if(m_pTheme)
        m_pTheme->Apply(this);
    return Save();
}

bool cbEditor::RenameTo(const wxString& filename, bool deleteOldFromDisk)
{
    wxLogWarning(_("Not implemented..."));
	//NotifyPlugins(cbEVT_EDITOR_RENAME);
	return false;
}

void cbEditor::DoAskForCodeCompletion()
{
    m_timerWait.Stop();
	NotifyPlugins(cbEVT_EDITOR_AUTOCOMPLETE);
}

void cbEditor::AutoComplete()
{
    MessageManager* msgMan = Manager::Get()->GetMessageManager();
    AutoCompleteMap& map = Manager::Get()->GetEditorManager()->GetAutoCompleteMap();
    int curPos = m_pControl->GetCurrentPos();
    int wordStartPos = m_pControl->WordStartPosition(curPos, true);
    wxString keyword = m_pControl->GetTextRange(wordStartPos, curPos);
    wxString lineIndent = GetLineIndentString(m_pControl->GetCurrentLine());
    msgMan->DebugLog(_("Auto-complete keyword: %s"), keyword.c_str());

    AutoCompleteMap::iterator it;
    for (it = map.begin(); it != map.end(); ++it)
    {
        if (keyword == it->first)
        {
            // found; auto-complete it
            msgMan->DebugLog(_("Match found"));
            m_pControl->BeginUndoAction();

            // indent code accordingly
            wxString code = it->second;
            code.Replace(_T("\n"), _T('\n') + lineIndent);

            // look for and replace macros
            int macroPos = code.Find(_T("$("));
            while (macroPos != -1)
            {
                // locate ending parenthesis
                int macroPosEnd = macroPos + 2;
                int len = (int)code.Length();
                while (macroPosEnd < len && code.GetChar(macroPosEnd) != _T(')'))
                    ++macroPosEnd;
                if (macroPosEnd == len)
                    break; // no ending parenthesis

                wxString macroName = code.SubString(macroPos + 2, macroPosEnd - 1);
                msgMan->DebugLog(_("Found macro: %s"), macroName.c_str());
                wxString macro = wxGetTextFromUser(_("Please enter the text for \"") + macroName + _T("\":"), _("Macro substitution"));
                code.Replace(_T("$(") + macroName + _T(")"), macro);
                macroPos = code.Find(_T("$("));
            }

            // delete keyword
            m_pControl->SetSelection(wordStartPos, curPos);
            m_pControl->ReplaceSelection(_T(""));
            curPos = wordStartPos;

            // add the text
            m_pControl->InsertText(curPos, code);

            // put cursor where "|" appears in code (if it appears)
            int caretPos = code.Find(_T('|'));
            if (caretPos != -1)
            {
                m_pControl->SetCurrentPos(curPos + caretPos);
                m_pControl->SetSelection(curPos + caretPos, curPos + caretPos + 1);
                m_pControl->ReplaceSelection(_T(""));
            }

            m_pControl->EndUndoAction();
            break;
        }
    }
}

void cbEditor::DoFoldAll(int fold)
{
    m_pControl->Colourise(0, -1); // the *most* important part!
    int count = m_pControl->GetLineCount();
    for (int i = 0; i < count; ++i)
		DoFoldLine(i, fold);
}

void cbEditor::DoFoldBlockFromLine(int line, int fold)
{
    m_pControl->Colourise(0, -1); // the *most* important part!
	int i = line;
    while (i != 0)
    {
		if (DoFoldLine(i, fold))
			return;
		--i;
    }
}

bool cbEditor::DoFoldLine(int line, int fold)
{
	int level = m_pControl->GetFoldLevel(line);
	if ((level & wxSCI_FOLDLEVELHEADERFLAG) &&
		(wxSCI_FOLDLEVELBASE == (level & wxSCI_FOLDLEVELNUMBERMASK)))
	{
		bool expand = false;
		int maxLine = m_pControl->GetLastChild(line, -1);
		if (fold == 2) // toggle
			expand = !m_pControl->GetFoldExpanded(line);
		else
			expand = fold == 0;
		m_pControl->SetFoldExpanded(line, expand);
		if (maxLine > line)
		{
			if (expand)
				m_pControl->ShowLines(line + 1, maxLine);
			else
				m_pControl->HideLines(line + 1, maxLine);
		}
		return true;
	}
	return false;
}

void cbEditor::FoldAll()
{
	DoFoldAll(1);
}

void cbEditor::UnfoldAll()
{
	DoFoldAll(0);
}

void cbEditor::ToggleAllFolds()
{
	DoFoldAll(2);
}

void cbEditor::FoldBlockFromLine(int line)
{
	if (line == -1)
		line = m_pControl->GetCurrentLine();
	DoFoldBlockFromLine(line, 1);
}

void cbEditor::UnfoldBlockFromLine(int line)
{
	if (line == -1)
		line = m_pControl->GetCurrentLine();
	DoFoldBlockFromLine(line, 0);
}

void cbEditor::ToggleFoldBlockFromLine(int line)
{
	if (line == -1)
		line = m_pControl->GetCurrentLine();
	DoFoldBlockFromLine(line, 2);
}

void cbEditor::GotoLine(int line, bool centerOnScreen)
{
    if (centerOnScreen)
    {
        int onScreen = m_pControl->LinesOnScreen() >> 1;
        m_pControl->GotoLine(line - onScreen);
        m_pControl->GotoLine(line + onScreen);
    }
    m_pControl->GotoLine(line);
}

bool cbEditor::AddBreakpoint(int line, bool notifyDebugger)
{
    if (HasBreakpoint(line))
        return false;
	if (line == -1)
		line = m_pControl->GetCurrentLine();
    MarkerToggle(BREAKPOINT_MARKER, line);
    if (notifyDebugger)
        NotifyPlugins(cbEVT_EDITOR_BREAKPOINT_ADD, line, m_Filename);
    return true;
}

bool cbEditor::RemoveBreakpoint(int line, bool notifyDebugger)
{
    if (!HasBreakpoint(line))
        return false;
	if (line == -1)
		line = m_pControl->GetCurrentLine();
    MarkerToggle(BREAKPOINT_MARKER, line);
    if (notifyDebugger)
        NotifyPlugins(cbEVT_EDITOR_BREAKPOINT_DELETE, line, m_Filename);
    return true;
}

void cbEditor::ToggleBreakpoint(int line, bool notifyDebugger)
{
    if (HasBreakpoint(line))
        RemoveBreakpoint(line, notifyDebugger);
    else
        AddBreakpoint(line, notifyDebugger);
}

bool cbEditor::HasBreakpoint(int line)
{
	if (line == -1)
		line = m_pControl->GetCurrentLine();
    return LineHasMarker(BREAKPOINT_MARKER, line);
}

void cbEditor::GotoNextBreakpoint()
{
    MarkerNext(BREAKPOINT_MARKER);
}

void cbEditor::GotoPreviousBreakpoint()
{
    MarkerPrevious(BREAKPOINT_MARKER);
}

void cbEditor::ToggleBookmark(int line)
{
    MarkerToggle(BOOKMARK_MARKER, line);
}

bool cbEditor::HasBookmark(int line)
{
    return LineHasMarker(BOOKMARK_MARKER, line);
}

void cbEditor::GotoNextBookmark()
{
    MarkerNext(BOOKMARK_MARKER);
}

void cbEditor::GotoPreviousBookmark()
{
    MarkerPrevious(BOOKMARK_MARKER);
}

void cbEditor::SetDebugLine(int line)
{
    MarkLine(DEBUG_MARKER, line);
    m_pData->m_LastDebugLine = line;
}

void cbEditor::SetErrorLine(int line)
{
    MarkLine(ERROR_LINE, line);
}

bool cbEditor::LineHasMarker(int marker, int line)
{
	if (line == -1)
		line = m_pControl->GetCurrentLine();
	return m_pControl->MarkerGet(line) & (1 << marker);
}

void cbEditor::MarkerToggle(int marker, int line)
{
	if (line == -1)
		line = m_pControl->GetCurrentLine();
    if (LineHasMarker(marker, line))
		m_pControl->MarkerDelete(line, marker);
	else
		m_pControl->MarkerAdd(line, marker);
}

void cbEditor::MarkerNext(int marker)
{
	int line = m_pControl->GetCurrentLine() + 1;
	int newLine = m_pControl->MarkerNext(line, 1 << marker);
	if (newLine != -1)
		GotoLine(newLine);
}

void cbEditor::MarkerPrevious(int marker)
{
	int line = m_pControl->GetCurrentLine() - 1;
	int newLine = m_pControl->MarkerPrevious(line, 1 << marker);
	if (newLine != -1)
		GotoLine(newLine);
}

void cbEditor::MarkLine(int marker, int line)
{
	m_pControl->MarkerDeleteAll(marker);
	if (line != -1)
	{
		m_pControl->MarkerAdd(line, marker);
		m_pControl->SetCaretLineVisible(false);
	}
	else
		m_pControl->SetCaretLineVisible(Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/highlight_caret_line"), true));
}

void cbEditor::GotoMatchingBrace()
{
    // this works only when the caret is *before* the brace
    int matchingBrace = m_pControl->BraceMatch(m_pControl->GetCurrentPos());

    // if we haven't found it, we 'll search at pos-1 too
    if(matchingBrace == wxSCI_INVALID_POSITION)
        matchingBrace = m_pControl->BraceMatch(m_pControl->GetCurrentPos() - 1);

    // now, we either found it or not
    if(matchingBrace != wxSCI_INVALID_POSITION)
        m_pControl->GotoPos(matchingBrace);
}

void cbEditor::HighlightBraces()
{
    ////// BRACES HIGHLIGHTING ///////
    int currPos = m_pControl->GetCurrentPos();
    int newPos = m_pControl->BraceMatch(currPos);
    if (newPos == wxSCI_INVALID_POSITION)
    {
        if(currPos > 0) currPos--;
        newPos = m_pControl->BraceMatch(currPos);
    }
    if (newPos != wxSCI_INVALID_POSITION)
        m_pControl->BraceHighlight(currPos, newPos);
    else
    {
        wxChar ch = m_pControl->GetCharAt(currPos);
        if (ch == _T('{') || ch == _T('[') || ch == _T('(') || ch == _T('<') ||
            ch == _T('}') || ch == _T(']') || ch == _T(')') || ch == _T('>'))
            m_pControl->BraceBadLight(currPos);
        else
            m_pControl->BraceHighlight(-1, -1);
    }
    m_pControl->Refresh(FALSE);
}

int cbEditor::GetLineIndentInSpaces(int line)
{
    int currLine = (line == -1)
                    ? m_pControl->LineFromPosition(m_pControl->GetCurrentPos())
                    : line;
    wxString text = m_pControl->GetLine(currLine);
    unsigned int len = text.Length();
    int spaceCount = 0;
    for (unsigned int i = 0; i < len; ++i)
    {
        if (text[i] == _T(' '))
            ++spaceCount;
        else if (text[i] == _T('\t'))
            spaceCount += m_pControl->GetTabWidth();
        else
            break;
    }
    return spaceCount;
}

wxString cbEditor::GetLineIndentString(int line)
{
    int currLine = (line == -1)
                    ? m_pControl->LineFromPosition(m_pControl->GetCurrentPos())
                    : line;
    wxString text = m_pControl->GetLine(currLine);
    unsigned int len = text.Length();
    wxString indent;
    for (unsigned int i = 0; i < len; ++i)
    {
        if (text[i] == _T(' ') || text[i] == _T('\t'))
            indent << text[i];
        else
            break;
    }
    return indent;
}

// Creates a submenu for a Context Menu based on the submenu's specific Id
wxMenu* cbEditor::CreateContextSubMenu(long id)
{
    wxMenu* menu = 0;
    if(id == idInsert)
    {
        menu = new wxMenu;
        menu->Append(idEmptyMenu, _("Empty"));
        menu->Enable(idEmptyMenu, false);
    }
    else if(id == idEdit)
    {
        menu = new wxMenu;
        menu->Append(idUndo, _("Undo"));
        menu->Append(idRedo, _("Redo"));
        menu->AppendSeparator();
        menu->Append(idCut, _("Cut"));
        menu->Append(idCopy, _("Copy"));
        menu->Append(idPaste, _("Paste"));
        menu->Append(idDelete, _("Delete"));
        menu->AppendSeparator();
        menu->Append(idSelectAll, _("Select All"));

        bool hasSel = m_pControl->GetSelectionEnd() - m_pControl->GetSelectionStart() != 0;

        menu->Enable(idUndo, m_pControl->CanUndo());
        menu->Enable(idRedo, m_pControl->CanRedo());
        menu->Enable(idCut, hasSel);
        menu->Enable(idCopy, hasSel);
#ifdef __WXGTK__
        // a wxGTK bug causes the triggering of unexpected events
        menu->Enable(idPaste, true);
#else
        menu->Enable(idPaste, m_pControl->CanPaste());
#endif
        menu->Enable(idDelete, hasSel);
    }
    else if(id == idBookmarks)
    {
        menu = new wxMenu;
        menu->Append(idBookmarksToggle, _("Toggle bookmark"));
        menu->Append(idBookmarksPrevious, _("Previous bookmark"));
        menu->Append(idBookmarksNext, _("Next bookmark"));
    }
    else if(id == idFolding)
    {
        menu = new wxMenu;
        menu->Append(idFoldingFoldAll, _("Fold all"));
        menu->Append(idFoldingUnfoldAll, _("Unfold all"));
        menu->Append(idFoldingToggleAll, _("Toggle all folds"));
        menu->AppendSeparator();
        menu->Append(idFoldingFoldCurrent, _("Fold current block"));
        menu->Append(idFoldingUnfoldCurrent, _("Unfold current block"));
        menu->Append(idFoldingToggleCurrent, _("Toggle current block"));
    }
    else
        menu = EditorBase::CreateContextSubMenu(id);

    return menu;
}

// Adds menu items to context menu (both before and after loading plugins' items)
void cbEditor::AddToContextMenu(wxMenu* popup,bool noeditor,bool pluginsdone)
{
    if(!pluginsdone)
    {
        wxMenu *bookmarks = 0, *folding = 0, *editsubmenu = 0, *insert = 0;
        if(!noeditor)
        {
            insert = CreateContextSubMenu(idInsert);
            editsubmenu = CreateContextSubMenu(idEdit);
            bookmarks = CreateContextSubMenu(idBookmarks);
            folding = CreateContextSubMenu(idFolding);
        }
        if(insert)
        {
            popup->Append(idInsert, _("Insert..."), insert);
            popup->AppendSeparator();
        }
        popup->Append(idSwapHeaderSource, _("Swap header/source"));
        if(!noeditor)
            popup->AppendSeparator();

        if(editsubmenu)
            popup->Append(idEdit, _("Edit"), editsubmenu);
        if(bookmarks)
            popup->Append(idBookmarks, _("Bookmarks"), bookmarks);
        if(folding)
            popup->Append(idFolding, _("Folding"), folding);
    }
    else
    {
        if(!noeditor)
            popup->Append(idConfigureEditor, _("Configure editor"));
        popup->Append(idProperties, _("Properties"));

        // remove "Insert/Empty" if more than one entry
        wxMenu* insert = 0;
        wxMenuItem* insertitem = popup->FindItem(idInsert);
        if(insertitem)
            insert = insertitem->GetSubMenu();
        if(insert)
        {
            if (insert->GetMenuItemCount() > 1)
                insert->Delete(idEmptyMenu);
        }
    }
}

bool cbEditor::OnBeforeBuildContextMenu(const wxPoint& position, bool noeditor)
{
    if (!noeditor)
    {
        wxPoint clientpos = ScreenToClient(position);
        if (clientpos.x < (48 + 16)) // 48, 16 are the margins widths (line 498)
        {
            // margin right-click

            // keep the line
            int pos = m_pControl->PositionFromPoint(clientpos);
            m_pData->m_LastMarginMenuLine = m_pControl->LineFromPosition(pos);

            // create special menu
            wxMenu* popup = new wxMenu;

            if (LineHasMarker(BREAKPOINT_MARKER, m_pData->m_LastMarginMenuLine))
            {
                popup->Append(idBreakpointEdit, _("Edit breakpoint"));
                popup->Append(idBreakpointRemove, _("Remove breakpoint"));
            }
            else
                popup->Append(idBreakpointAdd, _("Add breakpoint"));

            popup->AppendSeparator();

            if (LineHasMarker(BOOKMARK_MARKER, m_pData->m_LastMarginMenuLine))
                popup->Append(idBookmarkRemove, _("Remove bookmark"));
            else
                popup->Append(idBookmarkAdd, _("Add bookmark"));

            // display menu
            PopupMenu(popup, clientpos.x, clientpos.y);

            delete popup;
            return false;
        }

        // before the context menu creation, move the caret to where mouse is

        // get caret position and line from mouse cursor
        int pos = m_pControl->PositionFromPoint(m_pControl->ScreenToClient(wxGetMousePosition()));
        if(m_pControl->GetSelectionStart() > pos || m_pControl->GetSelectionEnd() < pos) // this re-enables 1-click "Find declaration of..."
			m_pControl->GotoPos(pos);                                              // but avoids losing selection for cut/copy
    }

    // follow default strategy
    return EditorBase::OnBeforeBuildContextMenu(position, noeditor);
}

void cbEditor::OnAfterBuildContextMenu(bool noeditor)
{
    // we don't care
}

void cbEditor::Print(bool selectionOnly, PrintColorMode pcm)
{
    switch (pcm)
    {
        case pcmAsIs:
            m_pControl->SetPrintColourMode(wxSCI_PRINT_NORMAL);
            break;
        case pcmBlackAndWhite:
            m_pControl->SetPrintColourMode(wxSCI_PRINT_BLACKONWHITE);
            break;
        case pcmColorOnWhite:
            m_pControl->SetPrintColourMode(wxSCI_PRINT_COLOURONWHITE);
            break;
        case pcmInvertColors:
            m_pControl->SetPrintColourMode(wxSCI_PRINT_INVERTLIGHT);
            break;
    }
    wxLogNull ln;
//    wxPrintDialogData printDialogData(*g_printData);
//    wxPrinter printer(&printDialogData);
    InitPrinting();
    wxPrinter printer;
    wxPrintout* printout = new cbEditorPrintout(m_Filename, m_pControl, selectionOnly);
    if (!printer.Print(this, printout, true))
    {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR)
        {
            wxMessageBox(_("There was a problem printing.\n"
                            "Perhaps your current printer is not set correctly?"), _("Printing"), wxICON_ERROR);
        }
    }
    else
        *g_printData = printer.GetPrintDialogData().GetPrintData();
    delete printout;
}

// events

void cbEditor::OnContextMenuEntry(wxCommandEvent& event)
{
	// we have a single event handler for all popup menu entries,
	// so that we can add/remove options without the need to recompile
	// the whole project (almost) but more importantly, to
	// *not* break cbEditor's interface for such a trivial task...
	const int id = event.GetId();

	if (id == idUndo)
		m_pControl->Undo();
	else if (id == idRedo)
		m_pControl->Redo();
	else if (id == idCut)
		m_pControl->Cut();
	else if (id == idCopy)
		m_pControl->Copy();
	else if (id == idPaste)
		m_pControl->Paste();
	else if (id == idDelete)
		m_pControl->ReplaceSelection(wxEmptyString);
	else if (id == idSelectAll)
		m_pControl->SelectAll();
	else if (id == idSwapHeaderSource)
		Manager::Get()->GetEditorManager()->SwapActiveHeaderSource();
	else if (id == idBookmarkAdd)
		m_pControl->MarkerAdd(m_pData->m_LastMarginMenuLine, BOOKMARK_MARKER);
	else if (id == idBookmarkRemove)
		m_pControl->MarkerDelete(m_pData->m_LastMarginMenuLine, BOOKMARK_MARKER);
	else if (id == idBookmarksToggle)
		MarkerToggle(BOOKMARK_MARKER);
	else if (id == idBookmarksNext)
		MarkerNext(BOOKMARK_MARKER);
	else if (id == idBookmarksPrevious)
		MarkerPrevious(BOOKMARK_MARKER);
	else if (id == idFoldingFoldAll)
		FoldAll();
	else if (id == idFoldingUnfoldAll)
		UnfoldAll();
	else if (id == idFoldingToggleAll)
		ToggleAllFolds();
	else if (id == idFoldingFoldCurrent)
		FoldBlockFromLine();
	else if (id == idFoldingUnfoldCurrent)
		UnfoldBlockFromLine();
	else if (id == idFoldingToggleCurrent)
		ToggleFoldBlockFromLine();
	else if (id == idConfigureEditor)
		Manager::Get()->GetEditorManager()->Configure();
	else if (id == idProperties)
	{
        if (m_pProjectFile)
            m_pProjectFile->ShowOptions(this);
    }
    else if (id == idBreakpointAdd)
        AddBreakpoint(m_pData->m_LastMarginMenuLine);
    else if (id == idBreakpointEdit)
        NotifyPlugins(cbEVT_EDITOR_BREAKPOINT_EDIT, m_pData->m_LastMarginMenuLine, m_Filename);
    else if (id == idBreakpointRemove)
        RemoveBreakpoint(m_pData->m_LastMarginMenuLine);
    else
        event.Skip();
	//Manager::Get()->GetMessageManager()->DebugLog(_("Leaving OnContextMenuEntry"));
}

void cbEditor::OnMarginClick(wxScintillaEvent& event)
{
    switch (event.GetMargin())
	{
		case 1: // bookmarks and breakpoints margin
			{
				int lineYpix = event.GetPosition();
				int line = m_pControl->LineFromPosition(lineYpix);

				ToggleBreakpoint(line);
				break;
			}
		case 2: // folding margin
			{
				int lineYpix = event.GetPosition();
				int line = m_pControl->LineFromPosition(lineYpix);

				m_pControl->ToggleFold(line);
				break;
			}
	}
}

void cbEditor::OnEditorUpdateUI(wxScintillaEvent& event)
{
	if (Manager::Get()->GetEditorManager()->GetActiveEditor() == this)
	{
        NotifyPlugins(cbEVT_EDITOR_UPDATE_UI);
		HighlightBraces(); // brace highlighting
    }
}

void cbEditor::OnEditorChange(wxScintillaEvent& event)
{
    SetModified(m_pControl->GetModify());
}

void cbEditor::OnEditorCharAdded(wxScintillaEvent& event)
{
    // if message manager is auto-hiding, this will close it if not needed open
    Manager::Get()->GetMessageManager()->Close();

    m_timerWait.Stop();
	int pos = m_pControl->GetCurrentPos();
	wxChar ch = event.GetKey();
	if (ch == _T('('))
	{
		if (m_pControl->CallTipActive())
			++m_ActiveCalltipsNest;
		NotifyPlugins(cbEVT_EDITOR_CALLTIP);
	}
	if (ch == _T(')'))
	{
		// cancel any active calltip
		m_pControl->CallTipCancel();
		if (m_ActiveCalltipsNest > 0)
		{
			--m_ActiveCalltipsNest;
			NotifyPlugins(cbEVT_EDITOR_CALLTIP);
		}
	}
	else if (ch == _T('\n'))
	{
        m_pControl->BeginUndoAction();
		// new-line: adjust indentation
		bool autoIndent = Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/auto_indent"), true);
		bool smartIndent = Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/smart_indent"), true);
		int currLine = m_pControl->LineFromPosition(pos);
		if (autoIndent && currLine > 0)
		{
            wxString indent = GetLineIndentString(currLine - 1);
            if (smartIndent)
            {
                // if the last entered char before newline was an opening curly brace,
                // increase indentation level (the closing brace is handled in another block)
                wxChar b = m_pData->GetLastNonWhitespaceChar();
                if (b == _T('{'))
                {
                    if(m_pControl->GetUseTabs())
                        indent << _T('\t'); // 1 tab
                    else
                        indent << wxString(_T(' '), m_pControl->GetTabWidth()); // n spaces
                }
            }
			m_pControl->InsertText(pos, indent);
			m_pControl->GotoPos(pos + indent.Length());
			m_pControl->ChooseCaretX();
		}
		m_pControl->EndUndoAction();
	}
	else if (ch == _T('}'))
	{
		bool smartIndent = Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/smart_indent"), true);
		if (smartIndent)
		{
            m_pControl->BeginUndoAction();
            // undo block indentation, if needed
            wxString str = m_pControl->GetLine(m_pControl->GetCurrentLine());
            str.Trim(false);
            str.Trim(true);
            if (str.Matches(_T("}")))
            {
                // just the brace here; unindent
                // find opening brace (skipping nested blocks)
                int pos = m_pControl->GetCurrentPos() - 2;
                pos = m_pData->FindBlockStart(pos, _T('{'), _T('}'));
                if (pos != -1)
                {
                    wxString indent = GetLineIndentString(m_pControl->LineFromPosition(pos));
                    indent << _T('}');
                    m_pControl->DelLineLeft();
                    m_pControl->DelLineRight();
                    pos = m_pControl->GetCurrentPos();
                    m_pControl->InsertText(pos, indent);
                    m_pControl->GotoPos(pos + indent.Length());
                    m_pControl->ChooseCaretX();
                }
            }
            m_pControl->EndUndoAction();
        }
	}
	// we use -2 because the char has already been added and Pos is ahead of it...
	else if ((ch == _T('"')) || // this and the next one are for #include's completion
             (ch == _T('<')) ||
             (ch == _T('.')) ||
			((ch == _T('>')) && (m_pControl->GetCharAt(pos - 2) == _T('-'))) ||
			((ch == _T(':')) && (m_pControl->GetCharAt(pos - 2) == _T(':'))))
	{
        int style = m_pControl->GetStyleAt(pos);
		//Manager::Get()->GetMessageManager()->DebugLog(_("Style at %d is %d (char '%c')"), pos, style, ch);
        if (ch == _T('"') || ch == _T('<'))
        {
             if (style != wxSCI_C_PREPROCESSOR)
                return;
        }
        else
        {
            if (style != wxSCI_C_DEFAULT && style != wxSCI_C_OPERATOR && style != wxSCI_C_IDENTIFIER)
                return;
        }

		int timerDelay = Manager::Get()->GetConfigManager(_T("editor"))->ReadInt(_T("/cc_delay"), 500);
		if (timerDelay == 0)
			DoAskForCodeCompletion();
		else
		{
            m_pData->lastPosForCodeCompletion = pos;
			m_timerWait.Start(timerDelay);
        }
    }
}

void cbEditor::OnEditorDwellStart(wxScintillaEvent& event)
{
	int pos = m_pControl->PositionFromPoint(wxPoint(event.GetX(), event.GetY()));
    int style = m_pControl->GetStyleAt(pos);
    NotifyPlugins(cbEVT_EDITOR_TOOLTIP, style, wxEmptyString, event.GetX(), event.GetY());
}

void cbEditor::OnEditorDwellEnd(wxScintillaEvent& event)
{
	NotifyPlugins(cbEVT_EDITOR_TOOLTIP_CANCEL);
}

void cbEditor::OnUserListSelection(wxScintillaEvent& event)
{
	NotifyPlugins(cbEVT_EDITOR_USERLIST_SELECTION, 0, event.GetText());
}

void cbEditor::OnTimer(wxTimerEvent& event)
{
    // ask for code-completion *only* if the editor is still after the "." or "->" operator
    if (m_pControl->GetCurrentPos() == m_pData->lastPosForCodeCompletion)
        DoAskForCodeCompletion();
}

void cbEditor::OnClose(wxCloseEvent& event)
{
	Manager::Get()->GetEditorManager()->Close(this);
}

void cbEditor::DoIndent()
{
    if(m_pControl)
        m_pControl->SendMsg(2327); // wxSCI_CMD_TAB
}

void cbEditor::DoUnIndent()
{
    if(m_pControl)
        m_pControl->SendMsg(2328); // wxSCI_CMD_BACKTAB
}
