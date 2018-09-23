//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_GLESRENDERER_H
#define SDL2_GLESRENDERER_H

#include <SDL2/SDL.h>
#include "platforms/gl/gles_renderer.h"

namespace c2d {

    class SDL2GLESRenderer : public GLESRenderer {

    public:

        SDL2GLESRenderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL2GLESRenderer();

        void flip(bool draw = true);

        void delay(unsigned int ms);

    private:

        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Surface *surface = nullptr;
    };
}

#endif //SDL2_GLESRENDERER_H
