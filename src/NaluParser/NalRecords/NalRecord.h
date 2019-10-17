#pragma once

struct BitStream;

struct NalRecord {
    virtual void LoadFrom(BitStream& bs) = 0;
};

BitStream& operator>>(BitStream& ist, NalRecord& nalRec);