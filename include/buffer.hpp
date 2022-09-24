/*
 * @file buffer.hpp
 * @author __NYA__
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright Copyright (c) 2022 __NYA__
 * 
 */

#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <cctype>
#include <cstdint>
#include <iosfwd>

/* -------------------------------------------------------------------------------- */

namespace EZSock {

/* -------------------------------------------------------------------------------- */

    /*
     * class Buffer
     * 
     * to store data to be sent/received.
     */
    class Buffer {
    private:
        size_t buf_size;
        uint8_t * buf_base;

    public:
        // to initialize buffer with a default or specified size.
        inline Buffer(size_t = 512);
        // to copy deeply.
        Buffer(const Buffer &);
        // to get control of memory from another Buffer instance.
        inline Buffer(Buffer &&) noexcept;
        // to destruct.
        inline ~Buffer();

        // to copy deeply.
        Buffer & operator=(const Buffer &);
        // to get control of memory from another Buffer instance.
        inline Buffer & operator=(Buffer &&) noexcept;

        // to read contents in buffer.
        // if parameter ranges over, return the last element.
        inline uint8_t operator[](size_t) const & noexcept;
        // to read contents in buffer.
        // if parameter ranges over, return the last element.
        inline uint8_t operator[](size_t) && noexcept;

        // to get size.
        inline size_t get_buf_size() const noexcept;
        // to get base address of buffer.
        inline uint8_t * get_buf_base() const noexcept;

        // to assign with a c string.
        const Buffer & operator=(const char *) noexcept;

        // to copy memory.
        template<typename T>
        size_t copy(const T *, size_t) noexcept;
        // to copy memory.
        template<typename T, size_t array_size>
        size_t copy(const T (&) [array_size]) noexcept;

        // to modify contents in buffer.
        // if parameter ranges over, return reference of the last element.
        inline uint8_t & operator[](size_t) & noexcept;

        // to print as string.
        friend std::ostream & operator<<(std::ostream &, const Buffer &);
    };

/* -------------------------------------------------------------------------------- */

    // implements of inline functions.

/* -------------------------------------------------------------------------------- */

    // Buffer

    inline Buffer::Buffer(size_t _buf_size) : buf_size(_buf_size), buf_base(new uint8_t[_buf_size]) {}

    inline Buffer::Buffer(Buffer && src) noexcept {
        buf_size = src.buf_size;
        buf_base = src.buf_base;

        src.buf_base = nullptr;
    }

    inline Buffer::~Buffer() {
        delete[] buf_base;
    }

    inline Buffer & Buffer::operator=(Buffer && src) noexcept {
        buf_size = src.buf_size;
        buf_base = src.buf_base;

        src.buf_base = nullptr;

        return *this;
    }

    inline uint8_t Buffer::operator[](size_t pos) && noexcept {
        if (pos < buf_size) return buf_base[pos];
        return buf_base[buf_size - 1];
    }

    inline uint8_t Buffer::operator[](size_t pos) const & noexcept {
        if (pos < buf_size) return buf_base[pos];
        return buf_base[buf_size - 1];
    }

    inline size_t Buffer::get_buf_size() const noexcept {
        return buf_size;
    }

    inline uint8_t * Buffer::get_buf_base() const noexcept {
        return buf_base;
    }

    inline uint8_t & Buffer::operator[](size_t pos) & noexcept {
        if (pos < buf_size) return buf_base[pos];
        return buf_base[buf_size - 1];
    }

}

/* -------------------------------------------------------------------------------- */

#endif