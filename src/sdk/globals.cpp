/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Id$
* $Date$
*/

#include "sdk_precomp.h"
#include "globals.h"
#include <wx/tokenzr.h>
#include <wx/filename.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>
#include <wx/file.h>

wxString GetStringFromArray(const wxArrayString& array, const wxString& separator)
{
    wxString out;
    for (unsigned int i = 0; i < array.GetCount(); ++i)
        out << array[i] << separator;
    return out;
}

wxArrayString GetArrayFromString(const wxString& text, const wxString& separator, bool trimSpaces)
{
    wxArrayString out;
    wxString search = text;
    int seplen = separator.Length();
    while (true)
    {
        int idx = search.Find(separator);
        if (idx == -1)
        {
            if (trimSpaces)
            {
                search.Trim(false);
                search.Trim(true);
            }
            if (!search.IsEmpty())
                out.Add(search);
            break;
        }
        wxString part = search.Left(idx);
        search.Remove(0, idx + seplen);
        if (trimSpaces)
        {
            part.Trim(false);
            part.Trim(true);
        }
        if (!part.IsEmpty())
            out.Add(part);
    }
    return out;
}

void AppendArray(const wxArrayString& from, wxArrayString& to)
{
    for (unsigned int i = 0; i < from.GetCount(); ++i)
    {
        to.Add(from[i]);
    }
}

wxString UnixFilename(const wxString& filename)
{
    wxString result = filename;
#ifdef __WXMSW__
    while (result.Replace(_T("/"), _T("\\")))
        ;
    while (result.Replace(_T("\\\\"), _T("\\")))
        ;
#else
    while (result.Replace(_T("\\"), _T("/")))
        ;
    while (result.Replace(_T("//"), _T("/")))
        ;
#endif
    return result;
}

void QuoteStringIfNeeded(wxString& str)
{
    if (!str.IsEmpty() && str.GetChar(0) != _T('"') && str.Find(_T(' ')) != -1)
        str = wxString(_T("\"")) + str + _T("\"");
}

FileType FileTypeOf(const wxString& filename)
{
    wxString ext = filename.AfterLast(_T('.')).Lower();

	if (ext.Matches(CPP_EXT) ||
		ext.Matches(C_EXT) ||
		ext.Matches(CC_EXT) ||
		ext.Matches(CXX_EXT)
		)
		return ftSource;

	else if (ext.Matches(HPP_EXT) ||
		ext.Matches(H_EXT) ||
		ext.Matches(HH_EXT) ||
		ext.Matches(HXX_EXT)
		)
		return ftHeader;

	else if (ext.Matches(CODEBLOCKS_EXT))
		return ftCodeBlocksProject;

	else if (ext.Matches(WORKSPACE_EXT))
		return ftCodeBlocksWorkspace;

	else if (ext.Matches(DEVCPP_EXT))
		return ftDevCppProject;

	else if (ext.Matches(MSVC_EXT))
		return ftMSVCProject;

	else if (ext.Matches(MSVS_EXT))
		return ftMSVSProject;

	else if (ext.Matches(MSVC_WORKSPACE_EXT))
		return ftMSVCWorkspace;

	else if (ext.Matches(MSVS_WORKSPACE_EXT))
		return ftMSVSWorkspace;

	else if (ext.Matches(OBJECT_EXT))
		return ftObject;

	else if (ext.Matches(XRCRESOURCE_EXT))
		return ftXRCResource;

	else if (ext.Matches(RESOURCE_EXT))
		return ftResource;

	else if (ext.Matches(RESOURCEBIN_EXT))
		return ftResourceBin;

	else if (ext.Matches(STATICLIB_EXT))
		return ftStaticLib;

	else if (ext.Matches(DYNAMICLIB_EXT))
		return ftDynamicLib;

	else if (ext.Matches(EXECUTABLE_EXT))
		return ftExecutable;

	return ftOther;
}

bool DoRememberExpandedNodes(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths, wxString& path)
{
    // remember expanded tree nodes of this tree
    if (!tree || !parent.IsOk())
        return false;

    wxString originalPath = path;
    bool found = false;
#if (wxMAJOR_VERSION == 2) && (wxMINOR_VERSION < 5)
    long int cookie = 0;
#else
    wxTreeItemIdValue cookie; //2.6.0
#endif
    wxTreeItemId child = tree->GetFirstChild(parent, cookie);
    while (child.IsOk())
    {
        if (tree->ItemHasChildren(child) && tree->IsExpanded(child))
        {
            found = true;
            path << _T("/") << tree->GetItemText(child);
            DoRememberExpandedNodes(tree, child, nodePaths, path);
            nodePaths.Add(path);
            path = originalPath;
        }
        child = tree->GetNextChild(parent, cookie);
    }
    return found;
}

