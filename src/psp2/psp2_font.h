//
// Created by cpasjuste on 01/12/16.
//

#ifndef _PSP2_FONT_H_
#define _PSP2_FONT_H_

#include "vita2d.h"
#include <skeleton/font.h>
#include <skeleton/renderer.h>

class PSP2Font : Font {

public:
    PSP2Font(Renderer *renderer, const char *path, int size);

    ~PSP2Font();

    void Draw(int x, int y, const char *fmt, ...);

    int GetWidth(const char *fmt, ...);

    int GetHeight(const char *fmt, ...);

private:
    vita2d_pgf *font = nullptr;

};

#endif //_PSP2_FONT_H_
