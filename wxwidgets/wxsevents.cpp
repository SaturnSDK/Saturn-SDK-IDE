#include "wxsevents.h"
#include "wxsitem.h"

#include <messagemanager.h>

#define HandlerXmlElementName   "handler"
#define HandlerXmlEntryName     "entry"
#define HandlerXmlTypeName      "type"
#define HandlerXmlFunctionName  "function"

wxsEvents::wxsEvents(const wxsEventDesc* Events,wxsItem* Item):
    m_Item(Item),
    m_EventArray(Events),
    m_Count(0)
{
    // Counting number of events
    if ( m_EventArray )
    {
        for ( const wxsEventDesc* Event = m_EventArray;
              Event->ET != wxsEventDesc::EndOfList;
              Event++ )
        {
            m_Count++;
        }
    }

    m_Functions.SetCount(m_Count);
}

void wxsEvents::XmlLoadFunctions(TiXmlElement* Element)
{
	for ( TiXmlElement* Handler = Element->FirstChildElement(HandlerXmlElementName);
          Handler;
          Handler = Handler->NextSiblingElement(HandlerXmlElementName) )
    {
    	const char* EntryName    = Handler->Attribute(HandlerXmlEntryName);
    	const char* TypeName     = Handler->Attribute(HandlerXmlTypeName);
    	const char* FunctionName = Handler->Attribute(HandlerXmlFunctionName);

        // There must be function name, otherwise this handler definition is useless
    	if ( !FunctionName ) continue;

    	if ( EntryName )
    	{
    	    // Function given by event entry
    	    wxString Name = cbC2U(EntryName);
    	    for ( int i=0; i<m_Count; i++ )
    	    {
    	        if ( m_EventArray[i].Entry == Name )
    	        {
    	            m_Functions[i] = cbC2U(FunctionName);
    	            break;
    	        }
    	    }
    	}
    	else
    	{
    	    // Function given by event type
    	    wxString Name = cbC2U(TypeName);
    	    for ( int i=0; i<m_Count; i++ )
    	    {
    	        if ( m_EventArray[i].Type == Name )
    	        {
    	            m_Functions[i] = cbC2U(FunctionName);
    	        }
    	    }
    	}
    }
}

void wxsEvents::XmlSaveFunctions(TiXmlElement* Element)
{
    for ( int i=0; i<m_Count; i++ )
    {
    	if ( !m_Functions[i].empty() )
    	{
    		TiXmlElement* Handler = Element->InsertEndChild( TiXmlElement(HandlerXmlElementName) ) -> ToElement();
    		Handler->SetAttribute(HandlerXmlFunctionName,cbU2C(m_Functions[i]));
    		if ( !m_EventArray[i].Entry.empty() )
    		{
                Handler->SetAttribute(HandlerXmlEntryName,cbU2C(m_EventArray[i].Entry));
    		}
    		else
    		{
    		    Handler->SetAttribute(HandlerXmlTypeName,cbU2C(m_EventArray[i].Type));
    		}
    	}
    }
}

void wxsEvents::GenerateBindingCode(wxString& Code,bool UsingXrc,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            wxString RealId = UsingXrc ?
                ( _T("XRCID(") + m_Item->GetIdName() + _T(")") ) :
                m_Item->GetIdName();

            // First we have to check i
            for ( int i=0; i<m_Count; i++ )
            {
                if ( !m_Functions[i].empty() )
                {
                    switch ( m_EventArray[i].ET )
                    {
                        case wxsEventDesc::Id:
                            Code << _T("Connect(") << RealId << _T(",")
                                 << m_EventArray[i].Type << _T("(wxObjectEventFunction)")
                                 << m_Functions[i] << _T(");\n");
                            break;

                        case wxsEventDesc::NoId:
                            Code << Item->GetVarName() << _T("->Connect(") << RealId
                                 << _T(",") << m_EventArray[i].Type
                                 << _T("(wxObjectEventFunction)") << m_Functions[i]
                                 << _T(",NULL,this);\n");
                            break;

                        default:
                            break;
                    }
                }
            }
            return;
        }
    }
}

bool wxsEvents::ForceVariable()
{
    for ( int i=0; i<Count; i++ )
    {
        if ( !m_Functions[i].empty() )
        {
            return true;
        }
    }
    return false;
}
