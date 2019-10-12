#include "Enums.h"

#include <ostream>

std::ostream& operator<<(std::ostream& ost, TagType tagType) {
    switch (tagType) {
    case TagType::Audio:
        ost << "audio";
        break;
    case TagType::Video:
        ost << "video";
        break;
    case TagType::ScriptData:
        ost << "scriptData";
        break;
    default:
        ost << "unsupported";
        break;
    }
    return ost;
}

std::ostream& operator<<(std::ostream& ost, VideoFrameType frameType) {
    switch (frameType) {
    case VideoFrameType::KeyFrame:
        ost << "keyFrame";
        break;
    case VideoFrameType::InterFrame:
        ost << "interFrame";
        break;
    case VideoFrameType::DisposableInterFrame:
        ost << "disposableInterFrame";
        break;
    case VideoFrameType::GeneratedKeyFrame:
        ost << "generatedKeyFrame";
        break;
    case VideoFrameType::InfoFrame:
        ost << "infoFrame";
        break;
    default:
        ost << "unsupported";
        break;
    }
    return ost;
}

std::ostream& operator<<(std::ostream& ost, VideoCodecId codecId) {
    switch (codecId) {
    case VideoCodecId::Jpeg:
        ost << "jpeg";
        break;
    case VideoCodecId::H263:
        ost << "h263";
        break;
    case VideoCodecId::ScreenVideo:
        ost << "screenVideo";
        break;
    case VideoCodecId::On2VP6:
        ost << "on2VP6";
        break;
    case VideoCodecId::On2VP6WithAlpha:
        ost << "on2VP6WithAlpha";
        break;
    case VideoCodecId::ScreenVideoVer2:
        ost << "screenVideoVer2";
        break;
    case VideoCodecId::Avc:
        ost << "avc";
        break;
    default:
        ost << "unsupported";
        break;
    }
    return ost;
}

std::ostream& operator<<(std::ostream& ost, AvcPacketType packetType) {
    switch (packetType) {
    case AvcPacketType::AvcSeqHeader:
        ost << "avcSeqHeader";
        break;
    case AvcPacketType::AvcNalu:
        ost << "avcNalu";
        break;
    case AvcPacketType::AvcSeqEnd:
        ost << "avcSeqEnd";
        break;
    default:
        ost << "unsupported";
        break;
    }
    return ost;
}
