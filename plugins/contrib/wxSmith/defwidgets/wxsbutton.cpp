#include "../wxsheaders.h"
#include "wxsbutton.h"

WXS_ST_BEGIN(wxsButtonStyles)
    WXS_ST_CATEGORY("wxButton")
    WXS_ST(wxBU_LEFT)
    WXS_ST(wxBU_TOP)
    WXS_ST(wxBU_RIGHT)
    WXS_ST(wxBU_BOTTOM)
    WXS_ST(wxBU_EXACTFIT)
    // cyberkoa : In XRC Default WindowsStyle, MSW and GTK only
    WXS_ST_MASK(wxNO_BORDER,wxsSFWin| wxsSFGTK ,0,true)

WXS_ST_END(wxsButtonStyles)

WXS_EV_BEGIN(wxsButtonEvents)
    WXS_EVI(EVT_BUTTON,wxCommandEvent,Click)
    WXS_EV_DEFAULTS()
WXS_EV_END(wxsButtonEvents)


wxsDWDefineBegin(wxsButton,wxButton,

    ThisWidget = new wxButton(parent,id,label,pos,size,style);
    if (def) ThisWidget->SetDefault();

    )

    wxsDWDefLongStr(label,"Label:","Label");
    wxsDWDefBoolX(def,"default","Default:",false);

wxsDWDefineEnd()
