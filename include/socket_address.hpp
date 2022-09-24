/*
 * @file socket_address.hpp
 * @author __NYA__
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright Copyright (c) 2022 __NYA__
 * 
 */

#ifndef __SOCKET_ADDRESS_HPP__
#define __SOCKET_ADDRESS_HPP__

#include <arpa/inet.h>
#include <sys/socket.h>
#include <iosfwd>

/* -------------------------------------------------------------------------------- */

namespace EZSock {

/* -------------------------------------------------------------------------------- */

    #define AUTO_IPV4_ADDRESS uint32_t(0)
    #define UNACCESSIBLE_PORT_NUMBER uint16_t(0)

/* -------------------------------------------------------------------------------- */

    /*
     * enum class SocketAddressFamily
     * 
     * to specify type of socket address.
     */
    enum class SocketAddressFamily : sa_family_t {
        UNSPEC          = AF_UNSPEC,
        INET            = AF_INET,
        INET6           = AF_INET6,
    };

/* -------------------------------------------------------------------------------- */

    // defined in udp_socket.hpp.
    class UDPSocket;

    /*
     * class SocketAddress
     * 
     * base class of all kinds of socket address classes.
     * parallel to struct sockaddr (built in).
     */
    class SocketAddress {
    private:
        // type of socket address
        SocketAddressFamily socket_address_family;

    protected:
        friend EZSock::UDPSocket;

        // to transfer from & to sockaddr struct (built in) explicitly.
        inline explicit SocketAddress(const sockaddr &) noexcept;
        inline explicit virtual operator sockaddr() const noexcept;

    public:
        // to initialize with socket address type.
        inline SocketAddress(SocketAddressFamily = SocketAddressFamily::UNSPEC) noexcept;

        SocketAddress(const SocketAddress &) = default;
        SocketAddress(SocketAddress &&) = default;
        ~SocketAddress() = default;
        SocketAddress & operator=(const SocketAddress &) = default;
        SocketAddress & operator=(SocketAddress &&) = default;

        // to set socket address type.
        inline void set_socket_address_family(SocketAddressFamily) noexcept;
        // to get socket address type.
        inline SocketAddressFamily get_socket_address_family() const noexcept;
    };

/* -------------------------------------------------------------------------------- */

    // some integer types.
    using IPv4_Address_t = uint32_t;
    using IPv4_Port_t = uint16_t;
    using IPv4_Port = IPv4_Port_t;

/* -------------------------------------------------------------------------------- */

    // defined below
    class SocketAddress_IPv4;

    /*
     * class IPv4_Address
     * 
     * encapsuled 32-bit unsigned integer.
     * can be transfered from & to 32-bit integer implicitly.
     */
    class IPv4_Address {
    private:
        // ipv4 address, stored locally aligned.
        IPv4_Address_t ipv4_address;

    private:
        friend EZSock::SocketAddress_IPv4;

        // to transfer from & to in_addr (built in) explicitly.
        inline explicit IPv4_Address(const in_addr &) noexcept;
        inline explicit operator in_addr() const noexcept;

    public:
        // to initialize with a 32-bit integer.
        inline IPv4_Address(IPv4_Address_t = IPv4_Address_t(0)) noexcept;

        IPv4_Address(const IPv4_Address &) = default;
        IPv4_Address(IPv4_Address &&) = default;
        ~IPv4_Address() = default;
        IPv4_Address & operator=(const IPv4_Address &) = default;
        IPv4_Address & operator=(IPv4_Address &&) = default;

        // to transfer to a 32-bit integer implicitly.
        inline operator IPv4_Address_t() const noexcept;

        // to get ip (32-bit integer) explicitly.
        inline IPv4_Address_t get() const noexcept;

        // to print as "xxx.xxx.xxx.xxx".
        inline friend std::ostream & operator<<(std::ostream &, const IPv4_Address &);

        // to transfer c string to ipv4 address.
        static IPv4_Address cstr_to_ipv4_address(const char *) noexcept;
    };

/* -------------------------------------------------------------------------------- */

    /*
     * class SocketAddress_IPv4
     * 
     * IPv4 address & port number
     */
    class SocketAddress_IPv4 : public SocketAddress {
    private:
        // ip & port.
        IPv4_Address ip;
        IPv4_Port port;

    private:
        friend class EZSock::UDPSocket;

        // to transfer from & to sockaddr & sockaddr_in struct (built in) explicitly.
        inline explicit SocketAddress_IPv4(const sockaddr &) noexcept;
        inline explicit virtual operator sockaddr() const noexcept override final;

    public:
        // to initialize with ip & port.
        inline SocketAddress_IPv4(const IPv4_Address & = AUTO_IPV4_ADDRESS, const IPv4_Port & = UNACCESSIBLE_PORT_NUMBER) noexcept;

