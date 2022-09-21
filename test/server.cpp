#include "udp_socket.hpp"

int main() {
    UDPSocket server_socket;
    server_socket.bind("0.0.0.0", 10750);

    UDPTarget client;

    while(1){
        server_socket.receive(client);
        server_socket.send(client);

        std::cout << client << std::endl;
    }

    server_socket.close();
}