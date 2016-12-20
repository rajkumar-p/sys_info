#include <iostream>
#include <sys_info_server.h>
#include <sys_info_server_session.h>
#include <thread>

sys_info_server::sys_info_server(boost::asio::io_service &io_service, int port, std::map<std::string,
        client_config> authorized_clients, db_adapter *db_handle)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
        , socket_(io_service), authorized_clients(authorized_clients)
        , db_handle(db_handle)
{
    // Accept incoming requests
    do_accept();
}

sys_info_server::~sys_info_server()
{

}

/*
 * Accept incoming requests asynchronously.
 * Open socket to read and write.
 */
void sys_info_server::do_accept()
{
    acceptor_.async_accept(socket_,
        [this] (boost::system::error_code ec)
        {
            if (!ec) {
                std::make_shared<sys_info_server_session>(std::move(socket_), authorized_clients, db_handle)->start();
            } else {
                // Error
                std::cerr <<ec.category().name()<<" : "<<ec.value()<<std::endl;
            }

            do_accept();
        }
    );
}
