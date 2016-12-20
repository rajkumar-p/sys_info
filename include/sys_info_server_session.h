#include <vector>
#include <boost/asio.hpp>
#include <boost/asio/detail/service_registry.hpp>
#include <set>
#include "client_config.h"
#include "db_adapter.h"
#include "pg_db_adapter.h"

using boost::asio::ip::tcp;

#ifndef SYSINFOSERVER_SYS_INFO_SERVER_SESSION_H
#define SYSINFOSERVER_SYS_INFO_SERVER_SESSION_H

class sys_info_server_session : public std::enable_shared_from_this<sys_info_server_session>
{
public:
    sys_info_server_session(tcp::socket socket, std::map<std::string, client_config> &authorized_clients, db_adapter * );
    void start();

private:
    void do_write(const char data[],std::size_t length);
    void do_read();

    void write_to_db(std::string );
    void send_email();

    // Variables section
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];

    char READ_SUCCESS[3] = "0\0";
    char READ_FAILURE[3] = "1\0";
    char NOT_AUTHORIZED[3] = "2\0";

    std::string str_data_;
    std::map<std::string, client_config> &authorized_clients;
    db_adapter *db_handle;
};

#endif //SYSINFOSERVER_SYS_INFO_SERVER_SESSION_H
