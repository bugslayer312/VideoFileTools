#include "AvcVideoPacketRecord.h"
#include "../FlvHeaders.h"
#include "AvcDecoderConfigurationRecord.h"
#include "RawDataRecord.h"

#include <iostream>
#include <sstream>

AvcVideoPacket::AvcVideoPacket()
    : Header(new AvcVideoPacketHeader())
{
}

void AvcVideoPacket::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    AvcVideoPacketData.reset(nullptr);
    StreamStart = ist.tellg();
    ist >> *Header.get();
    if (!ist) {
        return;
    }
    if (stream_end - ist.tellg()) {
        switch (Header->Type) {
        case AvcPacketType::AvcSeqHeader:
            if (stream_end - ist.tellg() >= sizeof(AvcDecoderConfigurationRecordHeader)) {
                AvcVideoPacketData.reset(new AvcDecoderConfigurationRecord());
            } else {
                AvcVideoPacketData.reset(new RawDataRecord("      AVCDecoderConfigurationRecord: "));
            }
            break;
        case AvcPacketType::AvcNalu:
            AvcVideoPacketData.reset(new RawDataRecord("      NALUs: "));
            break;
        case AvcPacketType::AvcSeqEnd:
            break;
        default:
            AvcVideoPacketData.reset(new RawDataRecord("      Unsupported: "));
            break;
        }
    }
    if (AvcVideoPacketData) {
        AvcVideoPacketData->LoadFromStream(ist, stream_end);
    }
}

void AvcVideoPacket::SaveToStream(std::ostream& ost) {
    ost << *Header.get();
    if (AvcVideoPacketData) {
        AvcVideoPacketData->SaveToStream(ost);
    }
}

void AvcVideoPacket::Print(std::ostream& ost) {
    ost << "    AvcVideoPacket: (StreamPos=" << StreamStart << ")" << std::endl;
    Header->Print(ost);
    if (AvcVideoPacketData) {
        return AvcVideoPacketData->Print(ost);
    }
}

void AvcVideoPacket::EditHeader() {
    std::cout << "Editing AvcVideoPacket" << std::endl;
    std::cout << "CompositionTime[" << Header->CompositionTime << "] > ";
    char buff[64];
    std::cin.getline(buff, 63);
    std::istringstream iss(buff);
    uint32_t tmp(0);
    if (!iss.str().empty()) {
        iss >> tmp;
        Header->CompositionTime = tmp;
    }
}

void AvcVideoPacket::Edit() {
    EditHeader();
    if (!AvcVideoPacketData) {
        std::cout << "AvcVideoPacketData block is absent. ";
        if (Util::AskYesNo("Create?")) {
            if (Header->Type == AvcPacketType::AvcSeqHeader) {
                AvcVideoPacketData.reset(new AvcDecoderConfigurationRecord());
            } else {
                std::cout << "Not implemented for AvcPacketType != AvcSeqHeader" << std::endl;
            }
        }
    }
    if (AvcVideoPacketData) {
        AvcVideoPacketData->Edit();
    }
}
