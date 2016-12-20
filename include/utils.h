#ifndef SYSINFOSERVER_UTILS_H
#define SYSINFOSERVER_UTILS_H

#include <string>
#include <map>
#include "client_config.h"
#include "client_detail.h"

/*
 * Function to return client details from server_config.xml
 * Returns a map with the client_key as the key and client_config structure as value
 */
std::map<std::string, client_config> get_authorized_clients_and_details(std::string from);

/*
 * Function to build a connection string from db_settings.xml
 */
std::string get_connection_string(std::string from);

/*
 * Function to generate test system info
 * Returns a string delimited by |
 */
std::string generate_random_sys_info_data(std::string client_key);

/*
 * Check if a client key is authorized to connect.
 */
bool is_client_authorized(std::map<std::string, client_config> &, std::string );

/*
 * Parses the client info and returns a structure of kind client_detail.
 * The string is delimited by |
 */
client_detail parse_to_client_detail(std::string );

/*
 * Generates an INSERT query for the client_detail struct.
 * Returns a string that can be executed immediately.
 */
std::string generate_insert_str_for(client_detail , std::string );
std::string generate_insert_str_for(client_detail );

/*
 * Based on client configuration, decide if to send an alert email
 */
bool should_send_email(std::map<std::string, client_config> &, std::string client_key, client_detail );

#endif //SYSINFOSERVER_UTILS_H
