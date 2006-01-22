/***************************************************************
 * Name:      astyle.h
 * Purpose:   Code::Blocks plugin
 * Author:    Yiannis Mandravellos<mandrav@codeblocks.org>
 * Created:   05/25/04 10:06:39
 * Copyright: (c) Yiannis Mandravellos
 * License:   GPL
 **************************************************************/

#ifndef ASTYLEPLUGIN_H
#define ASTYLEPLUGIN_H

// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <cbplugin.h> // the base class we 're inheriting
#include <settings.h> // needed to use the Code::Blocks SDK

class AStylePlugin : public cbToolPlugin
{
  public:
    AStylePlugin();
    ~AStylePlugin();
    int Configure();
    cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
    int Execute();
    void OnAttach(); // fires when the plugin is attached to the application
    void OnRelease(bool appShutDown); // fires when the plugin is released from the application
};

CB_DECLARE_PLUGIN();

#endif // ASTYLEPLUGIN_H
