#ifndef UDP_SOURCE_HPP
#define UDP_SOURCE_HPP

#include <arpa/inet.h>

#include <functional>

#include "event_loop.h"
#include "event_source.h"

#define BUFFER_LENGTH 2041

class UdpSource : public EventSource {
public:
	UdpSource(EventLoop* loop);
	~UdpSource();

	int connect(std::string remote_ip, unsigned int remote_port, unsigned int local_port);
	void set_receive_callback(std::function<void(const std::vector<char>)> callback);
	ssize_t send_packet(const char* message, ssize_t len);
	ssize_t send_packet(const std::vector<char> message);
	ssize_t send_packet(std::string message);

protected:
	void pollin_event();

private:
	struct sockaddr_in _uav_addr;
	struct sockaddr_in _loc_addr;
	uint8_t buf[BUFFER_LENGTH];

	std::function<void(const std::vector<char>)> _callback;
};

#endif // UDP_SOURCE_HPP
