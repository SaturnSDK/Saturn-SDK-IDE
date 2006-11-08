#include "wxsitemres.h"
#include "wxsitem.h"
#include "../wxscoder.h"

IMPLEMENT_CLASS(wxsItemRes,wxWidgetsRes)

namespace
{
    const wxString CppEmptySource =
        _T("#include \"$(Include)\"\n")
        _T("\n")
        + wxsCodeMarks::Beg(wxsCPP,_T("InternalHeaders"),_T("$(ClassName)"))
        + wxsCodeMarks::End(wxsCPP) +
        _T("\n")
        + wxsCodeMarks::Beg(wxsCPP,_T("IdInit"),_T("$(ClassName)"))
        + wxsCodeMarks::End(wxsCPP) +
        _T("\n")
        _T("BEGIN_EVENT_TABLE($(ClassName),$(BaseClassName))\n")
        _T("\t") + wxsCodeMarks::Beg(wxsCPP,_T("EventTable"),_T("$(ClassName)")) +
        _T("\t") + wxsCodeMarks::End(wxsCPP) +
        _T("END_EVENT_TABLE()\n")
        _T("\n")
        _T("$(ClassName)::$(ClassName)(wxWindow* parent,wxWindowID id)\n")
        _T("{\n")
        _T("\t") + wxsCodeMarks::Beg(wxsCPP,_T("Initialize"),_T("$(ClassName)")) +
        _T("\t") + wxsCodeMarks::End(wxsCPP) +
        _T("}\n")
        _T("\n")
        _T("$(ClassName)::~$(ClassName)()\n")
        _T("{\n")
        _T("}\n")
        _T("\n");

    const wxString CppEmptyHeader =
        _T("#ifndef $(Guard)\n")
        _T("#define $(Guard)\n")
        _T("\n")
        _T("#include <wx/wxprec.h>\n")
        _T("\n")
        _T("#ifdef __BORLANDC__\n")
        _T("    #pragma hdrstop\n")
        _T("#endif\n")
        _T("\n")
        + wxsCodeMarks::Beg(wxsCPP,_T("Headers"),_T("$(ClassName)")) + _T("\n")
        + wxsCodeMarks::End(wxsCPP) + _T("\n")
        _T("\n")
        _T("class $(ClassName): public $(BaseClassName)\n")
        _T("{\n")
        _T("\tpublic:\n")
        _T("\n")
        _T("\t\t$(ClassName)(wxWindow* parent,wxWindowID id = -1);\n")
        _T("\t\tvirtual ~$(ClassName)();\n")
        _T("\n")
        _T("\t\t") + wxsCodeMarks::Beg(wxsCPP,_T("Identifiers"),_T("$(ClassName)")) + _T("\n")
        _T("\t\t") + wxsCodeMarks::End(wxsCPP) + _T("\n")
        _T("\n")
        _T("\tprotected:\n")
        _T("\n")
        _T("\t\t") + wxsCodeMarks::Beg(wxsCPP,_T("Handlers"),_T("$(ClassName)")) + _T("\n")
        _T("\t\t") + wxsCodeMarks::End(wxsCPP) + _T("\n")
        _T("\n")
        _T("\t\t") + wxsCodeMarks::Beg(wxsCPP,_T("Declarations"),_T("$(ClassName)")) + _T("\n")
        _T("\t\t") + wxsCodeMarks::End(wxsCPP) + _T("\n")
        _T("\n")
        _T("\tprivate:\n")
        _T("\n")
        _T("\t\tDECLARE_EVENT_TABLE()\n")
        _T("};\n")
        _T("\n")
        _T("#endif\n");

    const wxString EmptyXrc =
        _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
        _T("<resource xmlns=\"http://www.wxwidgets.org/wxxrc\">\n")
        _T("</resource>\n");
}

