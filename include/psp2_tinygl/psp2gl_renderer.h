//
// Created by cpasjuste on 21/11/16.
//

#ifndef PSP2GL_RENDERER_H
#define PSP2GL_RENDERER_H

#include "skeleton/renderergl.h"

namespace c2d {

    class PSP2GLRenderer : public GLRenderer {

    public:

        PSP2GLRenderer(const Vector2f &size = Vector2f(0, 0));

        ~PSP2GLRenderer();

        void flip();

        void delay(unsigned int ms);
    };
}

#endif //PSP2GL_RENDERER_H
