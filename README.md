# A TCP Server and Client

## Overview
This is a simple TCP server and client. The server and client run in an intranet environment. The server is installed in a central location and the clients can be installed in 1000s of machines. The client is responsible of periodically sending machine information to the server. The server reads the data from the client and stores the data in the database. The server also sends an email if the client machine info crosses a pre-configured limit.

## Goals
**Performant :** The server and client should be highly performant. Operations should not block where possible  
**Simple :** The application model should be easy to think and verify  
**Loosely Coupled :** Components should not be highly coupled making it harder to change later  
**Modular :** The code base and the components need to be highly modular and encapsulate their functions  
**Best Practices :** Where possible, use the framework’s best practices and standards

## Build Instructions
* Modify **PG_INCLUDE_DIR** in Source\sys_info\CMakeLists.txt to point to **libpqxx** include dir  
* Modify the **target_link_libraries** entry in the same file to point to **libpqxx.a** file  
* cd into the build directory, and issue, **cmake ..**  
* Once, cmake is successful, run **make**  
* You have **sys_info_server**, **sys_info_client** and **sys_info_tests** binaries  
* sys_info_server usage - **sys_info_server \<port> \<path_to_server_config.xml> \<path_to_db_config.xml>**  
* sys_info_client usage - **sys_info_client \<ip> \<port> \<client_key>**  
* sys_info_tests usage - **sys_info_tests** runs all tests. See **sys_info_tests -h** for all options. Change the path to the config xmls before running the tests if you want to override defaults  
* **server_config.xml** and **db_config.xml** samples are provided at Source\sys_info\  

## Pre-requisites
* C++11 compiler. gcc/clang/vc++ later versions support C++11 features. To install one, either build from source or use a package manager for your platform 
* Boost libraries. Version 1.60.0 or greater. To install, either build from source or use a package manager for your platform  
* CMake. Version 3.7.0 or greater. The project uses CMake to generate the build files  
* PostgreSQL is used as the database backend. Install version 9.4 or greater. You can also install a postgreSQL client of your choice if you need an UI to connect to the DB  
* libpqxx. This is the C++ lib to interface with the PostgreSQL database. You can either build from source or use a package manager for your platform. More info at http://pqxx.org/development/libpqxx/


## Data Model
<code>
CREATE TABLE IF NOT EXISTS CLIENT_INFO ( ID SERIAL PRIMARY KEY,  CLIENT_KEY TEXT,  MEMORY_PCT INTEGER,  CPU_PCT INTEGER,  NO_PROCESSES INTEGER,  DT_TIME TEXT );
</code>


The data model is very simple. CLIENT_INFO represents the client data that is inserted into the database. The client keys can be pushed into an separate table and the references can be used in CLIENT_INFO but that level of normalisation is not needed in our application.

## DB Setup
* Create db, user, password to use with the application. You can use the details for tests as well as the actual run. You can refer here - https://www.postgresql.org/docs/9.1/static/index.html on how to create from command line. Config your app like http://stackoverflow.com/questions/4328679/how-to-configure-postgresql-so-it-accepts-loginpassword-auth so it accepts username and password  
* To setup the table needed for the app, run the create_client_info_tables.sql script under the root project folder. You can use the drop_client_info_tables.sql present under the same location to drop the tables


## Tests
Tests are implemented with the help of the [Catch](https://github.com/philsquared/Catch) test framework. Unit and Integration tests are implemented for the server and client. The tests can be run using the test target. Various test options can be seen using the -h switch.

## License
Free as in free speech.

## Contributions & Questions
Send me a mail on <raj@diskodev.com> or tweet me at <https://twitter.com/#!/rajkumar_p>