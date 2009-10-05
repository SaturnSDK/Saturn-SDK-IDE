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
    #include <wx/frame.h> // wxFrame
    #include <wx/menu.h>
    #include "cbplugin.h"
    #include "compiler.h" // GetSwitches
    #include "sdk_events.h"
    #include "manager.h"
    #include "projectbuildtarget.h"
    #include "cbproject.h"
    #include "logmanager.h"
#endif

#include <wx/toolbar.h>
#include "breakpointsdlg.h"
#include "cbstyledtextctrl.h"


cbPlugin::cbPlugin()
    : m_Type(ptNone),
    m_IsAttached(false)
{
	SetEvtHandlerEnabled(false);
}

cbPlugin::~cbPlugin()
{
}

void cbPlugin::Attach()
{
	if (m_IsAttached)
		return;
    wxWindow* window = Manager::Get()->GetAppWindow();
    if (window)
    {
		// push ourself in the application's event handling chain...
        window->PushEventHandler(this);
    }
    m_IsAttached = true;
	OnAttach();
	SetEvtHandlerEnabled(true);

	CodeBlocksEvent event(cbEVT_PLUGIN_ATTACHED);
	event.SetPlugin(this);
	// post event in the host's event queue
	Manager::Get()->ProcessEvent(event);
}

void cbPlugin::Release(bool appShutDown)
{
	if (!m_IsAttached)
		return;
	m_IsAttached = false;
	SetEvtHandlerEnabled(false);
	OnRelease(appShutDown);

	CodeBlocksEvent event(cbEVT_PLUGIN_RELEASED);
	event.SetPlugin(this);
	// ask the host to process this event immediately
	// it must be done this way, because if the host references
	// us (through event.GetEventObject()), we might not be valid at that time
	// (while, now, we are...)
	Manager::Get()->ProcessEvent(event);

	if (appShutDown)
        return; // nothing more to do, if the app is shutting down

    wxWindow* window = Manager::Get()->GetAppWindow();
    if (window)
    {
		// remove ourself from the application's event handling chain...
		window->RemoveEventHandler(this);
    }
}

void cbPlugin::NotImplemented(const wxString& log) const
{
    Manager::Get()->GetLogManager()->DebugLog(log + _T(" : not implemented"));
}

/////
///// cbCompilerPlugin
/////

cbCompilerPlugin::cbCompilerPlugin()
{
    m_Type = ptCompiler;
}

/////
///// cbDebuggerPlugin
/////

cbDebuggerPlugin::cbDebuggerPlugin() :
    m_toolbar(NULL)
{
    m_Type = ptDebugger;
}


void cbDebuggerPlugin::OnAttach()
{
    OnAttachReal();
    typedef cbEventFunctor<cbDebuggerPlugin, CodeBlocksEvent> EventOpened;

    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_OPEN, new EventOpened(this, &cbDebuggerPlugin::OnEditorOpened));
}

void cbDebuggerPlugin::BuildMenu(wxMenuBar* menuBar)
{
    if (!IsAttached())
        return;
    Manager::Get()->GetDebuggerManager()->GetMenu();
}

wxString cbDebuggerPlugin::GetEditorWordAtCaret()
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return _T("");
    int start = ed->GetControl()->WordStartPosition(ed->GetControl()->GetCurrentPos(), true);
    int end = ed->GetControl()->WordEndPosition(ed->GetControl()->GetCurrentPos(), true);
    return ed->GetControl()->GetTextRange(start, end);
}

void cbDebuggerPlugin::BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (!prj)
        return;
    if (!IsAttached())
        return;
    // we 're only interested in editor menus
    // we 'll add a "debug watches" entry only when the debugger is running...
    if (type != mtEditorManager || !menu)
        return;

    wxString word;
    if (IsRunning())
    {
        // has to have a word under the caret...
        word = GetEditorWordAtCaret();
    }
    Manager::Get()->GetDebuggerManager()->BuildContextMenu(*menu, word, IsRunning());
}

bool cbDebuggerPlugin::BuildToolBar(wxToolBar* toolBar)
{
    if (!IsAttached() || !toolBar)
        return false;
    Manager::Get()->GetDebuggerManager()->LoadToolbar(toolBar);
    m_toolbar = toolBar;
    return true;
}

wxToolBar* cbDebuggerPlugin::GetToolbar()
{
    return m_toolbar;
}

void cbDebuggerPlugin::ClearActiveMarkFromAllEditors()
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    for (int i = 0; i < edMan->GetEditorsCount(); ++i)
    {
        cbEditor* ed = edMan->GetBuiltinEditor(i);
        if (ed)
            ed->SetDebugLine(-1);
    }
}

