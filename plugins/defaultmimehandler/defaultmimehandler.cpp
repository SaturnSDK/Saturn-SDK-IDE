/***************************************************************
 * Name:      defaultmimehandler.cpp
 * Purpose:   Code::Blocks plugin
 * Author:    Yiannis An. Mandravellos<mandrav@codeblocks.org>
 * Created:   03/13/05 14:08:51
 * Copyright: (c) Yiannis An. Mandravellos
 * License:   GPL
 **************************************************************/

#include <sdk.h>

#include "defaultmimehandler.h"
#include <manager.h>
#include <editormanager.h>
#include <configmanager.h>
#include <cbeditor.h>
#include <licenses.h> // defines some common licenses (like the GPL)

#include <wx/intl.h>
#include <wx/xrc/xmlres.h>
#include <wx/fs_zip.h>
#include <wx/choicdlg.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include "editmimetypesdlg.h"
#include <globals.h>

CB_IMPLEMENT_PLUGIN(DefaultMimeHandler, "Files extension handler");

DefaultMimeHandler::DefaultMimeHandler()
{
	//ctor
	m_PluginInfo.name = _T("FilesExtensionHandler");
	m_PluginInfo.title = _("Files extension handler");
	m_PluginInfo.version = _T("1.0");
	m_PluginInfo.description = _("This is the default files extension handler for Code::Blocks");
	m_PluginInfo.author = _T("Yiannis An. Mandravellos");
	m_PluginInfo.authorEmail = _T("mandrav@codeblocks.org");
	m_PluginInfo.authorWebsite = _("http://www.codeblocks.org");
	m_PluginInfo.thanksTo = _T("Code::Blocks");
	m_PluginInfo.license = LICENSE_GPL;

    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxXmlResource::Get()->InitAllHandlers();
    wxString resPath = ConfigManager::GetDataFolder();
    wxXmlResource::Get()->Load(resPath + _T("/defaultmimehandler.zip#zip:*.xrc"));
}

DefaultMimeHandler::~DefaultMimeHandler()
{
	//dtor
}

void DefaultMimeHandler::OnAttach()
{
    // load configuration
    WX_CLEAR_ARRAY(m_MimeTypes);

    ConfigManager* conf = Manager::Get()->GetConfigManager(_T("mime_types"));
    wxArrayString list = conf->EnumerateKeys(_T("/"));
    for (unsigned int i = 0; i < list.GetCount(); ++i)
    {
        wxArrayString array = GetArrayFromString(conf->Read(list[i]));
        if (array.GetCount() == 3 || array.GetCount() == 4)
        {
            cbMimeType* mt = new cbMimeType;
            mt->useEditor = array[0] == _T("true");
            mt->programIsModal = array[1] == _T("true");
            mt->wildcard = array[2];
            mt->program = array.GetCount() == 4 ? array[3] : _T("");

            if (!mt->useEditor && mt->program.IsEmpty())
                delete mt;
            else
                m_MimeTypes.Add(mt);
        }
	}
}


void DefaultMimeHandler::OnRelease(bool appShutDown)
{
    // save configuration
    ConfigManager* conf = Manager::Get()->GetConfigManager(_T("mime_types"));
    wxArrayString list = conf->EnumerateKeys(_T("/"));
    for (unsigned int i = 0; i < list.GetCount(); ++i)
    {
        conf->UnSet(list[i]);
    }
	for (unsigned int i = 0; i < m_MimeTypes.GetCount(); ++i)
	{
        cbMimeType* mt = m_MimeTypes[i];
        wxString txt;
        txt << (mt->useEditor ? _T("true") : _T("false")) << _T(";");
        txt << (mt->programIsModal ? _T("true") : _T("false")) << _T(";");
        txt << mt->wildcard << _T(";");
        txt << mt->program;
        wxString key;
        key.Printf(_T("MimeType%d"), i);
		conf->Write(key, txt);
	}
    WX_CLEAR_ARRAY(m_MimeTypes);
}

int DefaultMimeHandler::Configure()
{
    return 0;
}

cbConfigurationPanel* DefaultMimeHandler::GetConfigurationPanel(wxWindow* parent)
{
    EditMimeTypesDlg* dlg = new EditMimeTypesDlg(parent, m_MimeTypes);
    return dlg;
}

