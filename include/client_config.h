#ifndef SYSINFOSERVER_CLIENT_INFO_H
#define SYSINFOSERVER_CLIENT_INFO_H

#include <string>

struct client_config {
    std::string client_key;
    std::string email;
    std::size_t memory_limit;
    std::size_t cpu_limit;
    std::size_t processes_limit;
};

#endif //SYSINFOSERVER_CLIENT_INFO_H
