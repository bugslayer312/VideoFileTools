#pragma once

#include "FlvRecord.h"

#include <memory>

struct AvcVideoPacketHeader;

struct AvcVideoPacket : FvlRecord  {
    std::unique_ptr<AvcVideoPacketHeader> Header;
    std::unique_ptr<FvlRecord> AvcVideoPacketData;

    AvcVideoPacket();
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    void EditHeader();
    virtual void Edit();
};
