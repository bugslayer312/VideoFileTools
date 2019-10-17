#include "RawDataRecord.h"
#include "../../NaluParser/NaluParser.h"

#include <iostream>
#include <sstream>

RawDataRecord::RawDataRecord(char const* printPrefix)
    : PrintPrefix(printPrefix)
{
}

void RawDataRecord::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    StreamStart = ist.tellg();
    RawData.clear();
    long size = stream_end - ist.tellg();
    if (size > 0) {
        RawData.resize(static_cast<size_t>(size));
        ist.read(reinterpret_cast<char*>(&RawData[0]), size);
    }
}

void RawDataRecord::SaveToStream(std::ostream& ost) {
    if (RawData.size()) {
        ost.write(reinterpret_cast<const char*>(&RawData[0]), static_cast<long>(RawData.size()));
    }
}

void RawDataRecord::Print(std::ostream& ost) {
    ost << PrintPrefix << "RawData(StreamPos:" << StreamStart << ", Size:" << RawData.size()
        << ")" << std::endl;
}
