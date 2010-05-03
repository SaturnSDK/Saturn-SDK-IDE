/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"
#ifndef CB_PRECOMP
    #include <wx/artprov.h>
    #include <wx/bmpbuttn.h>
    #include <wx/combobox.h>
    #include <wx/filedlg.h>
    #include <wx/frame.h>
    #include <wx/menu.h>
    #include <wx/sizer.h>
    #include <wx/stattext.h>

    #include "cbeditor.h"
    #include "cbexception.h"
    #include "cbplugin.h"
    #include "cbproject.h"
    #include "configmanager.h"
    #include "editormanager.h"
    #include "logmanager.h"
    #include "projectmanager.h"
#endif

#include <algorithm>
#include <wx/toolbar.h>

#include "debuggermanager.h"

#include "annoyingdialog.h"
#include "backtracedlg.h"
#include "breakpointsdlg.h"
#include "cpuregistersdlg.h"
#include "debuggermenu.h"
#include "disassemblydlg.h"
#include "examinememorydlg.h"
#include "loggers.h"
#include "manager.h"
#include "threadsdlg.h"
#include "watchesdlg.h"

cbBreakpoint::cbBreakpoint() :
    m_line(0),
    m_ignoreCount(0),
    m_type(Code),
    m_enabled(true),
    m_useIgnoreCount(false),
    m_useCondition(false)
{
}
cbBreakpoint::cbBreakpoint(const wxString &filename, int line) :
    m_filename(filename),
    m_line(line),
    m_ignoreCount(0),
    m_type(Code),
    m_enabled(true),
    m_useIgnoreCount(false),
    m_useCondition(false)
{
}

cbBreakpoint::cbBreakpoint(const wxString &dataExpression, bool breakOnRead, bool breakOnWrite) :
    m_type(Data),
    m_dataExpression(dataExpression),
    m_breakOnRead(breakOnRead),
    m_breakOnWrite(breakOnWrite)
{
}

void cbBreakpoint::SetLine(int line)
{
    m_line = line;
}

void cbBreakpoint::SetCondition(wxString const &condition)
{
    m_condition = condition;
}

void cbBreakpoint::SetIgnoreCount(int count)
{
    m_ignoreCount = count;
}

void cbBreakpoint::SetEnabled(bool flag)
{
    m_enabled = flag;
}

void cbBreakpoint::SetUseIgnoreCount(bool flag)
{
    m_useIgnoreCount = flag;
}

void cbBreakpoint::SetUseCondition(bool flag)
{
    m_useCondition = flag;
}

const wxString & cbBreakpoint::GetFilename() const
{
    return m_filename;
}

const wxString & cbBreakpoint::GetCondition() const
{
    return m_condition;
}

int cbBreakpoint::GetLine() const
{
    return m_line;
}

int cbBreakpoint::GetIgnoreCount() const
{
    return m_ignoreCount;
}

cbBreakpoint::Type cbBreakpoint::GetType() const
{
    return m_type;
}

bool cbBreakpoint::IsEnabled() const
{
    return m_enabled;
}

bool cbBreakpoint::UseIgnoreCount() const
{
    return m_useIgnoreCount;
}

bool cbBreakpoint::UseCondition() const
{
    return m_useCondition;
}

const wxString& cbBreakpoint::GetDataExpression() const
{
    return m_dataExpression;
}

bool cbBreakpoint::GetBreakOnRead() const
{
    return m_breakOnRead;
}

bool cbBreakpoint::GetBreakOnWrite() const
{
    return m_breakOnWrite;
}

cbWatch::cbWatch() :
    m_parent(NULL),
    m_changed(true),
    m_removed(false),
    m_expanded(false)
{
}

cbWatch::~cbWatch()
{
    for(PtrContainer::iterator it = m_children.begin(); it != m_children.end(); ++it)
        (*it)->Destroy();
    m_children.clear();
}

void cbWatch::Destroy()
{
    if (this)
        DoDestroy();
}

void cbWatch::AddChild(cbWatch *watch)
{
    watch->SetParent(this);
    m_children.push_back(watch);
}

void cbWatch::RemoveChild(int index)
{
    PtrContainer::iterator it = m_children.begin();
    std::advance(it, index);
    m_children.erase(it);
}

