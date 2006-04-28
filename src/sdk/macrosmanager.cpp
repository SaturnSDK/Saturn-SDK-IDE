/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/menu.h>

    #include "projectmanager.h"
    #include "editormanager.h"
    #include "messagemanager.h"
    #include "macrosmanager.h"
    #include "manager.h"
    #include "cbproject.h"
    #include "cbeditor.h"
    #include "uservarmanager.h"
    #include "configmanager.h"
    #include "globals.h"
#endif

#include <stdlib.h>

/*
    standard macros are:

    ${PROJECT_FILENAME} ${PROJECT_FILE} ${PROJECTFILE}
    ${PROJECT_NAME}
    ${PROJECT_DIR} ${PROJECTDIR} ${PROJECT_DIRECTORY}
    ${CODEBLOCKS} ${APP_PATH}  ${APPPATH}
    ${DATA_PATH} ${DATAPATH}
    ${PLUGINS}
    ${ACTIVE_EDITOR_FILENAME}
    ${ALL_PROJECT_FILES}
    ${MAKEFILE}
    ${FOO_OUTPUT_FILE} // per target
    ${BAR_OUTPUT_DIR} // per target
    $(TARGET_OUTPUT_DIR) // the current target's out dir
    $(TARGET_NAME)       // the current target's name (title)

    ${AMP} TODO: implement AddMacro() for custom macros (like this)
*/

MacrosManager::MacrosManager()
{
    Reset();
}

MacrosManager::~MacrosManager()
{
}

void MacrosManager::CreateMenu(wxMenuBar* menuBar)
{
}

void MacrosManager::ReleaseMenu(wxMenuBar* menuBar)
{
}

wxString MacrosManager::ReplaceMacros(const wxString& buffer, bool envVarsToo, ProjectBuildTarget* target)
{
    wxString tmp = buffer;
    ReplaceMacros(tmp, envVarsToo, target);
    return tmp;
}

void MacrosManager::Reset()
{
//	Manager::Get()->GetMessageManager()->DebugLog(_T("reset"));
    m_lastProject = 0;
    m_lastTarget = 0;
    m_lastEditor = 0;

    m_AppPath = UnixFilename(ConfigManager::GetExecutableFolder());
    m_DataPath = UnixFilename(ConfigManager::GetDataFolder());
    m_Plugins = UnixFilename(ConfigManager::GetDataFolder() + _T("/plugins"));
    ClearProjectKeys();
    m_re.Compile(_T("(%|\\$[({]?)(#?[A-Za-z_0-9.]+)([)}%/\\]?)"));
    m_uVarMan = Manager::Get()->GetUserVariableManager();
    srand(time(0));
}

void MacrosManager::ClearProjectKeys()
{
//	Manager::Get()->GetMessageManager()->DebugLog(_T("clear"));
    macros.clear();

    macros[_T("AMP")]   = _T("&");
    macros[_T("CODEBLOCKS")] = m_AppPath;
    macros[_T("APP_PATH")]  = m_AppPath;
    macros[_T("APP-PATH")]  = m_AppPath;
    macros[_T("APPPATH")]  = m_AppPath;
    macros[_T("DATA_PATH")]  = m_DataPath;
    macros[_T("DATA-PATH")]  = m_DataPath;
    macros[_T("DATAPATH")]  = m_DataPath;
    macros[_T("PLUGINS")]  = m_Plugins;
	macros[_T("LANGUAGE")]  = wxLocale::GetLanguageName(wxLocale::GetSystemLanguage());
	macros[_T("ENCODING")]  = wxLocale::GetSystemEncodingName();

}

