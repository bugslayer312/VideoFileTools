#include "ScriptDataValue.h"
#include "../../Utilities/BigEndian.h"

#include <iostream>

ScriptDataValue::ScriptDataValue(ScriptDataType dataType)
    : ScriptDataObject(dataType)
{
}

void ScriptDataValue::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    auto pos = ist.tellg();
    switch (Type)
    {
    case ScriptDataType::Number:
        {
            BigEndian::double_t beNum;
            if (stream_end - pos >= sizeof(beNum)) {
                ist.read(reinterpret_cast<char*>(&beNum), sizeof(beNum));
                Data.doubleVal = ToLittleEndian(beNum);
            }
        }
        break;
    case ScriptDataType::Boolean:
        {
            if (stream_end - pos >= sizeof(Data.boolVal)) {
                ist.read(reinterpret_cast<char*>(&Data.boolVal), sizeof(Data.boolVal));
            }
        }
        break;
    case ScriptDataType::String:
        {
            BigEndian::uint16_t beLength;
            if (stream_end - pos >= sizeof(beLength)) {
                ist.read(reinterpret_cast<char*>(&beLength), sizeof(beLength));
                uint16_t length = ToLittleEndian(beLength);
                if (stream_end - ist.tellg() >= length) {
                    ist.read(Data.stringVal, length);
                    Data.stringVal[length] = '\0';
                }
            }
        }
        break;
    default:
        break;
    }
}

void ScriptDataValue::SaveToStream(std::ostream& ost){}

void ScriptDataValue::Print(std::ostream& ost) {
    switch (Type)
    {       
    case ScriptDataType::Number:
        ost << "NumVal: " << Data.doubleVal;
        break;
    case ScriptDataType::Boolean:
        ost << "BoolVal: " << static_cast<uint16_t>(Data.boolVal);
        break;
    case ScriptDataType::String:
        ost << "StringVal: " << Data.stringVal;
        break;
    
    default:
        ost << "UnspecifiedVal";
        break;
    }
}

void ScriptDataValue::Edit(){}
