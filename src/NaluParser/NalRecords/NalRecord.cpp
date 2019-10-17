#include "NalRecord.h"

BitStream& operator>>(BitStream& ist, NalRecord& nalRec) {
    nalRec.LoadFrom(ist);
    return ist;
}