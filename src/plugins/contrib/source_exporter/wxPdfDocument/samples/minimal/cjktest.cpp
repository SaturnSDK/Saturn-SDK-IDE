///////////////////////////////////////////////////////////////////////////////
// Name:        cjktest.cpp
// Purpose:     Demonstration of CJK fonts in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-08-29
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
* Chinese, Japanese and Korean fonts
*
* This example demonstrates the use of CJK fonts.
*
* Remark: Only available in Unicode build. 
*/

void
cjktest()
{
  wxCSConv conv_we(wxT("cp-1252"));
  wxString s_we("���",conv_we);
  wxCSConv conv_cn(wxT("cp-950"));
  wxString s_cn("�{�ɮ�� 18 C ��� 83 %",conv_cn);
  s_cn += s_we;
  wxPdfDocument pdf;
  pdf.AddFontCJK(wxT("Big5"));
  pdf.Open();
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"), wxT(""),24);
  pdf.Write(10,wxT("Chinese"));
  pdf.Ln(12);
  pdf.SetFont(wxT("Big5"),wxT(""),20);
  pdf.Write(10,s_cn);

  wxCSConv conv_jp(wxT("cp-932"));
  wxString s_jp("9�����̌��J�e�X�g���o��PHP 3.0��1998�N6���Ɍ����Ƀ����[�X����܂����B",conv_jp);
  pdf.AddFontCJK(wxT("SJIS"));
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"), wxT(""),24);
  pdf.Write(10,wxT("Japanese"));
  pdf.Ln(12);
  pdf.SetFont(wxT("SJIS"),wxT(""),18);
  pdf.Write(8,s_jp);

  wxCSConv conv_kr(wxT("cp-949"));
  wxString s_kr("PHP 3.0�� 1998�� 6���� ���������� ������Ǿ���. �������� �׽�Ʈ ���ľ� 9�������̾���.",conv_kr);
  pdf.AddFontCJK(wxT("UHC"));
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"), wxT(""),24);
  pdf.Write(10,wxT("Korean"));
  pdf.Ln(12);
  pdf.SetFont(wxT("UHC"),wxT(""),18);
  pdf.Write(8,s_kr);

  pdf.SaveAsFile(wxT("cjktest.pdf"));
}
