//
// Created by cpasjuste on 21/11/16.
//

#if __VITA2D__

#ifndef _PSP2_RENDERER_VITA2D_H_
#define _PSP2_RENDERER_VITA2D_H_

#include "cross2d/skeleton/renderer.h"

namespace c2d {

    class PSP2Renderer : public Renderer {

    public:

        explicit PSP2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~PSP2Renderer() override;

        void draw(VertexArray *vertexArray, const Transform &transform,
                  Texture *texture, Sprite *sprite = nullptr) override;

        void flip(bool draw = true, bool inputs = true) override;

        void delay(unsigned int ms) override;
    };
}

#endif //_PSP2_RENDERER_VITA2D_H_
#endif // __VITA2D__
