#include "utils.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/random.hpp>

std::map<std::string, client_config> get_authorized_clients_and_details(std::string from)
{
    using boost::property_tree::ptree;
    ptree pt;

    read_xml(from, pt);
    std::map<std::string, client_config> authorized_clients_and_details;

    for (auto &client : pt.get_child("clients")) {
        if (client.first == "client") {
            client_config c_info;
            c_info.client_key = client.second.get<std::string>("key");
            c_info.email = client.second.get<std::string>("email");
            c_info.memory_limit = client.second.get_child("alerts").get<std::size_t>("memory");
            c_info.cpu_limit = client.second.get_child("alerts").get<std::size_t>("cpu");
            c_info.processes_limit = client.second.get_child("alerts").get<std::size_t>("processes");

            authorized_clients_and_details[c_info.client_key] = c_info;
        }
    }

    return authorized_clients_and_details;
};

std::string get_connection_string(std::string from)
{
    using boost::property_tree::ptree;
    ptree pt;

    read_xml(from, pt);

    std::string dbname = pt.get<std::string>("db_settings.dbname");
    std::string user = pt.get<std::string>("db_settings.user");
    std::string password = pt.get<std::string>("db_settings.password");
    std::string hostaddr = pt.get<std::string>("db_settings.hostaddr");
    std::string port = pt.get<std::string>("db_settings.port");

    return "dbname=" + dbname + " user=" + user + " password=" + password + " hostaddr=" + hostaddr + " port=" + port;
}

bool is_client_authorized(std::map<std::string, client_config> &authorized_clients, std::string client_key)
{
    return authorized_clients.find(client_key) != authorized_clients.end();
}

std::string generate_random_sys_info_data(std::string client_key)
{
    boost::random::mt19937 rng{ time(0) };
    boost::random::uniform_int_distribution<> percent(1, 100);

    int memory_percentage = percent(rng);
    int cpu_percentage = percent(rng);
    int no_of_processes = percent(rng);

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::string str_time = asctime(&tm);

    // De-limit the various data using the | delimiter
    std::string random_sys_info_line = client_key + "|" + std::to_string(memory_percentage) + "|"
                                     + std::to_string(cpu_percentage) + "|" + std::to_string(no_of_processes) + "|" + str_time;

    return random_sys_info_line;
}

client_detail parse_to_client_detail(std::string str_client_info)
{
    std::vector<std::string> info;
    boost::split(info, str_client_info, boost::is_any_of("|"));

    client_detail c_info;
    c_info.client_key = info[0];
    c_info.curr_memory_pct = atoi(info[1].c_str());
    c_info.curr_cpu_pct = atoi(info[2].c_str());
    c_info.curr_processes_cnt = atoi(info[3].c_str());

    boost::replace_all(info[4], "\n", "");
    c_info.str_dt_time = info[4];

    return c_info;
}

std::string generate_insert_str_for(client_detail c_detail, std::string table_name)
{
    std::string insert_query =
            "INSERT INTO " + table_name + " (CLIENT_KEY, MEMORY_PCT, CPU_PCT, NO_PROCESSES, DT_TIME) "
                    "VALUES ('" + c_detail.client_key + "', " + std::to_string(c_detail.curr_memory_pct) + ", " +
            std::to_string(c_detail.curr_cpu_pct) + ", " + std::to_string(c_detail.curr_processes_cnt) + ", '" + c_detail.str_dt_time + "');";

    return insert_query;
}

std::string generate_insert_str_for(client_detail c_detail)
{
    return generate_insert_str_for(c_detail, "CLIENT_INFO");
}

bool should_send_email(std::map<std::string, client_config> &authorized_clients, std::string client_key, client_detail c_detail)
{
    client_config c_config = authorized_clients[client_key];
    return c_detail.curr_memory_pct > c_config.memory_limit || c_detail.curr_cpu_pct > c_config.cpu_limit || c_detail.curr_processes_cnt > c_config.processes_limit;
}
