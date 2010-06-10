

import sys, time, math, os, os.path, pdb

import wx
import wx.propgrid as wxpg
import wx.stc


default_object_content2 = """\
object.title = "Object Title"
object.index = 1
object.PI = %f
object.wxpython_rules = True
"""%(math.pi)

default_object_content1 = """\

#
# Note that the results of autofill will appear on the second page.

#
# Set number of iterations appropriately to test performance
iterations = 100

#
# Test result for 100,000 iterations on Athlon XP 2000+:
#
# Time spent per property: 0.054ms
# Memory allocated per property: ~350 bytes (includes Python object)
#

for i in range(0,iterations):
    setattr(object,'title%i'%i,"Object Title")
    setattr(object,'index%i'%i,1)
    setattr(object,'PI%i'%i,3.14)
    setattr(object,'wxpython_rules%i'%i,True)
"""


class Display:
    property_descriptor = []
    def __init__(self):
        pass


class ValueObject:
    def __init__(self):
        pass


class IntProperty2(wxpg.PyProperty):
    """\
    This is a simple re-implementation of wxIntProperty.
    """
    def __init__(self, label, name = wxpg.LABEL_AS_NAME, value=0):
        wxpg.PyProperty.__init__(self, label, name)
        self.SetValue(value)

    def GetClassName(self):
        """\
        This is not 100% necessary and in future is probably going to be
        automated to return class name.
        """
        return "IntProperty2"

    def ValidateValue(self, value, validationInfo):
        """ Let's limit the value to range -10000 and 10000.
        """
        # Just test this function to make sure validationInfo and
        # wxPGVFBFlags work properly.
        oldvfb__ = validationInfo.GetFailureBehavior()

        # Mark the cell if validaton failred
        validationInfo.SetFailureBehavior(wxpg.PG_VFB_MARK_CELL)

        if value < -10000 or value > 10000:
            return False

        return (True, value)

    def GetEditor(self):
        return "TextCtrl"

    def GetValueAsString(self, flags):
        return str(self.GetValue())

    def StringToValue(self, s, flags):
        """ If failed, return False or (False, None). If success, return tuple
            (True, newValue).
        """
        try:
            v = int(s)
            if self.GetValue() != v:
                return (True, v)
        except (ValueError, TypeError):
            if flags & wxpg.PG_REPORT_ERROR:
                wx.MessageBox("Cannot convert '%s' into a number."%s, "Error")
        return False

    def IntToValue(self, v, flags):
        if (self.GetValue() != v):
            return (True, v)


class PyFilesProperty(wxpg.PyArrayStringProperty):
    def __init__(self, label, name = wxpg.LABEL_AS_NAME, value=[]):
        wxpg.PyArrayStringProperty.__init__(self, label, name, value)
        self.SetValue(value)

    def OnSetValue(self, v):
        self.value = v
        self.display = ', '.join(self.value)

    def GetValueAsString(self, argFlags):
        return self.display

    def StringToValue(self, s, flags):
        """ If failed, return False or (False, None). If success, return tuple
            (True, newValue).
        """
        v = [a.strip() for a in text.split(',')]
        return (True, v)

    def OnEvent(self, propgrid, ctrl, event):
        if event.GetEventType() == wx.wxEVT_COMMAND_BUTTON_CLICKED:
            # Show dialog to select a string, call DoSetValue and
            # return True, if value changed.
            return True

        return False


class PyObjectPropertyValue:
    """\
    Value type of our sample PyObjectProperty. We keep a simple dash-delimited
    list of string given as argument to constructor.
    """
    def __init__(self, s=None):
        try:
            self.ls = [a.strip() for a in s.split('-')]
        except:
            self.ls = []

    def __repr__(self):
        return ' - '.join(self.ls)



class PyObjectProperty(wxpg.PyProperty):
    """\
    Another simple example. This time our value is a PyObject (NOTE: we can't
    return an arbitrary python object in DoGetValue. It cannot be a simple
    type such as int, bool, double, or string, nor an array or wxObject based.
    Dictionary, None, or any user-specified Python object is allowed).
    """
    def __init__(self, label, name = wxpg.LABEL_AS_NAME, value=None):
        wxpg.PyProperty.__init__(self, label, name)
        self.SetValue(value)

    def GetClassName(self):
        return self.__class__.__name__

    def GetEditor(self):
        return "TextCtrl"

    def GetValueAsString(self, flags):
        return repr(self.GetValue())

    def StringToValue(self, s, flags):
        """ If failed, return False or (False, None). If success, return tuple
            (True, newValue).
        """
        v = PyObjectPropertyValue(s)
        return (True, v)