wxsItemRes::wxsItemRes(wxsProject* Owner,const wxString& Type,bool CanBeMain):
    wxWidgetsRes(Owner,Type),
    m_WxsFileName(wxEmptyString),
    m_SrcFileName(wxEmptyString),
    m_HdrFileName(wxEmptyString),
    m_XrcFileName(wxEmptyString),
    m_CanBeMain(CanBeMain)
{
}

wxsItemRes::wxsItemRes(const wxString& FileName,const TiXmlElement* XrcElem,const wxString& Type):
    wxWidgetsRes(NULL,Type),
    m_WxsFileName(wxEmptyString),
    m_SrcFileName(wxEmptyString),
    m_HdrFileName(wxEmptyString),
    m_XrcFileName(FileName)
{
    // TODO: Load resource
}

wxsItemRes::~wxsItemRes()
{
}

wxsEditor* wxsItemRes::OnCreateEditor()
{
    // TODO: Code it
    return NULL;
}

bool wxsItemRes::OnReadConfig(const TiXmlElement* Node)
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

bool wxsItemRes::OnWriteConfig(TiXmlElement* Node)
{
    Node->SetAttribute("wxs",cbU2C(m_WxsFileName));
    Node->SetAttribute("src",cbU2C(m_SrcFileName));
    Node->SetAttribute("hdr",cbU2C(m_HdrFileName));
    if ( !m_XrcFileName.empty() )
    {
        Node->SetAttribute("xrc",cbU2C(m_XrcFileName));
    }
    return true;
}

bool wxsItemRes::OnCanHandleFile(const wxString& FileName)
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

wxString wxsItemRes::OnGetDeclarationFile()
{
    return m_HdrFileName;
}

bool wxsItemRes::OnGetUsingXRC()
{
    return !m_XrcFileName.empty();
}

bool wxsItemRes::OnGetCanBeMain()
{
    return m_CanBeMain;
}

bool wxsItemRes::CreateNewResource(const wxString& Class,const wxString& Src, bool GenSrc,const wxString& Hdr, bool GenHdr,const wxString& Xrc, bool GenXrc)
{
    wxFileName HFN(GetProjectPath()+Hdr);
    SetLanguage(wxsCodeMarks::IdFromExt(HFN.GetExt()));

    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            SetResourceName(Class);

            m_HdrFileName = Hdr;
            if ( GenHdr )
            {
                wxString Name = GetProjectPath()+Hdr;
                wxFileName::Mkdir(wxFileName(Name).GetPath(),0777,wxPATH_MKDIR_FULL);
                wxFile File(Name,wxFile::write);
                wxString Guard = HFN.GetName().Upper() + _T("_H");
                wxString Header = CppEmptyHeader;
                Header.Replace(_T("$(Guard)"),Guard);
                Header.Replace(_T("$(ClassName)"),Class);
                Header.Replace(_T("$(BaseClassName)"),GetResourceType());
                if ( !File.Write(Header) ) return false;
            }

            m_SrcFileName = Src;
            if ( GenSrc )
            {
                wxString Name = GetProjectPath()+Src;
                wxFileName::Mkdir(wxFileName(Name).GetPath(),0777,wxPATH_MKDIR_FULL);
                wxFile File(Name,wxFile::write);
                HFN.MakeRelativeTo(Name);
                wxString Include = HFN.GetFullPath(wxPATH_UNIX);
                wxString Source = CppEmptySource;
                Source.Replace(_T("$(Include)"),Include);
                Source.Replace(_T("$(ClassName)"),Class);
                Source.Replace(_T("$(BaseClassName)"),GetResourceType());
                if ( !File.Write(Source) ) return false;
            }

            m_XrcFileName = Xrc;
            if ( !Xrc.empty() && GenXrc )
            {
                wxString Name = GetProjectPath()+Xrc;
                wxFileName::Mkdir(wxFileName(Name).GetPath(),0777,wxPATH_MKDIR_FULL);
                wxFile File(Name,wxFile::write);
                if ( !File.Write(EmptyXrc) ) return false;
            }

            // Searching for new wxs file name
            // TODO: Do not use constant folder name
            wxString WxsNameBase = _T("wxsmith");
            wxString WxsName;
            if ( !wxFileName::DirExists(GetProjectPath()+WxsNameBase) )
            {
                if ( !wxFileName::Mkdir(GetProjectPath()+WxsNameBase,0777,wxPATH_MKDIR_FULL) )
                {
                    return false;
                }
            }
            WxsNameBase += _T("/") + Class.Lower();
            WxsName = WxsNameBase + _T(".wxs");
            int Cnt = 0;
            for(;;)
            {
                if ( !wxFileName::FileExists(GetProjectPath()+WxsName) &&
                     !wxFileName::DirExists(GetProjectPath()+WxsName) )
                {
                    break;
                }
                Cnt++;
                WxsName = wxString::Format(_T("%s%d.wxs"),WxsNameBase.c_str(),Cnt);
            }

            m_WxsFileName = WxsName;
            return true;
        }

        default:;
    }

    SetLanguage(wxsUnknownLanguage);
    return false;
}

