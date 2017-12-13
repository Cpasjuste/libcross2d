//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

#include <SDL2/SDL.h>

#include "skeleton/renderer.h"

//#include "sdl2/sdl2_font.h"
//#include "sdl2/sdl2_texture.h"
//#include "sdl2/sdl2_fontcache.h"

namespace C2D {

    class SDL2Renderer : public Renderer {

    public:

        SDL2Renderer(int x = 0, int y = 0, int w = 0, int h = 0);

        ~SDL2Renderer();

        void DrawLine(Line *line);

        void DrawRectangle(Rectangle *rectangle);

        void Clear();

        void Flip();

        void Delay(unsigned int ms);

        void SetShader(int shader);

    private:

        SDL_Window *window = nullptr;
        SDL_Renderer *sdl_renderer = nullptr;
    };
}

#endif //SDL2_RENDERER_H
