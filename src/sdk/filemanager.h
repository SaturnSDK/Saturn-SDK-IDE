#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <sdk_precomp.h>
#undef new
#include <deque>
#include <memory>

#ifndef CB_PRECOMP
    #include <wx/file.h>
    #include <wx/filename.h>
    #include "configmanager.h"
	#include <wx/thread.h>
	#include <wx/string.h>
	#include <wx/log.h>
#endif

#include <backgroundthread.h>


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

public:

    typedef enum
    {
    status_queued,
    status_inprogress,
    status_ready,
    status_failed_badrequest,
    status_failed_noaccess,
    status_failed_network,
    status_failed_unknown
    }lb_status_t;

protected:
    wxString fileName;
    char *data;
    size_t len;
    lb_status_t status;

    void Ready()
    {
        sem.Post();
    };

public:
    LoaderBase() : wait(true), data(0), len(0), status(status_queued) {};
    ~LoaderBase();
    void operator()() {};
    wxString FileName() const { return fileName; };
    virtual bool Writable() const {return false;};

    bool Sync();
    char* GetData();
    size_t GetLength();
    lb_status_t GetStatus(){return status;};
};


class FileLoader : public LoaderBase
{
public:
    FileLoader(const wxString& name) { fileName = name; };
    bool Writable() const {return wxFile::Access(fileName, wxFile::write);};
    void operator()();
};


/*
* Delete a file after a grace period. This is useful since we do not know when the filesystem will sync its data to disk.
* In fact, the filesystem might be on a physically separate machine.
* Thus, whenever replacing an existing file with a new one, you would want to be sure the changes you made are really on disk
* before deleting any backup files (in case the user pulls the plug).
* The job does nothing but sleep (giving the OS an opportunity to flush caches), the actual work is done in the destructor.
* This enables you to feed the job to a BackgroundThread that owns its jobs (and gradually deletes them one by one).
* As the actual work is done in the destructor, no stale files will be left at application exit.
*/
class DelayedDelete : public AbstractJob
{
wxString target;
public:
    DelayedDelete(const wxString& name) : target(name){};
    void operator()()
    {
        unsigned int i = 20;
        while(--i)
        {
            if(Manager::IsAppShuttingDown()) // make sure we don't hang up the application for seconds
                break;
            wxMilliSleep(75);
        }
    };
    ~DelayedDelete()
    {
        wxLogNull nullLog;
        if(wxFile::Exists(target))
            wxRemove(target);
    };
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
    NullLoader(const wxString& name, char* buffer, size_t size) { fileName = name; data = buffer; len = size; status = LoaderBase::status_ready; Ready(); };
    void operator()(){};
};


class FileManager : public Mgr<FileManager>
{
    BackgroundThread fileLoaderThread;
    BackgroundThread uncLoaderThread;
    BackgroundThread urlLoaderThread;
    BackgroundThread delayedDeleteThread;
public:
    FileManager() : fileLoaderThread(false), uncLoaderThread(false), urlLoaderThread(false){};

    LoaderBase* Load(const wxString& file, bool reuseEditors = false);

    bool Save(const wxString& file, const wxString& data, wxFontEncoding encoding, bool bom);
    bool Save(const wxString& file, const char* data, size_t len);
};


#endif
