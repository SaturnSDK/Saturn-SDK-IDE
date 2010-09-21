/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"

#include "disassemblydlg.h"
#include <wx/wxscintilla.h>
#include <wx/intl.h>
#include <wx/xrc/xmlres.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/wfstream.h>
#include <wx/fontutil.h>
#include <globals.h>
#include <editorcolourset.h>
#include <manager.h>
#include <editormanager.h>
#include <configmanager.h>
#include <projectmanager.h>
#include <cbproject.h>
#include <filefilters.h>

const int DEBUG_MARKER = 4;
const int DEBUG_STYLE = wxSCI_MARK_ARROW;

BEGIN_EVENT_TABLE(cbDisassemblyDlg, wxPanel)
    EVT_BUTTON(XRCID("btnSave"), cbDisassemblyDlg::OnSave)
//    EVT_BUTTON(XRCID("btnRefresh"), cbDisassemblyDlg::OnRefresh)
END_EVENT_TABLE()

cbDisassemblyDlg::cbDisassemblyDlg(wxWindow* parent) :
    m_LastActiveAddr(0),
    m_ClearFlag(false)
{
    wxXmlResource::Get()->LoadPanel(this, parent, _T("dlgDisassembly"));

    m_pCode = new wxScintilla(this, wxID_ANY);
    m_pCode->SetReadOnly(true);
    m_pCode->SetCaretWidth(0);
    m_pCode->SetMarginWidth(0, 0);
    m_pCode->SetMarginType(1, wxSCI_MARGIN_SYMBOL);
    m_pCode->SetMarginSensitive(1, 0);
    m_pCode->SetMarginMask(1, (1 << DEBUG_MARKER));
    m_pCode->MarkerDefine(DEBUG_MARKER, DEBUG_STYLE);
    m_pCode->MarkerSetBackground(DEBUG_MARKER, wxColour(0xFF, 0xFF, 0x00));
    wxXmlResource::Get()->AttachUnknownControl(_T("lcCode"), m_pCode);

    // use the same font as editor's
    wxFont font(8, wxMODERN, wxNORMAL, wxNORMAL);
    wxString fontstring = Manager::Get()->GetConfigManager(_T("editor"))->Read(_T("/font"), wxEmptyString);
    if (!fontstring.IsEmpty())
    {
        wxNativeFontInfo nfi;
        nfi.FromString(fontstring);
        font.SetNativeFontInfo(nfi);
    }
    m_pCode->StyleSetFont(wxSCI_STYLE_DEFAULT, font);

    EditorColourSet* set = Manager::Get()->GetEditorManager()->GetColourSet();
    if (set)
    {
        HighlightLanguage lang = set->GetHighlightLanguage(wxSCI_LEX_ASM);
        set->Apply(lang, (cbStyledTextCtrl*)m_pCode);
    }

    cbStackFrame sf;
    Clear(sf);
}

void cbDisassemblyDlg::Clear(const cbStackFrame& frame)
{
    m_FrameFunction = frame.IsValid() ? frame.GetSymbol() : _T("??");
    m_FrameAddress = _T("??");
    if (frame.IsValid())
        m_FrameAddress.Printf(_T("%p"), (void*)frame.GetAddress());

    XRCCTRL(*this, "lblFunction", wxStaticText)->SetLabel(m_FrameFunction);
    XRCCTRL(*this, "lblAddress", wxStaticText)->SetLabel(m_FrameAddress);

    m_HasActiveAddr = false;

    m_pCode->SetReadOnly(false);

    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();

    if (frame.IsValid() && plugin->IsRunning())
    {
        // if debugger is running, show a message
        m_pCode->SetText(_("\"Please wait while disassembling...\""));
        m_ClearFlag = true; // clear the above message when adding the first line
    }
    else
    {
        // if debugger isn't running, just clear the window
        m_pCode->ClearAll();
        m_ClearFlag = false;
    }
    m_pCode->SetReadOnly(true);
    m_pCode->MarkerDeleteAll(DEBUG_MARKER);
}

void cbDisassemblyDlg::AddAssemblerLine(unsigned long int addr, const wxString& line)
{
    m_pCode->SetReadOnly(false);
    if (m_ClearFlag)
    {
        m_ClearFlag = false;
        m_pCode->ClearAll();
    }
    wxString fmt;
    fmt.Printf(_T("%p\t%s\n"), (void*)addr, line.c_str());
    if (!fmt.StartsWith(wxT("0x")))
        fmt = wxT("0x") + fmt;

    m_pCode->AppendText(fmt);
    SetActiveAddress(m_LastActiveAddr);
    m_pCode->SetReadOnly(true);
}

void cbDisassemblyDlg::SetActiveAddress(unsigned long int addr)
{
    if (m_HasActiveAddr && addr == m_LastActiveAddr)
        return;
    m_HasActiveAddr = false;
    m_LastActiveAddr = addr;
    for (int i = 0; i < m_pCode->GetLineCount(); ++i)
    {
        wxString str = m_pCode->GetLine(i).AfterFirst(_T('x')).BeforeFirst(_T('\t'));
        unsigned long int lineaddr;
        if (str.ToULong(&lineaddr, 16) && lineaddr >= addr)
        {
            m_pCode->MarkerDeleteAll(DEBUG_MARKER);
            m_pCode->MarkerAdd(i, DEBUG_MARKER);
            m_pCode->GotoLine(i);
            m_pCode->EnsureVisible(i);
            m_HasActiveAddr = true;
            break;
        }
    }
}

void cbDisassemblyDlg::OnSave(wxCommandEvent& event)
{
    wxFileDialog dlg(this,
                     _("Save as text file"),
                     _T("assembly_dump.txt"),
                     wxEmptyString,
                     FileFilters::GetFilterAll(),
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
        return;

    wxString output;
    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (prj)
    {
        output << _("Project title : ") << prj->GetTitle() << _T('\n');
        output << _("Project path  : ") << prj->GetBasePath() << _T('\n') << _T('\n');
    }

    output << _("Frame function: ") << m_FrameFunction << _T('\n');
    output << _("Frame address : ") << m_FrameAddress << _T('\n');
    output << wxString(_T('-'), 80) << _T('\n');
    output << m_pCode->GetText();

    if (!cbSaveToFile(dlg.GetPath(), output))
        cbMessageBox(_("Could not save file..."), _("Error"), wxICON_ERROR);
}

void cbDisassemblyDlg::OnRefresh(wxCommandEvent& event)
{
    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    cbAssert(plugin);
    plugin->RequestUpdate(cbDebuggerPlugin::Disassembly);
}
