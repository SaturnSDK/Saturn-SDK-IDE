#include "../wxsheaders.h"
#include "wxscheckbox.h"

WXS_ST_BEGIN(wxsCheckBoxStyles)
    WXS_ST_CATEGORY("wxCheckBox")
    WXS_ST(wxCHK_2STATE)
    WXS_ST(wxALIGN_RIGHT)
    WXS_ST_MASK(wxCHK_3STATE,wxsSFAll,wxsSFMGL |wxsSFGTK12 | wxsSFOS2,true)
    WXS_ST_MASK(wxCHK_ALLOW_3RD_STATE_FOR_USER,wxsSFAll,wxsSFMGL |wxsSFGTK12 | wxsSFOS2,true)
WXS_ST_END(wxsCheckBoxStyles)

WXS_EV_BEGIN(wxsCheckBoxEvents)
    WXS_EVI(EVT_CHECKBOX,wxCommandEvent,Change)
    WXS_EV_DEFAULTS()
WXS_EV_END(wxsCheckBoxEvents)

wxsDWDefineBegin(wxsCheckBox,wxCheckBox,

    ThisWidget = new wxCheckBox(parent,id,label,pos,size,style);
    ThisWidget->SetValue(checked);

    )

    wxsDWDefLongStr(label,"Label:","Label");
    wxsDWDefBool(checked,"Checked:",false);

wxsDWDefineEnd()
