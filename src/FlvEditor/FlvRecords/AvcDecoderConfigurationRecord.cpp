#include "AvcDecoderConfigurationRecord.h"
#include "../FlvHeaders.h"
#include "ParameterSetsRecord.h"
#include "RawDataRecord.h"

#include <iostream>
#include <sstream>

AvcDecoderConfigurationRecord::AvcDecoderConfigurationRecord()
    : Header(new AvcDecoderConfigurationRecordHeader())
{
}

void AvcDecoderConfigurationRecord::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    NumOfPictureParameterSets = 0;
    StreamStart = ist.tellg();
    ist >> *Header.get();
    if (!ist) {
        return;
    }
    SequenceParameterSets.reset(new ParameterSetsRecord("SequenceParameterSets", Header->NumOfSequenceParameterSets));
    SequenceParameterSets->LoadFromStream(ist, stream_end);
    if (!ist) {
        return;
    }
    if (ist.tellg() < stream_end) {
        if (stream_end - ist.tellg() >= sizeof(NumOfPictureParameterSets)) {
            if (!ist.read(reinterpret_cast<char*>(&NumOfPictureParameterSets), sizeof(NumOfPictureParameterSets))) {
                throw std::runtime_error("Failed to read NumOfPictureParameterSets");
            }
            PictureParameterSets.reset(new ParameterSetsRecord("PictureParameterSets", NumOfPictureParameterSets));
            PictureParameterSets->LoadFromStream(ist, stream_end);
        }
        if (ist.tellg() < stream_end) {
            LastRawData.reset(new RawDataRecord("        UnparsedChunk: "));
            LastRawData->LoadFromStream(ist, stream_end);
        }
    }
}

void AvcDecoderConfigurationRecord::SaveToStream(std::ostream& ost) {
    ost << *Header.get();
    if (SequenceParameterSets) {
        SequenceParameterSets->SaveToStream(ost);
    }
    ost.write(reinterpret_cast<const char*>(&NumOfPictureParameterSets), sizeof(NumOfPictureParameterSets));
    if (PictureParameterSets) {
        PictureParameterSets->SaveToStream(ost);
    }
}

void AvcDecoderConfigurationRecord::Print(std::ostream& ost) {
    ost << "      AvcDecoderConfigurationRecord (StreamPos=" << StreamStart << "):" << std::endl;
    Header->Print(ost);
    if (SequenceParameterSets) {
        ost << "        (StreamPos=" << SequenceParameterSets->StreamStart << ")" << std::endl;
        SequenceParameterSets->Print(ost);
    }
    ost << "        NumOfPictureParameterSets:" << static_cast<uint16_t>(NumOfPictureParameterSets) << std::endl;
    if (PictureParameterSets) {
        ost << "        (StreamPos=" << PictureParameterSets->StreamStart << ")" << std::endl;
        PictureParameterSets->Print(ost);
    }
    ost << "      ";
}

void AvcDecoderConfigurationRecord::Edit() {
    std::cout << "Editing AvcDecoderConfigurationRecord" << std::endl;
    char buff[64];
    uint32_t tmp(0);
    std::cout << "Version[" << static_cast<uint16_t>(Header->Version) << "] > ";
    std::cin.getline(buff, sizeof(buff)-1);
    std::istringstream iss(buff);
    iss >> tmp;
    Header->Version = static_cast<uint8_t>(tmp);

    std::cout << "AvcProfileIndication[" << static_cast<uint16_t>(Header->AvcProfileIndication) << "] > ";
    std::cin.getline(buff, sizeof(buff)-1);
    iss.str(buff); iss.clear();
    iss >> tmp;
    Header->AvcProfileIndication = static_cast<uint8_t>(tmp);

    std::cout << "ProfileCompatibility[" << static_cast<uint16_t>(Header->ProfileCompatibility) << "] > ";
    std::cin.getline(buff, sizeof(buff)-1);
    iss.str(buff); iss.clear();
    iss >> tmp;
    Header->ProfileCompatibility = static_cast<uint8_t>(tmp);

    std::cout << "AvcLevelIndication[" << static_cast<uint16_t>(Header->AvcLevelIndication) << "] > ";
    std::cin.getline(buff, sizeof(buff)-1);
    iss.str(buff); iss.clear();
    iss >> tmp;
    Header->AvcLevelIndication = static_cast<uint8_t>(tmp);

    std::cout << "Reserved[" << static_cast<uint16_t>(Header->Reserved) << "] > ";
    std::cin.getline(buff, sizeof(buff)-1);
    iss.str(buff); iss.clear();
    iss >> tmp;
    Header->Reserved = static_cast<uint8_t>(tmp);

    std::cout << "LengthSizeMinusOne[" << static_cast<uint16_t>(Header->LengthSizeMinusOne) << "] > ";
    std::cin.getline(buff, sizeof(buff)-1);
    iss.str(buff); iss.clear();
    iss >> tmp;
    Header->LengthSizeMinusOne = static_cast<uint8_t>(tmp);

    std::cout << "Reserved2[" << static_cast<uint16_t>(Header->Reserved2) << "] > ";
    std::cin.getline(buff, sizeof(buff)-1);
    iss.str(buff); iss.clear();
    iss >> tmp;
    Header->Reserved2 = static_cast<uint8_t>(tmp);

    if (!SequenceParameterSets) {
        SequenceParameterSets.reset(new ParameterSetsRecord("SequenceParameterSets", 0));
    }
    SequenceParameterSets->Edit();
    Header->NumOfSequenceParameterSets = SequenceParameterSets->Count;
    if (!PictureParameterSets) {
        PictureParameterSets.reset(new ParameterSetsRecord("PictureParameterSets", 0));
    }
    PictureParameterSets->Edit();
    NumOfPictureParameterSets = PictureParameterSets->Count;
}
