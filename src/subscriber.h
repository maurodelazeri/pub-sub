#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include <iostream>
#include <functional>

#include <nng/nng.h>
#include <nng/protocol/pubsub0/sub.h>

#include "event_loop.h"
#include "event_source.h"

class Subscriber : public EventSource {
public:
	Subscriber(EventLoop* loop);
	~Subscriber();

	int subscribe(std::string url);
	void set_receive_callback(std::function<void(const std::vector<char>)> callback);

protected:
	void pollin_event();

private:
	nng_socket _socket;
	std::string _url;

	std::function<void(const std::vector<char>)> _callback;
};

#endif // SUBSCRIBER_HPP
