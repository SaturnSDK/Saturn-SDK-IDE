/*
* This file is part of lib_finder plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*
* $Revision: 4504 $
* $Id: wxsmithpluginregistrants.cpp 4504 2007-10-02 21:52:30Z byo $
* $HeadURL: svn+ssh://byo@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/plugin/wxsmithpluginregistrants.cpp $
*/

#include <wx/arrstr.h>

#include "resultmap.h"

ResultMap::ResultMap()
{
}

ResultMap::~ResultMap()
{
    Clear();
}

void ResultMap::Clear()
{
    for ( ResultHashMap::iterator it = Map.begin(); it != Map.end(); ++it )
    {
        ResultArray& RA = it->second;
        for ( size_t i = 0; i<RA.Count(); ++i )
        {
            delete RA[i];
        }
    }
    Map.clear();
}

void ResultMap::GetAllResults(ResultArray& Array)
{
    for ( ResultHashMap::iterator it = Map.begin(); it != Map.end(); ++it )
    {
        ResultArray& RA = it->second;
        for ( size_t i = 0; i<RA.Count(); ++i )
        {
            Array.Add(RA[i]);
        }
    }
}

void ResultMap::GetShortCodes(wxArrayString& Array)
{
    for ( ResultHashMap::const_iterator it = Map.begin(); it != Map.end(); ++it )
    {
        Array.Add(it->first);
    }
}

bool ResultMap::IsShortCode(const wxString& Name)
{
    if ( Map.find(Name) == Map.end() ) return false;
    return !Map[Name].IsEmpty();
}
