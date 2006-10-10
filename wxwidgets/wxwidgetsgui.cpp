#include "wxwidgetsgui.h"
#include "wxwidgetsguiconfigpanel.h"
#include "wxwidgetsguiappadoptingdlg.h"
#include "../wxscoder.h"
#include "../wxsproject.h"

wxWidgetsGUI::wxWidgetsGUI(wxsProject* Project):
    wxsGUI(_T("wxWidgets"),Project),
    m_AppFile(),
    m_LoadedResources(),
    m_MainResource(),
    m_CallInitAll(true),
    m_CallInitAllNecessary(true),
    m_AppLanguage(wxsCPP)
{
}

wxWidgetsGUI::~wxWidgetsGUI()
{
}

cbConfigurationPanel* wxWidgetsGUI::OnBuildConfigurationPanel(wxWindow* Parent)
{
    return new wxWidgetsGUIConfigPanel(Parent,this);
}

void wxWidgetsGUI::OnRebuildApplicationCode()
{
}

bool wxWidgetsGUI::OnCheckIfApplicationManaged()
{
    return IsAppSourceManaged(m_AppFile);
}

bool wxWidgetsGUI::OnCreateApplicationBinding()
{
    wxWidgetsGUIAppAdoptingDlg Dlg(NULL,this);
    Dlg.ShowModal();
    return OnCheckIfApplicationManaged();
}

void wxWidgetsGUI::OnReadConfig(TiXmlElement* element)
{
    m_AppFile      = cbC2U(element->Attribute("app_src_file"));
    m_MainResource = cbC2U(element->Attribute("main_resource"));
    wxString InAll = cbC2U(element->Attribute("init_all_handlers"));
    if ( InAll == _T("never") )
    {
        m_CallInitAll = false;
        m_CallInitAllNecessary = false;
    }
    else if ( InAll == _T("always") )
    {
        m_CallInitAll = true;
        m_CallInitAllNecessary = false;
    }
    else
    {
        m_CallInitAll = true;
        m_CallInitAllNecessary = true;
    }

    TiXmlElement* LoadRes = element->FirstChildElement("load_resource");
    while ( LoadRes )
    {
        wxString FileName = cbC2U(LoadRes->Attribute("file_name"));
        if ( !FileName.empty() )
        {
            m_LoadedResources.Add(FileName);
        }
        LoadRes = LoadRes->NextSiblingElement("load_resource");
    }

}

void wxWidgetsGUI::OnWriteConfig(TiXmlElement* element)
{
    element->SetAttribute("app_src_file",cbU2C(m_AppFile));
    element->SetAttribute("main_resource",cbU2C(m_MainResource));
    if ( m_CallInitAll && m_CallInitAllNecessary )
    {
        element->SetAttribute("init_all_handlers","necessary");
    }
    else if ( m_CallInitAll )
    {
        element->SetAttribute("init_all_handlers","always");
    }
    else
    {
        element->SetAttribute("init_all_handlers","never");
    }

    for ( size_t i=0; i<m_LoadedResources.GetCount(); ++i )
    {
        TiXmlElement* LoadRes = element->InsertEndChild(TiXmlElement("load_resource"))->ToElement();
        LoadRes->SetAttribute("load_resource",cbU2C(m_LoadedResources[i]));
    }
}

bool wxWidgetsGUI::IsAppSourceManaged(const wxString& FileName)
{
    if ( FileName.empty() ) return false;

    if ( wxsCoder::Get()->GetCode(
            GetProjectPath()+FileName,
            wxsCodeMarks::Beg(m_AppLanguage,_T("AppInitialize")),
            wxsCodeMarks::End(m_AppLanguage)
            ).empty() )
    {
        return false;
    }

    if ( wxsCoder::Get()->GetCode(
            GetProjectPath()+FileName,
            wxsCodeMarks::Beg(m_AppLanguage,_T("AppHeaders")),
            wxsCodeMarks::End(m_AppLanguage)
            ).empty() )
    {
        return false;
    }
    return true;
}

