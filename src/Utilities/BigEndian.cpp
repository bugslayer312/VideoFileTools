#include "BigEndian.h"

BigEndian::uint16_t::uint16_t() : Data {0, 0}
{
}

BigEndian::uint16_t::uint16_t(BigEndian::uint16_t::LittleEndianType leNum) {
    Data[0] = static_cast<uint8_t>((leNum & 0xff00) >> 8);
    Data[1] = static_cast<uint8_t>(leNum & 0x00ff);
}

BigEndian::uint24_t::uint24_t() : Data {0, 0, 0}
{
}

BigEndian::uint24_t::uint24_t(BigEndian::uint24_t::LittleEndianType leNum) {
    Data[0] = static_cast<uint8_t>((leNum & 0x00ff0000) >> 16);
    Data[1] = static_cast<uint8_t>((leNum & 0x0000ff00) >> 8);
    Data[2] = static_cast<uint8_t>(leNum & 0x000000ff);
}

BigEndian::uint32_t::uint32_t() : Data {0, 0, 0, 0}
{
}

BigEndian::uint32_t::uint32_t(BigEndian::uint32_t::LittleEndianType leNum) {
    Data[0] = static_cast<uint8_t>((leNum & 0xff000000) >> 24);
    Data[1] = static_cast<uint8_t>((leNum & 0x00ff0000) >> 16);
    Data[2] = static_cast<uint8_t>((leNum & 0x0000ff00) >> 8);
    Data[3] = static_cast<uint8_t>(leNum & 0x000000ff);
}

BigEndian::double_t::double_t() {
    double* data = reinterpret_cast<double*>(Data);
    *data = 0;
}

BigEndian::double_t::double_t(BigEndian::double_t::LittleEndianType leNum) {
    uint8_t* leArray = reinterpret_cast<uint8_t*>(&leNum);
    for (int i=0; i < 8; ++i) {
        Data[i] = leArray[7-i];
    }
}

double ToLittleEndian(BigEndian::double_t const& num) {
    double res(0);
    uint8_t* resPtr = reinterpret_cast<uint8_t*>(&res);
    for (int i=0; i < 8; ++i) {
        resPtr[i] = num.Data[7-i];
    }
    return res;
}