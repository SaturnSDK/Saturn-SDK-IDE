/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef USER_VARIABLE_MANAGER_H
#define USER_VARIABLE_MANAGER_H

#include "settings.h"
#include "manager.h"
#include "cbexception.h"

#ifndef CB_PRECOMP
    #include "globals.h"
#endif

class DLLIMPORT UserVariableManager : public Mgr<UserVariableManager>
{
        friend class Manager;
        friend class Mgr<UserVariableManager>;
        friend class MacrosManager;
        ConfigManager * cfg;
        wxString activeSet;
        wxArrayString preempted;

    public:
        UserVariableManager();

        wxString Replace(const wxString& variable);

        void Preempt(const wxString& variable);
        void Arrogate();
        bool Exists(const wxString& variable) const;

        void Configure();
        void Migrate();

        UserVariableManager& operator=(const UserVariableManager& /*rhs*/) // prevent assignment operator
        {
        	cbThrow(_T("Can't assign a UserVariableManager* !!!"));
        	return *this;
		}
    private:
        UserVariableManager(const UserVariableManager& /*rhs*/); // prevent copy construction
};

#endif // USER_VARIABLE_MANAGER_H
