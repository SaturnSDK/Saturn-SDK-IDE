/***************************************************************
 * Name:      mimetypesarray.h
 * Purpose:   Contains the definition of the MIME types array
 * Author:    Yiannis An. Mandravellos<mandrav@codeblocks.org>
 * Created:   03/13/05 14:08:51
 * Copyright: (c) Yiannis An. Mandravellos
 * License:   GPL
 **************************************************************/

#ifndef MIMETYPESARRAY_H
#define MIMETYPESARRAY_H

// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/dynarray.h>

struct cbMimeType
{
    // e.g. "*.xrc" - case insensitive
    wxString wildcard;

    // e.g. "C:\Tools\XRCed.exe $(FILE)" - program to open this file
    // $(FILE) is substituted by the file in question
    // used only if useEditor == false
    wxString program;

    // if true, open it in the Code::Blocks editor, else use "program"
    bool useEditor;

    // should the IDE be disabled while working on this file?
    // valid only for external programs...
    bool programIsModal;
};
WX_DEFINE_ARRAY(cbMimeType*, MimeTypesArray);

#endif // MIMETYPESARRAY_H
