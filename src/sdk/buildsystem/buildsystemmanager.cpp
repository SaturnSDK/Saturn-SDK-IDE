#include <sdk_precomp.h>

#ifndef CB_PRECOMP
    #include <wx/string.h>
#endif

#include <annoyingdialog.h>
#include "buildsystemmanager.h"


#if defined(__WXMSW__)
const wxString compilerMask(_T("*-windows.xml"));
#include <wx/msw/registry.h>
#elif defined(__UNIX__)
const wxString compilerMask(_T("*-linux.xml"));
#elif defined(__WXMAC__)
const wxString compilerMask(_T("*-macos.xml"));
#else
const wxString compilerMask(_T("unknown"));
#endif


BuildSystemManager::BuildSystemManager() : feelgood(false)
{
    MessageManager *msgman = Manager::Get()->GetMessageManager();
    ConfigManager  *cfg = Manager::Get()->GetConfigManager(_T("buildsystem"));

    wxString compilerDir(ConfigManager::GetDataFolder() + _T("/compilers"));

    cfg->Read(_T("bhv"), &behaviours);
    if(behaviours.empty() && ParseBehaviours(compilerDir) == false)
    {
        msgman->DebugLog(_T("The build system did not find any valid behaviours."));
        return;
    }
msgman->DebugLog(_T("####################### detecting compilers #####################"));

    cfg->Read(_T("map"), &toolchains);
    if(toolchains.empty() && DetectCompilers(compilerDir) == false)
    {
        msgman->DebugLog(_T("The build system could not identify any valid toolchains."));
msgman->DebugLog(_T("####################### end #####################################"));

        return;
    }
msgman->DebugLog(_T("####################### end #####################################"));
    feelgood = true;
}


bool BuildSystemManager::DetectCompilers(const wxString& dir)
{
    TiXmlDocument doc;
    TiXmlElement* root;
    TiXmlElement* element;

    wxArrayString xmlfiles;
    wxDir::GetAllFiles(dir, &xmlfiles, compilerMask);

    typedef std::map<wxString, wxString> StrMap;
    StrMap pathMRU;

    MessageManager *mm = Manager::Get()->GetMessageManager();

    for(unsigned int i = 0; i < xmlfiles.GetCount(); ++i)
    {
        if(TinyXML::LoadDocument(xmlfiles[i], &doc) && (root = doc.FirstChildElement("toolchain")))
        {
        mm->DebugLog(cbC2U("examining file ") << xmlfiles[i]);
        wxString family(cbC2U(root->Attribute("family")));

            ToolChain *tc = new ToolChain(family, cbC2U(root->Attribute("id")), cbC2U(root->Attribute("title")));

            TiXmlElement* tool = 0;
            wxString bt_id;
            wxString bt_exe;
            wxString loc;
            wxString challenge;
            wxString response;
            BuildTool *bt;

            pathMRU.clear();

            while(tool = (TiXmlElement*) root->IterateChildren("tool", tool))
            {
                bt_id = cbC2U(tool->Attribute("id"));
                bt_exe = cbC2U(tool->Attribute("executable"));
                mm->DebugLog(cbC2U("processing tool ") << bt_id << _T("  : ") << bt_exe);

                if(bt_id.IsEmpty())
                {
                    mm->DebugLog(_T("## Missing ID in tool definition."));
                    continue;
                }
                if(bt_exe.IsEmpty())
                {
                    mm->DebugLog(_T("## Missing executable in tool definition."));
                    continue;
                }

                if((element = tool->FirstChildElement("challenge")) && element->FirstChild() && element->FirstChild()->ToText())
                    challenge = cbC2U(element->FirstChild()->ToText()->Value());

                if((element = tool->FirstChildElement("response")) && element->FirstChild() && element->FirstChild()->ToText())
                    response = cbC2U(element->FirstChild()->ToText()->Value());


                element = 0;
                while((element = (TiXmlElement*) tool->IterateChildren("location", element)) && element->FirstChild() && element->FirstChild()->ToText())
                {
                    loc = cbC2U(element->FirstChild()->ToText()->Value());
                    if(loc.GetChar(0) == _T('['))
                    {
                        mm->DebugLog(cbC2U("looking up  ") << loc);
                        loc = loc.Mid(1, loc.length()-2);
                        StrMap::iterator it = pathMRU.find(loc);
                        if(it != pathMRU.end())
                            {
                            mm->DebugLog(cbC2U(" --->  ") << it->second);
                            loc = it->second;
                            }
                        else
                            mm->DebugLog(cbC2U(" ---> not found"));
                }
                else
                {
                    mm->DebugLog(cbC2U("adding location  ") << loc);
                }

                pathMRU[bt_id] = loc;

                if(bt = DetectBuildTool(bt_id, family, loc, bt_exe, challenge, response))
                    *tc += bt;
                }

//                element = 0;
//                while(element = (TiXmlElement*) tool->IterateChildren("search", tool))
//                {
//                    if(element->Attribute("location"))
//                        bt <= cbC2U(element->Attribute("location"));
//
//#if(__WXMSW__)
//
//                    if(element->Attribute("registry"))
//                    {
//                        wxString r(cbC2U(element->Attribute("registry")));
//                        wxRegKey k(r.BeforeLast('\\'));
//                        if(k.Exists() && k.HasValue(r.AfterLast('\\')))
//                        {
//                            wxString path;
//                            k.QueryValue(r.AfterLast('\\'), path);
//                            bt <= path;
//                        }
//                    }
//#endif
//
//                }
//

if(tc)
toolchains[tc->id()] = tc;

//                bt && (toolchain += bt);
            }
        }
    }
    return !toolchains.empty();
}


