//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

#if __GLAD__

#include "glad/glad.h"

#endif

#if defined(__GL1__)

#include "cross2d/platforms/gl1/gl_renderer.h"
#include "cross2d/platforms/gl1/gl_texture.h"

#else

#if __GLES2__
#ifndef __GLAD__
#include <SDL2/SDL_opengles2.h>
#endif
// ?!
#undef GL_RGBA8
#define GL_RGBA8 GL_RGBA
#undef GL_RGB565
#define GL_RGB565 GL_RGB
// ?!
#ifndef GL_QUADS
#define GL_QUADS 0x0006
#endif
#endif

#include "cross2d/platforms/gl2/gl_renderer.h"
#include "cross2d/platforms/gl2/gl_shaders.h"
#include "cross2d/platforms/gl2/gl_texture.h"
#include "cross2d/platforms/gl2/gl_texture_buffer.h"

#endif

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

        static void exitCallback();
    };
}

#endif // SDL2_RENDERER_H
