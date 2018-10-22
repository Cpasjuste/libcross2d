//
// Created by cpasjuste on 22/06/18.
//

#include "cross2d/c2d.h"

using namespace c2d;

char *Utility::removeExt(const char *str, char sep) {

    char *retstr, *lastdot, *lastsep;

    // Error checks and allocate string.
    if (str == nullptr)
        return nullptr;
    if ((retstr = (char *) malloc(strlen(str) + 1)) == nullptr)
        return nullptr;

    // Make a copy and find the relevant characters.
    strcpy(retstr, str);
    lastdot = strrchr(retstr, '.');
    lastsep = (sep == 0) ? nullptr : strrchr(retstr, sep);

    // If it has an extension separator.
    if (lastdot != nullptr) {
        // and it's before the extenstion separator.a
        if (lastsep != nullptr) {
            if (lastsep < lastdot) {
                // then remove it.
                *lastdot = '\0';
            }
        } else {
            // Has extension separator with no path separator.
            *lastdot = '\0';
        }
    }

    // Return the modified string.
    return retstr;
}

bool Utility::startWith(const std::string &value, const std::string &str) {
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

