/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/app.h>
#include <wx/filename.h>
#endif

//(*AppHeaders
#include <wx/image.h>
//*)

#include "frame.h"

class TestApp : public wxApp
{
public:
    virtual bool OnInit();
    Frame* GetFrame() const { return m_frame; }

private:
    Frame* m_frame;
};

IMPLEMENT_APP(TestApp)
DECLARE_APP(TestApp)

bool TestApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    //*)

    m_frame = new Frame;
    m_frame->Center();
    m_frame->Show();
    if (wxFileName::FileExists(_T("test.h")))
    {
        m_frame->Start(_T("test.h"));
    }

    return wxsOK;
}

void ParserTrace(const wxChar* format, ...)
{
    va_list ap;
    va_start(ap, format);
    const wxString& log = wxString::FormatV(format, ap);
    va_end(ap);
    wxGetApp().GetFrame()->DoLog(log);
}

