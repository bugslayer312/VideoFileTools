#include "FlvHeaders.h"
#include "BigEndianFlvHeaders.h"

#include <iostream>
#include <cstring>

// ========== FlvHeader ==========

void FlvHeader::Print(std::ostream& ost) const {
    ost << "Header: label=" << Label[0] << Label[1] << Label[2]
        << ", ver=" << static_cast<uint16_t>(Version)
        << ", flagsReserved=" << static_cast<uint16_t>(FlagsReserved)
        << ", hasAudioTags=" << static_cast<uint16_t>(HasAudioTags)
        << ", flagsReserved2=" << static_cast<uint16_t>(FlagsReserved2)
        << ", hasVideoTags=" << static_cast<uint16_t>(HasVideoTags)
        << ", offset=" << Offset << std::endl;
}

std::istream& operator>>(std::istream& ist, FlvHeader& hFlv) {
    BigEndian::FlvHeader behFlv;
    if (!ist.read(reinterpret_cast<char*>(&behFlv), sizeof(behFlv))) {
        throw std::runtime_error("Failed to read FlvHeader");
    }
    memcpy(hFlv.Label, behFlv.Label, sizeof(hFlv.Label));
    hFlv.Version = behFlv.Version;
    hFlv.FlagsReserved = behFlv.FlagsReserved;
    hFlv.HasAudioTags = behFlv.HasAudioTags;
    hFlv.FlagsReserved2 = behFlv.FlagsReserved2;
    hFlv.HasVideoTags = behFlv.HasVideoTags;
    hFlv.Offset = ToLittleEndian(behFlv.Offset);
    return ist;
}

std::ostream& operator<<(std::ostream& ost, FlvHeader const& hFlv) {
    BigEndian::FlvHeader beHeader;
    memcpy(beHeader.Label, hFlv.Label, sizeof(beHeader.Label));
    beHeader.Version = hFlv.Version;
    beHeader.FlagsReserved = hFlv.FlagsReserved;
    beHeader.HasAudioTags = hFlv.HasAudioTags;
    beHeader.FlagsReserved2 = hFlv.FlagsReserved2;
    beHeader.HasVideoTags = hFlv.HasVideoTags;
    beHeader.Offset = BigEndian::uint32_t(hFlv.Offset);
    ost.write(reinterpret_cast<const char*>(&beHeader), sizeof(beHeader));
    return ost;
}


// ========== FlvTagHeader ==========

void FlvTagHeader::Print(std::ostream& ost) const {
    ost << "  type:" << Type << std::endl
        << "  dataSize:" << DataSize << std::endl
        << "  timestamp:" << GetFullTimestamp() << std::endl
        << "  streamId:" << StreamId << std::endl;
}

std::istream& operator>>(std::istream& ist, FlvTagHeader& hFlvTag) {
    BigEndian::FlvTagHeader behFlvTag;
    if (!ist.read(reinterpret_cast<char*>(&behFlvTag), sizeof(behFlvTag))) {
        if (ist.eof()) {
            return ist;
        }
        throw std::runtime_error("Failed to read FlvTagHeader");
    }
    hFlvTag.Type = behFlvTag.Type;
    hFlvTag.DataSize = ToLittleEndian(behFlvTag.DataSize);
    hFlvTag.Timestamp = ToLittleEndian(behFlvTag.Timestamp);
    hFlvTag.TimestampEx = behFlvTag.TimestampEx;
    hFlvTag.StreamId = ToLittleEndian(behFlvTag.StreamId);
    return ist;
}

std::ostream& operator<<(std::ostream& ost, FlvTagHeader const& hFlvTag) {
    BigEndian::FlvTagHeader beHeader;
    beHeader.Type = hFlvTag.Type;
    beHeader.DataSize = BigEndian::uint24_t(hFlvTag.DataSize);
    beHeader.Timestamp = BigEndian::uint24_t(hFlvTag.Timestamp);
    beHeader.TimestampEx = hFlvTag.TimestampEx;
    beHeader.StreamId = BigEndian::uint24_t(hFlvTag.StreamId);
    ost.write(reinterpret_cast<const char*>(&beHeader), sizeof(beHeader));
    return ost;
}


// ========== VideoDataHeader ==========

void VideoDataHeader::Print(std::ostream& ost) const {
    ost << "    CodecId:" << CodecId << std::endl
        << "    FrameType:" << FrameType << std::endl;
}

std::istream& operator>>(std::istream& ist, VideoDataHeader& hVideoData) {
    BigEndian::VideoDataHeader beHeader;
    if (!ist.read(reinterpret_cast<char*>(&beHeader), sizeof(beHeader))) {
        if (ist.eof()) {
            return ist;
        }
        throw std::runtime_error("Failed to read VideoDataHeader");
    }
    hVideoData.FrameType = beHeader.FrameType;
    hVideoData.CodecId = beHeader.CodecId;
    return ist;
}

