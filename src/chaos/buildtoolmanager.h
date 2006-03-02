#ifndef BUILDTOOLMAN_H
#define BUILDTOOLMAN_H


class TIDObject
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


class BuildTool : public TIDObject
{
    wxString executable;

public:
    BuildTool(const wxString& exe) : TIDObject(title, id), executable(exe) {};
    bool operator==(const BuildTool& compare) const { return GetID() == compare.GetID(); };
    wxString GetExecutable() const { return executable; };
    wxString operator(wxString)() const { return GetExecutable(); };
};


class Stage
{
...
};

class Stage
{
BuildTool *tool;
wxString toolID;
wxString DisplayName();

Stage() : tool(0) {};

BuildTool* Tool()
  {
  if(tool = 0)
    tool = uhhh.... GetTool(toolID)

  return tool;
  }
};



class BuildFamily : public TIDObject
{
    Stage   stage[4];

    struct
    {
        stage_t stage;
        wxArrayString
    };

    friend class BuildToolManager;
    BuildFamily() : TIDObject(title, id){};
    AddGroup(const wxString& stages, const wxString& extensions);

};





class BuildToolManager : public Mgr<BuildToolManager>
{
    typedef std::map<wxString, BuildTool*> ToolMap;

public:
    void InitBuildTools();                          // load info from config file

    bool AutoDetectBuildTool(const wxString& id);   // "reset defaults" for a specific BuildTool
    void DetectBuildTools();                        // parse and detect all

    void BailOut();
};




#endif
