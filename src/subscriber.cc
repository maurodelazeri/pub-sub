#include "subscriber.h"

Subscriber::Subscriber(EventLoop *loop) :
	_callback(nullptr)
{
	loop->add_event_source(this);
}

Subscriber::~Subscriber()
{

}

int Subscriber::subscribe(std::string url)
{
	_url = url;
	std::cout << "Subscribe: " << _url << std::endl;
	int rv;

	if ((rv = nng_sub0_open(&_socket)) != 0) {
		std::cerr << "[subscription] nng_sub0_open" << nng_strerror(rv) << std::endl;
		return 1;
	}

	if ((rv = nng_setopt(_socket, NNG_OPT_SUB_SUBSCRIBE, "", 0)) != 0) {
		std::cerr << "[subscription] nng_setopt" << nng_strerror(rv) << std::endl;
		return 1;
	}

	if ((rv = nng_dial(_socket, _url.c_str(), NULL, NNG_FLAG_NONBLOCK))) {
		std::cerr << "[subscription] nng_dial" << nng_strerror(rv) << std::endl;
		return 1;
	}

	// Set re-connect time to 500ms, helps when starting subscriber first
	if ((rv = nng_setopt_ms(_socket, NNG_OPT_RECONNMINT, 500)) != 0) {
		std::cerr << "[subscription] nng_setopt_ms" << nng_strerror(rv) << std::endl;
		return 1;
	}

	if ((rv = nng_setopt_ms(_socket, NNG_OPT_RECONNMAXT, 500)) != 0) {
		std::cerr << "[subscription] nng_setopt_ms" << nng_strerror(rv) << std::endl;
		return 1;
	}

	if ((rv = nng_getopt_int(_socket, NNG_OPT_RECVFD, &_fd))) {
		std::cerr << "[subscription] nng_getopt" << nng_strerror(rv) << std::endl;
		return 1;
	}

	return 0;
}

void Subscriber::set_receive_callback(std::function<void (const std::vector<char>)> callback)
{
	_callback = callback;
}

void Subscriber::pollin_event()
{
	nng_msg *msg;
	nng_recvmsg(_socket, &msg, 0);
	std::vector<char> message((char*)nng_msg_body(msg), (char*)nng_msg_body(msg) + nng_msg_len(msg));

	// free the message
	nng_msg_free(msg);

	// send the received message to the callback
	if (_callback == nullptr) {
		std::cerr << "No callback defined for subscriber" << std::endl;
	} else {
		_callback(message);
	}
}
