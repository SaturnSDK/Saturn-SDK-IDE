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
#include <wx/html/htmlwin.h>

#include "bzlib.h"

#include <wx/filesys.h>
#include <wx/fs_inet.h>
#include <wx/zstream.h>

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
wxFileSystem::AddHandler(new wxInternetFSHandler);
wxFileSystem fs;

wxFSFile* f = fs.OpenFile(_T("http://updates.codeblocks.org/"));
if(f)
{
Manager::Get()->GetMessageManager()->DebugLog(wxString(_T("time ")) << f->GetModificationTime().Format());

char c[200];

f->GetStream()->Read(c, 199);
}

	wxDialog *d = wxXmlResource::Get()->LoadDialog(Manager::Get()->GetAppWindow(), _T("package_select"));

	XRCCTRL(*d, "details", wxHtmlWindow)->SetPage(_T("<html><h2>foo</h2><br>bar</html>"));
    XRCCTRL(*d, "checklist", wxCheckListBox)->Append(_T("foobar"));

d->ShowModal();
	return 0;
}


bool Blocks::InstallTGZ(const wxString& filename)
{
    wxString m_Status;

    wxFileInputStream in(filename);
    wxZlibInputStream gz(in);

    TempFile temp;
    wxFileOutputStream out(temp);

    gz.Read(out);

    return UnTar(temp);
}



bool Blocks::InstallTBZ(const wxString& filename)
{
wxString m_Status;
    wxFFile f(filename.fn_str(), _T("rb"));
    if (!f.IsOpened())
    {
        m_Status = _("Error opening input file!");
        return false;
    }

    // open BZIP2 stream
    int bzerror;
    BZFILE* bz = BZ2_bzReadOpen(&bzerror, f.fp(), 0, 0, 0L, 0);
    if (!bz || bzerror != BZ_OK)
    {
        m_Status = _("Can't read compressed stream!");
        return false;
    }

    TempFile temp;
    if (!temp.IsOpened())
    {
        m_Status = _("Error opening output file!");
        return false;
    }

    size_t numBytes;
    char buffer[4096];

    while (bzerror != BZ_STREAM_END)
    {
        numBytes = BZ2_bzRead(&bzerror, bz, buffer, sizeof(buffer));
        if (bzerror != BZ_OK && bzerror != BZ_STREAM_END)
        {
            m_Status = _("Error reading from stream!");
            BZ2_bzReadClose(&bzerror, bz);
            return false;
        }
        temp.Write(buffer, numBytes);
    }

    BZ2_bzReadClose(&bzerror, bz);

    return UnTar(temp);
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


