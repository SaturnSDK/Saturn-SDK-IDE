/*
* This file is part of Code::Blocks, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision
* $Id
* $HeadURL
*/

#include "sdk_precomp.h"
#include "projectloader_hooks.h"

#include <map>

typedef std::map<int, ProjectLoaderHooks::HookFunctorBase*> HookFunctorsMap;
static HookFunctorsMap s_HookFunctorsMap;
static int s_UniqueID = 0;

int ProjectLoaderHooks::RegisterHook(ProjectLoaderHooks::HookFunctorBase* functor)
{
    for (HookFunctorsMap::iterator it = s_HookFunctorsMap.begin(); it != s_HookFunctorsMap.end(); ++it)
    {
        if (it->second == functor)
            return it->first;
    }
    s_HookFunctorsMap[s_UniqueID] = functor;
    return s_UniqueID++;
}

ProjectLoaderHooks::HookFunctorBase* ProjectLoaderHooks::UnregisterHook(int id, bool deleteHook)
{
    HookFunctorsMap::iterator it = s_HookFunctorsMap.find(id);
    if (it != s_HookFunctorsMap.end())
    {
        ProjectLoaderHooks::HookFunctorBase* functor = it->second;
        s_HookFunctorsMap.erase(it);
        if (deleteHook)
        {
            delete functor;
            return 0;
        }
        return functor;
    }
    return 0;
}

bool ProjectLoaderHooks::HasRegisteredHooks()
{
    return s_HookFunctorsMap.size() != 0;
}

void ProjectLoaderHooks::CallHooks(cbProject* project, TiXmlElement* elem, bool isLoading)
{
    for (HookFunctorsMap::iterator it = s_HookFunctorsMap.begin(); it != s_HookFunctorsMap.end(); ++it)
    {
        ProjectLoaderHooks::HookFunctorBase* functor = it->second;
        if (functor)
            functor->Call(project, elem, isLoading);
    }
}
