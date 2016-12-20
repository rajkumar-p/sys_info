#ifndef SYSINFOSERVER_SERVER_SERVICE_H
#define SYSINFOSERVER_SERVER_SERVICE_H

#include <boost/asio/io_service.hpp>
#include <thread>
#include <map>
#include <future>
#include "client_config.h"
#include "db_adapter.h"
#include "sys_info_server.h"

class server_service {
public:
    server_service();

    void start();
    void stop();

    void start_server(int , std::map<std::string, client_config> , db_adapter *);
    void finish_work_before_quiting();
    void wait_till_user_quits();

    boost::asio::io_service io_service;
    boost::asio::io_service::work service_work;
    std::unique_ptr<std::thread> service_thread;
    std::unique_ptr<sys_info_server> server;

private:
    void init_new_thread_for_service(boost::asio::io_service &);
};

#endif //SYSINFOSERVER_SERVER_SERVICE_H
