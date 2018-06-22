//
// Created by cpasjuste on 22/06/18.
//

#include "c2d.h"

using namespace c2d;

char *Utility::remove_ext(const char *str, char sep) {

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
        // and it's before the extenstion separator.
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
