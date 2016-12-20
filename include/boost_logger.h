#ifndef SYSINFOSERVER_BOOST_LOGGER_H
#define SYSINFOSERVER_BOOST_LOGGER_H

#include "logger.h"
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

class boost_logger : public  logger
{
public:
    boost_logger(std::string file);
    void log_message(logger_severity severity_level, std::string message);
};

boost_logger::boost_logger(std::string file)
        : logger()
{
    // Logger configurations
    boost::log::add_common_attributes();
    boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");

    boost::log::add_file_log
    (
        boost::log::keywords::file_name = file,
        boost::log::keywords::auto_flush = true,
        boost::log::keywords::format = "[%Severity%] %TimeStamp% [%Uptime%] (%LineID%) : %Message%"
    );

    // Log file location is printed out
    std::cout <<"Log file location : "<<file<<std::endl;
}

// Log based on severity_level
void boost_logger::log_message(logger_severity severity_level, std::string message)
{
    switch (severity_level) {
        case logger_severity::trace:
        {
            BOOST_LOG_TRIVIAL(trace) <<message;
            break;
        }
        case logger_severity::debug:
        {
            BOOST_LOG_TRIVIAL(debug) <<message;
            break;
        }
        case logger_severity::info:
        {

            BOOST_LOG_TRIVIAL(info) <<message;
            break;
        }
        case logger_severity::warning:
        {

            BOOST_LOG_TRIVIAL(warning) <<message;
            break;
        }
        case logger_severity::error:
        {

            BOOST_LOG_TRIVIAL(error) <<message;
            break;
        }
        case logger_severity::fatal:
        {

            BOOST_LOG_TRIVIAL(fatal) <<message;
            break;
        }
    }

}

#endif //SYSINFOSERVER_BOOST_LOGGER_H
