
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




