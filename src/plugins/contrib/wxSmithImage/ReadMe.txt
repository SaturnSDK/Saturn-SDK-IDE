This plugin provides wxImage and wxImageList as components for wxSmith, and
allows the images to be selected at design-time, and to compile the images
directly into the source code file.  This allows you to distribute your
program without the need for an external image file or external resource file.

The images are all stored internally as XPM image files.  The images may be
stored directly in the source code CPP file, or used as a "#include" file.

The wxImage component is normally used to hold small-to-medium sized images,
but is capable of storing any size image.  However, larger images (for example
400 x 400 pixels) will show a noticible slowing of the wxSmith editor.

The component wxImageList has an integrated image-list-editor dialog that
lets the programmer to add or delete images from an image-list, and to set
an optional transparent color for added images.

Note that for wxImageList, the "width" and "height" properties define the 
size of each image in the list (each image of course is the same size).  You
can change the width and height properties only when the image-list is empty.
Once it has any images stored in it, the size must remain the same.

A new property has also been created: "wxseditenumproperty.cpp"; this is 
similar to "wxsenumproperty.cpp", except that the item selected from the
drop-down list is returned as a wxString rather than a long integer.

As an example how to use wxImage and wxImageList, 2 other components have
been included: wxImageButton and wxImagePanel.

wxImageButton is simply a wxBitmapButton that takes it images from an 
image-list.  The wxImagePanel is simply a wxPanel that uses an OnPaint event
to show a picture; the picture is stored in a wxImage.  The wxImageButton
will work "out-of-the-box"; however, the programmer must include wxImagePanel
source code in his program for that to work properly.  (Notice the difference
between wxsImagePanel, and wxImagePanel).

All components are displayed on tool tab "Image Tools" in the wxSmith IDE.

All source code is included, but you must copy the "wxSimitImage.zip" file and
the "wxSmithImage.dll" file to the appropriate directories.

This has all been tested in Windows XP, but not in Linux or any other Unix-
style system.

Feel free to use it, ignore it, or change it any way that you need.

Ron Collins
rcoll@theriver.com



Component Summary:

>> wxImage
click the "Image" label on the property panel to select an image to assign to
  this component.  the image can come from an external file or from the default
  ArtProvider.
click the "Use Include File" label on the property panel to store the image
  data either as a "#include" file or directly into the source code.  if a
  #include file used, it will be saved into a subdirectory named "wxsimages".
When the wxImage component is created in the source code, a wxBitmap component
  will also be created (strictly as a convenience).  the wxBitmap has the
  same name as the wxImage with a "_BMP" suffix appended to it.
  
>> wxImageList
click the "Image Width" and "Image Height" labels on the property panel to
  define the size of each image in the image-list.  This sizes must be defined
  before any image is assigned to the image-list.  default size is 16 x 16.
click the "Image List" label on the property panel to bring up the image-list
  editor dialog.  this dialog allows you to add or remove individual images
  to the image-list, and optionally to select a transparent color for
  transparent images.  (select the transparent color by checking the
  "Transparent" check-box, then selecting the color with the color button or
  by clicking an area on the sample image).
the "Image Count" label on the property panel is read-only, and is provided
  only for information.
click the "Use Include File" label on the property panel to store the image
  data either as a "#include" file or directly into the source code.  if a
  #include file used, it will be saved into a subdirectory named "wxsimages".

>> wxImageButton
this is simply a wxBitmapButton that uses a wxImageList to select the images
for the button.
click on "Image List" to select a previously-defined wxImageList to use as a
  source for all the bitmaps
click on "Label Index", "Disabled Index", Selected Index", and "Focus Index"
  to select the individual image from the image-list to use on the button
click on "Is Default" to choose this button as the default button on the form

>> wxBitmapComboBox
this is a combo-box that allows a small bitmap to be displayed next to each
  selection in the drop-down list.
click on "Image List" to select a previously-defined wxImageList to use as a
  source for all the bitmaps
click on "Combo Items" to bring up the editor/dialog to assign new items to the
  combo-box;
click on "Items as Text" to see the same tree items stored as a wxArrayString;
  this was originally used as a debugging aid, but is handy to use sometimes.

>> wxImagePanel
this is not a "real" wxWidgets component, but was derived for the purposes
of the example program.  Use property "Image" to assign a wxImage to the
panel, and property "Stretch" to fit the image to the panel.  To use the
image-panel in another program, include the source code file and header file
in the project list of the new project.

>> wxImageTreeCtrl
This is a simple wxTreeCtrl, with a new editor/dialog to allow the user to
  create new items or sub-items, and assign both text and an image (from an
  image-list) to each item.
click on "Image List" to select a previously-defined wxImageList to use as a
  source for all the bitmaps
click on "Tree Items" to bring up the editor/dialog to assign new items and
  sub-items to the tree.
click on "Items as Text" to see the same tree items stored as a wxArrayString;
  this was originally used as a debugging aid, but is handy to use sometimes.
click on "Expand All" to expand all tree items automatically when your
  program starts.



  
