#include "wxsitemundobuffer.h"

namespace
{
    wxString Empty = wxEmptyString;
}

wxsItemUndoBuffer::wxsItemUndoBuffer(int MaxEnteries):
    m_CurrentPos(0),
    m_SavedPos(0),
    m_MaxEnteries(MaxEnteries)
{
}

wxsItemUndoBuffer::~wxsItemUndoBuffer()
{
}

void wxsItemUndoBuffer::StoreChange(const wxString& XmlData)
{
    // Removing all undo points after current one
    if ( m_CurrentPos < GetCount()-1 )
    {
        m_Enteries.RemoveAt(m_CurrentPos+1,GetCount()-m_CurrentPos-1);
    }

    // Removing all outdated undos
    if ( m_MaxEnteries > 0 )
    {
        int ToRemove = GetCount()-m_MaxEnteries;
        if ( ToRemove > 0 )
        {
            m_Enteries.RemoveAt(0,ToRemove);
            m_CurrentPos -= ToRemove;
            m_SavedPos -= ToRemove;
        }
    }

    m_Enteries.Add(XmlData);
    m_CurrentPos = GetCount()-1;
}

const wxString& wxsItemUndoBuffer::Undo()
{
    if ( m_CurrentPos == 0 ) return Empty;
    return m_Enteries[--m_CurrentPos];
}

const wxString& wxsItemUndoBuffer::Redo()
{
    if ( m_CurrentPos >= GetCount() - 1 ) return Empty;
    return m_Enteries[++m_CurrentPos];
}
