/***************************************************************
 * Name:      CppCheck.cpp
 * Purpose:   Code::Blocks CppCheck plugin: main functions
 * Author:    Lieven de Cock (aka killerbot)
 * Created:   12/11/2009
 * Copyright: (c) Lieven de Cock (aka killerbot)
 * License:   GPL
  **************************************************************/
#include "sdk.h"
#ifndef CB_PRECOMP
    #include <wx/arrstr.h>
    #include <wx/dir.h>
    #include <wx/file.h>
    #include <wx/fs_zip.h>
    #include <wx/intl.h>
    #include <wx/menu.h>
    #include <wx/string.h>
    #include <wx/xrc/xmlres.h>

    #include "cbproject.h"
    #include "cbplugin.h"
    #include "configmanager.h"
    #include "manager.h"
    #include "logmanager.h"
    #include "pluginmanager.h"
    #include "projectmanager.h"
    #include "macrosmanager.h"
#endif

#include <wx/busyinfo.h>
#include <wx/filedlg.h>
#include <wx/filefn.h>
#include <wx/utils.h>

#include "tinyxml/tinyxml.h"
#include "filefilters.h"
#include "loggers.h"

#include "CppCheck.h"
#include "CppCheckListLog.h"
#include "ConfigPanel.h"

// Register the plugin
namespace
{
    PluginRegistrant<CppCheck> reg(_T("CppCheck"));
};

namespace
{
    bool CheckRequirements()
    {
        cbProject* Project = Manager::Get()->GetProjectManager()->GetActiveProject();
        // if no project open, exit
        if (!Project)
        {
            wxString msg = _("You need to open a project\nbefore using the plugin!");
            cbMessageBox(msg, _("Error"), wxICON_ERROR | wxOK, Manager::Get()->GetAppWindow());
            Manager::Get()->GetLogManager()->DebugLog(msg);
            return false;
        }
        return true;
    }
}; // namespace


CppCheck::CppCheck()
{
    if (!Manager::LoadResource(_T("CppCheck.zip")))
        NotifyMissingFile(_T("CppCheck.zip"));

    m_LogPageIndex = 0; // good init value ???
    m_CppCheckLog = 0;
    m_ListLog = 0;
    m_ListLogPageIndex = 0;
}

CppCheck::~CppCheck()
{
}

void CppCheck::OnAttach()
{
    // do whatever initialization you need for your plugin
    // NOTE: after this function, the inherited member variable
    // IsAttached() will be TRUE...
    // You should check for it in other functions, because if it
    // is FALSE, it means that the application did *not* "load"
    // (see: does not need) this plugin...
    if (LogManager* LogMan = Manager::Get()->GetLogManager())
    {
        m_CppCheckLog = new TextCtrlLogger();
        m_LogPageIndex = LogMan->SetLog(m_CppCheckLog);
        LogMan->Slot(m_LogPageIndex).title = _("CppCheck");
        CodeBlocksLogEvent evtAdd1(cbEVT_ADD_LOG_WINDOW, m_CppCheckLog, LogMan->Slot(m_LogPageIndex).title);
        Manager::Get()->ProcessEvent(evtAdd1);

        wxArrayString Titles; wxArrayInt Widths;
        Titles.Add(_("File"));    Widths.Add(128);
        Titles.Add(_("Line"));    Widths.Add(48);
        Titles.Add(_("Message")); Widths.Add(640);
        m_ListLog = new CppCheckListLog(Titles, Widths);

        m_ListLogPageIndex = LogMan->SetLog(m_ListLog);
        LogMan->Slot(m_ListLogPageIndex).title = _("CppCheck messages");
        CodeBlocksLogEvent evtAdd2(cbEVT_ADD_LOG_WINDOW, m_ListLog, LogMan->Slot(m_ListLogPageIndex).title);
        Manager::Get()->ProcessEvent(evtAdd2);
    }
}

void CppCheck::OnRelease(bool /*appShutDown*/)
{
    // do de-initialization for your plugin
    // if appShutDown is false, the plugin is unloaded because Code::Blocks is being shut down,
    // which means you must not use any of the SDK Managers
    // NOTE: after this function, the inherited member variable
    // IsAttached() will be FALSE...
    if (Manager::Get()->GetLogManager())
    {
        if (m_CppCheckLog)
        {
            CodeBlocksLogEvent evt(cbEVT_REMOVE_LOG_WINDOW, m_CppCheckLog);
            Manager::Get()->ProcessEvent(evt);
        }
        if (m_ListLog)
        {
            CodeBlocksLogEvent evt(cbEVT_REMOVE_LOG_WINDOW, m_ListLog);
            m_ListLog->DestroyControls();
            Manager::Get()->ProcessEvent(evt);
        }
    }
    m_CppCheckLog = 0;
    m_ListLog = 0;
}

