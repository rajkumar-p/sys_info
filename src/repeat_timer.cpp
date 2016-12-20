//
#include <iostream>
#include "repeat_timer.h"

repeat_timer::repeat_timer(boost::posix_time::minutes minutes, std::atomic<bool> &stop_client)
    : io_service{}, interval(minutes), timer {io_service, interval}, stop(stop_client)
{
    std::cout <<"repeat_timer::repeat_time()"<<std::endl;
}

// Start the timer and do the work
void repeat_timer::start_timer()
{
    std::cout <<"Timer started."<<std::endl;
    timer.async_wait([this] (const boost::system::error_code &ec)
         {
             // Work to do
             work();
         }
    );
    io_service.run();
}

void repeat_timer::work()
{
    std::cout <<"Calling user work."<<std::endl;
    // Repeat work is the user suplied work. This is stored in an function
    repeat_work();

    std::cout <<"Stop? : "<<stop<<std::endl;

    // Repeat timer
    if (!stop) {
        timer.expires_at(timer.expires_at() + interval);
        timer.async_wait([this] (const boost::system::error_code &ec)
             {
                 work();
             }
        );
    } else {
        std::cout <<"Stopping timer."<<std::endl;
    }
}

void repeat_timer::to_do(std::function<void()> f)
{
    // User supplied work is set here.
    std::cout <<"Setting work to do."<<std::endl;
    repeat_work = f;
}

repeat_timer::~repeat_timer()
{
    // Clean up the resources
    timer.cancel();
    io_service.stop();

    std::cout <<"repeat_timer::~repeat_timer() done."<<std::endl;
}
