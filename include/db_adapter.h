#ifndef SYSINFOSERVER_DB_ADAPTER_H
#define SYSINFOSERVER_DB_ADAPTER_H


#include <string>

class db_adapter {
public:
    db_adapter();
    virtual ~db_adapter();

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void execute(std::string ) = 0;
    virtual bool test_connection() = 0;
};


#endif //SYSINFOSERVER_DB_ADAPTER_H