void MacrosManager::RecalcVars(cbProject* project,EditorBase* editor,ProjectBuildTarget* target)
{
//	Manager::Get()->GetMessageManager()->DebugLog(wxString("recalc...  (project == ") << (int) project << _T("   editor == ") << (int)editor << _T("   target == ") << (int)target << ")");
    if(!editor)
    {
        m_ActiveEditorFilename = wxEmptyString;
        m_lastEditor = NULL;
    }
    else if(editor != m_lastEditor)
    {
        m_ActiveEditorFilename = UnixFilename(editor->GetFilename());
        m_lastEditor = editor;
    }
    if(!project)
    {
//		Manager::Get()->GetMessageManager()->DebugLog("project == 0");
        m_ProjectFilename = wxEmptyString;
        m_ProjectName = wxEmptyString;
        m_ProjectDir = wxEmptyString;
        m_ProjectFiles = wxEmptyString;
        m_Makefile = wxEmptyString;
        m_lastProject = NULL;
        ClearProjectKeys();
        macros[_T("PROJECT_FILE")]  = wxEmptyString;
        macros[_T("PROJECT_FILENAME")] = wxEmptyString;
        macros[_T("PROJECT_FILE_NAME")] = wxEmptyString;
        macros[_T("PROJECTFILE")]  = wxEmptyString;
        macros[_T("PROJECTFILENAME")] = wxEmptyString;
        macros[_T("PROJECT_NAME")]  = wxEmptyString;
        macros[_T("PROJECTNAME")]  = wxEmptyString;
        macros[_T("PROJECT_DIR")]  = wxEmptyString;
        macros[_T("PROJECT_DIRECTORY")] = wxEmptyString;
        macros[_T("PROJECTDIR")]  = wxEmptyString;
        macros[_T("PROJECTDIRECTORY")] = wxEmptyString;
        macros[_T("PROJECT_TOPDIR")]  = wxEmptyString;
        macros[_T("PROJECT_TOPDIRECTORY")] = wxEmptyString;
        macros[_T("PROJECTTOPDIR")]  = wxEmptyString;
        macros[_T("PROJECTTOPDIRECTORY")] = wxEmptyString;
        macros[_T("MAKEFILE")]   = wxEmptyString;
        macros[_T("ALL_PROJECT_FILES")] = wxEmptyString;
    }
    else if(project != m_lastProject)
    {
//		Manager::Get()->GetMessageManager()->DebugLog("project != m_lastProject");
        m_lastTarget = NULL; // reset last target when project changes
        m_prjname.Assign(project->GetFilename());
        m_ProjectFilename = UnixFilename(m_prjname.GetFullName());
        m_ProjectName = project->GetTitle();
        m_ProjectDir = UnixFilename(project->GetBasePath());
        m_ProjectTopDir = UnixFilename(project->GetCommonTopLevelPath());
        m_Makefile = UnixFilename(project->GetMakefile());
        m_ProjectFiles = wxEmptyString;
        for (int i = 0; i < project->GetFilesCount(); ++i)
        {
            // quote filenames, if they contain spaces
            wxString out = UnixFilename(project->GetFile(i)->relativeFilename);
            QuoteStringIfNeeded(out);
            m_ProjectFiles << out << _T(' ');
        }

        ClearProjectKeys();
        macros[_T("PROJECT_FILE")]  = m_ProjectFilename;
        macros[_T("PROJECT_FILENAME")] = m_ProjectFilename;
        macros[_T("PROJECT_FILE_NAME")] = m_ProjectFilename;
        macros[_T("PROJECTFILE")]  = m_ProjectFilename;
        macros[_T("PROJECTFILENAME")] = m_ProjectFilename;
        macros[_T("PROJECTNAME")]  = m_ProjectName;
        macros[_T("PROJECT_NAME")]  = m_ProjectName;
        macros[_T("PROJECT_DIR")]  = m_ProjectDir;
        macros[_T("PROJECT_DIRECTORY")] = m_ProjectDir;
        macros[_T("PROJECTDIR")]  = m_ProjectDir;
        macros[_T("PROJECTDIRECTORY")] = m_ProjectDir;
        macros[_T("PROJECT_TOPDIR")]  = m_ProjectTopDir;
        macros[_T("PROJECT_TOPDIRECTORY")] = m_ProjectTopDir;
        macros[_T("PROJECTTOPDIR")]  = m_ProjectTopDir;
        macros[_T("PROJECTTOPDIRECTORY")] = m_ProjectTopDir;
        macros[_T("MAKEFILE")]   = m_Makefile;
        macros[_T("ALL_PROJECT_FILES")] = m_ProjectFiles;

        for (int i = 0; i < project->GetBuildTargetsCount(); ++i)
        {
            ProjectBuildTarget* target = project->GetBuildTarget(i);
            if (!target)
                continue;
            wxString title = target->GetTitle().Upper();
            while (title.Replace(_T(" "), _T("_")))
                ; // replace spaces with underscores (what about other invalid chars?)
            macros[title + _T("_OUTPUT_FILE")] = UnixFilename(target->GetOutputFilename());
            macros[title + _T("_OUTPUT_DIR")] = UnixFilename(target->GetBasePath());
        }
        m_lastProject = project;
    }

	if(project)
	{
        const StringHash& v = project->GetAllVars();
        for (StringHash::const_iterator it = v.begin(); it != v.end(); ++it)
        {
            macros[it->first.Upper()] = it->second;
        }
	}

    if(!target)
    {
        m_TargetOutputDir = wxEmptyString;
        m_TargetName = wxEmptyString;
        m_lastTarget = NULL;
    }
    else if(target != m_lastTarget)
    {
        m_TargetOutputDir = UnixFilename(target->GetBasePath());
        m_TargetName = UnixFilename(target->GetTitle());
        m_lastTarget = target;
    }

	if(target)
	{
        const StringHash& v = target->GetAllVars();
        for (StringHash::const_iterator it = v.begin(); it != v.end(); ++it)
        {
            macros[it->first.Upper()] = it->second;
        }
	}

    macros[_T("TARGET_OUTPUT_DIR")]   = m_TargetOutputDir;
    macros[_T("TARGET_NAME")]    = m_TargetName;
    macros[_T("ACTIVE_EDITOR_FILENAME")] = m_ActiveEditorFilename;

	wxDateTime now(wxDateTime::Now());
	wxDateTime nowGMT(now.ToGMT());

    macros[_T("TDAY")]   = now.Format(_T("%Y%m%d"));
    macros[_T("TODAY")]   = now.Format(_T("%Y-%m-%d"));
    macros[_T("NOW")]   = now.Format(_T("%Y-%m-%d-%H.%M"));
    macros[_T("NOW_L")]   = now.Format(_T("%Y-%m-%d-%H.%M.%S"));
    macros[_T("WEEKDAY")]  = now.Format(_T("%A"));
    macros[_T("TDAY_UTC")]  = nowGMT.Format(_T("%Y%m%d"));
    macros[_T("TODAY_UTC")]  = nowGMT.Format(_T("%Y-%m-%d"));
    macros[_T("NOW_UTC")]  = nowGMT.Format(_T("%Y-%m-%d-%H.%M"));
    macros[_T("NOW_L_UTC")]  = nowGMT.Format(_T("%Y-%m-%d-%H.%M.%S"));
    macros[_T("WEEKDAY_UTC")] = nowGMT.Format(_T("%A"));
}

