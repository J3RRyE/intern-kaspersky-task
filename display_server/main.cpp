#include "DisplayServer.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: display_server <port>\n";
        return 1;
    }

    int port = std::atoi(argv[1]);
    DisplayServer server(port);
    server.start();

    return 0;
}
