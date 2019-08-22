//
// Created by mauro on 8/22/19.
//

#include "src/event_loop.h"
#include "src/timer.h"
#include "src/publisher.h"

class TestPublisher : public EventLoop {
public:
    TestPublisher() :
            _periodic_timer(this),
            _periodic_pub("tcp://127.0.0.1:12345")
    {

    };
    ~TestPublisher(){};

    void init()
    {
        _periodic_timer.set_timeout_callback(std::bind(&TestPublisher::periodic, this));
        _periodic_timer.start_periodic(500);
    }

    void periodic()
    {
        static int i = 0;
        std::cout << "Pub" << std::endl;
        std::string data = std::to_string(i++);
        _periodic_pub.publish(data.c_str(), data.length());
    }

private:
    Timer _periodic_timer;
    Publisher _periodic_pub;
};

int main(int argc, char* argv[])
{
    TestPublisher testpub;

    testpub.init();
    testpub.run();
}