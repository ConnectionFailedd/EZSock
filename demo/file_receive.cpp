#include "udp_socket.hpp"
#include <fstream>
#include <iostream>

int main() {
    auto local_socket = EZSock::UDPSocket();
    auto local_address = EZSock::SocketAddress_IPv4(AUTO_IPV4_ADDRESS, 10750);
    local_socket.bind(local_address);

    auto target_address = EZSock::SocketAddress_IPv4();

    auto file_out = std::ofstream();
    file_out.open("./test/test_received.png", std::ios::binary | std::ios::trunc | std::ios::out);
    if(!file_out.is_open()){
        std::cout << "Output file not open!" << std::endl;
        return 0;
    }

    auto packet_count = int(0);

    while(true){
        local_socket.receive(target_address);

        packet_count = *(int *)local_socket.get_buf_ref_const().get_buf_base();

        std::cout << (std::string("Packet ") + std::to_string(packet_count) + " received.").c_str() << std::endl;
        std::cout << "Message from <" << target_address << ">:" << std::endl;
        std::cout << local_socket.get_buf_ref_const() << std::endl;

        if(packet_count != -1){
            file_out.write((const char *)local_socket.get_buf_ref().get_buf_base() + 4, 508);
        }

        local_socket.get_buf_ref() = (std::string("Packet ") + std::to_string(packet_count) + " received.").c_str();
        local_socket.send(target_address);

        if(packet_count == -1) break;
    }

    local_socket.close();
}