void CppCheck::WriteToLog(const wxString& Text)
{
    m_CppCheckLog->Clear();
    AppendToLog(Text);
}

void CppCheck::AppendToLog(const wxString& Text)
{
    if (LogManager* LogMan = Manager::Get()->GetLogManager())
    {
        CodeBlocksLogEvent evtSwitch(cbEVT_SWITCH_TO_LOG_WINDOW, m_CppCheckLog);
        Manager::Get()->ProcessEvent(evtSwitch);
        LogMan->Log(Text, m_LogPageIndex);
    }
}

bool CppCheck::DoCppCheckVersion()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("cppcheck"));
    wxString CommandLine = (cfg ? (cfg->Read(_T("cppcheck_app"),
#ifdef __WXMSW__
                                             _T("cppcheck.exe")) )
#else
                                             _T("cppcheck")) )
#endif
                                : _T("cppcheck"))
                         + _T(" --version");

    WriteToLog(CommandLine);
    wxArrayString Output, Errors;
    long pid = wxExecute(CommandLine, Output, Errors);
    if (pid==-1)
    {
        cbMessageBox(_("Failed to launch cppcheck.\nPlease setup the cppcheck executable accordingly in the settings."),
                     _("Error"), wxICON_ERROR | wxOK, Manager::Get()->GetAppWindow());
        return false;
    }

    int Count = Output.GetCount();
    for (int idxCount = 0; idxCount < Count; ++idxCount)
        AppendToLog(Output[idxCount]);

    Count = Errors.GetCount();
    for (int idxCount = 0; idxCount < Count; ++idxCount)
        AppendToLog(Errors[idxCount]);

    // and clear the list
    m_ListLog->Clear();
    return true;
}


int CppCheck::Execute()
{
    if (!CheckRequirements() || !DoCppCheckVersion())
        return -1;

    cbProject* Project = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (Project->GetFilesCount() < 1)
        return 0;

    const wxString Basepath = Project->GetBasePath();
    AppendToLog(_T("Switching working directory to : ") + Basepath);
    ::wxSetWorkingDirectory(Basepath);

    wxFile Input;
    const wxString InputFileName = _T("CppCheckInput.txt");
    if (!Input.Create(InputFileName, true))
        return -1;

    for (FilesList::iterator it = Project->GetFilesList().begin(); it != Project->GetFilesList().end(); ++it)
    {
        ProjectFile* pf = *it;
        // filter to avoid including non C/C++ files
        if (   pf->relativeFilename.EndsWith(FileFilters::C_DOT_EXT)
            || pf->relativeFilename.EndsWith(FileFilters::CPP_DOT_EXT)
            || pf->relativeFilename.EndsWith(FileFilters::CC_DOT_EXT)
            || pf->relativeFilename.EndsWith(FileFilters::CXX_DOT_EXT)
            || pf->relativeFilename.EndsWith(FileFilters::CPLPL_DOT_EXT)
            || (FileTypeOf(pf->relativeFilename) == ftHeader) )
        {
            Input.Write(pf->relativeFilename + _T("\n"));
        }
    }
    Input.Close();

    MacrosManager*      MacrosMgr = Manager::Get()->GetMacrosManager();
    ProjectBuildTarget* Target    = Project->GetBuildTarget(Project->GetActiveBuildTarget());

    // project include dirs
    wxString IncludeList;
    const wxArrayString& IncludeDirs = Project->GetIncludeDirs();
    for (unsigned int Dir = 0; Dir < IncludeDirs.GetCount(); ++Dir)
    {
        wxString IncludeDir(IncludeDirs[Dir]);
        if (Target)
            MacrosMgr->ReplaceMacros(IncludeDir, Target);
        else
            MacrosMgr->ReplaceMacros(IncludeDir);
        IncludeList += _T("-I\"") + IncludeDir + _T("\" ");
    }
    if (Target)
    {
        // target include dirs
        const wxArrayString& targetIncludeDirs = Target->GetIncludeDirs();
        for (unsigned int Dir = 0; Dir < targetIncludeDirs.GetCount(); ++Dir)
        {
            wxString IncludeDir(targetIncludeDirs[Dir]);
            MacrosMgr->ReplaceMacros(IncludeDir, Target);
            IncludeList += _T("-I\"") + IncludeDir + _T("\" ");
        }
    }

    // project #defines
    wxString DefineList;
    const wxArrayString& Defines = Project->GetCompilerOptions();
    for (unsigned int Opt = 0; Opt < Defines.GetCount(); ++Opt)
    {
        wxString Define(Defines[Opt]);
        if (Target)
            MacrosMgr->ReplaceMacros(Define, Target);
        else
            MacrosMgr->ReplaceMacros(Define);

        if ( Define.StartsWith(_T("-D")) )
            DefineList += Define + _T(" ");
    }
    if (Target)
    {
        // target #defines
        const wxArrayString& targetDefines = Target->GetCompilerOptions();
        for (unsigned int Opt = 0; Opt < targetDefines.GetCount(); ++Opt)
        {
            wxString Define(targetDefines[Opt]);
            MacrosMgr->ReplaceMacros(Define, Target);

            if ( Define.StartsWith(_T("-D")) )
                DefineList += Define + _T(" ");
        }
    }

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("cppcheck"));
    wxString CommandLine = (cfg ? (cfg->Read(_T("cppcheck_app"),
#ifdef __WXMSW__
                                             _T("cppcheck.exe")) )
#else
                                             _T("cppcheck")) )
