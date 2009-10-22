/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include "watchesdlg.h"

#include <wx/propgrid/propgrid.h>

namespace
{
    const int idGrid = wxNewId();

    const int idMenuRename = wxNewId();
    const int idMenuProperties = wxNewId();
    const int idMenuDelete = wxNewId();
    const int idMenuDeleteAll = wxNewId();
}

BEGIN_EVENT_TABLE(WatchesDlg, wxPanel)
    EVT_PG_ITEM_EXPANDED(idGrid, WatchesDlg::OnExpand)
    EVT_PG_ITEM_COLLAPSED(idGrid, WatchesDlg::OnCollapse)
    EVT_PG_SELECTED(idGrid, WatchesDlg::OnPropertySelected)
    EVT_PG_CHANGED(idGrid, WatchesDlg::OnPropertyChanged)
    EVT_PG_CHANGING(idGrid, WatchesDlg::OnPropertyChanging)
    EVT_PG_LABEL_EDIT_BEGIN(idGrid, WatchesDlg::OnPropertyLableEditBegin)
    EVT_PG_LABEL_EDIT_ENDING(idGrid, WatchesDlg::OnPropertyLableEditEnd)
    EVT_PG_RIGHT_CLICK(idGrid, WatchesDlg::OnPropertyRightClick)
    EVT_KEY_DOWN(WatchesDlg::OnKeyDown)
    EVT_KEY_UP(WatchesDlg::OnKeyDown)
    EVT_IDLE(WatchesDlg::OnIdle)

    EVT_MENU(idMenuRename, WatchesDlg::OnMenuRename)
    EVT_MENU(idMenuProperties, WatchesDlg::OnMenuProperties)
    EVT_MENU(idMenuDelete, WatchesDlg::OnMenuDelete)
    EVT_MENU(idMenuDeleteAll, WatchesDlg::OnMenuDeleteAll)
END_EVENT_TABLE()

