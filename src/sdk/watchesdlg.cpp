/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"
#ifndef CB_PRECOMP
    #include <wx/dnd.h>
    #include <wx/menu.h>
    #include <wx/sizer.h>

    #include "cbexception.h"
    #include "cbplugin.h"
    #include "logmanager.h"
    #include "scrollingdialog.h"
#endif

#include <map>
#include <wx/propgrid/propgrid.h>

#include "watchesdlg.h"

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


class cbTextCtrlAndButtonTooltipEditor : public wxPGTextCtrlAndButtonEditor
{
    DECLARE_DYNAMIC_CLASS(wxTextCtrlAndButtonTooltipEditor)
public:
    virtual wxPG_CONST_WXCHAR_PTR GetName() const;

    virtual wxPGWindowList CreateControls(wxPropertyGrid* propgrid, wxPGProperty* property,
                                          const wxPoint& pos, const wxSize& sz) const
    {
        wxPGWindowList const &list = wxPGTextCtrlAndButtonEditor::CreateControls(propgrid, property, pos, sz);

        list.m_secondary->SetToolTip(_("Click the button to see the value.\n"
                                       "Hold CONTROL to see the raw output string returned by the debugger.\n"
                                       "Hold SHIFT to see debugging representation of the cbWatch object."));
        return list;
    }

};

WX_PG_DECLARE_EDITOR(cbTextCtrlAndButtonTooltip);
WX_PG_IMPLEMENT_EDITOR_CLASS(cbTextCtrlAndButtonTooltip, cbTextCtrlAndButtonTooltipEditor, wxPGTextCtrlAndButtonEditor);

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
            return wxPG_EDITOR(cbTextCtrlAndButtonTooltip);
        }

        // Set what happens on button click
        virtual wxPGEditorDialogAdapter* GetEditorDialog() const;

        cbWatch* GetWatch() { return m_watch; }
        const cbWatch* GetWatch() const { return m_watch; }
        void SetWatch(cbWatch* watch) { m_watch = watch; }

    protected:
        cbWatch *m_watch;
};

class WatchRawDialogAdapter : public wxPGEditorDialogAdapter
{
    public:

        WatchRawDialogAdapter()
        {
        }

        virtual bool DoShowDialog(wxPropertyGrid* WXUNUSED(propGrid), wxPGProperty* property);

    protected:
};

/// @breif dialog to show the value of a watch
class WatchRawDialog : public wxScrollingDialog
{
    private:
        enum Type
        {
            TypeNormal,
            TypeDebug,
            TypeWatchTree
        };
    public:
        static WatchRawDialog* Create(const WatchesProperty* watch)
        {
            cbAssert(watch->GetWatch());

            WatchRawDialog *dlg;
            Map::iterator it = s_dialogs.find(watch->GetWatch());
            if (it != s_dialogs.end())
                dlg = it->second;
            else
            {
                dlg = new WatchRawDialog;
                s_dialogs[watch->GetWatch()] = dlg;
            }

            dlg->m_type = TypeNormal;

            if (wxGetKeyState(WXK_CONTROL))
                dlg->m_type = TypeDebug;
            else if (wxGetKeyState(WXK_SHIFT))
                dlg->m_type = TypeWatchTree;

            dlg->SetTitle(wxString::Format(wxT("Watch '%s' raw value"), watch->GetName().c_str()));
            dlg->SetValue(watch);
            dlg->Raise();

            return dlg;
        }

        static void UpdateValue(const WatchesProperty* watch)
        {
            Map::iterator it = s_dialogs.find(watch->GetWatch());
            if (it != s_dialogs.end())
                it->second->SetValue(watch);
        }
    private:
        WatchRawDialog() :
            wxScrollingDialog(Manager::Get()->GetAppWindow(),
                              wxID_ANY,
                              wxEmptyString,
                              wxDefaultPosition,
                              wxSize(400, 400),
                              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
            m_type(TypeNormal)
        {
            wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);
            m_text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                    wxTE_MULTILINE | wxTE_READONLY);
            bs->Add(m_text, 1, wxEXPAND | wxALL, 5);
            SetAutoLayout(TRUE);
            SetSizer(bs);
        }

        void OnClose(wxCloseEvent &event)
        {
            for (Map::iterator it = s_dialogs.begin(); it != s_dialogs.end(); ++it)
            {
                if (it->second == this)
                {
                    s_dialogs.erase(it);
                    break;
                }
            }
            Destroy();
        }

        void SetValue(const WatchesProperty* watch)
        {
            switch (m_type)
            {
                case TypeNormal:
                    m_text->SetValue(watch->GetValueAsString(wxPG_FULL_VALUE));
                    break;

                case TypeDebug:
                    m_text->SetValue(watch->GetWatch()->GetDebugString());
                    break;

                case TypeWatchTree:
                    {
                        wxString value;
                        WatchToString(value, *watch->GetWatch());
                        m_text->SetValue(value);
                    }
                    break;
            }
        }

        static void WatchToString(wxString &result, const cbWatch &watch, const wxString &indent = wxEmptyString)
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
    private:
        DECLARE_EVENT_TABLE()
    private:
        typedef std::map<cbWatch const*, WatchRawDialog*> Map;

        static Map s_dialogs;

        wxTextCtrl *m_text;
        Type        m_type;
};

