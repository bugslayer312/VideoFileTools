#pragma once

#include "NalRecord.h"

struct NalSPSRecord : NalRecord {
    
    virtual void LoadFrom(BitStream& bs);
};