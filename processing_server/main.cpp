#include "ProcessingServer.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: processing_server <port> <display-host> <display-port>\n";
        return 1;
    }

    int port = std::atoi(argv[1]);
    std::string display_host = argv[2];
    int display_port = std::atoi(argv[3]);

    ProcessingServer server(port, display_host, display_port);
    server.start();

    return 0;
}
