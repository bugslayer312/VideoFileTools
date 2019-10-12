//#include "Enums.h"
//#include "../Utilities/BigEndian.h"
//#include "FlvHeaders.h"
//#include "FlvRecord.h"
//#include "ParameterSetsRecord.h"
//#include "AvcDecoderConfigurationRecord.h"
//#include "AvcVideoPacketRecord.h"
//#include "FlvTagVideoDataRecord.h"
//#include "FlvTag.h"
#include "FlvProcessor.h"

#include <iostream>
#include <fstream>
//#include <memory>
#include <exception>
//#include <cstring>
#include <map>
#include <string>
//#include <vector>
//#include <sstream>
//#include <iterator>
//#include <algorithm>

/*void ReadHeader(std::istream& ist) {
    std::ios::pos_type pos = ist.tellg();
    FlvHeader header;
    ist >> header;
    header.Print(std::cout);
    pos += header.Offset;
    if (!ist.seekg(pos)) {
        throw std::runtime_error("Wrong flv header offset");
    }
    BigEndian::uint32_t prevTagSize;
    if (!ist.read(reinterpret_cast<char*>(&prevTagSize), sizeof(prevTagSize)) ||
            ToLittleEndian(prevTagSize) != 0) {
        throw std::runtime_error("Wrong first prev flv tag size");
    }
} */

typedef std::map<std::string, std::string> CmdOptions;

void ReadCmdOptions(int argc, char* argv[], CmdOptions& options) {
    for (int i=2; i < argc; ++i) {
        std::string strOpt = argv[i];
        if (strOpt.find("--") != 0) {
            continue;
        }
        strOpt.erase(0, 2);
        size_t pos = strOpt.find("=");
        std::string value;
        if (pos != std::string::npos) {
            value = strOpt.substr(pos+1, strOpt.size()-pos-1);
            strOpt = strOpt.substr(0, pos);
        }
        options[strOpt] = value;
    }
}

