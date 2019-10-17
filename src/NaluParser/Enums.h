#pragma once

#include <stdint.h>

namespace Nalu {
    enum class Type : uint8_t {
        Unspecified = 0,
        CodedSliceNonIDR = 1,
        CodedSliceDataPartitionA = 2,
        CodedSliceDataPartitionB = 3,
        CodedSliceDataPartitionC = 4,
        CodedSliceIDR = 5,
        SEI = 6,
        SPS = 7,
        PPS = 8,
        AUD = 9,
        EndOfSequence = 10,
        EndOfStream = 11,
        FillerData = 12,
        SPSExtension = 13,
        PrefixNALU = 14,
        SubsetSPS = 15,
        DPS = 16,
        Reserved1 = 17,
        Reserved2 = 18,
        CodedSliceAux = 19,
        CodedSliceSVCExtension = 20,
        CodedSlice3DAVCExtension = 21,
        Reserved4 = 22,
        Reserved5 = 23
    };
}