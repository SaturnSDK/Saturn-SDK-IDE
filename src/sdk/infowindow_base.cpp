/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6950 $
 * $Id: infowindow.cpp 6950 2011-02-01 07:13:54Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/branches/codeblocks_console/src/sdk/infowindow.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include "manager.h"
#endif

#include "infowindow_base.h"
#include "globals_base.h"


InfoWindow::InfoWindow(const wxString& title, const wxString& message)
    {
        cbMessageBox(message, title);
    };


InfoWindow::~InfoWindow()
{
};

// static
void InfoWindow::Display(const wxString& title, const wxString& message, unsigned int delay, unsigned int hysteresis)
{
    new InfoWindow(title, message);
}
