//
// Created by cpasjuste on 21/11/16.
//

#ifndef _PSP2_RENDERER_H_
#define _PSP2_RENDERER_H_

#include "cross2d/skeleton/renderer.h"

namespace c2d {

    class PSP2Renderer : public Renderer {

    public:

        PSP2Renderer(const Vector2f &size = Vector2f(0, 0));

        ~PSP2Renderer();

        void draw(VertexArray *vertexArray,
                  const Transform &transform,
                  const Texture *texture);

        void flip(bool draw = true);

        void delay(unsigned int ms);
    };
}

#endif //_PSP2_RENDERER_H_
