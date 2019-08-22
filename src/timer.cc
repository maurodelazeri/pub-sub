#include <unistd.h>
#include <sys/timerfd.h>
#include <iostream>
#include "timer.h"

Timer::Timer(EventLoop *loop) :
	_running(false)
{
	loop->add_event_source(this);

	_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);

}

Timer::~Timer()
{
	close(_fd);
}

void Timer::set_timeout_callback(std::function<void ()> callback)
{
	_callback = callback;
}

int Timer::start_periodic(int interval_ms)
{
	struct timespec ts;
	ts.tv_sec = interval_ms / 1000;
	ts.tv_nsec = (interval_ms % 1000) * 1e6;
	struct itimerspec its;
	its.it_interval = ts;
	its.it_value = ts;

	if (timerfd_settime(_fd, 0, &its, NULL) == -1) {
		std::cerr << "ERROR setting periodic timer" << std::endl;
		return 1;
	}
	_running = true;
	return 0;
}

int Timer::start_single_shot(int time_ms)
{
	struct timespec ts_zero = {0};
	struct timespec ts;
	ts.tv_sec = time_ms / 1000;
	ts.tv_nsec = (time_ms % 1000) * 1e6;
	struct itimerspec its;
	its.it_interval = ts_zero;
	its.it_value = ts;

	if (timerfd_settime(_fd, 0, &its, NULL) == -1) {
		std::cerr << "ERROR setting single shot timer" << std::endl;
	}
	_running = true;
	return 0;
}

int Timer::stop()
{
	if (!_running) {
		return 1;
	}

	struct timespec zero = {0};
	struct itimerspec its;
	its.it_interval = zero;
	its.it_value = zero;

	if (timerfd_settime(_fd, 0, &its, NULL) == -1) {
		std::cerr << "Error clearing timer" << std::endl;
		return 1;
	}

	// process any last events
	pollin_event();

	return 0;
}

void Timer::pollin_event()
{
	uint64_t num_timer_events;
	ssize_t recv_size = read(_fd, &num_timer_events, 8);
	if (recv_size > 0) {
		if (_callback == nullptr) {
			std::cout << "Timer event, but no callback defined" << std::endl;
		} else {
			_callback();
		}
	}
}
