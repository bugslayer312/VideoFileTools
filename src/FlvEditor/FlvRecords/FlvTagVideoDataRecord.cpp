#include "FlvTagVideoDataRecord.h"
#include "../FlvHeaders.h"
#include "AvcVideoPacketRecord.h"
#include "RawDataRecord.h"
#include "../EnumUtilities.h"

#include <iostream>
#include <map>

static const std::map<std::string, VideoFrameType> VideoFrameTypeDict = {
    {"keyFrame", VideoFrameType::KeyFrame},
    {"interFrame", VideoFrameType::InterFrame},
    {"disposableInterFrame", VideoFrameType::DisposableInterFrame},
    {"generatedKeyFrame", VideoFrameType::GeneratedKeyFrame},
    {"infoFrame", VideoFrameType::InfoFrame}
};

FlvTagVideoData::FlvTagVideoData()
    : Header(new VideoDataHeader())
{
}

void FlvTagVideoData::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    VideoPacket.reset(nullptr);
    StreamStart = ist.tellg();
    ist >> *Header.get();
    if (ist.eof()) {
        return;
    }
    if (Header->CodecId == VideoCodecId::Avc) {
        VideoPacket.reset(new AvcVideoPacket());
        VideoPacket->LoadFromStream(ist, stream_end);
    } else {
        if (ist.tellg() < stream_end) {
            VideoPacket.reset(new RawDataRecord("    "));
            VideoPacket->LoadFromStream(ist, stream_end);
        }
    }
}

void FlvTagVideoData::SaveToStream(std::ostream& ost) {
    ost << *Header.get();
    if (VideoPacket) {
        VideoPacket->SaveToStream(ost);
        return;
    }
    throw std::runtime_error("SaveToStream is not implemented for this VideoPacket");
}

void FlvTagVideoData::Print(std::ostream& ost) {
    ost << "  VideoData: (StreamPos=" << StreamStart << ")" << std::endl;
    Header->Print(ost);
    if (VideoPacket) {
        VideoPacket->Print(ost);
    }
}

void FlvTagVideoData::Edit() {
    std::cout << "Editing VideoData" << std::endl;
    Header->FrameType = ReadEnumValue("FrameType", Header->FrameType, VideoFrameTypeDict);
    if (!VideoPacket) {
        std::cout << "VideoPacket block is absent. ";
        if (Util::AskYesNo("Create?")) {
            if (Header->CodecId == VideoCodecId::Avc) {
                VideoPacket.reset(new AvcVideoPacket());
            } else {
                std::cout << "Not implemented for codecId=" << Header->CodecId << std::endl;
            }
        }
    }
    if (VideoPacket) {
        VideoPacket->Edit();
    }
}
