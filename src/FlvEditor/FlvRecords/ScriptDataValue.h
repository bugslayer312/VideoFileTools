#pragma once

#include "FlvRecord.h"
#include "../Enums.h"

struct ScriptDataValue : FvlRecord {
    ScriptDataType Type;

    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit();
};
