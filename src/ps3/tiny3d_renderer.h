//
// Created by cpasjuste on 21/11/16.
//

#ifndef TINY3D_RENDERER_H
#define TINY3D_RENDERER_H

#include "skeleton/renderer.h"

class TINY3DRenderer : Renderer {

public:
    TINY3DRenderer(int w, int h);

    virtual ~TINY3DRenderer();

    virtual void DrawLine(int x1, int y1, int x2, int y2, const Color &color);

    virtual void DrawRect(const Rect &rect, const Color &color, bool fill);

    virtual void Clear();

    virtual void Flip();

    virtual void Delay(unsigned int ms);

    virtual void SetShader(int shader);

};

#endif //TINY3D_RENDERER_H
