#ifndef SYSINFOSERVER_REPEAT_TIMER_H
#define SYSINFOSERVER_REPEAT_TIMER_H

#include <boost/asio.hpp>

class repeat_timer {
public:
    repeat_timer(boost::posix_time::minutes minutes, std::atomic<bool> &stop_client);
    ~repeat_timer();

    void start_timer();
    void to_do(std::function<void()> );

private:
    void work();

    // Variables section
    boost::asio::io_service io_service;
    boost::posix_time::minutes interval;
    boost::asio::deadline_timer timer;

    std::function<void()> repeat_work;
    std::atomic<bool> &stop;
};


#endif //SYSINFOSERVER_REPEAT_TIMER_H
