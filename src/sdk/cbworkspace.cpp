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

#include "cbworkspace.h"
#include "globals.h"
#include "manager.h"
#include "messagemanager.h"
#include "workspaceloader.h"
#include "msvcworkspaceloader.h"
#include "msvc7workspaceloader.h"
#include <wx/filedlg.h>
#include <wx/intl.h>

cbWorkspace::cbWorkspace(const wxString& filename)
{
	SC_CONSTRUCTOR_BEGIN
	//ctor
    if (filename.Matches(DEFAULT_WORKSPACE))
	{
        wxString tmp;
        // if no filename given, use the default workspace
        tmp = wxGetHomeDir();
        tmp << "/.CodeBlocks";
        if (!wxDirExists(tmp))
            wxMkdir(tmp, 0755);
        tmp << "/" << DEFAULT_WORKSPACE;
        m_Filename = tmp;
        m_IsDefault = true;
	}
	else
	{
        m_Filename = filename;
        m_IsDefault = false;
    }
    Load();
}

cbWorkspace::~cbWorkspace()
{
	SC_DESTRUCTOR_BEGIN
	//dtor
	SC_DESTRUCTOR_END
}

void cbWorkspace::Load()
{
    SANITY_CHECK();
    wxString fname = m_Filename.GetFullPath();
    if (fname.IsEmpty())
        return;
	Manager::Get()->GetMessageManager()->DebugLog("Loading workspace \"%s\"", fname.c_str());
	
	bool modified = false;
	IBaseWorkspaceLoader* pWsp = 0;
	switch (FileTypeOf(fname))
	{
        case ftCodeBlocksWorkspace: pWsp = new WorkspaceLoader; modified = false; break;
        case ftMSVCWorkspace: pWsp = new MSVCWorkspaceLoader; modified = true; break;
        case ftMSVSWorkspace: pWsp = new MSVC7WorkspaceLoader; modified = true; break;
        default: break;
    }
	m_IsOK = pWsp && (pWsp->Open(fname) || m_IsDefault);
	SANITY_CHECK();
	m_Title = pWsp ? pWsp->GetTitle() : wxString(wxEmptyString);
	SANITY_CHECK();
    
    SANITY_CHECK();
    m_Filename.SetExt(WORKSPACE_EXT);
    SetModified(modified);
    
    if (pWsp)
        delete pWsp;
}

bool cbWorkspace::Save(bool force)
{
    SANITY_CHECK(false);

    if (m_Filename.GetFullPath().IsEmpty())
        return SaveAs("");

    if (!force && !m_Modified)
        return true;

	Manager::Get()->GetMessageManager()->DebugLog("Saving workspace \"%s\"", m_Filename.GetFullPath().c_str());
	WorkspaceLoader wsp;
	bool ret = wsp.Save(m_Title, m_Filename.GetFullPath());
    SetModified(!ret);
    return ret;
}

bool cbWorkspace::SaveAs(const wxString& filename)
{
    SANITY_CHECK(false);
    wxFileDialog* dlg = new wxFileDialog(0,
                            _("Save workspace"),
                            m_Filename.GetPath(),
                            m_Filename.GetFullName(),
                            WORKSPACES_FILES_FILTER,
                            wxSAVE | wxOVERWRITE_PROMPT);
    if (dlg->ShowModal() != wxID_OK)
        return false;
    SANITY_CHECK(false);
    m_Filename = dlg->GetPath();
    m_IsDefault = false;
    return Save(true);
}

void cbWorkspace::SetTitle(const wxString& title)
{
    SANITY_CHECK();
    m_Title = title;
    SetModified(true);
}

void cbWorkspace::SetModified(bool modified)
{
    SANITY_CHECK();
    m_Modified = modified;
}
