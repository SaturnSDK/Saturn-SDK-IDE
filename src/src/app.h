/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CODEBLOCKS_APP_H
#define CODEBLOCKS_APP_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if !defined(WX_PRECOMP)
    #include <wx/wx.h>
    #include <wx/intl.h>
    #include <wx/laywin.h>
    #include <wx/image.h>
    #include <wx/filename.h>
    #include <wx/hashmap.h>
    #include <wx/docview.h> // recent files history
#endif

#ifndef CB_FOR_CONSOLE
    #if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "resources/icons/app.xpm"
    #endif

    #include <wx/taskbar.h>
    #include <wx/splash.h>
#endif // #ifndef CB_FOR_CONSOLE

#include <wx/snglinst.h>
#include "manager.h"
#ifndef CB_FOR_CONSOLE
    #include "main.h"
#else // #ifndef CB_FOR_CONSOLE
    #include "cbplugin.h"
    #include "cbproject.h"
#endif // #ifndef CB_FOR_CONSOLE

#ifndef CB_FOR_CONSOLE
class cbSplashScreen;

class CodeBlocksApp : public wxApp
#else // #ifndef CB_FOR_CONSOLE
class CodeBlocksApp : public wxAppConsole
#endif // #ifndef CB_FOR_CONSOLE
{
    public:
        virtual bool OnInit();
        virtual int OnExit();
        virtual int OnRun();
        wxString GetAppPath() const;
#ifndef CB_FOR_CONSOLE
        int ParseCmdLine(MainFrame* handlerFrame);
#else // #ifndef CB_FOR_CONSOLE
        int ParseCmdLine();
#endif // #ifndef CB_FOR_CONSOLE
        void OnAppActivate(wxActivateEvent& event);
        bool OnCmdLineParsed(wxCmdLineParser& parser);
        void OnFatalException();
#ifndef CB_FOR_CONSOLE
        void LoadDelayedFiles(MainFrame* frame); // command line or DDE (if available) files
#endif // #ifndef CB_FOR_CONSOLE
        void SetAutoFile(wxString& file); // method to set m_AutoFile
#ifdef __WXMAC__
        // in response of an open-document apple event
        virtual void         MacOpenFile(const wxString &fileName) ;
        // in response of a print-document apple event
        virtual void         MacPrintFile(const wxString &fileName) ;
#endif
    protected:
        void OnBatchBuildDone(CodeBlocksEvent& event);
#ifndef CB_FOR_CONSOLE
        void OnTBIconLeftDown(wxTaskBarIconEvent& event);
#endif // #ifndef CB_FOR_CONSOLE
        bool LoadConfig();
        void InitDebugConsole();
        void InitExceptionHandler();
        bool InitXRCStuff();
#ifndef CB_FOR_CONSOLE
        MainFrame* InitFrame();
#endif // #ifndef CB_FOR_CONSOLE
        void CheckVersion();
        void InitLocale();
        int BatchJob();
        wxLocale m_locale; // locale we'll be using
    private:
        void SetupPersonality(const wxString& personality);


        wxString m_Prefix; // --prefix switch
        wxString m_BatchTarget;
        wxString m_Script;
        wxString m_AutoFile; // --file foo.cpp[:line]

#ifndef CB_FOR_CONSOLE
        wxScrollingDialog* m_pBatchBuildDialog;
        MainFrame* m_Frame;
#endif // #ifndef CB_FOR_CONSOLE
        wxSingleInstanceChecker* m_pSingleInstance;

        bool m_Batch;
        bool m_BatchNotify;
        bool m_BatchWindowAutoClose; // default: true
        bool m_Build;
        bool m_ReBuild;
        bool m_Clean;
        bool m_HasProject;
        bool m_HasWorkSpace;
        bool m_Splash; // splash screen enabled
        bool m_HasDebugLog; // display debug log
        bool m_CrashHandler; // crash handler enabled
        bool m_SafeMode; // all plugins disabled

        void InitAssociations();

        bool m_DDE; // DDE enabled?
#ifdef __WXMSW__
        bool m_Assocs; // associations check enabled
        HINSTANCE m_ExceptionHandlerLib;
#endif
        int m_BatchExitCode;

        DECLARE_EVENT_TABLE()
};

DECLARE_APP(CodeBlocksApp);

#endif // CODEBLOCKS_APP_H
