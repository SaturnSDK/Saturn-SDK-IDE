#include "tinywxuni.h"
#include "tinyxml.h"

#include <wx/string.h>
#include <wx/file.h>


bool TinyXML::LoadDocument(const wxString& filename, TiXmlDocument *doc)
{
    if(!wxFile::Access(filename, wxFile::read))
        return false;

    wxFile file(filename);
    char *input = new char[file.Length()];
    file.Read(input, file.Length());
    doc->Parse(input);
    delete[] input;
    return true;
}

TiXmlDocument* TinyXML::LoadDocument(const wxString& filename)
{
    TiXmlDocument* doc = new TiXmlDocument();

    if(TinyXML::LoadDocument(filename, doc))
        return doc;
    else
        {
        delete doc;
        return 0;
        }
}

bool TinyXML::SaveDocument(const wxString& filename, TiXmlDocument* doc)
{
    if (!doc)
        return false;

    const char *buffer; // UTF-8 encoded data
  	size_t len;

  	#ifdef TIXML_USE_STL
        std::string outSt;
        outSt << *doc;
        buffer = outSt.c_str();
        len = outSt.length();
  	#else
        TiXmlOutStream outSt;
        outSt << *doc;
        buffer = outSt.c_str();
        len = outSt.length();
  	#endif

    wxTempFile file(filename);
    if (file.IsOpened())
    {
        if (!file.Write(buffer, strlen(buffer)))
            return false;
        if (!file.Commit())
            return false;
    }
    else
        return false;
  	return true;
}

