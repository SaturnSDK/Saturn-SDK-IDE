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

#include "wxsImageList.h"

using namespace std;

//------------------------------------------------------------------------------

namespace
{

// Loading images from xpm files

    #include "ImageList16.xpm"
    #include "ImageList32.xpm"

    wxsRegisterItem<wxsImageList> Reg(
        _T("wxImageList"),              // Class name
        wxsTTool,                       // Item type
        _T("wxWindows"),                // License
        _T("Ron Collins"),              // Author
        _T("rcoll@theriver.com"),       // Author's email
        _T(""),                         // Item's homepage
        _T("Image Tools"),              // Category in palette
        80,                             // Priority in palette
        _T("ImageList"),                // Base part of names for new items
        wxsCPP,                         // List of coding languages supported by this item
        1, 0,                           // Version
        wxBitmap(ImageList32_xpm),      // 32x32 bitmap
        wxBitmap(ImageList16_xpm),      // 16x16 bitmap
        false);                         // We do not allow this item inside XRC files

/*
    WXS_ST_BEGIN(wxsCompResourceStyles,_T(""))
        WXS_ST_CATEGORY("wxCompResource")
        WXS_ST_DEFAULTS()
    WXS_ST_END()
*/
}

//------------------------------------------------------------------------------

wxsImageList::wxsImageList(wxsItemResData* Data):
    wxsTool(Data, &Reg.Info, 0, 0)
{
int         i,n;
wxString    ss, tt;
wxFileName  fn;

//cout << "-- wxsImageList::wxsImageList" << endl;

// nothing selected yet

    mIsBuilt = false;
    mImageData.Clear();
    mWidth   = 16;
    mHeight  = 16;
    mCount   = 0;
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

void wxsImageList::OnBuildCreatingCode() {
int         i,n;
wxString    inc;
wxString    vname;                  // this variable name
wxString    bname;                  // name of the bitmap variable
wxString    fbase;                  // base name of XPM file without dirs or extension
wxString    fabs;                   // absolute name of XPM file
wxString    frel;                   // relative
wxString    dname;                  // name of XPM data array
bool        fexist;                 // does the file exist?
bool        bdiff;                  // different image requested than last time?
wxBitmap    bmp;                    // preview bitmap saved as XPM
wxString    ss, tt;                 // general use

// have we already been here?

    if (mIsBuilt) return;
    mIsBuilt = true;

// we only handle C++ constructs here

    if (GetLanguage() != wxsCPP) wxsCodeMarks::Unknown(_T("wxsImageList"),GetLanguage());

// important names

cout << "-- wxsImageList::OnBuildCreatingCode" << endl;

    vname = GetVarName();

// basic include files

    AddHeader(_("<wx/imaglist.h>"), GetInfo().ClassName, 0);

// store the XPM data someplace

    StoreXpmData();

//cout << "   mCount=" << mCount << endl;

// if there is no data, then just make empty image and bitmap

    if (mCount == 0) {
        Codef(_T("%s = new wxImageList(%d, %d, 1);\n"), vname.c_str(), mWidth, mHeight);
    }

// else fill it with XPM data

    else {
        Codef(_T("%s = new wxImageList(%d, %d, %d);\n"),  vname.c_str(), mWidth, mHeight, (mCount+1));
        for(i=0; i<mCount; i++) {
            ss.Printf(_("%s_%d_XPM"), vname.c_str(), i);
            Codef(_T("%s->Add(wxBitmap(%s));\n"), vname.c_str(), ss.c_str());
        };
    };
}



//------------------------------------------------------------------------------

void wxsImageList::OnEnumToolProperties(long Flags) {

cout << "-- wxsImageList::OnEnumToolProperties" << endl;

// starting a new build cycle

    mIsBuilt = false;
    mContext = GetCoderContext();


// details for the image list

    WXS_IMAGELIST(wxsImageList, mImageData, _T("Image List"), _T("mImageInfo"));
    WXS_ARRAYSTRING(wxsImageList, mImageData, _("Images as Text"), _("mImageDataText"), _("item2"));

    WXS_LONG(wxsImageList, mWidth,   _("Image Width"),      _("mWidth"),   16);
    WXS_LONG(wxsImageList, mHeight,  _("Image Height"),     _("mHeight"),  16);
    WXS_LONG(wxsImageList, mCount,   _("Image Count"),      _("mCount"),   0);
    WXS_BOOL(wxsImageList, mInclude, _("Use Include File"), _("mInclude"), false);


};

//------------------------------------------------------------------------------

void wxsImageList::StoreXpmData(void) {
int         i,n;
long        ll;
wxString    vname;
wxString    xname;
wxString    ss, tt, vv;
wxFile      ff;


// important names

    vname = GetVarName();

// if no XPM images, then just store the image size

    n = mImageData.GetCount();
    if (n <= 2) {
        if (mWidth <= 0) mWidth = 16;
        if (mHeight <= 0) mHeight = 16;
        mCount = 0;

        mImageData.Clear();
        ss.Printf(_("%d"), mWidth);
        mImageData.Add(ss);

        ss.Printf(_("%d"), mHeight);
        mImageData.Add(ss);
    }

// else reset the displayed size

    else {
        ss = mImageData.Item(0);
        if (ss.ToLong(&ll)) mWidth = ll;

        ss = mImageData.Item(1);
        if (ss.ToLong(&ll)) mHeight = ll;

        mCount = 0;
        for(i=0; i<mImageData.GetCount(); i++) {
             ss = mImageData.Item(i);
            if (ss.Find(_("xpm_data")) >= 0) mCount += 1;
        };
    };

// are we finished?

    if (mCount == 0) return;

// need to create a #include file?

    if (mInclude) {
        if (! wxFileName::DirExists(mIDir)) wxFileName::Mkdir(mIDir);
        ss  = mIDir;
        ss += mBase;
        ss += _("_");
        ss += vname;
        ss += _(".xpm");
        ff.Open(ss.fn_str(), wxFile::write);
    };

// go thru entire array, pulling out one XPM at a time into a single string

    n = 0;
    i = 2;
    tt = _("");
    while (i < mImageData.GetCount()) {
        ss = mImageData.Item(i);
        i += 1;

// the beginning of a new XPM image means the end of the previous image

        if (ss.Find(_("xpm_data")) >= 0) {
            vv.Printf(_("%s_%d_XPM"), vname.c_str(), n);
            ss.Replace(_("xpm_data"), vv);
            n += 1;

            if (tt.Length() > 0) {
                if (mInclude) ff.Write(tt);
                else          Codef(tt);
            };

            tt  = ss;
            tt += _("\n");
        }

// else just another data line

        else {
            tt += ss;
            tt += _("\n");
        };
    };

// the left-overs

    if (tt.Length() > 0) {
        if (mInclude) ff.Write(tt);
        else          Codef(tt);
    };

// include the #include file

    if (mInclude) {
        ff.Close();

        ss = _("\"");
        ss += mRDir;
        ss += mBase;
        ss += _("_");
        ss += vname;
        ss += _(".xpm");
        ss += _("\"");

        AddHeader(ss, GetInfo().ClassName, 0);
    };
}

//------------------------------------------------------------------------------

int  wxsImageList::GetCount(void) {

    return mCount;
}

//------------------------------------------------------------------------------

wxBitmap wxsImageList::GetPreview(int inIndex) {
int             i,j,k,n;
wxString        ss, tt;
wxArrayString   aa;
wxBitmap        bmp;

// no such image?

//cout << "imagelist preview " << inIndex << " of " << mCount << endl;

    if ((inIndex < 0) || (inIndex >= mCount)) return wxNullBitmap;
    if (mImageData.GetCount() == 0) return wxNullBitmap;

// count down to the start of that image data

    n = -1;             // found index at start of data
    j = 0;              // counter of data blocks
    i = 0;              // index into mImagedata

    while ((i < mImageData.GetCount()) && (n < 0)) {
        ss = mImageData.Item(i);
        i += 1;

        if (ss.Find(_("xpm_data")) >= 0) {
            if (j == inIndex) n = i;
            j += 1;
        };
    };

// still no data block?

//cout << "[1]array start at " << n << endl;

    if (n < 0) return wxNullBitmap;

// save that first line

    aa.Clear();
    aa.Add(ss);

// copy out the data block (until the next "xpm_data")

//cout << "[2]array start at " << n << endl;
    i = n;
    n = -1;
    while ((i < mImageData.GetCount()) && (n < 0)) {
        ss = mImageData.Item(i);
        i += 1;

        if (ss.Find(_("xpm_data")) >= 0) n = i;
        else                             aa.Add(ss);
    };

// turn that data block into a bitmap

    wxsImageListDialog::ArrayToBitmap(aa, bmp);

// done

    return bmp;
}

//------------------------------------------------------------------------------

void wxsImageList::GetImageList(wxImageList &aImageList) {

    aImageList.RemoveAll();
    wxsImageListDialog::ArrayToImageList(mImageData, aImageList);
}



//------------------------------------------------------------------------------
// this just exposes the "OnBuildCreatingCode()" method

void wxsImageList::DoBuild(void) {
    BuildCode(mContext);
}













