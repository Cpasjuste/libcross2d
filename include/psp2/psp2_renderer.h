//
// Created by cpasjuste on 21/11/16.
//

#ifndef _PSP2_RENDERER_H_
#define _PSP2_RENDERER_H_

#include "../skeleton/renderer.h"

#ifdef __PSP2_DEBUG__
#include <psp2/kernel/clib.h>
#define printf sceClibPrintf
#endif

class PSP2Renderer : Renderer {

public:
    PSP2Renderer(int w, int h);

    ~PSP2Renderer();

    void DrawLine(int x1, int y1, int x2, int y2, const Color &color);

    void DrawRect(const Rect &rect, const Color &color, bool fill);

    void Clip(const Rect &rect);

    void Clear();

    void Flip();

    void Delay(unsigned int ms);

    void SetShader(int shader);

    void StartDrawing();

private:

    bool drawing_started = false;
};

#endif //_PSP2_RENDERER_H_
