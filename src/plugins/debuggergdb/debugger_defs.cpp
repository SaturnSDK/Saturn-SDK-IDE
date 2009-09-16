/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk.h"
#ifndef CB_PRECOMP
#include <wx/dialog.h>
#include <wx/font.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/frame.h>
#include "manager.h"
#endif
#include "debugger_defs.h"
#include "debuggerdriver.h"
#include "debuggertree.h"
#include "watchesdlg.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(WatchesArray);

const int DEBUGGER_CURSOR_CHANGED = wxNewId();
const int DEBUGGER_SHOW_FILE_LINE = wxNewId();

DebuggerCmd::DebuggerCmd(DebuggerDriver* driver, const wxString& cmd, bool logToNormalLog)
    : m_Cmd(cmd),
    m_pDriver(driver),
    m_LogToNormalLog(logToNormalLog)
{
}

void DebuggerCmd::ParseOutput(const wxString& output)
{
    if (!output.IsEmpty() && m_LogToNormalLog)
        m_pDriver->Log(output);
}

DbgCmd_UpdateWatchesTree::DbgCmd_UpdateWatchesTree(DebuggerDriver* driver, DebuggerTree* tree)
    : DebuggerCmd(driver),
    m_pTree(tree)
{
}

void DbgCmd_UpdateWatchesTree::Action()
{
//    m_pTree->EndUpdateTree();
    Manager::Get()->GetDebuggerManager()->GetWatchesDialog()->UpdateWatches();
}

// Custom window to display output of DebuggerInfoCmd
class DebuggerInfoWindow : public wxDialog
{
    public:
        DebuggerInfoWindow(wxWindow *parent, const wxChar *title, const wxString& content)
            : wxDialog(parent, -1, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)
        {
            wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            wxFont font(8, wxMODERN, wxNORMAL, wxNORMAL);
            m_pText = new wxTextCtrl(this, -1, content, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH2 | wxHSCROLL);
            m_pText->SetFont(font);

            sizer->Add(m_pText, 1, wxGROW);

            SetSizer(sizer);
            sizer->Layout();
        }
        wxTextCtrl* m_pText;
};

void DebuggerInfoCmd::ParseOutput(const wxString& output)
{
    DebuggerInfoWindow win(Manager::Get()->GetAppWindow(), m_Title, output);
    win.ShowModal();
}


GDBWatch::GDBWatch(wxString const &symbol) :
    m_symbol(symbol),
    m_format(Undefined),
    m_array_start(0),
    m_array_count(0),
    m_is_array(false)
{
}
void GDBWatch::GetSymbol(wxString &symbol) const
{
    symbol = m_symbol;
}
void GDBWatch::GetValue(wxString &value) const
{
    value = m_raw_value;
}
bool GDBWatch::SetValue(const wxString &value)
{
    if(m_raw_value != value)
    {
        MarkAsChanged(true);
        m_raw_value = value;
    }
    return true;
}
void GDBWatch::GetFullWatchString(wxString &full_watch) const
{
    const cbWatch* parent = GetParent();
    if (parent)
    {
        parent->GetFullWatchString(full_watch);
        full_watch += wxT(".") + m_symbol;
    }
    else
        full_watch = m_symbol;
}

void GDBWatch::GetType(wxString &type) const
{
    type = m_type;
}
void GDBWatch::SetType(const wxString &type)
{
    m_type = type;
}

wxString const & GDBWatch::GetDebugString() const
{
    return m_debug_value;
}
void GDBWatch::SetDebugValue(wxString const &value)
{
    m_debug_value = value;
}

void GDBWatch::SetSymbol(const wxString& symbol)
{
    m_symbol = symbol;
}

void GDBWatch::DoDestroy()
{
    delete this;
}

void GDBWatch::SetFormat(WatchFormat format)
{
    m_format = format;
}

WatchFormat GDBWatch::GetFormat() const
{
    return m_format;
}

void GDBWatch::SetArray(bool flag)
{
    m_is_array = flag;
}

bool GDBWatch::IsArray() const
{
    return m_is_array;
}

void GDBWatch::SetArrayParams(int start, int count)
{
    m_array_start = start;
    m_array_count = count;
}

int GDBWatch::GetArrayStart() const
{
    return m_array_start;
}

int GDBWatch::GetArrayCount() const
{
    return m_array_count;
}
