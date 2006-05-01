/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* Name:      blocks.cpp
* Purpose:   Managing the blocks of code::blocks
* Author:    Thomas Denk
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include <sdk.h>
#include "blocks.h"
#include <wx/checklst.h>

// Implement the plugin's hooks
CB_IMPLEMENT_PLUGIN(Blocks, "Blocks");

Blocks::Blocks()
{
	m_PluginInfo.name = _T("Blocks");
	m_PluginInfo.title = _("Managing Blocks");
	m_PluginInfo.description = _T("Blocks - managing the blocks of code::blocks");
	m_PluginInfo.license = _T("[Code::Blocks license]");
	wxXmlResource::Get()->Load(ConfigManager::GetDataFolder() + _T("/blocks.zip#zip:blocks.xrc"));

}

Blocks::~Blocks()
{
}

void Blocks::OnAttach()
{
}

void Blocks::OnRelease(bool appShutDown)
{
}

cbConfigurationPanel* Blocks::GetConfigurationPanel(wxWindow* parent)
{
    BlocksConfigDlg* dlg = new BlocksConfigDlg(parent, this);

    return dlg;

}

int Blocks::Execute()
{
	NotImplemented(_T("Blocks::Execute()"));
	return 0;
}



BlocksConfigDlg::BlocksConfigDlg(wxWindow* parent, Blocks* plug) : plugin(plug)
{
    wxXmlResource::Get()->LoadPanel(this, parent, _T("blocksConfig"));

    LoadSettings();
}

void BlocksConfigDlg::LoadSettings()
{
    ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("blocks"));

    wxCheckListBox* c = XRCCTRL(*this, "channels", wxCheckListBox);
    if(c)
        {
        c->Append(_T("foo"));
        c->Append(_T("bar"));
        }
}

void BlocksConfigDlg::SaveSettings()
{

}