class ShapeProperty(wxpg.PyEnumProperty):
    """\
    Demonstrates use of OnCustomPaint method.
    """
    def __init__(self, label, name = wxpg.LABEL_AS_NAME, value=-1):
        wxpg.PyEnumProperty.__init__(self, label, name, ['Line','Circle','Rectangle'], [0,1,2], value)

    def OnMeasureImage(self, index):
        return wxpg.DEFAULT_IMAGE_SIZE

    def OnCustomPaint(self, dc, rect, paint_data):
        """\
        paint_data.m_choiceItem is -1 if we are painting the control,
        in which case we need to get the drawn item using DoGetValue.
        """
        item = paint_data.m_choiceItem
        if item == -1:
            item = self.DoGetValue()

        dc.SetPen(wx.Pen(wx.BLACK))
        dc.SetBrush(wx.Brush(wx.BLACK))

        if item == 0:
            dc.DrawLine(rect.x,rect.y,rect.x+rect.width,rect.y+rect.height)
        elif item == 1:
            half_width = rect.width / 2
            dc.DrawCircle(rect.x+half_width,rect.y+half_width,half_width-3)
        elif item == 2:
            dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height)


class TrivialPropertyEditor(wxpg.PyEditor):
    """\
    This is a simple re-creation of TextCtrlWithButton. Note that it does
    not take advantage of wx.TextCtrl and wx.Button creation helper functions
    in wx.PropertyGrid.
    """
    def __init__(self):
        wxpg.PyEditor.__init__(self)

    def CreateControls(self, propgrid, property, pos, sz):
        """ Create the actual wxPython controls here for editing the
            property value.

            You must use propgrid.GetPanel() as parent for created controls.

            Return value is either single editor control or tuple of two
            editor controls, of which first is the primary one and second
            is usually a button.
        """
        try:
            x, y = pos
            w, h = sz
            h = 64 + 6

            # Make room for button
            bw = propgrid.GetRowHeight()
            w -= bw

            s = property.GetDisplayedString();

            tc = wx.TextCtrl(propgrid.GetPanel(), wxpg.PG_SUBID1, s,
                             (x,y), (w,h),
                             wx.TE_PROCESS_ENTER)
            btn = wx.Button(propgrid.GetPanel(), wxpg.PG_SUBID2, '...',
                            (x+w, y),
                            (bw, h), wx.WANTS_CHARS)
            return (tc, btn)
        except:
            import traceback
            print(traceback.print_exc())

    def UpdateControl(self, property, ctrl):
        ctrl.SetValue(property.GetDisplayedString())

    def DrawValue(self, dc, rect, property, text):
        if not property.IsValueUnspecified():
            dc.DrawText(property.GetDisplayedString(), rect.x+5, rect.y)

    def OnEvent(self, propgrid, property, ctrl, event):
        """ Return True if modified editor value should be committed to
            the property. To just mark the property value modified, call
            propgrid.EditorsValueWasModified().
        """
        if not ctrl:
            return False

        evtType = event.GetEventType()

        if evtType == wx.wxEVT_COMMAND_TEXT_ENTER:
            if propgrid.IsEditorsValueModified():
                return True
        elif evtType == wx.wxEVT_COMMAND_TEXT_UPDATED:
            #
            # Pass this event outside wxPropertyGrid so that,
            # if necessary, program can tell when user is editing
            # a textctrl.
            event.Skip()
            event.SetId(propgrid.GetId())

            propgrid.EditorsValueWasModified()
            return False

        return False

    def GetValueFromControl(self, property, ctrl):
        tc = ctrl
        textVal = tc.GetValue()

        if property.UsesAutoUnspecified() and not textVal:
            return (True, None)

        # In wxPython 2.9+, call StringToValue() instead
        res, value = property.PyStringToValue(textVal,
                                              wxpg.PG_EDITABLE_VALUE)

        # Changing unspecified always causes event (returning
        # True here should be enough to trigger it).
        if not res and value is None:
            res = True

        return (res, value)

    def SetValueToUnspecified(self, property, ctrl):
        ctrl.Remove(0,len(ctrl.GetValue()))

    def SetControlStringValue(self, property, ctrl, text):
        ctrl.SetValue(text)

    def OnFocus(self, property, ctrl):
        ctrl.SetSelection(-1,-1)
        ctrl.SetFocus()


