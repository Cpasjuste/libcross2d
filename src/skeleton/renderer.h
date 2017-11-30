//
// Created by cpasjuste on 21/11/16.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <cstdio>

#include "font.h"
#include "texture.h"
#include "shaders.h"
#include "rect.h"
#include "color.h"

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

class Renderer {

public:

    Renderer(int w, int h);

    virtual ~Renderer();

    virtual void SetShader(int shader) {};  // to implement

    void DrawLine(int x1, int y1, int x2, int y2);

    virtual void DrawLine(int x1, int y1, int x2, int y2, const Color &color) {};  // to implement
    virtual void DrawRect(const Rect &rect, const Color &color, bool fill = true) {};  // to implement
    void DrawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool fill = true);

    void DrawBorder(const Rect &rect, const Color &color, bool inside = true);

    virtual void Clip(const Rect &rect) {};  // to implement

    virtual void Clear() {};  // to implement
    virtual void Flip() {};  // to implement

    virtual void Delay(unsigned int ms) {};  // to implement

    int width;
    int height;
    Color color;
    Shaders *shaders;
};


#endif //_RENDERER_H_
