//
// Created by cpasjuste on 06/01/18.
//

#ifndef CROSS2D_RENDERERGLES_H
#define CROSS2D_RENDERERGLES_H

#include "skeleton/renderer.h"

namespace c2d {

    class GLESRenderer : public Renderer {

    public:

        GLESRenderer(const Vector2f &size = Vector2f(0, 0));

        ~GLESRenderer();

        virtual void draw(const VertexArray &vertices,
                          const Transform &transform,
                          const Texture *texture);

        virtual void flip(bool draw = true);

        virtual void setSize(const c2d::Vector2f &size);

        virtual void setSize(float width, float height);
    };
}

#endif //CROSS2D_RENDERERGLES_H
