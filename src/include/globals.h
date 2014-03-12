/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef SDK_GLOBALS_H
#define SDK_GLOBALS_H

#include <vector>

#include <wx/string.h>
#include <wx/treectrl.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/intl.h>
#include <wx/msgdlg.h>
#include <wx/bitmap.h>

#include "settings.h"

class TiXmlDocument;

/// Known plugin types
enum PluginType
{
    ptNone = 1,
    ptTool,
    ptMime,
    ptCompiler,
    ptDebugger,
    ptCodeCompletion,
    ptWizard,
    ptOther
};

/// The type of module offering a context menu.
enum ModuleType
{
    mtProjectManager = 1,
    mtEditorManager,
    mtLogManager,
    mtOpenFilesList,
    mtEditorTab,
    mtUnknown
};

/// Known file types
enum FileType
{
    ftCodeBlocksProject = 0,
    ftCodeBlocksWorkspace,
    ftDevCppProject,
    ftMSVC6Project,
    ftMSVC7Project,
    ftMSVC10Project,
    ftMSVC6Workspace,
    ftMSVC7Workspace,
    ftXcode1Project,
    ftXcode2Project,
    ftSource,
    ftHeader,
    ftObject,
    ftXRCResource,
    ftResource,
    ftResourceBin,
    ftStaticLib,
    ftDynamicLib,
    ftExecutable,
    ftNative,
    ftXMLDocument,
    ftScript,
    ftOther
};

/** These are valid values for the state of each project file.
    If a value is casted to integer, it corresponds to the tree's imagelist index for the state.
    NOTE: Keep in sync with icons loaded in ProjectManager::BuildTree()!
*/
enum FileVisualState
{
    // The following are related to (editable, source-) file states
    fvsNormal = 0,
    fvsMissing,
    fvsModified,
    fvsReadOnly,
    // The following are related to version control systems (vc)
    fvsVcAdded,
    fvsVcConflict,
    fvsVcMissing,
    fvsVcModified,
    fvsVcOutOfDate,
    fvsVcUpToDate,
    fvsVcRequiresLock,
    fvsVcExternal,
    fvsVcGotLock,
    fvsVcLockStolen,
    fvsVcMismatch,
    fvsVcNonControlled,
    // The following are related to C::B workspace/project/folder/virtual
    fvsWorkspace,
    fvsWorkspaceReadOnly,
    fvsProject,
    fvsProjectReadOnly,
    fvsFolder,
    fvsVirtualFolder,

    /// do *not* use this, it exists just to know the number of items...
    fvsLast
};

class DLLIMPORT cbProjectTreeImages
{
    public:
        static wxImageList* MakeImageList();

        /** @return The workspace icon index in the image list.
            @param  read_only Return the read-only icon for a workspace?
         */
        static int WorkspaceIconIndex(bool read_only = false);
        /** @return The project icon index in the image list.
            @param  read_only Return the read-only icon for a project?
         */
        static int ProjectIconIndex(bool read_only = false);
        /** @return The folder icon index in the image list. */
        static int FolderIconIndex();
        /** @return The virtual folder icon index in the image list. */
        static int VirtualFolderIconIndex();
};

/** These are valid values for the visual style of the project tree.
    They can be OR'ed to modify the representation of the project tree.
*/
enum ProjectTreeVisualState
{
    ptvsNone           = 0x00, //!< The default style: All "off"
    ptvsCategorize     = 0x01, //!< If true, use virtual folders like "Sources", "Headers", etc.
    ptvsUseFolders     = 0x02, //!< If true, create folders as needed. If false, the list is flat (not compatible with "hie folder name")
    ptvsHideFolderName = 0x04, //!< If true, the folder name will be hidden and only the file name will be shown (not compatible with "use folders")
    ptvsDefault        = 0x03  //!< Just here for convenience, "categorise" + "use folders" ON
};

/** Template output types. */
enum TemplateOutputType
{
    totProject = 0, ///< template outputs a new project
    totTarget, ///< template adds a new target in a project
    totFiles, ///< template outputs a new file (or files)
    totCustom, ///< template produces custom output (entirely up to the wizard used)
    totUser ///< template is a user-saved project template
};

