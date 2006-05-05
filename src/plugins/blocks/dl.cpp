/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include <sdk.h>
#include "dl.h"

#include <wx/url.h>
#include <memory>


DownloadJob::StatusCode FileDownloadJob::operator()()
{
    wxURL url(urlStr);
    url.SetProxy(ConfigManager::GetProxy());

    if (url.GetError() != wxURL_NOERR)
        return ((url.GetError() == wxURL_CONNERR) || (url.GetError() == wxURL_PROTOERR )) ? DL_CANNOT_CONNECT : DL_BAD_URL;

    if(urlStr.AfterLast(_T('/')).IsEmpty()) // to save a file, we need a name
        return DL_BAD_URL;

    filename.assign(destDir + urlStr.AfterLast(_T('/')));

    std::auto_ptr<wxInputStream> stream(url.GetInputStream());

    if (stream.get() == 0 || stream->IsOk() == false)
        return DL_CANNOT_CONNECT;

    char buffer[8192];
    size_t chunk = 0;
    size = stream->GetSize();

    wxFileOutputStream out(filename);
    if(out.Ok() == false)
        return DL_FILE_ERROR;

    while(chunk = stream->Read(buffer, sizeof(buffer)).LastRead())
    {
        if(out.Write(buffer, chunk).LastWrite() != chunk)
            return DL_FILE_ERROR;
        downloaded += chunk;
    }
    return downloaded >= size ? DL_OK : DL_CONNECTION_ABORTED;
};





DownloadJob::StatusCode MemoryDownloadJob::operator()()
{
    wxURL url(urlStr);
    url.SetProxy(ConfigManager::GetProxy());

    if (url.GetError() != wxURL_NOERR)
        return ((url.GetError() == wxURL_CONNERR) || (url.GetError() == wxURL_PROTOERR )) ? DL_CANNOT_CONNECT : DL_BAD_URL;

    std::auto_ptr<wxInputStream> stream(url.GetInputStream());

    if (stream.get() == 0 || stream->IsOk() == false)
        return DL_CANNOT_CONNECT;

    char tmp[8192];
    size_t chunk = 0;

    while(chunk = stream->Read(tmp, sizeof(tmp)).LastRead())
        buffer.Append(tmp, chunk);

    return DL_OK;
};



