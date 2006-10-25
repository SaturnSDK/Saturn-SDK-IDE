#include "wxwidgetsresfactory.h"
#include "wxsdialogres.h"
#include "wxsnewwindowdlg.h"

namespace
{
    wxWidgetsResFactory Factory;

    enum Resources
    {
        wxDialogId = 0,
        /*=========*/
        ResourcesCount
    };

    const wxChar* NamesPtr[ResourcesCount] =
    {
        _T("wxDialog")
    };

    wxArrayString Names(ResourcesCount,NamesPtr);

    WX_DEFINE_ARRAY(TiXmlElement*,wxArrayElement);
}


wxWidgetsResFactory::wxWidgetsResFactory()
{
}

int wxWidgetsResFactory::OnGetCount()
{
    return ResourcesCount;
}

void wxWidgetsResFactory::OnGetInfo(int Number,wxString& Name,wxString& GUI)
{
    GUI = _T("wxWidgets");
    switch ( Number )
    {
        case wxDialogId: Name = _T("wxDialog"); break;
    }
}

wxsResource* wxWidgetsResFactory::OnCreate(int Number,wxsProject* Project)
{
    switch ( Number )
    {
        case wxDialogId: return new wxsDialogRes(Project);
    }
    return NULL;
}

bool wxWidgetsResFactory::OnCanHandleExternal(const wxString& FileName)
{
    return wxFileName(FileName).GetExt().Upper() == _T("XRC");
}

wxsResource* wxWidgetsResFactory::OnBuildExternal(const wxString& FileName)
{
    TiXmlDocument Doc;
    if ( !Doc.LoadFile(cbU2C(FileName)) ) return NULL;

    wxArrayString ResourcesFound;
    wxArrayElement XmlElements;
    TiXmlElement* Res = Doc.FirstChildElement("resource");
    if ( !Res )
    {
        // TODO: Some message box about invalid XRC resource structure
        return NULL;
    }

    for ( TiXmlElement* Object = Res->FirstChildElement("object"); Object; Object=Object->NextSiblingElement("object") )
    {
        wxString Class = cbC2U(Object->Attribute("class"));
        wxString Name = cbC2U(Object->Attribute("name"));
        if ( !Name.empty() && Names.Index(Class) != wxNOT_FOUND )
        {
            ResourcesFound.Add(Name + _T(" (") + Class + _T(")"));
        }
    }

    if ( ResourcesFound.empty() )
    {
        // TODO: Message box that there are no resoures which could be edited here
        return NULL;
    }

    int Choice = 0;
    if ( ResourcesFound.size() > 1 )
    {
        Choice = ::wxGetSingleChoiceIndex(
            _("There's more than one resource in this file.\n"
              "Please select which one should be edited."),
            _("Choose resource to edit"),
            ResourcesFound);
        if ( Choice<0 )
        {
            return NULL;
        }
    }

    TiXmlElement* Object = XmlElements[Choice];
    if ( !Object ) return NULL;

    wxString Class = cbC2U(Object->Attribute("class"));
    switch ( Names.Index(Class) )
    {
        case wxDialogId: return new wxsDialogRes(FileName,Object);
    }
    return NULL;
}

bool wxWidgetsResFactory::OnNewWizard(int Number,wxsProject* Project)
{
    wxsNewWindowDlg Dlg(NULL,NamesPtr[Number],Project);
    return Dlg.ShowModal() == wxID_OK;
}
