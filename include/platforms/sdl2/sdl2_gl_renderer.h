//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_GL_RENDERER_H
#define SDL2_GL_RENDERER_H

#include <SDL2/SDL.h>
#include "platforms/gl/gl_renderer.h"

namespace c2d {

    class SDL2Renderer : public GLRenderer {

    public:

        SDL2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL2Renderer();

        void flip(bool draw = true);

        void delay(unsigned int ms);

    private:

        SDL_Window *window = nullptr;
        SDL_GLContext ctx = nullptr;
    };
}

#endif // SDL2_GL_RENDERER_H
