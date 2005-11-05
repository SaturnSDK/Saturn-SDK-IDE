#ifndef X_CONFIGMANAGER_H
#define X_CONFIGMANAGER_H

#include <wx/wx.h>
#include "settings.h"


class DLLIMPORT XmlConfigManager
  {
    friend class CfgMgrBldr;
    TiXmlElement* root;
    wxString path;
    XmlConfigManager();

  public:

    wxString LocateDataFile(const wxString& filename) const;

    wxString GetHomeFolder() const;
    wxString GetConfigFolder() const;
    wxString GetPluginsFolder() const;
    wxString GetDataFolder() const;
    wxString GetExecutableFolder() const;


//    wxString GetPath() const;
//    void SetPath(const wxString& strPath);
//
//    bool Write(const wxString& name,  bool value);
//    bool Write(const wxString& name,  int value);
//    bool Write(const wxString& name,  long int value);
//    bool Write(const wxString& name,  float value);
//    bool Write(const wxString& name,  double value);
//    bool Write(const wxString& name,  const wxString& value);
//    bool Write(const wxString& name,  const wxArrayString& value);
//    bool Set(const wxString& name);
//    bool UnSet(const wxString& name);
//
//    bool Read(const wxString& key, wxString* str);
//    bool Read(const wxString& key, wxString* str, const wxString& defaultVal);
//    wxString Read(const wxString& key, const wxString& defaultVal = wxEmptyString);
//
//    bool Read(const wxString& key, long int* l);
//    bool Read(const wxString& key, long int* l, long defaultVal);
//    long int Read(const wxString& key, long int defaultVal);
//
//    bool Read(const wxString& key, double* d);
//    bool Read(const wxString& key, double* d, double defaultVal);
//
//    bool Read(const wxString& key, bool* b);
//
//    bool IsSet(const wxString& name){return KeyExists(name);};
//
//    bool GetFirstGroup(wxString& str, long& index);
//    bool GetNextGroup(wxString& str, long& index);
//    bool RenameGroup(const wxString&  oldName, const wxString&  newName);
//    bool GetFirstEntry(wxString& str, long& index);
//    bool GetNextEntry(wxString& str, long& index);
//    unsigned int GetNumberOfGroups(bool bRecursive = FALSE);
//    bool KeyExists(const wxString& name);
  };



WX_DECLARE_STRING_HASH_MAP(XmlConfigManager*, Map);

class DLLIMPORT CfgMgrBldr
  {
    Map map;
    TiXmlDocument doc;
    bool readonly;

    CfgMgrBldr();
    ~CfgMgrBldr();
    XmlConfigManager* Instantiate(const wxString& name_space);
  public:
    static XmlConfigManager* Get(const wxString& name_space);
  };


#endif

