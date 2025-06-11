#pragma once

#include <string>
#include <memory>
#include <thread>

class ProcessingServer {
public:
    explicit ProcessingServer(int port, const std::string& display_host, int display_port);
    void start();

private:
    int port_;
    std::string display_host_;
    int display_port_;

    void handle_client(int client_sock);
};
