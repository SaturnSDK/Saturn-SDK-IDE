#ifndef WXSBASEPROPERTIES_H
#define WXSBASEPROPERTIES_H

#include "../properties/wxsproperties.h"
#include "../wxsadvqpp.h"
#include "../wxscodinglang.h"

// TODO: Add wxWidgets-specific properties when they;re ready
/** \brief Structure holding most commonly used properties
 *
 * This class doesn't support style since it require too many informations
 * from base class.
 */
class wxsBaseProperties: public wxsPropertyContainer
{
    public:

        // Availability flags used in this container
        static const long flPosition        = 0x000100;  ///< \brief Item is using position
        static const long flSize            = 0x000200;  ///< \brief Item is using size
        static const long flEnabled         = 0x000400;  ///< \brief Item is using Enabled property
        static const long flFocused         = 0x000800;  ///< \brief Item is using Focused property
        static const long flHidden          = 0x001000;  ///< \brief Item is using Hidden property
        static const long flColours         = 0x002000;  ///< \brief Item is using colour properties (Fg and Bg)
        static const long flToolTip         = 0x004000;  ///< \brief Item is using tooltips
        static const long flFont            = 0x008000;  ///< \brief Item is using font
        static const long flHelpText        = 0x010000;  ///< \brief Item is using help text
        static const long flContainer       = flPosition | flSize | flColours | flToolTip | flHelpText;   ///< \brief Properties used by common containers
        static const long flAll             = 0x01FF00;  ///< \brief Using all base properties

//        wxsPositionData m_Position;       ///< \brief Position
//        wxsSizeData m_Size;               ///< \brief Size
        bool m_Enabled;                   ///< \brief If false, widget is disabled (true by deefault)
        bool m_Focused;                   ///< \brief If true, widget is focused (false by default)
        bool m_Hidden;                    ///< \brief If true, widget is hidden (false by default)
//        wxColourPropertyValue m_Fg;       ///< \brief Foreground colour when using custom colour
//        wxColourPropertyValue m_Bg;       ///< \brief Background colour when using custom colour
//        wxsFontData m_Font;               ///< \brief Font
        wxString m_ToolTip;               ///< \brief Tooltip
        wxString m_HelpText;              ///< \brief Help text

        /** \brief Constructor - it will be used to set default values */
        wxsBaseProperties():
            m_Enabled(true),
            m_Focused(false),
            m_Hidden(false),
//            m_Fg(wxsCOLOUR_DEFAULT),
//            m_Bg(wxsCOLOUR_DEFAULT),
            m_ToolTip(wxEmptyString),
            m_HelpText(wxEmptyString)
        {}

        /** \brief Function setting up created window using some of properties
         *
         * Properties set up are:
         *  - Enabled
         *  - Focused
         *  - Hidden (skipped when not exact preview)
         *  - FG - Foreground colour
         *  - BG - Background colour
         *  - Font
         *  - ToolTip
         *  - HelpText
         */
        void SetupWindow(wxWindow* Window,bool IsExact);

        /** \brief Function generating code setting up some properties on given window
         *
         * Created code will do the same as SetupWindow function
         */
        void BuildSetupWindowCode(wxString& Code,const wxString& WindowName,wxsCodingLang Language);

        /** \brief Function generating QPPChild Panel for most frequently used
         *         properties.
         */
        void AddQPPChild(wxsAdvQPP* QPP,long Flags);

    protected:

        virtual void EnumProperties(long Flags);
};

#endif
