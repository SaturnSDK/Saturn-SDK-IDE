#ifndef MYTAR_H
#define MYTAR_H

#include <wx/string.h>
#include <sdk.h>


class TAR
{
    public:
        struct Header
        {
            char name[100];
            char mode[8];
            char uid[8];
            char gid[8];
            char size[12];
            char mtime[12];
            char chksum[8];
            char typeflag;
            char linkname[100];
            char magic[6];
            char version[2];
            char uname[32];
            char gname[32];
            char devmajor[8];
            char devminor[8];
            char prefix[155];
            char padding[12];
//            char *gnu_longname;
//            char *gnu_longlink;
        } __attribute__((__packed__));

        enum FileType
        {
            ftNormal,          // Regular file
            ftLink,            // Link to another, previously archived, file (LinkName)
            ftSymbolicLink,    // Symbolic link to another file              (LinkName)
            ftCharacter,       // Character special files
            ftBlock,           // Block special files
            ftDirectory,       // Directory entry. Size is zero (unlimited) or max. number of bytes
            ftFifo,            // FIFO special file. No data stored in the archive.
            ftContiguous,      // Contiguous file, if supported by OS
            ftDumpDir,         // List of files
            ftMultiVolume,     // Multi-volume file part
            ftVolumeHeader,    // Volume header. Can appear only as first record in the archive
            ftLongName,
            ftLongLink
        };

        struct Record
        {
            wxString name;
            int size;
            size_t pos;
            FileType ft;
        };

        TAR(const wxString& filename = 0);
        void Reset();

        bool Next(Record* rec);
        bool ExtractFile(Record* rec, const wxString& destination);
        Record* FindFile(const wxString& filename, bool reset = false);

        wxString LastError() const {return error;};
    protected:
        int OctToInt(const char* oct);
        size_t OffsetRecords(size_t bytes);
        wxFile file;
        size_t skipBytes;
        size_t fileSize;
        wxString error;
};

#endif // MYTAR_H
