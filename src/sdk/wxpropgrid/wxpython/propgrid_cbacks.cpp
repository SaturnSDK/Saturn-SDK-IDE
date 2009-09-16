%{

// THIS FILE HAS BEEN AUTO-GENERATED

#ifndef SWIG_IsOK
    #define SWIG_IsOK(r)               (r >= 0)
#endif

#ifndef Py_RETURN_NONE
    #define Py_RETURN_NONE return Py_INCREF(Py_None), Py_None
#endif

void _deleteOwningObject(void* obj)
{
    // Crashes sometimes (on app exit, it seems), so we need to disable it
    /*if ( Py_IsInitialized() )
    {
        wxPyBlock_t blocked = wxPyBeginBlockThreads();
        Py_XDECREF((PyObject*)obj);
        wxPyEndBlockThreads(blocked);
    }*/
}
    

static bool gs_funcNamesInitialized = false;
static PyObject* gs___class___Name = NULL;
static PyObject* gs___dict___Name = NULL;
static PyObject* gs__super_call_Name = NULL;
static PyObject* gs_CanContainCustomImage_Name = NULL;
static PyObject* gs_ChildChanged_Name = NULL;
static PyObject* gs_ColourToString_Name = NULL;
static PyObject* gs_CreateControls_Name = NULL;
static PyObject* gs_DeleteItem_Name = NULL;
static PyObject* gs_DoGetAttribute_Name = NULL;
static PyObject* gs_DoGetValidator_Name = NULL;
static PyObject* gs_DoGetValue_Name = NULL;
static PyObject* gs_DoSetAttribute_Name = NULL;
static PyObject* gs_DoShowDialog_Name = NULL;
static PyObject* gs_DoTranslateVal_Name = NULL;
static PyObject* gs_DrawValue_Name = NULL;
static PyObject* gs_GenerateValueAsString_Name = NULL;
static PyObject* gs_GetCellRenderer_Name = NULL;
static PyObject* gs_GetChoiceInfo_Name = NULL;
static PyObject* gs_GetColour_Name = NULL;
static PyObject* gs_GetCustomColourIndex_Name = NULL;
static PyObject* gs_GetEditor_Name = NULL;
static PyObject* gs_GetEditorDialog_Name = NULL;
static PyObject* gs_GetEntry_Name = NULL;
static PyObject* gs_GetImageSize_Name = NULL;
static PyObject* gs_GetIndexForValue_Name = NULL;
static PyObject* gs_GetName_Name = NULL;
static PyObject* gs_GetValueAsString_Name = NULL;
static PyObject* gs_GetValueFromControl_Name = NULL;
static PyObject* gs_InsertItem_Name = NULL;
static PyObject* gs_IntToValue_Name = NULL;
static PyObject* gs_OnButtonClick_Name = NULL;
static PyObject* gs_OnCustomPaint_Name = NULL;
static PyObject* gs_OnCustomStringEdit_Name = NULL;
static PyObject* gs_OnEvent_Name = NULL;
static PyObject* gs_OnFocus_Name = NULL;
static PyObject* gs_OnMeasureImage_Name = NULL;
static PyObject* gs_OnSetValue_Name = NULL;
static PyObject* gs_OnValidationFailure_Name = NULL;
static PyObject* gs_RefreshChildren_Name = NULL;
static PyObject* gs_SetControlIntValue_Name = NULL;
static PyObject* gs_SetControlStringValue_Name = NULL;
static PyObject* gs_SetValueToUnspecified_Name = NULL;
static PyObject* gs_StringToValue_Name = NULL;
static PyObject* gs_UpdateControl_Name = NULL;
static PyObject* gs_ValidateValue_Name = NULL;

static void _InitFuncNames()
{
    gs___dict___Name = PyString_FromString("__dict__");
    gs___class___Name = PyString_FromString("__class__");
    gs__super_call_Name = PyString_FromString("_super_call");
    gs_CanContainCustomImage_Name = PyString_FromString("CanContainCustomImage_t_");
    gs_ChildChanged_Name = PyString_FromString("ChildChanged_t_");
    gs_ColourToString_Name = PyString_FromString("ColourToString_t_");
    gs_CreateControls_Name = PyString_FromString("CreateControls_t_");
    gs_DeleteItem_Name = PyString_FromString("DeleteItem_t_");
    gs_DoGetAttribute_Name = PyString_FromString("DoGetAttribute_t_");
    gs_DoGetValidator_Name = PyString_FromString("DoGetValidator_t_");
    gs_DoGetValue_Name = PyString_FromString("DoGetValue_t_");
    gs_DoSetAttribute_Name = PyString_FromString("DoSetAttribute_t_");
    gs_DoShowDialog_Name = PyString_FromString("DoShowDialog_t_");
    gs_DoTranslateVal_Name = PyString_FromString("DoTranslateVal_t_");
    gs_DrawValue_Name = PyString_FromString("DrawValue_t_");
    gs_GenerateValueAsString_Name = PyString_FromString("GenerateValueAsString_t_");
    gs_GetCellRenderer_Name = PyString_FromString("GetCellRenderer_t_");
    gs_GetChoiceInfo_Name = PyString_FromString("GetChoiceInfo_t_");
    gs_GetColour_Name = PyString_FromString("GetColour_t_");
    gs_GetCustomColourIndex_Name = PyString_FromString("GetCustomColourIndex_t_");
    gs_GetEditor_Name = PyString_FromString("GetEditor_t_");
    gs_GetEditorDialog_Name = PyString_FromString("GetEditorDialog_t_");
    gs_GetEntry_Name = PyString_FromString("GetEntry_t_");
    gs_GetImageSize_Name = PyString_FromString("GetImageSize_t_");
    gs_GetIndexForValue_Name = PyString_FromString("GetIndexForValue_t_");
    gs_GetName_Name = PyString_FromString("GetName_t_");
    gs_GetValueAsString_Name = PyString_FromString("GetValueAsString_t_");
    gs_GetValueFromControl_Name = PyString_FromString("GetValueFromControl_t_");
    gs_InsertItem_Name = PyString_FromString("InsertItem_t_");
    gs_IntToValue_Name = PyString_FromString("IntToValue_t_");
    gs_OnButtonClick_Name = PyString_FromString("OnButtonClick_t_");
    gs_OnCustomPaint_Name = PyString_FromString("OnCustomPaint_t_");
    gs_OnCustomStringEdit_Name = PyString_FromString("OnCustomStringEdit_t_");
    gs_OnEvent_Name = PyString_FromString("OnEvent_t_");
    gs_OnFocus_Name = PyString_FromString("OnFocus_t_");
    gs_OnMeasureImage_Name = PyString_FromString("OnMeasureImage_t_");
    gs_OnSetValue_Name = PyString_FromString("OnSetValue_t_");
    gs_OnValidationFailure_Name = PyString_FromString("OnValidationFailure_t_");
    gs_RefreshChildren_Name = PyString_FromString("RefreshChildren_t_");
    gs_SetControlIntValue_Name = PyString_FromString("SetControlIntValue_t_");
    gs_SetControlStringValue_Name = PyString_FromString("SetControlStringValue_t_");
    gs_SetValueToUnspecified_Name = PyString_FromString("SetValueToUnspecified_t_");
    gs_StringToValue_Name = PyString_FromString("StringToValue_t_");
    gs_UpdateControl_Name = PyString_FromString("UpdateControl_t_");
    gs_ValidateValue_Name = PyString_FromString("ValidateValue_t_");
    gs_funcNamesInitialized = true;
}


int _CommonCallback17(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPGChoiceInfo* choiceinfo)
{
    PyObject* res;
    PyObject* py_choiceinfo;
    py_choiceinfo = SWIG_NewPointerObj((void*)choiceinfo, SWIGTYPE_p_wxPGChoiceInfo, 0);
    res = PyObject_CallFunctionObjArgs(funcobj, self, py_choiceinfo, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_choiceinfo);
    if (PyErr_Occurred()) SWIG_fail;
    {
    int retval;
    retval = (int)PyInt_AS_LONG(res);
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return 0;
}

void _CommonCallback3(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text)
{
    PyObject* res;
    PyObject* py_dc;
    py_dc = SWIG_NewPointerObj((void*)&dc, SWIGTYPE_p_wxDC, 0);
    PyObject* py_rect;
    py_rect = SWIG_NewPointerObj((void*)&rect, SWIGTYPE_p_wxRect, 0);
    PyObject* py_property;
    py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);
    PyObject* py_text;
    #if wxUSE_UNICODE
        py_text = PyUnicode_FromWideChar((&text)->c_str(), (&text)->Len());
    #else
        py_text = PyString_FromStringAndSize((&text)->c_str(), (&text)->Len());
    #endif

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_dc, py_rect, py_property, py_text, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_text);
    Py_DECREF(py_property);
    Py_DECREF(py_rect);
    Py_DECREF(py_dc);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

void _CommonCallback39(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPGProperty* property, wxWindow* ctrl, int value)
{
    PyObject* res;
    PyObject* py_property;
        py_property = NULL;
    if ( property->m_scriptObject ) py_property = (PyObject*)property->m_scriptObject;
    if ( py_property ) Py_INCREF(py_property);
    else py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);

    PyObject* py_ctrl;
    py_ctrl = wxPyMake_wxObject(ctrl, (bool)0);

    PyObject* py_value;
    py_value = PyInt_FromLong((long)value);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_property, py_ctrl, py_value, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_value);
    Py_DECREF(py_ctrl);
    Py_DECREF(py_property);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

wxSize _CommonCallback23(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, int item)
{
    PyObject* res;
    PyObject* py_item;
    py_item = PyInt_FromLong((long)item);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_item, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_item);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxSize retval;
    wxSize temp;    wxSize* _tptr_0 = &retval;
        if ( ! wxSize_helper(res, &_tptr_0)) SWIG_fail;
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxSize();
}

void _CommonCallback9(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPGProperty* property, wxWindow* ctrl, int value)
{
    PyObject* res;
    PyObject* py_property;
    py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);
    PyObject* py_ctrl;
    py_ctrl = wxPyMake_wxObject(ctrl, (bool)0);

    PyObject* py_value;
    py_value = PyInt_FromLong((long)value);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_property, py_ctrl, py_value, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_value);
    Py_DECREF(py_ctrl);
    Py_DECREF(py_property);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

void _CommonCallback11(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxVariant& thisValue, int childIndex, wxVariant& childValue)
{
    PyObject* res;
    PyObject* py_thisValue;
    py_thisValue = wxVariant_to_PyObject((&thisValue));
        if ( !py_thisValue ) {
            PyErr_SetString(PyExc_TypeError, "this wxVariant type cannot be converted to Python object");
            SWIG_fail;
        }

    PyObject* py_childIndex;
    py_childIndex = PyInt_FromLong((long)childIndex);

    PyObject* py_childValue;
    py_childValue = wxVariant_to_PyObject((&childValue));
        if ( !py_childValue ) {
            PyErr_SetString(PyExc_TypeError, "this wxVariant type cannot be converted to Python object");
            SWIG_fail;
        }

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_thisValue, py_childIndex, py_childValue, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_childValue);
    Py_DECREF(py_childIndex);
    Py_DECREF(py_thisValue);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

wxValidator* _CommonCallback13(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj)
{
    PyObject* res;
    res = PyObject_CallFunctionObjArgs(funcobj, self, NULL);
    Py_DECREF(funcobj);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxValidator* retval;
    if ( !SWIG_IsOK(SWIG_ConvertPtr(res, (void**)&(retval), SWIGTYPE_p_wxValidator, 0)) ) {
        PyErr_SetString(PyExc_TypeError,"expected wxValidator");
        SWIG_fail;
    }
    PyObject_SetAttrString(res, "thisown", Py_False);
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return NULL;
}

wxPGVariantAndBool _CommonCallback26(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, int number, int argFlags)
{
    PyObject* res;
    PyObject* py_number;
    py_number = PyInt_FromLong((long)number);

    PyObject* py_argFlags;
    py_argFlags = PyInt_FromLong((long)argFlags);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_number, py_argFlags, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_argFlags);
    Py_DECREF(py_number);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxPGVariantAndBool retval;
    if ( !PyObject_to_wxPGVariantAndBool(res, retval) ) {
            PyErr_SetString(PyExc_TypeError, "this Python type cannot be converted to wxVariant");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxPGVariantAndBool();
}

wxPGVariantAndBool _CommonCallback38(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPGProperty* property, wxWindow* ctrl)
{
    PyObject* res;
    PyObject* py_property;
        py_property = NULL;
    if ( property->m_scriptObject ) py_property = (PyObject*)property->m_scriptObject;
    if ( py_property ) Py_INCREF(py_property);
    else py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);

    PyObject* py_ctrl;
    py_ctrl = wxPyMake_wxObject(ctrl, (bool)0);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_property, py_ctrl, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_ctrl);
    Py_DECREF(py_property);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxPGVariantAndBool retval;
    if ( !PyObject_to_wxPGVariantAndBool(res, retval) ) {
            PyErr_SetString(PyExc_TypeError, "this Python type cannot be converted to wxVariant");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxPGVariantAndBool();
}

wxSize _CommonCallback19(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj)
{
    PyObject* res;
    res = PyObject_CallFunctionObjArgs(funcobj, self, NULL);
    Py_DECREF(funcobj);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxSize retval;
    wxSize temp;    wxSize* _tptr_0 = &retval;
        if ( ! wxSize_helper(res, &_tptr_0)) SWIG_fail;
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxSize();
}

wxVariant _CommonCallback14(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj)
{
    PyObject* res;
    res = PyObject_CallFunctionObjArgs(funcobj, self, NULL);
    Py_DECREF(funcobj);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxVariant retval;
    if ( !PyObject_to_wxVariant(res, &retval) ) {
            PyErr_SetString(PyExc_TypeError, "this Python type cannot be converted to wxVariant");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxVariant();
}

wxPGWindowList _CommonCallback1(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz)
{
    PyObject* res;
    PyObject* py_propgrid;
    py_propgrid = SWIG_NewPointerObj((void*)propgrid, SWIGTYPE_p_wxPropertyGrid, 0);
    PyObject* py_property;
    py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);
    PyObject* py_pos;
    py_pos = wxPoint_to_PyObject((&pos));

    PyObject* py_sz;
    py_sz = wxSize_to_PyObject((&sz));

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_propgrid, py_property, py_pos, py_sz, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_sz);
    Py_DECREF(py_pos);
    Py_DECREF(py_property);
    Py_DECREF(py_propgrid);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxPGWindowList retval;
    if ( !PyObject_to_wxPGWindowList(res, &retval) ) {
            PyErr_SetString(PyExc_TypeError, "expected wxWindow or tuple of two wxWindows");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxPGWindowList();
}

bool _CommonCallback6(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event)
{
    PyObject* res;
    PyObject* py_propgrid;
    py_propgrid = SWIG_NewPointerObj((void*)propgrid, SWIGTYPE_p_wxPropertyGrid, 0);
    PyObject* py_property;
    py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);
    PyObject* py_primary;
    py_primary = wxPyMake_wxObject(primary, (bool)0);

    PyObject* py_event;
    py_event = SWIG_NewPointerObj((void*)&event, SWIGTYPE_p_wxEvent, 0);
    res = PyObject_CallFunctionObjArgs(funcobj, self, py_propgrid, py_property, py_primary, py_event, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_event);
    Py_DECREF(py_primary);
    Py_DECREF(py_property);
    Py_DECREF(py_propgrid);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
}

bool _CommonCallback22(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    PyObject* res;
    PyObject* py_propgrid;
    py_propgrid = SWIG_NewPointerObj((void*)propgrid, SWIGTYPE_p_wxPropertyGrid, 0);
    PyObject* py_wnd_primary;
    py_wnd_primary = wxPyMake_wxObject(wnd_primary, (bool)0);

    PyObject* py_event;
    py_event = SWIG_NewPointerObj((void*)&event, SWIGTYPE_p_wxEvent, 0);
    res = PyObject_CallFunctionObjArgs(funcobj, self, py_propgrid, py_wnd_primary, py_event, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_event);
    Py_DECREF(py_wnd_primary);
    Py_DECREF(py_propgrid);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
}

wxPGWindowList _CommonCallback34(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz)
{
    PyObject* res;
    PyObject* py_propgrid;
    py_propgrid = SWIG_NewPointerObj((void*)propgrid, SWIGTYPE_p_wxPropertyGrid, 0);
    PyObject* py_property;
        py_property = NULL;
    if ( property->m_scriptObject ) py_property = (PyObject*)property->m_scriptObject;
    if ( py_property ) Py_INCREF(py_property);
    else py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);

    PyObject* py_pos;
    py_pos = wxPoint_to_PyObject((&pos));

    PyObject* py_sz;
    py_sz = wxSize_to_PyObject((&sz));

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_propgrid, py_property, py_pos, py_sz, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_sz);
    Py_DECREF(py_pos);
    Py_DECREF(py_property);
    Py_DECREF(py_propgrid);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxPGWindowList retval;
    if ( !PyObject_to_wxPGWindowList(res, &retval) ) {
            PyErr_SetString(PyExc_TypeError, "expected wxWindow or tuple of two wxWindows");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxPGWindowList();
}

wxPGVariantAndBool _CommonCallback8(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPGProperty* property, wxWindow* ctrl)
{
    PyObject* res;
    PyObject* py_property;
    py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);
    PyObject* py_ctrl;
    py_ctrl = wxPyMake_wxObject(ctrl, (bool)0);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_property, py_ctrl, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_ctrl);
    Py_DECREF(py_property);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxPGVariantAndBool retval;
    if ( !PyObject_to_wxPGVariantAndBool(res, retval) ) {
            PyErr_SetString(PyExc_TypeError, "this Python type cannot be converted to wxVariant");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxPGVariantAndBool();
}

wxPGVariantAndBool _CommonCallback27(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, const wxString& text, int argFlags)
{
    PyObject* res;
    PyObject* py_text;
    #if wxUSE_UNICODE
        py_text = PyUnicode_FromWideChar((&text)->c_str(), (&text)->Len());
    #else
        py_text = PyString_FromStringAndSize((&text)->c_str(), (&text)->Len());
    #endif

    PyObject* py_argFlags;
    py_argFlags = PyInt_FromLong((long)argFlags);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_text, py_argFlags, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_argFlags);
    Py_DECREF(py_text);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxPGVariantAndBool retval;
    if ( !PyObject_to_wxPGVariantAndBool(res, retval) ) {
            PyErr_SetString(PyExc_TypeError, "this Python type cannot be converted to wxVariant");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxPGVariantAndBool();
}

bool _CommonCallback0(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj)
{
    PyObject* res;
    res = PyObject_CallFunctionObjArgs(funcobj, self, NULL);
    Py_DECREF(funcobj);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
}

bool _CommonCallback36(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event)
{
    PyObject* res;
    PyObject* py_propgrid;
    py_propgrid = SWIG_NewPointerObj((void*)propgrid, SWIGTYPE_p_wxPropertyGrid, 0);
    PyObject* py_property;
        py_property = NULL;
    if ( property->m_scriptObject ) py_property = (PyObject*)property->m_scriptObject;
    if ( py_property ) Py_INCREF(py_property);
    else py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);

    PyObject* py_primary;
    py_primary = wxPyMake_wxObject(primary, (bool)0);

    PyObject* py_event;
    py_event = SWIG_NewPointerObj((void*)&event, SWIGTYPE_p_wxEvent, 0);
    res = PyObject_CallFunctionObjArgs(funcobj, self, py_propgrid, py_property, py_primary, py_event, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_event);
    Py_DECREF(py_primary);
    Py_DECREF(py_property);
    Py_DECREF(py_propgrid);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
}

void _CommonCallback10(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPGProperty* property, wxWindow* ctrl, const wxString& txt)
{
    PyObject* res;
    PyObject* py_property;
    py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);
    PyObject* py_ctrl;
    py_ctrl = wxPyMake_wxObject(ctrl, (bool)0);

    PyObject* py_txt;
    #if wxUSE_UNICODE
        py_txt = PyUnicode_FromWideChar((&txt)->c_str(), (&txt)->Len());
    #else
        py_txt = PyString_FromStringAndSize((&txt)->c_str(), (&txt)->Len());
    #endif

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_property, py_ctrl, py_txt, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_txt);
    Py_DECREF(py_ctrl);
    Py_DECREF(py_property);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

bool _CommonCallback15(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, const wxString& name, wxVariant& value)
{
    PyObject* res;
    PyObject* py_name;
    #if wxUSE_UNICODE
        py_name = PyUnicode_FromWideChar((&name)->c_str(), (&name)->Len());
    #else
        py_name = PyString_FromStringAndSize((&name)->c_str(), (&name)->Len());
    #endif

    PyObject* py_value;
    py_value = wxVariant_to_PyObject((&value));
        if ( !py_value ) {
            PyErr_SetString(PyExc_TypeError, "this wxVariant type cannot be converted to Python object");
            SWIG_fail;
        }

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_name, py_value, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_value);
    Py_DECREF(py_name);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
}

void _CommonCallback7(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPGProperty* property, wxWindow* wnd)
{
    PyObject* res;
    PyObject* py_property;
    py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);
    PyObject* py_wnd;
    py_wnd = wxPyMake_wxObject(wnd, (bool)0);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_property, py_wnd, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_wnd);
    Py_DECREF(py_property);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

