#include "udp_socket.hpp"

/* -------------------------------------------------------------------------------- */

// utilities

// to fill in sockaddr_in struct with all 0.
static void in_addr_clear(sockaddr_in * sa_ptr) noexcept {
    auto sa_base = (uint8_t *)sa_ptr;
    for(int i = 0; i < sizeof(sockaddr_in); i ++){
        *(sa_base + i) = 0;
    }
}

// to check if a char can be printed.
static inline bool is_print(uint8_t c) {
    return c >= 32 && c <= 126;
}

/* -------------------------------------------------------------------------------- */

// Buffer

Buffer::Buffer(const Buffer & src) {
    buf_size = src.buf_size;
    buf_base = new uint8_t[buf_size];

    for(int i = 0; i < buf_size; i ++){
        buf_base[i] = src.buf_base[i];
    }
}

Buffer & Buffer::operator=(const Buffer & src) {
    buf_size = src.buf_size;
    buf_base = new uint8_t[buf_size];

    for(int i = 0; i < buf_size; i ++){
        buf_base[i] = src.buf_base[i];
    }

    return *this;
}

const Buffer & Buffer::operator=(const char * src) const noexcept {
    int i = 0;
    while(i < buf_size && *(src + i) != '\0'){
        *(buf_base + i) = *(src + i);
        i ++;
    }
    if(i < buf_size) *(buf_base + i) = *(src+ i);

    return *this;
}

template<typename T>
size_t Buffer::copy(const T * base, size_t size) const noexcept {
    auto lhs_ptr = (T *)buf_base;

    int i = 0;
    while(i < size && i * sizeof(T) < buf_size){
        *(lhs_ptr + i) = *(base + i);
        i ++;
    }

    return i * sizeof(T);
}

template<typename T, size_t array_size>
size_t Buffer::copy(const T (& array) [array_size]) const noexcept {
    auto lhs_ptr = (T *)buf_base;

    int i = 0;
    while(i < array_size && i * sizeof(T) < buf_size){
        *(lhs_ptr + i) = array[i];
        i ++;
    }

    return i * sizeof(T);
}

std::ostream & operator<<(std::ostream & ost, const Buffer & buffer) {
    for(int i = 0; i < buffer.buf_size; i ++){
        if(is_print(buffer[i])) std::cout << buffer[i];
        else std::cout << "਍";
    }
}

/* -------------------------------------------------------------------------------- */

// UDPSocket

UDPSocket::UDPSocket(size_t buf_size) {
    socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    in_addr_clear(&socket_addr);
    socket_addr.sin_len = sizeof(socket_addr);
    socket_addr.sin_family = AF_INET;

    is_active = false;

    buffer = Buffer(buf_size);
}

int UDPSocket::bind(const char * ip_addr, in_port_t port) {
    if(is_active) return -1;

    socket_addr.sin_addr.s_addr = inet_addr(ip_addr);
    socket_addr.sin_port = htons(port);

    auto res = ::bind(socket, (sockaddr *)&socket_addr, sizeof(socket_addr));
    is_active = res == 0 ? true : false;

    return res;
}

int UDPSocket::close() {
    auto res = ::close(socket);
    is_active = res == 0 ? false : true;

    return res;
}

/* -------------------------------------------------------------------------------- */

// UDPTarget

UDPTarget::UDPTarget(const char * ip, in_port_t port) {
    in_addr_clear(&target_addr);
    target_addr.sin_len = sizeof(sockaddr_in);
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(ip);
    target_addr.sin_port = htons(port);

    target_sock_len = sizeof(sockaddr_in);
}

UDPTarget::UDPTarget(in_addr_t ip, in_port_t port) {
    in_addr_clear(&target_addr);
    target_addr.sin_len = sizeof(sockaddr_in);
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = htonl(ip);
    target_addr.sin_port = htons(port);

    target_sock_len = sizeof(sockaddr_in);
}

/* -------------------------------------------------------------------------------- */