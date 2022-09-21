#ifndef __UDP_SOCKET_HPP__
#define __UDP_SOCKET_HPP__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

/* -------------------------------------------------------------------------------- */

// utilities

// to print ip as "xxx.xxx.xxx.xxx".
inline std::ostream & operator<<(std::ostream &, const in_addr &);

// to transfer in_addr_t (uint32_t) to in_addr.
inline in_addr ui_to_in_addr(in_addr_t);

/* -------------------------------------------------------------------------------- */

// Buffer

class Buffer{
private:
    size_t buf_size;
    uint8_t * buf_base;

public:
    // to initialize buffer with a default or specified size (8 bits per unit).
    inline Buffer(size_t = 512);
    // to copy deeply.
    Buffer(const Buffer & );
    // to get control of memory from another Buffer instance.
    inline Buffer(Buffer &&) noexcept;
    // to destruct.
    inline ~Buffer();

    // to copy deeply.
    Buffer & operator=(const Buffer & );
    // to get control of memory from another Buffer instance.
    inline Buffer & operator=(Buffer &&) noexcept;

    // to modify contents in buffer.
    // if parameter ranges over, return reference of the last element.
    inline uint8_t & operator[](size_t) const noexcept;

    // to get size.
    inline size_t get_buf_size() const noexcept;
    // to get base address of buffer.
    inline uint8_t * get_buf_base() const noexcept;

    // to assign with a c string.
    const Buffer & operator=(const char *) const noexcept;

    // to copy memory.
    template<typename T>
    size_t copy(const T *, size_t) const noexcept;
    template<typename T, size_t array_size>
    size_t copy(const T (& ) [array_size]) const noexcept;

    // to print as string.
    friend std::ostream & operator<<(std::ostream & , const Buffer & );
};

/* -------------------------------------------------------------------------------- */

// UDPSocket

class UDPTarget;

class UDPSocket{
private:
    int socket;
    sockaddr_in socket_addr;

    bool is_active;

    Buffer buffer;

public:
    // to initialize with an optional parameter as size of buffer (8 bits per unit).
    UDPSocket(size_t = 512);
    inline ~UDPSocket();

    // explicitly ban copy and move ctors to keep consistency.
    UDPSocket(const UDPSocket & ) = delete;
    UDPSocket(UDPSocket &&) = delete;
    UDPSocket & operator=(const UDPSocket & ) = delete;
    UDPSocket & operator=(UDPSocket &&) = delete;

    // to bind with an ip (default 0.0.0.0) and a port (default 8080).
    int bind(const char * = "0.0.0.0", in_port_t = 8080);
    // to close socket.
    int close();

    // to send buffer built in to target.
    inline ssize_t send(const UDPTarget & ) const;
    // to send specified buffer to target.
    inline ssize_t send(const UDPTarget & , const Buffer & ) const;
    // to receive datagram from target and store in buffer built in.
    inline ssize_t receive(UDPTarget & );

    // to get socket.
    inline int get_socket() const noexcept;
    // to get binded ip.
    inline in_addr_t get_ip() const noexcept;
    // to get binded port.
    inline in_port_t get_port() const noexcept;
    // to get status (true : active, false closed).
    inline bool get_status() const noexcept;
    // to get buffer for read/write.
    inline const Buffer & get_buf() const noexcept;

    // to print as "xxx.xxx.xxx.xxx:xxxx, is_active"
    inline friend std::ostream & operator<<(std::ostream & , const UDPSocket & );
};

/* -------------------------------------------------------------------------------- */

// UDPTarget

class UDPTarget{
    friend UDPSocket;

private:
    sockaddr_in target_addr;
    socklen_t target_sock_len;

public:
    // to initialize with ip address and port.
    UDPTarget(const char * = "127.0.0.1", in_port_t = 8080);
    // to initialize with ip address and port.
    UDPTarget(in_addr_t, in_port_t = 8080);
    // to destruct.
    inline ~UDPTarget();

    // to change target ip address and port.
    inline void set_addr(const char *, in_port_t);
    // to change target ip address and port.
    inline void set_addr(in_addr_t, in_port_t) noexcept;

    // to get ip address as unsigned int (32 bits).
    inline in_addr_t get_ip() const noexcept;
    // to get port number as unsigned short (16 bits).
    inline in_port_t get_port() const noexcept;