unsigned long wxsItemRes::GetPropertiesFilter()
{
    switch ( GetEditMode() )
    {
        case File:   return wxsItem::flFile;
        case Source: return wxsItem::flSource;
        case Mixed:  return wxsItem::flMixed;
    }
    return 0;
}

wxsItemRes::EditMode wxsItemRes::GetEditMode()
{
    if ( m_WxsFileName.empty() ) return File;
    if ( m_XrcFileName.empty() ) return Source;
    return Mixed;
}

void wxsItemRes::NotifyChange(wxsItem*)
{
    switch ( EditMode() )
    {
        case File:
            // If editing file (XRC not binded to application) we do nothing when resource changes
            break;

        case Source:
            RebuildSourceCode();
            break;

        case Mixed:
            RebuildSourceCode();
            RebuildXrcFile();
    }
}

void wxsItemRes::RebuildSourceCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            wxString InitializingCode;
            wxString GlobalVarsCode;
            wxString IdentifiersCode;
            wxString IdInitCode;
            wxString GlobalHeaders;
            wxString LocalHeaders;

            BuildVariablesCode(wxsCPP,InitializingCode,GlobalVarsCode);
            BuildCreatingCode(wxsCPP,InitializingCode);
            BuildEventHandlersCode(wxsCPP,InitializingCode);
            BuildIdentifiersCode(wxsCPP,IdentifiersCode,IdInitCode);
            BuildIncludesCode(wxsCPP,LocalHeaders,GlobalHeaders);

            // TODO: Maybe some group update ??
            wxsCoder::Get()->AddCode(
                GetProjectPath() + m_HdrFileName,
                wxsCodeMarks::Beg(wxsCPP,_T("Declarations"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                GlobalVarsCode);

            wxsCoder::Get()->AddCode(
                GetProjectPath() + m_HdrFileName,
                wxsCodeMarks::Beg(wxsCPP,_T("Identifiers"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                IdentifiersCode);

            wxsCoder::Get()->AddCode(
                GetProjectPath() + m_HdrFileName,
                wxsCodeMarks::Beg(wxsCPP,_T("Headers"),GetClassName()),
                wxsCodeMarks::End(),
                GlobalHeaders);

            wxsCoder::Get()->AddCode(
                GetProjectPath() + m_SrcFileName,
                wxsCodeMarks::Beg(wxsCPP,_T("Initialize"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                InitializingCode);

            wxsCoder::Get()->AddCode(
                GetProjectPath() + m_SrcFileName,
                wxsCodeMarks::Beg(wxsCPP,_T("IdInit"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                IdInitCode);

            wxsCoder::Get()->AddCode(
                GetProjectPath() + m_SrcFileName,
                wxsCodeMarks::Beg(wxsCPP,_T("InternalHeaders"),GetClassName()),
                wxsCodeMarks::End(),
                LocalHeaders);

            wxsCoder::Get()->AddCode(
                GetProjectPath() + m_SrcFileName,
                wxsCodeMarks::Beg(wxsCPP,_T("EventTable"),GetClassName()),
                wxsCodeMarks::End(),
                _T(""));    // This clears previously used event table for event binding

            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsItemRes::RebuildSourceCode"),GetLanguage());
        }
    }

}

void wxsItemRes::BuildVariablesCode(wxsCodingLang Lang,wxString& LocalCode, wxString& GlobalCode)
{
    switch ( Lang )
    {
        case wxsCPP:
        {
            // Creating local and global declarations
            BuildVariablesCodeReq(wxsCPP,m_RootItem,InitializingCode,GlobalVarsCode);
            if ( InitializingCode.Length()>1 )
            {
                // Adding one empty line between local declarations and
                // the rest of initializing code
                InitializingCode << _T("\n");
            }
            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsItemRes::BuildVariablesCode"),Lang);
        }
    }
}

void wxsItemRes::BuildVariablesCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& LocalCode, wxString& GlobalCode)
{
    wxsParent* Parent = Item->ToParent();
    if ( !Parent )
    {
        return false;
    }

    int ChildCnt = Parent->GetChildCount();
    for ( int i=0; i<ChildCnt; i++ )
    {
        wxsItem* Child = Parent->GetChild(i);

        if ( Child->GetPropertiesFlags() & wxsFLVariable )
        {
            if ( Child->GetIsMember() )
            {
                Child->BuildDeclarationCode(GlobalCode,Lang);
            }
            else if ( GetEditMode() == Source )
            {
                Child->BuildDeclarationCode(LocalCode,Lang);
            }
        }

        BuildVariablesCodeReq(Lang,Child,LocalCode,GlobalCode);
    }
}

void wxsItemRes::BuildCreatingCode(wxsCodingLang Lang,wxString& Code)
{
    switch ( GetEditMode() )
    {
        case Source:
            m_RootItem->BuildCreatingCode(Code,_T("parent"),Lang);
            break;

        case Mixed:
            OnBuildXrcLoadingCode(Code,Lang);
            BuildXrcItemsFetchingCode(Code,Lang);
            break;

        default:;
    }
}

void wxsItemRes::OnBuildXrcLoadingCode(wxsCodingLang Language,wxString& Code)
{
    switch ( Lang )
    {
        case wxsCPP:
        {
            Code << _T("wxXmlResource::Get()->LoadObject(this,parent,")
                 << wxsCodeMarks::WxString(wxsCPP,GetClassName()) << _T(",")
                 << wxsCodeMarks::WxString(wxsCPP,GetResourceType()) << _T(");\n");
            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsItemRes::OnBuildXrcLoadingCode"),Lang);
        }
    }
}

void wxsItemRes::BuildXrcItemsFetchingCode(wxsCodingLang Lang,wxString& Code)
{
    BuildXrcItemsFetchingCodeReq(Lang,m_RootItem,Code);
}

void wxsItemRes::BuildXrcItemsFetchingCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& Code)
{
    switch ( Lang )
    {
        case wxsCPP:
        {
            wxsParent* Parent = Item->ConvertToParent();
            if ( !Parent )
            {
                return;
            }

            int Cnt = Parent->GetChildCount();
            for ( int i=0; i<Cnt; i++ )
            {
                wxsItem* Child = Parent->GetChild(i);
                unsigned long Flags = Child->GetPropertiesFlags();
                if ( (Flags & (wxsFLVariable|wxsFLId)) == (wxsFLVariable|wxsFLId) )
                {
                    if ( Child->GetIsMember() )
                    {
                        Code << Child->GetVarName()
                             << _T(" = (") << Child->GetType() << _T(")")
                             << _T("FindWindow(XRCID(") + Child->GetIdName() + _T("));\n");
                    }
                }
                BuildXrcItemsFetchingCodeReq(Lang,Child,Code);
            }
            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsItemRes::BuildXrcItemsFetchingCodeReq"),Lang);
        }
    }
}

void wxsItemRes::BuildEventHandlersCode(wxsCodingLang Language,wxString& Code)
{
    BuildEventHandlersCodeReq(Language,m_RootItem,Code);
}

void wxsItemRes::BuildEventHandlersCodeReq(wxsCodingLang Language,wxsItem* Item,wxString& Code)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            wxsEvents& Events = Item->GetEvents();
            wxString IdString;
            wxString VarNameString;
            if ( GetEditMode() == Source )
            {
                IdString = Item->GetIdName();
                VarNameString = Item->GetVarName();
            }
            else
            {
                IdString = _T("XRCID(") + Item->GetIdName() + _T(")");
                if ( Item->m_IsMember )
                {
                    VarNameString = Item->GetVarName();
                }
                else
                {
                    VarNameString = _T("FindWindow(XRCID(") + Item->GetIdName() + _T("))");
                }
            }
            Events.GenerateBindingCode(Code,IdString,VarNameString,Language);

            wxsParent* Parent = Item->ConvertToParent();
            if ( Parent )
            {
                int Cnt = Parent->GetChildCount();
                for ( int i=0; i<Cnt; i++ )
                {
                    BuildEventHandlersCodeReq(Language,Parent->GetChild(i),Code);
                }
            }
            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsItemRes::BuildEventHandlersCodeReq"),Language);
        }
    }
}

