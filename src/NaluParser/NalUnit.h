#pragma once

#include "NalRecords/NalRecord.h"
#include "Enums.h"

#include <memory>

struct NalUnit : NalRecord {
    struct NaluHeader {
        uint8_t forbidden_zero_bit: 1;
        uint8_t nal_ref_idc: 2;
        Nalu::Type nal_unit_type: 5;
        bool svc_extension_flag: 1;
        bool avc_3d_extension_flag: 1;

    };
    NaluHeader Header;
    std::unique_ptr<NalRecord> Payload;

    NalUnit(uint32_t numBytesInNalu);
    virtual void LoadFrom(BitStream& bs);

private:
    void LoadPayload(BitStream& bs);

private:
    uint32_t NumBytesInNalu;
    uint32_t NumBytesInRBSP;
    uint32_t NaluHeaderBytes;
};