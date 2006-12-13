#include "wxscalendarctrl.h"

#include <wx/calctrl.h>

namespace
{
    wxsRegisterItem<wxsCalendarCtrl> Reg(_T("CalendarCtrl"),wxsTWidget,_T("Standard"),30);

    WXS_ST_BEGIN(wxsCalendarCtrlStyles,_T(""))
        WXS_ST_CATEGORY("wxCalendarCtrl")
        WXS_ST(wxCAL_SUNDAY_FIRST)
        WXS_ST(wxCAL_MONDAY_FIRST)
        WXS_ST(wxCAL_SHOW_HOLIDAYS)
        WXS_ST(wxCAL_NO_YEAR_CHANGE)
        WXS_ST(wxCAL_NO_MONTH_CHANGE)
        WXS_ST(wxCAL_SHOW_SURROUNDING_WEEKS)
        WXS_ST(wxCAL_SEQUENTIAL_MONTH_SELECTION)
    WXS_ST_END()


    WXS_EV_BEGIN(wxsCalendarCtrlEvents)
        WXS_EVI(EVT_CALENDAR_SEL_CHANGED,wxEVT_CALENDAR_SEL_CHANGED,wxCalendarEvent,Changed)
        WXS_EVI(EVT_CALENDAR_DAY,wxEVT_CALENDAR_DAY_CHANGED,wxCalendarEvent,DayChanged)
        WXS_EVI(EVT_CALENDAR_MONTH,wxEVT_CALENDAR_MONTH_CHANGED,wxCalendarEvent,MonthChanged)
        WXS_EVI(EVT_CALENDAR_YEAR,wxEVT_CALENDAR_YEAR_CHANGED,wxCalendarEvent,YearChanged)
        WXS_EVI(EVT_CALENDAR,wxCalendarEvent,wxEVT_CALENDAR_DOUBLECLICKED,DblClicked)
        WXS_EVI(EVT_CALENDAR_WEEKDAY_CLICKED,wxEVT_CALENDAR_WEEKDAY_CLICKED,wxCalendarEvent,WeekdayClicked)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}


wxsCalendarCtrl::wxsCalendarCtrl(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsCalendarCtrlEvents,
        wxsCalendarCtrlStyles)
{}

void wxsCalendarCtrl::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxCalendarCtrl(");
            }
            else
            {
                Code<< _T("Create(");
            }
            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << _T("wxDefaultDateTime") << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetIdName(),false) << _T(");\n");

            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsCalendarCtrl::OnBuildCreatingCode"),Language);
        }
    }
}

wxObject* wxsCalendarCtrl::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxCalendarCtrl* Preview = new wxCalendarCtrl(Parent,GetId(),wxDefaultDateTime,Pos(Parent),Size(Parent),Style());
    return SetupWindow(Preview,Flags);
}

void wxsCalendarCtrl::OnEnumWidgetProperties(long Flags)
{
}

void wxsCalendarCtrl::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/calctrl.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsCalendarCtrl::OnEnumDeclFiles"),Language);
    }
}
