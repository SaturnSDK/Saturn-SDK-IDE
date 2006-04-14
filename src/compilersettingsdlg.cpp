/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include <sdk.h>
#include <wx/xrc/xmlres.h>
#include <manager.h>
#include <configmanager.h>
#include <pluginmanager.h>
#include <cbplugin.h>
#include <wx/intl.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>

#include "configurationpanel.h"
#include "compilersettingsdlg.h"
#include "appglobals.h"
#ifdef __WXMSW__
    #include "associations.h"
#endif

// images by order of pages
const wxString base_imgs[] =
{
    _T("batch"),
};
const int IMAGES_COUNT = sizeof(base_imgs) / sizeof(wxString);

BEGIN_EVENT_TABLE(CompilerSettingsDlg, wxDialog)
    EVT_LISTBOOK_PAGE_CHANGING(XRCID("nbMain"), CompilerSettingsDlg::OnPageChanging)
    EVT_LISTBOOK_PAGE_CHANGED(XRCID("nbMain"), CompilerSettingsDlg::OnPageChanged)
END_EVENT_TABLE()

// This dialog initially contains only the batch-build settings.
// So we 'll add all compiler-related configuration before it
// and all debugger-related after it...

CompilerSettingsDlg::CompilerSettingsDlg(wxWindow* parent)
{
    wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgCompilerSettings"));
    wxListbook* lb = XRCCTRL(*this, "nbMain", wxListbook);
    wxImageList* images = new wxImageList(80, 80);
    lb->AssignImageList(images);
    int sel = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/environment/settings_size"), 0);
    SetSettingsIconsStyle(lb->GetListView(), (SettingsIconsStyle)sel);

    // tab "Batch builds"
#ifdef __WXMSW__
    ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("app"));
    XRCCTRL(*this, "txtBatchBuildsCmdLine", wxTextCtrl)->SetValue(cfg->Read(_T("/batch_build_args"), g_DefaultBatchBuildArgs));
#else
    XRCCTRL(*this, "txtBatchBuildsCmdLine", wxTextCtrl)->Enable(false);
#endif

    // add all plugins configuration panels
    AddPluginPanels();

    // make sure everything is laid out properly
    GetSizer()->SetSizeHints(this);
    CenterOnParent();
}

CompilerSettingsDlg::~CompilerSettingsDlg()
{
    //dtor
}

void CompilerSettingsDlg::AddPluginPanels()
{
    const wxString base = ConfigManager::GetDataFolder() + _T("/images/settings/");
    wxListbook* lb = XRCCTRL(*this, "nbMain", wxListbook);

    // we 'll remove the existing page and add it when appropriate
    wxWindow* existingPage = lb->GetPage(0);
    wxString existingTitle = lb->GetPageText(0);
    lb->RemovePage(0);

    // we 'll do this in two steps:
    // 1) get all compiler-related panels and put them before batch-builds
    // 2) get all debugger panels and put them after batch-builds

    ConfigurationPanelsArray local;

    // step 1
    Manager::Get()->GetPluginManager()->GetConfigurationPanels(cgCompiler, lb, local);
    for (size_t i = 0; i < local.GetCount(); ++i)
    {
        cbConfigurationPanel* panel = local[i];
        lb->AddPage(panel, panel->GetTitle());

        lb->GetImageList()->Add(LoadPNGWindows2000Hack(base + panel->GetBitmapBaseName() + _T(".png")));
        lb->GetImageList()->Add(LoadPNGWindows2000Hack(base + panel->GetBitmapBaseName() + _T("-off.png")));
        lb->SetPageImage(lb->GetPageCount() - 1, lb->GetImageList()->GetImageCount() - 2);

        // add it in our central container too
        m_PluginPanels.Add(panel);
    }

    // now load the builtin pages' images
    lb->AddPage(existingPage, existingTitle);
    wxBitmap bmp;
    bmp.LoadFile(base + base_imgs[0] + _T(".png"), wxBITMAP_TYPE_PNG);
    lb->GetImageList()->Add(bmp);
    bmp.LoadFile(base + base_imgs[0] + _T("-off.png"), wxBITMAP_TYPE_PNG);
    lb->GetImageList()->Add(bmp);
    lb->SetPageImage(lb->GetPageCount() -1, lb->GetImageList()->GetImageCount() - 2);

    // step 2
    Manager::Get()->GetPluginManager()->GetConfigurationPanels(cgDebugger, lb, local);
    for (size_t i = 0; i < local.GetCount(); ++i)
    {
        cbConfigurationPanel* panel = local[i];
        lb->AddPage(panel, panel->GetTitle());

        lb->GetImageList()->Add(LoadPNGWindows2000Hack(base + panel->GetBitmapBaseName() + _T(".png")));
        lb->GetImageList()->Add(LoadPNGWindows2000Hack(base + panel->GetBitmapBaseName() + _T("-off.png")));
        lb->SetPageImage(lb->GetPageCount() - 1, lb->GetImageList()->GetImageCount() - 2);

        // add it in our central container too
        m_PluginPanels.Add(panel);
    }

    UpdateListbookImages();
}

void CompilerSettingsDlg::UpdateListbookImages()
{
    wxListbook* lb = XRCCTRL(*this, "nbMain", wxListbook);
    int sel = lb->GetSelection();
    // set page images according to their on/off status
    for (size_t i = 0; i < IMAGES_COUNT + m_PluginPanels.GetCount(); ++i)
    {
        lb->SetPageImage(i, (i * 2) + (sel == (int)i ? 0 : 1));
    }

    // the selection color is ruining the on/off effect,
    // so make sure no item is selected ;)
    // (only if we have icons showing)
    if (GetSettingsIconsStyle(lb->GetListView()) != sisNoIcons)
        lb->GetListView()->Select(sel, false);

    // update the page title
    wxString label = lb->GetPageText(sel);
    // replace any stray & with && because label makes it an underscore
    while (label.Replace(_T(" & "), _T(" && ")))
        ;
    XRCCTRL(*this, "lblBigTitle", wxStaticText)->SetLabel(label);
}

void CompilerSettingsDlg::OnPageChanging(wxListbookEvent& event)
{
}

void CompilerSettingsDlg::OnPageChanged(wxListbookEvent& event)
{
    UpdateListbookImages();
}

void CompilerSettingsDlg::EndModal(int retCode)
{
    if (retCode == wxID_OK)
    {
        // tab "Batch builds"
#ifdef __WXMSW__
        ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("app"));
        wxString bbargs = XRCCTRL(*this, "txtBatchBuildsCmdLine", wxTextCtrl)->GetValue();
        if (bbargs != cfg->Read(_T("/batch_build_args"), g_DefaultBatchBuildArgs))
        {
            cfg->Write(_T("/batch_build_args"), bbargs);
            Associations::SetBatchBuildOnly();
        }
#endif

        // finally, apply settings in all plugins' panels
        for (size_t i = 0; i < m_PluginPanels.GetCount(); ++i)
        {
            cbConfigurationPanel* panel = m_PluginPanels[i];
            panel->OnApply();
        }
    }
    else
    {
        // finally, cancel settings in all plugins' panels
        for (size_t i = 0; i < m_PluginPanels.GetCount(); ++i)
        {
            cbConfigurationPanel* panel = m_PluginPanels[i];
            panel->OnCancel();
        }
    }

    wxDialog::EndModal(retCode);
}