wxString _CommonCallback4(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj)
{
    PyObject* res;
    res = PyObject_CallFunctionObjArgs(funcobj, self, NULL);
    Py_DECREF(funcobj);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxString retval;
    wxString* sptr = wxString_in_helper(res);
        if (sptr == NULL) SWIG_fail;
        retval = *sptr;
        delete sptr;
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxEmptyString;
}

void _CommonCallback37(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPGProperty* property, wxWindow* wnd)
{
    PyObject* res;
    PyObject* py_property;
        py_property = NULL;
    if ( property->m_scriptObject ) py_property = (PyObject*)property->m_scriptObject;
    if ( py_property ) Py_INCREF(py_property);
    else py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);

    PyObject* py_wnd;
    py_wnd = wxPyMake_wxObject(wnd, (bool)0);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_property, py_wnd, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_wnd);
    Py_DECREF(py_property);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

void _CommonCallback24(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj)
{
    PyObject* res;
    res = PyObject_CallFunctionObjArgs(funcobj, self, NULL);
    Py_DECREF(funcobj);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

void _CommonCallback35(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text)
{
    PyObject* res;
    PyObject* py_dc;
    py_dc = SWIG_NewPointerObj((void*)&dc, SWIGTYPE_p_wxDC, 0);
    PyObject* py_rect;
    py_rect = SWIG_NewPointerObj((void*)&rect, SWIGTYPE_p_wxRect, 0);
    PyObject* py_property;
        py_property = NULL;
    if ( property->m_scriptObject ) py_property = (PyObject*)property->m_scriptObject;
    if ( py_property ) Py_INCREF(py_property);
    else py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);

    PyObject* py_text;
    #if wxUSE_UNICODE
        py_text = PyUnicode_FromWideChar((&text)->c_str(), (&text)->Len());
    #else
        py_text = PyString_FromStringAndSize((&text)->c_str(), (&text)->Len());
    #endif

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_dc, py_rect, py_property, py_text, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_text);
    Py_DECREF(py_property);
    Py_DECREF(py_rect);
    Py_DECREF(py_dc);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

wxVariant _CommonCallback12(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, const wxString& name)
{
    PyObject* res;
    PyObject* py_name;
    #if wxUSE_UNICODE
        py_name = PyUnicode_FromWideChar((&name)->c_str(), (&name)->Len());
    #else
        py_name = PyString_FromStringAndSize((&name)->c_str(), (&name)->Len());
    #endif

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_name, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_name);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxVariant retval;
    if ( !PyObject_to_wxVariant(res, &retval) ) {
            PyErr_SetString(PyExc_TypeError, "this Python type cannot be converted to wxVariant");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxVariant();
}

wxPGCellRenderer* _CommonCallback16(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, int column)
{
    PyObject* res;
    PyObject* py_column;
    py_column = PyInt_FromLong((long)column);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_column, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_column);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxPGCellRenderer* retval;
    if ( !SWIG_IsOK(SWIG_ConvertPtr(res, (void**)&(retval), SWIGTYPE_p_wxPGCellRenderer, 0)) ) {
        PyErr_SetString(PyExc_TypeError,"expected wxPGCellRenderer");
        SWIG_fail;
    }
    PyObject_SetAttrString(res, "thisown", Py_False);
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return NULL;
}

const wxString* _CommonCallback30(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, size_t index, int* pvalue)
{
    PyObject* tpl;
    PyObject* py_index;
    py_index = PyInt_FromLong((size_t)index);

    tpl = PyObject_CallFunctionObjArgs(funcobj, self, py_index, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_index);
    if (PyErr_Occurred()) SWIG_fail;
    {
    const wxString* retval;
    long tpl_count = -1;
    if ( PySequence_Check(tpl) ) {
        PyObject* py_tpl_count = PyInt_FromLong((long)0);
        PySequence_Count(tpl, py_tpl_count);
        tpl_count = PyInt_AsLong(py_tpl_count);
        Py_DECREF(py_tpl_count);
    }
    if ( tpl_count != 2 ) {
        Py_DECREF(tpl);
        PyErr_SetString(PyExc_TypeError, "Expected tuple of 2 items as a return value.");
        SWIG_fail;
    }

    PyObject* res;
    res = PySequence_GetItem(tpl, 0);
    bool temp = false;    retval = wxString_in_helper(res);
        if (retval == NULL) SWIG_fail;
        temp = true;
    PyObject_SetAttrString(res, "thisown", Py_False);
    Py_DECREF(res);

    res = PySequence_GetItem(tpl, 1);
    *pvalue = (int)PyInt_AS_LONG(res);
    Py_DECREF(res);

    Py_DECREF(tpl);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return NULL;
}

void _CommonCallback40(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPGProperty* property, wxWindow* ctrl, const wxString& txt)
{
    PyObject* res;
    PyObject* py_property;
        py_property = NULL;
    if ( property->m_scriptObject ) py_property = (PyObject*)property->m_scriptObject;
    if ( py_property ) Py_INCREF(py_property);
    else py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);

    PyObject* py_ctrl;
    py_ctrl = wxPyMake_wxObject(ctrl, (bool)0);

    PyObject* py_txt;
    #if wxUSE_UNICODE
        py_txt = PyUnicode_FromWideChar((&txt)->c_str(), (&txt)->Len());
    #else
        py_txt = PyString_FromStringAndSize((&txt)->c_str(), (&txt)->Len());
    #endif

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_property, py_ctrl, py_txt, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_txt);
    Py_DECREF(py_ctrl);
    Py_DECREF(py_property);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

bool _CommonCallback29(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPropertyGrid* propGrid, wxPGProperty* property)
{
    PyObject* res;
    PyObject* py_propGrid;
    py_propGrid = SWIG_NewPointerObj((void*)propGrid, SWIGTYPE_p_wxPropertyGrid, 0);
    PyObject* py_property;
    py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);
    res = PyObject_CallFunctionObjArgs(funcobj, self, py_propGrid, py_property, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_property);
    Py_DECREF(py_propGrid);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
}

int _CommonCallback31(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, int value)
{
    PyObject* res;
    PyObject* py_value;
    py_value = PyInt_FromLong((long)value);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_value, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_value);
    if (PyErr_Occurred()) SWIG_fail;
    {
    int retval;
    retval = (int)PyInt_AS_LONG(res);
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return 0;
}

wxString _CommonCallback20(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, int argFlags)
{
    PyObject* res;
    PyObject* py_argFlags;
    py_argFlags = PyInt_FromLong((long)argFlags);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_argFlags, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_argFlags);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxString retval;
    wxString* sptr = wxString_in_helper(res);
        if (sptr == NULL) SWIG_fail;
        retval = *sptr;
        delete sptr;
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxEmptyString;
}

wxPGEditorDialogAdapter* _CommonCallback18(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj)
{
    PyObject* res;
    res = PyObject_CallFunctionObjArgs(funcobj, self, NULL);
    Py_DECREF(funcobj);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxPGEditorDialogAdapter* retval;
    if ( !SWIG_IsOK(SWIG_ConvertPtr(res, (void**)&(retval), SWIGTYPE_p_wxPGEditorDialogAdapter, 0)) ) {
        PyErr_SetString(PyExc_TypeError,"expected wxPGEditorDialogAdapter");
        SWIG_fail;
    }
    PyObject_SetAttrString(res, "thisown", Py_False);
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return NULL;
}

void _CommonCallback25(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxVariant& pendingValue)
{
    PyObject* res;
    PyObject* py_pendingValue;
    py_pendingValue = wxVariant_to_PyObject((&pendingValue));
        if ( !py_pendingValue ) {
            PyErr_SetString(PyExc_TypeError, "this wxVariant type cannot be converted to Python object");
            SWIG_fail;
        }

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_pendingValue, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_pendingValue);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

void _CommonCallback2(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxWindow* ctrl, int index)
{
    PyObject* res;
    PyObject* py_ctrl;
    py_ctrl = wxPyMake_wxObject(ctrl, (bool)0);

    PyObject* py_index;
    py_index = PyInt_FromLong((long)index);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_ctrl, py_index, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_index);
    Py_DECREF(py_ctrl);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

bool _CommonCallback45(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxPropertyGrid* propGrid, wxString& value)
{
    PyObject* res;
    PyObject* py_propGrid;
    py_propGrid = SWIG_NewPointerObj((void*)propGrid, SWIGTYPE_p_wxPropertyGrid, 0);
    PyObject* py_value;
    #if wxUSE_UNICODE
        py_value = PyUnicode_FromWideChar((&value)->c_str(), (&value)->Len());
    #else
        py_value = PyString_FromStringAndSize((&value)->c_str(), (&value)->Len());
    #endif

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_propGrid, py_value, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_value);
    Py_DECREF(py_propGrid);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
}

void _CommonCallback21(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    PyObject* res;
    PyObject* py_dc;
    py_dc = SWIG_NewPointerObj((void*)&dc, SWIGTYPE_p_wxDC, 0);
    PyObject* py_rect;
    py_rect = SWIG_NewPointerObj((void*)&rect, SWIGTYPE_p_wxRect, 0);
    PyObject* py_paintdata;
    py_paintdata = SWIG_NewPointerObj((void*)&paintdata, SWIGTYPE_p_wxPGPaintData, 0);
    res = PyObject_CallFunctionObjArgs(funcobj, self, py_dc, py_rect, py_paintdata, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_paintdata);
    Py_DECREF(py_rect);
    Py_DECREF(py_dc);
    if (PyErr_Occurred()) SWIG_fail;
    {
    Py_DECREF(res);
    }
  fail:
    wxPyEndBlockThreads(blocked);
}

wxPGVariantAndBool _CommonCallback28(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, const wxVariant& value, wxPGValidationInfo& validationInfo)
{
    PyObject* res;
    PyObject* py_value;
    py_value = wxVariant_to_PyObject((&value));
        if ( !py_value ) {
            PyErr_SetString(PyExc_TypeError, "this wxVariant type cannot be converted to Python object");
            SWIG_fail;
        }

    PyObject* py_validationInfo;
    py_validationInfo = SWIG_NewPointerObj((void*)&validationInfo, SWIGTYPE_p_wxPGValidationInfo, 0);
    res = PyObject_CallFunctionObjArgs(funcobj, self, py_value, py_validationInfo, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_validationInfo);
    Py_DECREF(py_value);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxPGVariantAndBool retval;
    if ( !PyObject_to_wxPGVariantAndBool(res, retval) ) {
            PyErr_SetString(PyExc_TypeError, "this Python type cannot be converted to wxVariant");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxPGVariantAndBool();
}

int _CommonCallback5(wxPyBlock_t blocked, PyObject* self, PyObject* funcobj, wxWindow* ctrl, const wxString& label, int index)
{
    PyObject* res;
    PyObject* py_ctrl;
    py_ctrl = wxPyMake_wxObject(ctrl, (bool)0);

    PyObject* py_label;
    #if wxUSE_UNICODE
        py_label = PyUnicode_FromWideChar((&label)->c_str(), (&label)->Len());
    #else
        py_label = PyString_FromStringAndSize((&label)->c_str(), (&label)->Len());
    #endif

    PyObject* py_index;
    py_index = PyInt_FromLong((long)index);

    res = PyObject_CallFunctionObjArgs(funcobj, self, py_ctrl, py_label, py_index, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_index);
    Py_DECREF(py_label);
    Py_DECREF(py_ctrl);
    if (PyErr_Occurred()) SWIG_fail;
    {
    int retval;
    retval = (int)PyInt_AS_LONG(res);
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return 0;
}

class PyCheckBoxEditor : public wxPGCheckBoxEditor {
public:
        PyCheckBoxEditor();
    virtual ~PyCheckBoxEditor();
    void _SetSelf(PyObject *self);
    virtual bool CanContainCustomImage() const;
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz ) const;
    virtual void DeleteItem( wxWindow* ctrl, int index ) const;
    virtual void DrawValue( wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text ) const;
    virtual wxString GetName() const;
    virtual int InsertItem( wxWindow* ctrl, const wxString& label, int index ) const;
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event ) const;
    virtual void OnFocus( wxPGProperty* property, wxWindow* wnd ) const;
    virtual wxPGVariantAndBool PyGetValueFromControl( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const;
    virtual void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const;
    virtual void SetValueToUnspecified( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const;
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyCheckBoxEditor_pyClass = NULL;


PyCheckBoxEditor::PyCheckBoxEditor()
    : wxPGCheckBoxEditor()
{
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::PyCheckBoxEditor()"));

    Init();
}

PyCheckBoxEditor::~PyCheckBoxEditor()
{
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::~PyCheckBoxEditor()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyCheckBoxEditor::_SetSelf(PyObject *self)
{
    if ( !gs_PyCheckBoxEditor_pyClass )
    {
        gs_PyCheckBoxEditor_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyCheckBoxEditor_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyCheckBoxEditor::CanContainCustomImage() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::CanContainCustomImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CanContainCustomImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CanContainCustomImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::CanContainCustomImage() exit (fall-back)"));
        return wxPGCheckBoxEditor::CanContainCustomImage();
    }
    return _CommonCallback0(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::CanContainCustomImage() exit"));
}


wxPGWindowList PyCheckBoxEditor::CreateControls(wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::CreateControls() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CreateControls_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CreateControls_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::CreateControls() exit (fall-back)"));
        return wxPGCheckBoxEditor::CreateControls(propgrid, property, pos, sz);
    }
    return _CommonCallback1(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, pos, sz);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::CreateControls() exit"));
}


void PyCheckBoxEditor::DeleteItem(wxWindow* ctrl, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::DeleteItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DeleteItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DeleteItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::DeleteItem() exit (fall-back)"));
        wxPGCheckBoxEditor::DeleteItem(ctrl, index);
        return;
    }
    _CommonCallback2(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, index);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::DeleteItem() exit"));
}


void PyCheckBoxEditor::DrawValue(wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::DrawValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DrawValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DrawValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::DrawValue() exit (fall-back)"));
        wxPGCheckBoxEditor::DrawValue(dc, rect, property, text);
        return;
    }
    _CommonCallback3(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, property, text);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::DrawValue() exit"));
}


wxString PyCheckBoxEditor::GetName() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::GetName() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetName_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetName_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::GetName() exit (fall-back)"));
        return wxPGCheckBoxEditor::GetName();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::GetName() exit"));
}


int PyCheckBoxEditor::InsertItem(wxWindow* ctrl, const wxString& label, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::InsertItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_InsertItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_InsertItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::InsertItem() exit (fall-back)"));
        return wxPGCheckBoxEditor::InsertItem(ctrl, label, index);
    }
    return _CommonCallback5(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, label, index);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::InsertItem() exit"));
}


bool PyCheckBoxEditor::OnEvent(wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::OnEvent() exit (fall-back)"));
        return wxPGCheckBoxEditor::OnEvent(propgrid, property, primary, event);
    }
    return _CommonCallback6(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, primary, event);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::OnEvent() exit"));
}


void PyCheckBoxEditor::OnFocus(wxPGProperty* property, wxWindow* wnd) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::OnFocus() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnFocus_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnFocus_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::OnFocus() exit (fall-back)"));
        wxPGCheckBoxEditor::OnFocus(property, wnd);
        return;
    }
    _CommonCallback7(blocked, (PyObject*)m_scriptObject, funcobj, property, wnd);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::OnFocus() exit"));
}


wxPGVariantAndBool PyCheckBoxEditor::PyGetValueFromControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::PyGetValueFromControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueFromControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueFromControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::PyGetValueFromControl() exit (fall-back)"));
        return wxPGCheckBoxEditor::PyGetValueFromControl(property, ctrl);
    }
    return _CommonCallback8(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::PyGetValueFromControl() exit"));
}


void PyCheckBoxEditor::SetControlIntValue(wxPGProperty* property, wxWindow* ctrl, int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::SetControlIntValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlIntValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlIntValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::SetControlIntValue() exit (fall-back)"));
        wxPGCheckBoxEditor::SetControlIntValue(property, ctrl, value);
        return;
    }
    _CommonCallback9(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, value);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::SetControlIntValue() exit"));
}


void PyCheckBoxEditor::SetControlStringValue(wxPGProperty* property, wxWindow* ctrl, const wxString& txt) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::SetControlStringValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlStringValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlStringValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::SetControlStringValue() exit (fall-back)"));
        wxPGCheckBoxEditor::SetControlStringValue(property, ctrl, txt);
        return;
    }
    _CommonCallback10(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, txt);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::SetControlStringValue() exit"));
}


void PyCheckBoxEditor::SetValueToUnspecified(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::SetValueToUnspecified() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetValueToUnspecified_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetValueToUnspecified_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::SetValueToUnspecified() exit (fall-back)"));
        wxPGCheckBoxEditor::SetValueToUnspecified(property, ctrl);
        return;
    }
    _CommonCallback7(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::SetValueToUnspecified() exit"));
}


void PyCheckBoxEditor::UpdateControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::UpdateControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_UpdateControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_UpdateControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCheckBoxEditor::UpdateControl() exit (fall-back)"));
        wxPGCheckBoxEditor::UpdateControl(property, ctrl);
        return;
    }
    _CommonCallback7(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyCheckBoxEditor::UpdateControl() exit"));
}


class PyFloatProperty : public wxFloatProperty {
public:
        PyFloatProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL,
                     double value = 0.0 );
    virtual ~PyFloatProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyFloatProperty_pyClass = NULL;


PyFloatProperty::PyFloatProperty(const wxString& label, const wxString& name, double value)
    : wxFloatProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyFloatProperty::PyFloatProperty()"));

    Init();
}

PyFloatProperty::~PyFloatProperty()
{
    MySWIGOutputDebugString(wxT("PyFloatProperty::~PyFloatProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyFloatProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyFloatProperty_pyClass )
    {
        gs_PyFloatProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyFloatProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyFloatProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::ChildChanged() exit (fall-back)"));
        wxFloatProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyFloatProperty::ChildChanged() exit"));
}


wxVariant PyFloatProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::DoGetAttribute() exit (fall-back)"));
        return wxFloatProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyFloatProperty::DoGetAttribute() exit"));
}


wxValidator* PyFloatProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::DoGetValidator() exit (fall-back)"));
        return wxFloatProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFloatProperty::DoGetValidator() exit"));
}


wxVariant PyFloatProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::DoGetValue() exit (fall-back)"));
        return wxFloatProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFloatProperty::DoGetValue() exit"));
}


bool PyFloatProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::DoSetAttribute() exit (fall-back)"));
        return wxFloatProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyFloatProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyFloatProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::GetCellRenderer() exit (fall-back)"));
        return wxFloatProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetCellRenderer() exit"));
}


int PyFloatProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::GetChoiceInfo() exit (fall-back)"));
        return wxFloatProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetChoiceInfo() exit"));
}


wxString PyFloatProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::GetEditor() exit (fall-back)"));
        return wxFloatProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyFloatProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::GetEditorDialog() exit (fall-back)"));
        return wxFloatProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetEditorDialog() exit"));
}


wxSize PyFloatProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::GetImageSize() exit (fall-back)"));
        return wxFloatProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetImageSize() exit"));
}


wxString PyFloatProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::GetValueAsString() exit (fall-back)"));
        return wxFloatProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyFloatProperty::GetValueAsString() exit"));
}


void PyFloatProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::OnCustomPaint() exit (fall-back)"));
        wxFloatProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnCustomPaint() exit"));
}


bool PyFloatProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::OnEvent() exit (fall-back)"));
        return wxFloatProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnEvent() exit"));
}


wxSize PyFloatProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::OnMeasureImage() exit (fall-back)"));
        return wxFloatProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnMeasureImage() exit"));
}


void PyFloatProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::OnSetValue() exit (fall-back)"));
        wxFloatProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnSetValue() exit"));
}


void PyFloatProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::OnValidationFailure() exit (fall-back)"));
        wxFloatProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyFloatProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyFloatProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::PyIntToValue() exit (fall-back)"));
        return wxFloatProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyFloatProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyFloatProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::PyStringToValue() exit (fall-back)"));
        return wxFloatProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyFloatProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyFloatProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::PyValidateValue() exit (fall-back)"));
        return wxFloatProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyFloatProperty::PyValidateValue() exit"));
}


void PyFloatProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFloatProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFloatProperty::RefreshChildren() exit (fall-back)"));
        wxFloatProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFloatProperty::RefreshChildren() exit"));
}


