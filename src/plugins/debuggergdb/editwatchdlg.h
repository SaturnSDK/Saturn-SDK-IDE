/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef EDITWATCHDLG_H
#define EDITWATCHDLG_H

#include "scrollingdialog.h"

class GDBWatch;

class EditWatchDlg : public wxScrollingDialog
{
    public:
        EditWatchDlg(GDBWatch* w = nullptr, wxWindow* parent = nullptr);
        virtual ~EditWatchDlg();

//        virtual const GDBWatch& GetWatch(){ return m_watch; }
    protected:
        void EndModal(int retCode);

        GDBWatch *m_watch;
};

#endif // EDITWATCHDLG_H