void DoExpandRememberedNode(wxTreeCtrl* tree, const wxTreeItemId& parent, const wxString& path)
{
    if (!path.IsEmpty())
    {
        //Manager::Get()->GetMessageManager()->Log(mltDevDebug, path);
        wxString tmpPath;
        tmpPath = path;
        wxString folder;
        int pos = tmpPath.Find(_T('/'));
        while (pos == 0)
        {
            tmpPath = tmpPath.Right(tmpPath.Length() - pos - 1);
            pos = tmpPath.Find(_T('/'));
        }

        if (pos < 0) // no '/'
        {
            folder = tmpPath;
            tmpPath.Clear();
        }
        else
        {
            folder = tmpPath.Left(pos);
            tmpPath = tmpPath.Right(tmpPath.Length() - pos - 1);
        }

        //Manager::Get()->GetMessageManager()->Log(mltDevDebug, "%s, %s", folder.c_str(), tmpPath.c_str());

#if (wxMAJOR_VERSION == 2) && (wxMINOR_VERSION < 5)
	    long int cookie = 0;
#else
	    wxTreeItemIdValue cookie; //2.6.0
#endif
        wxTreeItemId child = tree->GetFirstChild(parent, cookie);
        while (child.IsOk())
        {
            wxString itemText = tree->GetItemText(child);
            if (itemText.Matches(folder))
            {
                tree->Expand(child);
                DoExpandRememberedNode(tree, child, tmpPath);
                break;
            }
            child = tree->GetNextChild(parent, cookie);
        }
    }
}

void SaveTreeState(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths)
{
    nodePaths.Clear();
    if (!parent.IsOk() || !tree || !tree->ItemHasChildren(parent) || !tree->IsExpanded(parent))
        return;
    wxString tmp;
    if (!DoRememberExpandedNodes(tree, parent, nodePaths, tmp))
        nodePaths.Add(tmp); // just the tree root
}

void RestoreTreeState(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths)
{
    if (!parent.IsOk() || !tree)
        return;
    if (nodePaths.GetCount() == 0)
    {
        tree->Collapse(parent);
        return;
    }
    for (unsigned int i = 0; i < nodePaths.GetCount(); ++i)
        DoExpandRememberedNode(tree, parent, nodePaths[i]);
    nodePaths.Clear();
}

bool CreateDirRecursively(const wxString& full_path, int perms)
{
    wxFileName tmp(full_path);
    wxString sep = wxFileName::GetPathSeparator();
    wxString currdir = tmp.GetVolume() + tmp.GetVolumeSeparator() + sep;
    wxArrayString dirs = tmp.GetDirs();
    for (size_t i = 0; i < dirs.GetCount(); ++i)
    {
        currdir << dirs[i];
        if (!wxDirExists(currdir) && !wxMkdir(currdir, perms))
            return false;
        currdir << sep;
    }
    return true;
}

wxString ChooseDirectory(wxWindow* parent,
                         const wxString& message,
                         const wxString& initialPath,
                         const wxString& basePath,
                         bool askToMakeRelative, // relative to initialPath
                         bool showCreateDirButton) // where supported
{
    wxDirDialog dlg(parent,
                    message,
                    _T(""),
                    (showCreateDirButton ? wxDD_NEW_DIR_BUTTON : 0) | wxRESIZE_BORDER);
    dlg.SetPath(initialPath);
    if (dlg.ShowModal() != wxID_OK)
        return wxEmptyString;

    wxFileName path(dlg.GetPath());
    if (askToMakeRelative && !basePath.IsEmpty())
    {
        // ask the user if he wants it to be kept as relative
        if (wxMessageBox(_("Keep this as a relative path?"),
                        _("Question"),
                        wxICON_QUESTION | wxYES_NO) == wxYES)
        {
            path.MakeRelativeTo(basePath);
        }
    }
    return path.GetFullPath();
}

