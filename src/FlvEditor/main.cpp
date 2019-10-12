#include "Enums.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <exception>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

/*
enum class TagType : uint8_t {
    Audio = 8,
    Video = 9,
    ScriptData = 18
};

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

enum class VideoFrameType : uint8_t {
    KeyFrame = 1,
    InterFrame = 2,
    DisposableInterFrame = 3,
    GeneratedKeyFrame = 4,
    InfoFrame = 5
};

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

enum class VideoCodecId : uint8_t {
    Jpeg = 1,
    H263 = 2,
    ScreenVideo = 3,
    On2VP6 = 4,
    On2VP6WithAlpha = 5,
    ScreenVideoVer2 = 6,
    Avc = 7
};

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

enum class AvcPacketType : uint8_t {
    AvcSeqHeader = 0,
    AvcNalu = 1,
    AvcSeqEnd = 2
};

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
} */

namespace BigEndian {
    #pragma pack(1)
    struct uint16_t {
        uint8_t Data[2] = {0, 0};
        typedef ::uint16_t LittleEndianType;
    };
    struct uint24_t {
        uint8_t Data[3] = {0, 0, 0};
        typedef ::uint32_t LittleEndianType;
    };
    struct uint32_t {
        uint8_t Data[4] = {0, 0, 0, 0};
        typedef ::uint32_t LittleEndianType;
    };

    struct FlvHeader {
        char Label[3];
        uint8_t Version;
        uint8_t FlagsReserved: 5;
        uint8_t HasAudioTags: 1;
        uint8_t FlagsReserved2: 1;
        uint8_t HasVideoTags: 1;
        uint32_t Offset;
    };

    struct FlvTagHeader {
        TagType Type;
        uint24_t DataSize;
        uint24_t Timestamp;
        uint8_t TimestampEx;
        uint24_t StreamId;
    };

