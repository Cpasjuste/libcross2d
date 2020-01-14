//
// Created by cpasjuste on 21/11/16.
//

#ifndef DREAMCAST_RENDERER_H
#define DREAMCAST_RENDERER_H

#include "cross2d/platforms/gl1/gl_renderer.h"

namespace c2d {

    class DCRenderer : public GL1Renderer {

    public:

        explicit DCRenderer(const Vector2f &size = Vector2f(0, 0));

        ~DCRenderer() override;

        void flip(bool draw = true, bool inputs = true);

        void delay(unsigned int ms);

    };
}

#endif // DREAMCAST_RENDERER_H
