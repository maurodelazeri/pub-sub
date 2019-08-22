#include <string.h>
#include "replier.h"

Replier::Replier(EventLoop* loop) :
	_callback(nullptr)
{
	loop->add_event_source(this);
}

Replier::~Replier()
{

}

int Replier::listen(std::string url)
{
	_url = url;
	int rv;
	// Configure reply topic for utmsp request
	if ((rv = nng_rep0_open(&_socket)) != 0) {
		std::cerr << "[replier] nng_rep0_open: " << nng_strerror(rv) << std::endl;
		return 1;
	}

	if ((rv = nng_listen(_socket, _url.c_str(), NULL, NNG_FLAG_NONBLOCK))) {
		std::cerr << "[replier] nng_listen: " << nng_strerror(rv) << std::endl;
		return 1;
	}

	if ((rv = nng_getopt_int(_socket, NNG_OPT_RECVFD, &_fd))) {
		std::cerr << "[replier] nng_getopt: " << nng_strerror(rv) << std::endl;
		return 1;
	}

	return 0;
}

void Replier::set_receive_callback(std::function<void(const std::vector<char>, std::vector<char> &)> callback)
{
	_callback = callback;
}

void Replier::pollin_event()
{
	nng_msg *msg;
	nng_recvmsg(_socket, &msg, 0);

	std::vector<char> message((char*)nng_msg_body(msg), (char*)nng_msg_body(msg) + nng_msg_len(msg));

	// free the message
	nng_msg_free(msg);

	std::string reply_string = "NOTOK";
	std::vector<char> reply(reply_string.begin(), reply_string.end());

	if (_callback == nullptr) {
		std::cerr << "No callback defined!" << std::endl;
	} else {
		_callback(message, reply);
	}

	// send response
	nng_msg_alloc(&msg, reply.size());
	memcpy(nng_msg_body(msg), reply.data(), reply.size());
	nng_sendmsg(_socket, msg, 0);
}
