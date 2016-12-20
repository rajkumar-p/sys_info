#include <boost/asio.hpp>
#include <boost/asio/detail/service_registry.hpp>
#include <set>
#include "client_config.h"
#include "db_adapter.h"
#include "pg_db_adapter.h"

using boost::asio::ip::tcp;

#ifndef SYS_INFO_SYS_INFO_SERVER_H
#define SYS_INFO_SYS_INFO_SERVER_H

class sys_info_server
{
public:
    sys_info_server(boost::asio::io_service &io_service, int port, std::map<std::string, client_config> , db_adapter * );
    ~sys_info_server();

private:
    void do_accept();

    // Variable section
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::map<std::string, client_config> authorized_clients;
    db_adapter *db_handle;
};

#endif //SYS_INFO_SYS_INFO_SERVER_H
