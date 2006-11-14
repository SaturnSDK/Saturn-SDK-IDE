#ifndef WXSITEMEDITORDRAGASSIST_H
#define WXSITEMEDITORDRAGASSIST_H

#include "wxsitemeditorcontent.h"
#include "../wxsitem.h"
#include <wx/dc.h>
#include <wx/bitmap.h>

/** \brief Class drawing additional data to help dragging */
class wxsItemEditorDragAssist
{
    public:

        /** \brief Ctor */
        wxsItemEditorDragAssist(wxsItemEditorContent* Content);

        /** \brief Dctor */
        ~wxsItemEditorDragAssist();

        /** \brief Function starting new dragging process */
        void NewDragging();

        /** \brief Function draging additional stuff */
        void DrawExtra(wxsItem* Target,wxsItem* Parent,bool AddAfter,wxDC* DC);

    private:

        wxsItem* PreviousTarget;
        wxsItem* PreviousParent;
        bool PreviousAddAfter;

        wxBitmap* TargetBitmap;
        wxRect    TargetRect;
        bool      IsTarget;

        wxBitmap* ParentBitmap;
        wxRect    ParentRect;
        bool      IsParent;

        wxsItemEditorContent* Content;

        void UpdateAssist(wxsItem* NewTarget,wxsItem* NewParent,bool NewAddAfter);
        void RebuildParentAssist();
        void RebuildTargetAssist();
        inline int AssistType();
        inline wxColour TargetColour();
        inline wxColour ParentColour();
        void ColourMix(wxImage& Image,const wxColour& Colour);
        void UpdateRect(wxRect& Rect,const wxBitmap& bmp);
};

#endif