bool TestIfMarkedForRemoval(cbWatch *watch)
{
    if(watch->IsRemoved())
    {
        watch->Destroy();
        return true;
    }
    else
    {
        watch->RemoveMarkedChildren();
        return false;
    }
}

bool cbWatch::RemoveMarkedChildren()
{
    size_t start_size = m_children.size();
    PtrContainer::iterator new_last = std::remove_if(m_children.begin(), m_children.end(), &TestIfMarkedForRemoval);
    m_children.erase(new_last, m_children.end());

    return start_size != m_children.size();

}
void cbWatch::RemoveChildren()
{
    for(PtrContainer::iterator it = m_children.begin(); it != m_children.end(); ++it)
        (*it)->Destroy();
    m_children.clear();
}

int cbWatch::GetChildCount() const
{
    return m_children.size();
}

cbWatch* cbWatch::GetChild(int index)
{
    PtrContainer::iterator it = m_children.begin();
    std::advance(it, index);
    return *it;
}

const cbWatch* cbWatch::GetChild(int index) const
{
    PtrContainer::const_iterator it = m_children.begin();
    std::advance(it, index);
    return *it;
}

cbWatch* cbWatch::FindChild(const wxString& symbol)
{
    for (PtrContainer::iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
        wxString s;
        (*it)->GetSymbol(s);
        if(s == symbol)
            return *it;
    }
    return NULL;
}

int cbWatch::FindChildIndex(const wxString& symbol) const
{
    int index = 0;
    for (PtrContainer::const_iterator it = m_children.begin(); it != m_children.end(); ++it, ++index)
    {
        wxString s;
        (*it)->GetSymbol(s);
        if(s == symbol)
            return index;
    }
    return -1;
}

void cbWatch::SetParent(cbWatch *parent)
{
    m_parent = parent;
}

const cbWatch* cbWatch::GetParent() const
{
    return m_parent;
}

cbWatch* cbWatch::GetParent()
{
    return m_parent;
}

bool cbWatch::IsRemoved() const
{
    return m_removed;
}

bool cbWatch::IsChanged() const
{
    return m_changed;
}

void cbWatch::MarkAsRemoved(bool flag)
{
    m_removed = flag;
}

void cbWatch::MarkChildsAsRemoved()
{
    for(PtrContainer::iterator it = m_children.begin(); it != m_children.end(); ++it)
        (*it)->MarkAsRemoved(true);
}
void cbWatch::MarkAsChanged(bool flag)
{
    m_changed = flag;
}

void cbWatch::MarkAsChangedRecursive(bool flag)
{
    m_changed = flag;
    for(PtrContainer::iterator it = m_children.begin(); it != m_children.end(); ++it)
        (*it)->MarkAsChangedRecursive(flag);
}

bool cbWatch::IsExpanded() const
{
    return m_expanded;
}

void cbWatch::Expand(bool expand)
{
    m_expanded = expand;
}

cbWatch* DLLIMPORT GetRootWatch(cbWatch *watch)
{
    cbWatch *root = watch;
    while(root && root->GetParent())
        root = root->GetParent();
    return root;
}

cbStackFrame::cbStackFrame() :
    m_valid(false)
{
}

void cbStackFrame::SetNumber(int number)
{
    m_number = number;
}

void cbStackFrame::SetAddress(unsigned long int address)
{
    m_address = address;
}

void cbStackFrame::SetSymbol(const wxString& symbol)
{
    m_symbol = symbol;
}

void cbStackFrame::SetFile(const wxString& filename, const wxString &line)
{
    m_file = filename;
    m_line = line;
}

void cbStackFrame::MakeValid(bool flag)
{
    m_valid = flag;
}

int cbStackFrame::GetNumber() const
{
    return m_number;
}

unsigned long int cbStackFrame::GetAddress() const
{
    return m_address;
}

const wxString& cbStackFrame::GetSymbol() const
{
    return m_symbol;
}

const wxString& cbStackFrame::GetFilename() const
{
    return m_file;
}

const wxString& cbStackFrame::GetLine() const
{
    return m_line;
}

bool cbStackFrame::IsValid() const
{
    return m_valid;
}

