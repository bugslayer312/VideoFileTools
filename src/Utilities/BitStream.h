#pragma once

#include <stdint.h>

class BitStream {
public:
    struct PosType {
        PosType();
        uint32_t ByteOffset;
        uint8_t BitsLeft;

        PosType& operator+=(uint32_t bits);
    };
public:
    BitStream(uint8_t* buffer, uint32_t bufferSize);
    BitStream(BitStream const&) = delete;
    BitStream& operator=(BitStream&) = delete;
    uint32_t ReadBits(uint8_t n);
    uint32_t NextBits(uint8_t n);
    uint8_t ReadByte();
    bool Eof() const;
    bool ByteAgigned() const;
    void Seek(PosType const& pos);
    PosType Pos() const;

private:
    bool TryReadBit(bool& bit);
private:
    uint8_t* m_buffer;
    uint32_t m_bufferSize;
    PosType m_pos;
    uint8_t m_lastReadCount;
};