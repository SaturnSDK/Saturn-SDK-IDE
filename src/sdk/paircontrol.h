#ifndef PAIRCONTROL_H
#define PAIRCONTROL_H


class wxString;

class PairControl : public wxPanel
{
int flags;

wxTextCtrl *m_value;

public:

    typedef enum
    {
        ADD = 1,
        DEL = 2,
        FILESELECT = 4,
        RENAME = 8,
        ALL = 15
    } Flags;


    PairControl(wxWindow* parent, const wxString& caption, const wxString& value, int flags = 0);
    void Enter(wxCommandEvent& event);
    void FS(wxCommandEvent& event);
private:
    DECLARE_EVENT_TABLE()
};



class PairControlPanel : public wxPanel
{
    std::vector<PairControl*> controls;
public:

    PairControlPanel(wxWindow* parent) : wxPanel(parent){};
    void AddPair(PairControl *pc);
    void DeletePair(wxCommandEvent& event);
    void Enter(wxCommandEvent& event);

private:
    DECLARE_EVENT_TABLE()
};

#endif
