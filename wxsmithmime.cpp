#include "wxsmithmime.h"

#include "wxsmith.h"
#include "wxsproject.h"
#include "wxsextresmanager.h"

namespace
{
    PluginRegistrant<wxSmithMime> reg(_T("wxSmithMime"));
}

// TODO: Do not access wxSmith's maps directly

wxSmithMime::wxSmithMime()
{
}

bool wxSmithMime::CanHandleFile(const wxString& FileName) const
{
    if ( !wxsPlugin() ) return false;

    // Scanning for projects using this file
    for ( wxSmith::ProjectMapI i = wxsPlugin()->m_ProjectMap.begin();
          i!=wxsPlugin()->m_ProjectMap.end();
          ++i )
    {
        wxsProject* Proj = (*i).second;
        if ( Proj->CanOpenEditor(FileName) )
        {
            return true;
        }
    }

    if ( wxsExtRes()->CanOpen(FileName) )
    {
        return true;
    }

    return false;
}

int wxSmithMime::OpenFile(const wxString& FileName)
{
    if ( !wxsPlugin() ) return 1;

    // Scanning for projects using this file
    for ( wxSmith::ProjectMapI i = wxsPlugin()->m_ProjectMap.begin();
          i!=wxsPlugin()->m_ProjectMap.end();
          ++i )
    {
        wxsProject* Proj = (*i).second;
        if ( Proj->TryOpenEditor(FileName) )
        {
            return 0;
        }
    }

    if ( wxsExtRes()->Open(FileName) )
    {
        return 0;
    }

    return 1;
}