/// Reads a wxString from a non-unicode file. File must be open. File is closed automatically.
bool cbRead(wxFile& file, wxString& st)
{
    st.Empty();
    if (!file.IsOpened())
        return false;
    int len = file.Length();
    if(!len)
    {
        file.Close();
        return true;
    }
#if wxUSE_UNICODE
    char* buff = new char[len+1];
    if (!buff)
    {
        file.Close();
        return false;
    }
    file.Read((void*)buff, len);
    file.Close();
    buff[len]='\0';
    st = wxString((const char *)buff, wxConvUTF8);
    delete[] buff;
#else
    char* buff = st.GetWriteBuf(len); // GetWriteBuf already handles the extra '\0'.
    file.Read((void*)buff, len);
    file.Close();
    st.UngetWriteBuf();
#endif
    return true;
}

wxString cbReadFileContents(wxFile& file)
{
    wxString st;
    cbRead(file,st);
    return st;
}

/// Writes a wxString to a non-unicode file. File must be open. File is closed automatically.
bool cbWrite(wxFile& file, const wxString& buff)
{
    bool result = false;
    if (file.IsOpened())
    {
        result = file.Write(buff,wxConvUTF8);
        if(result)
            file.Flush();
        file.Close();
    }
    return result;
}

/// Writes a wxString to a file. Takes care of unicode and uses a temporary file
/// to save first and then it copies it over the original.
bool cbSaveToFile(const wxString& filename, const wxString& contents)
{
    wxTempFile file(filename);
    if (file.IsOpened())
    {
        if (!file.Write(contents, wxConvUTF8))
            return false;
        if (!file.Commit())
            return false;
    }
    else
        return false;
    return true;
}

// Base64 encode/decode functions stolen from wxCode / wxhttpengine,
// removed Turadg Aleahmad's POST modifications.
// Origin:
//   http://wxcode.sourceforge.net/components/wxhttpengine/
//   Author: Angelo Mandato (http://www.spaceblue.com)

const char base64_table[] =
	{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
	};

const char base64_pad = '=';

const short base64_reverse_table[256] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

wxString wxBase64Encode(const wxString &str )
{
  wxString szToReturn;
  int length = str.Length();
  int current = 0;

  while (length > 2) // keep going until we have less than 24 bits (each item is 8 bits)
  {
    szToReturn.Append( base64_table[str.GetChar(current) >> 2] );
    szToReturn.Append( base64_table[((str.GetChar(current) & 0x03) << 4) + (str.GetChar(current+1) >> 4)] );
    szToReturn.Append( base64_table[((str.GetChar(current+1) & 0x0f) << 2) + (str.GetChar(current+2) >> 6)] );
    szToReturn.Append( base64_table[str.GetChar(current+2) & 0x3f] );

		current += 3;
		length -= 3; // we just handle 3 octets of data
	}

	if (length != 0)
  {
    szToReturn.Append( base64_table[str.GetChar(current) >> 2] );

		//*p++ = base64_table[current[0] >> 2];
		if (length > 1)
    {
      szToReturn.Append( base64_table[((str.GetChar(current) & 0x03) << 4) + (str.GetChar(current+1) >> 4)] );
      szToReturn.Append( base64_table[(str.GetChar(current+1) & 0x0f) << 2] );
      szToReturn.Append( base64_pad );
		}
    else
    {
      szToReturn.Append( base64_table[(str.GetChar(current) & 0x03) << 4] );
      szToReturn.Append( base64_pad );
      szToReturn.Append( base64_pad );
		}
	}

  return szToReturn;
}

wxString wxBase64Decode(const wxString &str)
{
  wxString szToReturn;

  int length = str.Length();
  unsigned int current = 0;
  int i = 0, j = 0, k;
  wxChar ch = ' ';

  while( current != str.Length() && length-- > 0 )
  {
    ch = str.GetChar(current++);

    if (ch == base64_pad) break;

		ch = base64_reverse_table[ (int)ch]; //CHECK
		if (ch < 0) continue;

		switch(i % 4)
    {
		  case 0:

			  szToReturn.Append( ch << 2 );
			  break;
		  case 1:
        szToReturn.SetChar(j, szToReturn.GetChar(j) | ch >> 4 );
        j++;
        szToReturn.Append( (ch & 0x0f) << 4 );
			  break;
		  case 2:
        szToReturn.SetChar(j, szToReturn.GetChar(j) | ch >> 2 );
        j++;
        szToReturn.Append( (ch & 0x03) << 6 );
			  break;
		  case 3:
        szToReturn.SetChar(j, szToReturn.GetChar(j) | ch );
        j++;
			  break;
		}
		i++;
  }

  k = j;
	// mop things up if we ended on a boundary
	if (ch == base64_pad)
  {
		switch(i % 4)
    {
		  case 0:
		  case 1:
			  return wxEmptyString;
		}
	}

  return szToReturn;
}
