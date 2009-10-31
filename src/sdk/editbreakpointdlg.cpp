/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"
#ifndef CB_PRECOMP
    #include <wx/xrc/xmlres.h>

    #include "debuggermanager.h"
#endif

#include "editbreakpointdlg.h"
#include <wx/intl.h>
#include <wx/xrc/xmlres.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>

BEGIN_EVENT_TABLE(EditBreakpointDlg, wxScrollingDialog)
    EVT_UPDATE_UI(-1, EditBreakpointDlg::OnUpdateUI)
END_EVENT_TABLE()

EditBreakpointDlg::EditBreakpointDlg(cbBreakpoint* breakpoint, wxWindow* parent)
    : m_breakpoint(breakpoint)
{
    //ctor
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgEditBreakpoint"),_T("wxScrollingDialog"));

    XRCCTRL(*this, "chkEnabled", wxCheckBox)->SetValue(breakpoint->IsEnabled());
    XRCCTRL(*this, "chkIgnore", wxCheckBox)->SetValue(breakpoint->UseIgnoreCount());
    XRCCTRL(*this, "spnIgnoreCount", wxSpinCtrl)->SetValue(breakpoint->GetIgnoreCount());
    XRCCTRL(*this, "chkExpr", wxCheckBox)->SetValue(breakpoint->UseCondition());
    XRCCTRL(*this, "txtExpr", wxTextCtrl)->SetValue(breakpoint->GetCondition());
}

EditBreakpointDlg::~EditBreakpointDlg()
{
    //dtor
}

void EditBreakpointDlg::EndModal(int retCode)
{
    if (retCode == wxID_OK)
    {
        m_breakpoint->SetEnabled(XRCCTRL(*this, "chkEnabled", wxCheckBox)->GetValue());
        m_breakpoint->SetUseIgnoreCount(XRCCTRL(*this, "chkIgnore", wxCheckBox)->IsChecked());
        m_breakpoint->SetIgnoreCount(XRCCTRL(*this, "spnIgnoreCount", wxSpinCtrl)->GetValue());
        m_breakpoint->SetUseCondition(XRCCTRL(*this, "chkExpr", wxCheckBox)->IsChecked());
        m_breakpoint->SetCondition(XRCCTRL(*this, "txtExpr", wxTextCtrl)->GetValue());
    }
    wxScrollingDialog::EndModal(retCode);
}

void EditBreakpointDlg::OnUpdateUI(wxUpdateUIEvent& event)
{
    bool en = XRCCTRL(*this, "chkEnabled", wxCheckBox)->IsChecked();
    XRCCTRL(*this, "chkIgnore", wxCheckBox)->Enable(en && !XRCCTRL(*this, "chkExpr", wxCheckBox)->IsChecked());
    XRCCTRL(*this, "spnIgnoreCount", wxSpinCtrl)->Enable(en && XRCCTRL(*this, "chkIgnore", wxCheckBox)->IsChecked());
    XRCCTRL(*this, "chkExpr", wxCheckBox)->Enable(en && !XRCCTRL(*this, "chkIgnore", wxCheckBox)->IsChecked());
    XRCCTRL(*this, "txtExpr", wxTextCtrl)->Enable(en && XRCCTRL(*this, "chkExpr", wxCheckBox)->IsChecked());
}
