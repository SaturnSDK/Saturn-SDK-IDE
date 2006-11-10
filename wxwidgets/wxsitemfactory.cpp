#include "wxsitemfactory.h"
#include "wxsitemmanager.h"
#include "wxsitem.h"

wxsItem* wxsItemFactory::Build(const wxString& Name,wxsItemRes* Resource)
{
    ItemMapT::iterator it = ItemMap().find(Name);
    if ( it == ItemMap().end() ) return NULL;
    return it->second->OnBuild(Resource);
}

const wxsItemInfo* wxsItemFactory::GetInfo(const wxString& Name)
{
    ItemMapT::iterator it = ItemMap().find(Name);
    if ( it == ItemMap().end() ) return NULL;
    return it->second->m_Info;
}

const wxsItemInfo* wxsItemFactory::GetFirstInfo()
{
    m_Iter = ItemMap().begin();
    return (m_Iter==ItemMap().end()) ? NULL : m_Iter->second->m_Info;
}

const wxsItemInfo* wxsItemFactory::GetNextInfo()
{
    if ( m_Iter==ItemMap().end() ) return NULL;
    ++m_Iter;
    return (m_Iter==ItemMap().end()) ? NULL : m_Iter->second->m_Info;
}

wxsItemFactory::wxsItemFactory(const wxsItemInfo* Info):
    m_Info(Info)
{
    if ( Info==NULL ) return;
    ItemMap()[Info->ClassName] = this;
}

wxsItemFactory::~wxsItemFactory()
{
    if ( !m_Info ) return;
    ItemMapT::iterator it = ItemMap().find(m_Info->ClassName);
    if ( it == ItemMap().end() ) return;
    if ( it->second!=this ) return;
    ItemMap().erase(it);
}

wxsItemFactory::ItemMapT& wxsItemFactory::ItemMap()
{
    static ItemMapT Map;
    return Map;
}

wxsItemFactory::ItemMapT::iterator wxsItemFactory::m_Iter(wxsItemFactory::ItemMap().end());
