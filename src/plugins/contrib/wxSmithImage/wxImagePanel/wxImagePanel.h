
#ifndef _IMAGEPANEL_H_
#define _IMAGEPANEL_H_

#include    <wx/object.h>
#include    <wx/panel.h>
#include    <wx/image.h>
#include    <wx/bitmap.h>
#include    <wx/dcclient.h>


class wxImagePanel : public wxPanel
{
public:

        wxImagePanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = _("ImagePanel"));

        void        SetBitmap(wxBitmap &inBitmap);
        wxBitmap    GetBitmap(void);

        void        SetStretch(bool inStretch);
        bool        GetStretch(void);

        void        DoPaint(wxPaintEvent& event);



protected:

    wxBitmap    mBitmap;
    bool        mStretch;

    DECLARE_DYNAMIC_CLASS(wxImagePanel)

};



#endif      // _IMAGEPANEL_H_