bool DefaultMimeHandler::CanHandleFile(const wxString& filename) const
{
    // always return true
    // even if we don't know how to handle the file,
    // we 'll ask the user what to do when we are requested to open it...
	return true;
}

int DefaultMimeHandler::OpenFile(const wxString& filename)
{
	cbMimeType* mt = FindMimeTypeFor(filename);
	if (mt)
        return DoOpenFile(mt, filename);
    else
    {
        // not yet supported. ask the user how to open it.
        wxString choices[2] = {_("Select an external program to open it."),
                               _("Open it inside the Code::Blocks editor.")};
        wxSingleChoiceDialog dlg(0,
                                _("Code::Blocks does not yet know how to open this kind of file.\n"
                                "Please select what you want to do with it:"),
                                _("What to do?"),
                                sizeof(choices) / sizeof(choices[0]),
                                choices);
        dlg.SetSelection(0);
        PlaceWindow(&dlg);
        if (dlg.ShowModal() == wxID_OK)
        {
            wxString ext = wxFileName(filename).GetExt().Lower();
            wxString wild = ext.IsEmpty()
                            ? wxFileName(filename).GetName().Lower()
                            : wxString(_T("*.")) + ext;
            switch (dlg.GetSelection())
            {
                case 0: // choose external program
                {
                    wxString prg = ChooseExternalProgram();
                    if (!prg.IsEmpty())
                    {
                        mt = new cbMimeType;
                        mt->wildcard = wild;
                        mt->useEditor = false;
                        mt->program = prg;
                        mt->programIsModal = wxMessageBox(_("Do you want Code::Blocks to be disabled while the external program is running?"), _("Question"), wxICON_QUESTION | wxYES_NO) == wxYES;
                        m_MimeTypes.Add(mt);
                        return DoOpenFile(mt, filename);
                    }
                    break;
                }
                case 1: // open in editor
                {
                    mt = new cbMimeType;
                    mt->wildcard = wild;
                    mt->useEditor = true;
                    m_MimeTypes.Add(mt);
                    return DoOpenFile(mt, filename);
                    break;
                }
                default: break;
            }
        }
        else
        {
            return 0;
        }
    }
	return -1;
}

cbMimeType* DefaultMimeHandler::FindMimeTypeFor(const wxString& filename)
{
    wxString tmp = wxFileName(filename).GetFullName().Lower();

    // look for a registered type
    for (size_t i = 0; i < m_MimeTypes.GetCount(); ++i)
    {
        cbMimeType* mt = m_MimeTypes[i];
        if (tmp.Matches(mt->wildcard))
            return mt;
    }
    return 0;
}

wxString DefaultMimeHandler::ChooseExternalProgram()
{
    wxFileDialog* dlg = new wxFileDialog(0,
                            _("Select program"),
                            wxEmptyString,
                            wxEmptyString,
                            ALL_FILES_FILTER,
                            wxOPEN);
    PlaceWindow(dlg);
    if (dlg->ShowModal() == wxID_OK)
        return dlg->GetPath();
    return wxEmptyString;
}

int DefaultMimeHandler::DoOpenFile(cbMimeType* mt, const wxString& filename)
{
    if (!mt)
        return -1;

    if (mt->useEditor)
    {
        // easy. use internal editor.
		cbEditor* ed = Manager::Get()->GetEditorManager()->Open(filename);
		if (ed)
		{
//			ed->SetProjectFile(pf);
			ed->Show(true);
			return 0;
		}
    }
    else
    {
        // call external program

        // create command line
        wxString external = mt->program;
        if (external.Find(_T("$(FILE)")) != -1)
            external.Replace(_T("$(FILE)"), filename);
        else
            external << _T(" \"") << filename << _T("\""); // file args wrapped in quotes (bug #1187231)

        // launch external program
        int ret = 0;
        if (mt->programIsModal)
        {
            wxEnableTopLevelWindows(false);
            ret = wxExecute(external, wxEXEC_SYNC);
            wxEnableTopLevelWindows(true);
        }
        else
            wxExecute(external, wxEXEC_ASYNC);
        return ret;
    }

    // failed...
    return -1;
}
