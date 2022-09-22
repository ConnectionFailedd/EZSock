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
#include <iostream>

/* -------------------------------------------------------------------------------- */

namespace EZSock {

/* -------------------------------------------------------------------------------- */

    // defined in udp_socket.hpp
    class UDPSocket;

/* -------------------------------------------------------------------------------- */

    /*
     * enum class SocketAddressFamily
     * 
     * to specify type of socket address.
     */
    enum class SocketAddressFamily : sa_family_t {
        UNSPEC          = AF_UNSPEC,
        UNIX            = AF_UNIX,
        LOCAL           = AF_LOCAL,
        INET            = AF_INET,
        IMPLINK         = AF_IMPLINK,
        PUP             = AF_PUP,
        CHAOS           = AF_CHAOS,
        NS              = AF_NS,
        ISO             = AF_ISO,
        OSI             = AF_OSI,
        ECMA            = AF_ECMA,
        DATAKIT         = AF_DATAKIT,
        CCITT           = AF_CCITT,
        SNA             = AF_SNA,
        DECnet          = AF_DECnet,
        DLI             = AF_DLI,
        LAT             = AF_LAT,
        HYLINK          = AF_HYLINK,
        APPLETALK       = AF_APPLETALK,
        ROUTE           = AF_ROUTE,
        LINK            = AF_LINK,
        pseudo_XTP      = pseudo_AF_XTP,
        COIP            = AF_COIP,
        CNT             = AF_CNT,
        pseudo_RTIP     = pseudo_AF_RTIP,
        IPX             = AF_IPX,
        SIP             = AF_SIP,
        pseudo_PIP      = pseudo_AF_PIP,
        NDRV            = AF_NDRV,
        ISDN            = AF_ISDN,
        E164            = AF_E164,
        pseudo_KEY      = pseudo_AF_KEY,
        INET6           = AF_INET6,
        NATM            = AF_NATM,
        SYSTEM          = AF_SYSTEM,
        NETBIOS         = AF_NETBIOS,
        PPP             = AF_PPP,
        pseudo_HDRCMPLT = pseudo_AF_HDRCMPLT,
        RESERVED_36     = AF_RESERVED_36,
        IEEE80211       = AF_IEEE80211,
        UTUN            = AF_UTUN,
        VSOCK           = AF_VSOCK,
        MAX             = AF_MAX,
    };

/* -------------------------------------------------------------------------------- */

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
        inline SocketAddress(const sockaddr &) noexcept;
        inline explicit virtual operator sockaddr() const noexcept;

    public:
        inline SocketAddress(SocketAddressFamily = SocketAddressFamily::INET) noexcept;

        SocketAddress(const SocketAddress &) = default;
        SocketAddress(SocketAddress &&) = default;
        ~SocketAddress() = default;
        SocketAddress & operator=(const SocketAddress &) = default;
        SocketAddress & operator=(SocketAddress &&) = default;

        inline void set_socket_address_family(SocketAddressFamily) noexcept;
        inline SocketAddressFamily get_socket_address_family() const noexcept;
    };

/* -------------------------------------------------------------------------------- */

    // some integer types.
    using IPv4_Address_t = uint32_t;
    using IPv4_Port_t = uint16_t;
    using IPv4_Port = IPv4_Port_t;

/* -------------------------------------------------------------------------------- */
    /*
     * class IPv4_Address
     * 
     * encapsuled 32 bits unsigned integer.
     */
    class IPv4_Address {
    private:
        // ipv4 address, stored locally aligned.
        in_addr_t ipv4_address;

    private:
        friend EZSock::SocketAddress_IPv4;
        friend std::ostream & operator<<(std::ostream &, const UDPSocket &);
        inline IPv4_Address(const in_addr &);
        inline explicit operator in_addr() const noexcept;

    public:
        inline IPv4_Address(IPv4_Address_t = INADDR_ANY) noexcept;
        inline IPv4_Address(const char *);

        IPv4_Address(const IPv4_Address &) = default;
        IPv4_Address(IPv4_Address &&) = default;
        ~IPv4_Address() = default;
        IPv4_Address & operator=(const IPv4_Address &) = default;
        IPv4_Address & operator=(IPv4_Address &&) = default;

        inline explicit operator IPv4_Address_t() const noexcept;

        inline IPv4_Address_t get() const noexcept;

        inline friend std::ostream & operator<<(std::ostream &, IPv4_Address);
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
        friend EZSock::UDPSocket;
        inline SocketAddress_IPv4(const sockaddr &) noexcept;
        inline SocketAddress_IPv4(const sockaddr_in &) noexcept;
        inline explicit virtual operator sockaddr() const noexcept override final;
        inline explicit operator sockaddr_in() const noexcept;

    public:
        inline SocketAddress_IPv4(IPv4_Address = INADDR_ANY, IPv4_Port = 8080) noexcept;
        inline SocketAddress_IPv4(const char *, IPv4_Port = 8080);

        SocketAddress_IPv4(const SocketAddress_IPv4 &) = default;
        SocketAddress_IPv4(SocketAddress_IPv4 &&) = default;
        ~SocketAddress_IPv4() = default;
        SocketAddress_IPv4 & operator=(const SocketAddress_IPv4 &) = default;
        SocketAddress_IPv4 & operator=(SocketAddress_IPv4 &&) = default;

        void set_sa_family() = delete;

        inline void set_ipv4_address(IPv4_Address_t) noexcept;
        inline void set_ipv4_address(const char *) noexcept;
        inline void set_ipv4_port(IPv4_Port_t) noexcept;

        inline IPv4_Address get_ipv4_address() const noexcept;
        inline IPv4_Port get_ipv4_port() const noexcept;

        inline friend std::ostream operator<<(std::ostream &, const SocketAddress_IPv4 &);
    };

/* -------------------------------------------------------------------------------- */

}

/* -------------------------------------------------------------------------------- */

#endif