/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */
#ifndef X_COLOURMANAGER_H
#define X_COLOURMANAGER_H

#include <map>
#include <wx/colour.h>
#include <wx/string.h>

#include "manager.h"

class DLLIMPORT ColourManager : public Mgr<ColourManager>
{
        friend class Mgr<ColourManager>;
        friend class Manager;
    public:
        void Load();
        void Save();
        void RegisterColour(const wxString &category, const wxString &name,
                            const wxString &id, const wxColour &defaultColour);
        wxColour GetColour(const wxString &id) const;
        void SetColour(const wxString &id, const wxColour &colour);
    public:
        struct ColourDef
        {
            wxString category, name;
            wxColour value, defaultValue;

            bool IsValid() const
            {
                return !name.empty() && !category.empty();
            }
        };
        typedef std::map<wxString, ColourDef> ColourDefMap;

        const ColourDefMap& GetColourDefinitions() const;
    private:
        ColourDefMap m_colours;
};

#endif // X_COLOURMANAGER_H


