//
// Created by cpasjuste on 21/11/16.
//

#ifndef TINY3D_RENDERER_H
#define TINY3D_RENDERER_H

#include "cross2d/skeleton/renderer.h"

namespace c2d {

    class TINY3DRenderer : public Renderer {

    public:

        TINY3DRenderer(const Vector2f &size = Vector2f(0, 0));

        ~TINY3DRenderer();

        void draw(const VertexArray &vertices,
                  const Transform &transform,
                  const Texture *texture);

        void flip();

        void delay(unsigned int ms);

        void setShader(int shader);

    };
}

#endif //TINY3D_RENDERER_H
