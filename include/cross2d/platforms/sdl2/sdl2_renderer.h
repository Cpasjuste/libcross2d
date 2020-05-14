//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_GL_RENDERER_H
#define SDL2_GL_RENDERER_H

#if defined(__SDL2__) && defined(__GL2__)

#include <SDL2/SDL.h>
#include "cross2d/platforms/gl2/gl_renderer.h"

namespace c2d {

    class SDL2Renderer : public GLRenderer {

    public:

        explicit SDL2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL2Renderer() override;

        void flip(bool draw = true, bool inputs = true) override;

        void delay(unsigned int ms) override;

        SDL_Window *getWindow();

        SDL_GLContext getContext();

    private:

        SDL_Window *window = nullptr;
        SDL_GLContext context = nullptr;
    };
}

#endif // defined(__SDL2__) && defined(__GL2__)
#endif // SDL2_GL_RENDERER_H
