#include <iostream>

#include "src/event_loop.h"
#include "src/timer.h"
#include "src/subscriber.h"
#include "src/publisher.h"
#include "src/replier.h"
#include "src/requester.h"
#include "src/udp_source.h"

using namespace std::placeholders;

class TestNode : public EventLoop {
public:
    TestNode() :
            timer(this),
            sub(this),
            pub("ipc:///tmp/testsocket"),
            replier(this),
            requester(this, "ipc:///tmp/repliersocket"),
            udp_source(this),
            testsub(this)
    {

    }

    ~TestNode()
    {

    }

    void on_timeout() {
//		return;
        std::cout << "timeout callback" << std::endl;
        std::string message = "Publishing on timer";
        pub.publish(message.c_str(), message.length());

        std::string request = "Requestion on timer";
        requester.request(request);

        std::cout << "Send heartbeat" << std::endl;
        std::string hb = "HB";
        udp_source.send_packet(hb);
    }

    void sub_callback(const std::vector<char> message) {
        std::string received_string(message.data(), message.size());
        std::cout << "Subscriber received: " << received_string << std::endl;
    }

    void replier_callback(const std::vector<char> message, std::vector<char> &reply) {
        std::string received_string(message.data(), message.size());
        std::cout << "Replier received: " << received_string << std::endl;
        std::string reply_string = "request was received!";
        reply.assign(reply_string.begin(), reply_string.end());
    }

    void requester_callback(const std::vector<char> reply) {
        std::string received_string(reply.data(), reply.size());
        std::cout << "Requester received: " << received_string << std::endl;
    }

    void udp_callback(const std::vector<char> message) {
        std::cout << "UDP callback" << std::endl;
    }

    void testsub_callback(const std::vector<char> message) {
        std::string received_string(message.data(), message.size());
        std::cout << received_string << std::endl;
    }

    int init()
    {
        std::cout << "Node init" << std::endl;

        // bind timer to callback
        timer.set_timeout_callback(std::bind(&TestNode::on_timeout, this));
        timer.start_periodic(1500);

        sub.set_receive_callback(std::bind(&TestNode::sub_callback, this, _1));
        sub.subscribe("ipc:///tmp/testsocket");

        replier.set_receive_callback(std::bind(&TestNode::replier_callback, this, _1, _2));
        replier.listen("ipc:///tmp/repliersocket");

        requester.set_receive_callback(std::bind(&TestNode::requester_callback, this, _1));

        udp_source.set_receive_callback(std::bind(&TestNode::udp_callback, this, _1));
        udp_source.connect("127.0.0.1", 14557, 14551);

        testsub.set_receive_callback(std::bind(&TestNode::testsub_callback, this, _1));
        testsub.subscribe("tcp://127.0.0.1:12345");

        return 0;
    }

private:
    Timer timer;
    Subscriber sub;
    Publisher pub;
    Replier replier;
    Requester requester;
    UdpSource udp_source;

    Subscriber testsub;
};

int main(int argc, char* argv[])
{
    std::cout << "Start test program" << std::endl;

    TestNode node;

    node.init();
    node.run();


    return 0;
}