    struct AvcVideoPacketHeader {
        AvcPacketType Type;
        uint24_t CompositionTime;
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

BigEndian::uint16_t ToUINT16BigEndian(uint16_t num) {
    BigEndian::uint16_t result;
    result.Data[0] = static_cast<uint8_t>((num & 0xff00) >> 8);
    result.Data[1] = static_cast<uint8_t>(num & 0x00ff);
    return result;
}

BigEndian::uint24_t ToUINT24BigEndian(uint32_t num) {
    BigEndian::uint24_t result;
    result.Data[0] = static_cast<uint8_t>((num & 0x00ff0000) >> 16);
    result.Data[1] = static_cast<uint8_t>((num & 0x0000ff00) >> 8);
    result.Data[2] = static_cast<uint8_t>(num & 0x000000ff);
    return result;
}

BigEndian::uint32_t ToUINT32BigEndian(uint32_t num) {
    BigEndian::uint32_t result;
    result.Data[0] = static_cast<uint8_t>((num & 0xff000000) >> 24);
    result.Data[1] = static_cast<uint8_t>((num & 0x00ff0000) >> 16);
    result.Data[2] = static_cast<uint8_t>((num & 0x0000ff00) >> 8);
    result.Data[3] = static_cast<uint8_t>(num & 0x000000ff);
    return result;
}

#pragma pack(1)
struct FlvHeader {
    char Label[3];
    uint8_t Version;
    uint8_t FlagsReserved: 5;
    uint8_t HasAudioTags: 1;
    uint8_t FlagsReserved2: 1;
    uint8_t HasVideoTags: 1;
    uint32_t Offset;
    void Assign(BigEndian::FlvHeader const& behFlv) {
        memcpy(Label, behFlv.Label, sizeof(Label));
        Version = behFlv.Version;
        FlagsReserved = behFlv.FlagsReserved;
        HasAudioTags = behFlv.HasAudioTags;
        FlagsReserved2 = behFlv.FlagsReserved2;
        HasVideoTags = behFlv.HasVideoTags;
        Offset = ToLittleEndian(behFlv.Offset);
    }
    BigEndian::FlvHeader ToBigEndian() const {
        BigEndian::FlvHeader result;
        memcpy(result.Label, Label, sizeof(Label));
        result.Version = Version;
        result.FlagsReserved = FlagsReserved;
        result.HasAudioTags = HasAudioTags;
        result.FlagsReserved2 = FlagsReserved2;
        result.HasVideoTags = HasVideoTags;
        result.Offset = ToUINT32BigEndian(Offset);
        return result;
    }
    void Print(std::ostream& ost) const {
        ost << "Header: label=" << Label[0] << Label[1] << Label[2]
            << ", ver=" << static_cast<uint16_t>(Version)
            << ", flagsReserved=" << static_cast<uint16_t>(FlagsReserved)
            << ", hasAudioTags=" << static_cast<uint16_t>(HasAudioTags)
            << ", flagsReserved2=" << static_cast<uint16_t>(FlagsReserved2)
            << ", hasVideoTags=" << static_cast<uint16_t>(HasVideoTags)
            << ", offset=" << Offset << std::endl;
    }
};

struct FlvTagHeader {
    TagType Type;
    uint32_t DataSize: 24;
    uint32_t Timestamp: 24;
    uint8_t TimestampEx;
    uint32_t StreamId: 24;
    uint32_t GetFullTimestamp() const {
        return (Timestamp & 0x00ffffff) | (static_cast<std::uint32_t>(TimestampEx) << 24);
    }
    void Assign(BigEndian::FlvTagHeader const& behFlvTag) {
        Type = behFlvTag.Type;
        DataSize = ToLittleEndian(behFlvTag.DataSize);
        Timestamp = ToLittleEndian(behFlvTag.Timestamp);
        TimestampEx = behFlvTag.TimestampEx;
        StreamId = ToLittleEndian(behFlvTag.StreamId);
    }
    BigEndian::FlvTagHeader ToBigEndian() const {
        BigEndian::FlvTagHeader result;
        result.Type = Type;
        result.DataSize = ToUINT24BigEndian(DataSize);
        result.Timestamp = ToUINT24BigEndian(Timestamp);
        result.TimestampEx = TimestampEx;
        result.StreamId = ToUINT24BigEndian(StreamId);
        return result;
    }
    void Print(std::ostream& ost) const {
        ost << "  type:" << Type << std::endl
            << "  dataSize:" << DataSize << std::endl
            << "  timestamp:" << GetFullTimestamp() << std::endl
            << "  streamId:" << StreamId << std::endl;
    }
};

struct VideoDataHeader {
    VideoCodecId CodecId: 4;
    VideoFrameType FrameType: 4;
    void Print(std::ostream& ost) const {
        ost << "    CodecId:" << CodecId << std::endl
            << "    FrameType:" << FrameType << std::endl;
    }
};

struct AvcVideoPacketHeader {
    AvcPacketType Type;
    uint32_t CompositionTime: 24;
    void Assign(BigEndian::AvcVideoPacketHeader const& behAvcVideoPacket) {
        Type = behAvcVideoPacket.Type;
        CompositionTime = ToLittleEndian(behAvcVideoPacket.CompositionTime);
    }
    BigEndian::AvcVideoPacketHeader ToBigEndian() const {
        BigEndian::AvcVideoPacketHeader result;
        result.Type = Type;
        result.CompositionTime = ToUINT24BigEndian(CompositionTime);
        return result;
    }
    void Print(std::ostream& ost) const {
        ost << "      Type:" << Type << std::endl
            << "      CompositionTime:" << CompositionTime << std::endl;
    }
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

