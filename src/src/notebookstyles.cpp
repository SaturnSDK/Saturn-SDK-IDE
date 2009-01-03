/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <wx/dc.h>
#include <wx/window.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/settings.h>
#include <wx/dcclient.h>
#include <wx/aui/auibook.h>
#include "notebookstyles.h"

// Some general constants:
namespace
{
    const int c_vertical_border_padding = 4;
}

/******************************************************************************
* Renderer for Microsoft (tm) Visual Studio 7.1 like tabs                     *
******************************************************************************/

NbStyleVC71::NbStyleVC71() : wxAuiDefaultTabArt()
{
}

wxAuiTabArt* NbStyleVC71::Clone()
{
    NbStyleVC71* clone = new NbStyleVC71();
    
    clone->SetNormalFont(m_normal_font);
    clone->SetSelectedFont(m_selected_font);
    clone->SetMeasuringFont(m_measuring_font);
    
    return clone;
}

void NbStyleVC71::DrawTab(wxDC& dc, wxWindow* wnd,
                            const wxAuiNotebookPage& page,
                            const wxRect& in_rect, int close_button_state,
                            wxRect* out_tab_rect, wxRect* out_button_rect,
                            int* x_extent)
{
    // if the caption is empty, measure some temporary text
    wxString caption = page.caption;
    if (caption.empty())
        caption = wxT("Xj");

    wxCoord textx;
    wxCoord texty;
    if (page.active)
        dc.SetFont(m_selected_font);
    else
        dc.SetFont(m_normal_font);
    dc.GetTextExtent(caption, &textx, &texty);
    
    // Visual studio 7.1 style
    // This code is based on the renderer included in wxFlatNotebook:
    // http://svn.berlios.de/wsvn/codeblocks/trunk/src/sdk/wxFlatNotebook/src/wxFlatNotebook/renderer.cpp?rev=5106
    wxPen borderPen = wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));

    // figure out the size of the tab
    wxSize tab_size = GetTabSize(dc, wnd, page.caption, wxNullBitmap,
                                    page.active, close_button_state, x_extent);

    wxCoord tab_height = m_tab_ctrl_height - 3;
    wxCoord tab_width = tab_size.x;
    wxCoord tab_x = in_rect.x;
    wxCoord tab_y = in_rect.y + in_rect.height - tab_height;
    int clip_width = tab_width;
    if (tab_x + clip_width > in_rect.x + in_rect.width - 4)
        clip_width = (in_rect.x + in_rect.width) - tab_x - 4;
    dc.SetClippingRegion(tab_x, tab_y, clip_width + 1, tab_height - 3);
    
    dc.SetPen((page.active) ? wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)) : borderPen);
    dc.SetBrush((page.active) ? wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)) : wxBrush(wxColour(247, 243, 233)));

    if (page.active)
    {
        int tabH = (m_flags & wxAUI_NB_BOTTOM) ? tab_height - 5 : tab_height - 3;

        dc.DrawRectangle(tab_x, tab_y, tab_width, tabH);

        // Draw a black line on the left side of the
        // rectangle
        wxPen pen = wxPen(*wxBLACK);
        dc.SetPen(pen);

        int blackLineY1 = c_vertical_border_padding;
        int blackLineY2 = tabH;
        dc.DrawLine(tab_x + tab_width, blackLineY1, tab_x + tab_width, blackLineY2);

        // To give the tab more 3D look we do the following
        // Incase the tab is on top,
        // Draw a thik white line on topof the rectangle
        // Otherwise, draw a thin (1 pixel) black line at the bottom
        pen = wxPen((m_flags & wxAUI_NB_BOTTOM) ? *wxBLACK : *wxWHITE);
        dc.SetPen(pen);
        int whiteLinePosY = (m_flags & wxAUI_NB_BOTTOM) ? blackLineY2 : c_vertical_border_padding ;
        dc.DrawLine(tab_x, whiteLinePosY, tab_x + tab_width + 1, whiteLinePosY);

        // Draw a white vertical line to the left of the tab
        dc.SetPen(*wxWHITE_PEN);
        if (m_flags & wxAUI_NB_BOTTOM)
            blackLineY2 += 1;
        dc.DrawLine(tab_x, blackLineY1, tab_x, blackLineY2);
    }
    else
    {
        // We dont draw a rectangle for non selected tabs, but only
        // vertical line on the left
        int blackLineY1 = (m_flags & wxAUI_NB_BOTTOM) ? c_vertical_border_padding + 2 : c_vertical_border_padding + 1;
        int blackLineY2 = tab_height - 5;
        dc.DrawLine(tab_x + tab_width, blackLineY1, tab_x + tab_width, blackLineY2);
    }

    wxPoint border_points[2];
    if (m_flags & wxAUI_NB_BOTTOM)
    {
        border_points[0] = wxPoint(tab_x, tab_y);
        border_points[1] = wxPoint(tab_x, tab_y + tab_height - 6);
    }
    else // if (m_flags & wxAUI_NB_TOP)
    {
        border_points[0] = wxPoint(tab_x, tab_y + tab_height - 4);
        border_points[1] = wxPoint(tab_x, tab_y + 2);
    }

    int drawn_tab_yoff = border_points[1].y;
    int drawn_tab_height = border_points[0].y - border_points[1].y;    
    int text_offset = tab_x + 8;

    // draw tab text
    dc.DrawText(page.caption, text_offset,
                drawn_tab_yoff + drawn_tab_height / 2 - texty / 2 - 1);

    // draw 'x' on tab (if enabled)
    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    {
        int close_button_width = m_active_close_bmp.GetWidth();
        wxBitmap bmp = m_disabled_close_bmp;
        
        if ((close_button_state == wxAUI_BUTTON_STATE_HOVER) ||
                    (close_button_state == wxAUI_BUTTON_STATE_PRESSED))
            bmp = m_active_close_bmp;

        wxRect rect(tab_x + tab_width - close_button_width - 3,
                    drawn_tab_yoff + (drawn_tab_height / 2) - (bmp.GetHeight() / 2),
                    close_button_width, tab_height);

        // Indent the button if it is pressed down:
        if (close_button_state == wxAUI_BUTTON_STATE_PRESSED)
        {
            rect.x++;
            rect.y++;
        }
        dc.DrawBitmap(bmp, rect.x, rect.y, true);
        *out_button_rect = rect;
    }

    *out_tab_rect = wxRect(tab_x, tab_y, tab_width, tab_height);
    dc.DestroyClippingRegion();
}

int NbStyleVC71::GetBestTabCtrlSize(wxWindow* wnd,
                                    const wxAuiNotebookPageArray& WXUNUSED(pages),
                                    const wxSize& WXUNUSED(required_bmp_size))
{
    wxClientDC dc(wnd);
    dc.SetFont(m_measuring_font);
    int x_ext = 0;
    wxSize s = GetTabSize(dc, wnd, wxT("ABCDEFGHIj"), wxNullBitmap, true,
                            wxAUI_BUTTON_STATE_HIDDEN, &x_ext);
    return s.y + 4;
}