bool HasBreakpoint(cbDebuggerPlugin &plugin, wxString const &filename, int line)
{
    int count = plugin.GetBreakpointsCount();
    for (int ii = 0; ii < count; ++ii)
    {
        const cbBreakpoint* b = plugin.GetBreakpoint(ii);

        if (b->GetFilename() == filename && b->GetLine() == line)
            return true;
    }
    return false;
}

void cbDebuggerPlugin::EditorLinesAddedOrRemoved(cbEditor* editor, int startline, int lines)
{
    // here we keep the breakpoints in sync with the editors
    // (whenever lines are added or removed)
    if (!editor || lines == 0)
        return;

    const wxString& filename = editor->GetFilename();

    std::vector<int> breakpoints_for_file;
    int count = GetBreakpointsCount();
    for (int ii = 0; ii < count; ++ii)
    {
        const cbBreakpoint* b = GetBreakpoint(ii);

        if (b->GetFilename() == filename)
        {
            breakpoints_for_file.push_back(ii);
        }
    }

    if (lines < 0)
    {
        // removed lines
        // make "lines" positive, for easier reading below
        lines = -lines;
        int endline = startline + lines - 1;

        std::vector<cbBreakpoint*> to_remove;

        for (std::vector<int>::iterator it = breakpoints_for_file.begin(); it != breakpoints_for_file.end(); ++it)
        {
            cbBreakpoint* b = GetBreakpoint(*it);
            if (b->GetLine() > endline)
                ShiftBreakpoint(*it, -lines);
            else if (b->GetLine() >= startline && b->GetLine() <= endline)
                to_remove.push_back(b);
        }

        for (std::vector<cbBreakpoint*>::iterator it = to_remove.begin(); it != to_remove.end(); ++it)
            DeleteBreakpoint(*it);

        // special case:
        // when deleting a block of lines, if these lines contain at least one marker,
        // one marker is retained at the cursor position.
        // In our case here, this means that all breakpoints will be deleted in the range
        // but one "orphan" breakpoint (i.e. editor mark only, no actual breakpoint behind it)
        // will be visible on the line with the cursor.
        //
        // If we really have an "orphan", we remove it.
        bool is_orphan = !HasBreakpoint(*this, editor->GetFilename(), endline - lines + 1);
        if (is_orphan)
            editor->RemoveBreakpoint(endline - lines, false);
    }
    else
    {
        for (std::vector<int>::iterator it = breakpoints_for_file.begin(); it != breakpoints_for_file.end(); ++it)
        {
            cbBreakpoint* b = GetBreakpoint(*it);
            if (b->GetLine() > startline)
                ShiftBreakpoint(*it, lines);
        }
    }
    cbBreakpointsDlg *dlg = Manager::Get()->GetDebuggerManager()->GetBreakpointDialog();
    dlg->Reload();
}

void cbDebuggerPlugin::OnEditorOpened(CodeBlocksEvent& event)
{
    // when an editor opens, look if we have breakpoints for it
    // and notify it...
    EditorBase* ed = event.GetEditor();
    if (ed)
    {
        wxFileName bpFileName, edFileName;

        edFileName.Assign(ed->GetFilename());
        edFileName.Normalize();

        int count = GetBreakpointsCount();
        for (int ii = 0; ii < count; ++ii)
        {
            const cbBreakpoint* breakpoint = GetBreakpoint(ii);

            bpFileName.Assign(breakpoint->GetFilename());
            bpFileName.Normalize();

            if (bpFileName.GetFullPath().Matches(edFileName.GetFullPath()))
                ed->ToggleBreakpoint(breakpoint->GetLine() - 1, false);
        }

        if (IsRunning())
        {
            wxString filename;
            int line;
            GetCurrentPosition(filename, line);

            wxFileName dbgFileName(filename);
            dbgFileName.Normalize();
            if (dbgFileName.GetFullPath().IsSameAs(edFileName.GetFullPath()) && line != -1)
            {
                ed->SetDebugLine(line - 1);
            }
        }
    }
    event.Skip(); // must do
}

/////
///// cbToolPlugin
/////

cbToolPlugin::cbToolPlugin()
{
    m_Type = ptTool;
}

/////
///// cbMimePlugin
/////

cbMimePlugin::cbMimePlugin()
{
    m_Type = ptMime;
}

/////
///// cbCodeCompletionPlugin
/////

cbCodeCompletionPlugin::cbCodeCompletionPlugin()
{
    m_Type = ptCodeCompletion;
}

/////
///// cbWizardPlugin
/////

cbWizardPlugin::cbWizardPlugin()
{
    m_Type = ptWizard;
}
