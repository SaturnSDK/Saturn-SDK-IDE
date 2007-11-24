/////////////////////////////////////////////////////////////////////////////
// Name:        chart.h
// Purpose:     wxChart
// Author:      Paolo Gava
// Modified by:
// Created:
// Copyright:   (C) 2006, Paolo Gava
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if !defined( __CHART_H__ )
#define __CHART_H__

//----------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/dynarray.h>
#endif

#include "wx/chartpoints.h"

//----------------------------------------------------------------------------
// Declare Array/List of Points
//----------------------------------------------------------------------------
WX_DECLARE_OBJARRAY(wxChartPoints*, ListChartPoints);

//+++-S-cd-------------------------------------------------------------------
//	NAME:		wxChart
//	DESC:
//	INTERFACE:
//
//----------------------------------------------------------------------E-+++
class WXDLLIMPEXP_CHART wxChart
{
public:
	wxChart();
	virtual ~wxChart();

	// ChartPoints utility
	//--------------------
	void Add(wxChartPoints* cp);
	void Clear();

	// Get statistics
	//---------------
	ChartValue GetMaxX() const;
	ChartValue GetMinX() const;
	ChartValue GetMaxY() const;
	ChartValue GetMinY() const;
	int GetNumBarPoints() const;
	int GetNumBar3DPoints() const;

	// Set zoom
	//-------------
	void SetZoom(double z);
    double GetZoom() const;

	// Set sizes for drawing
	//----------------------
	void SetSizes(ChartSizes sizes);
    ChartSizes GetSizes() const;

	// Draw chart
	//-----------
	virtual void Draw(CHART_HPAINT hp, CHART_HRECT hr);

private:
	ListChartPoints m_LCP;
};

#endif // __CHART_H__
