//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

#include <SDL2/SDL.h>

#include "skeleton/renderer.h"

#include "sdl2/sdl2_font.h"
#include "sdl2/sdl2_texture.h"
#include "sdl2/sdl2_fontcache.h"

class SDL2Renderer : Renderer {

public:
    SDL2Renderer(int w, int h);

    virtual ~SDL2Renderer();

    virtual void DrawLine(int x1, int y1, int x2, int y2, const Color &color);

    virtual void DrawRect(const Rect &rect, const Color &color, bool fill);

    virtual void Clip(const Rect &rect);

    virtual void Clear();

    virtual void Flip();

    virtual void Delay(unsigned int ms);

    virtual void SetShader(int shader);

    SDL_Window *window = nullptr;
    SDL_Renderer *sdl_renderer = nullptr;
};

#endif //SDL2_RENDERER_H