bool wxWidgetsGUI::ScanForApp(ProjectFile* File)
{
    // If this file is already managed, returning true
    if ( IsAppSourceManaged(File->relativeFilename) ) return true;

    // Fetching source code
    wxString Source = wxsCoder::Get()->GetFullCode(File->file.GetFullPath());

    // Searching for app class
    if ( GetAppClassName(Source).empty() ) return false;

    // Searching for OnInit() function
    int Pos = Source.Find(_T("OnInit"));
    if ( Pos < 0 ) return false;

    // TODO: Do extra checks of this OnInit
    return true;
}

bool wxWidgetsGUI::AddSmithToApp(const wxString& RelativeFileName)
{
    if ( !IsAppSourceManaged(RelativeFileName) )
    {
        wxString FullPath = GetProjectPath()+RelativeFileName;
        // Need to create some extra blocks of code.
        wxString Source = wxsCoder::Get()->GetFullCode(FullPath);
        if ( Source.empty() ) return false;

        // First thing we need is to add new includes section
        // It is added right before IMPLEMENT_APP() macro
        int Pos = Source.Find(_T("IMPLEMENT_APP"));
        while ( Pos>0 && Source[Pos]!=_T('\n') ) Pos--;
        if ( Pos>0 ) Pos++;

        // TODO: Get valid EOL mode
        Source.insert(Pos,
            wxsCodeMarks::Beg(wxsCPP,_T("AppHeaders")) + _T("\n") +
            wxsCodeMarks::End(wxsCPP) + _T("\n\n"));

        wxString ClassName = GetAppClassName(Source);
        if ( ClassName.empty() ) return false;

        // Searching for ::OnInit member
        wxString SourceCpy = Source;
        Pos = 0;
        while ( !SourceCpy.empty() )
        {
            int ClassPos = SourceCpy.Find(ClassName);
            if ( ClassPos<0 ) return false;
            ClassPos += ClassName.Length();
            Pos += ClassPos;
            SourceCpy.Remove(0,ClassPos);
            int MemberPos = 0;
            while ( IsWhite(SourceCpy,MemberPos) ) MemberPos++;
            if ( MemberPos+1>=(int)(SourceCpy.Length()) ) continue;
            if ( SourceCpy[MemberPos]!=_T(':') && SourceCpy[MemberPos+1]!=_T(':') ) continue;
            MemberPos+=2;
            while ( IsWhite(SourceCpy,MemberPos) ) MemberPos++;
            if ( SourceCpy.Mid(MemberPos,6) != _T("OnInit") ) continue;
            MemberPos += 6;
            while ( IsWhite(SourceCpy,MemberPos) ) MemberPos++;
            if ( !Match(SourceCpy,MemberPos,_T('(')) ) continue;
            MemberPos++;
            while ( IsWhite(SourceCpy,MemberPos) ) MemberPos++;
            if ( !Match(SourceCpy,MemberPos,_T(')')) ) continue;
            MemberPos++;
            while ( IsWhite(SourceCpy,MemberPos) ) MemberPos++;
            if ( !Match(SourceCpy,MemberPos,_T('{')) ) continue;
            MemberPos++;
            // Ok, we're at function beginning, calculating indentation of {
            Pos += MemberPos;
            break;
        }

        if ( SourceCpy.empty() ) return false;

        // Calculating indentation of source
        int IndentPos = Pos;
        while ( IndentPos>0 && Source[IndentPos-1]!=_T('\n') && Source[IndentPos-1]!='\r' ) IndentPos--;
        wxString Indent;
        while ( IndentPos<Pos && (Source[IndentPos]==_T(' ') || Source[IndentPos]==_T('\t')) ) Indent += Source[IndentPos++];
        Indent.Append(_T("\t"));

        // Inserting AppInitializeBlock
        Source = Source(0,Pos) +
            _T("\n") +
            Indent + wxsCodeMarks::Beg(wxsCPP,_T("AppInitialize")) + _T("\n") +
            Indent + wxsCodeMarks::End(wxsCPP) + _T("\n") +
            Indent + _T("return wxsOK;\n") +
            Indent
            + Source.Mid(Pos);

        // Writing new source back to files / editor
        wxsCoder::Get()->PutFullCode(FullPath,Source);
    }

    // Applying default configuration
    m_AppFile = RelativeFileName;
    m_LoadedResources.Clear();
    m_MainResource.Clear();
    m_CallInitAll = true;
    m_CallInitAllNecessary = true;
    m_AppLanguage = wxsCPP;
    OnRebuildApplicationCode();
    return true;
}

