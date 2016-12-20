#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <pg_db_adapter.h>
#include <boost/asio/io_service.hpp>
#include <sys_info_server.h>
#include <thread>
#include <request.h>
#include <sys_info_client_request.h>
#include <server_service.h>
#include "catch.h"

#include "utils.h"

// Test XML parsing logic - db_settings.xml

TEST_CASE("Server config settings read from server_config.xml", "[Parsing server_config.xml]") {

    std::map<std::string, client_config> authorized_clients = get_authorized_clients_and_details("../test/resources/server_config.xml");
    SECTION("Number of authorized clients should be three.") {
        REQUIRE(authorized_clients.size() == 3);
    }

    SECTION("Entry with key [jill] should have an email of jill@jill.com") {
        REQUIRE(authorized_clients["jill"].email == "jill@jill.com");
    }

    SECTION("Entry with key [mary] should have limits of memory: 27, cpu: 24, processes: 45") {
        client_config c_config = authorized_clients["mary"];
        REQUIRE(c_config.memory_limit == 27);
        REQUIRE(c_config.cpu_limit == 36);
        REQUIRE(c_config.processes_limit == 45);
    }
}

TEST_CASE("Database settings read from db_settings.xml", "[Parsing db_settings.xml]") {

    std::string connection_string = "dbname=hector user=mkts password=password hostaddr=192.6.6.6 port=1111";
    REQUIRE(  get_connection_string("../test/resources/db_settings.xml") == connection_string );
}

TEST_CASE("Check various functions defined in utils.h", "[Testing utils.h]") {

    std::map<std::string, client_config> authorized_clients = get_authorized_clients_and_details("../test/resources/server_config.xml");

    SECTION("Client with client_key [ssss] should not be authorized.") {
        REQUIRE(is_client_authorized(authorized_clients, "ssss") == false);
    }

    SECTION("Client with client_key [jack] should be authorized.") {
        REQUIRE(is_client_authorized(authorized_clients, "jack") == true);
    }

    SECTION("Client details  should be parsed successfully.") {
        client_detail c_detail = parse_to_client_detail("xxx|16|88|25|Sat Nov 26 18:31:34 2016");
        REQUIRE(c_detail.client_key == "xxx");
        REQUIRE(c_detail.curr_memory_pct == 16);
        REQUIRE(c_detail.curr_cpu_pct == 88);
        REQUIRE(c_detail.curr_processes_cnt == 25);
        REQUIRE(c_detail.str_dt_time == "Sat Nov 26 18:31:34 2016");
    }

    SECTION("INSERT SQL should be generated from client_detail struct.") {
        client_detail c_detail = parse_to_client_detail("xxx|16|88|25|Sat Nov 26 18:31:34 2016");
        std::string insert_query =
                "INSERT INTO CLIENT_INFO (CLIENT_KEY, MEMORY_PCT, CPU_PCT, NO_PROCESSES, DT_TIME) VALUES ('xxx', 16, 88, 25, 'Sat Nov 26 18:31:34 2016');";

        REQUIRE(generate_insert_str_for(c_detail) == insert_query);
    }

    SECTION("Mail should be sent when alerts cross threshold") {
        std::map<std::string, client_config> authorized_clients = get_authorized_clients_and_details("../test/resources/server_config.xml");
        client_detail c_detail1 = parse_to_client_detail("jill|16|20|23|Sat Nov 26 18:31:34 2016");
        client_detail c_detail2 = parse_to_client_detail("mary|28|35|44|Sat Nov 26 18:31:34 2016");

        REQUIRE(should_send_email(authorized_clients, c_detail1.client_key, c_detail1) == false);
        REQUIRE(should_send_email(authorized_clients, c_detail2.client_key, c_detail2) == true);
    }
}

TEST_CASE("Check pg_db_adapter functionality", "[Testing pg_db_adapter]") {
    std::string connection_string = get_connection_string("../db_settings.xml") ;

    // Prepare test table
    pg_db_adapter db_handle(connection_string);
    db_handle.execute("CREATE TABLE IF NOT EXISTS CLIENT_INFO_TEST_DATA( ID SERIAL PRIMARY KEY, CLIENT_KEY TEXT, MEMORY_PCT INTEGER, CPU_PCT INTEGER, NO_PROCESSES INTEGER, DT_TIME TEXT);");

    SECTION("Check if rows are inserted correctly") {
        client_detail c_detail1 = parse_to_client_detail("xxx|16|88|25|Sat Nov 26 18:31:34 2016");
        std::string insert_string = generate_insert_str_for(c_detail1, "CLIENT_INFO_TEST_DATA");
        db_handle.execute(insert_string);
        std::string select_query = "SELECT ID FROM CLIENT_INFO_TEST_DATA WHERE CLIENT_KEY='" + c_detail1.client_key + "' AND DT_TIME = '" + c_detail1.str_dt_time + "';";

        REQUIRE(db_handle.query_returned_row(select_query) == true);
    }

    db_handle.execute("DROP TABLE CLIENT_INFO_TEST_DATA;");
}

TEST_CASE("Server and Client integeration tests", "[Server and Client integration tests]") {
    std::map<std::string, client_config> authorized_clients_and_details = get_authorized_clients_and_details("../test/resources/server_config.xml");
    // Note : Provide the db settings that will be used by the tests during integration testing
    std::string connection_string = get_connection_string("../tests_db_settings.xml") ;
    std::string port = "1335";
    std::string client_key = "jill";

    db_adapter *db_handle = new pg_db_adapter(connection_string);

    server_service s_service;
    s_service.start();
    s_service.start_server(atoi(port.c_str()), authorized_clients_and_details, db_handle);

    // Client
    std::auto_ptr<request> client_request(new sys_info_client_request());
    client_request->connect("127.0.0.1", port);

    std::string line_to_be_written = "jill|44|89|11|Sat Nov 26 17:25:44 2016";
    client_request->write(line_to_be_written);

    char reply1[3];
    client_request->read(reply1, 1);

    SECTION("Check if data written and read from the server correctly") {
        std::cout <<"Reply1 success."<<std::endl;
        REQUIRE(strcmp(reply1, "0") == 0);
    }

    line_to_be_written = "mary|14|65|30|Sat Nov 26 17:28:44 2016";
    client_request->write(line_to_be_written);

    char reply2[3];
    client_request->read(reply2, 1);

    SECTION("Check if data written and read from the server correctly") {
        std::cout <<"Reply2 success."<<std::endl;
        REQUIRE(strcmp(reply2, "0") == 0);
    }

    line_to_be_written = "xxx|14|65|30|Sat Nov 26 17:28:44 2016";
    client_request->write(line_to_be_written);

    char reply3[3];
    client_request->read(reply3, 1);

    SECTION("Check if data written and read from the server correctly") {
        std::cout <<"Reply3 Failure. So success."<<std::endl;
        REQUIRE(strcmp(reply3, "0") != 0);
    }

    // Wait for the work and do not return
    s_service.stop();
    s_service.finish_work_before_quiting();

    delete db_handle;

//    thread.join();
}

