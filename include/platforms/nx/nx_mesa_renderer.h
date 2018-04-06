//
// Created by cpasjuste on 21/11/16.
//

#ifndef NX_MESA_RENDERER_H
#define NX_MESA_RENDERER_H

#include <SDL2/SDL.h>
#include "platforms/gl/gl_renderer.h"

namespace c2d {

    class NXMESARenderer : public GLRenderer {

    public:

        NXMESARenderer(const Vector2f &size = Vector2f(0, 0));

        ~NXMESARenderer();

        void flip(bool draw = true);

        void delay(unsigned int ms);

        void setSize(const c2d::Vector2f &size);

        void setSize(float width, float height);

    private:

        u8 *fb = NULL;
        SDL_Window *window;
        SDL_Renderer *renderer;
    };
}

#endif // NX_MESA_RENDERER_H
