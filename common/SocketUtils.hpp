#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <stdexcept>

int create_tcp_socket();
void connect_to_host(int sock, const std::string& host, int port);
void send_all(int sock, const std::string& data);
std::string receive_all(int sock);
