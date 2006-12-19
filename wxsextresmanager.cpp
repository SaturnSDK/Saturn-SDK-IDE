#include "wxsextresmanager.h"
#include "wxsmith.h"
#include "wxsresourcefactory.h"

wxsExtResManager::wxsExtResManager():
    m_ClosingAll(false)
{
}

wxsExtResManager::~wxsExtResManager()
{
    int TestCnt = m_Files.size();
    while ( !m_Files.empty() )
    {
        // Each delete should remove one entry
        delete m_Files.begin()->second;
        // Just in case of invalid resources to avoid infinite loops
        if ( --TestCnt < 0 ) break;
    }
}

bool wxsExtResManager::CanOpen(const wxString& FileName)
{
    if ( m_Files.find(FileName) != m_Files.end() ) return true;
    return wxsResourceFactory::CanHandleExternal(FileName);
}

bool wxsExtResManager::Open(const wxString& FileName)
{
    if ( m_Files.find(FileName) == m_Files.end() )
    {
        wxsResource* NewResource = wxsResourceFactory::BuildExternal(FileName);
        if ( !NewResource ) return false;
        NewResource->BuildTreeEntry(wxsTree()->ExternalResourcesId());
        m_Files[FileName] = NewResource;
        NewResource->EditOpen();
        return true;
    }

    m_Files[FileName]->EditOpen();
    return true;
}

void wxsExtResManager::EditorClosed(wxsResource* Res)
{
    if ( m_ClosingAll ) return;

    for ( FilesMapI i = m_Files.begin(); i!=m_Files.end(); ++i )
    {
        if ( i->second == Res )
        {
            m_Files.erase(i);
            wxsTree()->Delete(Res->GetTreeItemId());
            delete Res;
            if ( m_Files.empty() )
            {
                wxsTree()->DeleteExternalResourcesId();
            }
            return;
        }
    }
}

void wxsExtResManager::DeleteAll()
{
    m_ClosingAll = true;

    for ( FilesMapI i = m_Files.begin(); i!=m_Files.end(); ++i )
    {
        delete i->second;
    }
    m_Files.clear();
    wxsTree()->DeleteExternalResourcesId();

    m_ClosingAll = false;
}

wxsExtResManager wxsExtResManager::m_Singleton;
