#include "Commons/eventList.hpp"

SSS_BEGIN;

inline std::vector<std::string> EventManager::eventList()
{
	std::vector<std::string> list;
	std::string tmp;
	for (const std::pair<std::string, int>& e : get()._eventlist)
	{
		tmp = "Event[" + e.first + "] : " + std::to_string(e.second);
		list.push_back(tmp);
	}
	return list;
}

void EventManager::registerEvent(const std::string &eventName)
{
	if (get()._eventlist.contains(eventName))
	{
		SSS::log_wrn("EVENT : [" + eventName + "] Already registered, risk of conflict");
		return;
	}
	
	get()._eventlist[eventName] = static_cast<int>(get()._eventlist.size());
	SSS::log_msg("EVENT : [" + eventName + "] "+SSS::toString(get()._eventlist[eventName])+ " Correctly registered");
}

int EventManager::eventID(const std::string& eventName)
{
	if (!get()._eventlist.contains(eventName)) {
		SSS::log_err("EVENT : [" + eventName + "] doesn't exist, or isn't registered");
		return INT32_MAX;
	}

	return get()._eventlist[eventName];
}

std::string EventManager::findName(const int& id)
{
	std::string tmp;
	for (const std::pair<std::string, int>& e : get()._eventlist)
	{
		if (e.second == id)
			return "Event<"+e.first+"> id :"+ SSS::toString(id);
	}
	return "Unknown Event["+SSS::toString(id)+"]";
}

SSS_END;