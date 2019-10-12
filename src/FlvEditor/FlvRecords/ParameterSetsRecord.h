#pragma once

#include "FlvRecord.h"
#include <vector>
#include <string>

struct ParameterSetsRecord : FvlRecord {
    std::string Name;
    uint8_t Count;
    std::vector<std::vector<uint8_t>> ParameterSets;

    ParameterSetsRecord(std::string name, uint8_t count);
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit();
};
