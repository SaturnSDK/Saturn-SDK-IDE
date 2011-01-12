/***************************************************************
 * Name:      SpeedTestApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ron Collins (rcoll@theriver.com)
 * Created:   2010-01-15
 * Copyright: Ron Collins ()
 * License:
 **************************************************************/
/*
* This file is part of a wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* wxSmith and this file is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
* 
* Ron Collins
* rcoll@theriver.com
* 4-Feb-2010
* 
*/
#include "SpeedTestApp.h"

//(*AppHeaders
#include "SpeedTestMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(SpeedTestApp);

bool SpeedTestApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	SpeedTestFrame* Frame = new SpeedTestFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
