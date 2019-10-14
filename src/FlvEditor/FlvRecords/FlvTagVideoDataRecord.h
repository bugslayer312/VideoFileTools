#pragma once

#include "FlvRecord.h"

#include <memory>

struct VideoDataHeader;

struct FlvTagVideoData : FvlRecord {
    std::unique_ptr<VideoDataHeader> Header;
    std::unique_ptr<FvlRecord> VideoPacket;

    FlvTagVideoData();
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    void EditHeader();
    virtual void Edit();
};
