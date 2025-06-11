#include "Client.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: client <host> <port>\n";
        return 1;
    }

    std::string host = argv[1];
    int port = std::atoi(argv[2]);

    Client client(host, port);
    client.run();
}
