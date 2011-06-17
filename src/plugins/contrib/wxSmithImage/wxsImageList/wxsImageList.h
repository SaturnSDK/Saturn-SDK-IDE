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

#ifndef wxsIMAGELIST_H
#define wxsIMAGELIST_H

#include    <stdio.h>
#include    <stdlib.h>
#include    <time.h>
#include    <cmath>
#include    <iostream>

//#include    <windef.h>
#include    <wx/settings.h>
#include    <wx/utils.h>
#include    <wx/string.h>
#include    <wx/filename.h>
#include    <wx/imaglist.h>

#include    <wxstool.h>
#include    <wxsitemresdata.h>
#include    "wxslongproperty.h"
#include    "wxsboolproperty.h"
#include    "wxsimagelistproperty.h"
#include    "wxsImageListDialog.h"


class wxsImageList : public wxsTool
{
    public:

        wxsImageList(wxsItemResData* Data);

        int             GetCount(void);
        wxBitmap        GetPreview(int inIndex);
        void            GetImageList(wxImageList &aImageList);
        void            DoBuild(void);

    private:

        virtual void OnBuildCreatingCode();
        virtual void OnEnumToolProperties(long Flags);

                void StoreXpmData(void);


        bool            mIsBuilt;                       // only build the code once
        wxsCoderContext *mContext;
        wxArrayString   mImageData;                     // store all images as XPM's
        long            mWidth, mHeight, mCount;        // size of each image, count of images
        bool            mInclude;                       // save as #include file?
        wxString        mBase;                          // base file name of source and include files
        wxString        mIDir;                          // abs path to image include directory
        wxString        mRDir;                          // relative dir spec for image files


};


#endif      // wxsIMAGELIST_H
