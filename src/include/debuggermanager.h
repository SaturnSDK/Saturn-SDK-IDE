/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef X_DEBUGGER_MANAGER_H
#define X_DEBUGGER_MANAGER_H

#include "sdk.h"

#ifndef CB_PRECOMP
#   include <map>
#   include <vector>

#   include <manager.h>
#   include <settings.h>
#   include <wx/string.h>

#   include "configmanager.h"
#endif

class wxMenu;
class wxToolBar;
class cbBacktraceDlg;
class cbBreakpointsDlg;
class cbCPURegistersDlg;
class cbDebuggerPlugin;
class cbDebugInterfaceFactory;
class cbDisassemblyDlg;
class cbExamineMemoryDlg;
class cbThreadsDlg;
class cbWatchesDlg;
class cbDebuggerMenuHandler;
class TextCtrlLogger;

class DLLIMPORT cbBreakpoint
{
    public:
        typedef cb::shared_ptr<cbBreakpoint> Pointer;
        typedef cb::shared_ptr<const cbBreakpoint> ConstPointer;

    public:
        virtual ~cbBreakpoint() {}

        virtual void SetEnabled(bool flag) = 0;
        virtual wxString GetLocation() const = 0;
        virtual int GetLine() const = 0;
        virtual wxString GetLineString() const = 0;
        virtual wxString GetType() const = 0;
        virtual wxString GetInfo() const = 0;
        virtual bool IsEnabled() const = 0;
        virtual bool IsVisibleInEditor() const = 0;
        virtual bool IsTemporary() const = 0;
};

class DLLIMPORT cbWatch
{
        cbWatch& operator =(cbWatch &);
        cbWatch(cbWatch &);

        typedef std::vector<cbWatch*> PtrContainer;
    public:
        typedef cb::shared_ptr<cbWatch> Pointer;
    public:
        cbWatch();
        void Destroy();
    public:
        virtual void GetSymbol(wxString &symbol) const = 0;
        virtual void GetValue(wxString &value) const = 0;
        virtual bool SetValue(const wxString &value) = 0;
        virtual void GetFullWatchString(wxString &full_watch) const = 0;
        virtual void GetType(wxString &type) const = 0;
        virtual void SetType(const wxString &type) = 0;

        virtual wxString const & GetDebugString() const = 0;
	protected:
        virtual ~cbWatch();
        virtual void DoDestroy() = 0;
	public:
        void AddChild(cbWatch *watch);
        void RemoveChild(int index);
        void RemoveChildren();
        bool RemoveMarkedChildren();
        int GetChildCount() const;
        cbWatch* GetChild(int index);
        const cbWatch* GetChild(int index) const;

        cbWatch* FindChild(const wxString& name);
        int FindChildIndex(const wxString& symbol) const;

        void SetParent(cbWatch *parent);
        const cbWatch* GetParent() const;
        cbWatch* GetParent();

        bool IsRemoved() const;
        bool IsChanged() const;

        void MarkAsRemoved(bool flag);
        void MarkChildsAsRemoved();
        void MarkAsChanged(bool flag);
        void MarkAsChangedRecursive(bool flag);

        bool IsExpanded() const;
        void Expand(bool expand);

    private:
        cbWatch*        m_parent;
        PtrContainer    m_children;
        bool            m_changed;
        bool            m_removed;
        bool            m_expanded;
};

cbWatch* DLLIMPORT cbGetRootWatch(cbWatch *watch);

class DLLIMPORT cbStackFrame
{
    public:
        cbStackFrame();

        void SetNumber(int number);
        void SetAddress(unsigned long int address);
        void SetSymbol(const wxString& symbol);
        void SetFile(const wxString& filename, const wxString &line);
        void MakeValid(bool flag);

        int GetNumber() const;
        unsigned long int GetAddress() const;
        const wxString& GetSymbol() const;
        const wxString& GetFilename() const;
        const wxString& GetLine() const;
        bool IsValid() const;
    private:
        bool m_valid; ///< Is this stack frame valid?
        int m_number; ///< Stack frame's number (used in backtraces).
        unsigned long int m_address; ///< Stack frame's address.
        wxString m_symbol; ///< Current function name.
        wxString m_file; ///< Current file.
        wxString m_line; ///< Current line in file.
};

class DLLIMPORT cbThread
{
    public:
        cbThread();
        cbThread(bool active, int number, const wxString& info);

        bool IsActive() const;
        int GetNumber() const;
        const wxString& GetInfo() const;

    private:
        bool m_active;
        int m_number;
        wxString m_info;
};

/**
  *
  */
class DLLIMPORT cbDebuggerConfiguration
{
    protected:
        cbDebuggerConfiguration(const cbDebuggerConfiguration &o);
        cbDebuggerConfiguration& operator =(const cbDebuggerConfiguration &);

    public:
        cbDebuggerConfiguration(const ConfigManagerWrapper &config);
        virtual ~cbDebuggerConfiguration() {}

        virtual cbDebuggerConfiguration* Clone() const = 0;

        virtual wxPanel* MakePanel(wxWindow *parent) = 0;
        virtual bool SaveChanges(wxPanel *panel) = 0;

        void SetName(const wxString &name);
        const wxString& GetName() const;

        const ConfigManagerWrapper& GetConfig() const;
        void SetConfig(const ConfigManagerWrapper &config);

        void SetMenuId(long id);
        long GetMenuId() const;

    protected:
        ConfigManagerWrapper m_config;
    private:
        wxString m_name;
        long m_menuId;
};

