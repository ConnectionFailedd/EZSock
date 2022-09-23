#include "udp_socket.hpp"

int main() {
    auto client_socket = EZSock::UDPSocket();
    auto client_address = EZSock::SocketAddress_IPv4("127.0.0.1", 9090);
    client_socket.bind(client_address);

    auto server_address = EZSock::SocketAddress_IPv4("127.0.0.1", 10850);

    client_socket.get_buf_ref() = "Hello, World!";

    client_socket.send(server_address);
    client_socket.receive();

    std::cout << "Message from <" << server_address << ">:" << std::endl;
    std::cout << client_socket.get_buf().get_buf_base() << std::endl;

    client_socket.close();
}