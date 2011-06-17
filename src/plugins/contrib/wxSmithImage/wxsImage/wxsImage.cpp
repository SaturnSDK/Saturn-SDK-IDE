/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
*/


#include "wxsImage.h"

using namespace std;

//------------------------------------------------------------------------------

namespace
{

// Loading images from xpm files

    #include "Image16.xpm"
    #include "Image32.xpm"

    wxsRegisterItem<wxsImage> Reg(
        _T("wxImage"),                  // Class name
        wxsTTool,                       // Item type
        _T("wxWindows"),                // License
        _T("Ron Collins"),              // Author
        _T("rcoll@theriver.com"),       // Author's email
        _T(""),                         // Item's homepage
        _T("Image Tools"),              // Category in palette
        90,                             // Priority in palette
        _T("Image"),                    // Base part of names for new items
        wxsCPP,                         // List of coding languages supported by this item
        1, 0,                           // Version
        wxBitmap(Image32_xpm),          // 32x32 bitmap
        wxBitmap(Image16_xpm),          // 16x16 bitmap
        false);                         // We do not allow this item inside XRC files

/*
    WXS_ST_BEGIN(wxsCompResourceStyles,_T(""))
        WXS_ST_CATEGORY("wxCompResource")
        WXS_ST_DEFAULTS()
    WXS_ST_END()
*/
}

//------------------------------------------------------------------------------
// a single global config object to prevent conflicts

//wxIniFile   *gAlignConfig = NULL;


//------------------------------------------------------------------------------

wxsImage::wxsImage(wxsItemResData* Data):
    wxsTool(Data, &Reg.Info, 0, 0)
{
int         i,n;
wxString    ss, tt;
wxFileName  fn;

    cout << "-- wxsImage::wxsImage" << endl;

// nothing selected yet

    mIsBuilt = false;
    mImageData.Clear();
    mInclude = false;

// make the absolute directory path where we store XPM image files
// this directory is always a sub-dir where the source code file is stored

    fn = Data->GetSrcFileName();
    ss = fn.GetPath((wxPATH_GET_VOLUME+wxPATH_GET_SEPARATOR));
    n  = ss.Len();
    ss = ss + _("wximages") + ss[n-1];

    mIDir = ss;
    mRDir = _("./wximages/");
    mBase = fn.GetName();
}

//------------------------------------------------------------------------------

void wxsImage::OnBuildCreatingCode() {
int         i,n;
wxString    inc;
wxString    vname;                  // this variable name
wxString    bname;                  // name of the bitmap variable
wxString    xname;                  // XPM data block name
wxString    ss, tt;                 // general use

// already been here?

    if (mIsBuilt) return;
    mIsBuilt = true;

// we only handle C++ constructs here

    if (GetLanguage() != wxsCPP) wxsCodeMarks::Unknown(_T("wxsImage"),GetLanguage());

// important names

cout << "-- wxsImage::OnBuildCreatingCode" << endl;

    vname = GetVarName();
    bname = vname + _("_BMP");
    xname = vname + _("_XPM");

// basic include files

    AddHeader(_("<wx/image.h>"), GetInfo().ClassName, 0);
    AddHeader(_("<wx/bitmap.h>"), GetInfo().ClassName, 0);

// store the XPM data someplace

    StoreXpmData();

// if there is no data, then just make empty image and bitmap

    if (mImageData.Count() == 0) {
        Codef(_T("%s = new wxImage();\n"), vname.c_str());
        Codef(_T("%s = new wxBitmap();\n"), bname.c_str());
    }

// else fill it with XPM data

    else {
        Codef(_T("%s = new wxImage(%s);\n"),  vname.c_str(), xname.c_str());
        Codef(_T("%s = new wxBitmap(%s);\n"), bname.c_str(), xname.c_str());
    };
}


//------------------------------------------------------------------------------

void wxsImage::OnEnumToolProperties(long Flags) {

cout << "-- wxsImage::OnEnumWidgetProperties" << endl;

// starting a new build cycle

    mIsBuilt = false;
    mContext = GetCoderContext();

// details

    WXS_IMAGE(wxsImage, mImageData, _("Image"), _("mImageData"));
    WXS_ARRAYSTRING(wxsImage, mImageData, _("Image as Text"), _("mImageDataText"), _("item2"));

    WXS_BOOL(wxsImage, mInclude, _("Use Include File"), _("mInclude"), false);
};

//------------------------------------------------------------------------------
// declare the var with a bitmap

void wxsImage::OnBuildDeclarationsCode() {
int         i,n;
wxString    vname;
wxString    bname;

cout << "-- wxsImage::OnBuildDeclarationsCode" << endl;


    vname = GetVarName();
    bname = vname + _("_BMP");

// put in all the vars, although some might be ignored later

    AddDeclaration(_T("wxImage               *") + vname + _T(";"));
    AddDeclaration(_T("wxBitmap              *") + bname + _T(";"));
}

//------------------------------------------------------------------------------
// save the XPM data block; may be stored as a #include file

void wxsImage::StoreXpmData(void) {
int         i,n;
wxString    vname;
wxString    xname;
wxString    ss, tt;
wxFile      ff;

// nothing to store?

    if (mImageData.Count() == 0) return;

// important names

    vname = GetVarName();
    xname = vname + _("_XPM");

// make a single string with the proper name


    tt = _("");
    n = mImageData.GetCount();
    if (n > 5) {
        n = (n * mImageData.Item(n-2).Length()) + 100;
        tt.Alloc(n);
    };

    for(i=0; i<mImageData.GetCount(); i++) {
        ss = mImageData.Item(i);
        if (ss.Find(_("xpm_data")) >= 0) ss.Replace(_("xpm_data"), xname);

        tt += ss;
        tt += _("\n");
    };

// store as an include file

    if (mInclude) {
        if (! wxFileName::DirExists(mIDir)) wxFileName::Mkdir(mIDir);
        ss  = mIDir;
        ss += mBase;
        ss += _("_");
        ss += xname;
        ss += _(".xpm");
        ff.Open(ss.fn_str(), wxFile::write);
        ff.Write(tt);
        ff.Close();

        ss = _("\"");
        ss += mRDir;
        ss += mBase;
        ss += _("_");
        ss += xname;
        ss += _(".xpm");
        ss += _("\"");

        AddHeader(ss, GetInfo().ClassName, 0);
    }

// store in-line in the main header file

    else {
//       AddDeclaration(tt);
        Codef(tt);
    };
}

//------------------------------------------------------------------------------

wxBitmap wxsImage::GetPreview(void) {
wxBitmap    bmp;

    if (mImageData.GetCount() == 0) return wxNullBitmap;

    wxsImageListDialog::ArrayToBitmap(mImageData, bmp);
    return bmp;
}

//------------------------------------------------------------------------------
// this just exposes the "OnBuildCreatingCode()" method

void wxsImage::DoBuild(void) {
   BuildCode(mContext);
}








