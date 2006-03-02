#ifndef TIDOBJECT_H
#define TIDOBJECT_H

#include <wx/string.h>

class TIDObject  //  Base class for an object that has a title and an ID
{
    wxString title;
    wxString id;

public:
    TIDObject(const wxString& xtitle, const wxString& xid) : title(xtitle), id(xid){};

    wxString SetID(const wxString& arg){id = arg;};
    wxString SetTitle(const wxString& arg){title = arg;};
    wxString GetID() const{return id;};
    wxString GetTitle() const{return title;};
};


#endif
