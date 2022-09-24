/*
 * @file socket_address.cpp
 * @author __NYA__
 * @version 0.1
 * @date 2022-09-23
 * 
 * @copyright Copyright (c) 2022 __NYA__
 * 
 */

#include "socket_address.hpp"
#include <iostream>

/* -------------------------------------------------------------------------------- */

namespace EZSock {

/* -------------------------------------------------------------------------------- */

    // IPv4_Address

    std::ostream & operator<<(std::ostream & ost, const IPv4_Address & src) {
        return ost << inet_ntoa(in_addr(src));
    }

/* -------------------------------------------------------------------------------- */

    // SocketAddress_IPv4

    std::ostream & operator<<(std::ostream & ost, const SocketAddress_IPv4 & socket_address_ipv4) {
        return ost << "IPv4@" << socket_address_ipv4.ip << ":" << socket_address_ipv4.port;
    }

}

/* -------------------------------------------------------------------------------- */