#include <iostream>
#include <sys_info_server_session.h>
#include <future>
#include "client_detail.h"
#include "utils.h"

// Contains the auth clients and the db handle
sys_info_server_session::sys_info_server_session(tcp::socket socket, std::map<std::string, client_config> &authorized_clients, db_adapter *db_handle)
        : socket_(std::move(socket)), authorized_clients(authorized_clients)
        , db_handle(db_handle)
{

}

void sys_info_server_session::start()
{
    // start reading data
    do_read();
}

// Read data asynchronously. Write response based on conditions
void sys_info_server_session::do_read()
{
    std::cout <<"sys_info_server_session::do_read()"<<std::endl;
    auto self(shared_from_this());
    // Read without blocking
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            std::string data_copy = std::string(data_, length);
            std::cout <<"sys_info_server_session::do_read(). length = "<<length<<std::endl;
            if (!ec)
            {
                std::cout <<"data : "<<data_copy<<std::endl;

                // Parse incoming data to client_detail struct
                client_detail c_detail = parse_to_client_detail(data_copy);

                // Check if client is authorized to write
                if (!is_client_authorized(authorized_clients, c_detail.client_key)) {
                    std::cout <<"Client not authorized."<<std::endl;
                    do_write(NOT_AUTHORIZED, strlen(NOT_AUTHORIZED));
                } else {
                    std::cout <<"Client authorized."<<std::endl;
                    // If authorized, insert into DB
                    write_to_db(generate_insert_str_for(c_detail));

                    // Check if to send an email based on client info
                    if (should_send_email(authorized_clients, c_detail.client_key, c_detail)) {
                        // If limit crossed, send email
                        send_email();
                    }
                    std::cout <<"do write."<<std::endl;
                    do_write(READ_SUCCESS, strlen(READ_SUCCESS));
                }
            } else {
                // Error
                std::cerr <<"Error in sys_info_server."<<std::endl;
                std::cerr <<ec.category().name()<<" : "<<ec.value()<<std::endl;
                do_write(READ_FAILURE, strlen(READ_FAILURE));
            }
        }
    );
}

// Write data asynchronously.
void sys_info_server_session::do_write(const char data[], std::size_t length)
{
    std::cout <<"Data : "<<data<<std::endl;
    auto self(shared_from_this());
    // Non blocking write
    boost::asio::async_write(socket_, boost::asio::buffer(data, length),
         [this, self, length, &data](boost::system::error_code ec, std::size_t /*length*/)
         {
             std::cout <<"sys_info_server_session::do_write(). length = "<<length<<std::endl;
             if (!ec) {
                 do_read();
             } else {
                 // Error
                 std::cerr <<"Error in sys_info_server."<<std::endl;
                 std::cerr <<ec.category().name()<<" : "<<ec.value()<<std::endl;
             }
         }
    );
}

// Write to the DB
void sys_info_server_session::write_to_db(std::string insert_SQL)
{
    std::async([this, insert_SQL]() {
        try {
            // Pass INSERT SQL to db
            db_handle->execute(insert_SQL);
            std::cout <<"Inserted into the database. "<<std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Exception: " << e.what() <<std::endl;
        }
    });
}

// Send email
void sys_info_server_session::send_email()
{
    std::cout <<"Sending email."<<std::endl;
    return;
}
