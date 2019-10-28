#pragma once

#include "ScriptDataObject.h"
#include <vector>
#include <memory>

struct ScriptDataObjectArray : ScriptDataObject {
    typedef std::pair<std::unique_ptr<ScriptDataObject>,
                      std::unique_ptr<ScriptDataObject>> ParamRecord;
    std::vector<ParamRecord> Objects;

    ScriptDataObjectArray(ScriptDataType dataType);
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit();
};