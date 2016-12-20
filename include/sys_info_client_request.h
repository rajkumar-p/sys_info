#ifndef SYSINFOSERVER_SYS_INFO_CLIENT_REQUEST_H
#define SYSINFOSERVER_SYS_INFO_CLIENT_REQUEST_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <boost/asio/connect.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include "request.h"

using boost::asio::ip::tcp;

class sys_info_client_request : public request
{
public:
    sys_info_client_request();
    ~sys_info_client_request();

    void connect(std::string, std::string);
    std::size_t read(char [], std::size_t);
    void write(std::string);

private:

    // Variable section
    boost::asio::io_service io_service;
    tcp::socket s;
};

sys_info_client_request::sys_info_client_request()
        : io_service{}, s(io_service)
{
    std::cout <<"sys_info_client_request::sys_info_client_request()."<<std::endl;
}

sys_info_client_request::~sys_info_client_request()
{
    s.close();
    io_service.stop();
}

void sys_info_client_request::connect(std::string host, std::string port)
{
    tcp::resolver::query query{host, port};
    tcp::resolver resolver(io_service);
    boost::asio::connect(s, resolver.resolve(query));
}

void sys_info_client_request::write(std::string data)
{
    boost::asio::write(s, boost::asio::buffer(data.c_str(), data.length()));
}

std::size_t sys_info_client_request::read(char data[], std::size_t length)
{
    return boost::asio::read(s, boost::asio::buffer(data, length));
}

#endif //SYSINFOSERVER_SYS_INFO_CLIENT_REQUEST_H
