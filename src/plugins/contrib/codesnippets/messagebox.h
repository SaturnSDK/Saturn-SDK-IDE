/*
	This file is part of SnipList, program to store information snippets.
	Copyright (C) 2006 Pecan Heber

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef myMessageBox_H
#define myMessageBox_H

#if defined(__GNUG__) && !defined(__APPLE__)
	#pragma interface "dragscroll.h"
#endif
// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/msgdlg.h>
#include <wx/textctrl.h>

#include "version.h"


int messageBox( const wxString& message, const wxString& title=wxEmptyString,
                long dialogStyle=wxOK,
                long textStyle=wxTE_CENTRE|wxTE_MULTILINE|wxTE_READONLY );

#endif // myMessageBox_H
