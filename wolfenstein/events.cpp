#include "events.hpp"
#include <iostream>
#include <string>

namespace wolf{

events& events::getInstance()
{
	static events instance;
	return instance;
}

void events::broadcast(const std::string& key) const
{
		//std::string wrong = "wrong";
		auto it = m_events.begin();
		while(it!= m_events.end())
		{
				if(it->second.first == key)
				{
						it->second.second();
				}
				it++;
		}
}

void events::register_event(std::string address, std::string key, std::function<void()> func)
{
			m_events.insert(std::make_pair(address,std::make_pair(key,func)));
}

void events::unregister_event(const std::string& address)
{
		m_events.erase(address);
}

void events::print()
{
    auto it = m_events.begin();
    while(it != m_events.end())
    {
         std::cout << it->second.first << std::endl;
         it++;
    }

}

void events::set_context(bool c)
{
	context_created = c;
}

bool events::get_context() const
{
	return context_created;
}
}
