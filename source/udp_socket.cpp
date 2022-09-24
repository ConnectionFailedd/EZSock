/*
 * @file udp_socket.cpp
 * @author __NYA__
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright Copyright (c) 2022 __NYA__
 * 
 */

#include "udp_socket.hpp"
#include <iostream>

/* -------------------------------------------------------------------------------- */

// utilities

// to fill in sockaddr_in struct with all 0.
static void in_addr_clear(sockaddr_in * sa_ptr) noexcept {
    auto sa_base = (uint8_t *)sa_ptr;
    for(int i = 0; i < sizeof(sockaddr_in); i ++){
        *(sa_base + i) = 0;
    }
}

/* -------------------------------------------------------------------------------- */

namespace EZSock {

/* -------------------------------------------------------------------------------- */

    // UDPSocket

    int UDPSocket::bind(const SocketAddress_IPv4 & address) {
        if(is_active) return -1;

        is_active = true;

        socket_address_ipv4 = address;

        auto sockaddr_tmp = sockaddr();
        auto sockaddr_ptr = &sockaddr_tmp;
        auto sockaddr_in_ptr = (sockaddr_in *)sockaddr_ptr;

        in_addr_clear(sockaddr_in_ptr);
        sockaddr_in_ptr->sin_addr.s_addr = htonl(IPv4_Address_t(address.get_ipv4_address()));
        sockaddr_in_ptr->sin_port = htons(address.get_ipv4_port());
        sockaddr_in_ptr->sin_family = std::underlying_type_t<SocketAddressFamily>(SocketAddressFamily::INET);

        return ::bind(socket, sockaddr_ptr, sizeof(sockaddr));
    }

    inline std::ostream & operator<<(std::ostream & ost, const UDPSocket & udp_socket) {
        ost << udp_socket.socket << " - " << udp_socket.get_socket_address() << " , ";

        if(udp_socket.is_active) return ost << "active";
        return ost << "closed";
    }

/* -------------------------------------------------------------------------------- */

}

/* -------------------------------------------------------------------------------- */