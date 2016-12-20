#include <server_service.h>
#include <iostream>

server_service::server_service()
    : io_service {}, service_work {io_service}
    , service_thread(nullptr), server(nullptr)
{

}

void server_service::start()
{
    init_new_thread_for_service(io_service);
}

void server_service::init_new_thread_for_service(boost::asio::io_service &io_service)
{
    service_thread = std::unique_ptr<std::thread>(new std::thread([&io_service]() { io_service.run(); } ));
}

void server_service::stop()
{
    io_service.stop();
}

void server_service::start_server(int port, std::map<std::string, client_config> authorized_clients, db_adapter *db_handle)
{
    server = std::unique_ptr<sys_info_server>(new sys_info_server(io_service, port, std::move(authorized_clients), db_handle));
}

void server_service::finish_work_before_quiting()
{
    service_thread->join();
}

void server_service::wait_till_user_quits()
{
    std::string user_input;
    do {
        std::cout <<"[enter <quit> anytime to stop server]"<<std::endl;
        std::getline(std::cin, user_input);
    } while (user_input != "quit");
}
