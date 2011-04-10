/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <set>

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
#   include "globals.h"
#   include "manager.h"
#   include "editormanager.h"
#   include "cbeditor.h"
#   include "cbplugin.h"

#   include <wx/button.h>
#   include <wx/checkbox.h>
#   include <wx/intl.h>
#   include <wx/listbox.h>
#   include <wx/listctrl.h>
#   include <wx/menu.h>
#   include <wx/textctrl.h>
#   include <wx/spinctrl.h>
#   include <wx/sizer.h>
#endif

#include "debuggermanager.h"
#include "editbreakpointdlg.h"

#include "breakpointsdlg.h"

namespace
{
	const int idList = wxNewId();
	const int idRemove = wxNewId();
	const int idRemoveAll = wxNewId();
	const int idProperties = wxNewId();
	const int idOpen = wxNewId();
};

BEGIN_EVENT_TABLE(cbBreakpointsDlg, wxPanel)
    EVT_MENU(idRemove, cbBreakpointsDlg::OnRemove)
    EVT_MENU(idRemoveAll, cbBreakpointsDlg::OnRemoveAll)
    EVT_MENU(idProperties, cbBreakpointsDlg::OnProperties)
    EVT_MENU(idOpen, cbBreakpointsDlg::OnOpen)

    EVT_KEY_UP(cbBreakpointsDlg::OnKeyUp)
END_EVENT_TABLE()

cbBreakpointsDlg::cbBreakpointsDlg() :
    wxPanel(Manager::Get()->GetAppWindow(), -1)
{
    wxBoxSizer* bs = new wxBoxSizer(wxVERTICAL);
    m_pList = new wxListCtrl(this, idList, wxDefaultPosition, wxDefaultSize,
                             wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES);
    bs->Add(m_pList, 1, wxEXPAND | wxALL);
    SetAutoLayout(TRUE);
    SetSizer(bs);

	m_pList->InsertColumn(Type, _("Type"), wxLIST_FORMAT_LEFT, 128);
	m_pList->InsertColumn(FilenameAddress, _("Filename/Address"), wxLIST_FORMAT_LEFT, 128);
	m_pList->InsertColumn(Line, _("Line"), wxLIST_FORMAT_LEFT, 44);
	m_pList->InsertColumn(Debugger, _("Debugger"), wxLIST_FORMAT_LEFT, 60);

    Connect(idList, -1, wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            (wxObjectEventFunction) (wxEventFunction) (wxListEventFunction)
            &cbBreakpointsDlg::OnDoubleClick);

    Connect(idList, -1, wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,
            (wxObjectEventFunction) (wxEventFunction) (wxListEventFunction)
            &cbBreakpointsDlg::OnRightClick);

    typedef cbEventFunctor<cbBreakpointsDlg, CodeBlocksEvent> CBEvent;

    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_BREAKPOINT_ADD,
                                      new CBEvent(this, &cbBreakpointsDlg::OnBreakpointAdd));
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_BREAKPOINT_EDIT,
                                      new CBEvent(this, &cbBreakpointsDlg::OnBreakpointEdit));
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_BREAKPOINT_DELETE,
                                      new CBEvent(this, &cbBreakpointsDlg::OnBreakpointDelete));


    Reload();
}

void cbBreakpointsDlg::Reload()
{
    m_pList->Freeze();
    m_pList->DeleteAllItems();
    m_breakpoints.clear();

    DebuggerManager::RegisteredPlugins const &debuggers = Manager::Get()->GetDebuggerManager()->GetAllDebuggers();
    for (DebuggerManager::RegisteredPlugins::const_iterator dbg = debuggers.begin(); dbg != debuggers.end(); ++dbg)
    {
        int count = dbg->first->GetBreakpointsCount();
        for (int ii = 0; ii < count; ++ii)
        {
            cbBreakpoint *bp = dbg->first->GetBreakpoint(ii);
            m_breakpoints.push_back(Item(bp, dbg->first, dbg->second.GetGUIName()));
        }
    }

    for (Items::const_iterator it = m_breakpoints.begin(); it != m_breakpoints.end(); ++it)
    {
        m_pList->InsertItem(m_pList->GetItemCount(), _T(""));
        long item = m_pList->GetItemCount() - 1;
        switch (it->breakpoint->GetType())
        {
            case cbBreakpoint::Code:
                m_pList->SetItem(item, Type, _("Code"));
                m_pList->SetItem(item, FilenameAddress, it->breakpoint->GetFilename());
                m_pList->SetItem(item, Line, wxString::Format(_T("%d"), it->breakpoint->GetLine()));
                break;
            case cbBreakpoint::Data:
                {
                    m_pList->SetItem(item, Type, _("Data"));
                    wxString s = it->breakpoint->GetDataExpression() + _T(" (read: ")
                                  + (it->breakpoint->GetBreakOnRead() ? _("yes") : _("no")) + _(" write: ")
                                  + (it->breakpoint->GetBreakOnWrite() ? _("yes") : _("no")) + _(")");
                    m_pList->SetItem(item, FilenameAddress, s);
                    break;
                }
            default:
                m_pList->SetItem(item, Type, _("Unknown"));
        }

        m_pList->SetItem(item, Debugger, it->pluginName);
    }

    if (!m_breakpoints.empty())
    {
        for (int column = 0; column < m_pList->GetColumnCount(); ++column)
            m_pList->SetColumnWidth(column, wxLIST_AUTOSIZE);
    }
    m_pList->Thaw();
}