class LargeImagePickerCtrl(wx.Panel):
    """\
    Control created and used by LargeImageEditor.
    """
    def __init__(self):
        pre = wx.PrePanel()
        self.PostCreate(pre)

    def Create(self, parent, id_, pos, size, style = 0):
        wx.Panel.Create(self, parent, id_, pos, size,
                        style | wx.BORDER_SIMPLE)
        img_spc = size[1]
        self.tc = wx.TextCtrl(self, -1, "", (img_spc,0), (2048,size[1]),
                              wx.BORDER_NONE)
        self.SetBackgroundColour(wx.WHITE)
        self.SetBackgroundStyle(wx.BG_STYLE_CUSTOM)
        self.property = None
        self.bmp = None
        self.Bind(wx.EVT_PAINT, self.OnPaint)

    def OnPaint(self, event):
        dc = wx.BufferedPaintDC(self)

        whiteBrush = wx.Brush(wx.WHITE)
        dc.SetBackground(whiteBrush)
        dc.Clear()

        bmp = self.bmp
        if bmp:
            dc.DrawBitmap(bmp, 2, 2)
        else:
            dc.SetPen(wx.Pen(wx.BLACK))
            dc.SetBrush(whiteBrush)
            dc.DrawRectangle(2, 2, 64, 64)

    def RefreshThumbnail(self):
        """\
        We use here very simple image scaling code.
        """
        if not self.property:
            self.bmp = None
            return

        path = self.property.DoGetValue()

        if not os.path.isfile(path):
            self.bmp = None
            return

        image = wx.Image(path)
        image.Rescale(64, 64)
        self.bmp = wx.BitmapFromImage(image)

    def SetProperty(self, property):
        self.property = property
        self.tc.SetValue(property.GetDisplayedString())
        self.RefreshThumbnail()

    def SetValue(self, s):
        self.RefreshThumbnail()
        self.tc.SetValue(s)

    def GetLastPosition(self):
        return self.tc.GetLastPosition()


class LargeImageEditor(wxpg.PyEditor):
    """\
    Double-height text-editor with image in front.
    """
    def __init__(self):
        wxpg.PyEditor.__init__(self)

    def CreateControls(self, propgrid, property, pos, sz):
        try:
            x, y = pos
            w, h = sz
            h = 64 + 6

            # Make room for button
            bw = propgrid.GetRowHeight()
            w -= bw

            lipc = LargeImagePickerCtrl()
            if sys.platform.startswith('win'):
                lipc.Hide()
            lipc.Create(propgrid.GetPanel(), wxpg.PG_SUBID1, (x,y), (w,h))
            lipc.SetProperty(property)
            # Hmmm.. how to have two-stage creation without subclassing?
            #btn = wx.PreButton()
            #pre = wx.PreWindow()
            #self.PostCreate(pre)
            #if sys.platform == 'win32':
            #    btn.Hide()
            #btn.Create(propgrid, wxpg.PG_SUBID2, '...', (x2-bw,pos[1]),
            #           (bw,h), wx.WANTS_CHARS)
            btn = wx.Button(propgrid.GetPanel(), wxpg.PG_SUBID2, '...',
                            (x+w, y),
                            (bw, h), wx.WANTS_CHARS)
            return (lipc, btn)
        except:
            import traceback
            print(traceback.print_exc())

    def UpdateControl(self, property, ctrl):
        ctrl.SetValue(property.GetDisplayedString())

    def DrawValue(self, dc, rect, property, text):
        if not property.IsValueUnspecified():
            dc.DrawText(property.GetDisplayedString(), rect.x+5, rect.y)

    def OnEvent(self, propgrid, property, ctrl, event):
        if not ctrl:
            return False

        evtType = event.GetEventType()

        if evtType == wx.wxEVT_COMMAND_TEXT_ENTER:
            if propgrid.IsEditorsValueModified():
                return True
        elif evtType == wx.wxEVT_COMMAND_TEXT_UPDATED:
            #
            # Pass this event outside wxPropertyGrid so that,
            # if necessary, program can tell when user is editing
            # a textctrl.
            event.Skip()
            event.SetId(propgrid.GetId())

            propgrid.EditorsValueWasModified()
            return False

        return False

    def GetValueFromControl(self, property, ctrl):
        tc = ctrl.tc
        textVal = tc.GetValue()

        if property.UsesAutoUnspecified() and not textVal:
            return (True, None)

        # In wxPython 2.9+, call StringToValue() instead
        res, value = property.PyStringToValue(textVal,
                                              wxpg.PG_EDITABLE_VALUE)

        # Changing unspecified always causes event (returning
        # True here should be enough to trigger it).
        if not res and value is None:
            res = True

        return (res, value)

    def SetValueToUnspecified(self, property, ctrl):
        ctrl.tc.Remove(0,len(ctrl.tc.GetValue()))

    def SetControlStringValue(self, property, ctrl, txt):
        ctrl.SetValue(txt)

    def OnFocus(self, property, ctrl):
        ctrl.tc.SetSelection(-1,-1)
        ctrl.tc.SetFocus()

    def CanContainCustomImage(self):
        return True


