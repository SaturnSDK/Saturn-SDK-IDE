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
*  Usage:
*  ------
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
    ISerializable();
    virtual ~ISerializable();
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

    WX_DECLARE_STRING_HASH_MAP(ISerializable *, SerializableObjectMap);

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

    void Write(const wxString& name,  bool value);
    bool Read(const wxString& name,  bool* value);
    bool  ReadBool(const wxString& name,  bool defaultVal = false);

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
    bool Read(const wxString& name, ISerializable* object);
    // Since ConfigManager has no knowledge of the real class, there is no such thing as
    // ISerializable ReadObject(const wxString& name)

    void Write(const wxString& name, const ConfigManagerContainer::StringToStringMap& map);
    void Read(const wxString& name, ConfigManagerContainer::StringToStringMap* map);
    ConfigManagerContainer::StringToStringMap ReadSSMap(const wxString& name);


    /* -----------------------------------------------------------------------------------------------------
    *  Maps and sets of serialized objects. You are responsible for deleting the objects in the map/set.
    *
    *
    *  Usage:
    *  ------
    *  WX_DECLARE_STRING_HASH_MAP(MySerializableClass *, MyMap);
    *  MyMap objMap;
    *  cfg->Read<MySerializableClass>("name", (ConfigManagerContainer::SerializableObjectMap*) &objMap);
    *  map["somekey"]->DoSomething();
    */
    void XmlConfigManager::Write(const wxString& name, const ConfigManagerContainer::SerializableObjectMap* map);
    template <class T> void Read(const wxString& name, ConfigManagerContainer::SerializableObjectMap *map)
    {
        wxString key(name);
        TiXmlHandle ph(AssertPath(key));
        TiXmlElement* e = 0;
        if(TiXmlNode *n = ph.FirstChild(key).FirstChild(_T("objmap")).Node())
            while(e = n->IterateChildren(e)->ToElement())
            {
                T *obj = new T;
                obj->SerializeIn(wxBase64Decode(e->FirstChild()->ToText()->Value()));
                (*map)[e->Value()] = obj;
            }
    };


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

    MySerializableLongIntClass() : m_int(0)
    {}
    ;

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

WX_DECLARE_STRING_HASH_MAP(MySerializableLongIntClass *, MyThingieMap);

//############################# END OF "REMOVE THIS" ########################################

#endif

