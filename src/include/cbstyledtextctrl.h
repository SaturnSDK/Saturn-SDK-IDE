/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef CBSTYLEDTEXTCTRL_H_INCLUDED
#define CBSTYLEDTEXTCTRL_H_INCLUDED

#include "wx/wxscintilla.h"

class wxContextMenuEvent;
class wxFocusEvent;
class wxMouseEvent;

class cbStyledTextCtrl : public wxScintilla
{
    public:
        cbStyledTextCtrl(wxWindow* pParent, int id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
        virtual ~cbStyledTextCtrl();
    private:
        void OnContextMenu(wxContextMenuEvent& event);
        void OnKillFocus(wxFocusEvent& event);
        void OnGPM(wxMouseEvent& event);

        wxWindow* m_pParent;
        DECLARE_EVENT_TABLE()
};

#endif // CBSTYLEDTEXTCTRL_H_INCLUDED
