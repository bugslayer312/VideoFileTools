#include "ParameterSetsRecord.h"
#include "../../Utilities/BigEndian.h"

#include <iostream>
#include <sstream>

ParameterSetsRecord::ParameterSetsRecord(std::string name, uint8_t count) : Name(name), Count(count){
}

void ParameterSetsRecord::LoadFromStream(std::istream& ist, std::ios::pos_type const& stream_end) {
    StreamStart = ist.tellg();
    BigEndian::uint16_t beLength;
    uint32_t length(0), searchCount(Count);
    ParameterSets.clear();
    Count = 0;
    for (uint16_t i(0); i < searchCount; ++i) {
        if (stream_end - ist.tellg() < sizeof(beLength)) {
            break;
        }
        std::ios::streampos prev_pos = ist.tellg();
        if (!ist.read(reinterpret_cast<char*>(&beLength), sizeof(beLength))) {
            throw std::runtime_error("Failed to read ParameterSetLength");
        }
        length = ToLittleEndian(beLength);
        if (stream_end - ist.tellg() < length) {
            ist.seekg(prev_pos);
            break; // maybe should throw exception
        }
        std::vector<std::uint8_t> params;
        if (length) {
            params.resize(length);
            if (!ist.read(reinterpret_cast<char*>(&params[0]), length)) {
                throw std::runtime_error("Failed to read ParameterSet");
            }
        }
        ParameterSets.push_back(std::move(params));
        ++Count;
    }
}

void ParameterSetsRecord::SaveToStream(std::ostream& ost) {
    for (size_t i(0); i < ParameterSets.size(); ++i) {
        auto const& v = ParameterSets[i];
        uint16_t length = static_cast<uint16_t>(v.size());
        BigEndian::uint16_t beLength(length);
        ost.write(reinterpret_cast<const char*>(&beLength), sizeof(beLength));
        if (length) {
            ost.write(reinterpret_cast<const char*>(&v[0]), length);
        }
    }
}

void ParameterSetsRecord::Print(std::ostream& ost) {
    for (uint16_t i(0); i < Count; ++i) {
        ost << "          " << i << ": ";
        for (size_t j(0); j < ParameterSets[i].size(); ++j) {
            ost << static_cast<uint16_t>(ParameterSets[i][j]) << " ";
        }
        ost << std::endl;
    }
}

void ParameterSetsRecord::Edit() {
    std::cout << "Editing " << Name << std::endl;
    Print(std::cout);
    std::string cmd;
    int16_t idx(-1);
    std::istringstream iss;
    char buff[512];
    bool atEnd = false;
    while(true)
    {
        std::cout << "Enter cmd [ins [pos] | rem [pos] | print | end]: > ";
        std::cin.getline(buff, 511);
        std::string str(buff);
        if (str.empty()) {
            break;
        }
        size_t pos = str.find(' ');
        if (pos == std::string::npos) {
            cmd = str;
            idx = -1;
        } else {
            cmd = str.substr(0, pos);
            str.erase(0, pos+1);
            std::istringstream iss(str);
            if(!(iss >> idx)) {
                idx = -1;
            }
        }
        if (cmd == "end") {
            break;
        }
        if (cmd == "rem") {
            if (ParameterSets.empty()) {
                std::cout << "Nothing to remove" << std::endl;
                continue;
            }
            atEnd = !(idx > 0 && idx < ParameterSets.size()-1);
            if (atEnd) {
                ParameterSets.pop_back();
            } else {
                ParameterSets.erase(ParameterSets.begin()+idx);
            }
            std::cout << "Removed ParameterSet from "
                      << (atEnd ? ParameterSets.size() : idx)
                      << std::endl;
            --Count;
        } else if (cmd == "ins") {
            std::cout << "Enter UI8 numbers separated by spaces > ";
            std::cin.getline(buff, 511);
            std::istringstream istr(buff);
            uint16_t num;
            std::vector<uint8_t> params;
            while (istr >> num) {
                params.push_back(static_cast<uint8_t>(num));
            }
            atEnd = !(idx > 0 && idx < ParameterSets.size());
            if (atEnd) {
                ParameterSets.push_back(std::move(params));
            } else {
                ParameterSets.insert(ParameterSets.begin()+idx, std::move(params));
            }
            std::cout << "Inserted new ParameterSet at "
                      << (atEnd ? ParameterSets.size() - 1 : idx)
                      << std::endl;
            ++Count;
        } else if (cmd == "print") {
            Print(std::cout);
        } else {
            std::cout << "Invalid command" << std::endl;
        }
    }
}
