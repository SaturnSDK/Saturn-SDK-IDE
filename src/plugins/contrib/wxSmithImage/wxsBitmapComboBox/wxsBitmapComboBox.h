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
* $Revision: 4850 $
* $Id: wxsbitmapbutton.h 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsbitmapbutton.h $
*/

#ifndef WXSBITMAPCOMBOBOX_H
#define WXSBITMAPCOMBOBOX_H

#include <wx/bmpcbox.h>

#include "wxswidget.h"
#include "wxsenumproperty.h"
#include "wxseditenumproperty.h"
#include "wxsarraystringproperty.h"
#include "wxsboolproperty.h"
#include "wxsimagecomboproperty.h"
#include "wxsImage.h"
#include "wxsImageList.h"
#include "wxsImageListDialog.h"


/** \brief Class for wxButton widget */
class wxsBitmapComboBox: public wxsWidget
{
    public:

        wxsBitmapComboBox(wxsItemResData* Data);

    private:

        virtual void        OnBuildCreatingCode();
        virtual wxObject   *OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void        OnEnumWidgetProperties(long Flags);

                void        FindAllImageLists(wxArrayString &aNames);
                void        UpdateComboItemList(void);
                void        ParseComboItem(wxString inSource, wxString &outItem, int &outIndex);



        wxString        mImageList;                 // list to use to select images
        wxArrayString   mItems;                     // text of drop-down items
};

#endif
