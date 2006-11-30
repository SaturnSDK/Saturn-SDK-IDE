#include "wxsversionconverter.h"

const wxsVersionConverter& wxsVersionConverter::Get()
{
    static wxsVersionConverter Singleton;
    return Singleton;
}

bool wxsVersionConverter::DetectOldConfig(TiXmlElement* Node) const
{
    return false;
}

TiXmlElement* wxsVersionConverter::ConvertFromOldConfig(TiXmlElement* ConfigNode,TiXmlDocument* Doc) const
{
    return NULL;
}

TiXmlElement* wxsVersionConverter::Convert(TiXmlElement* ConfigNode,TiXmlDocument* Doc) const
{
    return NULL;
}