/*
void AskAvcDecoderConfigurationRecordParameters(AvcDecoderConfigurationRecord& configurationRecord) {
    char buff[256];
    std::cout << "Enter AvcDecoderConfigurationRecord params" << std::endl;
    std::cout << "Version [1]: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header->Version = atoi(buff);
    std::cout << "AvcProfileIndication: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header->AvcProfileIndication = atoi(buff);
    std::cout << "ProfileCompatibility: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header->ProfileCompatibility = atoi(buff);
    std::cout << "AvcLevelIndication: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header->AvcLevelIndication = atoi(buff);
    std::cout << "Reserved: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header->Reserved = atoi(buff);
    std::cout << "LengthSizeMinusOne: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header->LengthSizeMinusOne = atoi(buff);
    std::cout << "Reserved2: ";
    std::cin.getline(buff, 255);
    configurationRecord.Header->Reserved2 = atoi(buff);
    std::vector<std::vector<uint8_t>> paramSet;
    while (true) {
        std::cout << "Add SequenceParameterSet ? [y/n]: ";
        std::cin.getline(buff, 255);
        if (std::string(buff) == "y") {
            std::cout << "Enter UI8 elements: ";
            std::cin.getline(buff, 255);
            std::istringstream istr(buff);
            uint16_t num;
            std::vector<uint8_t> v;
            while (istr >> num) {
                v.push_back(num);
            }
            for (int i=0;i < v.size();++i) {
                std::cout << static_cast<uint16_t>(v[i]) << " ";
            }
            std::cout << std::endl;
            paramSet.push_back(std::move(v));
        } else {
            break;
        }
    }
    configurationRecord.Header->NumOfSequenceParameterSets = paramSet.size();
    std::unique_ptr<ParameterSetsRecord> paramSetsRec(new ParameterSetsRecord(configurationRecord.Header->NumOfSequenceParameterSets));
    paramSetsRec->ParameterSets = std::move(paramSet);
    configurationRecord.SequenceParameterSets.reset(paramSetsRec.release());
    configurationRecord.NumOfPictureParameterSets = 0;
    configurationRecord.PictureParameterSets.reset(nullptr);
    std::cout << "Apply? [y/n]: ";
    std::cin.getline(buff, 255);
    if (std::string(buff) != "y") {
        std::cout << "Canceled" << std::endl;
        exit(0);
    }
}

void CopyStreamData(std::istream& src, std::ios::pos_type from, size_t count, std::ostream& dest) {
    std::string buffer;
    buffer.resize(count);
    src.seekg(from);
    src.read(&buffer[0], count);
    dest.write(buffer.data(), count);
} */

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Missing file path" << std::endl;
    }
    std::string const filename = argv[1];
    std::ifstream ifs(filename, std::ios_base::in|std::ios_base::binary);
    //std::stringstream copyStream;
    if (!ifs) {
        std::cerr << "Failed open " << filename << std::endl;
        return EXIT_FAILURE;
    }
    CmdOptions options;
    bool fix_avc_sequence_header = false;
    ReadCmdOptions(argc, argv, options);
    if (options.count("fix_avc_sequence_header")) {
        fix_avc_sequence_header = true;
    }

    FlvProcessor processor(ifs);
    processor.Run(options.count("break_on_video_tag") > 0);

    /*std::ios::pos_type pos = ifs.tellg();
    ReadHeader(ifs);
    if (fix_avc_sequence_header) {
        CopyStreamData(ifs, pos, ifs.tellg() - pos, copyStream);
    }

    char buff[256];
    FlvTag flvTag;
    bool were_fixes = false;
    while (ifs) {
        pos = ifs.tellg();
        if (ifs >> flvTag) {
            if (fix_avc_sequence_header) {
                if (flvTag.Header->Type == TagType::Video && flvTag.Data){
                    if (FlvTagVideoData* videoData = dynamic_cast<FlvTagVideoData*>(flvTag.Data.get())) {
                        if (videoData->Header->CodecId == VideoCodecId::Avc && videoData->VideoPacket) {
                            if (AvcVideoPacket* packet = dynamic_cast<AvcVideoPacket*>(videoData->VideoPacket.get())) {
                                if (packet->Header->Type == AvcPacketType::AvcSeqHeader &&
                                    flvTag.Header->DataSize < sizeof(VideoDataHeader) + sizeof(AvcVideoPacketHeader) + sizeof(AvcDecoderConfigurationRecordHeader)) {
                                    if (!packet->AvcVideoPacketData) {
                                        packet->AvcVideoPacketData.reset(new AvcDecoderConfigurationRecord());
                                    }
                                    AvcDecoderConfigurationRecord& configurationRecord =
                                        dynamic_cast<AvcDecoderConfigurationRecord&>(*packet->AvcVideoPacketData.get());
                                    AskAvcDecoderConfigurationRecordParameters(configurationRecord);
                                    std::cout << "Write fixed tag pos: " << copyStream.tellp() << std::endl;
                                    copyStream << flvTag;
                                    were_fixes = true;
                                    continue;
                                }
                            }
                        }
                    }
                }
                CopyStreamData(ifs, pos, ifs.tellg() - pos, copyStream);
            }
            else {
                flvTag.Print(std::cout);
                std::cout << std::endl;
                if (flvTag.Header->Type == TagType::Video && options.count("break_on_video_tag")) {
                    std::cout << "Continue? > ";
                    std::cin.getline(buff, 255);
                }
            }
        }
    };
    
    if (fix_avc_sequence_header && were_fixes) {
        std::string filecopyname = filename;
        size_t sep = filecopyname.rfind('.');
        if (sep == std::string::npos) {
            filecopyname += "_fixed";
        } else {
            filecopyname.insert(sep, "_fixed");
        }
        std::ofstream ofs(filecopyname, std::ios_base::out|std::ios_base::binary|std::ios_base::ate);
        ofs << copyStream.rdbuf();
    }*/

    return 0;
}