class PyEditorDialogAdapter : public wxPGEditorDialogAdapter {
public:
        PyEditorDialogAdapter();
    virtual ~PyEditorDialogAdapter();
    void _SetSelf(PyObject *self);
    virtual bool DoShowDialog( wxPropertyGrid* propGrid, wxPGProperty* property );
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyEditorDialogAdapter_pyClass = NULL;


PyEditorDialogAdapter::PyEditorDialogAdapter()
    : wxPGEditorDialogAdapter()
{
    MySWIGOutputDebugString(wxT("PyEditorDialogAdapter::PyEditorDialogAdapter()"));

    Init();
}

PyEditorDialogAdapter::~PyEditorDialogAdapter()
{
    MySWIGOutputDebugString(wxT("PyEditorDialogAdapter::~PyEditorDialogAdapter()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyEditorDialogAdapter::_SetSelf(PyObject *self)
{
    if ( !gs_PyEditorDialogAdapter_pyClass )
    {
        gs_PyEditorDialogAdapter_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyEditorDialogAdapter_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyEditorDialogAdapter::DoShowDialog(wxPropertyGrid* propGrid, wxPGProperty* property)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditorDialogAdapter::DoShowDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoShowDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoShowDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditorDialogAdapter::DoShowDialog() exit (not implemented!!!)"));
        PyErr_SetString(PyExc_TypeError,"this method must be implemented");
        return false;
    }
    return _CommonCallback29(blocked, (PyObject*)m_scriptObject, funcobj, propGrid, property);
    MySWIGOutputDebugString(wxT("PyEditorDialogAdapter::DoShowDialog() exit"));
}


class PyEnumProperty : public wxEnumProperty {
public:
        PyEnumProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL,
        const wxArrayString& labels = wxArrayString(), const wxArrayInt& values = wxArrayInt(),
        int value = 0 );
    virtual ~PyEnumProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual const wxString* GetEntry( size_t index, int* pvalue ) const;
    virtual wxSize GetImageSize() const;
    virtual int GetIndexForValue( int value ) const;
    virtual wxString GetValueAsString( int argFlags ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyEnumProperty_pyClass = NULL;


PyEnumProperty::PyEnumProperty(const wxString& label, const wxString& name, const wxArrayString& labels, const wxArrayInt& values, int value)
    : wxEnumProperty(label, name, labels, values, value)
{
    MySWIGOutputDebugString(wxT("PyEnumProperty::PyEnumProperty()"));

    Init();
}

PyEnumProperty::~PyEnumProperty()
{
    MySWIGOutputDebugString(wxT("PyEnumProperty::~PyEnumProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyEnumProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyEnumProperty_pyClass )
    {
        gs_PyEnumProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyEnumProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyEnumProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::ChildChanged() exit (fall-back)"));
        wxEnumProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyEnumProperty::ChildChanged() exit"));
}


wxVariant PyEnumProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::DoGetAttribute() exit (fall-back)"));
        return wxEnumProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyEnumProperty::DoGetAttribute() exit"));
}


wxValidator* PyEnumProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::DoGetValidator() exit (fall-back)"));
        return wxEnumProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEnumProperty::DoGetValidator() exit"));
}


wxVariant PyEnumProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::DoGetValue() exit (fall-back)"));
        return wxEnumProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEnumProperty::DoGetValue() exit"));
}


bool PyEnumProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::DoSetAttribute() exit (fall-back)"));
        return wxEnumProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyEnumProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyEnumProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::GetCellRenderer() exit (fall-back)"));
        return wxEnumProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetCellRenderer() exit"));
}


int PyEnumProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::GetChoiceInfo() exit (fall-back)"));
        return wxEnumProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetChoiceInfo() exit"));
}


wxString PyEnumProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::GetEditor() exit (fall-back)"));
        return wxEnumProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyEnumProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::GetEditorDialog() exit (fall-back)"));
        return wxEnumProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetEditorDialog() exit"));
}


const wxString* PyEnumProperty::GetEntry(size_t index, int* pvalue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetEntry() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEntry_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEntry_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::GetEntry() exit (fall-back)"));
        return wxEnumProperty::GetEntry(index, pvalue);
    }
    return _CommonCallback30(blocked, (PyObject*)m_scriptObject, funcobj, index, pvalue);
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetEntry() exit"));
}


wxSize PyEnumProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::GetImageSize() exit (fall-back)"));
        return wxEnumProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetImageSize() exit"));
}


int PyEnumProperty::GetIndexForValue(int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetIndexForValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetIndexForValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetIndexForValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::GetIndexForValue() exit (fall-back)"));
        return wxEnumProperty::GetIndexForValue(value);
    }
    return _CommonCallback31(blocked, (PyObject*)m_scriptObject, funcobj, value);
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetIndexForValue() exit"));
}


wxString PyEnumProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::GetValueAsString() exit (fall-back)"));
        return wxEnumProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyEnumProperty::GetValueAsString() exit"));
}


void PyEnumProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::OnCustomPaint() exit (fall-back)"));
        wxEnumProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnCustomPaint() exit"));
}


bool PyEnumProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::OnEvent() exit (fall-back)"));
        return wxEnumProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnEvent() exit"));
}


wxSize PyEnumProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::OnMeasureImage() exit (fall-back)"));
        return wxEnumProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnMeasureImage() exit"));
}


void PyEnumProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::OnSetValue() exit (fall-back)"));
        wxEnumProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnSetValue() exit"));
}


void PyEnumProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::OnValidationFailure() exit (fall-back)"));
        wxEnumProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyEnumProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyEnumProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::PyIntToValue() exit (fall-back)"));
        return wxEnumProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyEnumProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyEnumProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::PyStringToValue() exit (fall-back)"));
        return wxEnumProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyEnumProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyEnumProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::PyValidateValue() exit (fall-back)"));
        return wxEnumProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyEnumProperty::PyValidateValue() exit"));
}


void PyEnumProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEnumProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEnumProperty::RefreshChildren() exit (fall-back)"));
        wxEnumProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEnumProperty::RefreshChildren() exit"));
}


class PyArrayStringProperty : public wxArrayStringProperty {
public:
        PyArrayStringProperty( const wxString& label = wxString_wxPG_LABEL,
                           const wxString& name = wxString_wxPG_LABEL,
                           const wxArrayString& value = wxArrayString() );
    virtual ~PyArrayStringProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual void GenerateValueAsString();
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual bool OnButtonClick( wxPropertyGrid* propgrid,
                                wxWindow* primary,
                                const wxChar* cbt );
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnCustomStringEdit( wxWindow* parent, wxString& value );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyArrayStringProperty_pyClass = NULL;


PyArrayStringProperty::PyArrayStringProperty(const wxString& label, const wxString& name, const wxArrayString& value)
    : wxArrayStringProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyArrayStringProperty()"));

    Init();
}

PyArrayStringProperty::~PyArrayStringProperty()
{
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::~PyArrayStringProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyArrayStringProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyArrayStringProperty_pyClass )
    {
        gs_PyArrayStringProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyArrayStringProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyArrayStringProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::ChildChanged() exit (fall-back)"));
        wxArrayStringProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::ChildChanged() exit"));
}


wxVariant PyArrayStringProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoGetAttribute() exit (fall-back)"));
        return wxArrayStringProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoGetAttribute() exit"));
}


wxValidator* PyArrayStringProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoGetValidator() exit (fall-back)"));
        return wxArrayStringProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoGetValidator() exit"));
}


wxVariant PyArrayStringProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoGetValue() exit (fall-back)"));
        return wxArrayStringProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoGetValue() exit"));
}


bool PyArrayStringProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoSetAttribute() exit (fall-back)"));
        return wxArrayStringProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::DoSetAttribute() exit"));
}


void PyArrayStringProperty::GenerateValueAsString()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GenerateValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GenerateValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GenerateValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::GenerateValueAsString() exit (fall-back)"));
        wxArrayStringProperty::GenerateValueAsString();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GenerateValueAsString() exit"));
}


wxPGCellRenderer* PyArrayStringProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetCellRenderer() exit (fall-back)"));
        return wxArrayStringProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetCellRenderer() exit"));
}


int PyArrayStringProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetChoiceInfo() exit (fall-back)"));
        return wxArrayStringProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetChoiceInfo() exit"));
}


wxString PyArrayStringProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetEditor() exit (fall-back)"));
        return wxArrayStringProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyArrayStringProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetEditorDialog() exit (fall-back)"));
        return wxArrayStringProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetEditorDialog() exit"));
}


wxSize PyArrayStringProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetImageSize() exit (fall-back)"));
        return wxArrayStringProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetImageSize() exit"));
}


wxString PyArrayStringProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetValueAsString() exit (fall-back)"));
        return wxArrayStringProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::GetValueAsString() exit"));
}


bool PyArrayStringProperty::OnButtonClick(wxPropertyGrid* propgrid, wxWindow* primary, const wxChar* cbt)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnButtonClick() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnButtonClick_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnButtonClick_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnButtonClick() exit (fall-back)"));
        return wxArrayStringProperty::OnButtonClick(propgrid, primary, cbt);
    }
    PyObject* res;
    PyObject* py_propgrid;
    py_propgrid = SWIG_NewPointerObj((void*)propgrid, SWIGTYPE_p_wxPropertyGrid, 0);
    PyObject* py_primary;
    py_primary = wxPyMake_wxObject(primary, (bool)0);

    PyObject* py_cbt;
    #if wxUSE_UNICODE
        py_cbt = PyUnicode_FromWideChar(cbt, wxStrlen(cbt));
    #else
        py_cbt = PyString_FromStringAndSize(cbt, wxStrlen(cbt));
    #endif

    res = PyObject_CallFunctionObjArgs(funcobj, ((PyObject*)m_scriptObject), py_propgrid, py_primary, py_cbt, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_cbt);
    Py_DECREF(py_primary);
    Py_DECREF(py_propgrid);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnButtonClick() exit"));
}


void PyArrayStringProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnCustomPaint() exit (fall-back)"));
        wxArrayStringProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnCustomPaint() exit"));
}


bool PyArrayStringProperty::OnCustomStringEdit(wxWindow* parent, wxString& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnCustomStringEdit() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomStringEdit_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomStringEdit_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnCustomStringEdit() exit (fall-back)"));
        return wxArrayStringProperty::OnCustomStringEdit(parent, value);
    }
    PyObject* res;
    PyObject* py_parent;
    py_parent = wxPyMake_wxObject(parent, (bool)0);

    PyObject* py_value;
    #if wxUSE_UNICODE
        py_value = PyUnicode_FromWideChar((&value)->c_str(), (&value)->Len());
    #else
        py_value = PyString_FromStringAndSize((&value)->c_str(), (&value)->Len());
    #endif

    res = PyObject_CallFunctionObjArgs(funcobj, ((PyObject*)m_scriptObject), py_parent, py_value, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_value);
    Py_DECREF(py_parent);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnCustomStringEdit() exit"));
}


bool PyArrayStringProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnEvent() exit (fall-back)"));
        return wxArrayStringProperty::OnEvent(propgrid, primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, primary, event);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnEvent() exit"));
}


wxSize PyArrayStringProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnMeasureImage() exit (fall-back)"));
        return wxArrayStringProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnMeasureImage() exit"));
}


void PyArrayStringProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnSetValue() exit (fall-back)"));
        wxArrayStringProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnSetValue() exit"));
}


void PyArrayStringProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnValidationFailure() exit (fall-back)"));
        wxArrayStringProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyArrayStringProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyIntToValue() exit (fall-back)"));
        return wxArrayStringProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyArrayStringProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyStringToValue() exit (fall-back)"));
        return wxArrayStringProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyArrayStringProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyValidateValue() exit (fall-back)"));
        return wxArrayStringProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::PyValidateValue() exit"));
}


void PyArrayStringProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyArrayStringProperty::RefreshChildren() exit (fall-back)"));
        wxArrayStringProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyArrayStringProperty::RefreshChildren() exit"));
}


class PyBaseEnumProperty : public wxBaseEnumProperty {
public:
        PyBaseEnumProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL );
    virtual ~PyBaseEnumProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual const wxString* GetEntry( size_t index, int* pvalue ) const;
    virtual wxSize GetImageSize() const;
    virtual int GetIndexForValue( int value ) const;
    virtual wxString GetValueAsString( int argFlags ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyBaseEnumProperty_pyClass = NULL;


PyBaseEnumProperty::PyBaseEnumProperty(const wxString& label, const wxString& name)
    : wxBaseEnumProperty(label, name)
{
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyBaseEnumProperty()"));

    Init();
}

PyBaseEnumProperty::~PyBaseEnumProperty()
{
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::~PyBaseEnumProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyBaseEnumProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyBaseEnumProperty_pyClass )
    {
        gs_PyBaseEnumProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyBaseEnumProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyBaseEnumProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::ChildChanged() exit (fall-back)"));
        wxBaseEnumProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::ChildChanged() exit"));
}


wxVariant PyBaseEnumProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoGetAttribute() exit (fall-back)"));
        return wxBaseEnumProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoGetAttribute() exit"));
}


wxValidator* PyBaseEnumProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoGetValidator() exit (fall-back)"));
        return wxBaseEnumProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoGetValidator() exit"));
}


wxVariant PyBaseEnumProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoGetValue() exit (fall-back)"));
        return wxBaseEnumProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoGetValue() exit"));
}


bool PyBaseEnumProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoSetAttribute() exit (fall-back)"));
        return wxBaseEnumProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyBaseEnumProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetCellRenderer() exit (fall-back)"));
        return wxBaseEnumProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetCellRenderer() exit"));
}


int PyBaseEnumProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetChoiceInfo() exit (fall-back)"));
        return wxBaseEnumProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetChoiceInfo() exit"));
}


wxString PyBaseEnumProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetEditor() exit (fall-back)"));
        return wxBaseEnumProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyBaseEnumProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetEditorDialog() exit (fall-back)"));
        return wxBaseEnumProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetEditorDialog() exit"));
}


const wxString* PyBaseEnumProperty::GetEntry(size_t index, int* pvalue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetEntry() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEntry_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEntry_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetEntry() exit (not implemented!!!)"));
        PyErr_SetString(PyExc_TypeError,"this method must be implemented");
        return NULL;
    }
    return _CommonCallback30(blocked, (PyObject*)m_scriptObject, funcobj, index, pvalue);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetEntry() exit"));
}


wxSize PyBaseEnumProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetImageSize() exit (fall-back)"));
        return wxBaseEnumProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetImageSize() exit"));
}


int PyBaseEnumProperty::GetIndexForValue(int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetIndexForValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetIndexForValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetIndexForValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetIndexForValue() exit (fall-back)"));
        return wxBaseEnumProperty::GetIndexForValue(value);
    }
    return _CommonCallback31(blocked, (PyObject*)m_scriptObject, funcobj, value);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetIndexForValue() exit"));
}


wxString PyBaseEnumProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetValueAsString() exit (fall-back)"));
        return wxBaseEnumProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::GetValueAsString() exit"));
}


void PyBaseEnumProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnCustomPaint() exit (fall-back)"));
        wxBaseEnumProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnCustomPaint() exit"));
}


bool PyBaseEnumProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnEvent() exit (fall-back)"));
        return wxBaseEnumProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnEvent() exit"));
}


wxSize PyBaseEnumProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnMeasureImage() exit (fall-back)"));
        return wxBaseEnumProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnMeasureImage() exit"));
}


void PyBaseEnumProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnSetValue() exit (fall-back)"));
        wxBaseEnumProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnSetValue() exit"));
}


void PyBaseEnumProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnValidationFailure() exit (fall-back)"));
        wxBaseEnumProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyBaseEnumProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyIntToValue() exit (fall-back)"));
        return wxBaseEnumProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyBaseEnumProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyStringToValue() exit (fall-back)"));
        return wxBaseEnumProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyBaseEnumProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyValidateValue() exit (fall-back)"));
        return wxBaseEnumProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::PyValidateValue() exit"));
}


void PyBaseEnumProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBaseEnumProperty::RefreshChildren() exit (fall-back)"));
        wxBaseEnumProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBaseEnumProperty::RefreshChildren() exit"));
}


class PyComboBoxEditor : public wxPGComboBoxEditor {
public:
        PyComboBoxEditor();
    virtual ~PyComboBoxEditor();
    void _SetSelf(PyObject *self);
    virtual bool CanContainCustomImage() const;
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz ) const;
    virtual void DeleteItem( wxWindow* ctrl, int index ) const;
    virtual void DrawValue( wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text ) const;
    virtual wxString GetName() const;
    virtual int InsertItem( wxWindow* ctrl, const wxString& label, int index ) const;
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property,
        wxWindow* ctrl, wxEvent& event ) const;
    virtual void OnFocus( wxPGProperty* property, wxWindow* wnd ) const;
    virtual wxPGVariantAndBool PyGetValueFromControl( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const;
    virtual void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const;
    virtual void SetValueToUnspecified( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const;
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyComboBoxEditor_pyClass = NULL;


PyComboBoxEditor::PyComboBoxEditor()
    : wxPGComboBoxEditor()
{
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::PyComboBoxEditor()"));

    Init();
}

PyComboBoxEditor::~PyComboBoxEditor()
{
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::~PyComboBoxEditor()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyComboBoxEditor::_SetSelf(PyObject *self)
{
    if ( !gs_PyComboBoxEditor_pyClass )
    {
        gs_PyComboBoxEditor_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyComboBoxEditor_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyComboBoxEditor::CanContainCustomImage() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::CanContainCustomImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CanContainCustomImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CanContainCustomImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::CanContainCustomImage() exit (fall-back)"));
        return wxPGComboBoxEditor::CanContainCustomImage();
    }
    return _CommonCallback0(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::CanContainCustomImage() exit"));
}


wxPGWindowList PyComboBoxEditor::CreateControls(wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::CreateControls() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CreateControls_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CreateControls_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::CreateControls() exit (fall-back)"));
        return wxPGComboBoxEditor::CreateControls(propgrid, property, pos, sz);
    }
    return _CommonCallback1(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, pos, sz);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::CreateControls() exit"));
}


void PyComboBoxEditor::DeleteItem(wxWindow* ctrl, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::DeleteItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DeleteItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DeleteItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::DeleteItem() exit (fall-back)"));
        wxPGComboBoxEditor::DeleteItem(ctrl, index);
        return;
    }
    _CommonCallback2(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, index);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::DeleteItem() exit"));
}


void PyComboBoxEditor::DrawValue(wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::DrawValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DrawValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DrawValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::DrawValue() exit (fall-back)"));
        wxPGComboBoxEditor::DrawValue(dc, rect, property, text);
        return;
    }
    _CommonCallback3(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, property, text);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::DrawValue() exit"));
}


wxString PyComboBoxEditor::GetName() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::GetName() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetName_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetName_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::GetName() exit (fall-back)"));
        return wxPGComboBoxEditor::GetName();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::GetName() exit"));
}


int PyComboBoxEditor::InsertItem(wxWindow* ctrl, const wxString& label, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::InsertItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_InsertItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_InsertItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::InsertItem() exit (fall-back)"));
        return wxPGComboBoxEditor::InsertItem(ctrl, label, index);
    }
    return _CommonCallback5(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, label, index);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::InsertItem() exit"));
}


bool PyComboBoxEditor::OnEvent(wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::OnEvent() exit (fall-back)"));
        return wxPGComboBoxEditor::OnEvent(propgrid, property, ctrl, event);
    }
    return _CommonCallback6(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, ctrl, event);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::OnEvent() exit"));
}


void PyComboBoxEditor::OnFocus(wxPGProperty* property, wxWindow* wnd) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::OnFocus() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnFocus_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnFocus_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::OnFocus() exit (fall-back)"));
        wxPGComboBoxEditor::OnFocus(property, wnd);
        return;
    }
    _CommonCallback7(blocked, (PyObject*)m_scriptObject, funcobj, property, wnd);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::OnFocus() exit"));
}


wxPGVariantAndBool PyComboBoxEditor::PyGetValueFromControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::PyGetValueFromControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueFromControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueFromControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::PyGetValueFromControl() exit (fall-back)"));
        return wxPGComboBoxEditor::PyGetValueFromControl(property, ctrl);
    }
    return _CommonCallback8(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::PyGetValueFromControl() exit"));
}


void PyComboBoxEditor::SetControlIntValue(wxPGProperty* property, wxWindow* ctrl, int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::SetControlIntValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlIntValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlIntValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::SetControlIntValue() exit (fall-back)"));
        wxPGComboBoxEditor::SetControlIntValue(property, ctrl, value);
        return;
    }
    _CommonCallback9(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, value);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::SetControlIntValue() exit"));
}


void PyComboBoxEditor::SetControlStringValue(wxPGProperty* property, wxWindow* ctrl, const wxString& txt) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::SetControlStringValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlStringValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlStringValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::SetControlStringValue() exit (fall-back)"));
        wxPGComboBoxEditor::SetControlStringValue(property, ctrl, txt);
        return;
    }
    _CommonCallback10(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, txt);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::SetControlStringValue() exit"));
}


void PyComboBoxEditor::SetValueToUnspecified(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::SetValueToUnspecified() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetValueToUnspecified_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetValueToUnspecified_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::SetValueToUnspecified() exit (fall-back)"));
        wxPGComboBoxEditor::SetValueToUnspecified(property, ctrl);
        return;
    }
    _CommonCallback7(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::SetValueToUnspecified() exit"));
}


