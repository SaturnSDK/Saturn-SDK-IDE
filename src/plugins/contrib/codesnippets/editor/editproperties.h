#ifndef EDITPROPERTIES_H
#define EDITPROPERTIES_H

#include "scrollingdialog.h"

class ScbEditor;

//----------------------------------------------------------------------------
// EditProperties
//----------------------------------------------------------------------------
class EditProperties: public wxScrollingDialog {
//----------------------------------------------------------------------------

public:

    // constructor
    EditProperties (wxWindow* pParent, ScbEditor* pEditor, long style = 0);

private:
    void InitEditProperties (wxWindow* pParent, ScbEditor* pEditor, long style);

};

#endif //EDITPROPERTIES_H
