#ifndef EVENT_SOURCE_HPP
#define EVENT_SOURCE_HPP

class EventSource {
public:
	EventSource();
	virtual ~EventSource() = 0;

	virtual void pollin_event();

	int fd();

protected:
	int _fd;
};

#endif // EVENT_SOURCE_HPP
