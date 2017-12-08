//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL1_FONT_H_
#define _SDL1_FONT_H_

#include <skeleton/renderer.h>
#include <skeleton/font.h>
#include <SDL/SDL_ttf.h>

class SDL1Font : Font {

public:
    SDL1Font(Renderer *renderer, const char *path, int size);

    ~SDL1Font();

    void Draw(int x, int y, const char *fmt, ...);

    virtual int GetWidth(const char *fmt, ...);

    virtual int GetHeight(const char *fmt, ...);

private:
    TTF_Font *font = NULL;
};

#endif //_SDL1_FONT_H_
