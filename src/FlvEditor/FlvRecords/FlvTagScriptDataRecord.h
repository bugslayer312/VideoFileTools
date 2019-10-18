#pragma once

#include "FlvRecord.h"
#include "ScriptDataObject.h"
#include "RawDataRecord.h"

#include <vector>
#include <memory>

struct FlvTagScriptData : FvlRecord {
    std::vector<std::unique_ptr<ScriptDataObject>> Objects;
    std::unique_ptr<RawDataRecord> Remainder;
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit();
};
