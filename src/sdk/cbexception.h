#ifndef CBEXCEPTION_H
#define CBEXCEPTION_H

#include <wx/intl.h> // for _() and _T()
#include <wx/log.h> // for wxSafeShowMessage()
#include <wx/msgdlg.h> // for wxMessageBox()

#include "configmanager.h"

/**
@brief Code::Blocks error handling unit.

Two macros are defined to help error handling in Code::Blocks:
cbThrow() and cbAssert().
@note cbAssert() does nothing in release mode. For debug mode, compile with -DcbDEBUG
*/

class cbException
{
	public:
		cbException(const wxString& msg, const wxString& file, int line)
            : Message(msg),
            File(file),
            Line(line)
		{
		}
		virtual ~cbException(){}
		void ShowErrorMessage(bool safe = true)
		{
			wxString gccvers;
			#ifdef __GNUC__
			gccvers.Printf(_T("gcc %d.%d.%d"), __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
			#endif
            wxString title = _("Exception");
		    wxString err;
		    err.Printf(_("An exception has been raised!\n\n"
                        "The application encountered an error at %s, on line %d.\n"
                        "The error message is:\n\n%s\n\n"
                        "Code::Blocks Version revision %d (%s, "
                        "build: %s %s)"),
                        File.c_str(), Line, Message.c_str(),
                        ConfigManager::GetRevisionNumber(), gccvers.c_str(),
                        wxT(__DATE__), wxT(__TIME__));
            if (safe)
                wxSafeShowMessage(title, err);
            else
                wxMessageBox(err, title, wxICON_ERROR);
		}

        // public members
		wxString Message;
		wxString File;
		int Line;
};

#ifdef wxUSE_UNICODE
    #define cbThrow(message) throw cbException(message, _U(__FILE__), __LINE__)
#else
    #define cbThrow(message) throw cbException(message, __FILE__, __LINE__)
#endif

#ifndef cbDEBUG
    #define cbAssert(expr)
#else
    // In unix we use kill to terminate the application, that makes gdb
    // keep it alive which makes debugging easier.
    // (thanks go to an unknown author)
    #ifdef __WXMSW__
        #define DIE() exit(1)
    #else
        #include <csignal>
        #define DIE() kill(0, SIGTERM)
    #endif

    #define cbAssert(expr) \
        if (!(expr)) \
        { \
            wxString err; \
            err.Printf(_("Assertion %s failed (%s:%d)"), #expr, _U(__FILE__).c_str(), __LINE__); \
            wxSafeShowMessage(_("Assertion error"), err); \
            DIE(); \
        }
#endif

#endif // CBEXCEPTION_H
