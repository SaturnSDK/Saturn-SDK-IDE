/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef X_DEBUGGER_MANAGER_H
#define X_DEBUGGER_MANAGER_H

#include <map>

#ifndef CB_PRECOMP
#   include <manager.h>
#   include <settings.h>
#   include <wx/string.h>
#endif

class wxMenu;
class wxToolBar;
class cbBacktraceDlg;
class cbBreakpointsDlg;
class cbCPURegistersDlg;
class cbDebuggerPlugin;
class cbDisassemblyDlg;
class cbExamineMemoryDlg;
class cbThreadsDlg;
class DebuggerMenuHandler;
class DebuggerToolbarHandler;
class TextCtrlLogger;
class WatchesDlg;

class DLLIMPORT cbBreakpoint
{
    public:
        enum Type
        {
            Code,
            Data
        };

    public:
        cbBreakpoint();
        cbBreakpoint(const wxString &filename, int line);
        cbBreakpoint(const wxString &dataExpression, bool breakOnRead, bool breakOnWrite);

        void SetLine(int line);
        void SetCondition(wxString const &condition);
        void SetIgnoreCount(int count);
        void SetEnabled(bool flag);
        void SetUseIgnoreCount(bool flag);
        void SetUseCondition(bool flag);

        const wxString & GetFilename() const;
        const wxString & GetCondition() const;
        int GetLine() const;
        int GetIgnoreCount() const;
        Type GetType() const;
        bool IsEnabled() const;
        bool UseIgnoreCount() const;
        bool UseCondition() const;

        const wxString& GetDataExpression() const;
        bool GetBreakOnRead() const;
        bool GetBreakOnWrite() const;

    private:
        wxString m_filename;
        wxString m_condition;
        int m_line;
        int m_ignoreCount;
        Type m_type;
        bool m_enabled;
        bool m_useIgnoreCount;
        bool m_useCondition;
        wxString m_dataExpression;
        bool m_breakOnRead;
        bool m_breakOnWrite;
};

class DLLIMPORT cbWatch
{
        cbWatch& operator =(cbWatch &);
        cbWatch(cbWatch &);
	public:
        typedef std::vector<cbWatch*> PtrContainer;
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
        void RemoveChildren(bool only_marked = false);
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

        bool IsExpanded() const;
        void Expand(bool expand);

    private:
        cbWatch*        m_parent;
        PtrContainer    m_children;
        bool            m_changed;
        bool            m_removed;
        bool            m_expanded;
};

cbWatch* DLLIMPORT GetRootWatch(cbWatch *watch);

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

class DLLIMPORT DebuggerManager : public Mgr<DebuggerManager>
{
    private:
        DebuggerManager();
        ~DebuggerManager();

        friend class Mgr<DebuggerManager>;
        friend class Manager;
    public:
        struct PluginData
        {
            PluginData() :
                menu_id(-1)
            {
            }

            wxString name;
            int menu_id;
        };
        typedef std::map<cbDebuggerPlugin*, PluginData> RegisteredPlugins;

        enum SyncEditorResult
        {
            SyncOk = 0,
            SyncFileNotFound,
            SyncFileUnknown
        };
    public:
        bool RegisterDebugger(cbDebuggerPlugin *plugin, wxString const &name);
        bool UnregisterDebugger(cbDebuggerPlugin *plugin);

        wxMenu* GetMenu();
        void BuildContextMenu(wxMenu &menu, const wxString& word_at_caret, bool is_running);
        void LoadToolbar(wxToolBar *toolbar);

        TextCtrlLogger* GetLogger(bool for_debug, int &index);
        TextCtrlLogger* GetLogger(bool for_debug);
        void HideLogger(bool for_debug);

    public: // debugger windows
        cbBacktraceDlg* GetBacktraceDialog();
        cbBreakpointsDlg* GetBreakpointDialog();
        cbCPURegistersDlg* GetCPURegistersDialog();
        cbDisassemblyDlg* GetDisassemblyDialog();
        cbExamineMemoryDlg* GetExamineMemoryDialog();
        cbThreadsDlg* GetThreadsDialog();
        WatchesDlg* GetWatchesDialog();

    public: // tests if something should be done
        bool UpdateBacktrace();
        bool UpdateCPURegisters();
        bool UpdateDisassembly();
        bool UpdateExamineMemory();
        bool UpdateThreads();

    public:
        cbDebuggerPlugin* GetDebuggerHavingWatch(cbWatch *watch);

        RegisteredPlugins const & GetAllDebuggers() const;
        RegisteredPlugins & GetAllDebuggers();
        cbDebuggerPlugin* GetActiveDebugger();
        void SetActiveDebugger(cbDebuggerPlugin* activeDebugger);
        SyncEditorResult SyncEditor(const wxString& filename, int line, bool setMarker);
    private:
        bool RebuildActiveDebuggersMenu();
    private:

        RegisteredPlugins m_registered;
        cbDebuggerPlugin* m_activeDebugger;
        DebuggerMenuHandler* m_menuHandler;
        DebuggerToolbarHandler* m_toolbarHandler;

        cbBacktraceDlg* m_backtraceDialog;
        cbBreakpointsDlg* m_breakPointsDialog;
        cbCPURegistersDlg* m_cpuRegistersDialog;
        cbDisassemblyDlg* m_disassemblyDialog;
        cbExamineMemoryDlg* m_examineMemoryDialog;
        cbThreadsDlg* m_threadsDialog;
        WatchesDlg* m_watchesDialog;

        TextCtrlLogger* m_logger;
        TextCtrlLogger* m_debugLogger;
        int m_loggerIndex;
        int m_debugLoggerIndex;
};

#endif // X_DEBUGGER_MANAGER_H

