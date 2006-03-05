#ifndef PAIRCONTROL_H
#define PAIRCONTROL_H


class wxString;

class PairControl : public wxPanel
{
public:

    typedef enum
    {
        ADD = 1,
        DEL = 2,
        FILESELECT = 4,
        RENAME = 8,
        ALL = 15
    } Flags;


    PairControl::PairControl(wxWindow* parent, const wxString& caption, const wxString& value, int flags = 0);

};

#endif
