#pragma once
#ifndef SSS_EVENT_LIST_HPP
#define SSS_EVENT_LIST_HPP

#include "_includes.hpp"
#include "log.hpp"

#pragma warning(push, 2)
#pragma warning(disable: 4251)
/** @file
 *  Defines time-related functions and classes.
 */

// Register the event to the global Event List
#define REGISTER_EVENT(Str) SSS::EventManager::registerEvent(Str)
// Emit and notify the event to all the concerned observers
#define EMIT_EVENT(Str)     _notifyObservers(SSS::EventManager::eventID(Str))
// Get the ID of the Event from its name
#define EVENT_ID(Str)       SSS::EventManager::eventID(Str)

/*          Event Utils             */ 
// Get the complete registered event list
#define EVENT_LIST          SSS::EventManager::eventList()
// Return the name of the event from its ID
// Warning : This function should only be used as log support
#define EVENT_NAME(Id)      SSS::EventManager::findName(Id)

SSS_BEGIN;

template <typename Derived>
class _EventRegistry {
protected:
    _EventRegistry() {  std::call_once(init_flag, &_EventRegistry::init);   }

private:
    static std::once_flag init_flag;

    static void init() {
        log_msg("Event registration for class <" + SSS::toString(typeid(Derived).name()) + ">");
        Derived::_register();
    }
};

// One once_flag per Derived type
template <typename Derived>
std::once_flag _EventRegistry<Derived>::init_flag;


class SSS_COMMONS_API EventManager {
protected:
    /** \cond INTERNAL*/
    // Hide constructor & destructor
    EventManager() = default;
    ~EventManager() = default;
    /** \endcond*/
public:
    /** Returns the singleton instance (lazy instantiation pattern).*/
    static EventManager& get() {
        static EventManager instance;
        return instance;
    };
    /** Sets the silenced mode state to \c true or \c false.
     *  In silenced mode, all subsequent log flags act as if
     *  they were set to \c false without actually changing
     *  their value.
     */
    static std::vector<std::string>  eventList();
    static void registerEvent(const std::string &eventName);
    static int eventID(const std::string &eventName);
    static std::string findName(const int &id);
    /** Sets the loudened mode state to \c true or \c false.
     *  In loudened mode, all subsequent log flags act as if
     *  they were set to \c true without actually changing
     *  their value.
     */
protected :
    std::unordered_map < std::string, int > _eventlist;
};

SSS_END;

#endif // SSS_EVENT_LIST_HPP