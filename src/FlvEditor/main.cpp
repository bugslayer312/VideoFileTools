
#include "FlvProcessor.h"

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <exception>

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Missing file path" << std::endl;
    }
    std::string const filepath = argv[1];
    std::ifstream ifs(filepath, std::ios_base::in|std::ios_base::binary);
    if (!ifs) {
        std::cerr << "Failed open " << filepath << std::endl;
        return EXIT_FAILURE;
    }
    CmdOptions options;
    ReadCmdOptions(argc, argv, options);

    FlvProcessor processor(filepath, ifs);
    try {
        processor.Run(options.count("break_on_video_tag") > 0,
                      options.count("break_at_end") > 0);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