    void Print(std::ostream& ost) const {
        ost << "        Version:" << static_cast<uint16_t>(Version) << std::endl
            << "        AvcProfileIndication:" << static_cast<uint16_t>(AvcProfileIndication) << std::endl
            << "        ProfileCompatibility:" << static_cast<uint16_t>(ProfileCompatibility) << std::endl
            << "        AvcLevelIndication:" << static_cast<uint16_t>(AvcLevelIndication) << std::endl
            << "        Reserved:" << static_cast<uint16_t>(Reserved) << std::endl
            << "        LengthSizeMinusOne:" << static_cast<uint16_t>(LengthSizeMinusOne) << std::endl
            << "        Reserved2:" << static_cast<uint16_t>(Reserved2) << std::endl
            << "        NumOfSequenceParameterSets:" << static_cast<uint16_t>(NumOfSequenceParameterSets) << std::endl;
    }
};

#pragma pack(0)

std::istream& operator>>(std::istream& ist, FlvHeader& hFlv) {
    BigEndian::FlvHeader behFlv;
    if (!ist.read(reinterpret_cast<char*>(&behFlv), sizeof(behFlv))) {
        throw std::runtime_error("Failed to read FlvHeader");
    }
    hFlv.Assign(behFlv);
    return ist;
}

std::ostream& operator<<(std::ostream& ost, FlvHeader const& hFlv) {
    BigEndian::FlvHeader beHeader = hFlv.ToBigEndian();
    ost.write(reinterpret_cast<const char*>(&beHeader), sizeof(beHeader));
    return ost;
}

std::istream& operator>>(std::istream& ist, FlvTagHeader& hFlvTag) {
    BigEndian::FlvTagHeader behFlvTag;
    if (!ist.read(reinterpret_cast<char*>(&behFlvTag), sizeof(behFlvTag))) {
        if (ist.eof()) {
            return ist;
        }
        throw std::runtime_error("Failed to read FlvTagHeader");
    }
    hFlvTag.Assign(behFlvTag);
    return ist;
}

std::ostream& operator<<(std::ostream& ost, FlvTagHeader const& hFlvTag) {
    BigEndian::FlvTagHeader beHeader = hFlvTag.ToBigEndian();
    ost.write(reinterpret_cast<const char*>(&beHeader), sizeof(beHeader));
    return ost;
}

std::istream& operator>>(std::istream& ist, VideoDataHeader& hVideoData) {
    if (!ist.read(reinterpret_cast<char*>(&hVideoData), sizeof(hVideoData))) {
        if (ist.eof()) {
            return ist;
        }
        throw std::runtime_error("Failed to read VideoDataHeader");
    }
    return ist;
}

std::ostream& operator<<(std::ostream& ost, VideoDataHeader const& hVideoData) {
    ost.write(reinterpret_cast<const char*>(&hVideoData), sizeof(hVideoData));
    return ost;
}

std::istream& operator>>(std::istream& ist, AvcVideoPacketHeader& hAvcVideoPacket) {
    BigEndian::AvcVideoPacketHeader behAvcVideoPacket;
    if (!ist.read(reinterpret_cast<char*>(&behAvcVideoPacket), sizeof(behAvcVideoPacket))) {
        if (ist.eof()) {
            return ist;
        }
        throw std::runtime_error("Failed to read AvcVideoPacketHeader");
    }
    hAvcVideoPacket.Assign(behAvcVideoPacket);
    return ist;
}

std::ostream& operator<<(std::ostream& ost, AvcVideoPacketHeader const& hAvcVideoPacket) {
    BigEndian::AvcVideoPacketHeader beHeader = hAvcVideoPacket.ToBigEndian();
    ost.write(reinterpret_cast<const char*>(&beHeader), sizeof(beHeader));
    return ost;
}

std::istream& operator>>(std::istream& ist, AvcDecoderConfigurationRecordHeader& hAvcDecoderConfigurationRecord) {
    if (!ist.read(reinterpret_cast<char*>(&hAvcDecoderConfigurationRecord), sizeof(hAvcDecoderConfigurationRecord))) {
        if (ist.eof()) {
            return ist;
        }
        throw std::runtime_error("Failed to read AvcDecoderConfigurationRecord");
    }
    return ist;
}

std::ostream& operator<<(std::ostream& ost, AvcDecoderConfigurationRecordHeader const& hAvcDecoderConfigurationRecord) {
    ost.write(reinterpret_cast<const char*>(&hAvcDecoderConfigurationRecord), sizeof(hAvcDecoderConfigurationRecord));
    return ost;
}

struct FvlRecord {
    std::ios::pos_type StreamStart;
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) = 0;
    virtual void SaveToStream(std::ostream& ost) = 0;
    virtual void Print(std::ostream& ost) = 0;
    virtual ~FvlRecord(){}
};

struct ParameterSetsRecord : FvlRecord {
    uint8_t Count;
    std::vector<std::vector<uint8_t>> ParameterSets;
    ParameterSetsRecord(uint8_t count) : Count(count){
        ParameterSets.resize(Count);
    }
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
        StreamStart = ist.tellg();
        BigEndian::uint16_t beLength;
        uint32_t length;
        for (uint16_t i(0); i < Count; ++i) {
            if (stream_end - ist.tellg() < sizeof(beLength)) {
                return;
            }
            if (!ist.read(reinterpret_cast<char*>(&beLength), sizeof(beLength))) {
                throw std::runtime_error("Failed to read SequenceParameterSetLength");
            }
            length = ToLittleEndian(beLength);
            if (stream_end - ist.tellg() < length) {
                return; // maybe should throw exception
            }
            if (length) {
                ParameterSets[i].resize(length);
                if (!ist.read(reinterpret_cast<char*>(&(ParameterSets[i][0])), length)) {
                    throw std::runtime_error("Failed to read SequenceParameterSet");
                }
            }
        }
    }
    virtual void SaveToStream(std::ostream& ost) {
        for (size_t i(0); i < ParameterSets.size(); ++i) {
            auto const& v = ParameterSets[i];
            uint16_t length = static_cast<uint16_t>(v.size());
            BigEndian::uint16_t beLength = ToUINT16BigEndian(length);
            ost.write(reinterpret_cast<const char*>(&beLength), sizeof(beLength));
            if (length) {
                ost.write(reinterpret_cast<const char*>(&v[0]), length);
            }
        }
    }
    virtual void Print(std::ostream& ost) {
        for (uint16_t i(0); i < Count; ++i) {
            ost << "          " << i << ": ";
            for (size_t j(0); j < ParameterSets[i].size(); ++j) {
                ost << static_cast<uint16_t>(ParameterSets[i][j]) << " ";
            }
            ost << std::endl;
        }
    }
};

