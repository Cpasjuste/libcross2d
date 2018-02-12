//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

#include <SDL2/SDL.h>
#include "skeleton/renderergl.h"

namespace c2d {

    class SDL2Renderer : public GLRenderer {

    public:

        SDL2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL2Renderer();

        void flip();

        void delay(unsigned int ms);

    private:

        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Surface *surface = nullptr;
    };
}

#endif //SDL2_RENDERER_H
