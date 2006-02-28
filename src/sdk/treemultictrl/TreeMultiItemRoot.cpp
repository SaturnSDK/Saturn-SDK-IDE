//---------------------------------------------------------------------------
// $RCSfile$
// $Source$
// $Revision$
// $Date$
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "TreeMultiItemRoot.cpp"
#endif

/* for compilers that support precompilation
   includes "wx/wx.h" */

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "TreeMultiItemRoot.h"

/** TreeMultiItemRoot
 * This class is the class you use to create a tree. It contains all the
 * public methods from a TreeMultiItemNode, but you can add more to it 
 * because it is the root. A typical addition is the load entry for a 
 * directory, or other type of enumeration. You could store the base path
 * of a relative tree in this class.
 */

TreeMultiItemRoot::TreeMultiItemRoot()
	: TreeMultiItemNode(0, _T(""), _T(""))
{
    // TODO: Enter your constructor code here
}

TreeMultiItemRoot::~TreeMultiItemRoot()
{
    // TODO: Enter your destructor code here
}