void PyComboBoxEditor::UpdateControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::UpdateControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_UpdateControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_UpdateControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyComboBoxEditor::UpdateControl() exit (fall-back)"));
        wxPGComboBoxEditor::UpdateControl(property, ctrl);
        return;
    }
    _CommonCallback7(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyComboBoxEditor::UpdateControl() exit"));
}


class PyFileDialogAdapter : public wxPGFileDialogAdapter {
public:
        PyFileDialogAdapter();
    virtual ~PyFileDialogAdapter();
    void _SetSelf(PyObject *self);
    virtual bool DoShowDialog( wxPropertyGrid* propGrid, wxPGProperty* property );
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyFileDialogAdapter_pyClass = NULL;


PyFileDialogAdapter::PyFileDialogAdapter()
    : wxPGFileDialogAdapter()
{
    MySWIGOutputDebugString(wxT("PyFileDialogAdapter::PyFileDialogAdapter()"));
    Init();
}

PyFileDialogAdapter::~PyFileDialogAdapter()
{
    MySWIGOutputDebugString(wxT("PyFileDialogAdapter::~PyFileDialogAdapter()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyFileDialogAdapter::_SetSelf(PyObject *self)
{
    if ( !gs_PyFileDialogAdapter_pyClass )
    {
        gs_PyFileDialogAdapter_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyFileDialogAdapter_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyFileDialogAdapter::DoShowDialog(wxPropertyGrid* propGrid, wxPGProperty* property)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileDialogAdapter::DoShowDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoShowDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoShowDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileDialogAdapter::DoShowDialog() exit (fall-back)"));
        return wxPGFileDialogAdapter::DoShowDialog(propGrid, property);
    }
    return _CommonCallback29(blocked, (PyObject*)m_scriptObject, funcobj, propGrid, property);
    MySWIGOutputDebugString(wxT("PyFileDialogAdapter::DoShowDialog() exit"));
}


class PyStringProperty : public wxStringProperty {
public:
        PyStringProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL,
                      const wxString& value = wxEmptyString );
    virtual ~PyStringProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyStringProperty_pyClass = NULL;


PyStringProperty::PyStringProperty(const wxString& label, const wxString& name, const wxString& value)
    : wxStringProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyStringProperty::PyStringProperty()"));

    Init();
}

PyStringProperty::~PyStringProperty()
{
    MySWIGOutputDebugString(wxT("PyStringProperty::~PyStringProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyStringProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyStringProperty_pyClass )
    {
        gs_PyStringProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyStringProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyStringProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::ChildChanged() exit (fall-back)"));
        wxStringProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyStringProperty::ChildChanged() exit"));
}


wxVariant PyStringProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::DoGetAttribute() exit (fall-back)"));
        return wxStringProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyStringProperty::DoGetAttribute() exit"));
}


wxValidator* PyStringProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::DoGetValidator() exit (fall-back)"));
        return wxStringProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyStringProperty::DoGetValidator() exit"));
}


wxVariant PyStringProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::DoGetValue() exit (fall-back)"));
        return wxStringProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyStringProperty::DoGetValue() exit"));
}


bool PyStringProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::DoSetAttribute() exit (fall-back)"));
        return wxStringProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyStringProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyStringProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::GetCellRenderer() exit (fall-back)"));
        return wxStringProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyStringProperty::GetCellRenderer() exit"));
}


int PyStringProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::GetChoiceInfo() exit (fall-back)"));
        return wxStringProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyStringProperty::GetChoiceInfo() exit"));
}


wxString PyStringProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::GetEditor() exit (fall-back)"));
        return wxStringProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyStringProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyStringProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::GetEditorDialog() exit (fall-back)"));
        return wxStringProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyStringProperty::GetEditorDialog() exit"));
}


wxSize PyStringProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::GetImageSize() exit (fall-back)"));
        return wxStringProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyStringProperty::GetImageSize() exit"));
}


wxString PyStringProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::GetValueAsString() exit (fall-back)"));
        return wxStringProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyStringProperty::GetValueAsString() exit"));
}


void PyStringProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::OnCustomPaint() exit (fall-back)"));
        wxStringProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyStringProperty::OnCustomPaint() exit"));
}


bool PyStringProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::OnEvent() exit (fall-back)"));
        return wxStringProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyStringProperty::OnEvent() exit"));
}


wxSize PyStringProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::OnMeasureImage() exit (fall-back)"));
        return wxStringProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyStringProperty::OnMeasureImage() exit"));
}


void PyStringProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::OnSetValue() exit (fall-back)"));
        wxStringProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyStringProperty::OnSetValue() exit"));
}


void PyStringProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::OnValidationFailure() exit (fall-back)"));
        wxStringProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyStringProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyStringProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::PyIntToValue() exit (fall-back)"));
        return wxStringProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyStringProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyStringProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::PyStringToValue() exit (fall-back)"));
        return wxStringProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyStringProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyStringProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::PyValidateValue() exit (fall-back)"));
        return wxStringProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyStringProperty::PyValidateValue() exit"));
}


void PyStringProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyStringProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyStringProperty::RefreshChildren() exit (fall-back)"));
        wxStringProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyStringProperty::RefreshChildren() exit"));
}


class PyEditEnumProperty : public wxEditEnumProperty {
public:
        PyEditEnumProperty( const wxString& label, const wxString& name, const wxChar** labels,
        const long* values, const wxString& value );
    PyEditEnumProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL,
        const wxArrayString& labels = wxArrayString(), const wxArrayInt& values = wxArrayInt(),
        const wxString& value = wxEmptyString );
    PyEditEnumProperty( const wxString& label, const wxString& name,
        wxPGChoices& choices, const wxString& value = wxEmptyString );
    PyEditEnumProperty( const wxString& label, const wxString& name, const wxChar** labels,
        const long* values, wxPGChoices* choicesCache, const wxString& value );
    virtual ~PyEditEnumProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual const wxString* GetEntry( size_t index, int* pvalue ) const;
    virtual wxSize GetImageSize() const;
    virtual int GetIndexForValue( int value ) const;
    virtual wxString GetValueAsString( int argFlags ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyEditEnumProperty_pyClass = NULL;


PyEditEnumProperty::PyEditEnumProperty(const wxString& label, const wxString& name, const wxChar** labels, const long* values, const wxString& value)
    : wxEditEnumProperty(label, name, labels, values, value)
{
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyEditEnumProperty()"));

    Init();
}

PyEditEnumProperty::PyEditEnumProperty(const wxString& label, const wxString& name, const wxArrayString& labels, const wxArrayInt& values, const wxString& value)
    : wxEditEnumProperty(label, name, labels, values, value)
{
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyEditEnumProperty()"));

    Init();
}

PyEditEnumProperty::PyEditEnumProperty(const wxString& label, const wxString& name, wxPGChoices& choices, const wxString& value)
    : wxEditEnumProperty(label, name, choices, value)
{
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyEditEnumProperty()"));

    Init();
}

PyEditEnumProperty::PyEditEnumProperty(const wxString& label, const wxString& name, const wxChar** labels, const long* values, wxPGChoices* choicesCache, const wxString& value)
    : wxEditEnumProperty(label, name, labels, values, choicesCache, value)
{
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyEditEnumProperty()"));

    Init();
}

PyEditEnumProperty::~PyEditEnumProperty()
{
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::~PyEditEnumProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyEditEnumProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyEditEnumProperty_pyClass )
    {
        gs_PyEditEnumProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyEditEnumProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyEditEnumProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::ChildChanged() exit (fall-back)"));
        wxEditEnumProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::ChildChanged() exit"));
}


wxVariant PyEditEnumProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoGetAttribute() exit (fall-back)"));
        return wxEditEnumProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoGetAttribute() exit"));
}


wxValidator* PyEditEnumProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoGetValidator() exit (fall-back)"));
        return wxEditEnumProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoGetValidator() exit"));
}


wxVariant PyEditEnumProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoGetValue() exit (fall-back)"));
        return wxEditEnumProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoGetValue() exit"));
}


bool PyEditEnumProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoSetAttribute() exit (fall-back)"));
        return wxEditEnumProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyEditEnumProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetCellRenderer() exit (fall-back)"));
        return wxEditEnumProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetCellRenderer() exit"));
}


int PyEditEnumProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetChoiceInfo() exit (fall-back)"));
        return wxEditEnumProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetChoiceInfo() exit"));
}


wxString PyEditEnumProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetEditor() exit (fall-back)"));
        return wxEditEnumProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyEditEnumProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetEditorDialog() exit (fall-back)"));
        return wxEditEnumProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetEditorDialog() exit"));
}


const wxString* PyEditEnumProperty::GetEntry(size_t index, int* pvalue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetEntry() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEntry_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEntry_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetEntry() exit (fall-back)"));
        return wxEditEnumProperty::GetEntry(index, pvalue);
    }
    return _CommonCallback30(blocked, (PyObject*)m_scriptObject, funcobj, index, pvalue);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetEntry() exit"));
}


wxSize PyEditEnumProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetImageSize() exit (fall-back)"));
        return wxEditEnumProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetImageSize() exit"));
}


int PyEditEnumProperty::GetIndexForValue(int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetIndexForValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetIndexForValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetIndexForValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetIndexForValue() exit (fall-back)"));
        return wxEditEnumProperty::GetIndexForValue(value);
    }
    return _CommonCallback31(blocked, (PyObject*)m_scriptObject, funcobj, value);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetIndexForValue() exit"));
}


wxString PyEditEnumProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetValueAsString() exit (fall-back)"));
        return wxEditEnumProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::GetValueAsString() exit"));
}


void PyEditEnumProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnCustomPaint() exit (fall-back)"));
        wxEditEnumProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnCustomPaint() exit"));
}


bool PyEditEnumProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnEvent() exit (fall-back)"));
        return wxEditEnumProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnEvent() exit"));
}


wxSize PyEditEnumProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnMeasureImage() exit (fall-back)"));
        return wxEditEnumProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnMeasureImage() exit"));
}


void PyEditEnumProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnSetValue() exit (fall-back)"));
        wxEditEnumProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnSetValue() exit"));
}


void PyEditEnumProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnValidationFailure() exit (fall-back)"));
        wxEditEnumProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyEditEnumProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyIntToValue() exit (fall-back)"));
        return wxEditEnumProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyEditEnumProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyStringToValue() exit (fall-back)"));
        return wxEditEnumProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyEditEnumProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyValidateValue() exit (fall-back)"));
        return wxEditEnumProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::PyValidateValue() exit"));
}


void PyEditEnumProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditEnumProperty::RefreshChildren() exit (fall-back)"));
        wxEditEnumProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEditEnumProperty::RefreshChildren() exit"));
}


class PyChoiceAndButtonEditor : public wxPGChoiceAndButtonEditor {
public:
        PyChoiceAndButtonEditor();
    virtual ~PyChoiceAndButtonEditor();
    void _SetSelf(PyObject *self);
    virtual bool CanContainCustomImage() const;
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz ) const;
    virtual void DeleteItem( wxWindow* ctrl, int index ) const;
    virtual void DrawValue( wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text ) const;
    virtual wxString GetName() const;
    virtual int InsertItem( wxWindow* ctrl, const wxString& label, int index ) const;
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event ) const;
    virtual void OnFocus( wxPGProperty* property, wxWindow* wnd ) const;
    virtual wxPGVariantAndBool PyGetValueFromControl( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const;
    virtual void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const;
    virtual void SetValueToUnspecified( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const;
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyChoiceAndButtonEditor_pyClass = NULL;


PyChoiceAndButtonEditor::PyChoiceAndButtonEditor()
    : wxPGChoiceAndButtonEditor()
{
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::PyChoiceAndButtonEditor()"));

    Init();
}

PyChoiceAndButtonEditor::~PyChoiceAndButtonEditor()
{
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::~PyChoiceAndButtonEditor()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyChoiceAndButtonEditor::_SetSelf(PyObject *self)
{
    if ( !gs_PyChoiceAndButtonEditor_pyClass )
    {
        gs_PyChoiceAndButtonEditor_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyChoiceAndButtonEditor_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyChoiceAndButtonEditor::CanContainCustomImage() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::CanContainCustomImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CanContainCustomImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CanContainCustomImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::CanContainCustomImage() exit (fall-back)"));
        return wxPGChoiceAndButtonEditor::CanContainCustomImage();
    }
    return _CommonCallback0(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::CanContainCustomImage() exit"));
}


wxPGWindowList PyChoiceAndButtonEditor::CreateControls(wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::CreateControls() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CreateControls_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CreateControls_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::CreateControls() exit (fall-back)"));
        return wxPGChoiceAndButtonEditor::CreateControls(propgrid, property, pos, sz);
    }
    return _CommonCallback1(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, pos, sz);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::CreateControls() exit"));
}


void PyChoiceAndButtonEditor::DeleteItem(wxWindow* ctrl, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::DeleteItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DeleteItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DeleteItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::DeleteItem() exit (fall-back)"));
        wxPGChoiceAndButtonEditor::DeleteItem(ctrl, index);
        return;
    }
    _CommonCallback2(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, index);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::DeleteItem() exit"));
}


void PyChoiceAndButtonEditor::DrawValue(wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::DrawValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DrawValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DrawValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::DrawValue() exit (fall-back)"));
        wxPGChoiceAndButtonEditor::DrawValue(dc, rect, property, text);
        return;
    }
    _CommonCallback3(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, property, text);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::DrawValue() exit"));
}


wxString PyChoiceAndButtonEditor::GetName() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::GetName() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetName_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetName_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::GetName() exit (fall-back)"));
        return wxPGChoiceAndButtonEditor::GetName();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::GetName() exit"));
}


int PyChoiceAndButtonEditor::InsertItem(wxWindow* ctrl, const wxString& label, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::InsertItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_InsertItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_InsertItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::InsertItem() exit (fall-back)"));
        return wxPGChoiceAndButtonEditor::InsertItem(ctrl, label, index);
    }
    return _CommonCallback5(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, label, index);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::InsertItem() exit"));
}


bool PyChoiceAndButtonEditor::OnEvent(wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::OnEvent() exit (fall-back)"));
        return wxPGChoiceAndButtonEditor::OnEvent(propgrid, property, primary, event);
    }
    return _CommonCallback6(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, primary, event);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::OnEvent() exit"));
}


void PyChoiceAndButtonEditor::OnFocus(wxPGProperty* property, wxWindow* wnd) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::OnFocus() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnFocus_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnFocus_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::OnFocus() exit (fall-back)"));
        wxPGChoiceAndButtonEditor::OnFocus(property, wnd);
        return;
    }
    _CommonCallback7(blocked, (PyObject*)m_scriptObject, funcobj, property, wnd);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::OnFocus() exit"));
}


wxPGVariantAndBool PyChoiceAndButtonEditor::PyGetValueFromControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::PyGetValueFromControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueFromControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueFromControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::PyGetValueFromControl() exit (fall-back)"));
        return wxPGChoiceAndButtonEditor::PyGetValueFromControl(property, ctrl);
    }
    return _CommonCallback8(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::PyGetValueFromControl() exit"));
}


void PyChoiceAndButtonEditor::SetControlIntValue(wxPGProperty* property, wxWindow* ctrl, int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::SetControlIntValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlIntValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlIntValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::SetControlIntValue() exit (fall-back)"));
        wxPGChoiceAndButtonEditor::SetControlIntValue(property, ctrl, value);
        return;
    }
    _CommonCallback9(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, value);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::SetControlIntValue() exit"));
}


void PyChoiceAndButtonEditor::SetControlStringValue(wxPGProperty* property, wxWindow* ctrl, const wxString& txt) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::SetControlStringValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlStringValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlStringValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::SetControlStringValue() exit (fall-back)"));
        wxPGChoiceAndButtonEditor::SetControlStringValue(property, ctrl, txt);
        return;
    }
    _CommonCallback10(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, txt);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::SetControlStringValue() exit"));
}


void PyChoiceAndButtonEditor::SetValueToUnspecified(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::SetValueToUnspecified() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetValueToUnspecified_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetValueToUnspecified_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::SetValueToUnspecified() exit (fall-back)"));
        wxPGChoiceAndButtonEditor::SetValueToUnspecified(property, ctrl);
        return;
    }
    _CommonCallback7(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::SetValueToUnspecified() exit"));
}


void PyChoiceAndButtonEditor::UpdateControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::UpdateControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_UpdateControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_UpdateControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::UpdateControl() exit (fall-back)"));
        wxPGChoiceAndButtonEditor::UpdateControl(property, ctrl);
        return;
    }
    _CommonCallback7(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyChoiceAndButtonEditor::UpdateControl() exit"));
}


class PyCustomProperty : public wxCustomProperty {
public:
        PyCustomProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL );
    virtual ~PyCustomProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& a1, int a2, wxVariant& a3 ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyCustomProperty_pyClass = NULL;


PyCustomProperty::PyCustomProperty(const wxString& label, const wxString& name)
    : wxCustomProperty(label, name)
{
    MySWIGOutputDebugString(wxT("PyCustomProperty::PyCustomProperty()"));

    Init();
}

PyCustomProperty::~PyCustomProperty()
{
    MySWIGOutputDebugString(wxT("PyCustomProperty::~PyCustomProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyCustomProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyCustomProperty_pyClass )
    {
        gs_PyCustomProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyCustomProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyCustomProperty::ChildChanged(wxVariant& a1, int a2, wxVariant& a3) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::ChildChanged() exit (fall-back)"));
        wxCustomProperty::ChildChanged(a1, a2, a3);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, a1, a2, a3);
    MySWIGOutputDebugString(wxT("PyCustomProperty::ChildChanged() exit"));
}


wxVariant PyCustomProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::DoGetAttribute() exit (fall-back)"));
        return wxCustomProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyCustomProperty::DoGetAttribute() exit"));
}


wxValidator* PyCustomProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::DoGetValidator() exit (fall-back)"));
        return wxCustomProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCustomProperty::DoGetValidator() exit"));
}


wxVariant PyCustomProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::DoGetValue() exit (fall-back)"));
        return wxCustomProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCustomProperty::DoGetValue() exit"));
}


bool PyCustomProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::DoSetAttribute() exit (fall-back)"));
        return wxCustomProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyCustomProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyCustomProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::GetCellRenderer() exit (fall-back)"));
        return wxCustomProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetCellRenderer() exit"));
}


int PyCustomProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::GetChoiceInfo() exit (fall-back)"));
        return wxCustomProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetChoiceInfo() exit"));
}


wxString PyCustomProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::GetEditor() exit (fall-back)"));
        return wxCustomProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyCustomProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::GetEditorDialog() exit (fall-back)"));
        return wxCustomProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetEditorDialog() exit"));
}


wxSize PyCustomProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::GetImageSize() exit (fall-back)"));
        return wxCustomProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetImageSize() exit"));
}


wxString PyCustomProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::GetValueAsString() exit (fall-back)"));
        return wxCustomProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyCustomProperty::GetValueAsString() exit"));
}


void PyCustomProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::OnCustomPaint() exit (fall-back)"));
        wxCustomProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnCustomPaint() exit"));
}


bool PyCustomProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::OnEvent() exit (fall-back)"));
        return wxCustomProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnEvent() exit"));
}


wxSize PyCustomProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::OnMeasureImage() exit (fall-back)"));
        return wxCustomProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnMeasureImage() exit"));
}


void PyCustomProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::OnSetValue() exit (fall-back)"));
        wxCustomProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnSetValue() exit"));
}


void PyCustomProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::OnValidationFailure() exit (fall-back)"));
        wxCustomProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyCustomProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyCustomProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::PyIntToValue() exit (fall-back)"));
        return wxCustomProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyCustomProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyCustomProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::PyStringToValue() exit (fall-back)"));
        return wxCustomProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyCustomProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyCustomProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::PyValidateValue() exit (fall-back)"));
        return wxCustomProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyCustomProperty::PyValidateValue() exit"));
}


void PyCustomProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCustomProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCustomProperty::RefreshChildren() exit (fall-back)"));
        wxCustomProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCustomProperty::RefreshChildren() exit"));
}


