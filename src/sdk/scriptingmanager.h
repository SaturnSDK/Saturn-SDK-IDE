#ifndef SCRIPTING_H
#define SCRIPTING_H

#include "settings.h"
#include "manager.h"
#include <wx/intl.h>

/** All about scripting in Code::Blocks.
  *
  * The scripting engine used here is Squirrel (http:://www.squirrel-lang.org).
  *
  * Example to load and execute a script follows:
  * <code>
  * Manager::Get()->GetScriptingManager()->LoadScript(_T("some.script"));
  * </code>
  */
class DLLIMPORT ScriptingManager : public Mgr<ScriptingManager>
{
        friend class Mgr<ScriptingManager>;
    public:
        ~ScriptingManager();

        /** Loads a script and runs the function "main()" (if needed/exists).
         * @param script The script to run.
         * @param autorunMain If true, runs the "main()" function (if it exists).
         * @return True if the script loaded and compiled, false if not.
         */
        bool LoadScript(const wxString& filename, bool autorunMain = true);
	protected:
	private:
        ScriptingManager();
};

#endif // SCRIPTING_H
