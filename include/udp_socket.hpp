/*
 * @file udp_socket.hpp
 * @author __NYA__
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright Copyright (c) 2022 __NYA__
 * 
 */

#ifndef __UDP_SOCKET_HPP__
#define __UDP_SOCKET_HPP__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include "buffer.hpp"
#include "socket_address.hpp"

/* -------------------------------------------------------------------------------- */

namespace EZSock {

/* -------------------------------------------------------------------------------- */

    // UDPSocket

    class UDPSocket {
    private:
        int socket;
        SocketAddress_IPv4 socket_address_ipv4;

        bool is_active;

        Buffer buffer;

    public:
        // to initialize with an optional parameter as size of buffer.
        inline UDPSocket(size_t = 512);
        inline ~UDPSocket();

        // explicitly ban copy and move ctors to keep consistency.
        UDPSocket(const UDPSocket &) = delete;
        UDPSocket(UDPSocket &&) = delete;
        UDPSocket & operator=(const UDPSocket &) = delete;
        UDPSocket & operator=(UDPSocket &&) = delete;

        // to bind with a socket address.
        int bind(const SocketAddress_IPv4 &);
        // to close socket.
        inline int close();

        // to send buffer built in to target.
        inline ssize_t send(const SocketAddress_IPv4 &) const;
        // to send specified buffer to target.
        inline ssize_t send(const SocketAddress_IPv4 &, const Buffer &) const;
        // to receive datagram from target and store in buffer built in.
        inline ssize_t receive(SocketAddress_IPv4 &) const;

        // to get socket.
        inline int get_socket() const noexcept;
        // to get binded socket address.
        inline SocketAddress_IPv4 get_socket_address() const noexcept;
        // to get status (true : active, false : closed).
        inline bool get_status() const noexcept;
        // to get buffer to read.
        inline Buffer get_buf() const noexcept;
        // to get reference of buffer for read/write.
        inline Buffer & get_buf_ref() noexcept;

        // to print as "<socket> - IPv4 @ xxx.xxx.xxx.xxx:xxxx , <is_active>"
        inline friend std::ostream & operator<<(std::ostream &, const UDPSocket &);
    };

/* -------------------------------------------------------------------------------- */

    // implements of inline functions below.

/* -------------------------------------------------------------------------------- */

    // UDPSocket

    inline UDPSocket::UDPSocket(size_t buf_size) : socket(::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)), socket_address_ipv4(), is_active(false), buffer(buf_size) {}

    inline UDPSocket::~UDPSocket() {
        if(is_active) close();
    }

    inline int UDPSocket::close() {
        auto res = ::close(socket);
        is_active = res == 0 ? false : true;

        return res;
    }

    inline ssize_t UDPSocket::send(const SocketAddress_IPv4 & target) const {
        return ::sendto(socket, buffer.get_buf_base(), buffer.get_buf_size(), 0, &sockaddr(target), sizeof(sockaddr));
    }

    inline ssize_t UDPSocket::send(const SocketAddress_IPv4 & target, const Buffer & src_buf) const {
        return ::sendto(socket, src_buf.get_buf_base(), src_buf.get_buf_size(), 0, &sockaddr(target), sizeof(sockaddr));
    }

    inline ssize_t UDPSocket::receive(SocketAddress_IPv4 & target) const {
        auto sockaddr_tmp = sockaddr();
        auto sockaddr_ptr = &sockaddr_tmp;
        auto socklen_tmp = socklen_t();
        auto socklen_ptr = &socklen_tmp;

        auto res = ::recvfrom(socket, buffer.get_buf_base(), buffer.get_buf_size(), 0, sockaddr_ptr, socklen_ptr);

        target = sockaddr_tmp;

        return res;
    }

    inline int UDPSocket::get_socket() const noexcept {
        return socket;
    }

    inline SocketAddress_IPv4 UDPSocket::get_socket_address() const {
        return socket_address_ipv4;
    }

    inline bool UDPSocket::get_status() const noexcept {
        return is_active;
    }

    inline Buffer UDPSocket::get_buf() const noexcept {
        return buffer;
    }

    inline Buffer & UDPSocket::get_buf_ref() noexcept {
        return buffer;
    }

    inline std::ostream & operator<<(std::ostream & ost, const UDPSocket & udp_socket) {
        ost << udp_socket.socket << " - IPv4 @ ";
        ost << inet_ntoa(in_addr(udp_socket.get_socket_address().get_ipv4_address())) << ":";
        ost << udp_socket.get_socket_address().get_ipv4_port() << " , ";

        if(udp_socket.is_active) return ost << "active";
        return ost << "closed";
    }

/* -------------------------------------------------------------------------------- */

}

/* -------------------------------------------------------------------------------- */

#endif