class PyProperty : public wxPGProperty {
public:
        PyProperty();
    PyProperty( const wxString& label, const wxString& name );
    virtual ~PyProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyProperty_pyClass = NULL;


PyProperty::PyProperty()
    : wxPGProperty()
{
    MySWIGOutputDebugString(wxT("PyProperty::PyProperty()"));

    Init();
}

PyProperty::PyProperty(const wxString& label, const wxString& name)
    : wxPGProperty(label, name)
{
    MySWIGOutputDebugString(wxT("PyProperty::PyProperty()"));

    Init();
}

PyProperty::~PyProperty()
{
    MySWIGOutputDebugString(wxT("PyProperty::~PyProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyProperty_pyClass )
    {
        gs_PyProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::ChildChanged() exit (fall-back)"));
        wxPGProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyProperty::ChildChanged() exit"));
}


wxVariant PyProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::DoGetAttribute() exit (fall-back)"));
        return wxPGProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyProperty::DoGetAttribute() exit"));
}


wxValidator* PyProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::DoGetValidator() exit (fall-back)"));
        return wxPGProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyProperty::DoGetValidator() exit"));
}


wxVariant PyProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::DoGetValue() exit (fall-back)"));
        return wxPGProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyProperty::DoGetValue() exit"));
}


bool PyProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::DoSetAttribute() exit (fall-back)"));
        return wxPGProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::GetCellRenderer() exit (fall-back)"));
        return wxPGProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyProperty::GetCellRenderer() exit"));
}


int PyProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::GetChoiceInfo() exit (fall-back)"));
        return wxPGProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyProperty::GetChoiceInfo() exit"));
}


wxString PyProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::GetEditor() exit (fall-back)"));
        return wxPGProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::GetEditorDialog() exit (fall-back)"));
        return wxPGProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyProperty::GetEditorDialog() exit"));
}


wxSize PyProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::GetImageSize() exit (fall-back)"));
        return wxPGProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyProperty::GetImageSize() exit"));
}


wxString PyProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::GetValueAsString() exit (fall-back)"));
        return wxPGProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyProperty::GetValueAsString() exit"));
}


void PyProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::OnCustomPaint() exit (fall-back)"));
        wxPGProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyProperty::OnCustomPaint() exit"));
}


bool PyProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::OnEvent() exit (fall-back)"));
        return wxPGProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyProperty::OnEvent() exit"));
}


wxSize PyProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::OnMeasureImage() exit (fall-back)"));
        return wxPGProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyProperty::OnMeasureImage() exit"));
}


void PyProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::OnSetValue() exit (fall-back)"));
        wxPGProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyProperty::OnSetValue() exit"));
}


void PyProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::OnValidationFailure() exit (fall-back)"));
        wxPGProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::PyIntToValue() exit (fall-back)"));
        return wxPGProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::PyStringToValue() exit (fall-back)"));
        return wxPGProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::PyValidateValue() exit (fall-back)"));
        return wxPGProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyProperty::PyValidateValue() exit"));
}


void PyProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyProperty::RefreshChildren() exit (fall-back)"));
        wxPGProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyProperty::RefreshChildren() exit"));
}


class PyTextCtrlEditor : public wxPGTextCtrlEditor {
public:
        PyTextCtrlEditor();
    virtual ~PyTextCtrlEditor();
    void _SetSelf(PyObject *self);
    virtual bool CanContainCustomImage() const;
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz ) const;
    virtual void DeleteItem( wxWindow* ctrl, int index ) const;
    virtual void DrawValue( wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text ) const;
    virtual wxString GetName() const;
    virtual int InsertItem( wxWindow* ctrl, const wxString& label, int index ) const;
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event ) const;
    virtual void OnFocus( wxPGProperty* property, wxWindow* wnd ) const;
    virtual wxPGVariantAndBool PyGetValueFromControl( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const;
    virtual void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const;
    virtual void SetValueToUnspecified( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const;
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyTextCtrlEditor_pyClass = NULL;


PyTextCtrlEditor::PyTextCtrlEditor()
    : wxPGTextCtrlEditor()
{
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::PyTextCtrlEditor()"));

    Init();
}

PyTextCtrlEditor::~PyTextCtrlEditor()
{
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::~PyTextCtrlEditor()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyTextCtrlEditor::_SetSelf(PyObject *self)
{
    if ( !gs_PyTextCtrlEditor_pyClass )
    {
        gs_PyTextCtrlEditor_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyTextCtrlEditor_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyTextCtrlEditor::CanContainCustomImage() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::CanContainCustomImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CanContainCustomImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CanContainCustomImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::CanContainCustomImage() exit (fall-back)"));
        return wxPGTextCtrlEditor::CanContainCustomImage();
    }
    return _CommonCallback0(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::CanContainCustomImage() exit"));
}


wxPGWindowList PyTextCtrlEditor::CreateControls(wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::CreateControls() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CreateControls_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CreateControls_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::CreateControls() exit (fall-back)"));
        return wxPGTextCtrlEditor::CreateControls(propgrid, property, pos, sz);
    }
    return _CommonCallback34(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, pos, sz);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::CreateControls() exit"));
}


void PyTextCtrlEditor::DeleteItem(wxWindow* ctrl, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::DeleteItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DeleteItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DeleteItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::DeleteItem() exit (fall-back)"));
        wxPGTextCtrlEditor::DeleteItem(ctrl, index);
        return;
    }
    _CommonCallback2(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, index);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::DeleteItem() exit"));
}


void PyTextCtrlEditor::DrawValue(wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::DrawValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DrawValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DrawValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::DrawValue() exit (fall-back)"));
        wxPGTextCtrlEditor::DrawValue(dc, rect, property, text);
        return;
    }
    _CommonCallback35(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, property, text);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::DrawValue() exit"));
}


wxString PyTextCtrlEditor::GetName() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::GetName() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetName_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetName_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::GetName() exit (fall-back)"));
        return wxPGTextCtrlEditor::GetName();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::GetName() exit"));
}


int PyTextCtrlEditor::InsertItem(wxWindow* ctrl, const wxString& label, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::InsertItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_InsertItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_InsertItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::InsertItem() exit (fall-back)"));
        return wxPGTextCtrlEditor::InsertItem(ctrl, label, index);
    }
    return _CommonCallback5(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, label, index);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::InsertItem() exit"));
}


bool PyTextCtrlEditor::OnEvent(wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::OnEvent() exit (fall-back)"));
        return wxPGTextCtrlEditor::OnEvent(propgrid, property, primary, event);
    }
    return _CommonCallback36(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, primary, event);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::OnEvent() exit"));
}


void PyTextCtrlEditor::OnFocus(wxPGProperty* property, wxWindow* wnd) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::OnFocus() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnFocus_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnFocus_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::OnFocus() exit (fall-back)"));
        wxPGTextCtrlEditor::OnFocus(property, wnd);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, wnd);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::OnFocus() exit"));
}


wxPGVariantAndBool PyTextCtrlEditor::PyGetValueFromControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::PyGetValueFromControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueFromControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueFromControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::PyGetValueFromControl() exit (fall-back)"));
        return wxPGTextCtrlEditor::PyGetValueFromControl(property, ctrl);
    }
    return _CommonCallback38(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::PyGetValueFromControl() exit"));
}


void PyTextCtrlEditor::SetControlIntValue(wxPGProperty* property, wxWindow* ctrl, int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::SetControlIntValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlIntValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlIntValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::SetControlIntValue() exit (fall-back)"));
        wxPGTextCtrlEditor::SetControlIntValue(property, ctrl, value);
        return;
    }
    _CommonCallback39(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, value);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::SetControlIntValue() exit"));
}


void PyTextCtrlEditor::SetControlStringValue(wxPGProperty* property, wxWindow* ctrl, const wxString& txt) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::SetControlStringValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlStringValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlStringValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::SetControlStringValue() exit (fall-back)"));
        wxPGTextCtrlEditor::SetControlStringValue(property, ctrl, txt);
        return;
    }
    _CommonCallback40(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, txt);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::SetControlStringValue() exit"));
}


void PyTextCtrlEditor::SetValueToUnspecified(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::SetValueToUnspecified() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetValueToUnspecified_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetValueToUnspecified_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::SetValueToUnspecified() exit (fall-back)"));
        wxPGTextCtrlEditor::SetValueToUnspecified(property, ctrl);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::SetValueToUnspecified() exit"));
}


void PyTextCtrlEditor::UpdateControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::UpdateControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_UpdateControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_UpdateControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlEditor::UpdateControl() exit (fall-back)"));
        wxPGTextCtrlEditor::UpdateControl(property, ctrl);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyTextCtrlEditor::UpdateControl() exit"));
}


class PyDateProperty : public wxDateProperty {
public:
        PyDateProperty( const wxString& label = wxString_wxPG_LABEL,
                    const wxString& name = wxString_wxPG_LABEL,
                    const wxDateTime& value = wxDateTime() );
    virtual ~PyDateProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int flags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyDateProperty_pyClass = NULL;


PyDateProperty::PyDateProperty(const wxString& label, const wxString& name, const wxDateTime& value)
    : wxDateProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyDateProperty::PyDateProperty()"));

    Init();
}

PyDateProperty::~PyDateProperty()
{
    MySWIGOutputDebugString(wxT("PyDateProperty::~PyDateProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyDateProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyDateProperty_pyClass )
    {
        gs_PyDateProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyDateProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyDateProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::ChildChanged() exit (fall-back)"));
        wxDateProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyDateProperty::ChildChanged() exit"));
}


wxVariant PyDateProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::DoGetAttribute() exit (fall-back)"));
        return wxDateProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyDateProperty::DoGetAttribute() exit"));
}


wxValidator* PyDateProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::DoGetValidator() exit (fall-back)"));
        return wxDateProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDateProperty::DoGetValidator() exit"));
}


wxVariant PyDateProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::DoGetValue() exit (fall-back)"));
        return wxDateProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDateProperty::DoGetValue() exit"));
}


bool PyDateProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::DoSetAttribute() exit (fall-back)"));
        return wxDateProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyDateProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyDateProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::GetCellRenderer() exit (fall-back)"));
        return wxDateProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyDateProperty::GetCellRenderer() exit"));
}


int PyDateProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::GetChoiceInfo() exit (fall-back)"));
        return wxDateProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyDateProperty::GetChoiceInfo() exit"));
}


wxString PyDateProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::GetEditor() exit (fall-back)"));
        return wxDateProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDateProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyDateProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::GetEditorDialog() exit (fall-back)"));
        return wxDateProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDateProperty::GetEditorDialog() exit"));
}


wxSize PyDateProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::GetImageSize() exit (fall-back)"));
        return wxDateProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDateProperty::GetImageSize() exit"));
}


wxString PyDateProperty::GetValueAsString(int flags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::GetValueAsString() exit (fall-back)"));
        return wxDateProperty::GetValueAsString(flags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, flags);
    MySWIGOutputDebugString(wxT("PyDateProperty::GetValueAsString() exit"));
}


void PyDateProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::OnCustomPaint() exit (fall-back)"));
        wxDateProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyDateProperty::OnCustomPaint() exit"));
}


bool PyDateProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::OnEvent() exit (fall-back)"));
        return wxDateProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyDateProperty::OnEvent() exit"));
}


wxSize PyDateProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::OnMeasureImage() exit (fall-back)"));
        return wxDateProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyDateProperty::OnMeasureImage() exit"));
}


void PyDateProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::OnSetValue() exit (fall-back)"));
        wxDateProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDateProperty::OnSetValue() exit"));
}


void PyDateProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::OnValidationFailure() exit (fall-back)"));
        wxDateProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyDateProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyDateProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::PyIntToValue() exit (fall-back)"));
        return wxDateProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyDateProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyDateProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::PyStringToValue() exit (fall-back)"));
        return wxDateProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyDateProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyDateProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::PyValidateValue() exit (fall-back)"));
        return wxDateProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyDateProperty::PyValidateValue() exit"));
}


void PyDateProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDateProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDateProperty::RefreshChildren() exit (fall-back)"));
        wxDateProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDateProperty::RefreshChildren() exit"));
}


class PySystemColourProperty : public wxSystemColourProperty {
public:
        PySystemColourProperty( const wxString& label = wxString_wxPG_LABEL,
                            const wxString& name = wxString_wxPG_LABEL,
                            const wxColourPropertyValue& value = wxColourPropertyValue() );
    PySystemColourProperty( const wxString& label, const wxString& name,
        const wxChar** labels, const long* values, wxPGChoices* choicesCache,
        const wxColourPropertyValue& value );
    PySystemColourProperty( const wxString& label, const wxString& name,
        const wxChar** labels, const long* values, wxPGChoices* choicesCache,
        const wxColour& value );
    virtual ~PySystemColourProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxString ColourToString( const wxColour& col, int index ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxVariant DoTranslateVal( wxColourPropertyValue& v ) const;
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxColour GetColour( int index ) const;
    virtual int GetCustomColourIndex() const;
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual const wxString* GetEntry( size_t index, int* pvalue ) const;
    virtual wxSize GetImageSize() const;
    virtual int GetIndexForValue( int value ) const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PySystemColourProperty_pyClass = NULL;


PySystemColourProperty::PySystemColourProperty(const wxString& label, const wxString& name, const wxColourPropertyValue& value)
    : wxSystemColourProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PySystemColourProperty::PySystemColourProperty()"));

    Init();
}

PySystemColourProperty::PySystemColourProperty(const wxString& label, const wxString& name, const wxChar** labels, const long* values, wxPGChoices* choicesCache, const wxColourPropertyValue& value)
    : wxSystemColourProperty(label, name, labels, values, choicesCache, value)
{
    MySWIGOutputDebugString(wxT("PySystemColourProperty::PySystemColourProperty()"));

    Init();
}

PySystemColourProperty::PySystemColourProperty(const wxString& label, const wxString& name, const wxChar** labels, const long* values, wxPGChoices* choicesCache, const wxColour& value)
    : wxSystemColourProperty(label, name, labels, values, choicesCache, value)
{
    MySWIGOutputDebugString(wxT("PySystemColourProperty::PySystemColourProperty()"));

    Init();
}

PySystemColourProperty::~PySystemColourProperty()
{
    MySWIGOutputDebugString(wxT("PySystemColourProperty::~PySystemColourProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PySystemColourProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PySystemColourProperty_pyClass )
    {
        gs_PySystemColourProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PySystemColourProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PySystemColourProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::ChildChanged() exit (fall-back)"));
        wxSystemColourProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::ChildChanged() exit"));
}


wxString PySystemColourProperty::ColourToString(const wxColour& col, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::ColourToString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ColourToString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ColourToString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::ColourToString() exit (fall-back)"));
        return wxSystemColourProperty::ColourToString(col, index);
    }
    PyObject* res;
    PyObject* py_col;
    py_col = SWIG_NewPointerObj((void*)&col, SWIGTYPE_p_wxColour, 0);
    PyObject* py_index;
    py_index = PyInt_FromLong((long)index);

    res = PyObject_CallFunctionObjArgs(funcobj, ((PyObject*)m_scriptObject), py_col, py_index, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_index);
    Py_DECREF(py_col);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxString retval;
    wxString* sptr = wxString_in_helper(res);
        if (sptr == NULL) SWIG_fail;
        retval = *sptr;
        delete sptr;
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxEmptyString;
    MySWIGOutputDebugString(wxT("PySystemColourProperty::ColourToString() exit"));
}


wxVariant PySystemColourProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::DoGetAttribute() exit (fall-back)"));
        return wxSystemColourProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoGetAttribute() exit"));
}


wxValidator* PySystemColourProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::DoGetValidator() exit (fall-back)"));
        return wxSystemColourProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoGetValidator() exit"));
}


wxVariant PySystemColourProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::DoGetValue() exit (fall-back)"));
        return wxSystemColourProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoGetValue() exit"));
}


bool PySystemColourProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::DoSetAttribute() exit (fall-back)"));
        return wxSystemColourProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoSetAttribute() exit"));
}


wxVariant PySystemColourProperty::DoTranslateVal(wxColourPropertyValue& v) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoTranslateVal() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoTranslateVal_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoTranslateVal_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::DoTranslateVal() exit (fall-back)"));
        return wxSystemColourProperty::DoTranslateVal(v);
    }
    PyObject* res;
    PyObject* py_v;
    py_v = SWIG_NewPointerObj((void*)&v, SWIGTYPE_p_wxColourPropertyValue, 0);
    res = PyObject_CallFunctionObjArgs(funcobj, ((PyObject*)m_scriptObject), py_v, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_v);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxVariant retval;
    if ( !PyObject_to_wxVariant(res, &retval) ) {
            PyErr_SetString(PyExc_TypeError, "this Python type cannot be converted to wxVariant");
            SWIG_fail;
        }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxVariant();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::DoTranslateVal() exit"));
}


wxPGCellRenderer* PySystemColourProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetCellRenderer() exit (fall-back)"));
        return wxSystemColourProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetCellRenderer() exit"));
}


int PySystemColourProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetChoiceInfo() exit (fall-back)"));
        return wxSystemColourProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetChoiceInfo() exit"));
}


wxColour PySystemColourProperty::GetColour(int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetColour() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetColour_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetColour_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetColour() exit (fall-back)"));
        return wxSystemColourProperty::GetColour(index);
    }
    PyObject* res;
    PyObject* py_index;
    py_index = PyInt_FromLong((long)index);

    res = PyObject_CallFunctionObjArgs(funcobj, ((PyObject*)m_scriptObject), py_index, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_index);
    if (PyErr_Occurred()) SWIG_fail;
    {
    wxColour retval;
    wxColour temp;    wxColour* _tptr_0 = &retval;
        if ( ! wxColour_helper(res, &_tptr_0)) SWIG_fail;
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return wxColour();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetColour() exit"));
}


int PySystemColourProperty::GetCustomColourIndex() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetCustomColourIndex() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCustomColourIndex_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCustomColourIndex_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetCustomColourIndex() exit (fall-back)"));
        return wxSystemColourProperty::GetCustomColourIndex();
    }
    PyObject* res;
    res = PyObject_CallFunctionObjArgs(funcobj, ((PyObject*)m_scriptObject), NULL);
    Py_DECREF(funcobj);
    if (PyErr_Occurred()) SWIG_fail;
    {
    int retval;
    retval = (int)PyInt_AS_LONG(res);
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return 0;
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetCustomColourIndex() exit"));
}


wxString PySystemColourProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetEditor() exit (fall-back)"));
        return wxSystemColourProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PySystemColourProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetEditorDialog() exit (fall-back)"));
        return wxSystemColourProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetEditorDialog() exit"));
}


const wxString* PySystemColourProperty::GetEntry(size_t index, int* pvalue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetEntry() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEntry_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEntry_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetEntry() exit (fall-back)"));
        return wxSystemColourProperty::GetEntry(index, pvalue);
    }
    return _CommonCallback30(blocked, (PyObject*)m_scriptObject, funcobj, index, pvalue);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetEntry() exit"));
}


wxSize PySystemColourProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetImageSize() exit (fall-back)"));
        return wxSystemColourProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetImageSize() exit"));
}


int PySystemColourProperty::GetIndexForValue(int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetIndexForValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetIndexForValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetIndexForValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetIndexForValue() exit (fall-back)"));
        return wxSystemColourProperty::GetIndexForValue(value);
    }
    return _CommonCallback31(blocked, (PyObject*)m_scriptObject, funcobj, value);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetIndexForValue() exit"));
}


wxString PySystemColourProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::GetValueAsString() exit (fall-back)"));
        return wxSystemColourProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::GetValueAsString() exit"));
}


void PySystemColourProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::OnCustomPaint() exit (fall-back)"));
        wxSystemColourProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnCustomPaint() exit"));
}


bool PySystemColourProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::OnEvent() exit (fall-back)"));
        return wxSystemColourProperty::OnEvent(propgrid, primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, primary, event);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnEvent() exit"));
}


wxSize PySystemColourProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::OnMeasureImage() exit (fall-back)"));
        return wxSystemColourProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnMeasureImage() exit"));
}


void PySystemColourProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::OnSetValue() exit (fall-back)"));
        wxSystemColourProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnSetValue() exit"));
}


void PySystemColourProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::OnValidationFailure() exit (fall-back)"));
        wxSystemColourProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PySystemColourProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::PyIntToValue() exit (fall-back)"));
        return wxSystemColourProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PySystemColourProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::PyStringToValue() exit (fall-back)"));
        return wxSystemColourProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PySystemColourProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::PyValidateValue() exit (fall-back)"));
        return wxSystemColourProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::PyValidateValue() exit"));
}


void PySystemColourProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PySystemColourProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PySystemColourProperty::RefreshChildren() exit (fall-back)"));
        wxSystemColourProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PySystemColourProperty::RefreshChildren() exit"));
}


class PyFlagsProperty : public wxFlagsProperty {
public:
        PyFlagsProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL,
        const wxArrayString& labels = wxArrayString(),
        const wxArrayInt& values = wxArrayInt(), int value = 0 );
    virtual ~PyFlagsProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyFlagsProperty_pyClass = NULL;


PyFlagsProperty::PyFlagsProperty(const wxString& label, const wxString& name, const wxArrayString& labels, const wxArrayInt& values, int value)
    : wxFlagsProperty(label, name, labels, values, value)
{
    MySWIGOutputDebugString(wxT("PyFlagsProperty::PyFlagsProperty()"));

    Init();
}

