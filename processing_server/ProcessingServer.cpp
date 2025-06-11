#include "ProcessingServer.hpp"
#include "../common/SocketUtils.hpp"
#include "../common/Utils.h"
#include <iostream>
#include <thread>
#include <vector>

ProcessingServer::ProcessingServer(int port, const std::string& display_host, int display_port)
    : port_(port), display_host_(display_host), display_port_(display_port) {}

void ProcessingServer::handle_client(int client_sock) {
    try {
        while (true) {
            std::string data = receive_all(client_sock);
            if (data.empty()) {
                break;
            }

            std::cout << "[Processing] Received: " << data << std::endl;
            send_all(client_sock, "Processed\n");

            std::string processed = remove_duplicates(data);

            int display_sock = create_tcp_socket();
            connect_to_host(display_sock, display_host_, display_port_);
            send_all(display_sock, processed + '\n');
            close(display_sock);
        }

        close(client_sock);
    } catch (const std::exception& e) {
        std::cerr << "[Processing Error] " << e.what() << std::endl;
        close(client_sock);
    }
}

void ProcessingServer::start() {
    int listen_sock = create_tcp_socket();

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(listen_sock, SOMAXCONN) == -1) {
        throw std::runtime_error("Listen failed");
    }

    std::cout << "Processing server started on port " << port_ << std::endl;

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock >= 0) {
            std::thread(&ProcessingServer::handle_client, this, client_sock).detach();
        }
    }

    close(listen_sock);
}
