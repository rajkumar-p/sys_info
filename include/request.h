#ifndef SYSINFOSERVER_REQUEST_H
#define SYSINFOSERVER_REQUEST_H

#include <string>

class request {
public:
    request();
    virtual ~request();

    virtual void connect(std::string, std::string) = 0;
    virtual std::size_t read(char *, std::size_t) = 0;
    virtual void write(std::string) = 0;
};

request::request() {}
request::~request() {}

#endif //SYSINFOSERVER_REQUEST_H
