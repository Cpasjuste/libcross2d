//
// Created by cpasjuste on 21/11/16.
//

#ifndef _NX_RENDERER_H_
#define _NX_RENDERER_H_

#include <switch.h>
#include "../skeleton/renderer.h"

class NXRenderer : Renderer {

public:
    NXRenderer();

    ~NXRenderer();

    Font *LoadFont(const char *path, int size);

    void DrawFont(Font *font, int x, int y, const char *fmt, ...);

    Texture *CreateTexture(int w, int h);

    Texture *LoadTexture(const char *file);

    void DrawTexture(Texture *texture, int x, int y, int w, int h, float rotation);

    int LockTexture(Texture *texture, const Rect &rect, void **pixels, int *pitch);

    void DrawLine(int x1, int y1, int x2, int y2, const Color &color);

    void DrawRect(const Rect &rect, const Color &color, bool fill = true);

    void Clip(const Rect &rect);

    const Rect GetWindowSize();

    void Clear();

    void Flip();

    void Delay(unsigned int ms);

    void SetShader(int shader);

private:

};

#endif //_NX_RENDERER_H_
