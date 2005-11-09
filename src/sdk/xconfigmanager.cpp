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
    const int version = 1;
}


ISerializable::ISerializable()
{}

ISerializable::~ISerializable()
{}




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
        doc->InsertEndChild(TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T("yes")));
        doc->InsertEndChild(TiXmlElement(CfgMgrConsts::rootTag));
        doc->FirstChildElement(CfgMgrConsts::rootTag)->SetAttribute(_T("version"), CfgMgrConsts::version);
        doc->SetCondenseWhiteSpace(false);
    }

    TiXmlElement* docroot = doc->FirstChildElement(CfgMgrConsts::rootTag);
    if(!docroot)
    {
        wxString s;
        s.sprintf(_("Fatal error parsing configuration. The file  %s is not a valid Code::Blocks config file."), doc->Value());
        cbThrow(s);
    }

    const char *vers = docroot->Attribute(_T("version"));
    if(!vers || atoi(vers) != 1)
        Manager::Get()->GetMessageManager()->DebugLog(_("MessageManager encountered an unknown config file version. Continuing happily."));

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
        cbThrow(_("You attempted to get a ConfigManager instance without providing a namespace."));

    NamespaceMap::iterator it = namespaces.find(name_space);
    if(it != namespaces.end())
        return it->second;

    TiXmlElement* docroot = doc->FirstChildElement(CfgMgrConsts::rootTag);
    if(!docroot)
    {
        wxString err(_("Fatal error parsing supplied configuration file.\nParser error message:\n"));
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
        cbThrow(_("Unable to create namespace in document tree (actually not possible..?)"));

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
    searchPaths.AddEnvList(_T("PATH"));
    searchPaths.Add(_T("C:/"));
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
    searchPaths.AddEnvList(_T("PATH"));
    searchPaths.Add(_T("/usr/share/"));
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
    TiXmlElement *e = pathNode;
    wxString ret;
    ret.Alloc(64);

    ret = e->Value();
    while((e = e->Parent()->ToElement()) && e != root)
    {
        ret.Prepend(_T("/"));
        ret.Prepend(e->Value());
    }
    ret.Prepend(_T("/"));
    return ret;
};

void XmlConfigManager::SetPath(const wxString& path)
{
    wxString p(path + _T("/"));
    pathNode = AssertPath(p);
}


TiXmlElement* XmlConfigManager::AssertPath(wxString& path)
{
    if(!path.Contains(_T("/")))
    {
        path.UpperCase();
        if(path[0] < _T('A') || path[0] > _T('Z'))
        {
            wxString s;
            s.sprintf(_("Warning: The Configuration key %s does not meet the standard for variable naming. Variables names are required to start with a letter."), path.mb_str());
            Manager::Get()->GetMessageManager()->DebugLog(s);
        }
        return pathNode;
    }

    TiXmlElement* e = pathNode ? pathNode : root;

    path.LowerCase();
    path.Replace(_T("//"), _T("/"));

    wxString sub;
    do
    {
        sub = path.BeforeFirst(_T('/'));
        path = path.AfterFirst(_T('/'));

        if(sub.IsEmpty())
            e = root;
        else if(sub.IsSameAs(_T(".")))
            ;
        else if(e != root && sub.IsSameAs(_T("..")))
            e = e->Parent()->ToElement();
        else
        {
            TiXmlElement* n = e->FirstChildElement(sub);
            if(n)
                e = n;
            else
                e = (TiXmlElement*) e->InsertEndChild(TiXmlElement(sub));
        }
        if(doc->Error())
        {
            Manager::Get()->GetMessageManager()->DebugLog(wxString(_T("Error accessing config path: ")) + doc->ErrorDesc());
            doc->ClearError();
        }
    }
    while(path.Contains(_T("/")));

    path.UpperCase();
    if(!path.IsEmpty() && (path[0] < _T('A') || path[0] > _T('Z')))

    {
        wxString s;
        s.sprintf(_("Warning: The Configuration key %s does not meet the standard for variable naming. Variables names are required to start with a letter."), path.mb_str());
        Manager::Get()->GetMessageManager()->DebugLog(s);
    }
    return e;
}



/* ------------------------------------------------------------------------------------------------------------------
*  Utility functions for writing nodes
*/

TiXmlElement* XmlConfigManager::GetUniqElement(TiXmlElement* p, const wxString& q)
{
    TiXmlElement* r;
    if(r = p->FirstChildElement(q.mb_str()))
        return r;

    return (TiXmlElement*)(p->InsertEndChild(TiXmlElement(q.mb_str())));
}

