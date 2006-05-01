#ifndef BLOCKS_H
#define BLOCKS_H


#include <cbplugin.h>
#include <settings.h>
#include "configurationpanel.h"

class wxWindow;

class Blocks : public cbToolPlugin
{
	public:
		Blocks();
		~Blocks();
		int GetConfigurationPriority() const{ return  25; }
		int GetConfigurationGroup() const{ return  cgUnknown; }
		cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
		int Execute();
		void OnAttach();
		void OnRelease(bool appShutDown);
	protected:
	private:
};

CB_DECLARE_PLUGIN();


class BlocksConfigDlg : public cbConfigurationPanel
{
    Blocks* plugin;

	public:
		BlocksConfigDlg(wxWindow* parent, Blocks* plug);
		virtual ~BlocksConfigDlg(){};

        virtual wxString GetTitle() const { return _T("Blocks - organising blocks"); }
        virtual wxString GetBitmapBaseName() const { return _T("blocks"); }
        virtual void OnApply(){ SaveSettings(); }
        virtual void OnCancel(){}

        void LoadSettings();
        void SaveSettings();
};

#endif // BLOCKS_H

