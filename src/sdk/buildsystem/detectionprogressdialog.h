#ifndef DETECTPROG
#define DETECTPROG
#undef new

#include <lots_of_stuff>

class DetectProgressDialog : public wxDialog
{
wxTreeMultiCtrl *tree;

public:
    DetectProgressDialog(wxWindow *parent) : wxDialog(parent, -1, _T("Detecting build tools"))
    {
        wxFlexGridSizer *s = new wxFlexGridSizer( 1, 0, 0 );
        s->AddGrowableCol( 0 );
        s->AddGrowableRow( 0 );

        tree = new wxTreeMultiCtrl(this, -1, wxDefaultPosition, wxDefaultSize, wxTMC_DEFAULT_STYLE | wxTMC_SPAN_WIDTH);
        tree->SetSpacingY(2);

        wxFont f = tree->GetCaptionFont();
        f.SetWeight(wxFONTWEIGHT_BOLD);
        tree->SetCaptionFont(f);

        s->Add(tree, 0, wxGROW|wxALL, 5);

        SetSizer(s);
        s->SetSizeHints(this);
    };

    void AddItem(const wxString& title, const wxString& text);
    void SetCurrItemText(const wxString& text);

};

#endif
