#pragma once

#include "FlvRecord.h"
#include "../Enums.h"

enum class ScriptDataType : uint8_t {
    Number = 0,
    Boolean = 1,
    String = 2,
    Object = 3,
    MovieClip = 4,
    Null = 5,
    Undefined = 6,
    Reference = 7,
    ECMAArray = 8,
    StrictArray = 10,
    Date = 11,
    LongString = 12
};

struct ScriptDataObject : FvlRecord {
    ScriptDataType Type;
    ScriptDataObject(ScriptDataType type);
};