    // to print as "xxx.xxx.xxx.xxx:xxxx".
    inline friend std::ostream & operator<<(std::ostream & , const UDPTarget & );
};

/* -------------------------------------------------------------------------------- */

// implements of inline functions below.

/* -------------------------------------------------------------------------------- */

// utilities

// to print ip as "xxx.xxx.xxx.xxx".
inline std::ostream & operator<<(std::ostream & ost, const in_addr & src) {
    return ost << inet_ntoa(src);
}

// to transfer in_addr_t (uint32_t) to in_addr.
inline in_addr ui_to_in_addr(in_addr_t src) {
    return in_addr({htonl(src)});
}

/* -------------------------------------------------------------------------------- */

// Buffer

inline Buffer::Buffer(size_t _buf_size) : buf_size(_buf_size), buf_base(new uint8_t[_buf_size]) {}

inline Buffer::Buffer(Buffer && src) noexcept
{
    buf_size = src.buf_size;
    buf_base = src.buf_base;

    src.buf_base = nullptr;
}

inline Buffer::~Buffer()
{
    delete[] buf_base;
}

inline Buffer & Buffer::operator=(Buffer && src) noexcept
{
    buf_size = src.buf_size;
    buf_base = src.buf_base;

    src.buf_base = nullptr;

    return *this;
}

inline uint8_t & Buffer::operator[](size_t pos) const noexcept
{
    if (pos < buf_size)
        return buf_base[pos];
    else
        return buf_base[buf_size - 1];
}

inline size_t Buffer::get_buf_size() const noexcept
{
    return buf_size;
}

inline uint8_t * Buffer::get_buf_base() const noexcept
{
    return buf_base;
}

/* -------------------------------------------------------------------------------- */

// UDPSocket

inline UDPSocket::~UDPSocket()
{
    if (is_active)
        close();
}

inline ssize_t UDPSocket::send(const UDPTarget & target) const
{
    return ::sendto(socket, buffer.get_buf_base(), buffer.get_buf_size(), 0, (sockaddr *)&target.target_addr, target.target_sock_len);
}

inline ssize_t UDPSocket::send(const UDPTarget & target, const Buffer & _buffer) const
{
    return ::sendto(socket, _buffer.get_buf_base(), _buffer.get_buf_size(), 0, (sockaddr *)&target.target_addr, target.target_sock_len);
}

inline ssize_t UDPSocket::receive(UDPTarget & target)
{
    return ::recvfrom(socket, buffer.get_buf_base(), buffer.get_buf_size(), 0, (sockaddr *)&target.target_addr, & target.target_sock_len);
}

inline int UDPSocket::get_socket() const noexcept {
    return socket;
}

inline in_addr_t UDPSocket::get_ip() const noexcept {
    return socket_addr.sin_addr.s_addr;
}

inline in_port_t UDPSocket::get_port() const noexcept {
    return socket_addr.sin_port;
}

inline bool UDPSocket::get_status() const noexcept {
    return is_active;
}

inline const Buffer & UDPSocket::get_buf() const noexcept {
    return buffer;
}

inline std::ostream & operator<<(std::ostream & ost, const UDPSocket & src) {
    ost << inet_ntoa(src.socket_addr.sin_addr) << ":" << htons(src.socket_addr.sin_port);
    if(src.is_active) return ost << "active";
    else return ost << "closed";
}

/* -------------------------------------------------------------------------------- */

// UDPTarget

inline UDPTarget::~UDPTarget() {}

inline void UDPTarget::set_addr(const char * ip, in_port_t port)
{
    target_addr.sin_addr.s_addr = inet_addr(ip);
    target_addr.sin_port = htons(port);
}

inline void UDPTarget::set_addr(in_addr_t ip, in_port_t port) noexcept
{
    target_addr.sin_addr.s_addr = htonl(ip);
    target_addr.sin_port = htons(port);
}

inline in_addr_t UDPTarget::get_ip() const noexcept
{
    return ntohl(target_addr.sin_addr.s_addr);
}

inline in_port_t UDPTarget::get_port() const noexcept
{
    return ntohs(target_addr.sin_port);
}

inline std::ostream & operator<<(std::ostream & ost, const UDPTarget & src)
{
    return ost << inet_ntoa(src.target_addr.sin_addr) << ':' << htons(src.target_addr.sin_port);
}

/* -------------------------------------------------------------------------------- */

#endif