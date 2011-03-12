///////////////////////////////////////////////////////////////////////////////
// Name:        transformation.cpp
// Purpose:     Demonstration of transformations in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-09-07
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/pdfdoc.h"

/**
* Transformations
* 
* This example shows the effects of various geometric transformations
*/

void
transformation()
{
  wxPdfDocument pdf;
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"),wxT(""),12);

  // Scaling
  pdf.SetDrawColour(200);
  pdf.SetTextColour(200);
  pdf.Rect(50, 20, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(50, 19, wxT("Scale"));
  pdf.SetDrawColour(0);
  pdf.SetTextColour(0);
  // Start Transformation
  pdf.StartTransform();
  // Scale by 150% centered by (50,30) which is the lower left corner of the rectangle
  pdf.ScaleXY(150, 50, 30);
  pdf.Rect(50, 20, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(50, 19, wxT("Scale"));
  // Stop Transformation
  pdf.StopTransform();

  // Translation
  pdf.SetDrawColour(200);
  pdf.SetTextColour(200);
  pdf.Rect(125, 20, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(125, 19, wxT("Translate"));
  pdf.SetDrawColour(0);
  pdf.SetTextColour(0);
  // Start Transformation
  pdf.StartTransform();
  // Translate 20 to the right, 15 to the bottom
  pdf.Translate(20, 15);
  pdf.Rect(125, 20, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(125, 19, wxT("Translate"));
  // Stop Transformation
  pdf.StopTransform();

  //Rotation
  pdf.SetDrawColour(200);
  pdf.SetTextColour(200);
  pdf.Rect(50, 50, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(50, 49, wxT("Rotate"));
  pdf.SetDrawColour(0);
  pdf.SetTextColour(0);
  // Start Transformation
  pdf.StartTransform();
  // Rotate 20 degrees counter-clockwise centered by (50,60)
  // which is the lower left corner of the rectangle
  pdf.Rotate(20, 50, 60);
  pdf.Rect(50, 50, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(50, 49, wxT("Rotate"));
  // Stop Transformation
  pdf.StopTransform();

  // Skewing
  pdf.SetDrawColour(200);
  pdf.SetTextColour(200);
  pdf.Rect(125, 50, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(125, 49, wxT("Skew"));
  pdf.SetDrawColour(0);
  pdf.SetTextColour(0);
  // Start Transformation
  pdf.StartTransform();
  // skew 30 degrees along the x-axis centered by (125,60)
  // which is the lower left corner of the rectangle
  pdf.SkewX(30, 125, 60);
  pdf.Rect(125, 50, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(125, 49, wxT("Skew"));
  // Stop Transformation
  pdf.StopTransform();

  // Mirroring horizontally
  pdf.SetDrawColour(200);
  pdf.SetTextColour(200);
  pdf.Rect(50, 80, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(50, 79, wxT("MirrorH"));
  pdf.SetDrawColour(0);
  pdf.SetTextColour(0);
  // Start Transformation
  pdf.StartTransform();
  // mirror horizontally with axis of reflection at x-position 50 (left side of the rectangle)
  pdf.MirrorH(50);
  pdf.Rect(50, 80, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(50, 79, wxT("MirrorH"));
  // Stop Transformation
  pdf.StopTransform();

  // Mirroring vertically
  pdf.SetDrawColour(200);
  pdf.SetTextColour(200);
  pdf.Rect(125, 80, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(125, 79, wxT("MirrorV"));
  pdf.SetDrawColour(0);
  pdf.SetTextColour(0);
  // Start Transformation
  pdf.StartTransform();
  // mirror vertically with axis of reflection at y-position 90 (bottom side of the rectangle)
  pdf.MirrorV(90);
  pdf.Rect(125, 80, 40, 10, wxPDF_STYLE_DRAW);
  pdf.Text(125, 79, wxT("MirrorV"));
  // Stop Transformation
  pdf.StopTransform();

  pdf.SaveAsFile(wxT("transformation.pdf"));
}

