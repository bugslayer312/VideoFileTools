#include "ScriptDataObjectArray.h"
#include "ScriptDataValue.h"
#include "../../Utilities/BigEndian.h"

#include <iostream>

ScriptDataObjectArray::ScriptDataObjectArray(ScriptDataType dataType)
    : ScriptDataObject(dataType)
{
}

void ScriptDataObjectArray::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    bool break_read = false;
    BigEndian::uint32_t beSize;
    if (stream_end - ist.tellg() < sizeof(beSize)) {
        return;
    } 
    ist.read(reinterpret_cast<char*>(&beSize), sizeof(beSize));
    uint32_t size = ToLittleEndian(beSize);
    while (!break_read && size && ist.tellg() < stream_end) {
        auto pos = ist.tellg();
        if (stream_end - pos >= 3) {
            BigEndian::uint24_t end;
            ist.read(reinterpret_cast<char*>(&end), sizeof(end));
            if (ToLittleEndian(end) == 0x09) {
                break;
            }
            ist.seekg(pos);
        }
        ParamRecord param;
        param.first.reset(new ScriptDataValue(ScriptDataType::String));
        param.first->LoadFromStream(ist, stream_end);
        ScriptDataType type;
        ist.read(reinterpret_cast<char*>(&type), sizeof(type));
        switch (type)
        {
        case ScriptDataType::Number:
        case ScriptDataType::Boolean:
        case ScriptDataType::String:
            param.second.reset(new ScriptDataValue(type));
            param.second->LoadFromStream(ist, stream_end);
            --size;
            break;
        
        default:
            break_read = true;
            break;
        }
        if (!break_read) {
            Objects.push_back(std::move(param));
        }
    }
}

void ScriptDataObjectArray::SaveToStream(std::ostream& ost) {}

void ScriptDataObjectArray::Print(std::ostream& ost) {
    ost << "Array: size=" << Objects.size() << std::endl;
    for (size_t i=0; i < Objects.size(); ++i) {
        ost << i << ": ";
        Objects[i].first->Print(ost);
        ost << " : ";
        Objects[i].second->Print(ost);
        ost << std::endl;
    }

}

void ScriptDataObjectArray::Edit() {}