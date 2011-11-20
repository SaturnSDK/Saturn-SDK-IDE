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
* $Revision$
* $Id$
* $HeadURL$
*/

#ifndef WXSIMAGEBUTTON_H
#define WXSIMAGEBUTTON_H

#include "wxswidget.h"
#include "wxsenumproperty.h"
#include "wxseditenumproperty.h"
#include "wxsImage.h"
#include "wxsImageList.h"
#include "wxsImageListDialog.h"


/** \brief Class for wxButton widget */
class wxsImageButton: public wxsWidget
{
    public:

        wxsImageButton(wxsItemResData* Data);

    private:

        virtual void        OnBuildCreatingCode();
        virtual wxObject   *OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void        OnEnumWidgetProperties(long Flags);
        virtual void        OnBuildDeclarationsCode();

        bool        mIsDefault;
        wxString    mImageList;                     // list to use to select images
        wxString    mLabelIndex;                    // main image index
        wxString    mDisabledIndex;                 // disabled image
        wxString    mSelectedIndex;                 // selected image
        wxString    mFocusIndex;                    // focused image

        int mCount;
};

#endif
