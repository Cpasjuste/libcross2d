//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

#include <SDL2/SDL.h>

#include "skeleton/renderer.h"
#include "sdl2/sdl2_texture.h"

//#include "sdl2/sdl2_font.h"
//#include "sdl2/sdl2_texture.h"
//#include "sdl2/sdl2_fontcache.h"

namespace c2d {

    class SDL2Renderer : public Renderer {

    public:

        SDL2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL2Renderer();

        void drawRectangle(const Rectangle &rectangle, const Transform &transform, const Vector2f &scaling);

        void drawTexture(const Texture &texture, const Transform &transform, const Vector2f &scaling);

        void flip();

        void delay(unsigned int ms);

        void setShader(int shader);

        //private:
        SDL_Window *window = nullptr;
        SDL_Renderer *sdl_renderer = nullptr;
        SDL2Texture *draw_tex;
    };
}

#endif //SDL2_RENDERER_H
