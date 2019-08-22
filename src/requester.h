#ifndef REQUESTER_HPP
#define REQUESTER_HPP

#include <iostream>
#include <functional>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/req.h>

#include "event_loop.h"
#include "event_source.h"

class Requester : public EventSource {
public:
	Requester(EventLoop* loop, std::string url);
	~Requester();

	void request(const char* message, uint16_t len);
	void request(const std::vector<char> &message);
	void request(const std::string &message);
	void set_receive_callback(std::function<void(const std::vector<char>)> callback);

protected:
	void pollin_event();

private:
	nng_dialer _dialer;
	nng_socket _socket;
	std::string _url;

	std::function<void(const std::vector<char>)> _callback;

	bool _request_busy;

};

#endif // REQUESTER_HPP
