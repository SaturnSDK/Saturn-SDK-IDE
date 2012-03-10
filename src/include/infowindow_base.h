/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

/*
 * Objects of type InfoWindow are autonomous and must therefore always be instantiated using operator new.
 * In addition to this, InfoWindow does not have any non-static public class members.
 *
 * Since a (technically 100% correct) statement like
 *   new InfoWindow("foo", "bar");
 * is unintuitive, confusing or even objective to some people, this class uses a variation of the "Named Constructor Idiom".
 *
 * InfoWindow::Display("foo", "bar");
 * does the exact same thing as the above statement but looks a lot nicer.
 */

#ifndef INFOWINDOW_BASE_H
#define INFOWINDOW_BASE_H

#include <wx/string.h>

#include <list>
#include <algorithm>

class InfoWindow // : public wxInfoWindowBase
{
    InfoWindow(const wxString& title, const wxString& message);
    virtual ~InfoWindow();

    public:

    static void Display(const wxString& title, const wxString& message, unsigned int delay = 5000, unsigned int hysteresis = 1);
};

#endif
