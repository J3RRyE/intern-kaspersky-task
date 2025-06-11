#include "Client.hpp"
#include "../common/SocketUtils.hpp"
#include <iostream>

Client::Client(const std::string& host, int port)
    : host_(host), port_(port) {}

void Client::run() {
    try {
        int sock = create_tcp_socket();
        connect_to_host(sock, host_, port_);

        std::cout << "Connected to server. Type 'exit' to quit.\n";

        std::string line;
        while (true) {
            std::getline(std::cin, line);
            if (line == "exit") {
                break;
            }

            send_all(sock, line + "\n");

            std::string ack = receive_all(sock);
            std::cout << "[ACK] " << ack << std::endl;
        }

        close(sock);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}
