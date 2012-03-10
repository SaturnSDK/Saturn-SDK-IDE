/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef SCRIPTINGMANAGER_H
#define SCRIPTINGMANAGER_H

#include "scriptingmanager_base.h"

#ifndef CB_FOR_CONSOLE

#include <map>
#include <set>

#ifndef CB_PRECOMP
    #include "scriptingmanager_base.h"
    #include "cbexception.h" // cbThrow
    #include "globals.h" // cbC2U
#endif

#include "settings.h"
#include "manager.h"
#include "menuitemsmanager.h"
#include <wx/intl.h>

class SquirrelError;

/** @brief Provides scripting in Code::Blocks.
  *
  * The scripting engine used is Squirrel (http:://www.squirrel-lang.org).
  *
  * Here's an example to load and execute a script:
  *
  * @code
  * Manager::Get()->GetScriptingManager()->LoadScript(_T("some.script"));
  * @endcode
  *
  * And here's an example to call a script function:
  *
  * @code
  * // int return value
  * // C++ equivalent: int retValue = FunctionName("str_arg", 5, 1.0);
  * SqPlus::SquirrelFunction<int> myfunc("FunctionName");
  * int retValue = myfunc(_T("str_arg"), 5, 1.0);
  * // void return
  * // C++ equivalent: FunctionName("str_arg", 5, 1.0);
  * SqPlus::SquirrelFunction<void> myfunc("FunctionName");
  * myfunc(_T("str_arg"), 5, 1.0);
  * @endcode
  *
  * The templated type denotes the function's return type. Also note that the
  * function name is not unicode (we 're not using Squirrel in unicode mode).
  */
class DLLIMPORT ScriptingManager : public ScriptingManagerBase,  public Mgr<ScriptingManager>, wxEvtHandler
{
        friend class Mgr<ScriptingManager>;
        friend class ScriptingManagerBase;
    public:

        /** @brief Display error dialog.
          *
          * Displays an error dialog containing exception info and any other
          * script errors. Calls GetErrorString() internally.
          * You should normally call this function inside your catch handler for
          * SquirrelFunction<>() calls.
          * @param exception A pointer to the exception object containing the error. Can be NULL (default).
          * @param clearErrors If true (default), when this function returns all
          *        accumulated error messages are cleared.
          */
        virtual void DisplayErrors(SquirrelError* exception = 0, bool clearErrors = true);

        /** @brief Registers a script plugin menu IDs with the callback function.
          *
          * @param name The script plugin's name.
          * @param ids The menu IDs to bind.
          * @return True on success, false on failure.
          */
        virtual bool RegisterScriptPlugin(const wxString& name, const wxArrayInt& ids);

        /** @brief Script-bound function to register a script with a menu item.
          *
          * @param menuPath The full menu path. This can be separated by slashes (/)
          *                 to create submenus (e.g. "MyScripts/ASubMenu/MyItem").
          *                 If the last part of the string ("MyItem" in the example)
          *                 starts with a dash (-) (e.g. "-MyItem") then a menu
          *                 separator is prepended before the actual menu item.
          * @param scriptOrFunc The script's filename or a script's function name.
          * @param isFunction If true, the @c scriptOrFunc parameter is considered
          *       to be a script filename. If false, it is considered to be a
          *       script's function name.
          * @return True on success, false on failure.
          */
        virtual bool RegisterScriptMenu(const wxString& menuPath, const wxString& scriptOrFunc, bool isFunction);

        /** @brief Script-bound function to unregister a script's menu item.
          *
          * @param menuPath The full menu path to unregister.
          * @return True on success, false on failure.
          */
        virtual bool UnRegisterScriptMenu(const wxString& menuPath);

        /** @brief Unregister all scripts' menu items.
          *
          * @return True on success, false on failure.
          */
        virtual bool UnRegisterAllScriptMenus();

        // needed for SqPlus bindings
        ScriptingManager& operator=(const ScriptingManager& /*rhs*/) // prevent assignment operator
        {
        	cbThrow(_T("Can't assign a ScriptingManager* !!!"));
        	return *this;
		}
    private:
        // needed for SqPlus bindings
        ScriptingManager(const ScriptingManager& /*rhs*/); // prevent copy construction

        virtual void OnScriptMenu(wxCommandEvent& event);
        virtual void OnScriptPluginMenu(wxCommandEvent& event);

        ScriptingManager();
        virtual ~ScriptingManager(){};
        bool m_AttachedToMainWindow;
        MenuItemsManager m_MenuItemsManager;

        DECLARE_EVENT_TABLE()
};

#endif // #ifndef CB_FOR_CONSOLE
#endif // SCRIPTINGMANAGER_H
