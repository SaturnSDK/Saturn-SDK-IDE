#ifndef BUILDSYSTEM_H
#define BUILDSYSTEM_H

#undef new
#include <vector>
#include <map>

#include "tidobject.h"



namespace cbBuildSystem
{
    typedef std::vector<Stage> Stages;
    typedef std::map<wxString, Family> Families;
    typedef std::map<wxString, BuildTool*> ToolMap;


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
        BuildTool *tool;
        wxString toolID;
        wxString DisplayName();

    public:
        Stage() : tool(0) {};
        BuildTool* Tool();
    };



    class Family : public TIDObject
    {
        cbBuildSystem::Stages   stages;

        Family() : TIDObject(title, id){};
        AddGroup(const wxString& stages, const wxString& extensions);
    };

};

#endif
