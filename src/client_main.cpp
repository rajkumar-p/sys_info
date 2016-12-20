#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <iomanip>
#include <boost_logger.h>
#include <repeat_timer.h>
#include <boost/random.hpp>
#include <boost/algorithm/string.hpp>
#include <future>
#include <fstream>
#include <request.h>
#include <sys_info_client_request.h>
#include <utils.h>

using boost::asio::ip::tcp;


int main(int argc, char *argv[]) {
    std::auto_ptr<logger> make_logger(std::string );
    void wait_till_user_quits();

    // Client logger
    std::auto_ptr<logger> logger = make_logger("running_log.log");

    try
    {
        if (argc != 4)
        {
            std::cerr << "Error. Usage : sys_info_client <host> <port> <client_key>"<<std::endl;
            return 1;
        }

        std::string hostname {argv[1]};
        std::string port {argv[2]};
        std::string client_key {argv[3]};
        std::atomic<bool> stop_client{false};

        /*
         * Start a async task to write/read from the server.
         * The timer is configured to repeat after an interval.
         * The timer runs indefintely until cancelled by the user.
         * The periodic task is given to the timer object as an closure
         */
        std::future<void> async_client_task(std::async(
            [hostname, port, client_key, &stop_client, &logger]() {
                boost::posix_time::minutes repeat_interval{1};
                repeat_timer r_timer{repeat_interval, stop_client};
                r_timer.to_do(
                    [&logger, hostname, port, client_key, &stop_client]() {
                        /*
                         * Create a new client request.
                         * Connect and write data to the server ip and port.
                         */
                        std::auto_ptr<request> client_request(new sys_info_client_request());

                        std::cout <<"Establishing connection to the server."<<std::endl;
                        client_request->connect(hostname, port);
                        std::cout <<"Connected to "<<hostname<<" on port "<<port<<std::endl;

                        /*
                         * Client info is simulated.
                         * Random percentages are generated as client info.
                         * TODO: Write a cross platform & high performing lib to access machine resources info.
                         */
                        // Generate random sys_info data
                        std::string line_to_be_written = generate_random_sys_info_data(client_key);
                        logger->log_message(logger_severity::info, line_to_be_written);

                        // Write to data. No need for async write since this in itself is a async task
                        std::cout <<"Writing data to server. data : "<<line_to_be_written<<std::endl;
                        client_request->write(line_to_be_written);

                        // Read from data. No need for async read since this in itself is a async task
                        char reply[3];
                        std::cout <<"Reading data from server."<<std::endl;
                        std::size_t  reply_length = client_request->read(reply, 1);
                        std::cout <<"Length of data read - "<<reply_length<<std::endl;

                        // Success or Failure?
                        if (std::strcmp(reply, "1") == 0) {
                            // Failure
                            std::cout <<"Failure in reading data from server."<<std::endl;
                            logger->log_message(logger_severity::error, "Failure : Sending data failed. Data : " + line_to_be_written);
                            return;
                        } else if (std::strcmp(reply, "2") == 0) {
                            // Not Authorized. Log error and message the client to stop.
                            std::cout <<"Not authorized to read and write to server."<<std::endl;
                            logger->log_message(logger_severity::fatal, "Failure : Not Authorized");
                            stop_client = true;
                            return;
                        }

                        std::cout <<"Success in reading data. data : "<<reply<<std::endl;
                    }
                );
                r_timer.start_timer();
            }
        ));

        // Input from the user if to quit
        wait_till_user_quits();

        stop_client = true;

        // Complete any pending tasks
        std::cout <<"Shutting down client."<<std::endl;
        std::cout <<"Please wait..."<<std::endl;

        // Wait on the client task and not return until the user quits
        async_client_task.get();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        logger->log_message(logger_severity::error, e.what());
        return 1;
    }

    return 0;
}

std::auto_ptr<logger> make_logger(std::string filename)
{
    return std::auto_ptr<logger>(new boost_logger(filename));
}

void wait_till_user_quits()
{
    std::string user_input;
    do {
        std::cout <<"[enter <quit> anytime to stop server]"<<std::endl;
        std::getline(std::cin, user_input);
    } while (user_input != "quit");
}