cbThread::cbThread()
{
}

cbThread::cbThread(bool active, int number, const wxString& info)
{
    m_active = active;
    m_number = number;
    m_info = info;
}

bool cbThread::IsActive() const
{
    return m_active;
}

int cbThread::GetNumber() const
{
    return m_number;
}

const wxString& cbThread::GetInfo() const
{
    return m_info;
}


class DebugTextCtrlLogger : public TextCtrlLogger
{
public:
    DebugTextCtrlLogger(bool fixedPitchFont = false) :
        TextCtrlLogger(fixedPitchFont),
        m_panel(NULL)
    {
    }

    wxWindow* CreateTextCtrl(wxWindow *parent)
    {
        return TextCtrlLogger::CreateControl(parent);
    }

    virtual wxWindow* CreateControl(wxWindow* parent);

private:
    wxPanel     *m_panel;
};

class DebugLogPanel : public wxPanel
{
public:
    DebugLogPanel(wxWindow *parent, DebugTextCtrlLogger *text_control_logger) :
        wxPanel(parent),
        m_text_control_logger(text_control_logger)
    {
        int idDebug_LogEntryControl = wxNewId();
        int idDebug_ExecuteButton = wxNewId();
        int idDebug_ClearButton = wxNewId();
        int idDebug_LoadButton = wxNewId();

        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer *control_sizer = new wxBoxSizer(wxHORIZONTAL);

        wxWindow *text_control = text_control_logger->CreateTextCtrl(this);
        sizer->Add(text_control, wxEXPAND, wxEXPAND | wxALL , 0);
        sizer->Add(control_sizer, 0, wxEXPAND | wxALL, 0);

        wxStaticText *label = new wxStaticText(this, wxID_ANY, _T("Command:"),
                                               wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);

        m_command_entry = new wxComboBox(this, idDebug_LogEntryControl, wxEmptyString,
                                         wxDefaultPosition, wxDefaultSize, 0, 0,
                                         wxCB_DROPDOWN | wxTE_PROCESS_ENTER);

        wxBitmap execute_bitmap = wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_EXECUTABLE_FILE")),
                                                           wxART_BUTTON);
        wxBitmap clear_bitmap = wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DELETE")),wxART_BUTTON);
        wxBitmap file_open_bitmap =wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_OPEN")),
                                                            wxART_BUTTON);

        wxBitmapButton *button_execute;
        button_execute = new wxBitmapButton(this, idDebug_ExecuteButton, execute_bitmap, wxDefaultPosition,
                                            wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator,
                                            _T("idDebug_ExecuteButton"));
        button_execute->SetToolTip(_("Execute current command"));

        wxBitmapButton *button_load = new wxBitmapButton(this, idDebug_LoadButton, file_open_bitmap, wxDefaultPosition,
                                                         wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator,
                                                         _T("idDebug_LoadButton"));
        button_load->SetDefault();
        button_load->SetToolTip(_("Load from file"));

        wxBitmapButton *button_clear = new wxBitmapButton(this, idDebug_ClearButton, clear_bitmap, wxDefaultPosition,
                                                          wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator,
                                                          _T("idDebug_ClearButton"));
        button_clear->SetDefault();
        button_clear->SetToolTip(_("Clear output window"));

        control_sizer->Add(label, 0, wxALIGN_CENTER | wxALL, 2);
        control_sizer->Add(m_command_entry, wxEXPAND, wxEXPAND | wxALL, 2);
        control_sizer->Add(button_execute, 0, wxEXPAND | wxALL, 0);
        control_sizer->Add(button_load, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
        control_sizer->Add(button_clear, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

        SetSizer(sizer);

        Connect(idDebug_LogEntryControl,
                wxEVT_COMMAND_TEXT_ENTER,
                wxObjectEventFunction(&DebugLogPanel::OnEntryCommand));
        Connect(idDebug_ExecuteButton,
                wxEVT_COMMAND_BUTTON_CLICKED,
                wxObjectEventFunction(&DebugLogPanel::OnEntryCommand));
        Connect(idDebug_ClearButton,
                wxEVT_COMMAND_BUTTON_CLICKED,
                wxObjectEventFunction(&DebugLogPanel::OnClearLog));
        Connect(idDebug_LoadButton,
                wxEVT_COMMAND_BUTTON_CLICKED,
                wxObjectEventFunction(&DebugLogPanel::OnLoadFile));

    }

    void OnEntryCommand(wxCommandEvent& event)
    {
        assert(m_command_entry);
        wxString cmd = m_command_entry->GetValue();
        cmd.Trim(false);
        cmd.Trim(true);

        if (cmd.IsEmpty())
            return;
        cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
        if (plugin)
        {
            plugin->SendCommand(cmd);

            if (m_command_entry->FindString(cmd) == wxNOT_FOUND)
                m_command_entry->Insert(cmd, 0);
            m_command_entry->SetValue(wxEmptyString);
        }
    }

    void OnClearLog(wxCommandEvent& event)
    {
        assert(m_command_entry);
        assert(m_text_control_logger);
        m_text_control_logger->Clear();
        m_command_entry->SetFocus();
    }

    void OnLoadFile(wxCommandEvent& event)
    {
        cbDebuggerPlugin *plugin = Manager::Get()->GetDebuggerManager()->GetActiveDebugger();
        if (!plugin)
            return;

        ConfigManager* manager = Manager::Get()->GetConfigManager(_T("app"));
        wxString path = manager->Read(_T("/file_dialogs/file_run_dbg_script/directory"), wxEmptyString);

        wxFileDialog dialog(this, _("Load script"), path, wxEmptyString,
                            _T("Debugger script files (*.gdb)|*.gdb"), wxFD_OPEN | compatibility::wxHideReadonly);

        if (dialog.ShowModal() == wxID_OK)
        {
            manager->Write(_T("/file_dialogs/file_run_dbg_script/directory"), dialog.GetDirectory());

            plugin->SendCommand(_T("source ") + dialog.GetPath());
        }
    }
private:
    DebugTextCtrlLogger *m_text_control_logger;
    wxComboBox  *m_command_entry;
};

