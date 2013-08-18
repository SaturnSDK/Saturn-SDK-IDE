/////////////////////////////////////////////////////////////////////////////
// Name:        legendwindow.h
// Purpose:     wxChart
// Author:      Paolo Gava
// Modified by:
// Created:
// Copyright:   (C) 2006, Paolo Gava
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if !defined( __WXLEGENDWINDOW_H__ )
#define __WXLEGENDWINDOW_H__

//----------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "legendwindow.h"
#endif

#include <wx/window.h>

#include "wx/legend.h"

//----------------------------------------------------------------------------
// Consts
//----------------------------------------------------------------------------
const int LEGEND_WIDTH	= 70;	// width in pixels
const int LEGEND_HEIGHT	= 60;	// height in pixels

//+++-S-cd-------------------------------------------------------------------
//	NAME:		wxLegendWindow
//	DESC:
//	INTERFACE:
//
//----------------------------------------------------------------------E-+++
/* C::B begin */
class WXDLLIMPEXP_CHART wxLegendWindow : public wxWindow
/* C::B end */
{
public:
	wxLegendWindow() {};	// for IMPLEMENT_DYNAMIC_CLASS
	wxLegendWindow(wxWindow *parent);

	// List descriptos utilities
	// allow access to CWxLegend functionality
	//----------------------------------------
    void Add(const wxString &lbl, const ChartColor &col);
	void Clear();
	int GetCount() const;
	ChartColor GetColor(int n) const;
	wxString GetLabel(int n) const;

	// Draw legend window
	//-------------------
	void Draw(CHART_HPAINT hp, int x = 0, int y = 0);

private:
	wxLegend m_Legend;
	wxWindow *m_WinParent;

	// events
	//-------
    void OnPaint(wxPaintEvent &event);
    void OnMouse(wxMouseEvent &event);
    void OnMouseMove(wxMouseEvent &event);

/* C::B begin */
    DECLARE_DYNAMIC_CLASS(wxLegendWindow)
/* C::B end */
    DECLARE_EVENT_TABLE()
};

#endif // __WXLEGENDWINDOW_H__
