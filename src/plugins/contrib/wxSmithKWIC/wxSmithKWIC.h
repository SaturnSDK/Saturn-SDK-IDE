/**************************************************************************//**
 * \file		wxSmithKWIC.h
 * \author	Gary Harris
 * \date		19/4/2010.
 *
 * wxSmithKWIC - an add-on for wxSmith, Code::Blocks' GUI editor.					\n
 * Copyright (C) 2010 Gary Harris.
 *
 * wxSmithKWIC is free software: you can redistribute it and/or modify
 * it under the terms of the KWIC License
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * KWIC License for more details.
 *
 * You should have received a copy of the KWIC License along with this
 * program.  If not, see <http://www.koansoftware.com/kwic/kwic-license.htm>.
 *
 *****************************************************************************/
#ifndef WXSMITHKWIC_H_INCLUDED
#define WXSMITHKWIC_H_INCLUDED

// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cbplugin.h> // for "class cbPlugin"

class wxSmithKWIC : public cbPlugin
{
    public:
        /** Constructor. */
        wxSmithKWIC();
        /** Destructor. */
        virtual ~wxSmithKWIC();


        /** This method is called by Code::Blocks and is used by the plugin
          * to add any menu items it needs on Code::Blocks's menu bar.\n
          * It is a pure virtual method that needs to be implemented by all
          * plugins. If the plugin does not need to add items on the menu,
          * just do nothing ;)
          * @param menuBar the wxMenuBar to create items in
          */
        virtual void BuildMenu(wxMenuBar* menuBar){}

        /** This method is called by Code::Blocks core modules (EditorManager,
          * ProjectManager etc) and is used by the plugin to add any menu
          * items it needs in the module's popup menu. For example, when
          * the user right-clicks on a project file in the project tree,
          * ProjectManager prepares a popup menu to display with context
          * sensitive options for that file. Before it displays this popup
          * menu, it asks all attached plugins (by asking PluginManager to call
          * this method), if they need to add any entries
          * in that menu. This method is called.\n
          * If the plugin does not need to add items in the menu,
          * just do nothing ;)
          * @param type the module that's preparing a popup menu
          * @param menu pointer to the popup menu
          * @param data pointer to FileTreeData object (to access/modify the file tree)
          */
        virtual void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0){}

        /** This method is called by Code::Blocks and is used by the plugin
          * to add any toolbar items it needs on Code::Blocks's toolbar.\n
          * It is a pure virtual method that needs to be implemented by all
          * plugins. If the plugin does not need to add items on the toolbar,
          * just do nothing ;)
          * @param toolBar the wxToolBar to create items on
          * @return The plugin should return true if it needed the toolbar, false if not
          */
        virtual bool BuildToolBar(wxToolBar* toolBar){ return false; }
    protected:
        /** Any descendent plugin should override this virtual method and
          * perform any necessary initialization. This method is called by
          * Code::Blocks (PluginManager actually) when the plugin has been
          * loaded and should attach in Code::Blocks. When Code::Blocks
          * starts up, it finds and <em>loads</em> all plugins but <em>does
          * not</em> activate (attaches) them. It then activates all plugins
          * that the user has selected to be activated on start-up.\n
          * This means that a plugin might be loaded but <b>not</b> activated...\n
          * Think of this method as the actual constructor...
          */
        virtual void OnAttach();

        /** Any descendent plugin should override this virtual method and
          * perform any necessary de-initialization. This method is called by
          * Code::Blocks (PluginManager actually) when the plugin has been
          * loaded, attached and should de-attach from Code::Blocks.\n
          * Think of this method as the actual destructor...
          * @param appShutDown If true, the application is shutting down. In this
          *         case *don't* use Manager::Get()->Get...() functions or the
          *         behaviour is undefined...
          */
        virtual void OnRelease(bool appShutDown);

};

//##########################################################################
//									Main page for doxygen docs.
//##########################################################################

/*! \mainpage wxSmithKWIC
 *
 *
 * \author 	Gary Harris.
 * \date    	18 September, 2010
 *
 * \section intro Introduction
 * wxSmithKWIC is an add-on for wxSmith, Code::Blocks' GUI editor plug-in. It adds the Koan wxIndustrialControls to wxSmith's control palette.
 * wxSmithKWIC is based in part on the work of the KWIC project (http://www.koansoftware.com/kwic/index.htm).
 *
 * You can download wxSmithKWIC from http://wxsmithaddons.sourceforge.net/wxsmithkwic.html
 *
 * \section comments Comments & Feedback
 * Your input is welcomed. Please direct all comments, suggestions and feedback to the Code::Blocks forum at: http://forums.codeblocks.org/index.php/.
 *
 * \section changes Changes
 *	\li Added: Improved localisation.
 *
 * \section history History
 * Version 0.9.847 - 19 April, 2010.
 *	\li Added: Credit statement to manifest.xml.
 *	\li Added: Linux project and makefiles.
 *	\li Fixed: Various project settings.
 *	\li Fixed: Crash on Linux due to ProcessCommand() being MSW only.
 *
 * Version 0.9.841 - 19 April, 2010.
 *	\li Fixed another crash caused by an uninitialised variable in kwxBmpSwitcher.
 *
 * Version 0.7.660 - 19 April, 2010.
 * \li Updated: For C::B 10.5 and wxPropertyGrid 1.4.
 * \li Not released due to an oversight on my part.
 *
 * Version 1.4.440 - 1 April, 2010.
 * \li First release.
 *
 */

#endif // WXSMITHKWIC_H_INCLUDED