wxWindow* DebugTextCtrlLogger::CreateControl(wxWindow* parent)
{
    if(!m_panel)
        m_panel = new DebugLogPanel(parent, this);

    return m_panel;
}

template<> DebuggerManager* Mgr<DebuggerManager>::instance = 0;
template<> bool  Mgr<DebuggerManager>::isShutdown = false;

DebuggerManager::DebuggerManager() :
    m_activeDebugger(NULL),
    m_menuHandler(new DebuggerMenuHandler),
    m_toolbarHandler(new DebuggerToolbarHandler),
    m_backtraceDialog(NULL),
    m_breakPointsDialog(NULL),
    m_cpuRegistersDialog(NULL),
    m_disassemblyDialog(NULL),
    m_examineMemoryDialog(NULL),
    m_threadsDialog(NULL),
    m_watchesDialog(NULL),
    m_logger(NULL),
    m_debugLogger(NULL),
    m_loggerIndex(-1),
    m_debugLoggerIndex(-1)
{
    m_menuHandler->SetEvtHandlerEnabled(false);
    m_toolbarHandler->SetEvtHandlerEnabled(false);
    wxWindow* window = Manager::Get()->GetAppWindow();
    if (window)
    {
        window->PushEventHandler(m_menuHandler);
        window->PushEventHandler(m_toolbarHandler);
    }
    m_menuHandler->SetEvtHandlerEnabled(true);
    m_toolbarHandler->SetEvtHandlerEnabled(true);
}

DebuggerManager::~DebuggerManager()
{
}

bool DebuggerManager::RegisterDebugger(cbDebuggerPlugin *plugin, wxString const &name)
{
    RegisteredPlugins::const_iterator it = m_registered.find(plugin);
    if (it != m_registered.end())
        return false;

    PluginData data;
    data.name = name;
    data.menu_id = wxNewId();

    m_registered[plugin] = data;

    ConfigManager &config = *Manager::Get()->GetConfigManager(_T("debugger_common"));
    wxString active_debugger_name = config.Read(wxT("active_debugger"), wxEmptyString);

    if (!m_activeDebugger || active_debugger_name == name)
    {
        m_activeDebugger = plugin;
        m_menuHandler->SetActiveDebugger(m_activeDebugger);
    }
    RebuildActiveDebuggersMenu();
    return true;
}

