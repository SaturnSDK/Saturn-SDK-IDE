/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/file.h>
    #include <wx/filename.h>
    #include <wx/filesys.h>
    #include <wx/image.h>
    #include <wx/listctrl.h>
    #include <wx/menu.h>

    #include "cbexception.h"
    #include "configmanager.h" // ReadBool
    #include "filemanager.h"
    #include "globals.h"
    #include "logmanager.h"
    #include "manager.h"
    #include "projectmanager.h"
#endif

#include "tinyxml/tinyxml.h"

#include <wx/dirdlg.h>
#include <wx/filefn.h>
#include <wx/fontmap.h>
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>

#include <algorithm>
#include <string>

#include "filefilters.h"
#include "tinyxml/tinywxuni.h"
#include "filegroupsandmasks.h"

#ifndef __WXMSW__
    #include <unistd.h> // readlink
    #include <sys/stat.h> // lstat
#endif

void DoRememberSelectedNodes(wxTreeCtrl* tree, wxArrayString& selectedItemPaths)
{
    wxArrayTreeItemIds items;

    if (tree->GetSelections(items) < 1 )
        return;

    for (size_t i=0; i < items.GetCount(); ++i)
    {
        wxString path = wxEmptyString;
        wxTreeItemId item = items[i];
        while(item.IsOk())
        {
            path = _T("/") + tree->GetItemText(item) + path;
            item = tree->GetItemParent(item);
        }
        if (path != wxEmptyString)
            selectedItemPaths.Add(path);
    }
}

void DoSelectRememberedNode(wxTreeCtrl* tree, const wxTreeItemId& parent, wxString& selectedItemPath)
{
    if (tree && !selectedItemPath.IsEmpty())
    {
        wxString tmpPath;
        wxString folder;
        tmpPath = selectedItemPath;
        int pos = tmpPath.Find(_T('/'));
        while (pos == 0)
        {
            tmpPath = tmpPath.Right(tmpPath.Length() - pos - 1);
            pos = tmpPath.Find(_T('/'));
        }

        folder = tmpPath.Left(pos);
        tmpPath = tmpPath.Right(tmpPath.Length() - pos - 1);
        wxTreeItemId item = parent;
        wxTreeItemIdValue cookie = 0;

        while (item.IsOk())
        {
            if (tree->GetItemText(item) != folder)
                item = tree->GetNextSibling(item);
            else
            {
                if (pos < 0)
                {
                    tree->SelectItem(item);
                    break;
                }
                else
                {
                    item = tree->GetNextChild(item, cookie);
                    DoSelectRememberedNode(tree, item, tmpPath);
                }
            }
        }

    }
}

bool DoRememberExpandedNodes(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths, wxString& path)
{
    // remember expanded tree nodes of this tree
    if (!tree || !parent.IsOk())
        return false;

    wxString originalPath = path;
    bool found = false;

    wxTreeItemIdValue cookie = 0;

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
        //Manager::Get()->GetLogManager()->Log(mltDevDebug, path);
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

        //Manager::Get()->GetLogManager()->Log(mltDevDebug, "%s, %s", folder.c_str(), tmpPath.c_str());

        wxTreeItemIdValue cookie = 0;

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

void SaveTreeState(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths, wxArrayString& selectedItemPaths)
{
    nodePaths.Clear();
    if (!parent.IsOk() || !tree || !tree->ItemHasChildren(parent) || !tree->IsExpanded(parent))
        return;

    wxString tmp;
    if (!DoRememberExpandedNodes(tree, parent, nodePaths, tmp))
        nodePaths.Add(tmp); // just the tree root

    selectedItemPaths.Clear();
    DoRememberSelectedNodes(tree, selectedItemPaths);
}

void RestoreTreeState(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths, wxArrayString& selectedItemPaths)
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
    for (unsigned int i = 0; i < selectedItemPaths.GetCount(); ++i)
        DoSelectRememberedNode(tree, tree->GetRootItem(), selectedItemPaths[i]);

    selectedItemPaths.Clear();
}

wxString ChooseDirectory(wxWindow* parent,
                         const wxString& message,
                         const wxString& initialPath,
                         const wxString& basePath,
                         bool askToMakeRelative, // relative to initialPath
                         bool showCreateDirButton) // where supported
{
    wxDirDialog dlg(parent, message, _T(""),
                    (showCreateDirButton ? wxDD_NEW_DIR_BUTTON : 0) | wxRESIZE_BORDER);
    dlg.SetPath(initialPath);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
        return wxEmptyString;

    wxFileName path(dlg.GetPath());
    if (askToMakeRelative && !basePath.IsEmpty())
    {
        // ask the user if he wants it to be kept as relative
        if (cbMessageBox(_("Keep this as a relative path?"),
                         _("Question"),
                         wxICON_QUESTION | wxYES_NO) == wxID_YES)
        {
            path.MakeRelativeTo(basePath);
        }
    }
    return path.GetFullPath();
}

