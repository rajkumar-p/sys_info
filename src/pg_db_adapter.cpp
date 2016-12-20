#include <iostream>
#include "pg_db_adapter.h"

// INCOMPLETE. HACK
pg_db_adapter::pg_db_adapter(std::string connection_string)
    : connection_string(connection_string), pg_connection(connection_string)
{
    std::cout <<"pg_db_adapter constructed."<<std::endl;
}

pg_db_adapter::~pg_db_adapter()
{
    std::cout <<"pg_db_adapter destructor. Disconnecting."<<std::endl;
    pg_connection.disconnect();
}

void pg_db_adapter::connect()
{

}

void pg_db_adapter::disconnect()
{

}

bool pg_db_adapter::test_connection()
{
    return true;
}

bool pg_db_adapter::query_returned_row(std::string query)
{
    pqxx::nontransaction N(pg_connection);
    pqxx::result R(N.exec(query));
    return !R.empty();
}

void pg_db_adapter::execute(std::string query)
{
    pqxx::nontransaction N(pg_connection);
    pqxx::result R(N.exec(query));
}
