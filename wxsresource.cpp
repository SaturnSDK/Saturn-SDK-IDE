#include "wxsresource.h"
#include "wxsextresmanager.h"
#include "wxsresourcetreeitemdata.h"

#include <wx/wxFlatNotebook/wxFlatNotebook.h>
#include <editormanager.h>

namespace
{
    // This object will cause resource to open itself
    // when it's clicked in resource tree
    class wxsResourceOpening: public wxsResourceTreeItemData
    {
        public:
            wxsResourceOpening(wxsResource* Resource):
                wxsResourceTreeItemData(),
                m_Resource(Resource)
            {
            }

        private:

            virtual void OnSelect()
            {
                m_Resource->EditOpen();
            }

            wxsResource* m_Resource;
    };
}

wxsResource::wxsResource(wxsProject* Owner,const wxString& ResourceType,const wxString& GUI):
    m_ResourceType(ResourceType),
    m_ResourceName(wxEmptyString),
    m_GUI(GUI),
    m_Owner(Owner),
    m_Editor(NULL),
    m_Language(wxsCPP)
{}

wxsResource::~wxsResource()
{
    if ( m_Editor )
    {
        wxsEditor* EditorStore = m_Editor;
        m_Editor = NULL;
        EditorStore->Close();
    }
}

void wxsResource::EditOpen()
{
    if ( m_Editor )
    {
        m_Editor->Activate();
    }
    else
    {
        m_Editor = OnCreateEditor(Manager::Get()->GetEditorManager()->GetNotebook());
    }
}

void wxsResource::EditClose()
{
    if ( m_Editor )
    {
        m_Editor->Close();
        // Inside Close() m_Editor should be zeroed
    }
}

void wxsResource::EditorClosed()
{
    m_Editor = NULL;
    if ( !m_Owner )
    {
        wxsExtRes()->EditorClosed(this);
    }
}

void wxsResource::BuildTreeEntry(const wxsResourceItemId& Parent)
{
    m_TreeItemId = wxsTree()->AppendItem(
        Parent,
        GetResourceName(),
        -1, -1,
        new wxsResourceOpening(this));
}

bool wxsResource::ReadConfig(const TiXmlElement* Node)
{
    m_ResourceName = cbC2U(Node->Attribute("name"));
    m_Language = wxsCodeMarks::Id(cbC2U(Node->Attribute("language")));
    if ( GetResourceName().empty() ) return false;
    return OnReadConfig(Node);
}

bool wxsResource::WriteConfig(TiXmlElement* Node)
{
    bool Result = OnWriteConfig(Node);
    Node->SetAttribute("name",cbU2C(m_ResourceName));
    Node->SetAttribute("language",cbU2C(wxsCodeMarks::Name(m_Language)));
    return Result;
}

IMPLEMENT_CLASS(wxsResource,wxObject)
