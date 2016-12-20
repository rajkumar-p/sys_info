#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <sys_info_server.h>
#include <boost/property_tree/xml_parser.hpp>
#include <thread>
#include <server_service.h>
#include "utils.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    int port;

    try
    {
        if (argc != 4) {
            std::cerr << "Error. Usage : sys_info_server <port> <server_config.xml> <db_settings.xml>"<<std::endl;
            return 1;
        }

        // Check if port is valid
        port = std::atoi(argv[1]);

        if (!port) {
            std::cerr <<"Error. Not a valid port."<<std::endl;
            return 1;
        }

        // Get authorized clients and details from server_config.xml
        std::map<std::string, client_config> authorized_clients_and_details =
                get_authorized_clients_and_details(argv[2]);

        // Get connection string from db_settings.xml
        std::string connection_string = get_connection_string(argv[3]);
        db_adapter *db_handle = new pg_db_adapter(connection_string);

        /*
         * Create a io_service to start listening on port.
         * Each client connection is designed to run on a separate thread.
         * Dependency management is done by passing the necessary objects through the layers.
         */
        server_service s_service;
        s_service.start();
        s_service.start_server(port, authorized_clients_and_details, db_handle);

        // Input from the user if to quit
        s_service.wait_till_user_quits();

        s_service.stop();
        s_service.finish_work_before_quiting();

        // Clean up db resources
        delete  db_handle;

////        std::cout <<"main thread id : "<<std::this_thread::get_id()<<std::endl;
//        boost::asio::io_service io_service;
//        boost::asio::io_service::work work(io_service);
//        std::thread thread([&io_service]() { io_service.run(); });
//
//        // Auth clients and db handle passed through
//        sys_info_server s(io_service, port, authorized_clients_and_details, db_handle);
//
//        // Input from the user if to quit
//        wait_till_user_quits();
//
//        io_service.stop();
//
//        // Complete any pending tasks
//        std::cout <<"Shutting down server."<<std::endl;
//        std::cout <<"Please wait..."<<std::endl;
//
//        // Wait for the work and do not return
//        thread.join();
//
//        // Clean up db resources
//        delete  db_handle;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