/** Supported platforms */
enum SupportedPlatforms
{
    spMac       = 0x01,
    spUnix      = 0x02,
    spWindows   = 0x04,

    spAll       = 0xff
};
// NOTE: if you add/remove platforms, remember to update the relevant Get/Set globals

typedef wxString HighlightLanguage;
#define HL_AUTO _T(" ")
#define HL_NONE _T("  ")

extern DLLIMPORT const wxString DEFAULT_WORKSPACE;
extern DLLIMPORT const wxString DEFAULT_ARRAY_SEP;
extern DLLIMPORT const wxString DEFAULT_CONSOLE_TERM;
extern DLLIMPORT const wxString DEFAULT_CONSOLE_SHELL;

// global helper funcs
/// Reads a wxString from a non-unicode file. File must be open. File is closed automatically.
extern DLLIMPORT bool cbRead(wxFile& file, wxString& st, wxFontEncoding encoding = wxFONTENCODING_SYSTEM);
/// Reads a wxString from a non-unicode file. File must be open. File is closed automatically.
extern DLLIMPORT wxString cbReadFileContents(wxFile& file, wxFontEncoding encoding = wxFONTENCODING_SYSTEM);
/// Writes a wxString to a non-unicode file. File must be open. File is closed automatically.
extern DLLIMPORT bool cbWrite(wxFile& file, const wxString& buff, wxFontEncoding encoding = wxFONTENCODING_SYSTEM);
/// Writes a wxString to a file. Takes care of unicode and uses a temporary file
/// to save first and then it copies it over the original.
extern DLLIMPORT bool cbSaveToFile(const wxString& filename, const wxString& contents, wxFontEncoding encoding = wxFONTENCODING_SYSTEM, bool bom = false);
/// Saves a TinyXML document correctly, even if the path contains unicode characters.
extern DLLIMPORT bool cbSaveTinyXMLDocument(TiXmlDocument* doc, const wxString& filename);
/// Return @c str as a proper unicode-compatible string
extern DLLIMPORT wxString cbC2U(const char* str);
/// Return multibyte (C string) representation of the string
extern DLLIMPORT const wxWX2MBbuf cbU2C(const wxString& str);
/// Try converting a C-string from different encodings until a possible match is found.
/// This tries the following encoding converters (in the same order):
/// utf8, system, default and iso8859-1 to iso8859-15.
/// Returns the final encoding detected.
extern DLLIMPORT wxFontEncoding DetectEncodingAndConvert(const char* strIn, wxString& strOut, wxFontEncoding possibleEncoding = wxFONTENCODING_SYSTEM);

/// Return an integer representation of a platform string
extern DLLIMPORT int GetPlatformsFromString(const wxString& platforms);
/// Return a string representation of a platform / multiple platforms
extern DLLIMPORT wxString GetStringFromPlatforms(int platforms, bool forceSeparate = false);

// see globals.cpp for info on the third argument (bool SeparatorAtEnd)
extern DLLIMPORT wxString GetStringFromArray(const wxArrayString& array, const wxString& separator = DEFAULT_ARRAY_SEP, bool SeparatorAtEnd = true);
extern DLLIMPORT wxArrayString GetArrayFromString(const wxString& text, const wxString& separator = DEFAULT_ARRAY_SEP, bool trimSpaces = true);
typedef std::vector<wxString> wxStringVec;
extern DLLIMPORT wxStringVec GetVectorFromString(const wxString& text,
                                                 const wxString& separator = DEFAULT_ARRAY_SEP,
                                                 bool trimSpaces = true);
extern DLLIMPORT wxArrayString MakeUniqueArray(const wxArrayString& array, bool caseSens);
extern DLLIMPORT wxString MakeUniqueString(const wxString& text,  const wxString& separator = DEFAULT_ARRAY_SEP, bool caseSens = true);
extern DLLIMPORT void AppendArray(const wxArrayString& from, wxArrayString& to);

extern DLLIMPORT wxString UnixFilename(const wxString& filename, wxPathFormat format = wxPATH_NATIVE);
extern DLLIMPORT void QuoteStringIfNeeded(wxString& str);
extern DLLIMPORT bool NeedQuotes(const wxString &str);