PyFlagsProperty::~PyFlagsProperty()
{
    MySWIGOutputDebugString(wxT("PyFlagsProperty::~PyFlagsProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyFlagsProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyFlagsProperty_pyClass )
    {
        gs_PyFlagsProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyFlagsProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyFlagsProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::ChildChanged() exit (fall-back)"));
        wxFlagsProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::ChildChanged() exit"));
}


wxVariant PyFlagsProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::DoGetAttribute() exit (fall-back)"));
        return wxFlagsProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::DoGetAttribute() exit"));
}


wxValidator* PyFlagsProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::DoGetValidator() exit (fall-back)"));
        return wxFlagsProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::DoGetValidator() exit"));
}


wxVariant PyFlagsProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::DoGetValue() exit (fall-back)"));
        return wxFlagsProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::DoGetValue() exit"));
}


bool PyFlagsProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::DoSetAttribute() exit (fall-back)"));
        return wxFlagsProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyFlagsProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::GetCellRenderer() exit (fall-back)"));
        return wxFlagsProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetCellRenderer() exit"));
}


int PyFlagsProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::GetChoiceInfo() exit (fall-back)"));
        return wxFlagsProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetChoiceInfo() exit"));
}


wxString PyFlagsProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::GetEditor() exit (fall-back)"));
        return wxFlagsProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyFlagsProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::GetEditorDialog() exit (fall-back)"));
        return wxFlagsProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetEditorDialog() exit"));
}


wxSize PyFlagsProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::GetImageSize() exit (fall-back)"));
        return wxFlagsProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetImageSize() exit"));
}


wxString PyFlagsProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::GetValueAsString() exit (fall-back)"));
        return wxFlagsProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::GetValueAsString() exit"));
}


void PyFlagsProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::OnCustomPaint() exit (fall-back)"));
        wxFlagsProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnCustomPaint() exit"));
}


bool PyFlagsProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::OnEvent() exit (fall-back)"));
        return wxFlagsProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnEvent() exit"));
}


wxSize PyFlagsProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::OnMeasureImage() exit (fall-back)"));
        return wxFlagsProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnMeasureImage() exit"));
}


void PyFlagsProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::OnSetValue() exit (fall-back)"));
        wxFlagsProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnSetValue() exit"));
}


void PyFlagsProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::OnValidationFailure() exit (fall-back)"));
        wxFlagsProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyFlagsProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::PyIntToValue() exit (fall-back)"));
        return wxFlagsProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyFlagsProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::PyStringToValue() exit (fall-back)"));
        return wxFlagsProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyFlagsProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::PyValidateValue() exit (fall-back)"));
        return wxFlagsProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::PyValidateValue() exit"));
}


void PyFlagsProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFlagsProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFlagsProperty::RefreshChildren() exit (fall-back)"));
        wxFlagsProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFlagsProperty::RefreshChildren() exit"));
}


class PyTextCtrlAndButtonEditor : public wxPGTextCtrlAndButtonEditor {
public:
        PyTextCtrlAndButtonEditor();
    virtual ~PyTextCtrlAndButtonEditor();
    void _SetSelf(PyObject *self);
    virtual bool CanContainCustomImage() const;
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz ) const;
    virtual void DeleteItem( wxWindow* ctrl, int index ) const;
    virtual void DrawValue( wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text ) const;
    virtual wxString GetName() const;
    virtual int InsertItem( wxWindow* ctrl, const wxString& label, int index ) const;
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event ) const;
    virtual void OnFocus( wxPGProperty* property, wxWindow* wnd ) const;
    virtual wxPGVariantAndBool PyGetValueFromControl( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const;
    virtual void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const;
    virtual void SetValueToUnspecified( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const;
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyTextCtrlAndButtonEditor_pyClass = NULL;


PyTextCtrlAndButtonEditor::PyTextCtrlAndButtonEditor()
    : wxPGTextCtrlAndButtonEditor()
{
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::PyTextCtrlAndButtonEditor()"));

    Init();
}

PyTextCtrlAndButtonEditor::~PyTextCtrlAndButtonEditor()
{
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::~PyTextCtrlAndButtonEditor()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyTextCtrlAndButtonEditor::_SetSelf(PyObject *self)
{
    if ( !gs_PyTextCtrlAndButtonEditor_pyClass )
    {
        gs_PyTextCtrlAndButtonEditor_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyTextCtrlAndButtonEditor_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyTextCtrlAndButtonEditor::CanContainCustomImage() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::CanContainCustomImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CanContainCustomImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CanContainCustomImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::CanContainCustomImage() exit (fall-back)"));
        return wxPGTextCtrlAndButtonEditor::CanContainCustomImage();
    }
    return _CommonCallback0(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::CanContainCustomImage() exit"));
}


wxPGWindowList PyTextCtrlAndButtonEditor::CreateControls(wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::CreateControls() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CreateControls_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CreateControls_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::CreateControls() exit (fall-back)"));
        return wxPGTextCtrlAndButtonEditor::CreateControls(propgrid, property, pos, sz);
    }
    return _CommonCallback34(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, pos, sz);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::CreateControls() exit"));
}


void PyTextCtrlAndButtonEditor::DeleteItem(wxWindow* ctrl, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::DeleteItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DeleteItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DeleteItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::DeleteItem() exit (fall-back)"));
        wxPGTextCtrlAndButtonEditor::DeleteItem(ctrl, index);
        return;
    }
    _CommonCallback2(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, index);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::DeleteItem() exit"));
}


void PyTextCtrlAndButtonEditor::DrawValue(wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::DrawValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DrawValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DrawValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::DrawValue() exit (fall-back)"));
        wxPGTextCtrlAndButtonEditor::DrawValue(dc, rect, property, text);
        return;
    }
    _CommonCallback35(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, property, text);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::DrawValue() exit"));
}


wxString PyTextCtrlAndButtonEditor::GetName() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::GetName() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetName_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetName_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::GetName() exit (fall-back)"));
        return wxPGTextCtrlAndButtonEditor::GetName();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::GetName() exit"));
}


int PyTextCtrlAndButtonEditor::InsertItem(wxWindow* ctrl, const wxString& label, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::InsertItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_InsertItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_InsertItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::InsertItem() exit (fall-back)"));
        return wxPGTextCtrlAndButtonEditor::InsertItem(ctrl, label, index);
    }
    return _CommonCallback5(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, label, index);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::InsertItem() exit"));
}


bool PyTextCtrlAndButtonEditor::OnEvent(wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::OnEvent() exit (fall-back)"));
        return wxPGTextCtrlAndButtonEditor::OnEvent(propgrid, property, primary, event);
    }
    return _CommonCallback36(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, primary, event);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::OnEvent() exit"));
}


void PyTextCtrlAndButtonEditor::OnFocus(wxPGProperty* property, wxWindow* wnd) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::OnFocus() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnFocus_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnFocus_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::OnFocus() exit (fall-back)"));
        wxPGTextCtrlAndButtonEditor::OnFocus(property, wnd);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, wnd);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::OnFocus() exit"));
}


wxPGVariantAndBool PyTextCtrlAndButtonEditor::PyGetValueFromControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::PyGetValueFromControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueFromControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueFromControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::PyGetValueFromControl() exit (fall-back)"));
        return wxPGTextCtrlAndButtonEditor::PyGetValueFromControl(property, ctrl);
    }
    return _CommonCallback38(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::PyGetValueFromControl() exit"));
}


void PyTextCtrlAndButtonEditor::SetControlIntValue(wxPGProperty* property, wxWindow* ctrl, int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::SetControlIntValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlIntValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlIntValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::SetControlIntValue() exit (fall-back)"));
        wxPGTextCtrlAndButtonEditor::SetControlIntValue(property, ctrl, value);
        return;
    }
    _CommonCallback39(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, value);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::SetControlIntValue() exit"));
}


void PyTextCtrlAndButtonEditor::SetControlStringValue(wxPGProperty* property, wxWindow* ctrl, const wxString& txt) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::SetControlStringValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlStringValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlStringValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::SetControlStringValue() exit (fall-back)"));
        wxPGTextCtrlAndButtonEditor::SetControlStringValue(property, ctrl, txt);
        return;
    }
    _CommonCallback40(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, txt);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::SetControlStringValue() exit"));
}


void PyTextCtrlAndButtonEditor::SetValueToUnspecified(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::SetValueToUnspecified() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetValueToUnspecified_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetValueToUnspecified_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::SetValueToUnspecified() exit (fall-back)"));
        wxPGTextCtrlAndButtonEditor::SetValueToUnspecified(property, ctrl);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::SetValueToUnspecified() exit"));
}


void PyTextCtrlAndButtonEditor::UpdateControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::UpdateControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_UpdateControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_UpdateControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::UpdateControl() exit (fall-back)"));
        wxPGTextCtrlAndButtonEditor::UpdateControl(property, ctrl);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyTextCtrlAndButtonEditor::UpdateControl() exit"));
}


class PyFontProperty : public wxFontProperty {
public:
        PyFontProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL, const wxFont& value = wxFont() );
    virtual ~PyFontProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyFontProperty_pyClass = NULL;


PyFontProperty::PyFontProperty(const wxString& label, const wxString& name, const wxFont& value)
    : wxFontProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyFontProperty::PyFontProperty()"));

    Init();
}

PyFontProperty::~PyFontProperty()
{
    MySWIGOutputDebugString(wxT("PyFontProperty::~PyFontProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyFontProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyFontProperty_pyClass )
    {
        gs_PyFontProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyFontProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyFontProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::ChildChanged() exit (fall-back)"));
        wxFontProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyFontProperty::ChildChanged() exit"));
}


wxVariant PyFontProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::DoGetAttribute() exit (fall-back)"));
        return wxFontProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyFontProperty::DoGetAttribute() exit"));
}


wxValidator* PyFontProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::DoGetValidator() exit (fall-back)"));
        return wxFontProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFontProperty::DoGetValidator() exit"));
}


wxVariant PyFontProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::DoGetValue() exit (fall-back)"));
        return wxFontProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFontProperty::DoGetValue() exit"));
}


bool PyFontProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::DoSetAttribute() exit (fall-back)"));
        return wxFontProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyFontProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyFontProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::GetCellRenderer() exit (fall-back)"));
        return wxFontProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyFontProperty::GetCellRenderer() exit"));
}


int PyFontProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::GetChoiceInfo() exit (fall-back)"));
        return wxFontProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyFontProperty::GetChoiceInfo() exit"));
}


wxString PyFontProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::GetEditor() exit (fall-back)"));
        return wxFontProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFontProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyFontProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::GetEditorDialog() exit (fall-back)"));
        return wxFontProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFontProperty::GetEditorDialog() exit"));
}


wxSize PyFontProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::GetImageSize() exit (fall-back)"));
        return wxFontProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFontProperty::GetImageSize() exit"));
}


wxString PyFontProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::GetValueAsString() exit (fall-back)"));
        return wxFontProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyFontProperty::GetValueAsString() exit"));
}


void PyFontProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::OnCustomPaint() exit (fall-back)"));
        wxFontProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyFontProperty::OnCustomPaint() exit"));
}


bool PyFontProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::OnEvent() exit (fall-back)"));
        return wxFontProperty::OnEvent(propgrid, primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, primary, event);
    MySWIGOutputDebugString(wxT("PyFontProperty::OnEvent() exit"));
}


wxSize PyFontProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::OnMeasureImage() exit (fall-back)"));
        return wxFontProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyFontProperty::OnMeasureImage() exit"));
}


void PyFontProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::OnSetValue() exit (fall-back)"));
        wxFontProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFontProperty::OnSetValue() exit"));
}


void PyFontProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::OnValidationFailure() exit (fall-back)"));
        wxFontProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyFontProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyFontProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::PyIntToValue() exit (fall-back)"));
        return wxFontProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyFontProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyFontProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::PyStringToValue() exit (fall-back)"));
        return wxFontProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyFontProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyFontProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::PyValidateValue() exit (fall-back)"));
        return wxFontProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyFontProperty::PyValidateValue() exit"));
}


void PyFontProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFontProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFontProperty::RefreshChildren() exit (fall-back)"));
        wxFontProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFontProperty::RefreshChildren() exit"));
}


class PyDirProperty : public wxDirProperty {
public:
        PyDirProperty( const wxString& name = wxString_wxPG_LABEL, const wxString& label = wxString_wxPG_LABEL,
                   const wxString& value = wxEmptyString );
    virtual ~PyDirProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator() const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual bool OnButtonClick ( wxPropertyGrid* propGrid, wxString& value );
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyDirProperty_pyClass = NULL;


PyDirProperty::PyDirProperty(const wxString& name, const wxString& label, const wxString& value)
    : wxDirProperty(name, label, value)
{
    MySWIGOutputDebugString(wxT("PyDirProperty::PyDirProperty()"));

    Init();
}

PyDirProperty::~PyDirProperty()
{
    MySWIGOutputDebugString(wxT("PyDirProperty::~PyDirProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyDirProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyDirProperty_pyClass )
    {
        gs_PyDirProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyDirProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyDirProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::ChildChanged() exit (fall-back)"));
        wxDirProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyDirProperty::ChildChanged() exit"));
}


wxVariant PyDirProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::DoGetAttribute() exit (fall-back)"));
        return wxDirProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyDirProperty::DoGetAttribute() exit"));
}


wxValidator* PyDirProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::DoGetValidator() exit (fall-back)"));
        return wxDirProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDirProperty::DoGetValidator() exit"));
}


wxVariant PyDirProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::DoGetValue() exit (fall-back)"));
        return wxDirProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDirProperty::DoGetValue() exit"));
}


bool PyDirProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::DoSetAttribute() exit (fall-back)"));
        return wxDirProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyDirProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyDirProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::GetCellRenderer() exit (fall-back)"));
        return wxDirProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyDirProperty::GetCellRenderer() exit"));
}


int PyDirProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::GetChoiceInfo() exit (fall-back)"));
        return wxDirProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyDirProperty::GetChoiceInfo() exit"));
}


wxString PyDirProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::GetEditor() exit (fall-back)"));
        return wxDirProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDirProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyDirProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::GetEditorDialog() exit (fall-back)"));
        return wxDirProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDirProperty::GetEditorDialog() exit"));
}


wxSize PyDirProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::GetImageSize() exit (fall-back)"));
        return wxDirProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDirProperty::GetImageSize() exit"));
}


wxString PyDirProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::GetValueAsString() exit (fall-back)"));
        return wxDirProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyDirProperty::GetValueAsString() exit"));
}


bool PyDirProperty::OnButtonClick(wxPropertyGrid* propGrid, wxString& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::OnButtonClick() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnButtonClick_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnButtonClick_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::OnButtonClick() exit (fall-back)"));
        return wxDirProperty::OnButtonClick(propGrid, value);
    }
    return _CommonCallback45(blocked, (PyObject*)m_scriptObject, funcobj, propGrid, value);
    MySWIGOutputDebugString(wxT("PyDirProperty::OnButtonClick() exit"));
}


void PyDirProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::OnCustomPaint() exit (fall-back)"));
        wxDirProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyDirProperty::OnCustomPaint() exit"));
}


bool PyDirProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::OnEvent() exit (fall-back)"));
        return wxDirProperty::OnEvent(propgrid, primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, primary, event);
    MySWIGOutputDebugString(wxT("PyDirProperty::OnEvent() exit"));
}


wxSize PyDirProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::OnMeasureImage() exit (fall-back)"));
        return wxDirProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyDirProperty::OnMeasureImage() exit"));
}


void PyDirProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::OnSetValue() exit (fall-back)"));
        wxDirProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDirProperty::OnSetValue() exit"));
}


void PyDirProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::OnValidationFailure() exit (fall-back)"));
        wxDirProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyDirProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyDirProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::PyIntToValue() exit (fall-back)"));
        return wxDirProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyDirProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyDirProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::PyStringToValue() exit (fall-back)"));
        return wxDirProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyDirProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyDirProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::PyValidateValue() exit (fall-back)"));
        return wxDirProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyDirProperty::PyValidateValue() exit"));
}


void PyDirProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyDirProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyDirProperty::RefreshChildren() exit (fall-back)"));
        wxDirProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyDirProperty::RefreshChildren() exit"));
}


class PyCursorProperty : public wxCursorProperty {
public:
        PyCursorProperty( const wxString& label= wxString_wxPG_LABEL,
                      const wxString& name= wxString_wxPG_LABEL,
                      int value = 0 );
    virtual ~PyCursorProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual const wxString* GetEntry( size_t index, int* pvalue ) const;
    virtual wxSize GetImageSize() const;
    virtual int GetIndexForValue( int value ) const;
    virtual wxString GetValueAsString( int argFlags ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyCursorProperty_pyClass = NULL;


PyCursorProperty::PyCursorProperty(const wxString& label, const wxString& name, int value)
    : wxCursorProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyCursorProperty::PyCursorProperty()"));

    Init();
}

PyCursorProperty::~PyCursorProperty()
{
    MySWIGOutputDebugString(wxT("PyCursorProperty::~PyCursorProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyCursorProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyCursorProperty_pyClass )
    {
        gs_PyCursorProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyCursorProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyCursorProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::ChildChanged() exit (fall-back)"));
        wxCursorProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyCursorProperty::ChildChanged() exit"));
}


wxVariant PyCursorProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::DoGetAttribute() exit (fall-back)"));
        return wxCursorProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyCursorProperty::DoGetAttribute() exit"));
}


wxValidator* PyCursorProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::DoGetValidator() exit (fall-back)"));
        return wxCursorProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCursorProperty::DoGetValidator() exit"));
}


wxVariant PyCursorProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::DoGetValue() exit (fall-back)"));
        return wxCursorProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCursorProperty::DoGetValue() exit"));
}


bool PyCursorProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::DoSetAttribute() exit (fall-back)"));
        return wxCursorProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyCursorProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyCursorProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::GetCellRenderer() exit (fall-back)"));
        return wxCursorProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetCellRenderer() exit"));
}


int PyCursorProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::GetChoiceInfo() exit (fall-back)"));
        return wxCursorProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetChoiceInfo() exit"));
}


wxString PyCursorProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::GetEditor() exit (fall-back)"));
        return wxCursorProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyCursorProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::GetEditorDialog() exit (fall-back)"));
        return wxCursorProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetEditorDialog() exit"));
}


const wxString* PyCursorProperty::GetEntry(size_t index, int* pvalue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetEntry() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEntry_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEntry_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::GetEntry() exit (fall-back)"));
        return wxCursorProperty::GetEntry(index, pvalue);
    }
    return _CommonCallback30(blocked, (PyObject*)m_scriptObject, funcobj, index, pvalue);
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetEntry() exit"));
}


wxSize PyCursorProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::GetImageSize() exit (fall-back)"));
        return wxCursorProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetImageSize() exit"));
}


int PyCursorProperty::GetIndexForValue(int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetIndexForValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetIndexForValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetIndexForValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::GetIndexForValue() exit (fall-back)"));
        return wxCursorProperty::GetIndexForValue(value);
    }
    return _CommonCallback31(blocked, (PyObject*)m_scriptObject, funcobj, value);
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetIndexForValue() exit"));
}


wxString PyCursorProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::GetValueAsString() exit (fall-back)"));
        return wxCursorProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyCursorProperty::GetValueAsString() exit"));
}


void PyCursorProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::OnCustomPaint() exit (fall-back)"));
        wxCursorProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnCustomPaint() exit"));
}


bool PyCursorProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::OnEvent() exit (fall-back)"));
        return wxCursorProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnEvent() exit"));
}


wxSize PyCursorProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::OnMeasureImage() exit (fall-back)"));
        return wxCursorProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnMeasureImage() exit"));
}


void PyCursorProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::OnSetValue() exit (fall-back)"));
        wxCursorProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnSetValue() exit"));
}


void PyCursorProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::OnValidationFailure() exit (fall-back)"));
        wxCursorProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyCursorProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyCursorProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::PyIntToValue() exit (fall-back)"));
        return wxCursorProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyCursorProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyCursorProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::PyStringToValue() exit (fall-back)"));
        return wxCursorProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyCursorProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyCursorProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::PyValidateValue() exit (fall-back)"));
        return wxCursorProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyCursorProperty::PyValidateValue() exit"));
}


void PyCursorProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyCursorProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyCursorProperty::RefreshChildren() exit (fall-back)"));
        wxCursorProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyCursorProperty::RefreshChildren() exit"));
}


class PyFileProperty : public wxFileProperty {
public:
        PyFileProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL,
        const wxString& value = wxEmptyString );
    virtual ~PyFileProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator() const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyFileProperty_pyClass = NULL;


