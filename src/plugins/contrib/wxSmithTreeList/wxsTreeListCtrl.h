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

#ifndef WXSTREELISTCTRL_H
#define WXSTREELISTCTRL_H

#include    "wxswidget.h"
#include    "wxslongproperty.h"
#include    "wxsenumproperty.h"
#include    "wxsboolproperty.h"
#include    "treelistctrl.h"

/** \brief Class for wxsTreeCtrl widget */
class wxsTreeListCtrl: public wxsWidget
{
    public:

        wxsTreeListCtrl(wxsItemResData* Data);

    private:

        virtual void        OnBuildCreatingCode();
        virtual wxObject*   OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void        OnEnumWidgetProperties(long Flags);


                void        BuildItemCode(void);
                void        PreviewItemCode(wxTreeListCtrl *inPreview);
                void        ParseItems(wxString inSource, int &outLevel, wxArrayString &outItems);




        long            mColCount;                  // number of columns
        long            mColWidth;                  // size of the columns
        wxArrayString   mColTitles;                 // heading at top of each column
        long            mTabChar;                   // what separates items?
        wxArrayString   mTreeData;                  // text items in the tree
        bool            mReadOnly;                  // can edit data in tree?


};

#endif  // WXSTREELISTCTRL_H
