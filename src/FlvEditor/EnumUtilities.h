#pragma once

#include <iostream>
#include <string>
#include <map>

template<class EnumT>
EnumT ReadEnumValue(std::string const& name, EnumT curVal, std::map<std::string, EnumT> const& values) {
    std::cout << name << "[";
    for (auto const& key_value: values) {
        if (key_value.second == curVal) {
            std::cout << '*';
        }
        std::cout << key_value.first << ' ';
    }
    std::cout << "] > ";
    static char buff[256];
    auto found = values.cend();
    while(true) {
        std::cin.getline(buff, 255);
        if (std::string(buff).empty()) {
            return curVal;
        }
        found = values.find(buff);
        if (found == values.cend()) {
            std::cout << "Invalid value." << std::endl
                << "Enter value again or empty for skip > ";
            continue;
        }
        break;
    }
    return found->second;
}
