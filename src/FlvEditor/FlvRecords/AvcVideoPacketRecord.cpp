#include "AvcVideoPacketRecord.h"
#include "../FlvHeaders.h"
#include "AvcDecoderConfigurationRecord.h"

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
    switch (Header->Type)
    {
    case AvcPacketType::AvcSeqHeader:
        if (stream_end - ist.tellg() >= sizeof(AvcDecoderConfigurationRecordHeader)) {
            AvcVideoPacketData.reset(new AvcDecoderConfigurationRecord());
            AvcVideoPacketData->LoadFromStream(ist, stream_end);
        }
        break;

    default:
        break;
    }
}

void AvcVideoPacket::SaveToStream(std::ostream& ost) {
    ost << *Header.get();
    if (AvcVideoPacketData) {
        AvcVideoPacketData->SaveToStream(ost);
        return;
    }
    throw std::runtime_error("SaveToStream is not implemented for this AvcVideoPacketData");
}

void AvcVideoPacket::Print(std::ostream& ost) {
    ost << "    AvcVideoPacket: (StreamPos=" << StreamStart << ")" << std::endl;
    Header->Print(ost);
    if (AvcVideoPacketData) {
        return AvcVideoPacketData->Print(ost);
    }
    switch (Header->Type) {
    case AvcPacketType::AvcSeqHeader:
        ost << "        Unparsed Data: AVCDecoderConfigurationRecord" << std::endl;
        break;
    case AvcPacketType::AvcNalu:
        ost << "        UnparsedData: NALUs" << std::endl;
        break;
    case AvcPacketType::AvcSeqEnd:
        break;
    default:
        throw std::runtime_error("Unsupported AvcPacketType");
    }
    ost << "      ";
}

void AvcVideoPacket::Edit() {
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
    if (!AvcVideoPacketData) {
        std::cout << "AvcVideoPacketData block is absent. Create ? [y/n] > ";
        if (Header->Type == AvcPacketType::AvcSeqHeader) {
            AvcVideoPacketData.reset(new AvcDecoderConfigurationRecord());
        } else {
            std::cout << "Not implemented for AvcPacketType != AvcSeqHeader" << std::endl;
        }
    }
    if (AvcVideoPacketData) {
        AvcVideoPacketData->Edit();
    }
}
