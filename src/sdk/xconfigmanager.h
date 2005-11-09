#ifndef X_CONFIGMANAGER_H
#define X_CONFIGMANAGER_H

#include <wx/wx.h>
#include <wx/hashmap.h>
#include <wx/hashset.h>

#include "settings.h"



/* ------------------------------------------------------------------------------------------------------------------
*  Interface Serializable
*  ConfigManager can save arbitrary objects and even sets/maps of objects, provided they implement Serializable.
*
*  Example usage:
*   class MySerializableLongIntClass : public ISerializable
*   {
*   //...
*   wxString SerializeOut() const {wxString tmp; tmp << m_int; return tmp;};
*   void SerializeIn(const wxString& s){s.ToLong(&m_int);};
*   //...
*   long int m_int;
*   };
*/
class ISerializable
{
    public:
    virtual ~ISerializable(){};
    virtual wxString SerializeOut() const = 0;
    virtual void SerializeIn(const wxString& s) = 0;
};



/* ------------------------------------------------------------------------------------------------------------------
*  Containers supported by ConfigManager
*/
namespace ConfigManagerContainer
{
    WX_DECLARE_STRING_HASH_MAP( long int, StringToIntMap);
    WX_DECLARE_STRING_HASH_MAP( wxString, StringToStringMap);
    WX_DECLARE_HASH_MAP(long int, wxString, wxIntegerHash, wxIntegerEqual, IntToStringMap);

    WX_DECLARE_STRING_HASH_MAP( ISerializable *, SerializableObjectMap);
    WX_DECLARE_HASH_MAP(int, ISerializable *, wxIntegerHash, wxIntegerEqual, IntSerializableObjectMap);

    WX_DECLARE_HASH_SET(wxString, wxStringHash, wxStringEqual, StringSet);
};



/* ------------------------------------------------------------------------------------------------------------------
*  ConfigManager class
*/
class DLLIMPORT XmlConfigManager
{
    friend class CfgMgrBldr;

    TiXmlDocument *doc;
    TiXmlElement* root;
    TiXmlElement* pathNode;

    static wxString app_path;
    static wxString data_path;
    static wxString config_folder;
    static wxString home_folder;

    XmlConfigManager(TiXmlElement* r);
    TiXmlElement* AssertPath(wxString& path);
    TiXmlElement* GetUniqElement(TiXmlElement* p, const wxString& q);
    void SetNodeText(TiXmlElement *n, const TiXmlText& t);

public:

    static wxString LocateDataFile(const wxString& filename);

    static wxString GetHomeFolder();
    static wxString GetConfigFolder();
    static wxString GetPluginsFolder();
    static wxString GetDataFolder();
    static wxString GetExecutableFolder();

    wxString GetPath() const;
    void SetPath(const wxString& strPath);

    void Write(const wxString& name,  const wxString& value);
    wxString Read(const wxString& key, const wxString& defaultVal = wxEmptyString);
    bool Read(const wxString& key, wxString* str);

    void Write(const wxString& name,  int value);
    bool Read(const wxString& name,  int* value);
    int  ReadInt(const wxString& name,  int defaultVal = 0);

    void Write(const wxString& name,  double value);
    bool Read(const wxString& name,  double* value);
    double ReadDouble(const wxString& name,  double defaultVal = 0.0f);

    bool Exists(const wxString& name);
    void Set(const wxString& name);
    void UnSet(const wxString& name);

    void Write(const wxString& name,  const wxArrayString& as);
    void Read(const wxString& name,  wxArrayString* as);
    wxArrayString ReadArrayString(const wxString& name);

    void WriteBinary(const wxString& name,  const wxString& source);
    void WriteBinary(const wxString& name,  void* ptr, size_t len);
    wxString ReadBinary(const wxString& name);

    wxArrayString EnumerateSubPaths(const wxString& path);
    wxArrayString EnumerateKeys(const wxString& path);


    void Write(const wxString& name, const ISerializable& object);
    bool ReadObject(const wxString& name, ISerializable* object);

//    void Write(const wxString& name, const StringToIntMap& map);
//    void Write(const wxString& name, const StringToStringMap& map);
//    void Write(const wxString& name, const IntToStringMap& map);


};



/* ------------------------------------------------------------------------------------------------------------------
*  "Builder pattern" class for XmlConfigManager
*  Do not use this class.
*  --->  use Manager::Get()->GetConfigManager("yournamespace") instead.
*
*  Manager::Get()->GetConfigManager("yournamespace") is *guaranteed* to always work, and unlike the builder class,
*  it is aware of applicaton shutdowns.
*  The builder class will be changed in the near future. You have been warned.
*/

WX_DECLARE_STRING_HASH_MAP(XmlConfigManager*, NamespaceMap);

class DLLIMPORT CfgMgrBldr
{
    NamespaceMap namespaces;
    TiXmlDocument *doc;

    CfgMgrBldr();
    ~CfgMgrBldr();
    void Close();
    XmlConfigManager* Instantiate(const wxString& name_space);
public:
    static XmlConfigManager* Get(const wxString& name_space);
    void SwitchTo(const wxString& absFN);
};










//############################# REMOVE THIS ########################################
class MySerializableLongIntClass : public ISerializable
{
public:
    wxString SerializeOut() const
    {
        wxString tmp;
        tmp << m_int;
        return tmp;
    };
    void SerializeIn(const wxString& s)
    {
        s.ToLong(&m_int);
    };

    MySerializableLongIntClass(long int x)
    {
        m_int = x;
    };

    void Print()
    {
        Manager::Get()->GetMessageManager()->DebugLog(wxString("my value is ") << m_int);
    };
    long int m_int;
};
//############################# END OF "REMOVE THIS" ########################################

#endif