struct AvcDecoderConfigurationRecord : FvlRecord {
    AvcDecoderConfigurationRecordHeader Header;
    std::unique_ptr<FvlRecord> SequenceParameterSets;
    uint8_t NumOfPictureParameterSets;
    std::unique_ptr<FvlRecord> PictureParameterSets;

    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
        NumOfPictureParameterSets = 0;
        StreamStart = ist.tellg();
        ist >> Header;
        if (!ist) {
            return;
        }
        SequenceParameterSets.reset(new ParameterSetsRecord(Header.NumOfSequenceParameterSets));
        SequenceParameterSets->LoadFromStream(ist, stream_end);
        if (!ist || stream_end - ist.tellg() < sizeof(NumOfPictureParameterSets)) {
            return;
        }
        if (!ist.read(reinterpret_cast<char*>(&NumOfPictureParameterSets), sizeof(NumOfPictureParameterSets))) {
            throw std::runtime_error("Failed to read NumOfPictureParameterSets");
        }
        PictureParameterSets.reset(new ParameterSetsRecord(NumOfPictureParameterSets));
        PictureParameterSets->LoadFromStream(ist, stream_end);
    }
    virtual void SaveToStream(std::ostream& ost) {
        ost << Header;
        if (SequenceParameterSets) {
            SequenceParameterSets->SaveToStream(ost);
        }
        ost.write(reinterpret_cast<const char*>(&NumOfPictureParameterSets), sizeof(NumOfPictureParameterSets));
        if (PictureParameterSets) {
            PictureParameterSets->SaveToStream(ost);
        }
    }
    virtual void Print(std::ostream& ost) {
        ost << "      AvcDecoderConfigurationRecord (StreamPos=" << StreamStart << "):" << std::endl;
        Header.Print(ost);
        if (SequenceParameterSets) {
            ost << "        (StreamPos=" << SequenceParameterSets->StreamStart << ")" << std::endl;
            SequenceParameterSets->Print(ost);
        }
        ost << "        NumOfPictureParameterSets:" << static_cast<uint16_t>(NumOfPictureParameterSets) << std::endl;
        if (PictureParameterSets) {
            ost << "        (StreamPos=" << PictureParameterSets->StreamStart << ")" << std::endl;
            PictureParameterSets->Print(ost);
        }
        ost << "      ";
    }
};