void MacrosManager::ReplaceMacros(wxString& buffer, bool envVarsToo, ProjectBuildTarget* target)
{
//	Manager::Get()->GetMessageManager()->DebugLog(wxString("ReplaceMacros(\"") << buffer << "\")");
    if (buffer.IsEmpty())
        return;

    cbProject* project = target
                        ? target->GetParentProject()
                        : Manager::Get()->GetProjectManager()->GetActiveProject();
    EditorBase* editor = Manager::Get()->GetEditorManager()->GetActiveEditor();

    if(!target)
    {
        if (project)
        {
            // use the currently compiling target
            target = project->GetCurrentlyCompilingTarget();
            // if none,
            if (!target)
                // use the last known active target
                target = project->GetBuildTarget(project->GetActiveBuildTarget());
        }
    }

    if(project != m_lastProject || target != m_lastTarget || editor != m_lastEditor)
        RecalcVars(project, editor, target);

    wxString search;
    wxString replace;

    while(m_re.Matches(buffer))
    {
        replace.Empty();

        wxString var = m_re.GetMatch(buffer, 2).Upper();
        wxString right = m_re.GetMatch(buffer, 3);
        search = m_re.GetMatch(buffer, 0);

        if (var.GetChar(0) == _T('#'))
        {
            replace = UnixFilename(m_uVarMan->Replace(var));
			QuoteStringIfNeeded(replace);
        }
        else
        {
            if(var.IsSameAs(_T("COIN")))
                replace = rand() & 1 ? _T("1") : _T("0");
            else if(var.IsSameAs(_T("RANDOM")))
                replace = wxString::Format(_T("%d"), rand() & 0xffff);
            else
            {
                MacrosMap::iterator it;
                if((it = macros.find(var)) != macros.end())
                    replace = it->second;
            }
        }


		if(right.size() && (right.GetChar(0) == _T('\\') || right.GetChar(0) == _T('/'))) // workaround for foo\$variable\bar
			replace.append(right);

//		Manager::Get()->GetMessageManager()->DebugLog(wxString(wxString("replacing ") << search << " (variable: " << var << ") " << "with: ") << replace);

        int c = 0;
        if (!replace.IsEmpty())
        {
            c = buffer.Replace(search, replace);
        }
        else
        {
            if (envVarsToo)
            {
                wxString envactual;
                wxGetEnv(var, &envactual);
                c = buffer.Replace(search, envactual);
            }
        }

        if (c == 0)
        {
            buffer.Replace(search, replace);
            break; // avoid infinite loop when macro is invalid
        }
    }
	QuoteStringIfNeeded(replace);

//	Manager::Get()->GetMessageManager()->DebugLog(wxString("ReplaceMacros() ---> return: ") << buffer);
}

void MacrosManager::ReplaceEnvVars(wxString& buffer)
{
    ReplaceMacros(buffer, true);
}
