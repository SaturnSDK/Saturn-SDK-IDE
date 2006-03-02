/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "buildtoolmanager.h"


void BuildToolManager::DetectBuildTools()
{
    wxString cDir(Manager::Get()->GetConfigManager()->GetDataFolder() + _T("/BuildTools"));

    ::wxSetWorkingDirectory(cDir);

    TiXmlDocument layout("layout.xml");
    if(doc.Error())
    {
        Manager::Get()->GetMessageManager() && Manager::Get()->GetMessageManager()->DebugLog(cbC2U(doc.ErrorDesc()));
        BailOut();
        return;
    };

    TiXmlDocument layout("behaviours.xml");
    if(doc.Error())
    {
        Manager::Get()->GetMessageManager() && Manager::Get()->GetMessageManager()->DebugLog(cbC2U(doc.ErrorDesc()));
        BailOut();
        return;
    };

    bool found_some = false;


    if(TiXmlElement* buildsystem = layout.RootElement())
    {
        found_some |= DetectBuildTool(..................);
    };


    if(found_some == false)
        BailOut();
}


BuildToolManager::BailOut()
{
    AnnoyingDialog dlg( _("No build system available"),
                        _("Code::Blocks is being run without any valid build system being available.\n"
                          "Although that is generally possible, it may not be what you want (the likely reason is a bad installation).\n\n"
                          "Please make sure your installation (including compiler) is valid if you intend to use Code::Blocks "
                          "for more than just a bloated text editor."),
                        wxART_INFORMATION, AnnoyingDialog::OK, wxID_OK);
    dlg.ShowModal();
}
