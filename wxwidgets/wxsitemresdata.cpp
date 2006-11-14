#include "wxsitemresdata.h"
#include "wxsparent.h"
#include "../wxscoder.h"

wxsItemResData::wxsItemResData(wxsItemRes* Resource):
    m_Resource(Resource),
    m_RootItem(NULL)
{
}

wxsItemResData::~wxsItemResData()
{
}

void wxsItemResData::RebuildSourceCode()
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
                GetHdrFileName(),
                wxsCodeMarks::Beg(wxsCPP,_T("Declarations"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                GlobalVarsCode);

            wxsCoder::Get()->AddCode(
                GetHdrFileName(),
                wxsCodeMarks::Beg(wxsCPP,_T("Identifiers"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                IdentifiersCode);

            wxsCoder::Get()->AddCode(
                GetHdrFileName(),
                wxsCodeMarks::Beg(wxsCPP,_T("Headers"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                GlobalHeaders);

            wxsCoder::Get()->AddCode(
                GetSrcFileName(),
                wxsCodeMarks::Beg(wxsCPP,_T("Initialize"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                InitializingCode);

            wxsCoder::Get()->AddCode(
                GetSrcFileName(),
                wxsCodeMarks::Beg(wxsCPP,_T("IdInit"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                IdInitCode);

            wxsCoder::Get()->AddCode(
                GetSrcFileName(),
                wxsCodeMarks::Beg(wxsCPP,_T("InternalHeaders"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                LocalHeaders);

            wxsCoder::Get()->AddCode(
                GetSrcFileName(),
                wxsCodeMarks::Beg(wxsCPP,_T("EventTable"),GetClassName()),
                wxsCodeMarks::End(wxsCPP),
                _T(""));    // This clears previously used event table for event binding

            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsItemResData::RebuildSourceCode"),GetLanguage());
        }
    }

}

void wxsItemResData::BuildVariablesCode(wxsCodingLang Lang,wxString& LocalCode, wxString& GlobalCode)
{
    switch ( Lang )
    {
        case wxsCPP:
        {
            // Creating local and global declarations
            BuildVariablesCodeReq(wxsCPP,m_RootItem,LocalCode,GlobalCode);
            if ( LocalCode.Length()>1 )
            {
                // Adding one empty line between local declarations and
                // the rest of initializing code
                LocalCode << _T("\n");
            }
            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsItemResData::BuildVariablesCode"),Lang);
        }
    }
}

void wxsItemResData::BuildVariablesCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& LocalCode, wxString& GlobalCode)
{
    wxsParent* Parent = Item->ConvertToParent();
    if ( !Parent ) return;

    int ChildCnt = Parent->GetChildCount();
    for ( int i=0; i<ChildCnt; i++ )
    {
        wxsItem* Child = Parent->GetChild(i);

        if ( Child->GetPropertiesFlags() & wxsItem::flVariable )
        {
            if ( Child->GetIsMember() )
            {
                Child->BuildDeclarationCode(GlobalCode,Lang);
            }
            else if ( GetEditMode() == wxsItemRes::Source )
            {
                Child->BuildDeclarationCode(LocalCode,Lang);
            }
        }

        BuildVariablesCodeReq(Lang,Child,LocalCode,GlobalCode);
    }
}

void wxsItemResData::BuildCreatingCode(wxsCodingLang Lang,wxString& Code)
{
    switch ( GetEditMode() )
    {
        case wxsItemRes::Source:
            m_RootItem->BuildCreatingCode(Code,_T("parent"),Lang);
            break;

        case wxsItemRes::Mixed:
            BuildXrcLoadingCode(Lang,Code);
            BuildXrcItemsFetchingCode(Lang,Code);
            break;

        default:;
    }
}

void wxsItemResData::BuildXrcLoadingCode(wxsCodingLang Language,wxString& Code)
{
    switch ( Language )
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
            wxsCodeMarks::Unknown(_T("wxsItemResData::BuildXrcLoadingCode"),Language);
        }
    }
}

void wxsItemResData::BuildXrcItemsFetchingCode(wxsCodingLang Lang,wxString& Code)
{
    BuildXrcItemsFetchingCodeReq(Lang,m_RootItem,Code);
}

void wxsItemResData::BuildXrcItemsFetchingCodeReq(wxsCodingLang Lang,wxsItem* Item,wxString& Code)
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
                if ( (Flags & (wxsItem::flVariable|wxsItem::flId)) == (wxsItem::flVariable|wxsItem::flId) )
                {
                    if ( Child->GetIsMember() )
                    {
                        Code << Child->GetVarName()
                             << _T(" = (") << Child->GetClassName() << _T(")")
                             << _T("FindWindow(XRCID(") + Child->GetIdName() + _T("));\n");
                    }
                }
                BuildXrcItemsFetchingCodeReq(Lang,Child,Code);
            }
            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsItemResData::BuildXrcItemsFetchingCodeReq"),Lang);
        }
    }
}

void wxsItemResData::BuildEventHandlersCode(wxsCodingLang Language,wxString& Code)
{
    BuildEventHandlersCodeReq(Language,m_RootItem,Code);
}

void wxsItemResData::BuildEventHandlersCodeReq(wxsCodingLang Language,wxsItem* Item,wxString& Code)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            wxsEvents& Events = Item->GetEvents();
            wxString IdString;
            wxString VarNameString;
            if ( GetEditMode() == wxsItemRes::Source )
            {
                IdString = Item->GetIdName();
                VarNameString = Item->GetVarName();
            }
            else
            {
                IdString = _T("XRCID(") + Item->GetIdName() + _T(")");
                if ( Item->GetIsMember() )
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
            wxsCodeMarks::Unknown(_T("wxsItemResData::BuildEventHandlersCodeReq"),Language);
        }
    }
}

