//
// Created by cpasjuste on 06/01/18.
//

#ifndef CROSS2D_RENDERERGL_H
#define CROSS2D_RENDERERGL_H

#include "skeleton/renderer.h"

namespace c2d {

    class GLRenderer : public Renderer {

    public:

        GLRenderer(const Vector2f &size = Vector2f(0, 0));

        ~GLRenderer();

        virtual void draw(const VertexArray &vertices,
                          const Transform &transform,
                          const Texture *texture);

        virtual void flip(bool draw = true);

        unsigned int vbo = 0;
        unsigned int vbo_offset = 0;
        unsigned int vao = 0;
    };
}

#endif //CROSS2D_RENDERERGL_H
