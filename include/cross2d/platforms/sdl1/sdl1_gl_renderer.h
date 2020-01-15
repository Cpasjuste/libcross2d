//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL1_GL_RENDERER_H
#define SDL1_GL_RENDERER_H

#include <SDL/SDL.h>
#include "cross2d/platforms/gl1/gl_renderer.h"

namespace c2d {

    class SDL1Renderer : public GL1Renderer {

    public:

        explicit SDL1Renderer(const Vector2f &size = Vector2f(0, 0));

        ~SDL1Renderer() override;

        void flip(bool draw = true, bool inputs = true) override;

        void delay(unsigned int ms) override;

    };
}

#endif // SDL1_GL_RENDERER_H
