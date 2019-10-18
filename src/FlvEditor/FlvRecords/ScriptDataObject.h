#pragma once

#include "FlvRecord.h"
#include "ScriptDataString.h"
#include "ScriptDataValue.h"

#include <memory>

struct ScriptDataObject : FvlRecord {
    std::unique_ptr<ScriptDataString> ObjectName;
    std::unique_ptr<ScriptDataValue> ObjectValue;
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit();
};
