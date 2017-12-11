//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TINY3D_FONT_H_
#define _TINY3D_FONT_H_

#include <skeleton/renderer.h>
#include <skeleton/font.h>

class TINY3DFont : Font {

public:
    TINY3DFont(Renderer *renderer, const char *path, int size);

    ~TINY3DFont();

    void Draw(int x, int y, const char *fmt, ...);

    virtual int GetWidth(const char *fmt, ...);

    virtual int GetHeight(const char *fmt, ...);

private:
    u32 *pixels;
};

#endif //_TINY3D_FONT_H_
