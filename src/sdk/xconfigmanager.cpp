/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* Author: Thomas Denk
*
* $Id$
* $Date$
*/

#include "sdk_precomp.h"
#include "xconfigmanager.h"

#include <wx/file.h>

#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"

#ifdef __WXMSW__
#include <shlobj.h>
#endif

wxString XmlConfigManager::app_path = wxEmptyString;
wxString XmlConfigManager::data_path = wxEmptyString;
wxString XmlConfigManager::config_folder = wxEmptyString;
wxString XmlConfigManager::home_folder = wxEmptyString;


namespace CfgMgrConsts
{
    const wxString app_path(_T("app_path"));
    const wxString data_path(_T("data_path"));
    const char* rootTag = _T("CodeBlocksConfig");
}



/* ------------------------------------------------------------------------------------------------------------------
*  "Builder pattern" class for XmlConfigManager
*  Do not use this class  -  Manager::Get()->GetConfigManager() is a lot friendlier
*/

CfgMgrBldr::CfgMgrBldr()
{
    SwitchTo(XmlConfigManager::GetConfigFolder() + Manager::Get()->GetPersonalityManager()->GetPersonality() + _T(".conf"));
}

void CfgMgrBldr::SwitchTo(const wxString& absFileName)
{
    Close();
    doc = new TiXmlDocument(absFileName.mb_str());
    if(!doc->LoadFile()
            && !doc->LoadFile((XmlConfigManager::GetExecutableFolder() + _T("/default.conf")).mb_str())
            && !doc->LoadFile((XmlConfigManager::GetConfigFolder() + _T("/default.conf")).mb_str()))
    {
        doc->InsertEndChild(TiXmlDeclaration ("1.0", "UTF-8", "yes"));
        doc->InsertEndChild(TiXmlElement(CfgMgrConsts::rootTag));
    }
    doc->ClearError();
}


CfgMgrBldr::~CfgMgrBldr()
{
    NamespaceMap::iterator it;
    for( it = namespaces.begin(); it != namespaces.end(); ++it )
    {
        delete it->second;
    }
    namespaces.clear();
    Close();
}


void CfgMgrBldr::Close()
{
    if(doc)
    {
        doc->SaveFile();
        delete doc;
        doc = 0;
    }
}


XmlConfigManager* CfgMgrBldr::Get(const wxString& name_space)
{
    static CfgMgrBldr instance;
    return (&instance)->Instantiate(name_space);
}

XmlConfigManager* CfgMgrBldr::Instantiate(const wxString& name_space)
{
    if(name_space.IsEmpty())
        cbThrow("Manager::Get()->GetConfigManager was called without namespace.");

    NamespaceMap::iterator it = namespaces.find(name_space);
    if(it != namespaces.end())
        return it->second;

    TiXmlElement* docroot = doc->FirstChildElement(CfgMgrConsts::rootTag);
    if(!docroot)
    {
        wxString err(_T("Fatal error parsing supplied configuration file.\nTiXml error message:\n"));
        err << doc->ErrorDesc();
        cbThrow(err);
    }

    TiXmlElement* root = docroot->FirstChildElement(name_space);

    if(!root) // namespace does not exist
    {
        docroot->InsertEndChild(TiXmlElement(name_space));
        root = docroot->FirstChildElement(name_space);
    }

    if(!root) // now what!
        cbThrow("Unable to create namespace in document tree (not possible..?)");

    XmlConfigManager *c = new XmlConfigManager(root);
    namespaces[name_space] = c;
    return c;
}




/* ------------------------------------------------------------------------------------------------------------------
*  Functions to retrieve system paths and locate data files in a defined, consistent way.
*  Please note that the application determines app_path and data_path at runtime and passes the results
*  to ConfigManager. GetExecutableFolder() and GetDataFolder() are therefore under normal conditions
*  simply more efficient shortcuts for Read("app_path") and Read("data_path").
*/

wxString XmlConfigManager::GetExecutableFolder()
{
    if(!XmlConfigManager::app_path.IsEmpty())
        return XmlConfigManager::app_path;

#ifdef __WXMSW__

    wxChar name[MAX_PATH];
    GetModuleFileName(0L, name, MAX_PATH);
    wxFileName fname(name);
    XmlConfigManager::app_path = fname.GetPath(wxPATH_GET_VOLUME);
#else

    XmlConfigManager::app_path = wxString(SELFPATH,wxConvUTF8);
    XmlConfigManager::app_path = wxFileName(base).GetPath();
    if (XmlConfigManager::app_path.IsEmpty())
        XmlConfigManager::app_path = _T(".");
#endif

    return XmlConfigManager::app_path;
}

wxString XmlConfigManager::GetHomeFolder()
{
    if(XmlConfigManager::home_folder.IsEmpty())
    {
        wxFileName f;
        f.AssignHomeDir();
        XmlConfigManager::home_folder = f.GetFullPath();
    }
    return XmlConfigManager::home_folder;
}

#ifdef __WXMSW__

wxString XmlConfigManager::GetConfigFolder()
{
    if(XmlConfigManager::config_folder.IsEmpty())
    {
        TCHAR szPath[MAX_PATH];
        SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath);
        XmlConfigManager::config_folder = wxString(szPath) + _T("/codeblocks");
    }
    if(!wxDirExists(XmlConfigManager::config_folder))
        wxMkdir(XmlConfigManager::config_folder);

    return XmlConfigManager::config_folder;
}

