/*
* This file is part of a wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* wxSmith and this file is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
* 
* Ron Collins
* rcoll@theriver.com
* 12-Feb-2010
* 
*/



==============
wxTreeListCtrl
==============

This package implements the latest version of wxTreeListCtrl (dated 2008-07-21)
written by Ronan Chartois.  Please see the documentation available in the
/docs directory to read the author's original description on how to use
the control.

This package contains the Code::Blocks projects to build a static library for
wxTreeListCtrl, a Code::Blocks plugin so that wxTreeListCtrl may be added to
a GUI at design-time, and a program to test the tree control.



The wxTreeListCtrl build directory has the following structure:
TreeListCtrl\
    bin\                // this dir contains the binary file output of the 
                        // Code::Blocks projects; files include "TestTreeList.exe",
                        // "wxsSmithTreeList.dll",  "wxsSmithTreeList.zip", and 
                        // "CopySmithTreeList.bat"
    docs\               // original documentation from the authour of the
                        // component        
    include\            // the include file "treelistctrl.h" is here
    lib\                // the library "libTreeListCtrl.a" is here
    obj\                // intermediate files for all compiles
    project\            // this is the Code::Bocks project to create the
                        // wxTreeListCtrl library; it creates the file
                        // "libTreeListCtrl.a" and places the library file and
                        // the include file in the proper directories 
    src\                // source files for component wxTreeListCtrl
    test\               // this is the Code::Blocks project to create a test
                        // program for wxTreeListCtrl; the output file
                        // "TestTreeList.exe" is placed in the ..\bin\ directory
        wxsmith\        // used by Code::Blocks and wxSmith
    wxs\                // this is the Code::Blocks project to create the
                        // plugin for Code::Blocks and wxSmith, to put the
                        // component wxTreeListCtrl on the wxSmith palette.
                        // the output files "wxsSmithTreeList.dll" and
                        // and "wxsSmithTreeList.zip" are placed in the ..\bin\
                        // directory; they must be manually copied to the
                        // plugin directory of your Code::Blocks installation.
                        // see the batch file "CopySmithTreeList.bat" to
                        // make this easier  
        images\         // images used on the wxSmith palette 
        src\            // source code for the property pages used by
                        // wxSmith for the wxTreeListCtrl component

The Code::Blocks projects used here were created and tested under Windows XP,
using Code::Blocks SVN version 5866, and wxWidgets version 2.8.  The correct 
order to build these projects is:

1) Build the library (project found in the "project\" directory)
2) Build the wxSmith plugin (project found in the "wxs\" directory)
3) Exit Code::Blocks and install the plugins
   copy "bin\wxsSmithTreeList.dll" to "<Code::Blocks>\share\CodeBlocks\plugins"
   copy "bin\wwxsSmithTreeList.zip" to "<Code::Blocks>\share\CodeBlocks"
4) Re-start Code::Blocks and build the test program (project found in the
   "test\" directory)

(For Windows XP, all of these projects and programs are already compiled; you
only need to install the plugin files into your own Code::Blocks installation
directory).



----------------------   
Properties Description
----------------------

wxTreeListCtrl properties may be modified within the wxSmith GUI editor.  These
properties define the initial control appearance, number of columns of data
to use, initial data values, etc.  After placing a tree control on your form
(or panel, or whatever) and editing the items that show up in the property 
panel, you may see the actual generated code by viewing the *.cpp and *.h files 
associated with that GUI.

Thee are many properties that are common to all controls (for example, size
or position).  These "common" properties are not covered here, but only those
items which are unique or special to wxTreeListCtrl.

The following properties may be defined with wxSmith:

Column Count
type: integer
  This is the number of columns to be displayed on the tree control.  There
  must be at leat 1 column (the main column); if this value is less than 1,
  it will be reset to 1.

Column Width
type: integer
  Initial size of each column in pixels.

TAB Char
type: multiple choice
  In the properties "Column Titles" and "Tree Items", the user is allowed to
  enter data for the tree control using a text editor.  However, indentation
  of individual text lines is significant, and sometimes using the "tab" key
  is not the best choice.  This property allows the user to select a different
  character to use as an indentation marker and a field seperator.  The choices
  are to use the "tab" character, the ">" character, or the "+" character.

Column Titles
type: string array
  This allows the user to enter the title text to be displayed at the top of
  each column.  Text lines that begin with the "!" character are ignored, but
  blank lines are significant (a blank line is a blank title at the top of a
  column).

Tree Items
type: string array
  Define node data for a wxTreeListCtrl.
  Note that all lines that start with a "!" are comment lines and ignored.
  Also, all empty lines are ignored (a line of spaces is not an empty line).

  The node data is organized similar to a Tabbed Hierarchical Values (THV) file,
  such that each line of text represents one label for a single tree node.
  The number of leading TAB characters indicate the indentation level of the
  tree node.

  Space (or blank) characters are not used the same as TAB characters; if your
  editor substitutes spaces for TABs, use the "TAB Char" attribute to change
  the character used to indicate the tree node level.

Read Only
type: boolean
  At run-time, the user may be allowed to edit and modify the data shown in
  each tree-node; setting this value to "true" will disable that feature.

Var Name
type: identifier
  This is the name (within your program) of the tree.  By default, it is
  filled in automatically by wxSmith, but you can change it to something more
  descriptive of the tree's function if you want.

