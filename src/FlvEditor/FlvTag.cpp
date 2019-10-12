#include "FlvTag.h"
#include "FlvHeaders.h"
#include "FlvRecords/FlvTagVideoDataRecord.h"
#include "../Utilities/BigEndian.h"

#include <iostream>
#include <sstream>

FlvTag::FlvTag()
    : Header(new FlvTagHeader())
{
}

void FlvTag::Print(std::ostream& ost) const {
    ost << "-=Flv tag (StreamPos=" << StreamStart << ")=-" << std::endl;
    Header->Print(ost);
    if (Data) {
        Data->Print(ost);
    }
    ost << "  UnparsedDataSize:" << UnparsedDataSize << std::endl;
}

void FlvTag::Edit() {
    char buff[64];
    std::cout << "Editing tag header" << std::endl;
    std::cout << "timestamp[" << Header->GetFullTimestamp() << "] > ";
    std::cin.getline(buff, sizeof(buff)-1);
    std::istringstream iss(buff);
    uint32_t tmp(0);
    if (!iss.str().empty()) {
        iss >> tmp;
        Header->Timestamp = tmp & 0x00ffffff;
        Header->TimestampEx = static_cast<uint8_t>((tmp & 0xff000000) >> 24);
    }
    std::cout << "streamId[" << Header->StreamId << "] > ";
    std::cin.getline(buff, sizeof(buff)-1);
    iss.str(buff);
    iss.clear();
    if (!iss.str().empty()) {
        iss >> tmp;
        Header->StreamId = tmp;
    }
    if (!Data) {
        std::cout << "Data block is absent. Create ? [y/n] > ";
        std::cin.getline(buff, sizeof(buff)-1);
        if (std::string(buff) == "y") {
            if (Header->Type == TagType::Video) {
                Data.reset(new FlvTagVideoData());
            }
        }
    }
    if (Data) {
        Data->Edit();
    }
}

std::istream& operator>>(std::istream& ist, FlvTag& flvTag) {
    flvTag.Data.reset(nullptr);
    std::ios::pos_type start_pos = ist.tellg();
    std::ios::pos_type end_pos(start_pos);
    flvTag.StreamStart = start_pos;
    ist >> *flvTag.Header.get();
    if (ist.eof()) {
        return ist;
    }
    uint32_t blockSize = sizeof(FlvTagHeader) + flvTag.Header->DataSize;
    flvTag.UnparsedDataSize = flvTag.Header->DataSize;
    end_pos += blockSize;
    if (flvTag.Header->Type == TagType::Video) {
        flvTag.Data.reset(new FlvTagVideoData());
        flvTag.Data->LoadFromStream(ist, end_pos);
    }
    if (ist.tellg() > end_pos) {
        throw std::runtime_error("FlvTagData too long");
    }
    flvTag.UnparsedDataSize = end_pos - ist.tellg();
    ist.seekg(end_pos);
    BigEndian::uint32_t prevTagSize;
    if (!ist.read(reinterpret_cast<char*>(&prevTagSize), sizeof(prevTagSize)) ||
            blockSize != ToLittleEndian(prevTagSize)) {
        throw std::runtime_error("Wrong prev flv tag size");
    }
    return ist;
}

std::ostream& operator<<(std::ostream& ost, FlvTag const& flvTag) {
    std::ios::pos_type begin_pos = ost.tellp();
    ost << *flvTag.Header.get();
    if (flvTag.Data) {
        flvTag.Data->SaveToStream(ost);
    }
    std::ios::pos_type end_pos = ost.tellp();
    uint32_t dataSize = end_pos - begin_pos - sizeof(FlvTagHeader);
    BigEndian::uint24_t beDataSize(dataSize);
    begin_pos += sizeof(FlvTagHeader::Type);
    ost.seekp(begin_pos);
    ost.write(reinterpret_cast<const char*>(&beDataSize), sizeof(beDataSize));
    ost.seekp(end_pos);
    dataSize += sizeof(FlvTagHeader);
    BigEndian::uint32_t bePrevTagSize(dataSize);
    ost.write(reinterpret_cast<const char*>(&bePrevTagSize), sizeof(bePrevTagSize));
    return ost;
}