struct AvcVideoPacket : FvlRecord  {
    AvcVideoPacketHeader Header;
    std::unique_ptr<FvlRecord> AvcVideoPacketData;
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
        AvcVideoPacketData.reset(nullptr);
        StreamStart = ist.tellg();
        ist >> Header;
        if (!ist) {
            return;
        }
        switch (Header.Type)
        {
        case AvcPacketType::AvcSeqHeader:
            if (stream_end - ist.tellg() >= sizeof(AvcDecoderConfigurationRecordHeader)) {
                AvcVideoPacketData.reset(new AvcDecoderConfigurationRecord());
                AvcVideoPacketData->LoadFromStream(ist, stream_end);
            }
            break;
        
        default:
            break;
        }
    }
    virtual void SaveToStream(std::ostream& ost) {
        ost << Header;
        if (AvcVideoPacketData) {
            AvcVideoPacketData->SaveToStream(ost);
            return;
        }
        throw std::runtime_error("SaveToStream is not implemented for this AvcVideoPacketData");
    }
    virtual void Print(std::ostream& ost) {
        ost << "    AvcVideoPacket: (StreamPos=" << StreamStart << ")" << std::endl;
        Header.Print(ost);
        if (AvcVideoPacketData) {
            return AvcVideoPacketData->Print(ost);
        }
        switch (Header.Type) {
        case AvcPacketType::AvcSeqHeader:
            ost << "        Unparsed Data: AVCDecoderConfigurationRecord" << std::endl;
            break;
        case AvcPacketType::AvcNalu:
            ost << "        UnparsedData: NALUs" << std::endl;
            break;
        case AvcPacketType::AvcSeqEnd:
            break;
        default:
            throw std::runtime_error("Unsupported AvcPacketType");
        }
        ost << "      ";
    }
};

struct FlvTagVideoData : FvlRecord {
    VideoDataHeader Header;
    std::unique_ptr<FvlRecord> VideoPacket;
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
        VideoPacket.reset(nullptr);
        StreamStart = ist.tellg();
        ist >> Header;
        if (ist.eof()) {
            return;
        }
        if (Header.CodecId == VideoCodecId::Avc) {
            VideoPacket.reset(new AvcVideoPacket());
            VideoPacket->LoadFromStream(ist, stream_end);
        }
    }
    virtual void SaveToStream(std::ostream& ost) {
        ost << Header;
        if (VideoPacket) {
            VideoPacket->SaveToStream(ost);
            return;
        }
        throw std::runtime_error("SaveToStream is not implemented for this VideoPacket");
    }
    virtual void Print(std::ostream& ost) {
        ost << "  VideoData: (StreamPos=" << StreamStart << ")" << std::endl;
        Header.Print(ost);
        if (VideoPacket) {
            VideoPacket->Print(ost);
        }
    }
};

struct FlvTag {
    FlvTagHeader Header;
    std::ios::pos_type StreamStart;
    uint32_t UnparsedDataSize;
    std::unique_ptr<FvlRecord> Data;

    void Print(std::ostream& ost) const {
        ost << "-=Flv tag (StreamPos=" << StreamStart << ")=-" << std::endl;
        Header.Print(ost);
        if (Data) {
            Data->Print(ost);
        }
        ost << "  UnparsedDataSize:" << UnparsedDataSize << std::endl;
    } 
};

std::istream& operator>>(std::istream& ist, FlvTag& flvTag) {
    flvTag.Data.reset(nullptr);
    std::ios::pos_type start_pos = ist.tellg();
    std::ios::pos_type end_pos(start_pos);
    flvTag.StreamStart = start_pos;
    ist >> flvTag.Header;
    if (ist.eof()) {
        return ist;
    }
    uint32_t blockSize = sizeof(flvTag.Header) + flvTag.Header.DataSize;
    flvTag.UnparsedDataSize = flvTag.Header.DataSize;
    end_pos += blockSize;
    if (flvTag.Header.Type == TagType::Video) {
        flvTag.Data.reset(new FlvTagVideoData);
        flvTag.Data->LoadFromStream(ist, end_pos);
    }
    if (ist.tellg() > end_pos) {
        throw std::runtime_error("FlvTagData too long");
    }
    flvTag.UnparsedDataSize = end_pos - ist.tellg();
    ist.seekg(end_pos);
    BigEndian::uint32_t prevTagSize;
    if (!ist.read(reinterpret_cast<char*>(&prevTagSize), sizeof(prevTagSize)) ||
            blockSize != ToLittleEndian(prevTagSize)) {
        throw std::runtime_error("Wrong prev flv tag size");
    }
    return ist;
}

