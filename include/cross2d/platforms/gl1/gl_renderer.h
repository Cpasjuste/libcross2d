//
// Created by cpasjuste on 06/01/18.
//

#ifndef CROSS2D_GL1RENDER_H
#define CROSS2D_GL1RENDER_H

#include "cross2d/skeleton/renderer.h"

namespace c2d {

    class GL1Renderer : public Renderer {

    public:

        explicit GL1Renderer(const Vector2f &size = Vector2f(0, 0));

        ~GL1Renderer() override;

        virtual void glInit();

        void draw(VertexArray *vertexArray, const Transform &transform,
                  Texture *texture, Sprite *sprite = nullptr) override;

        void clear() override;

        void flip(bool draw = true, bool inputs = true) override;
    };
}

#endif //CROSS2D_GL1RENDER_H
