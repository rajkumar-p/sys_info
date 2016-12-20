#ifndef SYSINFOSERVER_LOGGER_H
#define SYSINFOSERVER_LOGGER_H

#include <string>

enum logger_severity {
    trace, debug, info, warning, error, fatal
};

class logger {
public:
    logger();
    virtual ~logger();

    virtual void log_message(logger_severity severity_level, std::string message) = 0;
};

logger::logger() {}
logger::~logger() {}

#endif //SYSINFOSERVER_LOGGER_H
