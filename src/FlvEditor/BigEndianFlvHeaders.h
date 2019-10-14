#pragma once

#include "../Utilities/BigEndian.h"

namespace BigEndian {
    #pragma pack(1)
    struct FlvHeader {
        char Label[3];
        uint8_t Version;
        uint8_t HasVideoTags: 1;
        uint8_t FlagsReserved2: 1;
        uint8_t HasAudioTags: 1;
        uint8_t FlagsReserved: 5;
        uint32_t Offset;
    };

    struct FlvTagHeader {
        TagType Type;
        uint24_t DataSize;
        uint24_t Timestamp;
        uint8_t TimestampEx;
        uint24_t StreamId;
    };

    struct VideoDataHeader {
        VideoCodecId CodecId: 4;
        VideoFrameType FrameType: 4;
    };

    struct AvcVideoPacketHeader {
        AvcPacketType Type;
        uint24_t CompositionTime;
    };

    struct AvcDecoderConfigurationRecordHeader {
        uint8_t Version;
        uint8_t AvcProfileIndication;
        uint8_t ProfileCompatibility;
        uint8_t AvcLevelIndication;
        uint8_t LengthSizeMinusOne: 2;
        uint8_t Reserved: 6;
        uint8_t NumOfSequenceParameterSets: 5;
        uint8_t Reserved2: 3;
    };
    #pragma pack(0)
};