/**
  *
  */
struct DLLIMPORT cbDebuggerCommonConfig
{
    enum Flags
    {
        AutoBuild = 0,
        AutoSwitchFrame,
        ShowDebuggersLog,
        JumpOnDoubleClick,
        RequireCtrlForTooltips,
        ShowTemporaryBreakpoints
    };

    static bool GetFlag(Flags flag);
    static void SetFlag(Flags flag, bool value);

    static wxString GetValueTooltipFont();
    static void SetValueTooltipFont(const wxString &font);
};

/**
  * Tries to detect the path to the debugger's executable.
  */
DLLIMPORT wxString cbDetectDebuggerExecutable(const wxString &exeName);

class DLLIMPORT DebuggerManager : public Mgr<DebuggerManager>
{
    private:
        DebuggerManager();
        ~DebuggerManager();

        friend class Mgr<DebuggerManager>;
        friend class Manager;
    public:
        typedef std::vector<cbDebuggerConfiguration*> ConfigurationVector;
        struct PluginData
        {
            friend class DebuggerManager;

            PluginData() :  m_lastConfigID(-1) {}

            const wxString &GetGUIName() const { return m_guiName; }
            const wxString &GetSettingsName() const { return m_settingsName; }

            ConfigurationVector& GetConfigurations() { return m_configurations; }
            const ConfigurationVector& GetConfigurations() const { return m_configurations; }

            cbDebuggerConfiguration* GetConfiguration(int index);

            void ClearConfigurations()
            {
                for (ConfigurationVector::iterator it = m_configurations.begin(); it != m_configurations.end(); ++it)
                    delete *it;
                m_configurations.clear();
            }
        private:
            ConfigurationVector m_configurations;
            wxString m_guiName;
            wxString m_settingsName;
            int m_lastConfigID;
        };
        typedef std::map<cbDebuggerPlugin*, PluginData> RegisteredPlugins;

    public:
        bool RegisterDebugger(cbDebuggerPlugin *plugin, const wxString &guiName, const wxString &settingsName);
        bool UnregisterDebugger(cbDebuggerPlugin *plugin);

        ConfigManagerWrapper NewConfig(cbDebuggerPlugin *plugin, const wxString &name);
        void RebuildAllConfigs();

        wxMenu* GetMenu();
        void BuildContextMenu(wxMenu &menu, const wxString& word_at_caret, bool is_running);

        TextCtrlLogger* GetLogger(bool for_debug, int &index);
        TextCtrlLogger* GetLogger(bool for_debug);
        void HideLogger(bool for_debug);

    public: // debugger windows
        void SetInterfaceFactory(cbDebugInterfaceFactory *factory);
        cbDebugInterfaceFactory* GetInterfaceFactory();
        void SetMenuHandler(cbDebuggerMenuHandler *handler);

        cbBacktraceDlg* GetBacktraceDialog();
        cbBreakpointsDlg* GetBreakpointDialog();
        cbCPURegistersDlg* GetCPURegistersDialog();
        cbDisassemblyDlg* GetDisassemblyDialog();
        cbExamineMemoryDlg* GetExamineMemoryDialog();
        cbThreadsDlg* GetThreadsDialog();
        cbWatchesDlg* GetWatchesDialog();

        bool ShowBacktraceDialog();

    public: // tests if something should be done
        bool UpdateBacktrace();
        bool UpdateCPURegisters();
        bool UpdateDisassembly();
        bool UpdateExamineMemory();
        bool UpdateThreads();

    public: // watches
        cbDebuggerPlugin* GetDebuggerHavingWatch(cbWatch *watch);
        bool ShowValueTooltip(const cbWatch::Pointer &watch, const wxRect &rect);

        RegisteredPlugins const & GetAllDebuggers() const;
        RegisteredPlugins & GetAllDebuggers();
        cbDebuggerPlugin* GetActiveDebugger();
        void SetActiveDebugger(cbDebuggerPlugin* activeDebugger, ConfigurationVector::iterator config);
        void SetTargetsDefaultAsActiveDebugger();
        bool IsActiveDebuggerTargetsDefault() const;

        bool IsDisassemblyMixedMode();
        void SetDisassemblyMixedMode(bool mixed);

    private:
        void ProcessSettings(RegisteredPlugins::iterator it);
        void FindTargetsDebugger();

        void OnProjectActivated(CodeBlocksEvent& event);
        void OnTargetSelected(CodeBlocksEvent& event);
        void OnSettingsChanged(CodeBlocksEvent& event);
        void OnPluginLoadingComplete(CodeBlocksEvent& event);
    private:

        cbDebugInterfaceFactory *m_interfaceFactory;

        RegisteredPlugins m_registered;
        cbDebuggerPlugin* m_activeDebugger;
        cbDebuggerMenuHandler* m_menuHandler;

        cbBacktraceDlg* m_backtraceDialog;
        cbBreakpointsDlg* m_breakPointsDialog;
        cbCPURegistersDlg* m_cpuRegistersDialog;
        cbDisassemblyDlg* m_disassemblyDialog;
        cbExamineMemoryDlg* m_examineMemoryDialog;
        cbThreadsDlg* m_threadsDialog;
        cbWatchesDlg* m_watchesDialog;

        TextCtrlLogger* m_logger;
        TextCtrlLogger* m_debugLogger;
        int m_loggerIndex;
        int m_debugLoggerIndex;
        bool m_isDisassemblyMixedMode;
        bool m_useTargetsDefault;
};

#endif // X_DEBUGGER_MANAGER_H

