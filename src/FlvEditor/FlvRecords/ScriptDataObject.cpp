#include "ScriptDataObject.h"

#include <iostream>

void ScriptDataObject::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    ObjectName.reset(new ScriptDataString(false));
    ObjectName->LoadFromStream(ist, stream_end);
    ObjectValue.reset(new ScriptDataValue());
    ObjectValue->LoadFromStream(ist, stream_end);
}

void ScriptDataObject::SaveToStream(std::ostream& ost){}

void ScriptDataObject::Print(std::ostream& ost) {
    ost << "ScriptDataObject:" << std::endl;
    ObjectName->Print(ost);
    ObjectValue->Print(ost);
}

void ScriptDataObject::Edit() {}
