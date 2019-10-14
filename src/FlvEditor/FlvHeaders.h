#pragma once

#include "Enums.h"

#include <stdint.h>
#include <iosfwd>

#pragma pack(1)
struct FlvHeader {
    char Label[3];
    uint8_t Version;
    uint8_t FlagsReserved: 5;
    uint8_t HasAudioTags: 1;
    uint8_t FlagsReserved2: 1;
    uint8_t HasVideoTags: 1;
    uint32_t Offset;
    void Print(std::ostream& ost) const;
};

struct FlvTagHeader {
    TagType Type;
    uint32_t DataSize: 24;
    uint32_t Timestamp: 24;
    uint8_t TimestampEx;
    uint32_t StreamId: 24;
    uint32_t GetFullTimestamp() const {
        return (Timestamp & 0x00ffffff) | (static_cast<uint32_t>(TimestampEx) << 24);
    }
    void Print(std::ostream& ost) const;
};

struct VideoDataHeader {
    VideoFrameType FrameType: 4;
    VideoCodecId CodecId: 4;
    void Print(std::ostream& ost) const;
};

struct AvcVideoPacketHeader {
    AvcPacketType Type;
    uint32_t CompositionTime: 24;
    void Print(std::ostream& ost) const;
};

struct AvcDecoderConfigurationRecordHeader {
    uint8_t Version;
    uint8_t AvcProfileIndication;
    uint8_t ProfileCompatibility;
    uint8_t AvcLevelIndication;
    uint8_t Reserved: 6;
    uint8_t LengthSizeMinusOne: 2;
    uint8_t Reserved2: 3;
    uint8_t NumOfSequenceParameterSets: 5;
    void Print(std::ostream& ost) const;
};

#pragma pack(0)

std::istream& operator>>(std::istream& ist, FlvHeader& hFlv);
std::ostream& operator<<(std::ostream& ost, FlvHeader const& hFlv);

std::istream& operator>>(std::istream& ist, FlvTagHeader& hFlvTag);
std::ostream& operator<<(std::ostream& ost, FlvTagHeader const& hFlvTag);

std::istream& operator>>(std::istream& ist, VideoDataHeader& hVideoData);
std::ostream& operator<<(std::ostream& ost, VideoDataHeader const& hVideoData);

std::istream& operator>>(std::istream& ist, AvcVideoPacketHeader& hAvcVideoPacket);
std::ostream& operator<<(std::ostream& ost, AvcVideoPacketHeader const& hAvcVideoPacket);

std::istream& operator>>(std::istream& ist, AvcDecoderConfigurationRecordHeader& hAvcDecoderConfigurationRecord);
std::ostream& operator<<(std::ostream& ost, AvcDecoderConfigurationRecordHeader const& hAvcDecoderConfigurationRecord);
