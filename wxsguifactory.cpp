#include "wxsguifactory.h"
#include "wxsgui.h"

wxsGUIFactory::wxsGUIFactory(const wxString& Name): m_Name(Name)
{
    // Registering this gui in new hash
    GetHash()[m_Name] = this;
}

wxsGUIFactory::~wxsGUIFactory()
{
    // Assuming that factories are available
    // during all wxSmith work time, it's not
    // necessarry to remove any bindings
}

wxsGUI* wxsGUIFactory::Build(const wxString& Name,wxsProject* Project)
{
    if ( GetHash().find(Name) == GetHash().end() ) return NULL;
    wxsGUIFactory* Factory = GetHash()[Name];
    wxsGUI* NewGUI = Factory->OnCreate(Project);
    if ( NewGUI->GetName() != Name )
    {
        // Some hack? Bug in factory?
        DBGLOG(_T("wxSmith: Error while creating wxsGUI object (name mismatch)."));
        DBGLOG(_T("wxSmith:   Looks like bug in one wf wxsGUIFactory-derived classes or"));
        DBGLOG(_T("wxSmith:   some hack attempt."));
        delete NewGUI;
        return NULL;
    }
    return NewGUI;
}

wxsGUI* wxsGUIFactory::SelectNew(const wxString& Message,wxsProject* Project)
{
    if ( GetHash().empty() )
    {
        return NULL;
    }
    if ( GetHash().size() == 1 )
    {
        return Build(GetHash().begin()->first,Project);
    }

    wxArrayString GUIList;
    for ( GUIItemHashT::iterator i = GetHash().begin(); i!=GetHash().end(); ++i )
    {
        GUIList.Add(i->first);
    }

    wxString SelectedGUI = ::wxGetSingleChoice(Message,_("Select GUI"), GUIList);
    if ( SelectedGUI.empty() )
    {
        return NULL;
    }

    return Build(SelectedGUI,Project);
}

inline wxsGUIFactory::GUIItemHashT& wxsGUIFactory::GetHash()
{
    static GUIItemHashT Hash;
    return Hash;
}
