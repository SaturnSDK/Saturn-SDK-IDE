#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <sdk_precomp.h>
#undef new
#include <deque>

class LoaderBase : public AbstractJob
{
    wxSemaphore sem;
    bool wait;

    void WaitReady()
    {
        if(wait)
        {
            wait = false;
            sem.Wait();
        }
    };

protected:
    wxString fileName;
    char *data;
    size_t len;

    void Ready()
    {
        sem.Post();
    };

public:
    LoaderBase() : wait(true), data(0), len(0) {};
    ~LoaderBase();
    void operator()() {};
    wxString FileName() const { return fileName; };

    bool Sync();
    char* GetData();
    size_t GetLength();
};


class FileLoader : public LoaderBase
{
public:
    FileLoader(const wxString& name) { fileName = name; };
    void operator()();
};




class AutoBuffer
{
std::auto_ptr<char> ptr;
size_t len;

public:
    AutoBuffer() : ptr(0), len(0){};
    AutoBuffer(size_t initial) : ptr(new char[initial]), len(initial){};

    void Alloc(size_t size)
    {
        std::auto_ptr<char> tmp(new char[len + size]);
        if(ptr.get())
            memcpy(tmp.get(), ptr.get(), len);
        ptr = tmp;
    };

    void Append(char *add_buf, size_t add_len)
    {
        Alloc(add_len);
        memcpy(ptr.get() + len, add_buf, add_len);
        len += add_len;
    };

    size_t Length() const {return len;};
    char *Data() const {return ptr.get();};
};


class URLLoader : public LoaderBase
{
AutoBuffer buffer;
public:
    URLLoader(const wxString& name) { fileName = name; };
    void operator()();
};


class NullLoader : public LoaderBase
{
public:
    NullLoader(const wxString& name, char* buffer, size_t size) { fileName = name; data = buffer; len = size; Ready(); };
    void operator()(){};
};


class FileManager : public Mgr<FileManager>
{
    BackgroundThread fileLoaderThread;
    BackgroundThread uncLoaderThread;
    BackgroundThread urlLoaderThread;
public:
    FileManager() : fileLoaderThread(false), uncLoaderThread(false), urlLoaderThread(false){};

    LoaderBase* Load(const wxString& file, bool reuseEditors = false);

    bool Save(const wxString& file, const wxString& contents)          { assert(0 /* not implemented */); };
    bool Save(const wxString& file, const char* contents, size_t len)  { assert(0 /* not implemented */); };
};


#endif