void RemoveDockWindow(wxWindow *window)
{
    if (window)
    {
        CodeBlocksDockEvent evt(cbEVT_REMOVE_DOCK_WINDOW);
        evt.pWindow = window;
        Manager::Get()->ProcessEvent(evt);
        window->Destroy();
    }
}

bool DebuggerManager::UnregisterDebugger(cbDebuggerPlugin *plugin)
{
    RegisteredPlugins::iterator it = m_registered.find(plugin);
    if(it == m_registered.end())
        return false;

    m_registered.erase(it);
    if (plugin == m_activeDebugger)
    {
        if (m_registered.empty())
            m_activeDebugger = NULL;
        else
            m_activeDebugger = m_registered.begin()->first;
    }
    RebuildActiveDebuggersMenu();

    if (m_registered.empty())
    {
        RemoveDockWindow(m_backtraceDialog);
        m_backtraceDialog = NULL;

        RemoveDockWindow(m_breakPointsDialog);
        m_breakPointsDialog = NULL;

        RemoveDockWindow(m_cpuRegistersDialog);
        m_cpuRegistersDialog = NULL;

        RemoveDockWindow(m_disassemblyDialog);
        m_disassemblyDialog = NULL;

        RemoveDockWindow(m_examineMemoryDialog);
        m_examineMemoryDialog = NULL;

        RemoveDockWindow(m_threadsDialog);
        m_threadsDialog = NULL;

        RemoveDockWindow(m_watchesDialog);
        m_watchesDialog = NULL;
    }

    return true;
}

wxMenu* DebuggerManager::GetMenu()
{
    wxMenuBar *menuBar = Manager::Get()->GetAppFrame()->GetMenuBar();
    cbAssert(menuBar);
    wxMenu *menu = NULL;

    int menu_pos = menuBar->FindMenu(_("&Debug"));

    if(menu_pos != wxNOT_FOUND)
        menu = menuBar->GetMenu(menu_pos);

    if (!menu)
    {
        menu = Manager::Get()->LoadMenu(_T("debugger_menu"),true);

        // ok, now, where do we insert?
        // three possibilities here:
        // a) locate "Compile" menu and insert after it
        // b) locate "Project" menu and insert after it
        // c) if not found (?), insert at pos 5
        int finalPos = 5;
        int projcompMenuPos = menuBar->FindMenu(_("&Build"));
        if (projcompMenuPos == wxNOT_FOUND)
            projcompMenuPos = menuBar->FindMenu(_("&Compile"));

        if (projcompMenuPos != wxNOT_FOUND)
            finalPos = projcompMenuPos + 1;
        else
        {
            projcompMenuPos = menuBar->FindMenu(_("&Project"));
            if (projcompMenuPos != wxNOT_FOUND)
                finalPos = projcompMenuPos + 1;
        }
        menuBar->Insert(finalPos, menu, _("&Debug"));

        RebuildActiveDebuggersMenu();
//        m_menu->Insert(0,
    }
    return menu;
}

void DebuggerManager::BuildContextMenu(wxMenu &menu, const wxString& word_at_caret, bool is_running)
{
    m_menuHandler->BuildContextMenu(menu, word_at_caret, is_running);
}

wxToolBar* DebuggerManager::GetToolbar(bool create)
{
    return m_toolbarHandler->GetToolbar(create);
}