std::ostream& operator<<(std::ostream& ost, VideoDataHeader const& hVideoData) {
    BigEndian::VideoDataHeader beHeader;
    beHeader.FrameType = hVideoData.FrameType;
    beHeader.CodecId = hVideoData.CodecId;
    ost.write(reinterpret_cast<const char*>(&beHeader), sizeof(beHeader));
    return ost;
}


// ========== AvcVideoPacketHeader ==========

void AvcVideoPacketHeader::Print(std::ostream& ost) const {
    ost << "      Type:" << Type << std::endl
        << "      CompositionTime:" << CompositionTime << std::endl;
}

std::istream& operator>>(std::istream& ist, AvcVideoPacketHeader& hAvcVideoPacket) {
    BigEndian::AvcVideoPacketHeader behAvcVideoPacket;
    if (!ist.read(reinterpret_cast<char*>(&behAvcVideoPacket), sizeof(behAvcVideoPacket))) {
        if (ist.eof()) {
            return ist;
        }
        throw std::runtime_error("Failed to read AvcVideoPacketHeader");
    }
    hAvcVideoPacket.Type = behAvcVideoPacket.Type;
    hAvcVideoPacket.CompositionTime = ToLittleEndian(behAvcVideoPacket.CompositionTime);
    return ist;
}

std::ostream& operator<<(std::ostream& ost, AvcVideoPacketHeader const& hAvcVideoPacket) {
    BigEndian::AvcVideoPacketHeader beHeader;
    beHeader.Type = hAvcVideoPacket.Type;
    beHeader.CompositionTime = BigEndian::uint24_t(hAvcVideoPacket.CompositionTime);
    ost.write(reinterpret_cast<const char*>(&beHeader), sizeof(beHeader));
    return ost;
}


// ========== AvcDecoderConfigurationRecordHeader ==========

void AvcDecoderConfigurationRecordHeader::Print(std::ostream& ost) const {
    ost << "        Version:" << static_cast<uint16_t>(Version) << std::endl
        << "        AvcProfileIndication:" << static_cast<uint16_t>(AvcProfileIndication) << std::endl
        << "        ProfileCompatibility:" << static_cast<uint16_t>(ProfileCompatibility) << std::endl
        << "        AvcLevelIndication:" << static_cast<uint16_t>(AvcLevelIndication) << std::endl
        << "        Reserved:" << static_cast<uint16_t>(Reserved) << std::endl
        << "        LengthSizeMinusOne:" << static_cast<uint16_t>(LengthSizeMinusOne) << std::endl
        << "        Reserved2:" << static_cast<uint16_t>(Reserved2) << std::endl
        << "        NumOfSequenceParameterSets:" << static_cast<uint16_t>(NumOfSequenceParameterSets) << std::endl;
}

std::istream& operator>>(std::istream& ist, AvcDecoderConfigurationRecordHeader& hAvcDecoderConfigurationRecord) {
    BigEndian::AvcDecoderConfigurationRecordHeader beHeader;
    if (!ist.read(reinterpret_cast<char*>(&beHeader), sizeof(beHeader))) {
        if (ist.eof()) {
            return ist;
        }
        throw std::runtime_error("Failed to read AvcDecoderConfigurationRecord");
    }
    hAvcDecoderConfigurationRecord.Version = beHeader.Version;
    hAvcDecoderConfigurationRecord.AvcProfileIndication = beHeader.AvcProfileIndication;
    hAvcDecoderConfigurationRecord.ProfileCompatibility = beHeader.ProfileCompatibility;
    hAvcDecoderConfigurationRecord.AvcLevelIndication = beHeader.AvcLevelIndication;
    hAvcDecoderConfigurationRecord.Reserved = beHeader.Reserved;
    hAvcDecoderConfigurationRecord.LengthSizeMinusOne = beHeader.LengthSizeMinusOne;
    hAvcDecoderConfigurationRecord.Reserved2 = beHeader.Reserved2;
    hAvcDecoderConfigurationRecord.NumOfSequenceParameterSets = beHeader.NumOfSequenceParameterSets;
    return ist;
}

std::ostream& operator<<(std::ostream& ost, AvcDecoderConfigurationRecordHeader const& hAvcDecoderConfigurationRecord) {
    BigEndian::AvcDecoderConfigurationRecordHeader beHeader;
    beHeader.Version = hAvcDecoderConfigurationRecord.Version;
    beHeader.AvcProfileIndication = hAvcDecoderConfigurationRecord.AvcProfileIndication;
    beHeader.ProfileCompatibility = hAvcDecoderConfigurationRecord.ProfileCompatibility;
    beHeader.AvcLevelIndication = hAvcDecoderConfigurationRecord.AvcLevelIndication;
    beHeader.LengthSizeMinusOne = hAvcDecoderConfigurationRecord.LengthSizeMinusOne;
    beHeader.Reserved = hAvcDecoderConfigurationRecord.Reserved;
    beHeader.NumOfSequenceParameterSets = hAvcDecoderConfigurationRecord.NumOfSequenceParameterSets;
    beHeader.Reserved2 = hAvcDecoderConfigurationRecord.Reserved2;
    ost.write(reinterpret_cast<const char*>(&beHeader), sizeof(beHeader));
    return ost;
}
