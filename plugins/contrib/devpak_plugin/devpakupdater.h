/***************************************************************
 * Name:      devpakupdater.h
 * Purpose:   Code::Blocks plugin
 * Author:    Yiannis Mandravellos<mandrav@codeblocks.org>
 * Created:   05/12/05 15:44:32
 * Copyright: (c) Yiannis Mandravellos
 * License:   GPL
 **************************************************************/

#ifndef DEVPAKUPDATER_H
#define DEVPAKUPDATER_H

#if defined(__GNUG__) && !defined(__APPLE__)
	#pragma interface "devpakupdater.h"
#endif
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

class DevPakUpdater : public cbToolPlugin
{
	public:
		DevPakUpdater();
		~DevPakUpdater();
		int Configure();
		int Execute();
		void OnAttach(); // fires when the plugin is attached to the application
		void OnRelease(bool appShutDown); // fires when the plugin is released from the application
	protected:
        bool ConfigurationValid();
	private:
};

CB_DECLARE_PLUGIN();

#endif // DEVPAKUPDATER_H

