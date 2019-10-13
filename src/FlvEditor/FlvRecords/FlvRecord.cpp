#include "FlvRecord.h"

#include <string>
#include <iostream>

bool Util::AskYesNo(const char* question) {
    static char buff[4];
    static std::string yes("y");
    std::cout << question << " [y/n] > ";
    std::cin.getline(buff, 3);
    buff[3] = '\0';
    return yes == buff;
}
