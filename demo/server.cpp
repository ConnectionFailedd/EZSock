/*
 * @file server.cpp
 * @author __NYA__
 * @version 0.1
 * @date 2022-09-24
 * 
 * @copyright Copyright (c) 2022 __NYA__
 * 
 */

#include "udp_socket.hpp"

int main() {
    auto server_socket = EZSock::UDPSocket();
    auto server_address = EZSock::SocketAddress_IPv4(AUTO_IPV4_ADDRESS, 10750);
    server_socket.bind(server_address);

    auto client_address = EZSock::SocketAddress_IPv4();

    while(1){
        server_socket.receive(client_address);
        server_socket.send(client_address);

        std::cout << "Message from: <" << client_address << ">:" << std::endl;
        std::cout << server_socket.get_buf().get_buf_base() << std::endl;
    }

    server_socket.close();
}