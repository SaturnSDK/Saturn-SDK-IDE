
// too lazy to deal with event handlers, we will abuse the "About" box to start a test suite
// #include this from dlgabout.cpp


XmlConfigManager *c = Manager::Get()->GetConfigManager("main");
MessageManager *m = Manager::Get()->GetMessageManager();

m->DebugLog("APP_PATH=" +  c->Read("app_path"));

c->SetPath("/foo");
c->SetPath("/bar/some/path");
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
m->DebugLog(wxString("exists prz=") << c->Exists("prz"));
c->UnSet("prz");
m->DebugLog(wxString("exists prz=") << c->Exists("prz"));

m->DebugLog(wxString("read /foo/honk=") << c->ReadInt("/foo/honk", -1));
m->DebugLog(wxString("read /foo/honk=") << c->ReadDouble("/foo/honk", -5.5));
m->DebugLog(wxString("read /invalid=") << c->ReadInt("/invalid", 4242));

wxString a("hello world");
m->DebugLog(a);
m->DebugLog(wxBase64Encode(a));
m->DebugLog(wxBase64Decode(wxBase64Encode(a)));

