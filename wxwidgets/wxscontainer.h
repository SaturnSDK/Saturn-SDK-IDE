#ifndef WXSCONTAINER_H
#define WXSCONTAINER_H

#include "wxsparent.h"
#include "wxsstyle.h"
#include "wxsbaseproperties.h"

/** \brief Container is a class which represents widget that can
 *         have child items or one of root items
 */
class wxsContainer: public wxsParent
{
    public:

        /** \brief Ctor
         *  \param Data data managment object handling this item
         *  \param PropertiesFlags flags filtering sed base properties
         *         (see wxsBaseProperties for details)
         *  \param Info pointer to static widget info
         *  \param EventArray pointer to static set of events
         *  \param StyleSet set of used styles, if NULL, this widget won't
         *         provide styles by default
         *  \param DefaultStyle default style used on read errors, string can
         *         contain one or more style names separated with '|' character
         */
        wxsContainer(
            wxsItemResData* Data,
            const wxsItemInfo* Info,
            long PropertiesFlags,
            const wxsEventDesc* EventArray = NULL,
            const wxsStyle* StyleSet=NULL,
            const wxString& DefaultStyle=wxEmptyString);

    protected:

        /** \brief Function enumerating properties for this container only
         *
         * This function should enumerate all extra properties
         * required by item (extra means not enumerated in base properties,
         * not id or variable name).
         * These properties will be placed at the beginning, right after
         * there will be Variable name and identifier and at the end, all
         * required base properties.
         */
        virtual void OnEnumContainerProperties(long Flags) = 0;

        /** \brief Function which adds new items to QPP
         *
         * This function may be used to add special quick properties for
         * this item.
         *
         * All QPPChild panels will be added before additional panels
         * added by widget.
         */
        virtual void OnAddContainerQPP(wxsAdvQPP* QPP) { }

        /** \brief Checking if can add child item
         *
         * This function is by fedault implemented inside wxsContainer.
         * In case of some limitations made for children (like inside
         * wxSplitterWindow, this functino should be overridden
         * to avoid invalidating item).
         *
         * Default implementation matches few rules:
         *  - spacer item can be added into sizers only
         *  - sizer can be added into other sizers or into empty container
         *    (cannot put sizer when there's other one inside container
         *     or when there's any item added)
         *  - if non-sizer container already has sizer, nothing else can be
         *    added into it
         */
        virtual bool OnCanAddChild(wxsItem* Item,bool ShowMessage);

        /** \brief Easy access to position */
        inline wxPoint Pos(wxWindow* Parent)
        {
            return GetBaseProps()->m_Position.GetPosition(Parent);
        }

        /** \brief Easy access to size */
        inline wxSize Size(wxWindow* Parent)
        {
            return GetBaseProps()->m_Size.GetSize(Parent);
        }

        /** \brief Easy access to style (can be used directly in wxWidgets */
        inline long Style()
        {
            return wxsStyleProperty::GetWxStyle(m_StyleBits,m_StyleSet,false);
        }

        /** \brief Function setting up standard widget properties after
         *         the window is created.
         *
         * Included properties:
         *  - Enabled
         *  - Focused
         *  - Hidden (skipped when not exact preview)
         *  - FG - Foreground colour
         *  - BG - Background colour
         *  - Font
         *  - ToolTip
         *  - HelpText
         *  - Extra style
         *
         * \param Preview window for which properties must be applied
         * \param IsExact value of IsExact argument passed to BuildPreview
         * \return window after settig up properties (value of Preview is returned)
         */
        wxWindow* SetupWindow(wxWindow* Preview,bool IsExact);

        /** \brief Easy acces to position code */
        inline wxString PosCode(const wxString& Parent,wxsCodingLang Language)
        {
            return GetBaseProps()->m_Position.GetPositionCode(Parent,Language);
        }

        /** \brief Easy acces to size code */
        inline wxString SizeCode(const wxString& Parent,wxsCodingLang Language)
        {
            return GetBaseProps()->m_Size.GetSizeCode(Parent,Language);
        }

        /** \brief Easy access to style code */
        inline wxString StyleCode(wxsCodingLang Language)
        {
            return wxsStyleProperty::GetString(m_StyleBits,m_StyleSet,false,Language);
        }

        /** \brief Function adding code setting up properties after window
         *         is created.
         *
         * Behaviour of this function is simillar to SetupWindow but this
         * one creates source code doing it instead of setting up real window
         */
        void SetupWindowCode(wxString& Code,wxsCodingLang Language);

        /** \brief Function adding children items into preview window */
        void AddChildrenPreview(wxWindow* This,bool Exact,bool StoreInLastPreview);

        /** \brief Function adding code generating child items */
        void AddChildrenCode(wxString& Code,wxsCodingLang Language);

    private:

        /** \brief Function enumerating properties with default ones
         *
         * Function enumerating item properties. The implementation
         * does call EnumContainerProperties() and adds all default properties.
         */
        virtual void OnEnumItemProperties(long Flags);

        /** \brief Function Adding QPPChild panels for base properties of this
         *         container.
         *
         * This function calls internally AddContainerQPP to add any additional
         * QPPChild panels.
         */
        virtual void OnAddItemQPP(wxsAdvQPP* QPP);

        const wxsStyle* m_StyleSet;
        wxString m_DefaultStyle;
        long m_StyleBits;
        long m_ExStyleBits;
};

#endif
