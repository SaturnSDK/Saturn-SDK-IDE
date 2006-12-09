#include "wxsitemresdataobject.h"
#include "wxsitem.h"
#include "wxsitemfactory.h"
#include <sstream>

wxsItemResDataObject::wxsItemResDataObject(): m_ItemCount(0)
{
	Clear();
}

wxsItemResDataObject::~wxsItemResDataObject()
{
}

void wxsItemResDataObject::GetAllFormats(wxDataFormat *formats, Direction dir) const
{
    formats[0] = wxDataFormat(wxDF_TEXT);
    formats[1] = wxDataFormat(wxsDF_WIDGET);
}

bool wxsItemResDataObject::GetDataHere(const wxDataFormat& format,void *buf) const
{
	wxString XmlData = GetXmlData();
	wxWX2MBbuf str = cbU2C(XmlData);
    memcpy(buf,str,strlen(str)+1);
   	return true;
}

size_t wxsItemResDataObject::GetDataSize(const wxDataFormat& format) const
{
	wxString XmlData = GetXmlData();
	wxWX2MBbuf str = cbU2C(XmlData);
    return strlen(str)+1;
}

size_t wxsItemResDataObject::GetFormatCount(Direction dir) const
{
    return 2;
}

wxDataFormat wxsItemResDataObject::GetPreferredFormat(Direction dir) const
{
    return wxDataFormat(wxsDF_WIDGET);
}

bool wxsItemResDataObject::SetData(const wxDataFormat& format, size_t len, const void *buf)
{
    char* CharBuff = new char[len+1];
    memcpy(CharBuff,buf,len);

    // Just to make sure we won't go out of buffer
    CharBuff[len] = '\0';

    bool Ret = SetXmlData(cbC2U(CharBuff));
    delete[] CharBuff;
    return Ret;
}

void wxsItemResDataObject::Clear()
{
	m_XmlDoc.Clear();
	m_XmlElem = m_XmlDoc.InsertEndChild(TiXmlElement("resource"))->ToElement();
	m_ItemCount = 0;
}

bool wxsItemResDataObject::AddItem(wxsItem* Item)
{
	if ( !Item ) return false;
    TiXmlElement* Elem = m_XmlElem->InsertEndChild(TiXmlElement("object"))->ToElement();
    if ( !Elem ) return false;
    if ( !Item->XmlWrite(Elem,true,true) )
    {
        m_XmlElem->RemoveChild(Elem);
        return false;
    }
	m_ItemCount++;
	return true;
}

int wxsItemResDataObject::GetItemCount() const
{
	return m_ItemCount;
}

wxsItem* wxsItemResDataObject::BuildItem(wxsItemResData* Data,int Index) const
{
	if ( Index < 0 || Index >= m_ItemCount ) return NULL;

	TiXmlElement* Root = m_XmlElem->FirstChildElement("object");
	if ( !Root ) return NULL;
	while ( Index )
	{
		Index--;
		Root = Root->NextSiblingElement("object");
		if ( !Root ) return NULL;
	}
	const char* Class = Root->Attribute("class");
	if ( !Class || !*Class ) return NULL;

	wxsItem* Item = wxsItemFactory::Build(cbC2U(Class),Data);
	if ( !Item )
	{
        Item = wxsItemFactory::Build(_T("Custom"),Data);
        if ( !Item ) return false;
	}

	Item->XmlRead(Root,true,true);
	return Item;
}

bool wxsItemResDataObject::SetXmlData(const wxString& Data)
{
    m_XmlDoc.Clear();
    m_ItemCount = 0;
	m_XmlDoc.Parse(cbU2C(Data));
    if ( m_XmlDoc.Error() )
    {
        DBGLOG(_T("wxSmith: Error loading Xml data -> ") + cbC2U(m_XmlDoc.ErrorDesc()));
    	Clear();
    	return false;
    }

    m_XmlElem = m_XmlDoc.FirstChildElement("resource");
    if ( !m_XmlElem )
    {
    	Clear();
    	return false;
    }

    for ( TiXmlElement* Elem = m_XmlElem->FirstChildElement("object");
          Elem;
          Elem = Elem->NextSiblingElement("object") )
    {
    	m_ItemCount++;
    }

    return true;
}

wxString wxsItemResDataObject::GetXmlData() const
{
    TiXmlPrinter Printer;
    m_XmlDoc.Accept(&Printer);
    return cbC2U(Printer.CStr());
}
