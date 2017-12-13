//
// Created by cpasjuste on 01/12/16.
//

#include <cstdio>
#include <cstdarg>
#include "../../include/nx/nx_font.h"

NXFont::NXFont(const char *path, int size) : Font(path, size) {

    this->size = size;
    // TODO
}

NXFont::~NXFont() {
    // TODO
}

int NXFont::GetWidth(const char *fmt, ...) {

    // TODO
    return 0;
}

int NXFont::GetHeight(const char *fmt, ...) {

    // TODO
    return 0;
}
