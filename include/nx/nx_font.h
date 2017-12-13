//
// Created by cpasjuste on 01/12/16.
//

#ifndef _NX_FONT_H_
#define _NX_FONT_H_

#include "../skeleton/font.h"
#include "../skeleton/renderer.h"

class NXFont : Font {

public:
    NXFont(const char *path, int size);

    ~NXFont();

    int GetWidth(const char *fmt, ...);

    int GetHeight(const char *fmt, ...);

    // TODO
    void *font = NULL;
};

#endif //_NX_FONT_H_
