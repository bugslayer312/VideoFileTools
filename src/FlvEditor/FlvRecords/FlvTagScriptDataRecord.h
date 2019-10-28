#pragma once

#include "FlvRecord.h"

#include <vector>
#include <memory>

struct FlvTagScriptData : FvlRecord {
    std::vector<std::unique_ptr<FvlRecord>> Objects;
    std::unique_ptr<FvlRecord> Remainder;
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit();
};