TextCtrlLogger* DebuggerManager::GetLogger(bool for_debug, int &index)
{
    LogManager* msgMan = Manager::Get()->GetLogManager();

    if (for_debug)
    {
        if (!m_debugLogger)
        {
            m_debugLogger = new DebugTextCtrlLogger(true);
            m_debugLoggerIndex = msgMan->SetLog(m_debugLogger);
            LogSlot &slot = msgMan->Slot(m_debugLoggerIndex);

            slot.title = _("Debugger (debug)");
            // set log image
            wxString prefix = ConfigManager::GetDataFolder() + _T("/images/");
            wxBitmap* bmp = new wxBitmap(cbLoadBitmap(prefix + _T("contents_16x16.png"), wxBITMAP_TYPE_PNG));
            slot.icon = bmp;

            CodeBlocksLogEvent evtAdd(cbEVT_ADD_LOG_WINDOW, m_debugLogger, slot.title, slot.icon);
            Manager::Get()->ProcessEvent(evtAdd);
        }
        index = m_debugLoggerIndex;
        return m_debugLogger;
    }
    else
    {
        if (!m_logger)
        {
            m_logger = new DebugTextCtrlLogger(true);
            m_loggerIndex = msgMan->SetLog(m_logger);
            LogSlot &slot = msgMan->Slot(m_loggerIndex);
            slot.title = _("Debugger");
            // set log image
            wxString prefix = ConfigManager::GetDataFolder() + _T("/images/");
            wxBitmap* bmp = new wxBitmap(cbLoadBitmap(prefix + _T("misc_16x16.png"), wxBITMAP_TYPE_PNG));
            slot.icon = bmp;

            CodeBlocksLogEvent evtAdd(cbEVT_ADD_LOG_WINDOW, m_logger, slot.title, slot.icon);
            Manager::Get()->ProcessEvent(evtAdd);
        }

        index = m_loggerIndex;
        return m_logger;
    }
}

TextCtrlLogger* DebuggerManager::GetLogger(bool for_debug)
{
    int index;
    return GetLogger(for_debug, index);
}

void DebuggerManager::HideLogger(bool for_debug)
{
    if (for_debug)
    {
        CodeBlocksLogEvent evt(cbEVT_REMOVE_LOG_WINDOW, m_debugLogger);
        Manager::Get()->ProcessEvent(evt);
        m_debugLogger = NULL;
        m_debugLoggerIndex = -1;
    }
    else
    {
        CodeBlocksLogEvent evt(cbEVT_REMOVE_LOG_WINDOW, m_logger);
        Manager::Get()->ProcessEvent(evt);
        m_logger = NULL;
        m_loggerIndex = -1;
    }
}

bool DebuggerManager::RebuildActiveDebuggersMenu()
{
    wxMenu* debug_menu = GetMenu();
    if (!debug_menu)
        return true;
    int item_id = debug_menu->FindItem(_("&Active debuggers"));
    wxMenuItem *menuitem = debug_menu->FindItem(item_id, NULL);
    if (!menuitem)
        return false;

    wxMenu *menu = menuitem->GetSubMenu();
    if (!menu)
        return false;
    while (menu->GetMenuItemCount() > 0)
    {
        wxMenuItem *item = menu->FindItemByPosition(0);
        menu->Destroy(item);
    }

    for (RegisteredPlugins::const_iterator it = m_registered.begin(); it != m_registered.end(); ++it)
    {
        PluginData const &data = it->second;
        menu->AppendRadioItem(data.menu_id, data.name);

        m_menuHandler->Connect(data.menu_id, -1, wxEVT_COMMAND_MENU_SELECTED,
                               (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
                               &DebuggerMenuHandler::OnActiveDebuggerClick);
    }

    if (m_activeDebugger)
    {
        RegisteredPlugins::const_iterator it = m_registered.find(m_activeDebugger);
        cbAssert(it != m_registered.end());

        menu->Check(it->second.menu_id, true);
    }

    return true;
}

cbBacktraceDlg* DebuggerManager::GetBacktraceDialog()
{
    if (!m_backtraceDialog)
    {
        m_backtraceDialog = new cbBacktraceDlg(Manager::Get()->GetAppWindow());

        CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
        evt.name = _T("CallStackPane");
        evt.title = _("Call stack");
        evt.pWindow = m_backtraceDialog;
        evt.dockSide = CodeBlocksDockEvent::dsFloating;
        evt.desiredSize.Set(150, 150);
        evt.floatingSize.Set(450, 150);
        evt.minimumSize.Set(150, 150);
        Manager::Get()->ProcessEvent(evt);
    }
    return m_backtraceDialog;
}

cbBreakpointsDlg* DebuggerManager::GetBreakpointDialog()
{
    if (!m_breakPointsDialog)
    {
        m_breakPointsDialog = new cbBreakpointsDlg;

        CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
        evt.name = _T("BreakpointsPane");
        evt.title = _("Breakpoints");
        evt.pWindow = m_breakPointsDialog;
        evt.dockSide = CodeBlocksDockEvent::dsFloating;
        evt.desiredSize.Set(350, 250);
        evt.floatingSize.Set(350, 250);
        evt.minimumSize.Set(150, 150);
        Manager::Get()->ProcessEvent(evt);
    }
    return m_breakPointsDialog;
}

cbCPURegistersDlg* DebuggerManager::GetCPURegistersDialog()
{
    if (!m_cpuRegistersDialog)
    {
        m_cpuRegistersDialog = new cbCPURegistersDlg(Manager::Get()->GetAppWindow());

        CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
        evt.name = _T("CPURegistersPane");
        evt.title = _("CPU Registers");
        evt.pWindow = m_cpuRegistersDialog;
        evt.dockSide = CodeBlocksDockEvent::dsFloating;
        evt.desiredSize.Set(350, 250);
        evt.floatingSize.Set(350, 250);
        evt.minimumSize.Set(150, 150);
        Manager::Get()->ProcessEvent(evt);
    }
    return m_cpuRegistersDialog;
}

cbDisassemblyDlg* DebuggerManager::GetDisassemblyDialog()
{
    if (!m_disassemblyDialog)
    {
        m_disassemblyDialog = new cbDisassemblyDlg(Manager::Get()->GetAppWindow());
        CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);

        evt.name = _T("DisassemblyPane");
        evt.title = _("Disassembly");
        evt.pWindow = m_disassemblyDialog;
        evt.dockSide = CodeBlocksDockEvent::dsFloating;
        evt.desiredSize.Set(350, 250);
        evt.floatingSize.Set(350, 250);
        evt.minimumSize.Set(150, 150);
        Manager::Get()->ProcessEvent(evt);
    }

    return m_disassemblyDialog;
}

