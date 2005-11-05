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



XmlConfigManager::XmlConfigManager()
{}




CfgMgrBldr::CfgMgrBldr()
{
  wxString personality(Manager::Get()->GetPersonalityManager()->GetPersonality());

  // personality = xxxFolder + personality...

  if(!doc.LoadFile(personality.mb_str()))
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

    wxString base;
#ifdef __WXMSW__

    wxChar name[MAX_PATH] =
      {
        0
      };
    GetModuleFileName(0L, name, MAX_PATH);
    wxFileName fname(name);
    base = fname.GetPath(wxPATH_GET_VOLUME);
#else

    if (!m_Prefix.IsEmpty())
      return m_Prefix;

    // SELFPATH is a macro from prefix.h (binreloc)
    // it returns the absolute filename of us
    // similar to win32 GetModuleFileName()...
    base = wxString(SELFPATH,wxConvUTF8);
    base = wxFileName(base).GetPath();
    if (base.IsEmpty())
      base = _T(".");
#endif

    return base;
  }

wxString XmlConfigManager::GetHomeFolder()  const
  {
    wxFileName f;
    f.AssignHomeDir();
    return f.GetFullPath();
  }


#ifdef __WXMSW__

wxString XmlConfigManager::GetConfigFolder()  const
  {
    TCHAR szPath[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath);
    return wxString(szPath) + _T("codeblocks");
  }

wxString XmlConfigManager::GetPluginsFolder()  const
  {
    return GetExecutableFolder() + _T("/share/codeblocks/plugins");
  }

wxString XmlConfigManager::GetDataFolder()  const
  {
    return GetExecutableFolder() + _T("/share/codeblocks");
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
    return GetHomeFolder() + _T("/.codeblocks");
  }

wxString XmlConfigManager::GetPluginsFolder()  const
  {
    return wxString(_T("/usr/share/codeblocks/plugins"));
  }

wxString XmlConfigManager::GetDataFolder()  const
  {
    return wxString(_T("/usr/share/codeblocks"));
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