#endif
                                : _T("cppcheck"))
                         + _T(" ")
                         + cfg->Read(_T("cppcheck_args"), _T("--verbose --enable=all --enable=style --xml"))
                         + _T(" --file-list=") + InputFileName;
    if (!IncludeList.IsEmpty())
        CommandLine += _T(" ") + IncludeList.Trim() + _T(" ") + DefineList.Trim();
    AppendToLog(CommandLine);

    wxArrayString Output, Errors;
    {
        wxWindowDisabler disableAll;
        wxBusyInfo running(_("Running cppcheck... please wait (this may take several minutes)..."),
                           Manager::Get()->GetAppWindow());
        const long pid = wxExecute(CommandLine, Output, Errors, wxEXEC_SYNC);
        if (pid==-1)
        {
            wxString msg = _("Failed to launch cppcheck.\nMake sure the application is in the path!");
            AppendToLog(msg);
            cbMessageBox(msg, _("Error"), wxICON_ERROR | wxOK, Manager::Get()->GetAppWindow());
            ::wxRemoveFile(InputFileName);
            return -1;
        }
    } // end lifetime of wxWindowDisabler, wxBusyInfo
    ::wxRemoveFile(InputFileName);

    for (size_t idxCount = 0; idxCount < Output.GetCount(); ++idxCount)
        AppendToLog(Output[idxCount]);

    wxString Xml;
    for (size_t idxCount = 0; idxCount < Errors.GetCount(); ++idxCount)
    {
        AppendToLog(Errors[idxCount]);
        Xml += Errors[idxCount];
    }

    TiXmlDocument Doc;
    Doc.Parse(Xml.ToAscii());
    if (Doc.Error())
    {
        wxString msg = _("Failed to parse cppcheck XML file.\nProbably it's not produced correctly.");
        AppendToLog(msg);
        cbMessageBox(msg, _("Error"), wxICON_ERROR | wxOK, Manager::Get()->GetAppWindow());
    }
    else
    {
        bool ErrorsPresent = false;
        TiXmlHandle Handle(&Doc);
        Handle = Handle.FirstChildElement("results");
        for (const TiXmlElement* Error = Handle.FirstChildElement("error").ToElement(); Error;
                Error = Error->NextSiblingElement("error"))
        {
            wxString File;
            if (const char* FileValue = Error->Attribute("file"))
                File = wxString::FromAscii(FileValue);
            wxString Line;
            if (const char* LineValue = Error->Attribute("line"))
                Line = wxString::FromAscii(LineValue);
            wxString Id;
            if (const char* IdValue = Error->Attribute("id"))
                Id = wxString::FromAscii(IdValue);
            wxString Severity;
            if (const char* SeverityValue = Error->Attribute("severity"))
                Severity = wxString::FromAscii(SeverityValue);
            wxString Message;
            if (const char* MessageValue = Error->Attribute("msg"))
                Message = wxString::FromAscii(MessageValue);
            const wxString FulllMessage = Id + _T(" : ") + Severity + _T(" : ") + Message;
            if (!File.IsEmpty() && !Line.IsEmpty() && !FulllMessage.IsEmpty())
            {
                wxArrayString Arr;
                Arr.Add(File);
                Arr.Add(Line);
                Arr.Add(FulllMessage);
                m_ListLog->Append(Arr);
                ErrorsPresent = true;
            }
        }
        if (ErrorsPresent)
        {
            if (Manager::Get()->GetLogManager())
            {
                CodeBlocksLogEvent evtSwitch(cbEVT_SWITCH_TO_LOG_WINDOW, m_ListLog);
                Manager::Get()->ProcessEvent(evtSwitch);
            }
        }

        Doc.SaveFile("CppCheckResults.xml");
    }

    return 0;
}

cbConfigurationPanel* CppCheck::GetConfigurationPanel(wxWindow* parent)
{
    // Called by plugin manager to show config panel in global Setting Dialog
    if ( !IsAttached() )
        return NULL;

    return new ConfigPanel(parent);
}