cbExamineMemoryDlg* DebuggerManager::GetExamineMemoryDialog()
{
    if (!m_examineMemoryDialog)
    {
        m_examineMemoryDialog = new cbExamineMemoryDlg(Manager::Get()->GetAppWindow());

        CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
        evt.name = _T("ExamineMemoryPane");
        evt.title = _("Memory");
        evt.pWindow = m_examineMemoryDialog;
        evt.dockSide = CodeBlocksDockEvent::dsFloating;
        evt.desiredSize.Set(450, 250);
        evt.floatingSize.Set(450, 250);
        evt.minimumSize.Set(350, 150);
        Manager::Get()->ProcessEvent(evt);
    }
    return m_examineMemoryDialog;
}

cbThreadsDlg* DebuggerManager::GetThreadsDialog()
{
    if (!m_threadsDialog)
    {
        m_threadsDialog = new cbThreadsDlg(Manager::Get()->GetAppWindow());

        CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
        evt.name = _T("ThreadsPane");
        evt.title = _("Running threads");
        evt.pWindow = m_threadsDialog;
        evt.dockSide = CodeBlocksDockEvent::dsFloating;
        evt.desiredSize.Set(350, 75);
        evt.floatingSize.Set(450, 75);
        evt.minimumSize.Set(250, 75);
        Manager::Get()->ProcessEvent(evt);
    }

    return m_threadsDialog;
}

WatchesDlg* DebuggerManager::GetWatchesDialog()
{
    if (!m_watchesDialog)
    {
        m_watchesDialog = new WatchesDlg;
        CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
        evt.name = _T("WatchesPane");
        evt.title = _("Watches (new)");
        evt.pWindow = m_watchesDialog;
        evt.dockSide = CodeBlocksDockEvent::dsFloating;
        evt.desiredSize.Set(150, 250);
        evt.floatingSize.Set(150, 250);
        evt.minimumSize.Set(150, 150);
        Manager::Get()->ProcessEvent(evt);
    }

    return m_watchesDialog;
}

bool DebuggerManager::ShowBacktraceDialog()
{
    cbBacktraceDlg *dialog = GetBacktraceDialog();

    if (!IsWindowReallyShown(dialog))
    {
        // show the backtrace window
        CodeBlocksDockEvent evt(cbEVT_SHOW_DOCK_WINDOW);
        evt.pWindow = dialog;
        Manager::Get()->ProcessEvent(evt);
        return true;
    }
    else
        return false;
}

