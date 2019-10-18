#include "ScriptDataString.h"
#include "../../Utilities/BigEndian.h"

#include <iostream>

ScriptDataString::ScriptDataString(bool isLong) : IsLong(isLong) {
}

void ScriptDataString::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    uint32_t length = 0;
    if (IsLong) {
        BigEndian::uint32_t be;
        if (stream_end - ist.tellg() >= 4) {
            BigEndian::uint32_t be;
            ist.read(reinterpret_cast<char*>(&be), sizeof(be));
            length = ToLittleEndian(be);
        }
    } else {
        if (stream_end - ist.tellg() >= 2) {
            BigEndian::uint16_t be;
            ist.read(reinterpret_cast<char*>(&be), sizeof(be));
            length = ToLittleEndian(be);
        }
    }
    if (stream_end - ist.tellg() >= length) {
        StringData.resize(length);
        ist.read(reinterpret_cast<char*>(&StringData[0]), sizeof(length));
    }
}

void ScriptDataString::SaveToStream(std::ostream& ost){}

void ScriptDataString::Print(std::ostream& ost) {
    std::cout << "String:" << StringData << std::endl;
}

void ScriptDataString::Edit(){}
