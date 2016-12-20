#ifndef SYSINFOSERVER_PG_DB_ADAPTER_H
#define SYSINFOSERVER_PG_DB_ADAPTER_H


#include "db_adapter.h"
#include <pqxx/pqxx>

class pg_db_adapter : public db_adapter {
public:
    pg_db_adapter(std::string );
    ~pg_db_adapter();

    void connect();
    void disconnect();
    void execute(std::string );
    bool test_connection();

    bool query_returned_row(std::string );

private:
    std::string connection_string;
    pqxx::connection pg_connection;
};


#endif //SYSINFOSERVER_PG_DB_ADAPTER_H
