#include "FlvProcessor.h"
#include "FlvHeaders.h"
#include "../Utilities/BigEndian.h"
#include "FlvTag.h"

#include <iostream>
#include <sstream>

FlvProcessor::FlvProcessor(std::istream& ist) : m_ist(ist) {
}

void FlvProcessor::Run(bool breakOnVideoTag) {
    ReadHeader();
    FlvTag flvTag;
    char buff[256];
    while (m_ist) {
        if (m_ist >> flvTag) {
            flvTag.Print(std::cout);
            std::cout << std::endl;
            if (breakOnVideoTag && flvTag.Header->Type == TagType::Video) {
                std::cout << "Stopped. Edit this tag? [y/n] > ";
                std::cin.getline(buff, 255);
                if (std::string("y") == buff) {
                    flvTag.Edit();
                }
            }
        }
    }
}

void FlvProcessor::ReadHeader() {
    std::ios::pos_type pos = m_ist.tellg();
    FlvHeader header;
    m_ist >> header;
    header.Print(std::cout);
    pos += header.Offset;
    if (!m_ist.seekg(pos)) {
        throw std::runtime_error("Wrong flv header offset");
    }
    BigEndian::uint32_t prevTagSize;
    if (!m_ist.read(reinterpret_cast<char*>(&prevTagSize), sizeof(prevTagSize)) ||
            ToLittleEndian(prevTagSize) != 0) {
        throw std::runtime_error("Wrong first prev flv tag size");
    }
}
