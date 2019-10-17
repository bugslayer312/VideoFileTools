#include "NalUnit.h"
#include "../../Utilities/BitStream.h"
#include "NalRecords/NalSPSRecord.h"

#include <cassert>
#include <vector>

NalUnit::NalUnit(uint32_t numBytesInNalu)
    : NumBytesInNalu(numBytesInNalu)
    , NumBytesInRBSP(0)
    , NaluHeaderBytes(1)
{
}

void NalUnit::LoadFrom(BitStream& bs) {
    auto pos = bs.Pos();
    Header.forbidden_zero_bit = bs.ReadBits(1);
    Header.nal_ref_idc = bs.ReadBits(2);
    Header.nal_unit_type = static_cast<Nalu::Type>(bs.ReadBits(5));
    if (Header.nal_unit_type == Nalu::Type::PrefixNALU ||
        Header.nal_unit_type == Nalu::Type::CodedSliceSVCExtension ||
        Header.nal_unit_type == Nalu::Type::CodedSlice3DAVCExtension) {
        
        if (Header.nal_unit_type != Nalu::Type::CodedSlice3DAVCExtension) {
            Header.svc_extension_flag = bs.ReadBits(1);
        } else {
            Header.avc_3d_extension_flag = bs.ReadBits(1);
        }
        if (Header.svc_extension_flag) {
            // nal_unit_header_svc_extension( )
            NaluHeaderBytes += 3;
        } else if (Header.avc_3d_extension_flag) {
            // nal_unit_header_3davc_extension( )
            NaluHeaderBytes += 2;
        } else {
            // nal_unit_header_mvc_extension( )
            NaluHeaderBytes += 3;
        }
    }

    pos += 8 * NaluHeaderBytes;

    std::vector<uint8_t> rbsp;
    uint32_t i = NaluHeaderBytes;
    while (i < NumBytesInNalu) {
        if (i+2 < NumBytesInNalu && bs.NextBits(24) == 0x000003) {
            rbsp.push_back(bs.ReadByte());
            rbsp.push_back(bs.ReadByte());
            i += 2;
            uint8_t emulation_prevention_three_byte = bs.ReadByte();
            assert((emulation_prevention_three_byte == 0x03) && "must be == 0x03");
        } else {
            rbsp.push_back(bs.ReadByte());
        }
    }
    BitStream payloadBS(&rbsp[0], rbsp.size());
    LoadPayload(bs);
}

void LoadPayload(BitStream& bs) {
    switch (Header.nal_unit_type)
    {
    case Nalu::Type::SPS:
        Payload.reset(new NalSPSRecord());
        Payload->LoadFrom(bs);
        break;
    
    default:
        break;
    }
}