bool DebuggerManager::UpdateBacktrace()
{
    return m_backtraceDialog && IsWindowReallyShown(m_backtraceDialog);
}

bool DebuggerManager::UpdateCPURegisters()
{
    return m_cpuRegistersDialog && IsWindowReallyShown(m_cpuRegistersDialog);
}

bool DebuggerManager::UpdateDisassembly()
{
    return m_disassemblyDialog && IsWindowReallyShown(m_disassemblyDialog);
}

bool DebuggerManager::UpdateExamineMemory()
{
    return m_examineMemoryDialog && IsWindowReallyShown(m_examineMemoryDialog);
}

bool DebuggerManager::UpdateThreads()
{
    return m_threadsDialog && IsWindowReallyShown(m_threadsDialog);
}

cbDebuggerPlugin* DebuggerManager::GetDebuggerHavingWatch(cbWatch *watch)
{
    watch = GetRootWatch(watch);
    for (RegisteredPlugins::iterator it = m_registered.begin(); it != m_registered.end(); ++it)
    {
        if (it->first->HasWatch(watch))
            return it->first;
    }
    return NULL;
}

DebuggerManager::RegisteredPlugins const & DebuggerManager::GetAllDebuggers() const
{
    return m_registered;
}
DebuggerManager::RegisteredPlugins & DebuggerManager::GetAllDebuggers()
{
    return m_registered;
}
cbDebuggerPlugin* DebuggerManager::GetActiveDebugger()
{
    return m_activeDebugger;
}

void DebuggerManager::SetActiveDebugger(cbDebuggerPlugin* activeDebugger)
{
    if (m_activeDebugger && m_activeDebugger->IsRunning())
    {
        AnnoyingDialog dlg(_("Debugger is running"),
                           _("Can't change the active debugger while it is running!"),
                           wxART_WARNING, AnnoyingDialog::OK, wxID_OK);
        dlg.ShowModal();
        return;
    }

    RegisteredPlugins::const_iterator it = m_registered.find(activeDebugger);
    cbAssert(it != m_registered.end());

    ConfigManager &config = *Manager::Get()->GetConfigManager(_T("debugger_common"));
    config.Write(wxT("active_debugger"), it->second.name);

    m_activeDebugger = activeDebugger;
    m_menuHandler->SetActiveDebugger(activeDebugger);
}

DebuggerManager::SyncEditorResult DebuggerManager::SyncEditor(const wxString& filename, int line, bool setMarker)
{
    if (setMarker)
    {
        //  was in ClearActiveMarkFromAllEditors

        EditorManager* edMan = Manager::Get()->GetEditorManager();
        for (int i = 0; i < edMan->GetEditorsCount(); ++i)
        {
            cbEditor* ed = edMan->GetBuiltinEditor(i);
            if (ed)
                ed->SetDebugLine(-1);
        }
    }
    FileType ft = FileTypeOf(filename);
    if (ft != ftSource && ft != ftHeader && ft != ftResource)
        return SyncFileUnknown; // don't try to open unknown files

    cbProject* project = Manager::Get()->GetProjectManager()->GetActiveProject();
    ProjectFile* f = project ? project->GetFileByFilename(filename, false, true) : 0;

    wxString unixfilename = UnixFilename(filename);
    wxFileName fname(unixfilename);

    if (project && fname.IsRelative())
        fname.MakeAbsolute(project->GetBasePath());

    // gdb can't work with spaces in filenames, so we have passed it the shorthand form (C:\MYDOCU~1 etc)
    // revert this change now so the file can be located and opened...
    // we do this by calling GetLongPath()
    cbEditor* ed = Manager::Get()->GetEditorManager()->Open(fname.GetLongPath());
    if (ed)
    {
        ed->Show(true);
        if (f && !ed->GetProjectFile())
            ed->SetProjectFile(f);
        ed->GotoLine(line - 1, false);
        if (setMarker)
            ed->SetDebugLine(line - 1);
        return SyncOk;
    }
    else
        return SyncFileNotFound;
}
