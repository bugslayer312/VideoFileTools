#pragma once

#include "FlvRecords/FlvRecord.h"

#include <memory>
#include <ios>
#include <iostream>

struct FlvTagHeader;

struct FlvTag {
    std::unique_ptr<FlvTagHeader> Header;
    std::ios::pos_type StreamStart;
    std::unique_ptr<FvlRecord> Data;

    FlvTag();
    void Print(std::ostream& ost) const;
    void EditVideoTagHeader();
    void Edit();
};

std::istream& operator>>(std::istream& ist, FlvTag& flvTag);
std::ostream& operator<<(std::ostream& ost, FlvTag const& flvTag);
