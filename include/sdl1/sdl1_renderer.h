//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL1_RENDERER_H
#define SDL1_RENDERER_H

#include <SDL/SDL.h>

#include "../skeleton/renderer.h"
#include "sdl1_font.h"
#include "sdl1_texture.h"

class SDL1Renderer : Renderer {

public:
    SDL1Renderer(int w, int h);

    virtual ~SDL1Renderer();

    virtual void DrawLine(int x1, int y1, int x2, int y2, const Color &color);

    virtual void DrawRect(const Rect &rect, const Color &color, bool fill);

    virtual void Clear();

    virtual void Flip();

    virtual void Delay(unsigned int ms);

    virtual void SetShader(int shader);

    SDL_Surface *screen;
};

#endif //SDL1_RENDERER_H