void wxsItemResData::BuildIdentifiersCode(wxsCodingLang Lang,wxString& IdCode,wxString& IdInitCode)
{
    if ( GetEditMode() == wxsItemRes::Source )
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
                wxsCodeMarks::Unknown(_T("wxsItemResData::BuildIdentifiersCode"),Lang);
            }
        }
    }
}

void wxsItemResData::BuildIdsArrayReq(wxsItem* Item,wxArrayString& Array)
{
    wxsParent* Parent = Item->ConvertToParent();
    if ( !Parent ) return;

	int Cnt = Parent->GetChildCount();
	for ( int i=0; i<Cnt; i++ )
	{
		wxsItem* Child = Parent->GetChild(i);
		if ( Child->GetPropertiesFlags() & wxsItem::flId )
		{
		    const wxString& Name = Child->GetIdName();

		    if ( !wxsPredefinedIDs::Check(Name) )
		    {
                Array.Add(Name);
		    }
		}
		BuildIdsArrayReq(Child,Array);
	}
}

void wxsItemResData::BuildIncludesCode(wxsCodingLang Language,wxString& LocalIncludes,wxString& GlobalIncludes)
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
            LocalHeaders.Add(_T("<wx/fontmap.h>"));

            if ( GetEditMode() == wxsItemRes::Mixed )
            {
                LocalHeaders.Add(_T("<wx/xrc/xmlres.h>"));
            }

            GlobalHeaders.Sort();
            LocalHeaders.Sort();

            wxString Previous = _T("");
            for ( size_t i=0; i<GlobalHeaders.Count(); i++ )
            {
                if ( GlobalHeaders[i] != Previous )
                {
                    Previous = GlobalHeaders[i];
                    GlobalIncludes << _T("#include ") << Previous << _T("\n");
                }
            }

            Previous = _T("");
            for ( size_t i=0; i<LocalHeaders.Count(); i++ )
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
            wxsCodeMarks::Unknown(_T("wxsItemResData::BuildIncludesCode"),Language);
        }
    }
}

void wxsItemResData::BuildHeadersReq(wxsCodingLang Lang,wxsItem* Item,wxArrayString& LocalHeaders,wxArrayString& GlobalHeaders)
{
    Item->EnumDeclFiles(GlobalHeaders,LocalHeaders,Lang);

    wxsParent* Parent = Item->ConvertToParent();
    if ( !Parent ) return;

	int Cnt = Parent->GetChildCount();
	for ( int i=0; i<Cnt; i++ )
	{
	    BuildHeadersReq(Lang,Parent->GetChild(i),LocalHeaders,GlobalHeaders);
	}
}

void wxsItemResData::RebuildXrcFile()
{
    // First - opening file
    TiXmlDocument Doc;
    TiXmlElement* Resources;
    TiXmlElement* Object;

    if ( Doc.LoadFile(cbU2C(GetXrcFileName())) )
    {
        Resources = Doc.FirstChildElement("resource");
    }

    if ( !Resources )
    {
        Doc.Clear();
        Doc.InsertEndChild(TiXmlDeclaration("1.0","utf-8",""));
        Resources = Doc.InsertEndChild(TiXmlElement("resource"))->ToElement();
        Resources->SetAttribute("xmlns","http://www.wxwidgets.org/wxxrc");
    }

    // Searching for object representing this resource
    for ( Object = Resources->FirstChildElement("object"); Object; Object = Object->NextSiblingElement("object") )
    {
        if ( cbC2U(Object->Attribute("name")) == GetClassName() )
        {
            Object->Clear();
            while ( Object->FirstAttribute() )
            {
                Object->RemoveAttribute(Object->FirstAttribute()->Name());
            }
            break;
        }
    }

    if ( !Object )
    {
        Object = Resources->InsertEndChild(TiXmlElement("object"))->ToElement();
    }

    // The only things left are: to dump item into object ...
    m_RootItem->XmlWrite(Object,true,false);

    // ... and save back the file
    Doc.SaveFile(cbU2C(GetXrcFileName()));
}
