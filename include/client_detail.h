#ifndef SYSINFOSERVER_CLIENT_DETAIL_H
#define SYSINFOSERVER_CLIENT_DETAIL_H

#include <string>

struct client_detail {
    std::string client_key;
    std::size_t curr_memory_pct;
    std::size_t curr_cpu_pct;
    std::size_t curr_processes_cnt;
    std::string str_dt_time;
};

#endif //SYSINFOSERVER_CLIENT_DETAIL_H
