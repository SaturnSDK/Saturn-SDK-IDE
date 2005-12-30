#include "wxsheaders.h"
#include "wxsnewwindowdlg.h"
#include "resources/wxswindowres.h"
#include "wxsmith.h"

#include <wx/xrc/xmlres.h>
#include <projectmanager.h>
#include "globals.h"

BEGIN_EVENT_TABLE(wxsNewWindowDlg,wxDialog)
    //(*EventTable(wxsNewWindowDlg)
    EVT_TEXT(XRCID("ID_TEXTCTRL1"),wxsNewWindowDlg::OnClassChanged)
    EVT_TEXT(XRCID("ID_TEXTCTRL2"),wxsNewWindowDlg::OnHeaderChanged)
    EVT_TEXT(XRCID("ID_TEXTCTRL3"),wxsNewWindowDlg::OnSourceChanged)
    EVT_CHECKBOX(XRCID("ID_CHECKBOX1"),wxsNewWindowDlg::OnUseXrcChange)
    EVT_TEXT(XRCID("ID_TEXTCTRL4"),wxsNewWindowDlg::OnXrcChanged)
    EVT_BUTTON(XRCID("ID_BUTTON1"),wxsNewWindowDlg::OnCancel)
    EVT_BUTTON(XRCID("ID_BUTTON2"),wxsNewWindowDlg::OnCreate)
    //*)
END_EVENT_TABLE()

wxsNewWindowDlg::wxsNewWindowDlg(wxWindow* parent,const wxString& ResType):
    SourceNotTouched(true),
    HeaderNotTouched(true),
    XrcNotTouched(true),
    BlockText(false),
    Type(ResType)
{
    //(*Initialize(wxsNewWindowDlg)
    wxXmlResource::Get()->LoadDialog(this,parent,_T("wxsNewWindowDlg"));
    Class = XRCCTRL(*this,"ID_TEXTCTRL1",wxTextCtrl);
    Header = XRCCTRL(*this,"ID_TEXTCTRL2",wxTextCtrl);
    Source = XRCCTRL(*this,"ID_TEXTCTRL3",wxTextCtrl);
    UseXrc = XRCCTRL(*this,"ID_CHECKBOX1",wxCheckBox);
    Xrc = XRCCTRL(*this,"ID_TEXTCTRL4",wxTextCtrl);
    //*)

    if ( !Class || !Source || !Header )
    {
        Close();
    }
    else
    {
    	BlockText = true;
    	wxString ResName = wxString::Format(_("New%s"),ResType.c_str());
        Class->SetValue(ResName);
        Source->SetValue(ResName.Lower()+_T(".cpp"));
        Header->SetValue(ResName.Lower()+_T(".h"));
        Xrc->SetValue(ResName.Lower()+_T(".xrc"));
        Xrc->Disable();
        SetTitle(wxString::Format(_("New %s resource"),ResType.c_str()));
        BlockText = false;
    }
}

wxsNewWindowDlg::~wxsNewWindowDlg()
{
}

void wxsNewWindowDlg::OnCancel(wxCommandEvent& event)
{
    Close();
}

