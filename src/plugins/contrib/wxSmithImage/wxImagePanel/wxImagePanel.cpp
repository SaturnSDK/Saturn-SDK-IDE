
#include "wxImagePanel.h"

#include    <iostream>



IMPLEMENT_CLASS(wxImagePanel, wxPanel)


//-----------------------------------------------------------------------------
// create a new panel

wxImagePanel::wxImagePanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
            wxPanel(parent, id, pos, size, style, name) {

    mBitmap  = wxNullBitmap;
    mStretch = false;

    Connect(wxEVT_PAINT, (wxObjectEventFunction)&wxImagePanel::DoPaint, 0, this);

};


//-----------------------------------------------------------------------------

void    wxImagePanel::SetBitmap(wxBitmap &inBitmap) {

    mBitmap = inBitmap;
    Refresh();
}

wxBitmap wxImagePanel::GetBitmap(void) {

    return mBitmap;
}

//-----------------------------------------------------------------------------

void    wxImagePanel::SetStretch(bool inStretch) {

    mStretch = inStretch;
    Refresh();
}

bool    wxImagePanel::GetStretch(void) {

    return mStretch;
}



//-----------------------------------------------------------------------------

void    wxImagePanel::DoPaint(wxPaintEvent& event) {
int         i,n;
int         ww, wh;
int         bw, bh;
double      sx, sy;
wxColour    cc;
wxBrush     brush;
wxPaintDC   dc(this);

// fill in the background color

/**
    cc = GetBackgroundColour();
    if (cc.IsOk()) {

    };
**/

    dc.Clear();

// no valid picture?

    if (! mBitmap.IsOk()) return;

// need to stretch it?

    if (mStretch) {
        GetSize(&ww, &wh);
        bw = mBitmap.GetWidth();
        bh = mBitmap.GetHeight();
        if ((bw > 0) && (bh > 0)) {
            sx = ((double) ww) / ((double) bw);
            sy = ((double) wh) / ((double) bh);
            dc.SetUserScale(sx, sy);
        };
    };

// draw it

    dc.DrawBitmap(mBitmap, 0, 0, false);

// reset the scale

    dc.SetUserScale(1.0, 1.0);
}



