/*
 * @file buffer.cpp
 * @author __NYA__
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright Copyright (c) 2022 __NYA__
 * 
 */

#include "buffer.hpp"
#include <iostream>

/* -------------------------------------------------------------------------------- */

// utilities

// to check if a char can be printed.
static inline bool is_print(uint8_t c) {
    return c >= 32 && c <= 126;
}

/* -------------------------------------------------------------------------------- */

namespace EZSock {

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

    const Buffer & Buffer::operator=(const char * src) noexcept {
        int i = 0;
        while(i < buf_size && *(src + i) != '\0'){
            *(buf_base + i) = *(src + i);
            i ++;
        }
        if(i < buf_size) *(buf_base + i) = *(src+ i);

        return *this;
    }

    template<typename T>
    size_t Buffer::copy(const T * base, size_t size) noexcept {
        auto lhs_ptr = (T *)buf_base;

        int i = 0;
        while(i < size && i * sizeof(T) < buf_size){
            *(lhs_ptr + i) = *(base + i);
            i ++;
        }

        return i * sizeof(T);
    }

    template<typename T, size_t array_size>
    size_t Buffer::copy(const T (& array) [array_size]) noexcept {
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
            if(is_print(buffer[i])) ost << buffer[i];
            else ost << "਍";
        }
        return ost;
    }

/* -------------------------------------------------------------------------------- */

}

/* -------------------------------------------------------------------------------- */