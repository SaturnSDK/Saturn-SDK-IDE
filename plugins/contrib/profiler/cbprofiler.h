/***************************************************************
 * Name:      cbprofiler.h
 * Purpose:   Code::Blocks plugin
 * Author:    Dark Lord & Zlika
 * Created:   07/20/05 11:12:57
 * Copyright: (c) Dark Lord & Zlika
 * Thanks:    Yiannis Mandravellos and his Source code formatter (AStyle) sources
 * License:   GPL
 **************************************************************/

#ifndef CBPROFILER_H
#define CBPROFILER_H

#include "cbplugin.h" // the base class we 're inheriting

class cbConfigurationPanel;
class CBProfilerExecDlg;
class wxWindow;

/*
 * Save per pro config, or should I use exe path?
 * User can make a global variable, or per project? with the name of the file to profile or path (2 vars)
 * User can chose the target to profile
 */

class CBProfiler : public cbToolPlugin
{
	public:
		CBProfiler();
		~CBProfiler();
		int GetConfigurationGroup(){ return cgCompiler; }
        cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
		int Execute();
		void OnAttach(); // fires when the plugin is attached to the application
		void OnRelease(bool appShutDown); // fires when the plugin is released from the application
		//void ShowDialog();
	private:
        CBProfilerExecDlg* dlg;
};

CB_DECLARE_PLUGIN();

#endif // CBPROFILER_H

