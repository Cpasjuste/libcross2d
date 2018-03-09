//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL1_RENDERER_H
#define SDL1_RENDERER_H

#include "SDL/SDL.h"
#include "platforms/gl/gl_renderer.h"

namespace c2d {

    class SDL1Renderer : public GLRenderer {

    public:

        SDL1Renderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL1Renderer();

        void flip(bool draw = true);

        void delay(unsigned int ms);

    private:

        SDL_Surface *screen = NULL;
    };
}

#endif //PSP2GL_RENDERER_H
