# CMake minimum version
cmake_minimum_required(VERSION 3.6)
project(SysInfoServer)

# Using c++11 std
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD 14)

# Find Boost package
find_package(Boost 1.60.0 COMPONENTS system thread log REQUIRED)

# If Boost is found, add the include and link libraries
if(Boost_FOUND)
#    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
endif()

# Catch test framework settings
add_library(Catch INTERFACE)

# PostgreSQL settings
set(PG_INCLUDE_DIR /usr/local/Cellar/libpqxx/4.0.1/include)
set(PG_LINKER_OPTION_PQXX "/usr/local/Cellar/libpqxx/4.0.1/lib/libpqxx.a -lp")
set(PG_LINKER_OPTION_PQ -lp)

# Include directories
include_directories(include)
include_directories(${PG_INCLUDE_DIR})

# Add sources
set(SERVER_SOURCES "src/server_main.cpp" include/sys_info_server.h src/sys_info_server.cpp include/sys_info_server_session.h src/sys_info_server_session.cpp src/repeat_timer.cpp include/repeat_timer.h include/client_config.h src/db_adapter.cpp include/db_adapter.h src/pg_db_adapter.cpp include/pg_db_adapter.h include/utils.h src/utils.cpp include/client_detail.h include/server_service.h src/server_service.cpp)
set(CLIENT_SOURCES "src/client_main.cpp" include/logger.h include/boost_logger.h include/repeat_timer.h src/repeat_timer.cpp include/request.h include/sys_info_client_request.h src/sys_info_client_request.cpp include/utils.h src/utils.cpp)
set(TEST_SOURCES include/catch.h test/sys_info_tests.cpp)

# Settings for boost logging lib
add_definitions(-DBOOST_LOG_DYN_LINK)

add_executable(sys_info_server ${SERVER_SOURCES} include/sys_info_server.h src/sys_info_server.cpp include/sys_info_server_session.h src/sys_info_server_session.cpp src/repeat_timer.cpp include/repeat_timer.h include/client_config.h src/db_adapter.cpp include/db_adapter.h src/pg_db_adapter.cpp include/pg_db_adapter.h include/utils.h src/utils.cpp include/client_detail.h include/server_service.h src/server_service.cpp)
add_executable(sys_info_client ${CLIENT_SOURCES} include/logger.h include/boost_logger.h include/repeat_timer.h src/repeat_timer.cpp include/request.h include/sys_info_client_request.h src/sys_info_client_request.cpp include/utils.h src/utils.cpp)
add_executable(sys_info_tests ${TEST_SOURCES} test/sys_info_tests.cpp include/utils.h src/utils.cpp include/db_adapter.h include/pg_db_adapter.h src/db_adapter.cpp src/pg_db_adapter.cpp include/request.h include/sys_info_client_request.h src/sys_info_client_request.cpp include/sys_info_server.h src/sys_info_server.cpp include/sys_info_server_session.h src/sys_info_server_session.cpp include/server_service.h src/server_service.cpp)

if(Boost_FOUND)
    target_link_libraries(sys_info_server ${Boost_LIBRARIES})
    target_link_libraries(sys_info_client ${Boost_LIBRARIES})
    target_link_libraries(sys_info_tests ${Boost_LIBRARIES})
endif()

target_link_libraries(sys_info_server /usr/local/Cellar/libpqxx/4.0.1/lib/libpqxx.a -lpq)
target_link_libraries(sys_info_tests /usr/local/Cellar/libpqxx/4.0.1/lib/libpqxx.a -lpq)

## Set directory for library installation
#install(TARGETS sys_info_server DESTINATION ../bin)
#install(TARGETS sys_info_client DESTINATION ../bin)
