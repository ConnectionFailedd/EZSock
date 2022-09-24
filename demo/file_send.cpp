#include "udp_socket.hpp"
#include <fstream>
#include <iostream>

int main() {
    auto local_socket = EZSock::UDPSocket();
    auto local_address = EZSock::SocketAddress_IPv4(AUTO_IPV4_ADDRESS, 8080);
    local_socket.bind(local_address);

    auto target_address = EZSock::SocketAddress_IPv4(EZSock::IPv4_Address::cstr_to_ipv4_address("175.24.226.74"), 10750);

    auto file_in = std::ifstream();
    file_in.open("./test/test.png", std::ios::binary | std::ios::in);
    if(!file_in.is_open()){
        std::cout << "File not found!" << std::endl;
        return 0;
    }

    auto packet_count = int(0);

    while(!file_in.eof()){
        file_in.read((char *)local_socket.get_buf_ref().get_buf_base() + 4, 508);

        *(int *)&(local_socket.get_buf_ref()[0]) = packet_count;

        local_socket.send(target_address);
        local_socket.receive();

        std::cout << "Message from <" << target_address << ">: " << local_socket.get_buf_ref_const().get_buf_base() << std::endl;

        packet_count ++;
    }

    *(int *)&(local_socket.get_buf_ref()[0]) = -1;
    local_socket.send(target_address);
    local_socket.receive();

    std::cout << "Message from <" << target_address << ">: " << local_socket.get_buf_ref_const().get_buf_base() << std::endl;

    std::cout << "File transportation finished." << std::endl;

    local_socket.close();
}