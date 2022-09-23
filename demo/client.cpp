/*
 * @file client.cpp
 * @author __NYA__
 * @version 0.1
 * @date 2022-09-24
 * 
 * @copyright Copyright (c) 2022 __NYA__
 * 
 */

#include "udp_socket.hpp"

int main() {
    auto client_socket = EZSock::UDPSocket();
    auto client_address = EZSock::SocketAddress_IPv4(AUTO_IPV4_ADDRESS, 8080);
    client_socket.bind(client_address);

    auto server_address = EZSock::SocketAddress_IPv4(EZSock::IPv4_Address::cstr_to_ipv4_address("175.24.226.74"), 10750);

    client_socket.get_buf_ref() = "Hello, World!";

    client_socket.send(server_address);
    client_socket.receive();

    std::cout << "Message from <" << server_address << ">:" << std::endl;
    std::cout << client_socket.get_buf().get_buf_base() << std::endl;

    client_socket.close();
}