wxString wxWidgetsGUI::GetAppClassName(const wxString& Source)
{
    // Doing some trick - searching for IMPLEMENT_APP macro followed
    // by '(' and class name - here we can fetch name of application class
    int Pos = Source.Find(_T("IMPLEMENT_APP"));
    if ( Pos<0 ) return wxEmptyString;
    Pos += 13;// strlen("IMPLEMENT_APP")
    while ( IsWhite(Source,Pos) ) Pos++;
    if ( Pos >= (int)Source.Length() ) return wxEmptyString;
    if ( Source[Pos++] != _T('(') ) return wxEmptyString;
    while ( IsWhite(Source,Pos) ) Pos++;
    wxString ClassName;
    static const wxString AllowedChars(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_"));
    while ( (Pos < (int)Source.Length()) && (AllowedChars.Find(Source[Pos])>=0) )
    {
        ClassName += Source[Pos];
        Pos++;
    }
    while ( IsWhite(Source,Pos) ) Pos++;
    if ( Pos >= (int)Source.Length() ) return wxEmptyString;
    if ( Source[Pos] != _T(')') ) return wxEmptyString;
    return ClassName;
}

bool wxWidgetsGUI::CreateNewApp(const wxString& FileName)
{
    wxFile Fl(FileName,wxFile::write);
    if ( !Fl.IsOpened() )
    {
        wxMessageBox(_("Couldn't overwrite file"));
        return false;
    }

    Fl.Write(
        _T("#include <wx/wxprec.h>\n")
        _T("\n")
        _T("#ifdef __BORLANDC__\n")
        _T("    #pragma hdrstop\n")
        _T("#endif\n")
        _T("\n")
        _T("#ifndef WX_PRECOMP\n")
        _T("    #include <wx/app.h>\n")
        _T("#endif\n")
        _T("\n")
        _T("//(*AppHeaders\n")
        _T("//*)\n")
        _T("\n")
        _T("\n")
        _T("class MyApp : public wxApp\n")
        _T("{\n")
        _T("	public:\n")
        _T("		virtual bool OnInit();\n")
        _T("};\n")
        _T("\n")
        _T("IMPLEMENT_APP(MyApp);\n")
        _T("\n")
        _T("bool MyApp::OnInit()\n")
        _T("{\n")
        _T("    //(*AppInitialize\n")
        _T("    //*)\n")
        _T("    return wxsOK;\n")
        _T("}\n")
        _T("\n"));

    // Applying default configuration
    wxFileName FN(FileName);
    FN.MakeRelativeTo(GetProjectPath());
    m_AppFile = FN.GetFullPath();
    m_LoadedResources.Clear();
    m_MainResource.Clear();
    m_CallInitAll = true;
    m_CallInitAllNecessary = true;
    m_AppLanguage = wxsCPP;
    OnRebuildApplicationCode();
    return true;

}

inline bool wxWidgetsGUI::IsWhite(wxChar Ch)
{
    return (Ch==_T(' ')) || (Ch==_T('\n')) || (Ch==_T('\r')) || (Ch==_T('\t'));
}

inline bool wxWidgetsGUI::IsWhite(const wxString& Str,int Pos)
{
    if ( Pos>=(int)Str.Length() ) return false;
    return IsWhite(Str[Pos]);
}

inline bool wxWidgetsGUI::Match(const wxString& Str,int Pos,wxChar Ch)
{
    if ( Pos>=(int)Str.Length() ) return false;
    return Str[Pos]==Ch;
}

static wxsRegisterGUI<wxWidgetsGUI> Reg(_T("wxWidgets"));
