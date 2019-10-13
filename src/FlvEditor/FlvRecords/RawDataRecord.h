#pragma once

#include "FlvRecord.h"
#include <vector>

struct RawDataRecord : FvlRecord {
    std::vector<uint8_t> RawData;
    std::string PrintPrefix;

    RawDataRecord(char const* printPrefix);
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit(){}
};
