//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL2_FONT_H_
#define _SDL2_FONT_H_

#include <skeleton/renderer.h>
#include <skeleton/font.h>
#include <sdl2/sdl2_fontcache.h>

class SDL2Font : Font {

public:
    SDL2Font(Renderer *renderer, const char *path, int size);

    ~SDL2Font();

    void Draw(int x, int y, const char *fmt, ...);

    virtual int GetWidth(const char *fmt, ...);

    virtual int GetHeight(const char *fmt, ...);

private:
    FC_Font *font = nullptr;

};

#endif //_SDL2_FONT_H_
