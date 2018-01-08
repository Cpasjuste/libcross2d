//
// Created by cpasjuste on 21/11/16.
//

#ifndef PSP2OSMESA_RENDERER_H
#define PSP2OSMESA_RENDERER_H

#include "skeleton/renderergl.h"

namespace c2d {

    class PSP2OSMESARenderer : public GLRenderer {

    public:

        PSP2OSMESARenderer(const Vector2f &size = Vector2f(0, 0));

        ~PSP2OSMESARenderer();

        void flip();

        void delay(unsigned int ms);
    };
}

#endif //PSP2OSMESA_RENDERER_H
