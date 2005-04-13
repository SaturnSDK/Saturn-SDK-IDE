#ifndef CBEDITORPRINTOUT_H
#define CBEDITORPRINTOUT_H

#include <wx/print.h>
#include <wx/stc/stc.h>

class cbEditorPrintout : public wxPrintout
{
    public:
        cbEditorPrintout(const wxString& title, wxStyledTextCtrl* control, bool selectionOnly);
        ~cbEditorPrintout();
        bool OnPrintPage(int page);
        bool HasPage(int page);
        void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);
        bool OnBeginDocument(int startPage, int endPage);
    protected:
        bool ScaleDC(wxDC *dc);
        wxStyledTextCtrl* m_TextControl;
        wxRect m_pageRect;
        wxRect m_printRect;
        int m_printed;
        int m_SelStart;
        int m_SelEnd;
};

#endif // CBEDITORPRINTOUT_H
