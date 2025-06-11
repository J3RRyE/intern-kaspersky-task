#include "SocketUtils.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <stdexcept>

int create_tcp_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Failed to create socket");
    }
    return sock;
}

void connect_to_host(int sock, const std::string& host, int port) {
    struct addrinfo hints {};
    struct addrinfo* res = nullptr;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);

    int status = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res);
    if (status != 0) {
        throw std::runtime_error("getaddrinfo failed: " + std::string(gai_strerror(status)));
    }

    if (!res) {
        throw std::runtime_error("No address info found");
    }

    if (connect(sock, res->ai_addr, res->ai_addrlen) == -1) {
        freeaddrinfo(res);
        throw std::runtime_error("Connection failed");
    }

    freeaddrinfo(res);
}

void send_all(int sock, const std::string& data) {
    size_t total_sent = 0;
    while (total_sent < data.size()) {
        ssize_t sent = send(sock, data.data() + total_sent, data.size() - total_sent, 0);
        if (sent <= 0) {
            throw std::runtime_error("Send failed");
        }
        total_sent += sent;
    }
}

std::string receive_all(int sock) {
    char buffer[4096];
    std::string result;

    while (true) {
        ssize_t received = recv(sock, buffer, sizeof(buffer), 0);
        if (received > 0) {
            result.append(buffer, received);

            if (result.find('\n') != std::string::npos) {
                break;
            }
        } else if (received == 0) {
            throw std::runtime_error("Connection closed by peer");
        } else {
            throw std::runtime_error("Receive failed");
        }
    }

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