WatchRawDialog::Map WatchRawDialog::s_dialogs;

BEGIN_EVENT_TABLE(WatchRawDialog, wxScrollingDialog)
    EVT_CLOSE(WatchRawDialog::OnClose)
END_EVENT_TABLE()


bool WatchRawDialogAdapter::DoShowDialog(wxPropertyGrid* WXUNUSED(propGrid), wxPGProperty* property)
{
    WatchesProperty *watch = static_cast<WatchesProperty*>(property);
    if (watch->GetWatch())
    {
        WatchRawDialog *dlg = WatchRawDialog::Create(watch);
        dlg->Show();
    }
    return false;
}

wxPGEditorDialogAdapter* WatchesProperty::GetEditorDialog() const
{
    return new WatchRawDialogAdapter();
}

class WatchesDropTarget : public wxTextDropTarget
{
public:
    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text)
    {
        cbDebuggerPlugin *activeDebugger = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
        cbWatch *watch = activeDebugger->AddWatch(text);
        if (watch)
            Manager::Get()->GetDebuggerManager()->GetWatchesDialog()->AddWatch(watch);
        // we return false here to veto the operation, otherwise the dragged text might get cut,
        // because we use wxDrag_DefaultMove in ScintillaWX::StartDrag (seems to happen only on windows)
        return false;
    }
private:
};

WatchesDlg::WatchesDlg() :
    wxPanel(Manager::Get()->GetAppWindow(), -1),
    m_append_empty_watch(false)
{
    wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);
    m_grid = new wxPropertyGrid(this, idGrid, wxDefaultPosition, wxDefaultSize,
                                wxPG_SPLITTER_AUTO_CENTER | wxTAB_TRAVERSAL);

    m_grid->SetExtraStyle(wxPG_EX_DISABLE_TLP_TRACKING | wxPG_EX_HELP_AS_TOOLTIPS);
    m_grid->SetDropTarget(new WatchesDropTarget);
    m_grid->SetColumnCount(3);
    bs->Add(m_grid, 1, wxEXPAND | wxALL);
    SetAutoLayout(TRUE);
    SetSizer(bs);

    wxPGRegisterEditorClass(cbTextCtrlAndButtonTooltip);

    m_grid->SetColumnProportion(0, 40);
    m_grid->SetColumnProportion(1, 40);
    m_grid->SetColumnProportion(2, 20);

    wxPGProperty *prop = m_grid->Append(new WatchesProperty(wxEmptyString, wxEmptyString, NULL));
    m_grid->SetPropertyAttribute(prop, wxT("Units"), wxEmptyString);
}

void AppendChildren(wxPropertyGrid &grid, wxPGProperty &property, cbWatch &watch)
{
    for(int ii = 0; ii < watch.GetChildCount(); ++ii)
    {
        cbWatch &child = *watch.GetChild(ii);

        wxString symbol, value, type;
        child.GetSymbol(symbol);
        child.GetValue(value);
        child.GetType(type);

        wxPGProperty *prop = new WatchesProperty(symbol, value, &child);
        prop->SetExpanded(child.IsExpanded());
        wxPGProperty *new_prop = grid.AppendIn(&property, prop);
        grid.SetPropertyAttribute(new_prop, wxT("Units"), type);
        if (value.empty())
            grid.SetPropertyHelpString(new_prop, wxEmptyString);
        else
            grid.SetPropertyHelpString(new_prop, symbol + wxT("=") + value);

        if(child.IsChanged())
        {
            grid.SetPropertyTextColour(prop, wxColor(255, 0, 0));
            WatchRawDialog::UpdateValue(static_cast<const WatchesProperty*>(prop));
        }
        else
            grid.SetPropertyColourToDefault(prop);

        AppendChildren(grid, *prop, child);
    }
}

