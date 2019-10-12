#pragma once

#include <ios>
#include <iosfwd>

struct FvlRecord {
    std::ios::pos_type StreamStart;
    virtual void LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) = 0;
    virtual void SaveToStream(std::ostream& ost) = 0;
    virtual void Print(std::ostream& ost) = 0;
    virtual void Edit() = 0;
    virtual ~FvlRecord(){}
};
