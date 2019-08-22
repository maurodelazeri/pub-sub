#include <poll.h>
#include <iostream>
#include <csignal>
#include "event_loop.h"

namespace {
	volatile std::sig_atomic_t gSignalStatus;
}

void signal_handler(int signum)
{
	gSignalStatus = signum;
}

EventLoop::EventLoop()
{

}

void EventLoop::run()
{
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);

	int len = _event_sources.size();
	struct pollfd fds[len];

	for (ssize_t i = 0; i < len; i++) {
		fds[i].fd = _event_sources.at(i)->fd();
		std::cout << "fd: " << fds[i].fd << std::endl;
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}

	while (true) {

		int ret = poll(fds, len, 2000);

		if (gSignalStatus != 0) {
			std::cout << "Ending event loop" << std::endl;
			break;
		}

		if (ret == 0) {
			std::cout << "No event for 2000ms" << std::endl;
			continue;
		}

		// check which event it was, and trigger the callback
		for (ssize_t i = 0; i < len; i++) {
			if (fds[i].revents & POLLIN) {
				_event_sources.at(i)->pollin_event();
				continue;
			}
		}
	}

	std::cout << std::endl << "Loop closed" << std::endl;

}

int EventLoop::add_event_source(EventSource *es)
{
	_event_sources.push_back(es);
}

EventLoop::~EventLoop()
{
	std::cout << "EventLoop destructor called." << std::endl;
}
