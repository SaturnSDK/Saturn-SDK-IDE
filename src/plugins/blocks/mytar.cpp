#include "mytar.h"

#include <sdk.h>
#include <wx/log.h>

TAR::TAR(const wxString& filename)
    :    skipBytes(0), fileSize(0)
{
    if(filename && file.Open(filename))
        fileSize = file.Length();
}


void TAR::Reset()
{
    if (file.IsOpened())
        file.Seek(0);
    skipBytes = 0;
}

int TAR::OctToInt(const char* oct)
{
	int i = 0;
	if (sscanf(oct, "%o", &i) != 1)
        i = 0;
    return i;
}

size_t TAR::OffsetRecords(size_t bytes)
{
    size_t i = bytes / sizeof(TAR::Header);
    if (bytes % sizeof(TAR::Header) > 0)
        ++i;
    return i;
}

bool TAR::Next(TAR::Record* rec)
{
    if (!rec)
        return false;
    rec->name.Clear();
    rec->size = 0;
    rec->pos = 0;
    if (skipBytes > 0)
        file.Seek(OffsetRecords(skipBytes) * sizeof(TAR::Header), wxFromCurrent);

    TAR::Header buffer;
    memset(&buffer, 0, sizeof(TAR::Header));

    // reached end of file?
    size_t pos = file.Tell();
    if (pos + sizeof(buffer) > fileSize)
        return false; // yes
    if (file.Read(&buffer, sizeof(buffer)) == wxInvalidOffset)
        return false;

    rec->pos = pos;
    rec->name = cbC2U(buffer.name);
    rec->size = OctToInt(buffer.size);


    if (buffer.name[0] == 0)
        return false;

    switch (buffer.typeflag)
    {
        case 0:
        case _T('0'): rec->ft = ftNormal; break;
        case _T('1'): rec->ft = ftLink; break;
        case _T('2'): rec->ft = ftSymbolicLink; break;
        case _T('3'): rec->ft = ftCharacter; break;
        case _T('4'): rec->ft = ftBlock; break;
        case _T('5'): rec->ft = ftDirectory; break;
        case _T('6'): rec->ft = ftFifo; break;
        case _T('7'): rec->ft = ftContiguous; break;
        case _T('D'): rec->ft = ftDumpDir; break;
        case _T('M'): rec->ft = ftMultiVolume; break;
        case _T('V'): rec->ft = ftVolumeHeader; break;
//        case _T('L'): rec.ft = ftLongName; break;
//        case _T('K'): rec.ft = ftLongLink; break;
        default: break;
    }

    switch (rec->ft)
    {
        case ftLink:
        case ftSymbolicLink:
        case ftDirectory:
        case ftFifo:
        case ftVolumeHeader:
            skipBytes = 0;
            break;
        default:
            skipBytes = rec->size;
            break;
    }
    return true;
}



bool TAR::ExtractFile(Record* rec, const wxString& destination)
{
    if (!rec)
        return false;

    wxLogNull ln;

    wxString path;
    if (rec->name.IsEmpty())
        return false;

    path = rec->name;

    if(rec->ft != ftNormal)
        return false;

    CreateDirRecursively(path);

    wxFile out(destination, wxFile::write);
    if (!out.IsOpened())
    {
        error = wxString(_("Cannot open file ")) << destination;
        return false;
    }

    if (rec->size > 0)
    {
        size_t oldpos = file.Tell();
        char* buffer = new char[rec->size];
        memset(buffer, 0, rec->size);
        if (file.Read(buffer, rec->size) == wxInvalidOffset)
        {
            file.Seek(oldpos);
            error = wxString(_("Failure reading file ")) << path << _T("\n");
            return false;
        }
        out.Write(buffer, rec->size);
        delete[] buffer;
        file.Seek(oldpos);
    }

    return true;
}

TAR::Record* TAR::FindFile(const wxString& filename, bool reset)
{
    if (filename.IsEmpty())
        return 0;

    if(reset)
        Reset();

    static TAR::Record r;

    while (Next(&r))
    {
        if (r.name.Matches(filename))
            return &r;
    }
    return 0;
}
