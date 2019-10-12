#pragma once

#include "FlvRecord.h"

#include <memory>

struct AvcDecoderConfigurationRecordHeader;
struct ParameterSetsRecord;

struct AvcDecoderConfigurationRecord : FvlRecord {
    std::unique_ptr<AvcDecoderConfigurationRecordHeader> Header;
    std::unique_ptr<ParameterSetsRecord> SequenceParameterSets;
    uint8_t NumOfPictureParameterSets;
    std::unique_ptr<ParameterSetsRecord> PictureParameterSets;

    AvcDecoderConfigurationRecord();
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit();
};