class TestPropertyGridPanel(wx.Panel):
    def __init__(self, parent, log):
        # Use the WANTS_CHARS style so the panel doesn't eat the Return key.
        wx.Panel.__init__(self, parent, -1, style=wx.WANTS_CHARS)

        self.log = log
        tID = wx.NewId()

        topsizer = wx.BoxSizer(wx.VERTICAL)

        # Difference between using PropertyGridManager vs PropertyGrid is that
        # the manager supports multiple pages and a description box.
        self.pg = pg = wxpg.PropertyGridManager(self,
            style=wxpg.PG_SPLITTER_AUTO_CENTER|wxpg.PG_AUTO_SORT|wxpg.PG_TOOLBAR)  # |wxpg.PG_DESCRIPTION
        #self.pg = pg = wxpg.PropertyGrid(self,
        #    style=wxpg.PG_SPLITTER_AUTO_CENTER|wxpg.PG_AUTO_SORT)

        # Show help as tooltips
        pg.SetExtraStyle(wxpg.PG_EX_HELP_AS_TOOLTIPS |
                         wxpg.PG_EX_MULTIPLE_SELECTION)

        pg.Bind( wxpg.EVT_PG_CHANGED, self.OnPropGridChange )
        pg.Bind( wxpg.EVT_PG_PAGE_CHANGED, self.OnPropGridPageChange )
        pg.Bind( wxpg.EVT_PG_SELECTED, self.OnPropGridSelect )
        pg.Bind( wxpg.EVT_PG_RIGHT_CLICK, self.OnPropGridRightClick )

        # Needed by custom image editor
        wx.InitAllImageHandlers()

        #
        # Let's use some simple custom editor
        #
        # NOTE: Editor must be registered *before* adding a property that
        # uses it.
        pg.RegisterEditor(TrivialPropertyEditor)
        pg.RegisterEditor(LargeImageEditor)

        #
        # Add properties
        #

        pg.AddPage( "Page 1 - Testing All" )

        pg.Append( wxpg.PropertyCategory("1 - Basic Properties") )

        pg.Append( wxpg.StringProperty("String",value="Some Text") )
        pg.Append( wxpg.IntProperty("Int",value=100) )
        pg.Append( wxpg.FloatProperty("Float",value=100.0) )
        pg.Append( wxpg.BoolProperty("Bool",value=True) )
        pg.Append( wxpg.BoolProperty("Bool_with_Checkbox",value=True) )
        pg.SetPropertyAttribute("Bool_with_Checkbox", "UseCheckbox", True)

        pg.Append( wxpg.PropertyCategory("2 - More Properties") )
        pg.Append( wxpg.LongStringProperty("LongString",value="This is a\\nmulti-line string\\nwith\\ttabs\\nmixed\\tin.") )
        pg.Append( wxpg.DirProperty("Dir",value="C:\\Windows") )
        pg.Append( wxpg.FileProperty("File",value="C:\\Windows\\system.ini") )
        pg.Append( wxpg.ArrayStringProperty("ArrayString",value=['A','B','C']) )

        pg.Append( wxpg.EnumProperty("Enum","Enum",
                                     ['wxPython Rules','wxPython Rocks','wxPython Is The Best'],
                                     [10,11,12],0) )
        pg.Append( wxpg.EditEnumProperty("EditEnum","EditEnumProperty",['A','B','C'],[0,1,2],"Text Not in List") )

        pg.Append( wxpg.PropertyCategory("3 - Advanced Properties") )
        pg.Append( wxpg.DateProperty("Date",value=wx.DateTime_Now()) )
        pg.Append( wxpg.FontProperty("Font",value=self.GetFont()) )
        pg.Append( wxpg.ColourProperty("Colour",value=self.GetBackgroundColour()) )
        pg.Append( wxpg.SystemColourProperty("SystemColour") )
        pg.Append( wxpg.ImageFileProperty("ImageFile") )
        pg.Append( wxpg.MultiChoiceProperty("MultiChoice",choices=['wxWidgets','QT','GTK+']) )

        pg.Append( wxpg.PropertyCategory("4 - Additional Properties") )
        pg.Append( wxpg.PointProperty("Point",value=self.GetPosition()) )
        pg.Append( wxpg.SizeProperty("Size",value=self.GetSize()) )
        pg.Append( wxpg.FontDataProperty("FontData") )
        pg.Append( wxpg.IntProperty("IntWithSpin",value=256) )
        pg.SetPropertyEditor("IntWithSpin","SpinCtrl")
        pg.Append( wxpg.DirsProperty("Dirs",value=['C:/Lib','C:/Bin']) )
        pg.SetPropertyHelpString( "String", "String Property help string!" )
        pg.SetPropertyHelpString( "Dirs", "Dirs Property help string!" )

        pg.SetPropertyAttribute( "File", wxpg.PG_FILE_SHOW_FULL_PATH, 0 )
        pg.SetPropertyAttribute( "File", wxpg.PG_FILE_INITIAL_PATH, "C:\\Program Files\\Internet Explorer" )
        pg.SetPropertyAttribute( "Date", wxpg.PG_DATE_PICKER_STYLE, wx.DP_DROPDOWN|wx.DP_SHOWCENTURY )

        pg.Append( wxpg.PropertyCategory("5 - Custom Properties") )
        pg.Append( IntProperty2("IntProperty2", value=1024) )

        pg.Append( ShapeProperty("ShapeProperty", value=0) )
        pg.Append( PyObjectProperty("PyObjectProperty") )

        prop = pg.Append( wxpg.StringProperty("StringWithCustomEditor",
                                              value="test value") )
        pg.SetPropertyEditor(prop, "TrivialPropertyEditor")

        pg.Append( wxpg.ImageFileProperty("ImageFileWithLargeEditor") )
        pg.SetPropertyEditor("ImageFileWithLargeEditor", "LargeImageEditor")

        # When page is added, it will become the target page for AutoFill
        # calls (and for other property insertion methods as well)
        pg.AddPage( "Page 2 - Results of AutoFill will appear here" )

        pg.SetPropertyClientData( "Point", 1234 )
        if pg.GetPropertyClientData( "Point" ) != 1234:
            raise ValueError("Set/GetPropertyClientData() failed")

        # Test setting unicode string
        pg.GetPropertyByName("String").SetValue(u"Some Unicode Text")

        #
        # Test some code that *should* fail (but not crash)
        try:
            a_ = pg.GetPropertyValue( "NotARealProperty" )
            pg.EnableProperty( "NotAtAllRealProperty", False )
            pg.SetPropertyHelpString( "AgaintNotARealProperty", "Dummy Help String" )
        except:
            pass
            #raise

        topsizer.Add(pg,1,wx.EXPAND)

        rowsizer = wx.BoxSizer(wx.HORIZONTAL)
        but = wx.Button(self,-1,"SetPropertyValues")
        but.Bind( wx.EVT_BUTTON, self.OnSetPropertyValues )
        rowsizer.Add(but,1)
        but = wx.Button(self,-1,"GetPropertyValues")
        but.Bind( wx.EVT_BUTTON, self.OnGetPropertyValues )
        rowsizer.Add(but,1)
        topsizer.Add(rowsizer,0,wx.EXPAND)
        rowsizer = wx.BoxSizer(wx.HORIZONTAL)
        but = wx.Button(self,-1,"GetPropertyValues(as_strings=True)")
        but.Bind( wx.EVT_BUTTON, self.OnGetPropertyValues2 )
        rowsizer.Add(but,1)
        but = wx.Button(self,-1,"AutoFill")
        but.Bind( wx.EVT_BUTTON, self.OnAutoFill )
        rowsizer.Add(but,1)
        topsizer.Add(rowsizer,0,wx.EXPAND)
        rowsizer = wx.BoxSizer(wx.HORIZONTAL)
        but = wx.Button(self,-1,"Delete")
        but.Bind( wx.EVT_BUTTON, self.OnDeleteProperty )
        rowsizer.Add(but,1)
        but = wx.Button(self,-1,"Run Tests")
        but.Bind( wx.EVT_BUTTON, self.RunTests )
        rowsizer.Add(but,1)
        topsizer.Add(rowsizer,0,wx.EXPAND)

        self.SetSizer(topsizer)
        topsizer.SetSizeHints(self)

    def OnPropGridChange(self, event):
        p = event.GetProperty()
        if p:
            print('%s changed to "%s"'%(p.GetName(),p.GetValueAsString()))

    def OnPropGridSelect(self, event):
        p = event.GetProperty()
        if p:
            print('%s selected'%(event.GetPropertyName()))
        else:
            print('Nothing selected')

    def OnDeleteProperty(self, event):
        p = self.pg.GetSelectedProperty()
        if p:
            self.pg.Delete(p)
        else:
            wx.MessageBox("First select a property to delete")

    def OnReserved(self, event):
        pass

    def OnSetPropertyValues(self,event):
        try:
            d = self.pg.GetPropertyValues(inc_attributes=True)

            ss = []
            for k,v in d.iteritems():
                v = repr(v)
                if not v or v[0] != '<':
                    if k.startswith('@'):
                        ss.append('setattr(object, "%s", %s)'%(k,v))
                    else:
                        ss.append('object.%s = %s'%(k,v))

            dlg = MemoDialog(self,"Enter Content for Object Used in SetPropertyValues",
                '\n'.join(ss))  # default_object_content1

            if dlg.ShowModal() == wx.ID_OK:
                sandbox = {'object':ValueObject(),'wx':wx}
                exec dlg.tc.GetValue() in sandbox
                t_start = time.time()
                #print(sandbox['object'].__dict__)
                self.pg.SetPropertyValues(sandbox['object'])
                t_end = time.time()
                print('SetPropertyValues finished in %.0fms' % \
                      ((t_end-t_start)*1000.0))
        except:
            import traceback
            traceback.print_exc()

    def OnGetPropertyValues(self,event):
        try:
            t_start = time.time()
            d = self.pg.GetPropertyValues(inc_attributes=True)
            t_end = time.time()
            print('GetPropertyValues finished in %.0fms' % \
                  ((t_end-t_start)*1000.0))
            ss = ['%s: %s'%(k,repr(v)) for k,v in d.iteritems()]
            dlg = MemoDialog(self,"GetPropertyValues Result",
                             'Contents of resulting dictionary:\n\n'+'\n'.join(ss))
            dlg.ShowModal()
        except:
            import traceback
            traceback.print_exc()

    def OnGetPropertyValues2(self,event):
        try:
            t_start = time.time()
            d = self.pg.GetPropertyValues(as_strings=True)
            t_end = time.time()
            print('GetPropertyValues(as_strings=True) finished in %.0fms' % \
                  ((t_end-t_start)*1000.0))
            ss = ['%s: %s'%(k,repr(v)) for k,v in d.iteritems()]
            dlg = MemoDialog(self,"GetPropertyValues Result",
                             'Contents of resulting dictionary:\n\n'+'\n'.join(ss))
            dlg.ShowModal()
        except:
            import traceback
            traceback.print_exc()

    def OnAutoFill(self,event):
        try:
            dlg = MemoDialog(self,"Enter Content for Object Used for AutoFill",default_object_content1)
            if dlg.ShowModal() == wx.ID_OK:
                sandbox = {'object':ValueObject(),'wx':wx}
                exec dlg.tc.GetValue() in sandbox
                t_start = time.time()
                self.pg.AutoFill(sandbox['object'])
                t_end = time.time()
                print('AutoFill finished in %.0fms' % \
                      ((t_end-t_start)*1000.0))
        except:
            import traceback
            traceback.print_exc()

    def OnPropGridRightClick(self, event):
        p = event.GetProperty()
        if p:
            print('%s right clicked'%(event.GetPropertyName()))
        else:
            print('Nothing right clicked')

    def OnPropGridPageChange(self, event):
        index = self.pg.GetSelectedPage()
        print('Page Changed to \'%s\''%(self.pg.GetPageName(index)))

    def RunTests(self, event):
        pg = self.pg
        grid = pg.GetGrid()

        # Iterator

        it = pg.GetPage(0).GetIterator(wxpg.PG_ITERATE_ALL)
        while not it.AtEnd():
            print('Iterating \'%s\''%(it.GetProperty().GetName()))
            it.Next()

        # VIterator

        it = pg.GetVIterator(wxpg.PG_ITERATE_ALL)
        while not it.AtEnd():
            print('Iterating \'%s\''%(it.GetProperty().GetName()))
            it.Next()

        # Selection
        grid.ClearSelection()
        grid.AddToSelection("Date")
        selected_properties = grid.GetSelectedProperties()
        if len(selected_properties) != 1:
            raise AssertionError
        if selected_properties[0].GetName() != "Date":
            raise AssertionError

        # Hit Test
        htr = grid.HitTest((50, 50))
        print('HitTestResult:')
        print('  property: %s' % htr.property)
        print('  column: %s' % htr.column)
        print('  splitter: %s' % htr.splitter)
        print('  splitterHitOffset: %s' % htr.splitterHitOffset)