void XmlConfigManager::SetNodeText(TiXmlElement* n, const TiXmlText& t)
{
    TiXmlNode *c = n->FirstChild();
    if(c)
        n->ReplaceChild(c, t);
    else
        n->InsertEndChild(t);
}



/* ------------------------------------------------------------------------------------------------------------------
*  Write and read values
*  Regardless of namespaces, the string keys app_path and data_path always refer to the location of the application's executable
*  and the data path, respectively. These values are never saved to the configuration, but kept in static variables.
*  The application makes use of this by "writing" to the configuration file after determining these values at runtime.
*/
void XmlConfigManager::Write(const wxString& name,  const wxString& value)
{
    if(name.IsSameAs(CfgMgrConsts::app_path))
    {
        XmlConfigManager::app_path = value;
        return;
    }
    else if(name.IsSameAs(CfgMgrConsts::data_path))
    {
        XmlConfigManager::data_path = value;
        return;
    }

    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *str = GetUniqElement(e, key);

    TiXmlElement *s = GetUniqElement(str, _T("str"));
    SetNodeText(s, TiXmlText(value.mb_str()));
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
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlText *t = (TiXmlText *) parentHandle.FirstChild(key).FirstChild(_T("str")).FirstChild().Node();

    if(t)
    {
        str->assign(t->Value());
        return true;
    }
    return false;
}


void XmlConfigManager::Write(const wxString& name,  int value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);
    TiXmlElement *leaf = GetUniqElement(e, key);

    leaf->SetAttribute(_T("int"), value);
}

int  XmlConfigManager::ReadInt(const wxString& name,  int defaultVal)
{
    int ret;

    if(Read(name, &ret))
        return ret;
    else
        return defaultVal;
}

bool XmlConfigManager::Read(const wxString& name,  int* value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlElement *leaf = parentHandle.FirstChild(key).Element();

    if(leaf)
        return leaf->QueryIntAttribute(_T("int"), value) == TIXML_SUCCESS;
    return false;
}


void XmlConfigManager::Write(const wxString& name,  bool value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);
    TiXmlElement *leaf = GetUniqElement(e, key);

    leaf->SetAttribute(_T("bool"), value ? _T("1") : _T("0"));
}

bool  XmlConfigManager::ReadBool(const wxString& name,  bool defaultVal)
{
    bool ret;

    if(Read(name, &ret))
        return ret;
    else
        return defaultVal;
}

bool XmlConfigManager::Read(const wxString& name,  bool* value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlElement *leaf = parentHandle.FirstChild(key).Element();

    if(leaf && leaf->Attribute("bool"))
    {
        *value = leaf->Attribute("bool")[0] == _T('1');
        return true;
    }
    return false;
}


void XmlConfigManager::Write(const wxString& name,  double value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);
    TiXmlElement *leaf = GetUniqElement(e, key);

    leaf->SetDoubleAttribute(_T("double"), value);
}

double  XmlConfigManager::ReadDouble(const wxString& name,  double defaultVal)
{
    double ret;

    if(Read(name, &ret))
        return ret;
    else
        return defaultVal;
}

bool XmlConfigManager::Read(const wxString& name,  double* value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlElement *leaf = parentHandle.FirstChild(key).Element();

    if(leaf)
        return leaf->QueryDoubleAttribute(_T("double"), value) == TIXML_SUCCESS;
    return false;
}


void XmlConfigManager::Set(const wxString& name)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);
    GetUniqElement(e, key);
}

void XmlConfigManager::UnSet(const wxString& name)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlNode *leaf = GetUniqElement(e, key);
    e->RemoveChild(leaf);
}

bool XmlConfigManager::Exists(const wxString& name)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlElement *leaf = parentHandle.FirstChild(key).Element();

    return leaf;
}



void XmlConfigManager::Write(const wxString& name,  const wxArrayString& arrayString)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *leaf = GetUniqElement(e, key);

    TiXmlElement *as;
    as = GetUniqElement(leaf, _T("astr"));
    leaf->RemoveChild(as);
    as = GetUniqElement(leaf, _T("astr"));

    for(unsigned int i = 0; i < arrayString.GetCount(); ++i)
    {
        TiXmlElement s("s");
        s.InsertEndChild(TiXmlText(arrayString[i].mb_str()));
        as->InsertEndChild(s);
    }
}

void XmlConfigManager::Read(const wxString& name, wxArrayString *arrayString)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlNode *asNode = parentHandle.FirstChild(key).FirstChild(_T("astr")).Node();

    TiXmlNode *curr = 0;
    if(asNode)
    {
        while(curr = asNode->IterateChildren(_T("s"), curr)->ToElement())
            arrayString->Add(curr->FirstChild()->ToText()->Value());
    }
}

