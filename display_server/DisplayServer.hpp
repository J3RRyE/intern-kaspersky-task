#pragma once

#include <string>

class DisplayServer {
    int port;
public:
    explicit DisplayServer(int port) : port(port) {}
    void start();
};