bool cbBreakpointsDlg::AddBreakpoint(const wxString& filename, int line)
{
    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    if (!plugin)
        return false;

    if (plugin->AddBreakpoint(filename, line))
    {
        Reload();
        return true;
    }
    else
        return false;
}

bool cbBreakpointsDlg::RemoveBreakpoint(const wxString& filename, int line)
{
    bool found = false;
    for (Items::const_iterator it = m_breakpoints.begin(); it != m_breakpoints.end(); ++it)
    {
        if (it->breakpoint->GetType() == cbBreakpoint::Code)
        {
            if (it->breakpoint->GetFilename() == filename && it->breakpoint->GetLine() == line)
            {
                it->plugin->DeleteBreakpoint(it->breakpoint);
                found = true;
            }
        }
    }
    if (found)
        Reload();
    return found;
}

void cbBreakpointsDlg::RemoveBreakpoint(int sel)
{
    if(sel < 0 || sel >= static_cast<int>(m_breakpoints.size()))
        return;
    Item const &item = m_breakpoints[sel];

    if (item.breakpoint->GetType() == cbBreakpoint::Code)
    {
        cbEditor* ed = Manager::Get()->GetEditorManager()->IsBuiltinOpen(item.breakpoint->GetFilename());
        if (ed)
            ed->RemoveBreakpoint(item.breakpoint->GetLine() - 1, false);
    }

    item.plugin->DeleteBreakpoint(item.breakpoint);
}

void cbBreakpointsDlg::OnRemove(wxCommandEvent& event)
{
    long item = m_pList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item == -1)
        return;
    RemoveBreakpoint(item);
    Reload();
}

void cbBreakpointsDlg::OnRemoveAll(wxCommandEvent& event)
{
    std::set<cbDebuggerPlugin*> plugins;

    for (Items::iterator it = m_breakpoints.begin(); it != m_breakpoints.end(); ++it)
    {
        cbBreakpoint& b = *it->breakpoint;

        if (b.GetType() == cbBreakpoint::Code)
        {
            cbEditor* ed = Manager::Get()->GetEditorManager()->IsBuiltinOpen(b.GetFilename());
            if (ed)
            {
                ed->RemoveBreakpoint(b.GetLine() - 1, false);

                plugins.insert(it->plugin);
            }
        }
        else
            plugins.insert(it->plugin);
    }
    for (std::set<cbDebuggerPlugin*>::iterator it = plugins.begin(); it != plugins.end(); ++it)
    {
        (*it)->DeleteAllBreakpoints();
    }

    Reload();
}

void cbBreakpointsDlg::OnProperties(wxCommandEvent& event)
{
    int sel = m_pList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (sel < 0 || sel >= static_cast<int>(m_breakpoints.size()))
        return;
    Item const &item = m_breakpoints[sel];
    item.plugin->UpdateBreakpoint(item.breakpoint);
    Reload();
}

void cbBreakpointsDlg::OnOpen(wxCommandEvent& event)
{
    long item_index = m_pList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item_index < 0 || item_index >= static_cast<int>(m_breakpoints.size()))
        return;

    Item const &item = m_breakpoints[item_index];

    if (item.breakpoint->GetType() == cbBreakpoint::Code)
    {
        cbEditor* ed = Manager::Get()->GetEditorManager()->Open(item.breakpoint->GetFilename());
        if (ed)
        {
            ed->GotoLine(item.breakpoint->GetLine() - 1, true);
            ed->Activate();
        }
    }
}

void cbBreakpointsDlg::OnRightClick(wxListEvent& event)
{
    wxMenu menu;
    menu.Append(idOpen, _("Open in editor"));
    menu.Append(idProperties, _("Breakpoint properties"));
    menu.AppendSeparator();
    menu.Append(idRemove, _("Remove breakpoint"));
    menu.Append(idRemoveAll, _("Remove all breakpoints"));
    PopupMenu(&menu);
}

void cbBreakpointsDlg::OnDoubleClick(wxListEvent& event)
{
    wxCommandEvent evt;
    OnOpen(evt);
}

void cbBreakpointsDlg::OnKeyUp(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_DELETE || event.GetKeyCode() == WXK_NUMPAD_DELETE)
    {
        long item = m_pList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (item == -1)
            return;
        RemoveBreakpoint(item);
        Reload();
    }
}

void cbBreakpointsDlg::OnBreakpointAdd(CodeBlocksEvent& event)
{
// TODO (obfuscated#) add the line text to the breakpoint
    DebuggerManager *manager = Manager::Get()->GetDebuggerManager();
    if(manager->GetActiveDebugger())
        manager->GetActiveDebugger()->AddBreakpoint(event.GetString(), event.GetInt());

    Reload();
}

void cbBreakpointsDlg::OnBreakpointEdit(CodeBlocksEvent& event)
{
    const wxString& filename = event.GetString();
    int line = event.GetInt();

    for (Items::iterator it = m_breakpoints.begin(); it != m_breakpoints.end(); ++it)
    {
        if (it->breakpoint->GetFilename() == filename && it->breakpoint->GetLine() == line)
        {
            it->plugin->UpdateBreakpoint(it->breakpoint);
            Reload();
        }
    }
}

void cbBreakpointsDlg::OnBreakpointDelete(CodeBlocksEvent& event)
{
    RemoveBreakpoint(event.GetString(), event.GetInt());
}

