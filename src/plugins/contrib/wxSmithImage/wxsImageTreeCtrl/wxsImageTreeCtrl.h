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
* $Id: wxsImageTreeCtrl.h 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsImageTreeCtrl.h $
*/

#ifndef WXSIMAGETREECTRL_H
#define WXSIMAGETREECTRL_H

#include <wx/treectrl.h>
#include "wxswidget.h"
#include "wxsenumproperty.h"
#include "wxseditenumproperty.h"
#include "wxsarraystringproperty.h"
#include "wxsboolproperty.h"
#include "wxsimagetreeproperty.h"
#include "wxsImage.h"
#include "wxsImageList.h"
#include "wxsImageListDialog.h"
#include "wxsImageTreeEditDialog.h"




/** \brief Class for wxsImageTreeCtrl widget */
class wxsImageTreeCtrl: public wxsWidget
{
    public:

        wxsImageTreeCtrl(wxsItemResData* Data);

    private:

        virtual void        OnBuildCreatingCode();
        virtual wxObject   *OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void        OnEnumWidgetProperties(long Flags);
        virtual void        OnBuildDeclarationsCode();

                void        FindAllImageLists(wxArrayString &aNames);
                void        UpdateTreeItemList(void);
                void        ParseTreeItem(wxString inSource, wxString &outItem, int &outIndex);





        wxArrayString   mItems;                     // individual items for the tree control
        wxString        mImageName;                 // list to use to select images
        wxImageList     mImageList;                 // a copy of that list
        bool            mExpand;                    // start with everything expanded?

};

#endif      // WXSIMAGETREECTRL_H
