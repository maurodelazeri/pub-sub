#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include <vector>

#include "event_source.h"

class EventLoop {
public:

	EventLoop();
	virtual ~EventLoop() = 0;

	void run();
	int add_event_source(EventSource* es);


private:
	std::vector<EventSource*> _event_sources;

};

#endif // EVENT_LOOP_HPP
