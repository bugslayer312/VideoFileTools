#pragma once

#include "ScriptDataObject.h"

struct ScriptDataValue : ScriptDataObject {
    union DataType {
        double doubleVal;
        char stringVal[64];
        uint8_t boolVal;
    };
    DataType Data;

    ScriptDataValue(ScriptDataType dataType);
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end);
    virtual void SaveToStream(std::ostream& ost);
    virtual void Print(std::ostream& ost);
    virtual void Edit();
};
