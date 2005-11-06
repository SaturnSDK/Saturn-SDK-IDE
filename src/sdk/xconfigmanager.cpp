#include "sdk_precomp.h"
#include "xconfigmanager.h"
#include "manager.h"
#include "personalitymanager.h"
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
    const wxString app_path(_T("app_path"));   // compare pointers, avoid calling wxString::wxString(const char*) 1000 times
    const wxString data_path(_T("data_path"));
}


XmlConfigManager::XmlConfigManager()
{}




CfgMgrBldr::CfgMgrBldr()
{
    wxString personality(Manager::Get()->GetPersonalityManager()->GetPersonality());

    if(personality.IsEmpty())
        personality = _T("default.conf");
    else
        personality << _T(".conf");

    wxString filename;

    if(!doc.LoadFile(filename.mb_str()))
    {
        // try another location
    }

    readonly = !(wxFile::Access(personality, wxFile::write));
}

CfgMgrBldr::~CfgMgrBldr()
{
    Map::iterator it;
    for( it = map.begin(); it != map.end(); ++it )
    {
        delete it->second;
    }
    map.clear();

    if(!readonly)
        doc.SaveFile();
}

XmlConfigManager* CfgMgrBldr::Get(const wxString& name_space)
{
    static CfgMgrBldr instance;
    return (&instance)->Instantiate(name_space);
}

XmlConfigManager* CfgMgrBldr::Instantiate(const wxString& name_space)
{
    Map::iterator it = map.find(name_space);
    if(it != map.end())
        return it->second;

    XmlConfigManager *c = new XmlConfigManager;

    TiXmlElement* root = doc.FirstChildElement("CBConfig");
    c->root = root->FirstChildElement(name_space);

    if(c->root == 0) // must create
    {
        root->InsertEndChild(TiXmlElement(name_space));
        c->root = root->FirstChildElement(name_space);
    }

    if(c->root == 0) // now what!
    {
        delete c;
        return 0;
    }

    map[name_space] = c;
    return c;
}









wxString XmlConfigManager::GetExecutableFolder()  const
{
    //  #include "../src/app.h" gives me nasty errors?
    //  return ((CodeBlocksApp)wxTheApp)->GetAppPath();

    if(!XmlConfigManager::app_path.IsEmpty())
        return XmlConfigManager::app_path;

#ifdef __WXMSW__

    wxChar name[MAX_PATH] =
        {
            0
        };
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

wxString XmlConfigManager::GetHomeFolder()  const
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

wxString XmlConfigManager::GetConfigFolder()  const
{
    if(XmlConfigManager::config_folder.IsEmpty())
    {
        TCHAR szPath[MAX_PATH];
        SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath);
        XmlConfigManager::config_folder = wxString(szPath) + _T("/codeblocks");
    }
    return XmlConfigManager::config_folder;
}

wxString XmlConfigManager::GetPluginsFolder()  const
{
    return GetExecutableFolder() + _T("/share/codeblocks/plugins");
}

wxString XmlConfigManager::GetDataFolder()  const
{
    if(XmlConfigManager::data_path.IsEmpty())
        XmlConfigManager::data_path = GetExecutableFolder() + _T("/share/codeblocks");
    return XmlConfigManager::data_path;
}

wxString XmlConfigManager::LocateDataFile(const wxString& filename)  const
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

    // else... write to xml file
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
