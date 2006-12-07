#include "wxsitemrestreedata.h"
#include "wxsitemresdata.h"
#include "wxsitem.h"

wxsItemResTreeData::wxsItemResTreeData(wxsItem* Item): m_Item(Item)
{
}

wxsItemResTreeData::~wxsItemResTreeData()
{
}

void wxsItemResTreeData::OnSelect()
{
    if ( m_Item )
    {
        m_Item->GetResourceData()->SelectItem(m_Item,true);
    }
}

void wxsItemResTreeData::OnRightClick()
{
}
