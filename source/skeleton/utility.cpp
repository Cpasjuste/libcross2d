//
// Created by cpasjuste on 22/06/18.
//

#include "cross2d/c2d.h"

using namespace c2d;

std::string Utility::toLower(const std::string &str) {
    std::string ret = str;
    std::transform(ret.begin(), ret.end(), ret.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return ret;
}

bool Utility::startWith(const std::string &str, const std::string &value, bool sensitive) {
    if (sensitive) {
        return str.compare(0, value.length(), value) == 0;
    } else {
        std::string str_low = toLower(str);
        std::string val_low = toLower(value);
        return str_low.compare(0, val_low.length(), val_low) == 0;
    }
}

bool Utility::endsWith(const std::string &value, const std::string &ending, bool sensitive) {
    if (ending.size() > value.size()) return false;
    if (sensitive) {
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    } else {
        std::string val_low = toLower(value);
        std::string end_low = toLower(ending);
        return std::equal(end_low.rbegin(), end_low.rend(), val_low.rbegin());
    }
}

std::string Utility::removeLastSlash(const std::string &string) {

    std::string str = string;
    size_t pos = str.find_last_of('/');
    if (pos == str.length() - 1) {
        str.erase(str.length() - 1);
    }

    return str;
}

std::string Utility::removeExt(const std::string &str) {
    size_t pos = str.find_last_of('.');
    if (pos != std::string::npos) {
        return str.substr(0, pos);
    } else {
        return str;
    }
}
