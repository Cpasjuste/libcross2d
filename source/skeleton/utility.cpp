//
// Created by cpasjuste on 22/06/18.
//

#include "cross2d/c2d.h"

using namespace c2d;

bool Utility::startWith(const std::string &str, const std::string &value) {
    return str.compare(0, value.length(), value) == 0;
}

bool Utility::endsWith(const std::string &value, const std::string &ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
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
    return str.substr(0, str.find_last_of('.'));
}