        SocketAddress_IPv4(const SocketAddress_IPv4 &) = default;
        SocketAddress_IPv4(SocketAddress_IPv4 &&) = default;
        ~SocketAddress_IPv4() = default;
        SocketAddress_IPv4 & operator=(const SocketAddress_IPv4 &) = default;
        SocketAddress_IPv4 & operator=(SocketAddress_IPv4 &&) = default;

        // socket address type is fixed to SocketAddressFamily::INET.
        void set_socket_address_family() = delete;

        // to change ip address.
        inline void set_ipv4_address(const IPv4_Address &) noexcept;
        // to change ip address.
        inline void set_ipv4_address(const char *) noexcept;
        // to change port number.
        inline void set_ipv4_port(const IPv4_Port &) noexcept;

        // to get ip address.
        inline IPv4_Address get_ipv4_address() const noexcept;
        // to get port number.
        inline IPv4_Port get_ipv4_port() const noexcept;

        // to print as "xxx.xxx.xxx.xxx:xxxx"
        friend std::ostream & operator<<(std::ostream &, const SocketAddress_IPv4 &);
    };

/* -------------------------------------------------------------------------------- */

    // implements of inline functions.

/* -------------------------------------------------------------------------------- */

    // SocketAddress

    inline SocketAddress::SocketAddress(const sockaddr & src) noexcept : socket_address_family(SocketAddressFamily(src.sa_family)) {}

    inline SocketAddress::operator sockaddr() const noexcept {
        auto res = sockaddr();
        res.sa_family = sa_family_t(socket_address_family);

        return res;
    }

    inline SocketAddress::SocketAddress(SocketAddressFamily src_socket_address_family) noexcept : socket_address_family(src_socket_address_family) {}

    inline void SocketAddress::set_socket_address_family(SocketAddressFamily src_socket_address_family) noexcept {
        socket_address_family = src_socket_address_family;
    }

    inline SocketAddressFamily SocketAddress::get_socket_address_family() const noexcept {
        return socket_address_family;
    }

/* -------------------------------------------------------------------------------- */

    // IPv4_Address

    inline IPv4_Address::IPv4_Address(const in_addr & src) noexcept {
        ipv4_address = ntohl(src.s_addr);
    }

    inline IPv4_Address::operator in_addr() const noexcept {
        auto res = in_addr();
        res.s_addr = htonl(ipv4_address);

        return res;
    }

    inline IPv4_Address::IPv4_Address(IPv4_Address_t src_ipv4_address) noexcept : ipv4_address(src_ipv4_address) {}

    inline IPv4_Address::operator IPv4_Address_t() const noexcept {
        return ipv4_address;
    }

    inline IPv4_Address_t IPv4_Address::get() const noexcept {
        return ipv4_address;
    }

    inline IPv4_Address IPv4_Address::cstr_to_ipv4_address(const char * src_ipv4_str) noexcept {
        return ntohl(inet_addr(src_ipv4_str));
    }

/* -------------------------------------------------------------------------------- */

    // SocketAddress_IPv4

    inline SocketAddress_IPv4::SocketAddress_IPv4(const sockaddr & src) noexcept : SocketAddress(SocketAddressFamily::INET) {
        const auto * sockaddr_in_ptr = (sockaddr_in *)&src;

        ip = IPv4_Address(sockaddr_in_ptr->sin_addr);
        port = IPv4_Port(htons(sockaddr_in_ptr->sin_port));
    }

    inline SocketAddress_IPv4::operator sockaddr() const noexcept {
        auto res = sockaddr();

        auto sockaddr_in_ptr = (sockaddr_in *)&res;

        sockaddr_in_ptr->sin_family = sa_family_t(get_socket_address_family());
        sockaddr_in_ptr->sin_addr = in_addr(get_ipv4_address());
        sockaddr_in_ptr->sin_port = htons(get_ipv4_port());

        return res;
    }

    inline SocketAddress_IPv4::SocketAddress_IPv4(const IPv4_Address & src_ip, const IPv4_Port & src_port) noexcept : SocketAddress(SocketAddressFamily::INET), ip(src_ip), port(src_port) {}

    inline void SocketAddress_IPv4::set_ipv4_address(const IPv4_Address & src_ip) noexcept {
        ip = src_ip;
    }

    inline void SocketAddress_IPv4::set_ipv4_address(const char * src_ip_str) noexcept {
        ip = ntohl(inet_addr(src_ip_str));
    }

    inline void SocketAddress_IPv4::set_ipv4_port(const IPv4_Port & src_port) noexcept {
        port = src_port;
    }

    inline IPv4_Address SocketAddress_IPv4::get_ipv4_address() const noexcept {
        return ip;
    }

    inline IPv4_Port SocketAddress_IPv4::get_ipv4_port() const noexcept {
        return port;
    }

/* -------------------------------------------------------------------------------- */

}

/* -------------------------------------------------------------------------------- */

#endif