/// Escapes spaces and tabs (NOT quoting the string)
extern DLLIMPORT wxString EscapeSpaces(const wxString& str);

extern DLLIMPORT FileType FileTypeOf(const wxString& filename);
extern DLLIMPORT wxString cbFindFileInPATH(const wxString &filename);

extern DLLIMPORT void SaveTreeState(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths, wxArrayString& selectedItemPaths);
extern DLLIMPORT void RestoreTreeState(wxTreeCtrl* tree, const wxTreeItemId& parent, wxArrayString& nodePaths, wxArrayString& selectedItemPaths);

extern DLLIMPORT bool CreateDirRecursively(const wxString& full_path, int perms = 0755);
extern DLLIMPORT bool CreateDir(const wxString& full_path, int perms = 0755);
extern DLLIMPORT wxString ChooseDirectory(wxWindow* parent,
                                          const wxString& message = _("Select directory"),
                                          const wxString& initialPath = _T(""),
                                          const wxString& basePath = _T(""),
                                          bool askToMakeRelative = false, // relative to basePath
                                          bool showCreateDirButton = false); // where supported

extern DLLIMPORT bool NormalizePath(wxFileName& f,const wxString& base);
extern DLLIMPORT bool IsSuffixOfPath(wxFileName const & suffix, wxFileName const & path);

/** Reads settings if eolMode is -1
  * Expected input (defined in sdk/wxscintilla/include/wx/wxscintilla.h) is:
  * wxSCI_EOL_CRLF=0, wxSCI_EOL_CR=1, or wxSCI_EOL_LF=2
  */
extern DLLIMPORT wxString GetEOLStr(int eolMode = -1);

extern DLLIMPORT wxString URLEncode(const wxString &str);

extern DLLIMPORT wxString ExpandBackticks(wxString &str);

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

/// Check if CommonControls version is at least 6 (XP and up)
extern DLLIMPORT bool UsesCommonControls6();

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
/** Get the icons style set in environment-dialog.
  * This version is left for backwardscompatibility.
  * @return The icons style.
  */
extern DLLIMPORT SettingsIconsStyle GetSettingsIconsStyle(wxListCtrl* lc);
/** Get the icons style set in environment-dialog.
  * @return The icons style.
  */
extern DLLIMPORT SettingsIconsStyle GetSettingsIconsStyle();

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

/** wxMessageBox wrapper.
  *
  * Use this instead of wxMessageBox(), as this uses PlaceWindow() to show it in the correct monitor.
  * @note The return value for wxOK would be wxID_OK and so on for other buttons.
  * So, read wxMessageDialog 's documentation, *not* wxMessageBox 's...
  */
extern DLLIMPORT int cbMessageBox(const wxString& message, const wxString& caption = wxEmptyString, int style = wxOK, wxWindow *parent = NULL, int x = -1, int y = -1);

inline void NotifyMissingFile(const wxString &name)
{
    wxString msg;
    msg.Printf(_T("The file %s could not be found.\nPlease check your installation."), name.c_str());
    cbMessageBox(msg);
}

/// Result values of cbDirAccessCheck()
enum DirAccessCheck
{
    dacInvalidDir, ///< Invalid directory (does not exist).
    dacReadWrite, ///< Current user has read-write access to the directory.
    dacReadOnly ///< Current user has read-only access to the directory.
};

/** Check what access the current user has in a directory.
  *
  * @param dir The directory in question.
  *            It may or may not contain an ending path separator.
  * @return The type of access the current user has in the directory.
  */
extern DLLIMPORT DirAccessCheck cbDirAccessCheck(const wxString& dir);

namespace platform
{
    typedef enum
    {
        winver_NotWindows = 0,
        winver_UnknownWindows,
        winver_Windows9598ME,
        winver_WindowsNT2000,
        winver_WindowsXP,
        winver_WindowsServer2003,
        winver_WindowsVista,
        winver_Windows7
    }windows_version_t;

    extern DLLIMPORT windows_version_t WindowsVersion();
}

// returns the real path of a file by resolving symlinks
// not yet optimal but should do for now
// one thing that's not checked yet are circular symlinks - watch out!
extern DLLIMPORT wxString realpath(const wxString& path);

#endif // SDK_GLOBALS_H
