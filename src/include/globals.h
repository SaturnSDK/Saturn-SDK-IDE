/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef SDK_GLOBALS_H
#define SDK_GLOBALS_H

#include "globals_base.h"

#include <wx/treectrl.h>
#include <wx/intl.h>
#include <wx/msgdlg.h>
#include <wx/bitmap.h>

class TiXmlDocument;

extern DLLIMPORT void SaveTreeState(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths, wxArrayString& selectedItemPaths);
extern DLLIMPORT void RestoreTreeState(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths, wxArrayString& selectedItemPaths);

extern DLLIMPORT wxString ChooseDirectory(wxWindow* parent,
                                          const wxString& message = _("Select directory"),
                                          const wxString& initialPath = _T(""),
                                          const wxString& basePath = _T(""),
                                          bool askToMakeRelative = false, // relative to basePath
                                          bool showCreateDirButton = false); // where supported

/** This function creates a new wxMenu object on the heap and recursively
  * copies a given menu into it.
  *
  * It's up to the user to make sure the returned menu is deleted.
  *
  * @param mnu The menu to copy
  * @param with_accelerators If true, also copies the accelerators (defaults to false).
  * @return The copied menu or a nullptr, if the menu to copy was empty or NULL
  */
extern DLLIMPORT wxMenu* CopyMenu(wxMenu* mnu, bool with_accelerators = false);

/** This function loads a bitmap from disk.
  * Always use this to load bitmaps because it takes care of various
  * issues with pre-XP windows (actually common controls < 6.00).
  */
extern DLLIMPORT wxBitmap cbLoadBitmap(const wxString& filename, wxBitmapType bitmapType = wxBITMAP_TYPE_PNG);

// compatibility function
inline wxBitmap LoadPNGWindows2000Hack(const wxString& filename){ return cbLoadBitmap(filename); }

/** Finds out if a window is really shown.
  *
  * win->IsShown() is not that good because we don't know if the
  * window's container (parent) is actually shown or not...
  *
  * This is usually used to find out if docked windows are shown.
  *
  * @param win The window in question.
  * @return True if @c win is shown, false if not.
  */
extern DLLIMPORT bool IsWindowReallyShown(wxWindow* win);

/** Icons styles for settings dialogs.
  */
enum SettingsIconsStyle
{
    sisLargeIcons    = 0, ///< Large icons (default)
    sisNoIcons       = 1  ///< No icons, just text
};

class wxListCtrl;

/** Set the icons style for the supplied list control.
  * @param lc The wxListCtrl.
  * @param style The style to use.
  */
extern DLLIMPORT void SetSettingsIconsStyle(wxListCtrl* lc, SettingsIconsStyle style);
/** Get the icons style for the supplied list control.
  * @return The icons style.
  */
extern DLLIMPORT SettingsIconsStyle GetSettingsIconsStyle(wxListCtrl* lc);

class wxWindow;

enum cbPlaceDialogMode
{
    pdlDont = 0,
    pdlBest,
    pdlCentre,
    pdlHead,
    pdlConstrain,
    pdlClip
};

extern DLLIMPORT void PlaceWindow(wxTopLevelWindow *w, cbPlaceDialogMode mode = pdlBest, bool enforce = false);

#endif // SDK_GLOBALS_H