void wxsNewWindowDlg::OnCreate(wxCommandEvent& event)
{
	bool CreateXrc = UseXrc->GetValue();

    // Need to do some checks

    // First - validating name
    const wxChar* ClassName = Class->GetValue().c_str();


    if ( !wxsValidateIdentifier(ClassName) )
    {
        wxMessageBox(_("Invalid class name"));
        return;
    }

    // Second - checking if there's given resoure in current project

    wxsProject* Proj = wxsPLUGIN()->GetSmithProject(Manager::Get()->GetProjectManager()->GetActiveProject());
    if ( !Proj ) { return; }

    if ( Proj->FindResource(Class->GetValue()) )
    {
        wxMessageBox(wxString::Format(_("Resource '%s' already exists"),Class->GetValue().c_str()));
        return;
    }

    // Third - checking if files already exist

    if ( wxFileName::FileExists(Proj->GetProjectFileName(Header->GetValue())) )
    {
        if ( wxMessageBox(
              wxString::Format(_("File '%s' already exists. It will be overwritten.\nContinue ?"),Header->GetValue().c_str()),
              _("File exists"),wxYES_NO|wxICON_ERROR) != wxYES )
        {
            return;
        }
    }

    if ( wxFileName::FileExists(Proj->GetProjectFileName(Source->GetValue())) )
    {
        if ( wxMessageBox(
              wxString::Format(_("File '%s' already exists. It will be overwritten.\nContinue ?"),Source->GetValue().c_str()),
              _("File exists"),wxYES_NO|wxICON_ERROR) != wxYES )
        {
            return;
        }
    }

    if ( CreateXrc && wxFileName::FileExists(Proj->GetProjectFileName(Xrc->GetValue())) )
    {
        if ( wxMessageBox(
              wxString::Format(_("File '%s' already exists. It will be overwritten.\nContinue ?"),Source->GetValue().c_str()),
              _("File exists"),wxYES_NO|wxICON_ERROR) != wxYES )
        {
            return;
        }
    }

    // Creating dialog

    wxString WxsFile = Class->GetValue() + _T(".wxs");
    wxsWindowRes* NewWindow = NULL;

    int ResType = CreateXrc ? wxsREMMixed : wxsREMSource;
    wxString XrcFile = CreateXrc ? Xrc->GetValue() : _T("");

    if ( Type == _T("Dialog") )
    {
        NewWindow = new wxsDialogRes(
            Proj,
            ResType,
            Class->GetValue(),
            Proj->GetInternalFileName(WxsFile),
            Source->GetValue(),
            Header->GetValue(),
            XrcFile);
    }
    else if ( Type == _T("Frame") )
    {
        NewWindow = new wxsFrameRes(
            Proj,
            ResType,
            Class->GetValue(),
            Proj->GetInternalFileName(WxsFile),
            Source->GetValue(),
            Header->GetValue(),
            XrcFile);
    }
    else if ( Type == _T("Panel") )
    {
        NewWindow = new wxsPanelRes(
            Proj,
            ResType,
            Class->GetValue(),
            Proj->GetInternalFileName(WxsFile),
            Source->GetValue(),
            Header->GetValue(),
            XrcFile);
    }

    if ( !NewWindow->GenerateEmptySources() )
    {
        wxMessageBox(_("Couldn't generate sources"),_("Error"),wxOK|wxICON_ERROR);
        delete NewWindow;
        return;
    }

    // Adding new files

    wxArrayInt targets;
    Manager::Get()->GetProjectManager()->AddFileToProject(Header->GetValue(), Proj->GetCBProject(), targets);
    if (targets.GetCount() != 0)
    {
        Manager::Get()->GetProjectManager()->AddFileToProject(Source->GetValue(), Proj->GetCBProject(), targets);
    }
    Manager::Get()->GetProjectManager()->RebuildTree();

    if ( !PrepareResource(NewWindow) )
    {
        delete NewWindow;
        Close();
    }

    // Adding dialog to project and opening editor for it

    if ( Type == _T("Dialog") )
    {
        Proj->AddDialog((wxsDialogRes*)NewWindow);
    }
    else if ( Type == _T("Frame") )
    {
        Proj->AddFrame((wxsFrameRes*)NewWindow);
    }
    else if ( Type == _T("Panel") )
    {
        Proj->AddPanel((wxsPanelRes*)NewWindow);
    }

    wxsSelectRes(NewWindow);
    NewWindow->NotifyChange();
    Close();
}

void wxsNewWindowDlg::OnClassChanged(wxCommandEvent& event)
{
    if ( BlockText ) return;
    BlockText = true;
    if ( HeaderNotTouched ) Header->SetValue((Class->GetValue() + _T(".h")).MakeLower());
    if ( SourceNotTouched ) Source->SetValue((Class->GetValue() + _T(".cpp")).MakeLower());
    if ( XrcNotTouched ) Xrc->SetValue((Class->GetValue() + _T(".xrc")).MakeLower());
    BlockText = false;
}

void wxsNewWindowDlg::OnSourceChanged(wxCommandEvent& event)
{
    if ( BlockText ) return;
    BlockText = true;
    SourceNotTouched = false;
    BlockText = false;
}

void wxsNewWindowDlg::OnHeaderChanged(wxCommandEvent& event)
{
    if ( BlockText ) return;
    BlockText = true;
    wxFileName FN(Header->GetValue());
    FN.SetExt(_T("cpp"));
    if ( SourceNotTouched )
    {
        Source->SetValue(FN.GetFullPath());
    }
    FN.SetExt(_T("xrc"));
    if ( XrcNotTouched )
    {
        Xrc->SetValue(FN.GetFullPath());
    }
    HeaderNotTouched = false;
    BlockText = false;
}

void wxsNewWindowDlg::OnUseXrcChange(wxCommandEvent& event)
{
    Xrc->Enable(UseXrc->GetValue());
}

void wxsNewWindowDlg::OnXrcChanged(wxCommandEvent& event)
{
    if ( BlockText ) return;
    BlockText = true;
    XrcNotTouched = false;
    BlockText = false;
}
