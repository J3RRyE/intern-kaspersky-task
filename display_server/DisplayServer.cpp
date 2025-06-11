#include "DisplayServer.hpp"
#include "../common/SocketUtils.hpp"
#include <iostream>
#include <thread>
#include <mutex>

static std::mutex cout_mutex;

void DisplayServer::start() {
    int listen_sock = create_tcp_socket();

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(listen_sock, SOMAXCONN) == -1) {
        throw std::runtime_error("Listen failed");
    }

    std::cout << "Display server started on port " << port << std::endl;

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock >= 0) {
            std::thread([client_sock]() {
                try {
                    std::string result = receive_all(client_sock);
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "[RESULT] " << result << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "[Display Error] " << e.what() << std::endl;
                }
                close(client_sock);
            }).detach();
        }
    }

    close(listen_sock);
}
