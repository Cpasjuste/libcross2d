//
// Created by cpasjuste on 21/11/16.
//

#ifndef PSP2GL_RENDERER_H
#define PSP2GL_RENDERER_H

#include "skeleton/renderer.h"

namespace c2d {

    class PSP2GLRenderer : public Renderer {

    public:

        PSP2GLRenderer(const Vector2f &size = Vector2f(0, 0));

        ~PSP2GLRenderer();

        void draw(const VertexArray &vertices,
                  const Transform &transform,
                  const Texture *texture);

        void flip();

        void delay(unsigned int ms);
    };
}

#endif //PSP2GL_RENDERER_H