PyFileProperty::PyFileProperty(const wxString& label, const wxString& name, const wxString& value)
    : wxFileProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyFileProperty::PyFileProperty()"));

    Init();
}

PyFileProperty::~PyFileProperty()
{
    MySWIGOutputDebugString(wxT("PyFileProperty::~PyFileProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyFileProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyFileProperty_pyClass )
    {
        gs_PyFileProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyFileProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyFileProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::ChildChanged() exit (fall-back)"));
        wxFileProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyFileProperty::ChildChanged() exit"));
}


wxVariant PyFileProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::DoGetAttribute() exit (fall-back)"));
        return wxFileProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyFileProperty::DoGetAttribute() exit"));
}


wxValidator* PyFileProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::DoGetValidator() exit (fall-back)"));
        return wxFileProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFileProperty::DoGetValidator() exit"));
}


wxVariant PyFileProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::DoGetValue() exit (fall-back)"));
        return wxFileProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFileProperty::DoGetValue() exit"));
}


bool PyFileProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::DoSetAttribute() exit (fall-back)"));
        return wxFileProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyFileProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyFileProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::GetCellRenderer() exit (fall-back)"));
        return wxFileProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyFileProperty::GetCellRenderer() exit"));
}


int PyFileProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::GetChoiceInfo() exit (fall-back)"));
        return wxFileProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyFileProperty::GetChoiceInfo() exit"));
}


wxString PyFileProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::GetEditor() exit (fall-back)"));
        return wxFileProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFileProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyFileProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::GetEditorDialog() exit (fall-back)"));
        return wxFileProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFileProperty::GetEditorDialog() exit"));
}


wxSize PyFileProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::GetImageSize() exit (fall-back)"));
        return wxFileProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFileProperty::GetImageSize() exit"));
}


wxString PyFileProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::GetValueAsString() exit (fall-back)"));
        return wxFileProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyFileProperty::GetValueAsString() exit"));
}


void PyFileProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::OnCustomPaint() exit (fall-back)"));
        wxFileProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyFileProperty::OnCustomPaint() exit"));
}


bool PyFileProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::OnEvent() exit (fall-back)"));
        return wxFileProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyFileProperty::OnEvent() exit"));
}


wxSize PyFileProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::OnMeasureImage() exit (fall-back)"));
        return wxFileProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyFileProperty::OnMeasureImage() exit"));
}


void PyFileProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::OnSetValue() exit (fall-back)"));
        wxFileProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFileProperty::OnSetValue() exit"));
}


void PyFileProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::OnValidationFailure() exit (fall-back)"));
        wxFileProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyFileProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyFileProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::PyIntToValue() exit (fall-back)"));
        return wxFileProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyFileProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyFileProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::PyStringToValue() exit (fall-back)"));
        return wxFileProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyFileProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyFileProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::PyValidateValue() exit (fall-back)"));
        return wxFileProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyFileProperty::PyValidateValue() exit"));
}


void PyFileProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyFileProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyFileProperty::RefreshChildren() exit (fall-back)"));
        wxFileProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyFileProperty::RefreshChildren() exit"));
}


class PyMultiChoiceProperty : public wxMultiChoiceProperty {
public:
        PyMultiChoiceProperty( const wxString& label,
                           const wxString& name,
                           const wxArrayString& strings,
                           const wxArrayString& value );
    virtual ~PyMultiChoiceProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int flags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyMultiChoiceProperty_pyClass = NULL;


PyMultiChoiceProperty::PyMultiChoiceProperty(const wxString& label, const wxString& name, const wxArrayString& strings, const wxArrayString& value)
    : wxMultiChoiceProperty(label, name, strings, value)
{
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyMultiChoiceProperty()"));

    Init();
}

PyMultiChoiceProperty::~PyMultiChoiceProperty()
{
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::~PyMultiChoiceProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyMultiChoiceProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyMultiChoiceProperty_pyClass )
    {
        gs_PyMultiChoiceProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyMultiChoiceProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyMultiChoiceProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::ChildChanged() exit (fall-back)"));
        wxMultiChoiceProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::ChildChanged() exit"));
}


wxVariant PyMultiChoiceProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoGetAttribute() exit (fall-back)"));
        return wxMultiChoiceProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoGetAttribute() exit"));
}


wxValidator* PyMultiChoiceProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoGetValidator() exit (fall-back)"));
        return wxMultiChoiceProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoGetValidator() exit"));
}


wxVariant PyMultiChoiceProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoGetValue() exit (fall-back)"));
        return wxMultiChoiceProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoGetValue() exit"));
}


bool PyMultiChoiceProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoSetAttribute() exit (fall-back)"));
        return wxMultiChoiceProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyMultiChoiceProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetCellRenderer() exit (fall-back)"));
        return wxMultiChoiceProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetCellRenderer() exit"));
}


int PyMultiChoiceProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetChoiceInfo() exit (fall-back)"));
        return wxMultiChoiceProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetChoiceInfo() exit"));
}


wxString PyMultiChoiceProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetEditor() exit (fall-back)"));
        return wxMultiChoiceProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyMultiChoiceProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetEditorDialog() exit (fall-back)"));
        return wxMultiChoiceProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetEditorDialog() exit"));
}


wxSize PyMultiChoiceProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetImageSize() exit (fall-back)"));
        return wxMultiChoiceProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetImageSize() exit"));
}


wxString PyMultiChoiceProperty::GetValueAsString(int flags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetValueAsString() exit (fall-back)"));
        return wxMultiChoiceProperty::GetValueAsString(flags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, flags);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::GetValueAsString() exit"));
}


void PyMultiChoiceProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnCustomPaint() exit (fall-back)"));
        wxMultiChoiceProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnCustomPaint() exit"));
}


bool PyMultiChoiceProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnEvent() exit (fall-back)"));
        return wxMultiChoiceProperty::OnEvent(propgrid, primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, primary, event);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnEvent() exit"));
}


wxSize PyMultiChoiceProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnMeasureImage() exit (fall-back)"));
        return wxMultiChoiceProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnMeasureImage() exit"));
}


void PyMultiChoiceProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnSetValue() exit (fall-back)"));
        wxMultiChoiceProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnSetValue() exit"));
}


void PyMultiChoiceProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnValidationFailure() exit (fall-back)"));
        wxMultiChoiceProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyMultiChoiceProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyIntToValue() exit (fall-back)"));
        return wxMultiChoiceProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyMultiChoiceProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyStringToValue() exit (fall-back)"));
        return wxMultiChoiceProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyMultiChoiceProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyValidateValue() exit (fall-back)"));
        return wxMultiChoiceProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::PyValidateValue() exit"));
}


void PyMultiChoiceProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::RefreshChildren() exit (fall-back)"));
        wxMultiChoiceProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyMultiChoiceProperty::RefreshChildren() exit"));
}


class PyIntProperty : public wxIntProperty {
public:
        PyIntProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL,
                   long value = 0 );
    PyIntProperty( const wxString& label, const wxString& name, const wxLongLong& value );
    virtual ~PyIntProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator() const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyIntProperty_pyClass = NULL;


PyIntProperty::PyIntProperty(const wxString& label, const wxString& name, long value)
    : wxIntProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyIntProperty::PyIntProperty()"));

    Init();
}

PyIntProperty::PyIntProperty(const wxString& label, const wxString& name, const wxLongLong& value)
    : wxIntProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyIntProperty::PyIntProperty()"));

    Init();
}

PyIntProperty::~PyIntProperty()
{
    MySWIGOutputDebugString(wxT("PyIntProperty::~PyIntProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyIntProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyIntProperty_pyClass )
    {
        gs_PyIntProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyIntProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyIntProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::ChildChanged() exit (fall-back)"));
        wxIntProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyIntProperty::ChildChanged() exit"));
}


wxVariant PyIntProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::DoGetAttribute() exit (fall-back)"));
        return wxIntProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyIntProperty::DoGetAttribute() exit"));
}


wxValidator* PyIntProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::DoGetValidator() exit (fall-back)"));
        return wxIntProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyIntProperty::DoGetValidator() exit"));
}


wxVariant PyIntProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::DoGetValue() exit (fall-back)"));
        return wxIntProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyIntProperty::DoGetValue() exit"));
}


bool PyIntProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::DoSetAttribute() exit (fall-back)"));
        return wxIntProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyIntProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyIntProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::GetCellRenderer() exit (fall-back)"));
        return wxIntProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyIntProperty::GetCellRenderer() exit"));
}


int PyIntProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::GetChoiceInfo() exit (fall-back)"));
        return wxIntProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyIntProperty::GetChoiceInfo() exit"));
}


wxString PyIntProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::GetEditor() exit (fall-back)"));
        return wxIntProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyIntProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyIntProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::GetEditorDialog() exit (fall-back)"));
        return wxIntProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyIntProperty::GetEditorDialog() exit"));
}


wxSize PyIntProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::GetImageSize() exit (fall-back)"));
        return wxIntProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyIntProperty::GetImageSize() exit"));
}


wxString PyIntProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::GetValueAsString() exit (fall-back)"));
        return wxIntProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyIntProperty::GetValueAsString() exit"));
}


void PyIntProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::OnCustomPaint() exit (fall-back)"));
        wxIntProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyIntProperty::OnCustomPaint() exit"));
}


bool PyIntProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::OnEvent() exit (fall-back)"));
        return wxIntProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyIntProperty::OnEvent() exit"));
}


wxSize PyIntProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::OnMeasureImage() exit (fall-back)"));
        return wxIntProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyIntProperty::OnMeasureImage() exit"));
}


void PyIntProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::OnSetValue() exit (fall-back)"));
        wxIntProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyIntProperty::OnSetValue() exit"));
}


void PyIntProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::OnValidationFailure() exit (fall-back)"));
        wxIntProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyIntProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyIntProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::PyIntToValue() exit (fall-back)"));
        return wxIntProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyIntProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyIntProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::PyStringToValue() exit (fall-back)"));
        return wxIntProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyIntProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyIntProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::PyValidateValue() exit (fall-back)"));
        return wxIntProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyIntProperty::PyValidateValue() exit"));
}


void PyIntProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyIntProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyIntProperty::RefreshChildren() exit (fall-back)"));
        wxIntProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyIntProperty::RefreshChildren() exit"));
}


class PyEditor : public wxPGEditor {
public:
        PyEditor();
    virtual ~PyEditor();
    void _SetSelf(PyObject *self);
    virtual bool CanContainCustomImage() const;
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property,
        const wxPoint& pos, const wxSize& size ) const;
    virtual void DeleteItem( wxWindow* ctrl, int index ) const;
    virtual void DrawValue( wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text ) const;
    virtual wxString GetName() const;
    virtual int InsertItem( wxWindow* ctrl, const wxString& label, int index ) const;
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property,
        wxWindow* wnd_primary, wxEvent& event ) const;
    virtual void OnFocus( wxPGProperty* property, wxWindow* wnd ) const;
    virtual wxPGVariantAndBool PyGetValueFromControl( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const;
    virtual void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const;
    virtual void SetValueToUnspecified( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const;
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyEditor_pyClass = NULL;


PyEditor::PyEditor()
    : wxPGEditor()
{
    MySWIGOutputDebugString(wxT("PyEditor::PyEditor()"));

    Init();
}

PyEditor::~PyEditor()
{
    MySWIGOutputDebugString(wxT("PyEditor::~PyEditor()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyEditor::_SetSelf(PyObject *self)
{
    if ( !gs_PyEditor_pyClass )
    {
        gs_PyEditor_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyEditor_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyEditor::CanContainCustomImage() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::CanContainCustomImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CanContainCustomImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CanContainCustomImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::CanContainCustomImage() exit (fall-back)"));
        return wxPGEditor::CanContainCustomImage();
    }
    return _CommonCallback0(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEditor::CanContainCustomImage() exit"));
}


wxPGWindowList PyEditor::CreateControls(wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& size) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::CreateControls() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CreateControls_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CreateControls_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::CreateControls() exit (not implemented!!!)"));
        PyErr_SetString(PyExc_TypeError,"this method must be implemented");
        return wxPGWindowList();
    }
    return _CommonCallback34(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, pos, size);
    MySWIGOutputDebugString(wxT("PyEditor::CreateControls() exit"));
}


void PyEditor::DeleteItem(wxWindow* ctrl, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::DeleteItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DeleteItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DeleteItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::DeleteItem() exit (fall-back)"));
        wxPGEditor::DeleteItem(ctrl, index);
        return;
    }
    _CommonCallback2(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, index);
    MySWIGOutputDebugString(wxT("PyEditor::DeleteItem() exit"));
}


void PyEditor::DrawValue(wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::DrawValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DrawValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DrawValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::DrawValue() exit (fall-back)"));
        wxPGEditor::DrawValue(dc, rect, property, text);
        return;
    }
    _CommonCallback35(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, property, text);
    MySWIGOutputDebugString(wxT("PyEditor::DrawValue() exit"));
}


wxString PyEditor::GetName() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::GetName() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetName_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetName_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::GetName() exit (not implemented!!!)"));
        PyErr_SetString(PyExc_TypeError,"this method must be implemented");
        return wxEmptyString;
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyEditor::GetName() exit"));
}


int PyEditor::InsertItem(wxWindow* ctrl, const wxString& label, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::InsertItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_InsertItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_InsertItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::InsertItem() exit (fall-back)"));
        return wxPGEditor::InsertItem(ctrl, label, index);
    }
    return _CommonCallback5(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, label, index);
    MySWIGOutputDebugString(wxT("PyEditor::InsertItem() exit"));
}


bool PyEditor::OnEvent(wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* wnd_primary, wxEvent& event) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::OnEvent() exit (not implemented!!!)"));
        PyErr_SetString(PyExc_TypeError,"this method must be implemented");
        return false;
    }
    return _CommonCallback36(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyEditor::OnEvent() exit"));
}


void PyEditor::OnFocus(wxPGProperty* property, wxWindow* wnd) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::OnFocus() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnFocus_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnFocus_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::OnFocus() exit (fall-back)"));
        wxPGEditor::OnFocus(property, wnd);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, wnd);
    MySWIGOutputDebugString(wxT("PyEditor::OnFocus() exit"));
}


wxPGVariantAndBool PyEditor::PyGetValueFromControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::PyGetValueFromControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueFromControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueFromControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::PyGetValueFromControl() exit (fall-back)"));
        return wxPGEditor::PyGetValueFromControl(property, ctrl);
    }
    return _CommonCallback38(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyEditor::PyGetValueFromControl() exit"));
}


void PyEditor::SetControlIntValue(wxPGProperty* property, wxWindow* ctrl, int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::SetControlIntValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlIntValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlIntValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::SetControlIntValue() exit (fall-back)"));
        wxPGEditor::SetControlIntValue(property, ctrl, value);
        return;
    }
    _CommonCallback39(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, value);
    MySWIGOutputDebugString(wxT("PyEditor::SetControlIntValue() exit"));
}


void PyEditor::SetControlStringValue(wxPGProperty* property, wxWindow* ctrl, const wxString& txt) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::SetControlStringValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlStringValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlStringValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::SetControlStringValue() exit (fall-back)"));
        wxPGEditor::SetControlStringValue(property, ctrl, txt);
        return;
    }
    _CommonCallback40(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, txt);
    MySWIGOutputDebugString(wxT("PyEditor::SetControlStringValue() exit"));
}


void PyEditor::SetValueToUnspecified(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::SetValueToUnspecified() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetValueToUnspecified_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetValueToUnspecified_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::SetValueToUnspecified() exit (not implemented!!!)"));
        PyErr_SetString(PyExc_TypeError,"this method must be implemented");
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyEditor::SetValueToUnspecified() exit"));
}


void PyEditor::UpdateControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyEditor::UpdateControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_UpdateControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_UpdateControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyEditor::UpdateControl() exit (not implemented!!!)"));
        PyErr_SetString(PyExc_TypeError,"this method must be implemented");
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyEditor::UpdateControl() exit"));
}


class PyChoiceEditor : public wxPGChoiceEditor {
public:
        PyChoiceEditor();
    virtual ~PyChoiceEditor();
    void _SetSelf(PyObject *self);
    virtual bool CanContainCustomImage() const;
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz ) const;
    virtual void DeleteItem( wxWindow* ctrl, int index ) const;
    virtual void DrawValue( wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text ) const;
    virtual wxString GetName() const;
    virtual int InsertItem( wxWindow* ctrl, const wxString& label, int index ) const;
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event ) const;
    virtual void OnFocus( wxPGProperty* property, wxWindow* wnd ) const;
    virtual wxPGVariantAndBool PyGetValueFromControl( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const;
    virtual void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const;
    virtual void SetValueToUnspecified( wxPGProperty* property, wxWindow* ctrl ) const;
    virtual void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const;
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyChoiceEditor_pyClass = NULL;


PyChoiceEditor::PyChoiceEditor()
    : wxPGChoiceEditor()
{
    MySWIGOutputDebugString(wxT("PyChoiceEditor::PyChoiceEditor()"));

    Init();
}

PyChoiceEditor::~PyChoiceEditor()
{
    MySWIGOutputDebugString(wxT("PyChoiceEditor::~PyChoiceEditor()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyChoiceEditor::_SetSelf(PyObject *self)
{
    if ( !gs_PyChoiceEditor_pyClass )
    {
        gs_PyChoiceEditor_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyChoiceEditor_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyChoiceEditor::CanContainCustomImage() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::CanContainCustomImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CanContainCustomImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CanContainCustomImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::CanContainCustomImage() exit (fall-back)"));
        return wxPGChoiceEditor::CanContainCustomImage();
    }
    return _CommonCallback0(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::CanContainCustomImage() exit"));
}


wxPGWindowList PyChoiceEditor::CreateControls(wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::CreateControls() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_CreateControls_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_CreateControls_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::CreateControls() exit (fall-back)"));
        return wxPGChoiceEditor::CreateControls(propgrid, property, pos, sz);
    }
    return _CommonCallback34(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, pos, sz);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::CreateControls() exit"));
}


void PyChoiceEditor::DeleteItem(wxWindow* ctrl, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::DeleteItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DeleteItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DeleteItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::DeleteItem() exit (fall-back)"));
        wxPGChoiceEditor::DeleteItem(ctrl, index);
        return;
    }
    _CommonCallback2(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, index);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::DeleteItem() exit"));
}


void PyChoiceEditor::DrawValue(wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::DrawValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DrawValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DrawValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::DrawValue() exit (fall-back)"));
        wxPGChoiceEditor::DrawValue(dc, rect, property, text);
        return;
    }
    _CommonCallback35(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, property, text);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::DrawValue() exit"));
}


wxString PyChoiceEditor::GetName() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::GetName() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetName_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetName_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::GetName() exit (fall-back)"));
        return wxPGChoiceEditor::GetName();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::GetName() exit"));
}


int PyChoiceEditor::InsertItem(wxWindow* ctrl, const wxString& label, int index) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::InsertItem() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_InsertItem_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_InsertItem_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::InsertItem() exit (fall-back)"));
        return wxPGChoiceEditor::InsertItem(ctrl, label, index);
    }
    return _CommonCallback5(blocked, (PyObject*)m_scriptObject, funcobj, ctrl, label, index);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::InsertItem() exit"));
}


bool PyChoiceEditor::OnEvent(wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* primary, wxEvent& event) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::OnEvent() exit (fall-back)"));
        return wxPGChoiceEditor::OnEvent(propgrid, property, primary, event);
    }
    return _CommonCallback36(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, property, primary, event);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::OnEvent() exit"));
}


void PyChoiceEditor::OnFocus(wxPGProperty* property, wxWindow* wnd) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::OnFocus() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnFocus_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnFocus_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::OnFocus() exit (fall-back)"));
        wxPGChoiceEditor::OnFocus(property, wnd);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, wnd);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::OnFocus() exit"));
}


wxPGVariantAndBool PyChoiceEditor::PyGetValueFromControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::PyGetValueFromControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueFromControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueFromControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::PyGetValueFromControl() exit (fall-back)"));
        return wxPGChoiceEditor::PyGetValueFromControl(property, ctrl);
    }
    return _CommonCallback38(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::PyGetValueFromControl() exit"));
}


void PyChoiceEditor::SetControlIntValue(wxPGProperty* property, wxWindow* ctrl, int value) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::SetControlIntValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlIntValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlIntValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::SetControlIntValue() exit (fall-back)"));
        wxPGChoiceEditor::SetControlIntValue(property, ctrl, value);
        return;
    }
    _CommonCallback39(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, value);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::SetControlIntValue() exit"));
}


void PyChoiceEditor::SetControlStringValue(wxPGProperty* property, wxWindow* ctrl, const wxString& txt) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::SetControlStringValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetControlStringValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetControlStringValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::SetControlStringValue() exit (fall-back)"));
        wxPGChoiceEditor::SetControlStringValue(property, ctrl, txt);
        return;
    }
    _CommonCallback40(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl, txt);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::SetControlStringValue() exit"));
}


