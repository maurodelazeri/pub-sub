#include "publisher.h"

Publisher::Publisher(std::string url)
{
	_url = url;
	int rv;
	// start the publisher
	if ((rv = nng_pub0_open(&_socket)) != 0) {
		std::cerr << "[publisher] nng_pub0_open" << nng_strerror(rv) << std::endl;
	}

	if ((rv = nng_listen(_socket, url.c_str(), NULL, NNG_FLAG_NONBLOCK)) != 0) {
		std::cerr << "[publisher] nng_listen" << nng_strerror(rv) << std::endl;
	}
}

Publisher::~Publisher()
{

}

void Publisher::publish(const char *message, uint16_t len)
{
	nng_msg *msg;
	nng_msg_alloc(&msg, 0);
	nng_msg_append(msg, message, len);
	nng_sendmsg(_socket, msg, 0);
}

void Publisher::publish(const std::vector<char> &message)
{
	publish(message.data(), message.size());
}

void Publisher::publish(const std::string &message)
{
	publish(message.c_str(), message.length());
}
