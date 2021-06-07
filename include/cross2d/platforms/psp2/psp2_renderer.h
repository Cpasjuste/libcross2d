//
// Created by cpasjuste on 21/11/16.
//

#ifndef PSP2_RENDERER_H
#define PSP2_RENDERER_H

#include <vitaGL.h>
#define GL_RGBA8 GL_RGBA
#define GL_RGB565 GL_RGB
#include "cross2d/platforms/gl2/gl_renderer.h"
#include "cross2d/platforms/gl2/gl_shaders.h"
#include "cross2d/platforms/gl2/gl_texture.h"
#include "cross2d/platforms/gl2/gl_texture_buffer.h"

namespace c2d {

    class PSP2Renderer : public GLRenderer {

    public:

        explicit PSP2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~PSP2Renderer() override;

        void flip(bool draw = true, bool inputs = true) override;

        void delay(unsigned int ms) override;
    };
}

#endif // PSP2_RENDERER_H
