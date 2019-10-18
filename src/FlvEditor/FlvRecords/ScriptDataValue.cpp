#include "ScriptDataValue.h"

#include <iostream>

void ScriptDataValue::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    ist.read(reinterpret_cast<char*>(&Type), 1);
}

void ScriptDataValue::SaveToStream(std::ostream& ost){}

void ScriptDataValue::Print(std::ostream& ost) {
    ost << "Type: " << (int)Type << std::endl;
}

void ScriptDataValue::Edit(){}
