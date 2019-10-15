#pragma once

#include <stdint.h>

class NaluParser {
public:
    void ReadRawData(uint8_t*, int from, int size);
};