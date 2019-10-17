#pragma once

#include <stdint.h>

class NaluParser {
public:
    void ReadNalu(uint8_t*, int size);
};