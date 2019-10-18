#include "FlvTag.h"
#include "FlvHeaders.h"
#include "FlvRecords/FlvTagVideoDataRecord.h"
#include "FlvRecords/FlvTagScriptDataRecord.h"
#include "FlvRecords/RawDataRecord.h"
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
}

void FlvTag::EditVideoTagHeader() {
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
}

void FlvTag::Edit() {
    EditVideoTagHeader();
    if (!Data) {
        std::cout << "Data block is absent. ";
        if (Util::AskYesNo("Create?")) {
            if (Header->Type == TagType::Video) {
                Data.reset(new FlvTagVideoData());
            } else {
                std::cout << "Not implemented for TagType=" << Header->Type << std::endl;
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
    end_pos += blockSize;
    switch (flvTag.Header->Type) {
    case TagType::Video:
        flvTag.Data.reset(new FlvTagVideoData());
        break;
    //case TagType::ScriptData:
    //    flvTag.Data.reset(new FlvTagScriptData());
    //    break;
    default:
        flvTag.Data.reset(new RawDataRecord("  "));
        break;
    }
    flvTag.Data->LoadFromStream(ist, end_pos);
    /*if (flvTag.Header->Type == TagType::Video) {
        flvTag.Data.reset(new FlvTagVideoData());
        flvTag.Data->LoadFromStream(ist, end_pos);
    } else {
        if (ist.tellg() < end_pos) {
            flvTag.Data.reset(new RawDataRecord("  "));
            flvTag.Data->LoadFromStream(ist, end_pos);
        }
    } */
    if (ist.tellg() > end_pos) {
        throw std::runtime_error("FlvTagData too long");
    }
    ist.seekg(end_pos);
    BigEndian::uint32_t prevTagSize;
    if (!ist.read(reinterpret_cast<char*>(&prevTagSize), sizeof(prevTagSize)) ||
            blockSize != ToLittleEndian(prevTagSize)) {
        std::ostringstream oss;
        oss << "Wrong prev flv tag size. DataSize=" << blockSize << ", prevTagSize=" << ToLittleEndian(prevTagSize);
        throw std::runtime_error(oss.str());
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
