#ifndef WXSITEMFACTORY_H
#define WXSITEMFACTORY_H

#include <wx/string.h>
#include <wx/hashmap.h>

class wxsItem;
class wxsItemInfo;
class wxsItemRes;

/** \brief Class managing creation, destruction and enumeration of supported
 *         items
 *
 * All public functinos are static which means that this class is something
 * like singleton but using static members for global operations.
 * Each instance of this class does represent one wxsItem - provides it's info
 * and is able to build this item.
 * Addind new supported item to factory is done by deriving other class from
 * this one and writing it's
 */
class wxsItemFactory
{
	public:

        /** \brief Creating item with given name */
        static wxsItem* Build(const wxString& ClassName,wxsItemRes* Res);

        /** \brief Returning info for item with given name
         *  \param ClassName name of item's class
         *  \return Pointer to info or NULL if there's no such item
         */
        static const wxsItemInfo* GetInfo(const wxString& ClassName);

        /** \brief Getting onfo of first item */
        static const wxsItemInfo* GetFirstInfo();

        /** \brief Continuing getting item infos */
        static const wxsItemInfo* GetNextInfo();

    protected:

        /** \brief Ctor - can be accessed from derived classes only */
		wxsItemFactory(const wxsItemInfo* Info);

		/** \brief Dctor */
		virtual ~wxsItemFactory();

        /** \brief Building item */
        virtual wxsItem* OnBuild(wxsItemRes* Resource) = 0;

	private:

        WX_DECLARE_STRING_HASH_MAP(wxsItemFactory*,ItemMapT);

        /** \brief Function for getting global item's map */
        static ItemMapT& ItemMap();

        const wxsItemInfo* m_Info;          ///< \brief Info of item handled by this instance
        static ItemMapT::iterator m_Iter;   ///< \brief Iterator used for GetFirstInfo / GetNextInfo
};

#endif
