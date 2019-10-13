#include "FlvProcessor.h"
#include "FlvHeaders.h"
#include "../Utilities/BigEndian.h"
#include "FlvTag.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <set>

FlvProcessor::FlvProcessor(std::string const& filepath, std::istream& ist)
    : m_filepath(filepath)
    , m_ist(ist)
{
}

void FlvProcessor::Run(bool breakOnVideoTag, bool breakAtEnd) {
    ReadHeader();
    std::string command;
    FlvTag flvTag;
    std::ios::pos_type beforeTagPos;
    while (m_ist) {
        beforeTagPos = m_ist.tellg();
        if (m_ist >> flvTag) {
            flvTag.Print(std::cout);
            std::cout << std::endl;
            if (breakOnVideoTag && flvTag.Header->Type == TagType::Video) {
                std::cout << "Stopped. ";
                command = AskCommand();
                if (command == "next" || command == "continue") {
                    if (command == "continue") {
                        breakOnVideoTag = false;
                    }
                    continue;
                }
                else if (command == "edit") {
                    EditTag(flvTag, beforeTagPos, m_ist.tellg());
                }
                else if (command == "save") {
                    std::cout << "Save tag to file\n";
                }
                else if (command == "load") {
                    std::cout << "Load tag from file\n";
                }
            }
        }
    }
    if (!m_editedTagsData.empty()) {
        if (Util::AskYesNo("Save changes?")) {
            SaveChanges();
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

void FlvProcessor::StoreUpToPos(std::ostream& ost, std::ios::pos_type pos) {
    static long const buffSize = 1024;
    static char buff[buffSize];
    long storeSize = pos - m_ist.tellg();
    if (storeSize <= 0) {
        return;
    }
    while (storeSize > 0) {
        long chuncSize = storeSize > buffSize ? buffSize : storeSize;
        m_ist.read(buff, chuncSize);
        ost.write(buff, chuncSize);
        storeSize -= chuncSize;
    }
}

void FlvProcessor::StoreUpToEnd(std::ostream& ost) {
    static long const buffSize = 1024;
    static char buff[1024];
    while (m_ist.read(buff, buffSize)) {
        ost.write(buff, buffSize);
    }
    if (long lastReadSize = m_ist.gcount()) {
        ost.write(buff, lastReadSize);
    }
}

void FlvProcessor::SaveChanges() {
    size_t pos = m_filepath.rfind("/");
    std::string folder;
    if (pos != std::string::npos) {
        folder = m_filepath.substr(0, pos+1);
    }
    std::string filename = pos == std::string::npos ? m_filepath
                                                    : m_filepath.substr(pos+1, m_filepath.size()-pos-1);
    pos = filename.rfind(".");
    if (pos == std::string::npos) {
        filename += "_fixed";
    } else {
        filename.insert(pos, "_fixed");
    }
    char buff[128];
    std::cout << "Enter new file name[" << filename << "] > ";
    std::cin.getline(buff, 127);
    std::string newFileName = buff;
    if (newFileName.empty()) {
        newFileName = filename;
    }
    std::string savePath = folder + newFileName;
    std::ofstream ofs(savePath, std::ios_base::out|std::ios_base::binary|std::ios_base::ate);
    m_ist.clear();
    m_ist.seekg(0);
    while (!m_editedTagsData.empty()) {
        EditedTagStream& editedTagStream = m_editedTagsData.front();
        StoreUpToPos(ofs, editedTagStream.InStartPos);
        ofs << editedTagStream.StoreStream.rdbuf();
        m_ist.seekg(editedTagStream.InEndPos);
        m_editedTagsData.pop_front();
    }
    StoreUpToEnd(ofs);
}

std::string FlvProcessor::AskCommand() {
    static std::set<std::string> const commands = {"next", "continue", "edit", "save", "load"};
    char buff[64];
    std::string result;
    while (true) {
        std::cout << "Enter command [*next, continue, edit, save, load, help] > ";
        std::cin.getline(buff, 63);
        buff[63] = '\0';
        result = buff;
        if (result == "help") {
            PrintCommandsHelp();
            continue;
        }
        if (result.empty()) {
            result = "next";
        }
        if (commands.find(result) != commands.end()) {
            break;
        }
        std::cout << "Invalid command" << std::endl;
    }
    return result;
}

void FlvProcessor::PrintCommandsHelp() {
    std::cout << "next: stop on next tag" << std::endl
              << "continue: continue without breaks till end" << std::endl
              << "edit: edit current tag" << std::endl
              << "save: save current tag to file" << std::endl
              << "load: load current tag from file" << std::endl
              << "help: print this help" << std::endl;
}

void FlvProcessor::EditTag(FlvTag& flvTag, std::ios::pos_type beforeTagPos, std::ios::pos_type afterTagPos) {
    flvTag.Edit();
    flvTag.Print(std::cout);
    if (Util::AskYesNo("Apply changes?")) {
        EditedTagStream editedTagStream;
        editedTagStream.InStartPos = beforeTagPos;
        editedTagStream.InEndPos = afterTagPos;
        editedTagStream.StoreStream << flvTag;
        m_editedTagsData.push_back(std::move(editedTagStream));
        std::cout << "Applied\n";
    }
}
