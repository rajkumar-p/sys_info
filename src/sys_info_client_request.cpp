//#include <sys_info_client_request.h>
//#include <iostream>
//#include <boost/asio.hpp>
//
//sys_info_client_request::sys_info_client_request()
//    : io_service{}, s(io_service)
//{
//    std::cout <<"sys_info_client_request::sys_info_client_request()."<<std::endl;
//}
//
//sys_info_client_request::~sys_info_client_request()
//{
//    s.close();
//    io_service.stop();
//}
//
//void sys_info_client_request::connect(std::string host, std::string port)
//{
//    tcp::resolver::query query{host, port};
//    tcp::resolver resolver(io_service);
//    boost::asio::connect(s, resolver.resolve(query));
//}
//
//void sys_info_client_request::write(std::string data)
//{
//    boost::asio::write(s, boost::asio::buffer(data.c_str(), data.length()));
//}
//
//std::size_t sys_info_client_request::read(char *data, std::size_t length)
//{
//    return boost::asio::read(s, boost::asio::buffer(data, length));
//}