wxString XmlConfigManager::GetPluginsFolder()
{
    return GetExecutableFolder() + _T("/share/codeblocks/plugins");
}

wxString XmlConfigManager::GetDataFolder()
{
    if(XmlConfigManager::data_path.IsEmpty())
        XmlConfigManager::data_path = GetExecutableFolder() + _T("/share/codeblocks");
    return XmlConfigManager::data_path;
}

wxString XmlConfigManager::LocateDataFile(const wxString& filename)
{
    wxPathList searchPaths;
    searchPaths.Add(GetDataFolder());
    searchPaths.Add(GetExecutableFolder());
    searchPaths.Add(GetHomeFolder());
    searchPaths.AddEnvList("PATH");
    searchPaths.Add("C:/");
    return searchPaths.FindValidPath(filename);
}

#else


wxString XmlConfigManager::GetConfigFolder()  const
{
    if(XmlConfigManager::config_folder.IsEmpty())
        XmlConfigManager::config_folder = GetHomeFolder() + _T("/.codeblocks");
    if(!wxDir::Exists(XmlConfigManager::config_folder))
        wxMkdir(XmlConfigManager::config_folder);

    return XmlConfigManager::config_folder;
}

wxString XmlConfigManager::GetPluginsFolder()  const
{
    return wxString(_T("/usr/share/codeblocks/plugins"));
}

wxString XmlConfigManager::GetDataFolder()  const
{
    if(XmlConfigManager::data_path.IsEmpty())
        XmlConfigManager::data_path(_T("/usr/share/codeblocks"));
    return XmlConfigManager::data_path;
}

wxString XmlConfigManager::LocateDataFile(const wxString& filename)  const
{
    wxPathList searchPaths;
    searchPaths.Add(GetDataFolder());
    searchPaths.Add(GetHomeFolder());
    searchPaths.AddEnvList("PATH");
    searchPaths.Add("/usr/share/");
    return searchPaths.FindValidPath(filename);
}

#endif




/* ------------------------------------------------------------------------------------------------------------------
*  XmlConfigManager
*/

XmlConfigManager::XmlConfigManager(TiXmlElement* r) : root(r)
{
    doc = root->GetDocument();
}




/* ------------------------------------------------------------------------------------------------------------------
*  Configuration path handling
*  Note that due to namespaces, you do no longer need to care about saving/restoring the current path in the normal case.
*  Mostly, there will be only one module working with one namespace, and every namespace keeps track of its own path.
*  Also, please note that GetPath() is more expensive than it seems (not a mere accessor to a member variable!), while
*  SetPath() not only sets the current path, but also creates the respective nodes in the XML document if these don't exist.
*/

wxString XmlConfigManager::GetPath() const
{
    return wxEmptyString; //FIXME: implement a bottom-up tree walk here
};

void XmlConfigManager::SetPath(const wxString& path)
{
    wxString p(path + _T("/"));
    pathNode = AssertPath(p);
}

TiXmlElement* XmlConfigManager::AssertPath(wxString& path)
{
    if(!path.Contains(_T("/")))
        return pathNode;

    TiXmlElement* e = pathNode ? pathNode : root;

    path.LowerCase();
    path.Replace(_T("//"), _T("/"));
    Manager::Get()->GetMessageManager()->DebugLog("path=" +path);
    wxString sub;
    do
    {
        sub = path.BeforeFirst(_T('/'));
        path = path.AfterFirst(_T('/'));

        Manager::Get()->GetMessageManager()->DebugLog(" path=" +path);
        Manager::Get()->GetMessageManager()->DebugLog(" sub =" +sub);
        if(sub.IsEmpty())
            e = root;
        else if(sub.IsSameAs(_T(".")))
            ;
        else if(e != root && sub.IsSameAs(_T("..")))
            e = e->Parent()->ToElement();
        else
        {
            TiXmlElement* n = e->FirstChildElement(sub);
            if(!n)
                e = (TiXmlElement*) e->InsertEndChild(TiXmlElement(sub));
            else
                e = n;
        }
        if(doc->Error())
        {
            Manager::Get()->GetMessageManager()->DebugLog(wxString(_T("Error accessing config path: ")) + doc->ErrorDesc());
            doc->ClearError();
        }
    }
    while(path.Contains(_T("/")));

    return e;
}




/* ------------------------------------------------------------------------------------------------------------------
*  Write and read wxString values
*  Regardless of namespaces, the keys app_path and data_path always refer to the location of the application's executable
*  and the data path, respectively. These values are never saved to the configuration, but kept in static variables.
*  The application makes use of this by "writing" to the configuration file after determining these values at runtime.
*/
bool XmlConfigManager::Write(const wxString& name,  const wxString& value)
{
    if(name.IsSameAs(CfgMgrConsts::app_path))
    {
        XmlConfigManager::app_path = value;
        return true;
    }
    else if(name.IsSameAs(CfgMgrConsts::data_path))
    {
        XmlConfigManager::data_path = value;
        return true;
    }

    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    return false;
}

wxString XmlConfigManager::Read(const wxString& name, const wxString& defaultVal)
{
    if(name.IsSameAs(CfgMgrConsts::app_path))
        return XmlConfigManager::app_path;
    else if(name.IsSameAs(CfgMgrConsts::data_path))
        return XmlConfigManager::data_path;

    wxString ret;

    if(Read(name, &ret))
        return ret;
    else
        return defaultVal;
}

bool XmlConfigManager::Read(const wxString& name, wxString* str)
{
    return false;
}
