#ifndef BUILDTOOLMAN_H
#define BUILDTOOLMAN_H

#undef new
#include <vector>
#include <map>

#include "tidobject.h"
#include "buildsystem.h"



class BuildToolManager : public Mgr<BuildToolManager>
{

public:
    void InitBuildTools();                          // load info from config file

    bool AutoDetectBuildTool(const wxString& id);   // "reset defaults" for a specific BuildTool
    void DetectBuildTools();                        // parse and detect all

    void BailOut();
};




#endif