wxBitmap cbLoadBitmap(const wxString& filename, wxBitmapType bitmapType)
{
    // cache this, can't change while we 're running :)
    static bool oldCommonControls = !UsesCommonControls6();

    wxImage im;
    wxFileSystem* fs = new wxFileSystem;
    wxFSFile* f = fs->OpenFile(filename);
    if (f)
    {
        wxInputStream* is = f->GetStream();
        im.LoadFile(*is, bitmapType);
        delete f;
    }
    delete fs;
    if (oldCommonControls && im.HasAlpha())
        im.ConvertAlphaToMask();

    return wxBitmap(im);
}

void SetSettingsIconsStyle(wxListCtrl* lc, SettingsIconsStyle style)
{
// this doesn't work under wxGTK...
#ifdef __WXMSW__
    long flags = lc->GetWindowStyleFlag();
    switch (style)
    {
        case sisNoIcons: flags = (flags & ~wxLC_ICON) | wxLC_SMALL_ICON; break;
        default: flags = (flags & ~wxLC_SMALL_ICON) | wxLC_ICON; break;
    }
    lc->SetWindowStyleFlag(flags);
#endif
}

SettingsIconsStyle GetSettingsIconsStyle(wxListCtrl* lc)
{
// this doesn't work under wxGTK...
#ifdef __WXMSW__
    long flags = lc->GetWindowStyleFlag();
    if (flags & wxLC_SMALL_ICON)
        return sisNoIcons;
#endif
    return sisLargeIcons;
}

#ifdef __WXMSW__

typedef APIENTRY HMONITOR (*MonitorFromWindow_t)(HWND, DWORD);
typedef APIENTRY BOOL (*GetMonitorInfo_t)(HMONITOR, LPMONITORINFO);

void PlaceWindow(wxTopLevelWindow *w, cbPlaceDialogMode mode, bool enforce)
{
    HMONITOR hMonitor;
    MONITORINFO mi;
    RECT        r;

    int the_mode;

    if (!w)
        cbThrow(_T("Passed NULL pointer to PlaceWindow."));

    ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("app"));
    if (!enforce && cfg->ReadBool(_T("/dialog_placement/do_place")) == false)
        return;

    wxWindow* referenceWindow = Manager::Get()->GetAppWindow();
    if (!referenceWindow)    // no application window available, so this is as good as we can get
        referenceWindow = w;

    wxRect windowRect = w->GetRect();

    if (mode == pdlBest)
        the_mode = cfg->ReadInt(_T("/dialog_placement/dialog_position"), (int) pdlCentre);
    else
        the_mode = (int) mode;


    static MonitorFromWindow_t MonitorFromWindowProc = (MonitorFromWindow_t) GetProcAddress(GetModuleHandle(_T("user32.dll")), "MonitorFromWindow");
    static GetMonitorInfo_t    GetMonitorInfoProc    = (GetMonitorInfo_t)    GetProcAddress(GetModuleHandle(_T("user32.dll")), "GetMonitorInfoA");
    int monitorWidth;
    int monitorHeight;

    if (GetMonitorInfoProc)
    {
        hMonitor = MonitorFromWindowProc((HWND) referenceWindow->GetHandle(), MONITOR_DEFAULTTONEAREST);

        mi.cbSize = sizeof(mi);
        GetMonitorInfoProc(hMonitor, &mi);
        r = mi.rcWork;

        monitorWidth  = r.right - r.left;
        monitorHeight = r.bottom - r. top;
    }
    else // Win95, NT4: support only single monitor
    {
        wxDisplaySize(&monitorWidth, &monitorHeight);
        r.left = r.top = 0;
    }


    switch(the_mode)
    {
        case pdlCentre:
        {
            windowRect.x = r.left + (monitorWidth  - windowRect.width)/2;
            windowRect.y = r.top  + (monitorHeight - windowRect.height)/2;
        }
        break;


        case pdlHead:
        {
            windowRect.x = r.left + (monitorWidth  - windowRect.width)/2;
            windowRect.y = r.top  + (monitorHeight - windowRect.height)/3;
        }
        break;


        case pdlConstrain:
        {
            int x1 = windowRect.x;
            int x2 = windowRect.x + windowRect.width;
            int y1 = windowRect.y;
            int y2 = windowRect.y + windowRect.height;

            if (windowRect.width > monitorWidth) // cannot place without clipping, so centre it
            {
                x1 = r.left + (monitorWidth  - windowRect.width)/2;
                x2 = x1 + windowRect.width;
            }
            else
            {
                x2 = std::min((int) r.right, windowRect.GetRight());
                x1 = std::max(x2 - windowRect.width, (int) r.left);
                x2 = x1 + windowRect.width;
            }
            if (windowRect.height > monitorHeight) // cannot place without clipping, so centre it
            {
                y1 = r.top + (monitorHeight  - windowRect.height)/2;
                y2 = y1 + windowRect.height;
            }
            else
            {
                y2 = std::min((int) r.bottom, windowRect.GetBottom());
                y1 = std::max(y2 - windowRect.height, (int) r.top);
                y2 = y1 + windowRect.height;
            }
            windowRect = wxRect(x1, y1, x2-x1, y2-y1);
        }
        break;


        case pdlClip:
        {
            int x1 = windowRect.x;
            int x2 = windowRect.x + windowRect.width;
            int y1 = windowRect.y;
            int y2 = windowRect.y + windowRect.height;

            x1 = std::max(x1, (int) r.left);
            x2 = std::min(x2, (int) r.right);
            y1 = std::max(y1, (int) r.top);
            y2 = std::min(y2, (int) r.bottom);

            windowRect = wxRect(x1, y1, x2-x1, y2-y1);
        }
        break;
    }

    w->SetSize(windowRect.x,  windowRect.y, windowRect.width, windowRect.height, wxSIZE_ALLOW_MINUS_ONE);
}


