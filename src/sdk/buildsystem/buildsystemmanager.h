#ifndef BSMAN_H
#define BSMAN_H
#undef new

#include <tinyxml/tinywxuni.h>
#include "toolchain.h"
#include "buildtool.h"
#include "configmanager.h"

class Behaviour : public ISerializable
{
public:
    virtual wxString SerializeOut() const {return wxEmptyString;};
    virtual void SerializeIn(const wxString& s) {};
};

class BuildSystemManager : public Mgr<BuildSystemManager>
{
typedef std::map<wxString, ToolChain*> toolchainMap;
typedef std::map<wxString, Behaviour*> behaviourMap;

//typedef std::vector<Stage> StageVector;

bool feelgood;

bool BuildSystemManager::DetectCompilers(const wxString& dir);
bool BuildSystemManager::ParseBehaviours(const wxString& dir);

toolchainMap toolchains;
behaviourMap behaviours;

public:
    BuildSystemManager();

    BuildTool* DetectBuildTool(const wxString& new_id, const wxString& the_family, const wxString& location, const wxString& executable, const wxString& challenge, const wxString& response);

    void BailOut() const;

};





#endif
