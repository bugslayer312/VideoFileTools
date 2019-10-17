#include "BitStream.h"

#include <vector>
#include <cassert>

BitStream::PosType::PosType() : ByteOffset(0), BitsLeft(8)
{
}

BitStream::PosType& BitStream::PosType::operator+=(uint32_t bits) {
    uint32_t bytesOff = bits / 8;
    bits -= bytesOff*8;
    ByteOffset += bytesOff;
    if (BitsLeft <= bits) {
        ++ByteOffset;
        BitsLeft = BitsLeft + 8 - bits;
    } else {
        BitsLeft -= bits;
    }
}
 
BitStream::BitStream(uint8_t* buffer, uint32_t bufferSize)
    : m_buffer(buffer)
    , m_bufferSize(bufferSize)
    , m_lastReadCount(0)
{
}

bool BitStream::Eof() const {
    return m_pos.ByteOffset >= m_bufferSize;
}

bool BitStream::ByteAgigned() const {
    return m_pos.BitsLeft == 8;
}

bool BitStream::TryReadBit(bool& bit) {
    if (Eof()) {
        return false;
    }
    bit = (*(m_buffer + m_pos.ByteOffset) & (0x01 << (8 - m_pos.BitsLeft))) > 0;
    if (!--m_pos.BitsLeft) {
        m_pos.BitsLeft = 8;
        ++m_pos.ByteOffset;
    }
}

uint32_t BitStream::ReadBits(uint8_t n) {
    m_lastReadCount = 0;
    std::vector<bool> bits;
    bits.reserve(n);
    bool bit(false);
    while (TryReadBit(bit)) {
        ++m_lastReadCount;
        bits.push_back(bit);
    }
    if (m_lastReadCount < n) {
        return 0;
    }
    uint32_t result(0);
    for (uint8_t i(0); i < m_lastReadCount; ++i) {
        if (bits[m_lastReadCount - i - 1]) {
            result |= (0x00000001 << i);
        }
    }
    return result;
}

void BitStream::Seek(PosType const& pos) {
    m_pos = pos;
}

BitStream::PosType BitStream::Pos() const {
    return m_pos;
}

uint32_t BitStream::NextBits(uint8_t n) {
    PosType prev = m_pos;
    uint32_t result = ReadBits(n);
    m_pos = prev;
}

uint8_t BitStream::ReadByte() {
    assert(ByteAgigned() && "ReadByte: not byte aligned");
    return m_buffer[m_pos.ByteOffset++];
}