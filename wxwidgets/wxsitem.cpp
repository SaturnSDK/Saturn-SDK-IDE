#include "wxsitem.h"
#include "wxsparent.h"
#include "wxsitemres.h"

wxsItem::wxsItem(wxsItemRes* Resource,const wxsItemInfo* Info,unsigned long PropertiesFlags,const wxsEventDesc* Events):
    m_Info(Info),
    m_Events(Events,this),
    m_Parent(NULL),
    m_Resource(Resource),
    m_VarName(_T("")),
    m_IdName(_T("")),
    m_IsMember(true),
    m_BaseProperties(NULL),
    m_PropertiesFlags(PropertiesFlags),
    m_LastPreview(NULL),
    m_IsSelected(false),
    m_IsExpanded(false)
{
    if ( m_PropertiesFlags & wxsBaseProperties::flAll )
    {
        m_BaseProperties = new wxsBaseProperties;
    }
}

wxsItem::~wxsItem()
{
    delete m_BaseProperties;
}

void wxsItem::OnEnumProperties(long Flags)
{
    if ( (Flags & flPropGrid) && (m_Parent != NULL) )
    {
        // Parent item does take care of enumerating properties if we are
        // ceating property grid
        m_Parent->OnEnumChildProperties(this,Flags);
    }
    else
    {
        EnumItemProperties(Flags);
    }
}

wxsQuickPropsPanel* wxsItem::OnCreateQuickProperties(wxWindow* ParentWnd)
{
    wxsAdvQPP* Panel = new wxsAdvQPP(ParentWnd,this);

    if ( m_Parent != NULL )
    {
        // Parent item does take care of inserting QPP Children
        m_Parent->OnAddChildQPP(this,Panel);
    }
    else
    {
        OnAddItemQPP(Panel);
    }

    // Appending qpp's from base properties
    if ( m_BaseProperties )
    {
        m_BaseProperties->AddQPPChild(Panel,GetPropertiesFlags());
    }

    return Panel;
}

long wxsItem::OnGetPropertiesFlags()
{
    return m_Resource->GetPropertiesFilter() | m_PropertiesFlags;
}

void wxsItem::EnumItemProperties(long Flags)
{
    // Registering variable name / identifier
    // these values are skipped when storing into xml variable
    // because itis stored as attribute of XML element
    if ( (Flags & (flPropGrid|flPropStream)) != 0 )
    {
        WXS_STRING(wxsItem,m_VarName,flVariable,_("Var name"),_T("var_name"),wxEmptyString,false,false);
        WXS_BOOL(wxsItem,m_IsMember,flVariable,_(" Is member"),_T("var_is_member"),true);
        WXS_STRING(wxsItem,m_IdName,flId,_("Identifier"),_T("identifier"),wxEmptyString,false,false);
    }

    OnEnumItemProperties();

    // Now enumerating all properties from wxsBaseProperties if any
    if ( m_BaseProperties )
    {
        SubContainer(m_BaseProperties);
    }
}

void wxsItem::OnBuildDeclarationCode(wxString& Code,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
            Code << GetClassName() << _T("* ") << GetVarName() << _T(";\n");
            return;

        default:
            wxsCodeMarks::Unknown(_T("wxsItem::OnBuildDeclarationCode"),Language);
    }
}

bool wxsItem::OnXmlRead(TiXmlElement* Element,bool IsXRC,bool IsExtra)
{
    if ( IsXRC )
    {
        wxsPropertyContainer::XmlRead(Element);
        m_IdName = cbC2U(Element->Attribute("name"));
    }

    if ( IsExtra )
    {
        m_VarName = cbC2U(Element->Attribute("variable"));
        const char* MbrText = Element->Attribute("member");
        m_IsMember = !MbrText || !strcmp(MbrText,"yes");
        m_Events.XmlLoadFunctions(Element);
    }

    return true;
}

bool wxsItem::OnXmlWrite(TiXmlElement* Element,bool IsXRC,bool IsExtra)
{
    if ( IsXRC )
    {
        Element->SetAttribute("class",cbU2C(GetClassName()));
        wxsPropertyContainer::XmlWrite(Element);
        if ( GetPropertiesFlags() & flId )
        {
            Element->SetAttribute("name",cbU2C(m_IdName));
        }
    }

    if ( IsExtra )
    {
        if ( GetPropertiesFlags() & flVariable )
        {
            Element->SetAttribute("variable",cbU2C(m_VarName));
            Element->SetAttribute("member",m_IsMember ? "yes" : "no" );
        }
        m_Events.XmlSaveFunctions(Element);
    }

    return true;
}

void wxsItem::BuildItemTree(wxsResourceTree* Tree,wxsResourceItemId Parent,int Position)
{
    /* TODO: Code wxsResourceTreeData replacement for wxsItem class */
    if ( Position<0 || Position>=(int)Tree->GetChildrenCount(Parent) )
    {
        m_LastTreeId = Tree->AppendItem(Parent,GetClassName(),-1,-1/*,new wxsResourceTreeData(this)*/);
    }
    else
    {
        m_LastTreeId = Tree->InsertItem(Parent,Position,GetClassName(),-1,-1/*,new wxsResourceTreeData(this)*/);
    }
    if ( !GetIsExpanded() )
    {
        Tree->Collapse(m_LastTreeId);
    }

    wxsParent* ParentItem = ConvertToParent();
    if ( ParentItem )
    {
        int Count = ParentItem->GetChildCount();
        for ( int i=0; i<Count; i++ )
        {
            ParentItem->GetChild(i)->BuildItemTree(Tree,m_LastTreeId);
        }
    }
}

wxObject* wxsItem::BuildPreview(wxWindow* Parent,bool Exact,bool StoreInLastPreview)
{
    wxObject* Preview = OnBuildPreview(Parent,Exact);
    if ( StoreInLastPreview )
    {
        m_LastPreview = Preview;
    }
    return Preview;
}

void wxsItem::ClearSelection()
{
    m_IsSelected = false;
    wxsParent* Parent = ConvertToParent();
    if ( Parent )
    {
        for ( int i = Parent->GetChildCount(); i-->0; )
        {
            Parent->GetChild(i)->ClearSelection();
        }
    }
}

void wxsItem::OnPropertyChanged()
{
    GetResource()->NotifyChange(this);
}

void wxsItem::OnSubPropertyChanged(wxsPropertyContainer*)
{
    GetResource()->NotifyChange(this);
}