void WatchesDlg::UpdateWatches()
{
    for(WatchItems::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        if(!it->property)
            continue;

        wxString value, symbol, type;
        it->watch->GetSymbol(symbol);
        it->watch->GetValue(value);
        it->property->SetValue(value);
        it->property->SetExpanded(it->watch->IsExpanded());
        it->watch->GetType(type);
        if(it->watch->IsChanged())
            m_grid->SetPropertyTextColour(it->property, wxColor(255, 0, 0));
        else
            m_grid->SetPropertyColourToDefault(it->property);
        m_grid->SetPropertyAttribute(it->property, wxT("Units"), type);
        if (value.empty())
            m_grid->SetPropertyHelpString(it->property, wxEmptyString);
        else
            m_grid->SetPropertyHelpString(it->property, symbol + wxT("=") + value);

        it->property->DeleteChildren();

        if(it->property->GetName() != symbol)
        {
            m_grid->SetPropertyName(it->property, symbol);
            m_grid->SetPropertyLabel(it->property, symbol);
        }

        AppendChildren(*m_grid, *it->property, *it->watch);

        WatchRawDialog::UpdateValue(static_cast<const WatchesProperty*>(it->property));
    }
    m_grid->Refresh();
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
        m_grid->Append(new WatchesProperty(wxEmptyString, wxEmptyString, NULL));
    }
    else
    {
        item.property = m_grid->Append(new WatchesProperty(symbol, value, watch));
    }

    item.property->SetExpanded(watch->IsExpanded());
    item.watch = watch;
    m_watches.push_back(item);
    m_grid->Refresh();
}

void WatchesDlg::OnExpand(wxPropertyGridEvent &event)
{
    WatchesProperty *prop = static_cast<WatchesProperty*>(event.GetProperty());
    prop->GetWatch()->Expand(true);

    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    cbAssert(plugin);
    plugin->ExpandWatch(prop->GetWatch());
}

void WatchesDlg::OnCollapse(wxPropertyGridEvent &event)
{
    WatchesProperty *prop = static_cast<WatchesProperty*>(event.GetProperty());
    prop->GetWatch()->Expand(false);

    cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
    cbAssert(plugin);
    plugin->CollapseWatch(prop->GetWatch());
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

    if (label == wxEmptyString)
        return;

    if (plugin && prop)
    {
        // if the user have edited existing watch, we replace it.
        if (prop->GetWatch())
        {
            cbWatch *old_watch = prop->GetWatch();
            prop->SetWatch(nullptr);
            plugin->DeleteWatch(old_watch);
            cbWatch *new_watch = plugin->AddWatch(label);
            prop->SetWatch(new_watch);

            for (WatchItems::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
            {
                if (it->property == prop)
                    it->watch = new_watch;
            }
            prop->SetExpanded(new_watch->IsExpanded());
            m_grid->Refresh();
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
    if (m_append_empty_watch)
    {
        wxPGProperty *new_prop = m_grid->Append(new WatchesProperty(wxEmptyString, wxEmptyString, NULL));
        m_grid->SelectProperty(new_prop, true);
        m_grid->Refresh();
        m_append_empty_watch = false;
    }
}

void WatchesDlg::OnPropertySelected(wxPropertyGridEvent &event)
{
    if (event.GetProperty() && event.GetPropertyLabel() == wxEmptyString)
        m_grid->BeginLabelEdit(0);
}

void WatchesDlg::DeleteProperty(WatchesProperty &prop)
{
    cbWatch *watch = prop.GetWatch();
    if (!watch)
        return;

    cbDebuggerPlugin *debugger = Manager::Get()->GetDebuggerManager()->GetDebuggerHavingWatch(watch);
    debugger->DeleteWatch(watch);

    wxPGProperty *parent = prop.GetParent();
    if(parent && parent->IsRoot())
    {
        for (WatchItems::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
        {
            if (!it->property)
                continue;
            if (it->property == &prop)
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

    if (!prop || !prop->GetParent() || !prop->GetParent()->IsRoot() || m_grid->GetLabelEditor())
        return;

    switch (event.GetKeyCode())
    {
        case WXK_DELETE:
            {
                wxPGProperty *prop_to_select = m_grid->GetNextSiblingProperty(watches_prop);

                DeleteProperty(*watches_prop);

                if (prop_to_select)
                    m_grid->SelectProperty(prop_to_select, false);
            }
            break;
        case WXK_INSERT:
            m_grid->BeginLabelEdit(0);
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
    if (!m_grid->GetLabelEditor())
    {
        m_grid->SetFocus();
        m_grid->BeginLabelEdit(0);
    }
}

void WatchesDlg::OnMenuProperties(wxCommandEvent &event)
{
    wxPGProperty *selected = m_grid->GetSelection();
    if (selected)
    {
        WatchesProperty *prop = static_cast<WatchesProperty*>(selected);
        cbWatch *watch = prop->GetWatch();
        if (watch)
        {
            cbDebuggerPlugin *debugger = Manager::Get()->GetDebuggerManager()->GetDebuggerHavingWatch(watch);
            if (debugger)
                debugger->ShowWatchProperties(watch);
        }
    }
}

void WatchesDlg::OnMenuDelete(wxCommandEvent &event)
{
    wxPGProperty *selected = m_grid->GetSelection();
    if (selected)
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
