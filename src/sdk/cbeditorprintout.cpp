#include "cbeditorprintout.h"
#include "printing_types.h"

cbEditorPrintout::cbEditorPrintout(const wxString& title, wxStyledTextCtrl* control, bool selectionOnly)
        : wxPrintout(title),
        m_TextControl(control)
{
    // ctor
    m_SelStart = 0;
    m_SelEnd = control->GetLength();
    if (selectionOnly && !control->GetSelectedText().IsEmpty())
    {
        m_SelStart = control->GetSelectionStart();
        m_SelEnd = control->GetSelectionEnd();
    }
}

cbEditorPrintout::~cbEditorPrintout()
{
    // dtor
}

bool cbEditorPrintout::OnPrintPage(int page)
{
    wxDC *dc = GetDC();
    if (dc)
    {
        // scale DC
        ScaleDC(dc);

        // print page
        if (page == 1)
            m_printed = m_SelStart;
        m_printed = m_TextControl->FormatRange (1, m_printed, m_SelEnd,
                                                dc, dc, m_printRect, m_pageRect);
        return true;
    }
    return false;
}

bool cbEditorPrintout::HasPage(int page)
{
    return (m_printed < m_SelEnd);
}

void cbEditorPrintout::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
{
    // initialize values
    *minPage = 0;
    *maxPage = 0;
    *selPageFrom = 0;
    *selPageTo = 0;
    // get print page information and convert to printer pixels
    wxSize ppiScr;
    GetPPIScreen (&ppiScr.x, &ppiScr.y);
    if (ppiScr.x == 0 || ppiScr.y == 0)
    {
        // guess
        ppiScr.x = 96;
        ppiScr.y = 96;
    }
    wxSize page = g_pageSetupData->GetPaperSize();
    page.x = static_cast<int> (page.x * ppiScr.x / 25.4);
    page.y = static_cast<int> (page.y * ppiScr.y / 25.4);
    m_pageRect = wxRect (0,
                         0,
                         page.x,
                         page.y);
    // get margins information and convert to printer pixels
    int  top = 25; // default 25
    int  bottom = 25; // default 25
    int  left = 20; // default 20
    int  right = 20; // default 20
    wxPoint (top, left) = g_pageSetupData->GetMarginTopLeft();
    wxPoint (bottom, right) = g_pageSetupData->GetMarginBottomRight();
    top = static_cast<int> (top * ppiScr.y / 25.4);
    bottom = static_cast<int> (bottom * ppiScr.y / 25.4);
    left = static_cast<int> (left * ppiScr.x / 25.4);
    right = static_cast<int> (right * ppiScr.x / 25.4);
    m_printRect = wxRect (left,
                          top,
                          page.x - (left + right),
                          page.y - (top + bottom));

    // wxWidgets 2.4.2, have some printing-related bugs.
    // one of them is that that GetDC always returns 0 in GetPageInfo.
    // this means we can't count the pages...

    //    wxDC *dc = GetDC();
    //    if (!dc) return;
    //    ScaleDC(dc);

    // count pages
    //    m_printed = m_SelStart;
    //    while (HasPage (*maxPage)) {
    //        m_printed = m_TextControl->FormatRange (0, m_printed, m_SelEnd,
    //                                         dc, dc, m_printRect, m_pageRect);
    //        *maxPage += 1;
    //    }
    *maxPage = 32000;
    if (*maxPage > 0)
        *minPage = 1;
    *selPageFrom = *minPage;
    *selPageTo = *maxPage;
    m_printed = m_SelStart;
}

bool cbEditorPrintout::OnBeginDocument(int startPage, int endPage)
{
    return wxPrintout::OnBeginDocument(startPage, endPage);
}

bool cbEditorPrintout::ScaleDC(wxDC *dc)
{
    if (!dc)
        return false;
    // get printer and screen sizing values
    wxSize ppiScr;
    GetPPIScreen (&ppiScr.x, &ppiScr.y);
    if (ppiScr.x == 0)
    { // most possible guess 96 dpi
        ppiScr.x = 96;
        ppiScr.y = 96;
    }
    wxSize ppiPrt;
    GetPPIPrinter (&ppiPrt.x, &ppiPrt.y);
    if (ppiPrt.x == 0)
    { // scaling factor to 1
        ppiPrt.x = ppiScr.x;
        ppiPrt.y = ppiScr.y;
    }
    wxSize dcSize = dc->GetSize();
    wxSize pageSize;
    GetPageSizePixels (&pageSize.x, &pageSize.y);
    // set user scale
    float scale_x = (float)(ppiPrt.x * dcSize.x) /
                    (float)(ppiScr.x * pageSize.x);
    float scale_y = (float)(ppiPrt.y * dcSize.y) /
                    (float)(ppiScr.y * pageSize.y);
    dc->SetUserScale (scale_x, scale_y);
    return true;
}
