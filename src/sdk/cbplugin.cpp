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
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/menu.h>
    #include "cbplugin.h"
    #include "sdk_events.h"
    #include "manager.h"
    #include "licenses.h" // license macros
    #include "projectbuildtarget.h"
    #include "cbproject.h"
    #include "messagemanager.h"
#endif

#include <wx/toolbar.h>


cbPlugin::cbPlugin()
    : m_Type(ptNone),
    m_IsAttached(false)
{
	SetEvtHandlerEnabled(false);
    m_PluginInfo.name = _T("");
    m_PluginInfo.title = _T("");
    m_PluginInfo.version = _T("");
    m_PluginInfo.description = _T("");
    m_PluginInfo.author = _T("");
    m_PluginInfo.authorEmail = _T("");
    m_PluginInfo.authorWebsite = _T("");
    m_PluginInfo.thanksTo = _T("");
    m_PluginInfo.license = LICENSE_GPL;
}

cbPlugin::~cbPlugin()
{
}

void cbPlugin::Attach()
{
	if (m_IsAttached)
		return;
    wxFrame* frame = Manager::Get()->GetAppWindow();
    if (frame)
    {
		// push ourself in the application's event handling chain...
        frame->PushEventHandler(this);

        CodeBlocksEvent event(cbEVT_PLUGIN_ATTACHED);
        event.SetPlugin(this);
        // post event in the host's event queue
        wxPostEvent(frame, event);

        m_IsAttached = true;
    }
	OnAttach();
	SetEvtHandlerEnabled(true);
}

void cbPlugin::Release(bool appShutDown)
{
	if (!m_IsAttached)
		return;
	m_IsAttached = false;
	SetEvtHandlerEnabled(false);
	OnRelease(appShutDown);

	if (appShutDown)
        return; // nothing more to do, if the app is shutting down

    wxFrame* frame = Manager::Get()->GetAppWindow();
    if (frame)
    {
        CodeBlocksEvent event(cbEVT_PLUGIN_RELEASED);
        event.SetPlugin(this);
        // ask the host to process this event immediately
        // it must be done this way, because if the host references
        // us (through event.GetEventObject()), we might not be valid at that time
        // (while, now, we are...)
        frame->ProcessEvent(event);

		// remove ourself from the application's event handling chain...
		frame->RemoveEventHandler(this);
    }
}

void cbPlugin::NotImplemented(const wxString& log)
{
    Manager::Get()->GetMessageManager()->DebugLog(_("%s : not implemented"), log.c_str());
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

cbDebuggerPlugin::cbDebuggerPlugin()
{
    m_Type = ptDebugger;
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
///// cbProjectWizardPlugin
/////

cbProjectWizardPlugin::cbProjectWizardPlugin()
{
    m_Type = ptProjectWizard;
}
