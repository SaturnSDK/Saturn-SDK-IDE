#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "filemanager.h"
    #include "safedelete.h"
#endif

#include <wx/url.h>

LoaderBase::~LoaderBase()
{
    delete[] data;
};

bool LoaderBase::Sync()
{
    WaitReady();
    return data;
};

char* LoaderBase::GetData()
{
    WaitReady();
    return data;
};

size_t LoaderBase::GetLength()
{
    WaitReady();
    return len;
};



void FileLoader::operator()()
{
    if(!wxFile::Access(fileName, wxFile::read))
        {
        Ready();
        return;
        }

    wxFile file(fileName);
    len = file.Length();

    data = new char[len+1];
    data[len] = '\0';

    if(file.Read(data, len) == wxInvalidOffset)
    {
        delete[] data;
        data = 0;
        len = 0;
    }
    Ready();
}

void URLLoader::operator()()
{
    wxURL url(fileName);
    url.SetProxy(ConfigManager::GetProxy());

    if (url.GetError() != wxURL_NOERR)
        {
        Ready();
        return;
        }

    std::auto_ptr<wxInputStream> stream(url.GetInputStream());

    if (stream.get() == 0 || stream->IsOk() == false)
        {
        Ready();
        return;
        }

    char tmp[8192];
    size_t chunk = 0;

    while(chunk = stream->Read(tmp, sizeof(tmp)).LastRead())
        buffer.Append(tmp, chunk);

    data = buffer.Data();
    len = buffer.Length();
    Ready();
}


LoaderBase* FileManager::Load(const wxString& file, bool reuseEditors)
{
    if(reuseEditors)
    {
        EditorManager* em = Manager::Get()->GetEditorManager();
        if(em)
        {
            wxFileName fileName(file);
            for(int i = 0; i < em->GetEditorsCount(); ++i)
            {
                cbEditor* ed = em->GetBuiltinEditor(em->GetEditor(i));
                if(ed && fileName == ed->GetFilename())
                {
                    wxString s(ed->GetControl()->GetText());
                    NullLoader *nl = new NullLoader(file, (char*) s.c_str(), s.length() * sizeof(wxChar));
                    return nl;
                }
            }
        }
    }

    if(file.StartsWith(_T("http://")))
    {
        URLLoader *ul = new URLLoader(file);
        urlLoaderThread.Queue(ul);
        return ul;
    }

    FileLoader *fl = new FileLoader(file);

    if(file.length() > 2 && file[0] == _T('\\') && file[1] == _T('\\'))
    {
        // UNC files behave like "normal" files, but since we know they are served over the network,
        // we can run them independently from local filesystem files for higher concurrency
        uncLoaderThread.Queue(fl);
        return fl;
    }

    fileLoaderThread.Queue(fl);
    return fl;
}
