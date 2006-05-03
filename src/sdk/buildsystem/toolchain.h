#ifndef TOOLCHAIN_H
#define TOOLCHAIN_H
#undef new

#include <wx/string.h>

#include "configmanager.h"
#include "buildtool.h"
#include "buildstage.h"

#include <vector>
#include <map>



class ToolChain : public ISerializable, public ID
{
    wxString family;
    wxString title;

    typedef std::vector<BuildStage*> StageVector;
    typedef std::map<wxString, BuildTool*> ToolMap;

    StageVector stages;
    ToolMap tools;

public:
    ToolChain(const wxString& t_family, const wxString& t_id, const wxString& t_title);
    ToolChain();

    virtual wxString SerializeOut() const
    {
        return wxEmptyString;
    };

    virtual void SerializeIn(const wxString& s)
    {
    // nothing here yet?
    };


    void operator+=(BuildTool* bt)
    {
        tools[bt->id()] = bt;
    };

    void operator+=(BuildStage* st)
    {
        stages.push_back(st);
    };

    bool Valid() const {return tools.empty() == 0;};
    operator bool() const {return Valid();};
};


#endif
