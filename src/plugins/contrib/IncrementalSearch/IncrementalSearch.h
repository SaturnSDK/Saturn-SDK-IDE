/***************************************************************
 * Name:      IncrementalSearch
 * Purpose:   Code::Blocks plugin
 * Author:    Jens Lody (debian@jenslody.de)
 * Created:   2008-10-01
 * Copyright: Jens Lody
 * License:   GPL
 **************************************************************/

#ifndef INCREMENTAL_SEARCH_H_INCLUDED
#define INCREMENTAL_SEARCH_H_INCLUDED

#include <wx/colour.h>
#include <wx/string.h>
#include "cbplugin.h" // for "class cbPlugin"

class cbStyledTextCtrl;

class IncrementalSearch : public cbPlugin
{
    public:
        /** Constructor. */
        IncrementalSearch();
        /** Destructor. */
        virtual ~IncrementalSearch();

        /** Return the plugin's configuration priority.
          * This is a number (default is 50) that is used to sort plugins
          * in configuration dialogs. Lower numbers mean the plugin's
          * configuration is put higher in the list.
          */
        virtual int GetConfigurationPriority() const { return 50; }

        /** Return the configuration group for this plugin. Default is cgUnknown.
          * Notice that you can logically OR more than one configuration groups,
          * so you could set it, for example, as "cgCompiler | cgContribPlugin".
          */
        virtual int GetConfigurationGroup() const { return cgEditor; }

        /** Return plugin's configuration panel.
          * @param parent The parent window.
          * @return A pointer to the plugin's cbConfigurationPanel. It is deleted by the caller.
          */
        virtual cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);

        /** Return plugin's configuration panel for projects.
          * The panel returned from this function will be added in the project's
          * configuration dialog.
          * @param parent The parent window.
          * @param project The project that is being edited.
          * @return A pointer to the plugin's cbConfigurationPanel. It is deleted by the caller.
          */
        virtual cbConfigurationPanel* GetProjectConfigurationPanel(wxWindow* parent, cbProject* project){ return 0; }

        /** This method is called by Code::Blocks and is used by the plugin
          * to add any menu items it needs on Code::Blocks's menu bar.\n
          * It is a pure virtual method that needs to be implemented by all
          * plugins. If the plugin does not need to add items on the menu,
          * just do nothing ;)
          * @param menuBar the wxMenuBar to create items in
          */
        virtual void BuildMenu(wxMenuBar* menuBar);

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
        virtual bool BuildToolBar(wxToolBar* toolBar);
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

    private:
        void HighlightText();
        void OnFocusToolbar(wxCommandEvent& event);
        void OnTextChanged(wxCommandEvent& event);
        void OnSearchPrev(wxCommandEvent& event);
        void OnSearchNext(wxCommandEvent& event);
        void OnToggleToolbar(wxCommandEvent& event);
        void OnEditorEvent(CodeBlocksEvent& event);
        void OnToggleHighlight(wxCommandEvent& event);
        void OnToggleSelectedOnly(wxCommandEvent& event);
        void OnToggleMatchCase(wxCommandEvent& event);
        void SearchText();
        void SearchForward(int fromPos);
        void SearchBackward(int fromPos);
        void VerifyPosition();
        void VerifyRange();
		wxString m_SearchText;
		wxColour m_textCtrlBG_Default;
        wxToolBar* m_pToolbar;
        wxTextCtrl* m_pTextCtrl;
		cbStyledTextCtrl* m_pControl;
		int m_NewPos;
		int m_OldPos;
		int m_SelStart;
		int m_SelEnd;
		int m_MinPos;
		int m_MaxPos;
		bool m_Highlight;
		bool m_SelectedOnly;
		int m_flags;

        DECLARE_EVENT_TABLE();
};

#endif // INCREMENTAL_SEARCH_H_INCLUDED
