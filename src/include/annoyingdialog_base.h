/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "settings.h"
//#ifndef CB_FOR_CONSOLE
//    #include "scrollingdialog.h"
//#endif // #ifndef CB_FOR_CONSOLE
#include <wx/string.h>
#include <wx/artprov.h>

//#ifdef CB_FOR_CONSOLE
//    #define ANNOYINGDIALOG_BASE AnnoyingDialog
//#else // #ifdef CB_FOR_CONSOLE
//    #define ANNOYINGDIALOG_BASE AnnoyingDialogBase
//#endif //#ifdef CB_FOR_CONSOLE

//class wxCheckBox;

/**
Dialog that contains a "Don't annoy me" checkbox.

Using this dialog, the user can select not to display this dialog again.
The dialog can be then re-enabled in "Settings->Environment->Dialogs"...
*/
//#ifndef CB_FOR_CONSOLE
//class DLLIMPORT ANNOYINGDIALOG_BASE : public wxScrollingDialog
//#else// #ifndef CB_FOR_CONSOLE
class DLLIMPORT AnnoyingDialog
//#endif // #ifndef CB_FOR_CONSOLE
{
    public:
        enum dStyle
        {
            OK,
            YES_NO,
            YES_NO_CANCEL,
            OK_CANCEL,
            ONE_BUTTON,
            TWO_BUTTONS,
            THREE_BUTTONS
        };

        AnnoyingDialog(const wxString& caption, const wxString& message, const wxArtID icon = wxART_INFORMATION,
                       dStyle style = YES_NO, int defaultReturn = wxID_YES, bool separate = true,
                       const wxString& b1 = wxEmptyString, const wxString& b2 = wxEmptyString, const wxString& b3 = wxEmptyString);
        virtual ~AnnoyingDialog(){}
        virtual int ShowModal();
    private:
//#ifndef CB_FOR_CONSOLE
//        void OnButton(wxCommandEvent& event);
//        wxCheckBox *cb;
//#endif // #ifndef CB_FOR_CONSOLE
        bool dontAnnoy;
        int defRet;
//#ifndef CB_FOR_CONSOLE
//        DECLARE_EVENT_TABLE()
//#else // #ifndef CB_FOR_CONSOLE
        wxString m_Message;
        wxString m_Caption;
//#endif // #ifndef CB_FOR_CONSOLE

};
