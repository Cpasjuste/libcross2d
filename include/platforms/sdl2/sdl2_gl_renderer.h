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

        virtual void setSize(const c2d::Vector2f &size);

        virtual void setSize(float width, float height);

        virtual void flip(bool draw = true);

        virtual void delay(unsigned int ms);

        SDL_Window *getWindow();

        SDL_GLContext getContext();

    private:

        SDL_Window *window = nullptr;
        SDL_GLContext context = nullptr;
    };
}

#endif // SDL2_GL_RENDERER_H
