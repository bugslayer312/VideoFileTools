#pragma once

#include <stdint.h>

namespace BigEndian {
    #pragma pack(1)
    struct uint16_t {
        uint8_t Data[2] = {0, 0};
        typedef ::uint16_t LittleEndianType;
        uint16_t();
        uint16_t(LittleEndianType leNum);
    };
    struct uint24_t {
        uint8_t Data[3] = {0, 0, 0};
        typedef ::uint32_t LittleEndianType;
        uint24_t();
        uint24_t(LittleEndianType leNum);
    };
    struct uint32_t {
        uint8_t Data[4];
        typedef ::uint32_t LittleEndianType;
        uint32_t();
        uint32_t(LittleEndianType leNum);
    };
    #pragma pack(0)
};

template<class T>
typename T::LittleEndianType ToLittleEndian(T const& num) {
    typename T::LittleEndianType res(0);
    int sz = sizeof(num.Data);
    for (int i(0); i < sz; ++i) {
        res |= num.Data[i] << 8*(sz - i - 1);
    }
    return res;
}
