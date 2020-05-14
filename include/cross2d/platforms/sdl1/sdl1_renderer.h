//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#if __GLAD__

#include <glad/glad.h>

#endif

#if defined(__GL1__)

#include "cross2d/platforms/gl1/gl_renderer.h"
#include "cross2d/platforms/gl1/gl_texture.h"

#else

#include "cross2d/platforms/gl2/gl_renderer.h"
#include "cross2d/platforms/gl2/gl_shaders.h"
#include "cross2d/platforms/gl2/gl_texture.h"
#include "cross2d/platforms/gl2/gl_texture_buffer.h"

#endif

namespace c2d {

    class SDLRenderer : public GLRenderer {

    public:

        explicit SDLRenderer(const Vector2f &size = Vector2f(0, 0));

        ~SDLRenderer() override;

        void flip(bool draw = true, bool inputs = true) override;

        void delay(unsigned int ms) override;

    };
}

#endif // SDL_RENDERER_H
