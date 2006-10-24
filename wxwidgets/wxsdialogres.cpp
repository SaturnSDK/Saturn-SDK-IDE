#include "wxsdialogres.h"


wxsDialogRes::wxsDialogRes(wxsProject* Owner,const wxString& ResourceName,wxsCodingLang Language):
    wxsResource(Owner,ResourceName,_T("wxDialog"),_T("wxWidgets"),Language)
{
}

wxsDialogRes::~wxsDialogRes()
{
}

wxsEditor* wxsDialogRes::OnCreateEditor()
{
    // TODO: Code it
    return NULL;
}

bool wxsDialogRes::OnReadConfig(const TiXmlElement* Node)
{
    m_WxsFileName = cbC2U(Node->Attribute("wxs"));
    m_SrcFileName = cbC2U(Node->Attribute("src"));
    m_HdrFileName = cbC2U(Node->Attribute("hdr"));
    m_XrcFileName = cbC2U(Node->Attribute("xrc"));
    return !m_WxsFileName.empty() &&
           !m_SrcFileName.empty() &&
           !m_HdrFileName.empty();
    // m_XrcFileName may be empty because it's not used when generating full source code
}

bool wxsDialogRes::OnWriteConfig(TiXmlElement* Node)
{
    Node->SetAttribute("wxs",cbU2C(m_WxsFileName));
    Node->SetAttribute("src",cbU2C(m_SrcFileName));
    Node->SetAttribute("hdr",cbU2C(m_HdrFileName));
    if ( !m_XrcFileName.empty() )
    {
        Node->SetAttribute("xrc",cbU2C(m_XrcFileName));
    }
}

bool wxsDialogRes::OnCanHandleFile(const wxString& FileName)
{
    wxFileName Normalized(GetProjectPath()+m_WxsFileName);
    Normalized.Normalize(wxPATH_NORM_DOTS);
    if ( Normalized.GetFullPath() == FileName )
    {
        return true;
    }
    if ( m_XrcFileName.empty() )
    {
        return false;
    }
    Normalized.Assign(GetProjectPath()+m_XrcFileName);
    Normalized.Normalize(wxPATH_NORM_DOTS);
    if ( Normalized.GetFullPath() == FileName )
    {
        return true;
    }
    return false;
}

wxString wxsDialogRes::OnGetDeclarationFile()
{
    return m_HdrFileName;
}

wxString wxsDialogRes::OnGetAppBuildingCode()
{
    return wxString::Format(
        _T("%s Dlg(NULL);\n")
        _T("Dlg.ShowModal();\n")
        _T("wxsOK = false;\n"),
            GetResourceName().c_str());
}

bool wxsDialogRes::OnGetUsingXRC()
{
    return !m_XrcFileName.empty();
}

bool wxsDialogRes::OnGetCanBeMain()
{
    return true;
}
