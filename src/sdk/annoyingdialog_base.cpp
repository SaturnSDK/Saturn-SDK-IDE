/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6392 $
 * $Id: annoyingdialog.cpp 6392 2010-07-12 08:38:40Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/annoyingdialog.cpp $
 */

#include "sdk_precomp.h"
#ifndef CB_PRECOMP
#include "cbexception.h"
#include "configmanager.h"
#include "manager.h"
#include "globals.h"
#endif
#include "annoyingdialog_base.h"

AnnoyingDialog::AnnoyingDialog(const wxString& caption, const wxString& message, const wxArtID icon,
                               dStyle style, int defaultReturn, bool /*separate*/,
                               const wxString& b1, const wxString& b2, const wxString& b3)
        :
        dontAnnoy(false),
        defRet(defaultReturn)
{
	// Code::Blocks needs wxWidgets 2.8
	CompileTimeAssertion<wxMinimumVersion<2,8>::eval>::Assert();

    ConfigManagerContainer::StringSet disabled = Manager::Get()->GetConfigManager(_T("an_dlg"))->ReadSSet(_T("/disabled"));
    if(disabled.find(caption) != disabled.end())
    {
        dontAnnoy = true;
        return;
    }

    m_Message = message;
    m_Caption = caption;
}

int AnnoyingDialog::ShowModal()
{
    if(dontAnnoy)
        return defRet;
    cbMessageBox(m_Message, m_Caption);
    return wxID_OK;
};
