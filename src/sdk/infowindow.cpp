#include "sdk_precomp.h"

BEGIN_EVENT_TABLE(InfoWindow, wxPopupWindow)
EVT_TIMER(-1, InfoWindow::OnTimer)
END_EVENT_TABLE()

const wxColour titleBackground(96,96,96);
const wxColour textBackground(245,245,245);

Stacker InfoWindow::stacker;
int InfoWindow::screenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
int InfoWindow::screenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

InfoWindow::InfoWindow(const wxString& title, const wxString& message, unsigned int delay, unsigned int hysteresis)
            : wxPopupWindow(Manager::Get()->GetAppWindow(), wxSIMPLE_BORDER | wxWS_EX_TRANSIENT | wxCLIP_CHILDREN),
              m_timer(new wxTimer(this, 0)), status(0), m_delay(delay)
    {
        wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);

        wxStaticText *titleC = new wxStaticText(this, -1, title, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
        titleC->SetForegroundColour(*wxWHITE);
        titleC->SetBackgroundColour(titleBackground);
        titleC->SetFont(wxFont(11, wxSWISS, wxNORMAL, wxBOLD));
        bs->Add(titleC, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

        wxStaticText *text = new wxStaticText(this, -1, message, wxDefaultPosition, wxDefaultSize, 0);
        text->SetBackgroundColour(textBackground);
        bs->Add(text, 0, wxALIGN_CENTER|wxALL, 10);
        SetBackgroundColour(textBackground);
        SetSizer(bs);
        bs->SetSizeHints(this);
        Layout();


        wxCoord w, h;
        GetClientSize(&w, &h);

        pos = stacker.StackMe(w);

        left = screenWidth - pos;
        hMin = screenHeight - h;
        top = screenHeight;

        Move(left, top);

        Show();
        m_timer->Start(hysteresis, false);
    };


InfoWindow::~InfoWindow()
    {
        delete m_timer;
        stacker.ReleaseMe(pos);
    };

void InfoWindow::OnTimer(wxTimerEvent& e)
{
    switch(status)
    {
    case 0:
        status = 1;
        m_timer->Start(1, false);
        break;
    case 1:
        top -= 2;
        Move(left, top);
        if(top <= hMin)
        {
            status = 2;
            m_timer->Start(m_delay, true);
        }
        break;
    case 2:
        status = 3;
        m_timer->Start(1, false);
        break;
    case 3:
        top += 2;
        Move(left, top);
        if(top > screenHeight)
        {
            Hide();
            Destroy();
        }
        break;
    };
};



