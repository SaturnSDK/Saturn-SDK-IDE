#include "wxsversionconverter.h"
#include "wxsproject.h"

#include <globals.h>
#include <wx/string.h>

// TODO: This is same as in wxsproject.h, make one set instead of two
namespace
{
    const int CurrentVersion = 1;
    const char* CurrentVersionStr = "1";
}

const wxsVersionConverter& wxsVersionConverter::Get()
{
    static wxsVersionConverter Singleton;
    return Singleton;
}

bool wxsVersionConverter::DetectOldConfig(TiXmlElement* Node,wxsProject* Project) const
{
    // New wxSmith style has resources put into <resources> node
    // and configuration to <gui> node
    if ( Node->FirstChildElement("dialog")        ) return true;
    if ( Node->FirstChildElement("frame")         ) return true;
    if ( Node->FirstChildElement("panel")         ) return true;
    if ( Node->FirstChildElement("configuration") ) return true;

    return false;
}

TiXmlElement* wxsVersionConverter::ConvertFromOldConfig(TiXmlElement* ConfigNode,TiXmlDocument* Doc,wxsProject* Project) const
{
    if ( cbMessageBox(_("This project uses old wxSmith configuration format\n"
                        "Would you like me to convert to new one?\n"),
                      _("wxSmith: Converting from old format"),
                      wxYES_NO) != wxID_YES ) return NULL;

    TiXmlElement* NewConfig = Doc->InsertEndChild(TiXmlElement("wxSmith"))->ToElement();
    TiXmlElement* Resources = NewConfig->InsertEndChild(TiXmlElement("resources"))->ToElement();
    NewConfig->SetAttribute("version",CurrentVersionStr);
    for ( TiXmlElement* Node = ConfigNode->FirstChildElement(); Node; Node = Node->NextSiblingElement() )
    {
        wxString NodeName = cbC2U(Node->Value());
        if ( NodeName == _T("configuration") )
        {
            const char* AppSrc  = Node->Attribute("app_src_file");
            const char* Main    = Node->Attribute("main_resource");
            const char* InitAll = Node->Attribute("init_all_handlers");

            if ( AppSrc )
            {
                TiXmlElement* GUINode = NewConfig->InsertEndChild(TiXmlElement("gui"))->ToElement();
                GUINode->SetAttribute("name","wxWidgets");
                GUINode->SetAttribute("src",AppSrc);
                GUINode->SetAttribute("main",Main?Main:"");
                GUINode->SetAttribute("init_handlers",InitAll?InitAll:"necessary");
                GUINode->SetAttribute("language","CPP");
            }
        }
        else
        {
            if ( NodeName == _T("dialog") ||
                 NodeName == _T("frame") ||
                 NodeName == _T("panel") )
            {
                const char* Wxs   = Node->Attribute("wxs_file");
                const char* Class = Node->Attribute("class");
                const char* Src   = Node->Attribute("src_file");
                const char* Hdr   = Node->Attribute("header_file");
                const char* Xrc   = Node->Attribute("xrc_file");
                const char* Mode  = Node->Attribute("edit_mode");

                if ( Wxs && Class && Src && Hdr && Mode )
                {
                    if ( cbC2U(Mode) == _T("Source") ) Xrc = NULL;
                    TiXmlElement* Res = Resources->InsertEndChild(TiXmlElement(
                        NodeName == _T("dialog") ? "wxDialog" :
                        NodeName == _T("frame")  ? "wxFrame" :
                                                   "wxPanel" ))->ToElement();

                    Res->SetAttribute("wxs",cbU2C(_T("wxsmith/")+cbC2U(Wxs)));
                    Res->SetAttribute("src",Src);
                    Res->SetAttribute("hdr",Hdr);
                    if ( Xrc ) Res->SetAttribute("xrc",Xrc);
                    Res->SetAttribute("name",Class);
                    Res->SetAttribute("language","CPP");

                    ConvertOldWxsFile(Project->GetProjectPath()+_T("wxsmith/")+cbC2U(Wxs));
                }
            }
        }
    }
    return NewConfig;
}

void wxsVersionConverter::ConvertOldWxsFile(const wxString& FileName) const
{
    TiXmlDocument Doc(cbU2C(FileName));
    if ( !Doc.LoadFile() ) return;

    TiXmlElement* Smith = Doc.FirstChildElement("resource");
    if ( Smith )
    {
        Smith->SetValue("wxsmith");
    }

    Doc.SaveFile();
}

TiXmlElement* wxsVersionConverter::Convert(TiXmlElement* ConfigNode,TiXmlDocument* Doc,wxsProject* Project) const
{
    // Currently there's only one version of wxSmith, no need to convert
    return NULL;
}
