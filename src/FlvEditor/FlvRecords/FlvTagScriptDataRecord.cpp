#include "FlvTagScriptDataRecord.h"
#include "RawDataRecord.h"
#include "ScriptDataValue.h"
#include "ScriptDataObjectArray.h"
#include "../../Utilities/BigEndian.h"

#include <iostream>

void FlvTagScriptData::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    bool break_read = false;
    while (!break_read && ist.tellg() < stream_end) {
        auto pos = ist.tellg();
        if (stream_end - pos >= 3) {
            BigEndian::uint24_t end;
            ist.read(reinterpret_cast<char*>(&end), sizeof(end));
            if (ToLittleEndian(end) == 0x09) {
                break;
            }
            ist.seekg(pos);
        }
        ScriptDataType type;
        ist.read(reinterpret_cast<char*>(&type), sizeof(type));
        switch (type)
        {
        case ScriptDataType::Number:
        case ScriptDataType::Boolean:
        case ScriptDataType::String:
            Objects.emplace_back(new ScriptDataValue(type));
            Objects.back()->LoadFromStream(ist, stream_end);
            break;
        case ScriptDataType::ECMAArray:
            Objects.emplace_back(new ScriptDataObjectArray(type));
            Objects.back()->LoadFromStream(ist, stream_end);
            break;
        
        default:
            break_read = true;
            break;
        }
    }
    if (stream_end > ist.tellg()) {
        Remainder.reset(new RawDataRecord(""));
        Remainder->LoadFromStream(ist, stream_end);
    }
}

void FlvTagScriptData::SaveToStream(std::ostream& ost) {
}

void FlvTagScriptData::Print(std::ostream& ost) {
    for (size_t i(0); i < Objects.size(); ++i) {
        Objects[i]->Print(ost);
    }
    if (Remainder) {
        Remainder->Print(ost);
    }
}

void FlvTagScriptData::Edit(){
}