void wxsItemRes::BuildIdentifiersCode(wxsCodingLang Lang,wxString& IdCode,wxString& IdInitCode)
{
    if ( GetEditMode == Source )
    {
        wxArrayString IdsArray;
        BuildIdsArrayReq(m_RootItem,IdsArray);
        switch ( Lang )
        {
            case wxsCPP:
            {
                if ( IdsArray.Count() )
                {
                    for ( size_t i = 0; i<IdsArray.Count(); ++i )
                    {
                        const wxString Id = IdsArray[i];
                        if ( IdsArray[i].find(Id) == i )
                        {
                            IdCode << _T("static const long ") + IdsArray[i] + _T(";\n");
                            IdInitCode << _T("const long ") + GetClassName() + _T("::") + IdsArray[i] + _T(" = wxNewId();\n");
                        }
                    }
                }
                break;
            }

            default:
            {
                wxsCodeMarks::Unknown(_T("wxsItemRes::BuildIdentifiersCode"),Lang);
            }
        }
    }
}

void wxsItemRes::BuildIncludesCode(wxsCodingLang Language,wxString& LocalIncludes,wxString& GlobalIncludes)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            wxArrayString GlobalHeaders;
            wxArrayString LocalHeaders;
            BuildHeadersReq(wxsCPP,m_RootItem,LocalHeaders,GlobalHeaders);

            LocalHeaders.Add(_T("<wx/intl.h>"));
            // TODO: Use these headers dynamically, not always
            LocalHeaders.Add(_T("<wx/bitmap.h>"));
            LocalHeaders.Add(_T("<wx/image.h>"));
            LocalHeaders.Add(_T("<wx/font.h>"));

            if ( GetEditMode() == Mixed )
            {
                LocalHeaders.Add(_T("<wx/xrc/xmlres.h>"));
            }

            GlobalHeaders.Sort();
            LocalHeaders.Sort();

            wxString Previous = _T("");
            for ( size_t i=0; i<GlobalHeaders.Size(); i++ )
            {
                if ( GlobalHeaders[i] != Previous )
                {
                    Previous = GlobalHeaders[i];
                    GlobalIncludes << _T("#include ") << Previous << _T("\n");
                }
            }

            Previous = _T("");
            for ( size_t i=0; i<LocalHeaders.Size(); i++ )
            {
                if ( LocalHeaders[i] != Previous )
                {
                    Previous = LocalHeaders[i];
                    LocalIncludes << _T("#include ") << Previous << _T("\n");
                }
            }

            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsItemRes::BuildIncludesCode"),Language);
        }
    }
}

void wxsItemRes::RebuildXrcFile()
{
    // TODO
}

