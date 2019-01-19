#pragma once
#include <map>
#include <functional>

namespace wolf{
class events {
public:
	static events& getInstance();
	//void broadcast(const std::string& key) const;
	void broadcast(const std::string& key) const;
	//void register_event(std::string key, std::function<void()> func);
	void register_event(std::string address, std::string key, std::function<void()> func);
	void unregister_event(const std::string& address);
	void set_context(bool c);
	bool get_context() const;
  void print();

	events(events const&) = delete;
	void operator=(events const&) = delete;

private:

	events(){}
	//events(events const&);
	//void operator=(events const&);
	std::multimap<std::string, std::pair<std::string, std::function<void()>> > m_events;
	bool context_created = false;

public:


};
}
