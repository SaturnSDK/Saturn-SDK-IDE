#include "wxsitemeditorundobuffer.h"

#include "../wxsitemres.h"

wxsItemEditorUndoBuffer::wxsItemEditorUndoBuffer(wxsItemEditor* Editor,int MaxEnteries):
    m_Resource(Resource),
    m_CurrentPos(0),
    m_SavedPos(0),
    m_MaxEnteries(MaxEnteries)
{
    StoreChange();  // Saving content of resource right after load
    Saved();        // To mark this is image of file content we mark this undo point as saved
}

wxsItemEditorUndoBuffer::~wxsItemEditorUndoBuffer()
{
    Clear();
}

void wxsItemEditorUndoBuffer::Clear()
{
    m_Enteries.clear();
    m_CurrentPos = 0;
    m_SavedPos = 0;
}

void wxsItemEditorUndoBuffer::StoreChange()
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

    // Adding new undo
    m_Enteries.Add(m_Editor->GetXmlData());

    // TODO (SpOoN#1#): Add selection

    m_CurrentPos = GetCount()-1;
}

bool wxsItemEditorUndoBuffer::Undo()
{
    if ( m_CurrentPos == 0 ) return false;
    return m_Editor->SetXmlData(Enteries[--CurrentPos],false);
}

bool wxsItemEditorUndoBuffer::Redo()
{
    if ( m_CurrentPos >= GetCount() - 1 ) return false;
    return m_Resource->SetXmlData(m_Enteries[++m_CurrentPos],false);
}
