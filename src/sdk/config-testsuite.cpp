
// too lazy to deal with event handlers, we will abuse the "About" box to start a test suite
// #include this from dlgabout.cpp


XmlConfigManager *c = Manager::Get()->GetConfigManager("main");
MessageManager *m = Manager::Get()->GetMessageManager();

m->DebugLog("APP_PATH=" +  c->Read("app_path"));

c->SetPath("/foo");
c->SetPath("/bar/some/path");
c->SetPath("/bar/another");
c->SetPath("/bar/yetanother");
c->SetPath("/trailing/slash/");

m->DebugLog("PATH=" +  c->GetPath());

c->SetPath("../upalevel");

m->DebugLog("PATH=" +  c->GetPath());

c->Write("myname", "Thomas");
c->Write("/top", "bottom");


m->DebugLog("read myname=" + c->Read("myname"));
m->DebugLog("read /top=" + c->Read("/top"));

c->Write("/foo/blah", 5);
c->Write("/foo/honk", 2);
c->Write("/foo/honk", 44.8);

m->DebugLog(wxString("key_exists /foo/zork=") << c->Exists("/foo/zork"));

m->DebugLog(wxString("key_exists /foo/honk=") << c->Exists("/foo/honk"));

c->Set("prz");
c->UnSet("prz");

m->DebugLog(wxString("exists blah=") << c->Exists("/foo/blah"));
m->DebugLog(wxString("exists honk=") << c->Exists("/foo/honk"));
m->DebugLog(wxString("exists invalid=") << c->Exists("/invalid"));

m->DebugLog(wxString("read /foo/blah=") << c->ReadInt("/foo/blah", -1));
m->DebugLog(wxString("read /foo/honk=") << c->ReadInt("/foo/honk", -1));
m->DebugLog(wxString("read /foo/honk=") << c->ReadDouble("/foo/honk", -5.5));

m->DebugLog(wxString("read /invalid=") << c->ReadInt("/invalid", 4242));

{
    wxArrayString as;
    as.Add("some");
    as.Add("items");
    as.Add("in a wxArrayString");
    as.Add("saved");
    as.Add("here in config");

    c->Write("/trailing/slash/data", as);
}
wxArrayString as = c->ReadArrayString("/trailing/slash/data");
for(unsigned int i = 0; i < as.GetCount(); ++i)
    m->DebugLog(wxString("arraystring element: ") << as[i]);


c->WriteBinary("/trailing/slash/window-layout", wxString("Binary data. This can be anything which fits in a wxString or a pointer plus a size_t argument."));
m->DebugLog(c->ReadBinary("/trailing/slash/window-layout"));

wxArrayString enumerated;
enumerated = c->EnumerateSubPaths("/");
for(unsigned int i = 0; i < enumerated.GetCount(); ++i)
    m->DebugLog(wxString("subpaths of / : ") << enumerated[i]);

enumerated.Clear();
enumerated = c->EnumerateSubPaths("/bar/");
for(unsigned int i = 0; i < enumerated.GetCount(); ++i)
    m->DebugLog(wxString("subpaths of /bar : ") << enumerated[i]);


enumerated.Clear();
enumerated = c->EnumerateKeys("/bar"); // will be empty - bar does not contain keys
for(unsigned int i = 0; i < enumerated.GetCount(); ++i)
    m->DebugLog(wxString("keys in /bar : ") << enumerated[i]);

enumerated.Clear();
enumerated = c->EnumerateKeys("/foo");
for(unsigned int i = 0; i < enumerated.GetCount(); ++i)
    m->DebugLog(wxString("keys in /foo : ") << enumerated[i]);


//******* declaration in configmanager.h (remove before release!)
MySerializableLongIntClass serialLong(5);
MySerializableLongIntClass otherSerialLong(0);

serialLong.Print();
otherSerialLong.Print();
c->Write("nonsense", serialLong);
c->Read("nonsense", &otherSerialLong);
serialLong.Print();
otherSerialLong.Print();


ConfigManagerContainer::StringToStringMap map;
map["a"] = "the letter A";
map["b"] = "the letter B";
map["c"] = "the letter C";
c->Write("/foo/thismapsletters", map);

ConfigManagerContainer::StringToStringMap pam = c->ReadSSMap("/foo/thismapsletters");
m->DebugLog(pam["a"]);
m->DebugLog(pam["b"]);
m->DebugLog(pam["c"]);


MyThingieMap objMap1; // WX_DECLARE_STRING_HASH_MAP(MySerializableLongIntClass *, MyThingieMap);
MyThingieMap objMap2;
objMap1["a"] = new MySerializableLongIntClass(1234);
c->Write("boo", (ConfigManagerContainer::SerializableObjectMap*) &objMap1);
c->Read<MySerializableLongIntClass>("boo", (ConfigManagerContainer::SerializableObjectMap*) &objMap2);
objMap2["a"]->Print();



c->Write("/foo/one_or_zero", true);
m->DebugLog(wxString("read /foo/one_or_zero=") << c->ReadBool("/foo/one_or_zero"));
c->Write("/foo/one_or_zero", false);
m->DebugLog(wxString("read /foo/one_or_zero=") << c->ReadBool("/foo/one_or_zero"));

XmlConfigManager *cv = Manager::Get()->GetConfigManager("volatile:main");  // volatile offers DOM to store temporary data
cv->Write("word", "table");                                                // the volatile DOM is not saved to disk
m->DebugLog(wxString("main word:") << c->Read("word"));
m->DebugLog(wxString("volatile word:") << cv->Read("word"));
m->DebugLog(wxString("volatile top:") << cv->Read("/top"));


