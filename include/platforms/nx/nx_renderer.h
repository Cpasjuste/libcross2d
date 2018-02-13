//
// Created by cpasjuste on 21/11/16.
//

#ifndef NX_RENDERER_H
#define NX_RENDERER_H

#include "platforms/gl/gl_renderer.h"

namespace c2d {

    class NXRenderer : public GLRenderer {

    public:

        NXRenderer(const Vector2f &size = Vector2f(0, 0));

        ~NXRenderer();

        void flip();

        void delay(unsigned int ms);
    };
}

#endif // NX_RENDERER_H
