#ifndef PROJECTLOADER_HOOKS_H
#define PROJECTLOADER_HOOKS_H

#include "settings.h"

class TiXmlElement;

/** Provides static functions to add hooks to the project loading/saving procedure. */
namespace ProjectLoaderHooks
{
    /** Abstract base hook functor interface. */
    class DLLIMPORT HookFunctorBase
    {
        public:
            virtual void Call(TiXmlElement* elem, bool isLoading) const = 0;
    };

    /** Functor class for use as a project loading/saving hook.
      * Passed as the first parameter in RegisterHook() and
      * UnregisterHook().
      *
      * example:
      * ProjectLoaderHooks::HookFunctorBase* myhook = new ProjectLoaderHooks::HookFunctor<MyClass>(this, &MyClass::OnHookCalled);
      * int id = ProjectLoaderHooks::RegisterHook(myhook);
      * ...
      * (and before your class' destruction - or earlier):
      * ProjectLoaderHooks::UnregisterHook(id, true);
      *
      * Member functions used as hook callbacks must have the following signature:
      * void YourFunctionName(TiXmlElement*, bool)
      *
      * Use normal TinyXML procedures to work with the TiXmlElement* argument.
      * The isLoading argument is true if your hook is called when the project is being loaded,
      * and false when the project is saved.
      */
    template<class T> class DLLIMPORT HookFunctor : public HookFunctorBase
    {
        public:
            typedef void (T::*Func)(TiXmlElement*, bool);
            HookFunctor(T* obj, Func func)
                : m_pObj(obj),
                m_pFunc(func)
            {}
            virtual void Call(TiXmlElement* elem, bool isLoading) const
            {
                if (m_pObj && m_pFunc)
                    (m_pObj->*m_pFunc)(elem, isLoading);
            }
        protected:
            T* m_pObj;
            Func m_pFunc;
    };

    /** Register a project loading/saving hook.
      * @param functor The functor to use as a callback.
      * @return An ID. Use this to unregister your hook later.
      */
    extern DLLIMPORT int RegisterHook(HookFunctorBase* functor);
    /** Unregister a previously registered project loading/saving hook.
      * @param id The hook's ID. You should have the ID from when RegisterHook() was called.
      * @param deleteHook. If true, the hook will be deleted (default). If not, it's
      * up to you to delete it.
      * @return The functor. If @c deleteHook was true, it always returns NULL.
      */
    extern DLLIMPORT HookFunctorBase* UnregisterHook(int id, bool deleteHook = true);
    /** Are there any hooks registered?
      * @return True if any hooks are registered, false if none.
      */
    extern DLLIMPORT bool HasRegisteredHooks();
    /** Call all registered hooks using the supplied parameters.
      * This is called by ProjectLoader.
      */
    extern DLLIMPORT void CallHooks(TiXmlElement* elem, bool isLoading);
};

#endif // PROJECTLOADER_HOOKS_H
