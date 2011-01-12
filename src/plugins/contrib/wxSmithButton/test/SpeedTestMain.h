/***************************************************************
 * Name:      SpeedTestMain.h
 * Purpose:   Defines Application Frame
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
#ifndef SPEEDTESTMAIN_H
#define SPEEDTESTMAIN_H

//(*Headers(SpeedTestFrame)
#include "../images/ARROW3L.xpm"
#include "../images/FILEOPEN.xpm"
#include <wx/stattext.h>
#include <wx/menu.h>
#include "../images/CDDRIVE.xpm"
#include "../images/ARROW3R.xpm"
#include "../images/ARROW3D.xpm"
#include "../images/ARROW1U.xpm"
#include <wx/panel.h>
#include "../images/ARROW2U.xpm"
#include <wxSpeedButton.h>
#include "../images/FILENEW.xpm"
#include "../images/COMPPC2.xpm"
#include "../images/COMPMAC.xpm"
#include "../images/ARROW1R.xpm"
#include "../images/FILESAVE.xpm"
#include "../images/ARROW2R.xpm"
#include "../images/COMPPC1.xpm"
#include "../images/ARROW1L.xpm"
#include <wx/frame.h>
#include "../images/ARROW2D.xpm"
#include "../images/FILECLOS.xpm"
#include "../images/ARROW1D.xpm"
#include "../images/ARROW2L.xpm"
#include "../images/ARROW3U.xpm"
//*)

#include "wxSpeedButton.h"

class SpeedTestFrame: public wxFrame
{
    public:

        SpeedTestFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~SpeedTestFrame();

    private:

        //(*Handlers(SpeedTestFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnSpeedButton1LeftClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(SpeedTestFrame)
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_SPEEDBUTTON1;
        static const long ID_SPEEDBUTTON2;
        static const long ID_SPEEDBUTTON3;
        static const long ID_SPEEDBUTTON4;
        static const long ID_PANEL2;
        static const long ID_STATICTEXT3;
        static const long ID_SPEEDBUTTON5;
        static const long ID_SPEEDBUTTON6;
        static const long ID_SPEEDBUTTON7;
        static const long ID_SPEEDBUTTON8;
        static const long ID_PANEL3;
        static const long ID_STATICTEXT4;
        static const long ID_SPEEDBUTTON9;
        static const long ID_SPEEDBUTTON10;
        static const long ID_SPEEDBUTTON11;
        static const long ID_SPEEDBUTTON12;
        static const long ID_PANEL4;
        static const long ID_STATICTEXT5;
        static const long ID_SPEEDBUTTON13;
        static const long ID_SPEEDBUTTON14;
        static const long ID_SPEEDBUTTON15;
        static const long ID_SPEEDBUTTON16;
        static const long ID_SPEEDBUTTON17;
        static const long ID_SPEEDBUTTON18;
        static const long ID_SPEEDBUTTON19;
        static const long ID_SPEEDBUTTON20;
        static const long ID_PANEL5;
        static const long ID_PANEL1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        //*)

        //(*Declarations(SpeedTestFrame)
        wxSpeedButton* SpeedButton13;
        wxPanel* Panel5;
        wxSpeedButton* SpeedButton14;
        wxSpeedButton* SpeedButton18;
        wxSpeedButton* SpeedButton12;
        wxStaticText* StaticText2;
        wxPanel* Panel4;
        wxSpeedButton* SpeedButton20;
        wxSpeedButton* SpeedButton19;
        wxSpeedButton* SpeedButton5;
        wxPanel* Panel1;
        wxSpeedButton* SpeedButton4;
        wxStaticText* StaticText1;
        wxSpeedButton* SpeedButton15;
        wxStaticText* StaticText3;
        wxSpeedButton* SpeedButton8;
        wxPanel* Panel3;
        wxSpeedButton* SpeedButton7;
        wxSpeedButton* SpeedButton11;
        wxSpeedButton* SpeedButton2;
        wxStaticText* StaticText5;
        wxSpeedButton* SpeedButton9;
        wxSpeedButton* SpeedButton1;
        wxSpeedButton* SpeedButton17;
        wxSpeedButton* SpeedButton3;
        wxPanel* Panel2;
        wxSpeedButton* SpeedButton10;
        wxSpeedButton* SpeedButton6;
        wxSpeedButton* SpeedButton16;
        wxStaticText* StaticText4;
        //*)

        void SpeedButtonEvent(wxCommandEvent& event);

        wxSpeedButton   *sb1;


        DECLARE_EVENT_TABLE()
};

#endif // SPEEDTESTMAIN_H
