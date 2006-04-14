/*
This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision$
* $Id $
* $HeadURL$
*/

#ifndef AUTOSAVE_H
#define AUTOSAVE_H

#include <wx/intl.h>
#include <wx/string.h>

#include "cbplugin.h" // the base class we 're inheriting

class wxTimer;
class wxTimerEvent;

class Autosave : public cbPlugin
{
    wxTimer *timer1;
    wxTimer *timer2;

	public:
		Autosave();
		~Autosave();
		int Configure();
		int GetConfigurationPriority() const{ return  50; }
		int GetConfigurationGroup() const{ return  cgUnknown; }
		void BuildMenu(wxMenuBar* menuBar){ return ; }
		void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0){ return ; }
		bool BuildToolBar(wxToolBar* toolBar){ return false; }
		void OnAttach(); // fires when the plugin is attached to the application
		void OnRelease(bool appShutDown); // fires when the plugin is released from the application
		virtual cbConfigurationPanel* Autosave::GetConfigurationPanel(wxWindow* parent);
        void Autosave::OnTimer(wxTimerEvent& event);
    DECLARE_EVENT_TABLE()
};


class AutosaveConfigDlg : public cbConfigurationPanel
{
    Autosave* plugin;

	public:
		AutosaveConfigDlg(wxWindow* parent, Autosave* plug);
		virtual ~AutosaveConfigDlg(){};

        virtual wxString GetTitle(){ return _T("Autosave"); }
        virtual wxString GetBitmapBaseName(){ return _T("autosave"); }
        virtual void OnApply(){ SaveSettings(); }
        virtual void OnCancel(){}

        void LoadSettings();
        void SaveSettings();
};


// Declare the plugin's hooks
CB_DECLARE_PLUGIN();

#endif // AUTOSAVE_H

