#pragma once

#include <iosfwd>
#include <stdint.h>

enum class TagType : uint8_t {
    Audio = 8,
    Video = 9,
    ScriptData = 18
};
std::ostream& operator<<(std::ostream& ost, TagType tagType);

enum class VideoFrameType : uint8_t {
    KeyFrame = 1,
    InterFrame = 2,
    DisposableInterFrame = 3,
    GeneratedKeyFrame = 4,
    InfoFrame = 5
};
std::ostream& operator<<(std::ostream& ost, VideoFrameType frameType);

enum class VideoCodecId : uint8_t {
    Jpeg = 1,
    H263 = 2,
    ScreenVideo = 3,
    On2VP6 = 4,
    On2VP6WithAlpha = 5,
    ScreenVideoVer2 = 6,
    Avc = 7
};
std::ostream& operator<<(std::ostream& ost, VideoCodecId codecId);

enum class AvcPacketType : uint8_t {
    AvcSeqHeader = 0,
    AvcNalu = 1,
    AvcSeqEnd = 2
};
std::ostream& operator<<(std::ostream& ost, AvcPacketType packetType);

enum class ScriptDataType : uint8_t {
    Number = 0,
    Boolean = 1,
    String = 2,
    Object = 3,
    MovieClip = 4,
    Null = 5,
    Undefined = 6,
    Reference = 7,
    ECMAArray = 8,
    StrictArray = 10,
    Date = 11,
    LongString = 12
};
