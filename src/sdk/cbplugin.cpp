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

void cbDebuggerPlugin::RemoveBreakpointFromEditor(const wxString& filename, int line)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->IsBuiltinOpen(filename);
    if (ed)
        ed->RemoveBreakpoint(line, false);
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