void PyChoiceEditor::SetValueToUnspecified(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::SetValueToUnspecified() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_SetValueToUnspecified_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_SetValueToUnspecified_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::SetValueToUnspecified() exit (fall-back)"));
        wxPGChoiceEditor::SetValueToUnspecified(property, ctrl);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::SetValueToUnspecified() exit"));
}


void PyChoiceEditor::UpdateControl(wxPGProperty* property, wxWindow* ctrl) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyChoiceEditor::UpdateControl() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_UpdateControl_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_UpdateControl_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyChoiceEditor::UpdateControl() exit (fall-back)"));
        wxPGChoiceEditor::UpdateControl(property, ctrl);
        return;
    }
    _CommonCallback37(blocked, (PyObject*)m_scriptObject, funcobj, property, ctrl);
    MySWIGOutputDebugString(wxT("PyChoiceEditor::UpdateControl() exit"));
}


class PyLongStringDialogAdapter : public wxPGLongStringDialogAdapter {
public:
        PyLongStringDialogAdapter();
    virtual ~PyLongStringDialogAdapter();
    void _SetSelf(PyObject *self);
    virtual bool DoShowDialog( wxPropertyGrid* propGrid, wxPGProperty* property );
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyLongStringDialogAdapter_pyClass = NULL;


PyLongStringDialogAdapter::PyLongStringDialogAdapter()
    : wxPGLongStringDialogAdapter()
{
    MySWIGOutputDebugString(wxT("PyLongStringDialogAdapter::PyLongStringDialogAdapter()"));
    Init();
}

PyLongStringDialogAdapter::~PyLongStringDialogAdapter()
{
    MySWIGOutputDebugString(wxT("PyLongStringDialogAdapter::~PyLongStringDialogAdapter()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyLongStringDialogAdapter::_SetSelf(PyObject *self)
{
    if ( !gs_PyLongStringDialogAdapter_pyClass )
    {
        gs_PyLongStringDialogAdapter_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyLongStringDialogAdapter_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


bool PyLongStringDialogAdapter::DoShowDialog(wxPropertyGrid* propGrid, wxPGProperty* property)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringDialogAdapter::DoShowDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoShowDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoShowDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringDialogAdapter::DoShowDialog() exit (fall-back)"));
        return wxPGLongStringDialogAdapter::DoShowDialog(propGrid, property);
    }
    PyObject* res;
    PyObject* py_propGrid;
    py_propGrid = SWIG_NewPointerObj((void*)propGrid, SWIGTYPE_p_wxPropertyGrid, 0);
    PyObject* py_property;
        py_property = NULL;
    if ( property->m_scriptObject ) py_property = (PyObject*)property->m_scriptObject;
    if ( py_property ) Py_INCREF(py_property);
    else py_property = SWIG_NewPointerObj((void*)property, SWIGTYPE_p_wxPGProperty, 0);

    res = PyObject_CallFunctionObjArgs(funcobj, ((PyObject*)m_scriptObject), py_propGrid, py_property, NULL);
    Py_DECREF(funcobj);
    Py_DECREF(py_property);
    Py_DECREF(py_propGrid);
    if (PyErr_Occurred()) SWIG_fail;
    {
    bool retval;
        if ( !SWIG_IsOK(SWIG_AsVal_bool(res, &retval)) ) {
        PyErr_SetString(PyExc_TypeError,"expected bool");
        SWIG_fail;
    }
    Py_DECREF(res);
    wxPyEndBlockThreads(blocked);
    return retval;
    }
  fail:
    if ( PyErr_Occurred() ) PyErr_Print();
    wxPyEndBlockThreads(blocked);
    return false;
    MySWIGOutputDebugString(wxT("PyLongStringDialogAdapter::DoShowDialog() exit"));
}


class PyBoolProperty : public wxBoolProperty {
public:
        PyBoolProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL,
                    bool value = false );
    virtual ~PyBoolProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyBoolProperty_pyClass = NULL;


PyBoolProperty::PyBoolProperty(const wxString& label, const wxString& name, bool value)
    : wxBoolProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyBoolProperty::PyBoolProperty()"));

    Init();
}

PyBoolProperty::~PyBoolProperty()
{
    MySWIGOutputDebugString(wxT("PyBoolProperty::~PyBoolProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyBoolProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyBoolProperty_pyClass )
    {
        gs_PyBoolProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyBoolProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyBoolProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::ChildChanged() exit (fall-back)"));
        wxBoolProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyBoolProperty::ChildChanged() exit"));
}


wxVariant PyBoolProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::DoGetAttribute() exit (fall-back)"));
        return wxBoolProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyBoolProperty::DoGetAttribute() exit"));
}


wxValidator* PyBoolProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::DoGetValidator() exit (fall-back)"));
        return wxBoolProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBoolProperty::DoGetValidator() exit"));
}


wxVariant PyBoolProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::DoGetValue() exit (fall-back)"));
        return wxBoolProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBoolProperty::DoGetValue() exit"));
}


bool PyBoolProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::DoSetAttribute() exit (fall-back)"));
        return wxBoolProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyBoolProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyBoolProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::GetCellRenderer() exit (fall-back)"));
        return wxBoolProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetCellRenderer() exit"));
}


int PyBoolProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::GetChoiceInfo() exit (fall-back)"));
        return wxBoolProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetChoiceInfo() exit"));
}


wxString PyBoolProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::GetEditor() exit (fall-back)"));
        return wxBoolProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyBoolProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::GetEditorDialog() exit (fall-back)"));
        return wxBoolProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetEditorDialog() exit"));
}


wxSize PyBoolProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::GetImageSize() exit (fall-back)"));
        return wxBoolProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetImageSize() exit"));
}


wxString PyBoolProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::GetValueAsString() exit (fall-back)"));
        return wxBoolProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyBoolProperty::GetValueAsString() exit"));
}


void PyBoolProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::OnCustomPaint() exit (fall-back)"));
        wxBoolProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnCustomPaint() exit"));
}


bool PyBoolProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::OnEvent() exit (fall-back)"));
        return wxBoolProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnEvent() exit"));
}


wxSize PyBoolProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::OnMeasureImage() exit (fall-back)"));
        return wxBoolProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnMeasureImage() exit"));
}


void PyBoolProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::OnSetValue() exit (fall-back)"));
        wxBoolProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnSetValue() exit"));
}


void PyBoolProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::OnValidationFailure() exit (fall-back)"));
        wxBoolProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyBoolProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyBoolProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::PyIntToValue() exit (fall-back)"));
        return wxBoolProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyBoolProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyBoolProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::PyStringToValue() exit (fall-back)"));
        return wxBoolProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyBoolProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyBoolProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::PyValidateValue() exit (fall-back)"));
        return wxBoolProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyBoolProperty::PyValidateValue() exit"));
}


void PyBoolProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyBoolProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyBoolProperty::RefreshChildren() exit (fall-back)"));
        wxBoolProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyBoolProperty::RefreshChildren() exit"));
}


class PyUIntProperty : public wxUIntProperty {
public:
        PyUIntProperty( const wxString& label = wxString_wxPG_LABEL, const wxString& name = wxString_wxPG_LABEL,
                   unsigned long value = 0 );
    PyUIntProperty( const wxString& label, const wxString& name, const wxULongLong& value );
    virtual ~PyUIntProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyUIntProperty_pyClass = NULL;


PyUIntProperty::PyUIntProperty(const wxString& label, const wxString& name, unsigned long value)
    : wxUIntProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyUIntProperty::PyUIntProperty()"));

    Init();
}

PyUIntProperty::PyUIntProperty(const wxString& label, const wxString& name, const wxULongLong& value)
    : wxUIntProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyUIntProperty::PyUIntProperty()"));

    Init();
}

PyUIntProperty::~PyUIntProperty()
{
    MySWIGOutputDebugString(wxT("PyUIntProperty::~PyUIntProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyUIntProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyUIntProperty_pyClass )
    {
        gs_PyUIntProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyUIntProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyUIntProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::ChildChanged() exit (fall-back)"));
        wxUIntProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyUIntProperty::ChildChanged() exit"));
}


wxVariant PyUIntProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::DoGetAttribute() exit (fall-back)"));
        return wxUIntProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyUIntProperty::DoGetAttribute() exit"));
}


wxValidator* PyUIntProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::DoGetValidator() exit (fall-back)"));
        return wxUIntProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyUIntProperty::DoGetValidator() exit"));
}


wxVariant PyUIntProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::DoGetValue() exit (fall-back)"));
        return wxUIntProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyUIntProperty::DoGetValue() exit"));
}


bool PyUIntProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::DoSetAttribute() exit (fall-back)"));
        return wxUIntProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyUIntProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyUIntProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::GetCellRenderer() exit (fall-back)"));
        return wxUIntProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetCellRenderer() exit"));
}


int PyUIntProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::GetChoiceInfo() exit (fall-back)"));
        return wxUIntProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetChoiceInfo() exit"));
}


wxString PyUIntProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::GetEditor() exit (fall-back)"));
        return wxUIntProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyUIntProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::GetEditorDialog() exit (fall-back)"));
        return wxUIntProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetEditorDialog() exit"));
}


wxSize PyUIntProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::GetImageSize() exit (fall-back)"));
        return wxUIntProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetImageSize() exit"));
}


wxString PyUIntProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::GetValueAsString() exit (fall-back)"));
        return wxUIntProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyUIntProperty::GetValueAsString() exit"));
}


void PyUIntProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::OnCustomPaint() exit (fall-back)"));
        wxUIntProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnCustomPaint() exit"));
}


bool PyUIntProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::OnEvent() exit (fall-back)"));
        return wxUIntProperty::OnEvent(propgrid, wnd_primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, wnd_primary, event);
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnEvent() exit"));
}


wxSize PyUIntProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::OnMeasureImage() exit (fall-back)"));
        return wxUIntProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnMeasureImage() exit"));
}


void PyUIntProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::OnSetValue() exit (fall-back)"));
        wxUIntProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnSetValue() exit"));
}


void PyUIntProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::OnValidationFailure() exit (fall-back)"));
        wxUIntProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyUIntProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyUIntProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::PyIntToValue() exit (fall-back)"));
        return wxUIntProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyUIntProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyUIntProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::PyStringToValue() exit (fall-back)"));
        return wxUIntProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyUIntProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyUIntProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::PyValidateValue() exit (fall-back)"));
        return wxUIntProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyUIntProperty::PyValidateValue() exit"));
}


void PyUIntProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyUIntProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyUIntProperty::RefreshChildren() exit (fall-back)"));
        wxUIntProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyUIntProperty::RefreshChildren() exit"));
}


class PyLongStringProperty : public wxLongStringProperty {
public:
        PyLongStringProperty( const wxString& label = wxString_wxPG_LABEL,
                          const wxString& name = wxString_wxPG_LABEL,
                          const wxString& value = wxEmptyString );
    virtual ~PyLongStringProperty();
    void _SetSelf(PyObject *self);
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
    virtual wxVariant DoGetAttribute( const wxString& name ) const;
    virtual wxValidator* DoGetValidator () const;
    virtual wxVariant DoGetValue() const;
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
    virtual wxString GetEditor() const;
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    virtual wxSize GetImageSize() const;
    virtual wxString GetValueAsString( int argFlags = 0 ) const;
    virtual bool OnButtonClick( wxPropertyGrid* propgrid, wxString& value );
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item = -1 ) const;
    virtual void OnSetValue();
    virtual void OnValidationFailure( wxVariant& pendingValue );
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value, wxPGValidationInfo& validationInfo ) const;
    virtual void RefreshChildren();
private:
    void Init() { if ( !gs_funcNamesInitialized ) _InitFuncNames(); }
};

static PyObject* gs_PyLongStringProperty_pyClass = NULL;


PyLongStringProperty::PyLongStringProperty(const wxString& label, const wxString& name, const wxString& value)
    : wxLongStringProperty(label, name, value)
{
    MySWIGOutputDebugString(wxT("PyLongStringProperty::PyLongStringProperty()"));

    Init();
}

PyLongStringProperty::~PyLongStringProperty()
{
    MySWIGOutputDebugString(wxT("PyLongStringProperty::~PyLongStringProperty()"));
    if (m_scriptObject) { _deleteOwningObject(m_scriptObject); m_scriptObject = NULL; }
}

void PyLongStringProperty::_SetSelf(PyObject *self)
{
    if ( !gs_PyLongStringProperty_pyClass )
    {
        gs_PyLongStringProperty_pyClass = PyObject_GetAttr(self, gs___class___Name);
        Py_DECREF(gs_PyLongStringProperty_pyClass);
    }
    if ( !m_scriptObject ) {
        m_scriptObject = self;
        Py_INCREF(self);
    }
}


void PyLongStringProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::ChildChanged() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ChildChanged_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ChildChanged_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::ChildChanged() exit (fall-back)"));
        wxLongStringProperty::ChildChanged(thisValue, childIndex, childValue);
        return;
    }
    _CommonCallback11(blocked, (PyObject*)m_scriptObject, funcobj, thisValue, childIndex, childValue);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::ChildChanged() exit"));
}


wxVariant PyLongStringProperty::DoGetAttribute(const wxString& name) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::DoGetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::DoGetAttribute() exit (fall-back)"));
        return wxLongStringProperty::DoGetAttribute(name);
    }
    return _CommonCallback12(blocked, (PyObject*)m_scriptObject, funcobj, name);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::DoGetAttribute() exit"));
}


wxValidator* PyLongStringProperty::DoGetValidator() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::DoGetValidator() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValidator_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValidator_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::DoGetValidator() exit (fall-back)"));
        return wxLongStringProperty::DoGetValidator();
    }
    return _CommonCallback13(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::DoGetValidator() exit"));
}


wxVariant PyLongStringProperty::DoGetValue() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::DoGetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoGetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoGetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::DoGetValue() exit (fall-back)"));
        return wxLongStringProperty::DoGetValue();
    }
    return _CommonCallback14(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::DoGetValue() exit"));
}


bool PyLongStringProperty::DoSetAttribute(const wxString& name, wxVariant& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::DoSetAttribute() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_DoSetAttribute_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_DoSetAttribute_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::DoSetAttribute() exit (fall-back)"));
        return wxLongStringProperty::DoSetAttribute(name, value);
    }
    return _CommonCallback15(blocked, (PyObject*)m_scriptObject, funcobj, name, value);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::DoSetAttribute() exit"));
}


wxPGCellRenderer* PyLongStringProperty::GetCellRenderer(int column) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetCellRenderer() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetCellRenderer_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetCellRenderer_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::GetCellRenderer() exit (fall-back)"));
        return wxLongStringProperty::GetCellRenderer(column);
    }
    return _CommonCallback16(blocked, (PyObject*)m_scriptObject, funcobj, column);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetCellRenderer() exit"));
}


int PyLongStringProperty::GetChoiceInfo(wxPGChoiceInfo* choiceinfo)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetChoiceInfo() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetChoiceInfo_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetChoiceInfo_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::GetChoiceInfo() exit (fall-back)"));
        return wxLongStringProperty::GetChoiceInfo(choiceinfo);
    }
    return _CommonCallback17(blocked, (PyObject*)m_scriptObject, funcobj, choiceinfo);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetChoiceInfo() exit"));
}


wxString PyLongStringProperty::GetEditor() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetEditor() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditor_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditor_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::GetEditor() exit (fall-back)"));
        return wxLongStringProperty::GetEditor();
    }
    return _CommonCallback4(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetEditor() exit"));
}


wxPGEditorDialogAdapter* PyLongStringProperty::GetEditorDialog() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetEditorDialog() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetEditorDialog_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetEditorDialog_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::GetEditorDialog() exit (fall-back)"));
        return wxLongStringProperty::GetEditorDialog();
    }
    return _CommonCallback18(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetEditorDialog() exit"));
}


wxSize PyLongStringProperty::GetImageSize() const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetImageSize() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetImageSize_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetImageSize_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::GetImageSize() exit (fall-back)"));
        return wxLongStringProperty::GetImageSize();
    }
    return _CommonCallback19(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetImageSize() exit"));
}


wxString PyLongStringProperty::GetValueAsString(int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetValueAsString() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_GetValueAsString_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_GetValueAsString_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::GetValueAsString() exit (fall-back)"));
        return wxLongStringProperty::GetValueAsString(argFlags);
    }
    return _CommonCallback20(blocked, (PyObject*)m_scriptObject, funcobj, argFlags);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::GetValueAsString() exit"));
}


bool PyLongStringProperty::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnButtonClick() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnButtonClick_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnButtonClick_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::OnButtonClick() exit (fall-back)"));
        return wxLongStringProperty::OnButtonClick(propgrid, value);
    }
    return _CommonCallback45(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, value);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnButtonClick() exit"));
}


void PyLongStringProperty::OnCustomPaint(wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnCustomPaint() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnCustomPaint_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnCustomPaint_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::OnCustomPaint() exit (fall-back)"));
        wxLongStringProperty::OnCustomPaint(dc, rect, paintdata);
        return;
    }
    _CommonCallback21(blocked, (PyObject*)m_scriptObject, funcobj, dc, rect, paintdata);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnCustomPaint() exit"));
}


bool PyLongStringProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnEvent() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnEvent_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnEvent_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::OnEvent() exit (fall-back)"));
        return wxLongStringProperty::OnEvent(propgrid, primary, event);
    }
    return _CommonCallback22(blocked, (PyObject*)m_scriptObject, funcobj, propgrid, primary, event);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnEvent() exit"));
}


wxSize PyLongStringProperty::OnMeasureImage(int item) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnMeasureImage() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnMeasureImage_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnMeasureImage_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::OnMeasureImage() exit (fall-back)"));
        return wxLongStringProperty::OnMeasureImage(item);
    }
    return _CommonCallback23(blocked, (PyObject*)m_scriptObject, funcobj, item);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnMeasureImage() exit"));
}


void PyLongStringProperty::OnSetValue()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnSetValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnSetValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnSetValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::OnSetValue() exit (fall-back)"));
        wxLongStringProperty::OnSetValue();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnSetValue() exit"));
}


void PyLongStringProperty::OnValidationFailure(wxVariant& pendingValue)
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnValidationFailure() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_OnValidationFailure_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_OnValidationFailure_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::OnValidationFailure() exit (fall-back)"));
        wxLongStringProperty::OnValidationFailure(pendingValue);
        return;
    }
    _CommonCallback25(blocked, (PyObject*)m_scriptObject, funcobj, pendingValue);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::OnValidationFailure() exit"));
}


wxPGVariantAndBool PyLongStringProperty::PyIntToValue(int number, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::PyIntToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_IntToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_IntToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::PyIntToValue() exit (fall-back)"));
        return wxLongStringProperty::PyIntToValue(number, argFlags);
    }
    return _CommonCallback26(blocked, (PyObject*)m_scriptObject, funcobj, number, argFlags);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::PyIntToValue() exit"));
}


wxPGVariantAndBool PyLongStringProperty::PyStringToValue(const wxString& text, int argFlags) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::PyStringToValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_StringToValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_StringToValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::PyStringToValue() exit (fall-back)"));
        return wxLongStringProperty::PyStringToValue(text, argFlags);
    }
    return _CommonCallback27(blocked, (PyObject*)m_scriptObject, funcobj, text, argFlags);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::PyStringToValue() exit"));
}


wxPGVariantAndBool PyLongStringProperty::PyValidateValue(const wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::PyValidateValue() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_ValidateValue_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_ValidateValue_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::PyValidateValue() exit (fall-back)"));
        return wxLongStringProperty::PyValidateValue(value, validationInfo);
    }
    return _CommonCallback28(blocked, (PyObject*)m_scriptObject, funcobj, value, validationInfo);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::PyValidateValue() exit"));
}


void PyLongStringProperty::RefreshChildren()
{
    wxPyBlock_t blocked = wxPyBeginBlockThreads();
    MySWIGOutputDebugString(wxT("PyLongStringProperty::RefreshChildren() entry"));

    PyObject* cls_ = PyObject_GetAttr((PyObject*)m_scriptObject, gs___class___Name);
    PyObject* funcobj = NULL;
    if ( PyObject_HasAttr(cls_, gs_RefreshChildren_Name) == 1 ) funcobj = PyObject_GetAttr(cls_, gs_RefreshChildren_Name);
    Py_DECREF(cls_);
    if ( !funcobj || PyObject_HasAttr((PyObject*)m_scriptObject, gs__super_call_Name) == 1 )
    {
        wxPyEndBlockThreads(blocked);
        MySWIGOutputDebugString(wxT("PyLongStringProperty::RefreshChildren() exit (fall-back)"));
        wxLongStringProperty::RefreshChildren();
        return;
    }
    _CommonCallback24(blocked, (PyObject*)m_scriptObject, funcobj);
    MySWIGOutputDebugString(wxT("PyLongStringProperty::RefreshChildren() exit"));
}


%}