BuildTool* BuildSystemManager::DetectBuildTool(const wxString& new_id, const wxString& the_family, const wxString& location, const wxString& executable, const wxString& challenge, const wxString& response)
{
    wxArrayString candidates;
    wxArrayString out;
    wxString out_str;

    wxLogNull makeWxWidgetsSomewhatLessAnnoying;

    MessageManager *mm = Manager::Get()->GetMessageManager();

    wxRegEx reg(response);
    if(reg.IsValid() == false)
    {
        wxString tmp(_T("Bad expression: "));
        mm->DebugLog(tmp + response);
        return 0;
    }

    if(wxDir::Exists(location))
    {
        mm->DebugLog(cbC2U("looking into directory ") << location);
        wxDir::GetAllFiles(location, &candidates, executable);
    }
    else if(wxFile::Exists(location))
    {
        mm->DebugLog(cbC2U("trying exact location ") << location);
        candidates.Add(location);
    }
    else
        mm->DebugLog(cbC2U("discarding non-existent location: ") << location);

    for(unsigned int i = 0; i < candidates.GetCount(); ++i)
    {
        Manager::Get()->GetMessageManager()->DebugLog(candidates[i] + _T(" ") + challenge +_T(" ..."));
        if(wxExecute(candidates[i] + _T(" ") + challenge, out, wxEXEC_SYNC | wxEXEC_NODISABLE) != -1)
        {
            out_str = GetStringFromArray(out);
            if(!challenge.IsEmpty() && reg.Matches(out_str))
            {
                out_str = reg.GetMatch(out_str, (size_t) 1);
                out_str.Printf(_T("%s (%s)"), new_id.c_str(), out_str.c_str());
                mm->DebugLog(out_str + _T(" detected"));
                return new BuildTool(out_str, the_family, candidates[i]);
            }
            else
            {
                mm->DebugLog(out_str);
                mm->DebugLog(_T("(failed challenge)"));
            }
        }
        else
        {
            mm->DebugLog(_T("(execution failed)"));
        }
    }
return 0;
}

bool BuildSystemManager::ParseBehaviours(const wxString& dir)
{
    //    TiXmlDocument doc;
    //    TiXmlElement* root;
    //
    //    if(TinyXML::LoadDocument(dir + _T("behaviours.xml"), &doc) && (root = doc.FirstChildElement("buildsystem")))
    //    {
    //        TiXmlElement* family = 0;
    //        while(family = (TiXmlElement*) root->IterateChildren("family", family))
    //        {
    //            ToolChain tc(cbC2U(family->Attribute("id")), cbC2U(family->Attribute("title")));
    //
    //            TiXmlElement* stage = 0;
    //            while(stage = (TiXmlElement*) family->IterateChildren("stage", stage))
    //            {
    //                if(TiXmlText *t = stage->FirstChild()->ToText())
    //                    tc.AddStage(cbC2U(stage->Attribute("id")), cbC2U(t->Value()));
    //            }
    //            if(tc)
    //                behaviour[tc.GetID()] = tc;
    //            return true;
    //        }
    //    }
    //    return behaviour.empty() == false;
    return true;
}






//wxDir dir(compilerDir);
//
//bool cont = dir.GetFirst(&filename, filespec, flags);
//    while ( cont )
//    {
//        printf("%s\n", filename.c_str());
//
//        cont = dir.GetNext(&filename);
//    }

void BuildSystemManager::BailOut() const
{
    AnnoyingDialog dlg( _("No build system available"),
                        _("Code::Blocks is being run without any valid build system being available.\n"
                          "Although that is generally possible, it may not be what you want (the likely reason is a bad installation).\n\n"
                          "Please make sure your installation (including compiler) is valid if you intend to use Code::Blocks "
                          "for more than just a bloated text editor."),
                        wxART_INFORMATION, AnnoyingDialog::OK, wxID_OK);
    dlg.ShowModal();
}
