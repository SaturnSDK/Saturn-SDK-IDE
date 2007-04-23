/*
	This file is part of Code Snippets, a plugin for Code::Blocks
	Copyright (C) 2006 Arto Jonsson
	Copyright (C) 2007 Pecan Heber

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
// RCS-ID: $Id: messageBoxForm.cpp 58 2007-04-22 04:40:57Z Pecan $

#ifdef WX_PRECOMP
    #include "wx_pch.h"
#else
#endif


#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "messageBoxForm.h"

///////////////////////////////////////////////////////////////////////////

messageBoxForm::messageBoxForm( wxWindow* parent, int id, wxString title,
            wxPoint pos, wxSize size, int dlgStyle, int txtStyle )
                    : wxDialog( parent, id, title, pos, size, dlgStyle )
{
	wxStaticBoxSizer* sbSizer;
	sbSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("") ), wxVERTICAL );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	//m_messageBoxTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT("messageBox Text Ctrl"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_MULTILINE|wxTE_READONLY|wxNO_BORDER );
	m_messageBoxTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT("messageBox Text Ctrl"), wxDefaultPosition, wxDefaultSize, txtStyle );
	bSizer1->Add( m_messageBoxTextCtrl, 1, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	//m_staticline = new wxStaticLine( this, ID_DEFAULT, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	//bSizer1->Add( m_staticline, 0, wxALL, 5 );

	sbSizer->Add( bSizer1, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
    m_pYesButton = m_pNoButton = m_pOkButton = m_pCancelButton = 0;

	m_sdbSizer = new wxStdDialogButtonSizer();
	if (dlgStyle & wxYES)
	m_sdbSizer->AddButton( m_pYesButton = new wxButton( this, wxID_YES ) );
	if (dlgStyle & wxNO)
	m_sdbSizer->AddButton( m_pNoButton = new wxButton( this, wxID_NO ) );
	if (dlgStyle & wxOK)
	m_sdbSizer->AddButton( m_pOkButton = new wxButton( this, wxID_OK ) );
	if (dlgStyle & wxCANCEL)
	m_sdbSizer->AddButton( m_pCancelButton = new wxButton( this, wxID_CANCEL ) );
	m_sdbSizer->Realize();
	bSizer3->Add( m_sdbSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	sbSizer->Add( bSizer3, 0, wxEXPAND, 5 );

	this->SetSizer( sbSizer );
	this->Layout();
}