wxArrayString XmlConfigManager::ReadArrayString(const wxString& name)
{
    wxArrayString as;
    Read(name, &as);
    return as;
}


void XmlConfigManager::WriteBinary(const wxString& name,  const wxString& source)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *str = GetUniqElement(e, key);

    TiXmlElement *s = GetUniqElement(str, _T("bin"));
    SetNodeText(s, TiXmlText(wxBase64Encode(source).mb_str()));
}

void XmlConfigManager::WriteBinary(const wxString& name,  void* ptr, size_t len)
{
    wxString s((const char*)ptr, len);
    WriteBinary(name,  s);
}

wxString XmlConfigManager::ReadBinary(const wxString& name)
{
    wxString str;
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlText *t = (TiXmlText *) parentHandle.FirstChild(key).FirstChild(_T("bin")).FirstChild().Node();

    if(t)
    {
        str.assign(t->Value());
        return wxBase64Decode(str);
    }
    return wxEmptyString;
}


wxArrayString XmlConfigManager::EnumerateSubPaths(const wxString& path)
{
    wxString key(path + _T("/")); // the trailing slash hack is required because AssertPath expects a key name
    TiXmlNode* e = AssertPath(key);
    wxArrayString ret;

    TiXmlElement *curr = 0;
    if(e)
    {
        while(curr = e->IterateChildren(curr)->ToElement())
        {
            wxChar c = *(curr->Value());
            if(c < _T('A') || c > _T('Z')) // first char must be a letter, uppercase letters are key names
                ret.Add(curr->Value());
        }
    }
    return ret;
}

wxArrayString XmlConfigManager::EnumerateKeys(const wxString& path)
{
    wxString key(path + _T("/")); // the trailing slash hack is required because AssertPath expects a key name
    TiXmlNode* e = AssertPath(key);
    wxArrayString ret;

    TiXmlElement *curr = 0;
    if(e)
    {
        while(curr = e->IterateChildren(curr)->ToElement())
        {
            wxChar c = *(curr->Value());
            if(c >= _T('A') && c <= _T('Z')) // opposite of the above
                ret.Add(curr->Value());
        }
    }
    return ret;
}

void XmlConfigManager::Write(const wxString& name, const ISerializable& object)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *obj = GetUniqElement(e, key);

    TiXmlElement *s = GetUniqElement(obj, _T("obj"));
    SetNodeText(s, TiXmlText(wxBase64Encode(object.SerializeOut()).mb_str()));
}

bool XmlConfigManager::Read(const wxString& name, ISerializable* object)
{
    wxString str;
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlText *t = (TiXmlText *) parentHandle.FirstChild(key).FirstChild(_T("obj")).FirstChild().Node();

    if(t)
    {
        str.assign(t->Value());
        object->SerializeIn(wxBase64Decode(str));
    }
    return wxEmptyString;
}

void XmlConfigManager::Write(const wxString& name, const ConfigManagerContainer::StringToStringMap& map)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *leaf = GetUniqElement(e, key);

    TiXmlElement *mNode;
    mNode = GetUniqElement(leaf, _T("ssmap"));
    leaf->RemoveChild(mNode);
    mNode = GetUniqElement(leaf, _T("ssmap"));

    for(ConfigManagerContainer::StringToStringMap::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        TiXmlElement s(it->first);
        s.InsertEndChild(TiXmlText(it->second));
        mNode->InsertEndChild(s);
    }
}

void XmlConfigManager::Read(const wxString& name, ConfigManagerContainer::StringToStringMap* map)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlNode *mNode = parentHandle.FirstChild(key).FirstChild(_T("ssmap")).Node();

    TiXmlNode *curr = 0;
    if(mNode)
    {
        while(curr = mNode->IterateChildren(curr)->ToElement())
            (*map)[curr->Value()] = curr->FirstChild()->ToText()->Value();
    }
}

ConfigManagerContainer::StringToStringMap XmlConfigManager::ReadSSMap(const wxString& name)
{
    ConfigManagerContainer::StringToStringMap ret;
    Read(name, &ret);
    return ret;
}

void XmlConfigManager::Write(const wxString& name, const ConfigManagerContainer::SerializableObjectMap* map)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *leaf = GetUniqElement(e, key);

    TiXmlElement *mNode;
    mNode = GetUniqElement(leaf, _T("objmap"));
    leaf->RemoveChild(mNode);
    mNode = GetUniqElement(leaf, _T("objmap"));

    for(ConfigManagerContainer::SerializableObjectMap::const_iterator it = map->begin(); it != map->end(); ++it)
    {
        TiXmlElement s(it->first);
        s.InsertEndChild(TiXmlText(wxBase64Encode(it->second->SerializeOut())));
        mNode->InsertEndChild(s);
    }
}





