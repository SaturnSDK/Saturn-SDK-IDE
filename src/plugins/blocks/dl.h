#ifndef DLODLKFJDLF_H
#define DLODLKFJDLF_H

#include "sdk_precomp.h"
#include <wx/url.h>


class AutoBuffer
{
/*
* AutoBuffer is a wrapper around a smart pointer managing a char buffer that grows to accomodate more data.
* One could think of it as a cross-breed of a std::vector and a file opened in append mode  :-)
* This nasty voodoo is necessary because wxWidgets is so fucking useless.
* The input stream returned by wxURL does not properly handle content length in 50% of the cases,
* returning -1 instead of the actual length, so you don't know how much memory to allocate.
* Instead of having the problem fixed, the function is now called "actually deprecated",
* and it is recommended to use wxInternetFSHandler...
* wxInternetFSHandler is even more braindead as it works on a local copy of the remote file which it downloads but never deletes!
* Apart from leaving gigabytes of dead files on your hard disk, it is entirely unable to determine the remote document's modification date,
* and to tell you a file's size, it has to download it first.
* You wonder why HTTP features a HEAD request and a Content-Length header...
*/
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


class DownloadJob
{
public:
    typedef enum
    {
        DL_OK,
        DL_CANNOT_CONNECT,
        DL_CONNECTION_ABORTED,
        DL_FILE_ERROR,
        DL_BAD_URL
    }StatusCode;

    typedef enum
    {
        DL_FILE,
        DL_MEMORY
    }Type;

    virtual Type GetType() const = 0;
    virtual StatusCode operator()() = 0;
    virtual int Progress() const {return -1;};
    virtual ~DownloadJob(){};
};



class FileDownloadJob : public DownloadJob
{
    wxString urlStr;
    wxString destDir;
    wxString filename;

    volatile int size;
    volatile int downloaded;

public:
    FileDownloadJob(const wxString& from, const wxString& to = wxEmptyString) : urlStr(from), destDir(to), size(0), downloaded(0)
    {
    if(destDir.IsEmpty())
        destDir = ConfigManager::GetTempFolder();
    };

    StatusCode operator()();

    Type GetType() const {return DownloadJob::DL_FILE;};
    int Progress() const {return size > 0 ? (int)(10000.0 * downloaded/size) : -1;};
    wxString FileName() const {return filename;};
};



class MemoryDownloadJob : public DownloadJob
{
    AutoBuffer buffer;
    wxString urlStr;

public:
    MemoryDownloadJob(const wxString& from) : urlStr(from){};

    const char* Data() const {return buffer.Data();};
    size_t Length() const {return buffer.Length();};
    Type GetType() const {return DownloadJob::DL_MEMORY;};

    StatusCode operator()();
};


#endif