#---------------------------------------------------------------------------


class MemoDialog(wx.Dialog):
    """\
    Dialog for multi-line text editing.
    """
    def __init__(self,parent=None,title="",text="",pos=None,size=(500,500)):
        wx.Dialog.__init__(self,parent,-1,title,style=wx.DEFAULT_DIALOG_STYLE|wx.RESIZE_BORDER)

        topsizer = wx.BoxSizer( wx.VERTICAL )

        tc = wx.TextCtrl(self,11,text,style=wx.TE_MULTILINE)
        self.tc = tc
        topsizer.Add(tc,1,wx.EXPAND|wx.ALL,8)

        rowsizer = wx.BoxSizer( wx.HORIZONTAL )
        rowsizer.Add(wx.Button(self,wx.ID_OK,'Ok'),0,wx.ALIGN_RIGHT|wx.ALIGN_CENTRE_VERTICAL,8)
        rowsizer.Add((0,0),1,wx.ALIGN_RIGHT|wx.ALIGN_CENTRE_VERTICAL,8)
        rowsizer.Add(wx.Button(self,wx.ID_CANCEL,'Cancel'),0,wx.ALIGN_RIGHT|wx.ALIGN_CENTRE_VERTICAL,8)
        topsizer.Add(rowsizer,0,wx.EXPAND|wx.ALL,8)

        self.SetSizer( topsizer )
        topsizer.Layout()

        self.SetSize( size )
        if not pos:
            self.CenterOnScreen()
        else:
            self.Move(pos)


#---------------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestPropertyGridPanel(nb, log)
    return win





class MainFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, -1, "wx.PropertyGrid Test", (10,10), (350,550))
        self.panel = TestPropertyGridPanel(self,None)



class MyApp(wx.App):
    def OnInit(self):
        self.main_frame = MainFrame()
        self.main_frame.Show()

        return True

    def OnExit(self):
        return True


def main():

    # now, silence a deprecation warning for py2.3
    import warnings
    warnings.filterwarnings("ignore", "integer", DeprecationWarning,
                            "wxPython.gdi")

    redirect = True
    if __debug__: redirect = False

    print('Using Python %i.%i.%i' % \
          (sys.version_info[0],sys.version_info[1],sys.version_info[2]))
    print('Using wxPython %s'%wx.__version__)
    app = MyApp(redirect=redirect)
    app.MainLoop()
    #print 'main loop ends'

main()