std::ostream& operator<<(std::ostream& ost, FlvTag const& flvTag) {
    std::ios::pos_type begin_pos = ost.tellp();
    ost << flvTag.Header;
    if (flvTag.Data) {
        flvTag.Data->SaveToStream(ost);
    }
    std::ios::pos_type end_pos = ost.tellp();
    uint32_t dataSize = end_pos - begin_pos - sizeof(FlvTagHeader);
    BigEndian::uint24_t beDataSize = ToUINT24BigEndian(dataSize);
    begin_pos += sizeof(FlvTagHeader::Type);
    ost.seekp(begin_pos);
    ost.write(reinterpret_cast<const char*>(&beDataSize), sizeof(beDataSize));
    ost.seekp(end_pos);
    dataSize += sizeof(FlvTagHeader);
    BigEndian::uint32_t bePrevTagSize = ToUINT32BigEndian(dataSize);
    ost.write(reinterpret_cast<const char*>(&bePrevTagSize), sizeof(bePrevTagSize));
    return ost;
}

void ReadHeader(std::istream& ist) {
    std::ios::pos_type pos = ist.tellg();
    FlvHeader header;
    ist >> header;
    header.Print(std::cout);
    pos += header.Offset;
    if (!ist.seekg(pos)) {
        throw std::runtime_error("Wrong flv header offset");
    }
    BigEndian::uint32_t prevTagSize;
    if (!ist.read(reinterpret_cast<char*>(&prevTagSize), sizeof(prevTagSize)) ||
            ToLittleEndian(prevTagSize) != 0) {
        throw std::runtime_error("Wrong first prev flv tag size");
    }
}

typedef std::map<std::string, std::string> CmdOptions;

void ReadCmdOptions(int argc, char* argv[], CmdOptions& options) {
    for (int i=2; i < argc; ++i) {
        std::string strOpt = argv[i];
        if (strOpt.find("--") != 0) {
            continue;
        }
        strOpt.erase(0, 2);
        size_t pos = strOpt.find("=");
        std::string value;
        if (pos != std::string::npos) {
            value = strOpt.substr(pos+1, strOpt.size()-pos-1);
            strOpt = strOpt.substr(0, pos);
        }
        options[strOpt] = value;
    }
}

void AskAvcDecoderConfigurationRecordParameters(AvcDecoderConfigurationRecord& configurationRecord) {
    char buff[256];
    std::cout << "Enter AvcDecoderConfigurationRecord params" << std::endl;
    std::cout << "Version [1]: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header.Version = atoi(buff);
    std::cout << "AvcProfileIndication: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header.AvcProfileIndication = atoi(buff);
    std::cout << "ProfileCompatibility: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header.ProfileCompatibility = atoi(buff);
    std::cout << "AvcLevelIndication: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header.AvcLevelIndication = atoi(buff);
    std::cout << "Reserved: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header.Reserved = atoi(buff);
    std::cout << "LengthSizeMinusOne: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header.LengthSizeMinusOne = atoi(buff);
    std::cout << "Reserved2: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header.Reserved2 = atoi(buff);
    std::vector<std::vector<uint8_t>> paramSet;
    while (true) {
        std::cout << "Add SequenceParameterSet ? [y/n]: ";
        std::cin.getline(buff, 255);
        if (std::string(buff) == "y") {
            std::cout << "Enter UI8 elements: ";
            std::cin.getline(buff, 255);
            std::istringstream istr(buff);
            uint16_t num;
            std::vector<uint8_t> v;
            while (istr >> num) {
                v.push_back(num);
            }
            for (int i=0;i < v.size();++i) {
                std::cout << static_cast<uint16_t>(v[i]) << " ";
            }
            std::cout << std::endl;
            paramSet.push_back(std::move(v));
        } else {
            break;
        }
    }
    configurationRecord.Header.NumOfSequenceParameterSets = paramSet.size();
    std::unique_ptr<ParameterSetsRecord> paramSetsRec(new ParameterSetsRecord(configurationRecord.Header.NumOfSequenceParameterSets));
    paramSetsRec->ParameterSets = std::move(paramSet);
    configurationRecord.SequenceParameterSets.reset(paramSetsRec.release());
    configurationRecord.NumOfPictureParameterSets = 0;
    configurationRecord.PictureParameterSets.reset(nullptr);
    std::cout << "Apply? [y/n]: ";
    std::cin.getline(buff, 255);
    if (std::string(buff) != "y") {
        std::cout << "Canceled" << std::endl;
        exit(0);
    }
}

