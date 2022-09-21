#include "udp_socket.hpp"

int main() {
    UDPSocket client_socket;
    client_socket.bind("0.0.0.0", 1234);

    UDPTarget server("175.24.226.74", 10750);

    Buffer buf;

    buf[0] = 'H';
    buf[1] = 'e';
    buf[2] = 'l';
    buf[3] = 'l';
    buf[4] = 'o';
    buf[5] = ',';
    buf[6] = ' ';
    buf[7] = 'w';
    buf[8] = 'o';
    buf[9] = 'r';
    buf[10] = 'l';
    buf[11] = 'd';
    buf[12] = '!';
    buf[13] = '\0';

    client_socket.send(server, buf);
    client_socket.receive(server);

    std::cout << server << std::endl;
    std::cout << client_socket.get_buf().get_buf_base() << std::endl;
}