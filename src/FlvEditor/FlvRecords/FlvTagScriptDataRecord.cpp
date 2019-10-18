#include "FlvTagScriptDataRecord.h"
#include "RawDataRecord.h"
#include "ScriptDataObject.h"

#include <iostream>

void FlvTagScriptData::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    std::unique_ptr<ScriptDataObject> object(new ScriptDataObject());
    object->LoadFromStream(ist, stream_end);
    Objects.push_back(std::move(object));
    if (ist.tellg() < stream_end) {
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