void CopyStreamData(std::istream& src, std::ios::pos_type from, size_t count, std::ostream& dest) {
    std::string buffer;
    buffer.resize(count);
    src.seekg(from);
    src.read(&buffer[0], count);
    dest.write(buffer.data(), count);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Missing file path" << std::endl;
    }
    std::string const filename = argv[1];
    std::ifstream ifs(filename, std::ios_base::in|std::ios_base::binary);
    std::stringstream copyStream;
    if (!ifs) {
        std::cerr << "Failed open " << filename << std::endl;
        return EXIT_FAILURE;
    }
    CmdOptions options;
    bool fix_avc_sequence_header = false;
    ReadCmdOptions(argc, argv, options);
    if (options.count("fix_avc_sequence_header")) {
        fix_avc_sequence_header = true;
    }
    std::ios::pos_type pos = ifs.tellg();
    ReadHeader(ifs);
    if (fix_avc_sequence_header) {
        CopyStreamData(ifs, pos, ifs.tellg() - pos, copyStream);
    }

    char buff[256];
    FlvTag flvTag;
    bool were_fixes = false;
    while (ifs) {
        pos = ifs.tellg();
        if (ifs >> flvTag) {
            if (fix_avc_sequence_header) {
                if (flvTag.Header.Type == TagType::Video && flvTag.Data){
                    if (FlvTagVideoData* videoData = dynamic_cast<FlvTagVideoData*>(flvTag.Data.get())) {
                        if (videoData->Header.CodecId == VideoCodecId::Avc && videoData->VideoPacket) {
                            if (AvcVideoPacket* packet = dynamic_cast<AvcVideoPacket*>(videoData->VideoPacket.get())) {
                                if (packet->Header.Type == AvcPacketType::AvcSeqHeader &&
                                    flvTag.Header.DataSize < sizeof(VideoDataHeader) + sizeof(AvcVideoPacketHeader) + sizeof(AvcDecoderConfigurationRecordHeader)) {
                                    if (!packet->AvcVideoPacketData) {
                                        packet->AvcVideoPacketData.reset(new AvcDecoderConfigurationRecord());
                                    }
                                    AvcDecoderConfigurationRecord& configurationRecord =
                                        dynamic_cast<AvcDecoderConfigurationRecord&>(*packet->AvcVideoPacketData.get());
                                    AskAvcDecoderConfigurationRecordParameters(configurationRecord);
                                    std::cout << "Write fixed tag pos: " << copyStream.tellp() << std::endl;
                                    copyStream << flvTag;
                                    were_fixes = true;
                                    continue;
                                }
                            }
                        }
                    }
                }
                CopyStreamData(ifs, pos, ifs.tellg() - pos, copyStream);
            }
            else {
                flvTag.Print(std::cout);
                std::cout << std::endl;
                if (flvTag.Header.Type == TagType::Video && options.count("break_on_video_tag")) {
                    std::cout << "Continue? > ";
                    std::cin.getline(buff, 255);
                }
            }
        }
    };
    
    if (fix_avc_sequence_header && were_fixes) {
        std::string filecopyname = filename;
        size_t sep = filecopyname.rfind('.');
        if (sep == std::string::npos) {
            filecopyname += "_fixed";
        } else {
            filecopyname.insert(sep, "_fixed");
        }
        std::ofstream ofs(filecopyname, std::ios_base::out|std::ios_base::binary|std::ios_base::ate);
        ofs << copyStream.rdbuf();
    }

    return 0;
}
