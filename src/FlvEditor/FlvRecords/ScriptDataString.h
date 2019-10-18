#pragma once

#include "FlvRecord.h"

#include <string>

struct ScriptDataString : FvlRecord {
    std::string StringData;
    bool IsLong;
    ScriptDataString(bool isLong);
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit();
};
