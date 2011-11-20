/***************************************************************
 * Name:      TestTreeListApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2010-02-09
 * Copyright:  ()
 * License:
 **************************************************************/

#include "TestTreeListApp.h"

//(*AppHeaders
#include "TestTreeListMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(TestTreeListApp);

bool TestTreeListApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	TestTreeListFrame* Frame = new TestTreeListFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