#else // ----- non-Windows ----------------------------------------------


void PlaceWindow(wxTopLevelWindow *w, cbPlaceDialogMode mode, bool enforce)
// TODO (thomas#1#): The non-Windows implementation is *pathetic*.
// However, I don't know how to do it well under GTK / X / Xinerama / whatever.
// Anyone?
{
    int the_mode;

    wxWindow* referenceWindow = Manager::Get()->GetAppWindow();
    if (!referenceWindow) // let's not crash on shutdown
        return;

    if (!w)
        cbThrow(_T("Passed NULL pointer to PlaceWindow."));


    ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("app"));
    if (!enforce && cfg->ReadBool(_T("/dialog_placement/do_place")) == false)
        return;

    if (mode == pdlBest)
        the_mode = cfg->ReadInt(_T("/dialog_placement/dialog_position"), (int) pdlCentre);
    else
        the_mode = (int) mode;


    if (the_mode == pdlCentre || the_mode == pdlHead)
    {
        w->CentreOnScreen();
        return;
    }
    else
    {
        wxRect windowRect = w->GetRect();
        wxRect parentRect = referenceWindow->GetRect();   // poo again!

        int x1 = windowRect.x;
        int x2 = windowRect.x + windowRect.width;
        int y1 = windowRect.y;
        int y2 = windowRect.y + windowRect.height;

        x1 = std::max(x1, parentRect.x);
        x2 = std::min(x2, parentRect.GetRight());
        y1 = std::max(y1, parentRect.y);
        y2 = std::min(y2, parentRect.GetBottom());

        w->SetSize(x1, y1, x2-x1, y2-y1, wxSIZE_ALLOW_MINUS_ONE);
    }
}

#endif //platform-specific placement code

int cbMessageBox(const wxString& message, const wxString& caption, int style, wxWindow *parent, int x, int y)
{
    if (!parent)
        parent = Manager::Get()->GetAppWindow();

    // Cannot create a wxMessageDialog with a NULL as parent
    if (!parent)
    {
      // wxMessage*Box* returns any of: wxYES, wxNO, wxCANCEL, wxOK.
      int answer = wxMessageBox(message, caption, style, parent, x, y);
      switch (answer)
      {
        // map answer to the one of wxMessage*Dialog* to ensure compatibility
        case (wxOK):
          return wxID_OK;
        case (wxCANCEL):
          return wxID_CANCEL;
        case (wxYES):
          return wxID_YES;
        case (wxNO):
          return wxID_NO;
        default:
          return -1; // NOTE: Cannot happen unless wxWidgets API changes
      }
    }

    wxMessageDialog dlg(parent, message, caption, style, wxPoint(x,y));
    PlaceWindow(&dlg);
    // wxMessage*Dialog* returns any of wxID_OK, wxID_CANCEL, wxID_YES, wxID_NO
    return dlg.ShowModal();
}