/// @breif dialog to show the value of a watch
class WatchRawDialog : public wxScrollingDialog
{
    public:
        WatchRawDialog(const wxString& symbol, const wxString &value) :
            wxScrollingDialog(Manager::Get()->GetAppWindow(),
                              wxID_ANY,
                              wxString::Format(wxT("Watch '%s' raw value"), symbol.c_str()),
                              wxDefaultPosition,
                              wxSize(400, 400),
                              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
        {
            wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);
            wxTextCtrl *text = new wxTextCtrl(this, wxID_ANY, value, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
            bs->Add(text, 1, wxEXPAND | wxALL, 5);
            SetAutoLayout(TRUE);
            SetSizer(bs);
        }

        void OnClose(wxCloseEvent &event)
        {
            Destroy();
        }
    private:
        DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(WatchRawDialog, wxScrollingDialog)
    EVT_CLOSE(WatchRawDialog::OnClose)
END_EVENT_TABLE()

class WatchesProperty;

class WatchRawDialogAdapter : public wxPGEditorDialogAdapter
{
    public:

        WatchRawDialogAdapter()
        {
        }

        virtual bool DoShowDialog(wxPropertyGrid* WXUNUSED(propGrid), wxPGProperty* property);

        void WatchToString(wxString &result, const cbWatch &watch, const wxString &indent = wxEmptyString)
        {
            wxString sym, value;
            watch.GetSymbol(sym);
            watch.GetValue(value);

            result += indent + wxT("[symbol = ") + sym + wxT("]\n");
            result += indent + wxT("[value = ") + value + wxT("]\n");
            result += indent + wxString::Format(wxT("[children = %d]\n"), watch.GetChildCount());

            for(int child_index = 0; child_index < watch.GetChildCount(); ++child_index)
            {
                const cbWatch *child = watch.GetChild(child_index);

                result += indent + wxString::Format(wxT("[child %d]\n"), child_index);
                WatchToString(result, *child, indent + wxT("    "));
            }
        }

    protected:
};

class WatchesProperty : public wxStringProperty
{
    public:
        WatchesProperty(const wxString& label, const wxString& value, cbWatch *watch) :
            wxStringProperty(label, wxPG_LABEL, value),
            m_watch(watch)
        {
        }

        // Set editor to have button
        virtual const wxPGEditor* DoGetEditorClass() const
        {
            return wxPG_EDITOR(TextCtrlAndButton);
        }

        // Set what happens on button click
        virtual wxPGEditorDialogAdapter* GetEditorDialog() const
        {
            return new WatchRawDialogAdapter();
        }

        cbWatch* GetWatch() { return m_watch; }
        void SetWatch(cbWatch* watch) { m_watch = watch; }

    protected:
        cbWatch *m_watch;
};

bool WatchRawDialogAdapter::DoShowDialog(wxPropertyGrid* WXUNUSED(propGrid), wxPGProperty* property)
{
    WatchesProperty *watch = static_cast<WatchesProperty*>(property);
    wxString value;

    if (watch->GetWatch())
    {
        if (wxGetKeyState(WXK_CONTROL))
            value = watch->GetWatch()->GetDebugString();
        else if (wxGetKeyState(WXK_SHIFT))
            WatchToString(value, *watch->GetWatch());
        else
            value = property->GetValueAsString(wxPG_FULL_VALUE);
    }
    else
        value = property->GetValueAsString(wxPG_FULL_VALUE);

    WatchRawDialog *dlg = new WatchRawDialog(property->GetName(), value);
    //dlg.ShowModal();
    dlg->Show();

    return false;
}

WatchesDlg::WatchesDlg() :
    wxPanel(Manager::Get()->GetAppWindow(), -1),
    m_append_empty_watch(false)
{
    wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);
    m_grid = new wxPropertyGrid(this, idGrid, wxDefaultPosition, wxDefaultSize,
                                wxPG_SPLITTER_AUTO_CENTER | wxTAB_TRAVERSAL);

    m_grid->SetExtraStyle(wxPG_EX_DISABLE_TLP_TRACKING);
    bs->Add(m_grid, 1, wxEXPAND | wxALL);
    SetAutoLayout(TRUE);
    SetSizer(bs);

//    m_grid->MakeColumnEditable(0);
    m_grid->Append(new WatchesProperty(wxT(""), wxT(""), NULL));
}

void AppendChildren(wxPropertyGrid &grid, wxPGProperty &property, cbWatch &watch)
{
    for(int ii = 0; ii < watch.GetChildCount(); ++ii)
    {
        cbWatch &child = *watch.GetChild(ii);

        wxString symbol, value;
        child.GetSymbol(symbol);
        child.GetValue(value);

        wxPGProperty *prop = new WatchesProperty(symbol, value, &child);
        prop->SetExpanded(child.IsExpanded());
        grid.AppendIn(&property, prop);

        if(child.IsChanged())
            grid.SetPropertyTextColour(prop, wxColor(255, 0, 0));
        else
            grid.SetPropertyColourToDefault(prop);
        child.MarkAsChanged(false);

        AppendChildren(grid, *prop, child);
    }
}

void WatchesDlg::UpdateWatches()
{
    for(WatchItems::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        if(!it->property)
            continue;

        wxString value, symbol;
        it->watch->GetSymbol(symbol);
        it->watch->GetValue(value);
        it->property->SetValue(value);
        if(it->watch->IsChanged())
            m_grid->SetPropertyTextColour(it->property, wxColor(255, 0, 0));
        else
            m_grid->SetPropertyColourToDefault(it->property);
        it->watch->MarkAsChanged(false);

        it->property->DeleteChildren();

        if(it->property->GetName() != symbol)
        {
            m_grid->SetPropertyName(it->property, symbol);
            m_grid->SetPropertyLabel(it->property, symbol);
        }

        AppendChildren(*m_grid, *it->property, *it->watch);
    }
}

void WatchesDlg::AddWatch(cbWatch *watch)
{
    wxPGProperty *last_prop = m_grid->GetLastProperty();

    WatchItem item;
    wxString symbol, value;
    watch->GetSymbol(symbol);

    if(last_prop && last_prop->GetLabel() == wxEmptyString)
    {
        item.property = last_prop;

        // if we are editing the label the calls SetPropertyLabel and SetPropertyName don't work,
        // so we stop the edit operationś
        if(m_grid->GetLabelEditor())
            m_grid->EndLabelEdit(0);
        m_grid->SetPropertyLabel(item.property, symbol);
        m_grid->SetPropertyName(item.property, symbol);

        WatchesProperty *watches_prop = static_cast<WatchesProperty*>(last_prop);
        watches_prop->SetWatch(watch);
        m_grid->Append(new WatchesProperty(wxT(""), wxT(""), NULL));
    }
    else
    {
        item.property = m_grid->Append(new WatchesProperty(symbol, value, watch));
    }

    item.property->SetExpanded(watch->IsExpanded());
    item.watch = watch;
    m_watches.push_back(item);
}

void WatchesDlg::OnExpand(wxPropertyGridEvent &event)
{
    WatchesProperty *prop = static_cast<WatchesProperty*>(event.GetProperty());
    prop->GetWatch()->Expand(true);
}

void WatchesDlg::OnCollapse(wxPropertyGridEvent &event)
{
    WatchesProperty *prop = static_cast<WatchesProperty*>(event.GetProperty());
    prop->GetWatch()->Expand(false);
}

void WatchesDlg::OnPropertyChanged(wxPropertyGridEvent &event)
{
    WatchesProperty *prop = static_cast<WatchesProperty*>(event.GetProperty());
    if (prop)
    {
        cbWatch *watch = prop->GetWatch();
        if (watch)
        {
            cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetDebuggerHavingWatch(watch);
            if (plugin)
            {
                plugin->SetWatchValue(watch, prop->GetValue());
            }
        }
    }
}

void WatchesDlg::OnPropertyChanging(wxPropertyGridEvent &event)
{
    if(event.GetProperty()->GetCount() > 0)
        event.Veto(true);
//    Manager::Get()->GetLogManager()->DebugLog(wxT("OnPropertyChanged"));
}

void WatchesDlg::OnPropertyLableEditBegin(wxPropertyGridEvent &event)
{
    Manager::Get()->GetLogManager()->DebugLog(wxT("OnPropertyLableEditBegin"));
    wxPGProperty *prop = event.GetProperty();

    if(prop)
    {
        wxPGProperty *prop_parent = prop->GetParent();
        if(prop_parent && !prop_parent->IsRoot())
            event.Veto(true);
    }
}

void WatchesDlg::OnPropertyLableEditEnd(wxPropertyGridEvent &event)
{
    const wxString& label = m_grid->GetLabelEditor()->GetValue();
    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    WatchesProperty *prop = static_cast<WatchesProperty*>(event.GetProperty());

    if(label == wxEmptyString)
        return;

    if(plugin && prop)
    {
        // if the user have edited existing watch, we replace it.
        if(prop->GetWatch())
        {
            cbWatch *old_watch = prop->GetWatch();
            prop->SetWatch(nullptr);
            plugin->DeleteWatch(old_watch);
            cbWatch *new_watch = plugin->AddWatch(label);
            prop->SetWatch(new_watch);

            for(WatchItems::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
            {
                if(it->property == prop)
                {
                    it->watch = new_watch;
                }
            }
            prop->SetExpanded(new_watch->IsExpanded());
        }
        else
        {
            WatchItem item;
            item.property = prop;
            item.watch = plugin->AddWatch(label);
            prop->SetWatch(item.watch);
            m_watches.push_back(item);
            prop->SetExpanded(item.watch->IsExpanded());

            m_append_empty_watch = true;
        }
    }
}

void WatchesDlg::OnIdle(wxIdleEvent &event)
{
    if(m_append_empty_watch)
    {
        m_grid->Append(new WatchesProperty(wxT(""), wxT(""), NULL));
        m_append_empty_watch = false;
    }
}

void WatchesDlg::OnPropertySelected(wxPropertyGridEvent &event)
{
    if(event.GetProperty() && event.GetPropertyLabel() == wxEmptyString)
        m_grid->BeginLabelEdit(0);
}

void WatchesDlg::DeleteProperty(WatchesProperty &prop)
{
    cbWatch *watch = prop.GetWatch();
    if(!watch)
        return;

    cbDebuggerPlugin *debugger = Manager::Get()->GetDebuggerManager()->GetDebuggerHavingWatch(watch);
    debugger->DeleteWatch(watch);

    wxPGProperty *parent = prop.GetParent();
    if(parent && parent->IsRoot())
    {
        for(WatchItems::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
        {
            if(!it->property)
                continue;
            if(it->property == &prop)
            {
                m_watches.erase(it);
                break;
            }
        }
        prop.DeleteChildren();
        m_grid->DeleteProperty(&prop);
    }
}

void WatchesDlg::OnKeyDown(wxKeyEvent &event)
{
    wxPGProperty *prop = m_grid->GetSelection();
    WatchesProperty *watches_prop = static_cast<WatchesProperty*>(prop);
    switch(event.GetKeyCode())
    {
        case WXK_DELETE:
            if(prop && !m_grid->GetLabelEditor())
                DeleteProperty(*watches_prop);
            break;
        case WXK_INSERT:
            if(prop && prop->GetParent() && prop->GetParent()->IsRoot())
            {
                if(!m_grid->GetLabelEditor())
                    m_grid->BeginLabelEdit(0);
            }
            break;
    }
}

void WatchesDlg::OnPropertyRightClick(wxPropertyGridEvent &event)
{
    wxMenu m;
    m.Append(idMenuRename, _("Rename"), _("Rename the watch"));
    m.Append(idMenuProperties, _("Properties"), _("Show the properties for the watch"));
    m.Append(idMenuDelete, _("Delete"), _("Delete the currently selected watch"));
    m.Append(idMenuDeleteAll, _("Delete All"), _("Delete all watches"));

    PopupMenu(&m);
}

void WatchesDlg::OnMenuRename(wxCommandEvent &event)
{
    if(!m_grid->GetLabelEditor())
    {
        m_grid->SetFocus();
        m_grid->BeginLabelEdit(0);
    }
}

void WatchesDlg::OnMenuProperties(wxCommandEvent &event)
{
    wxPGProperty *selected = m_grid->GetSelection();
    if(selected)
    {
        WatchesProperty *prop = static_cast<WatchesProperty*>(selected);
        cbWatch *watch = prop->GetWatch();
        if(watch)
        {
            cbDebuggerPlugin *debugger = Manager::Get()->GetDebuggerManager()->GetDebuggerHavingWatch(watch);
            if(debugger)
                debugger->ShowWatchProperties(watch);
        }
    }
}

void WatchesDlg::OnMenuDelete(wxCommandEvent &event)
{
    wxPGProperty *selected = m_grid->GetSelection();
    if(selected)
    {
        WatchesProperty *prop = static_cast<WatchesProperty*>(selected);
        DeleteProperty(*prop);
    }
}

void WatchesDlg::OnMenuDeleteAll(wxCommandEvent &event)
{
    for (WatchItems::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        cbDebuggerPlugin *debugger = Manager::Get()->GetDebuggerManager()->GetDebuggerHavingWatch(it->watch);
        debugger->DeleteWatch(it->watch);
        m_grid->DeleteProperty(it->property);
    }
    m_watches.clear();
}
