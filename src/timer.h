#ifndef TIMER_HPP
#define TIMER_HPP

#include <functional>

#include "event_loop.h"
#include "event_source.h"

class Timer : public EventSource {
public:
	Timer(EventLoop* loop);
	~Timer();

	int start_periodic(int interval_ms);
	int start_single_shot(int time_ms);
	int stop();
	void set_timeout_callback(std::function<void(void)> callback);

protected:
	void pollin_event();

private:
	std::function<void(void)> _callback = nullptr;
	bool _running;
};

#endif // TIMER_HPP
