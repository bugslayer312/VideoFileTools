#pragma once

#include <iosfwd>

struct FlvTag;

class FlvProcessor {
public:
    FlvProcessor(std::istream& ist);
    void Run(bool breakOnVideoTag);

private:
    void ReadHeader();
private:
    std::istream& m_ist;
};
