#pragma once

#include <string>

class Client {
public:
    Client(const std::string& host, int port);
    void run();

private:
    std::string host_;
    int port_;
};
