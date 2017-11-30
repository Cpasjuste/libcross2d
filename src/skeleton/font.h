//
// Created by cpasjuste on 01/12/16.
//

#ifndef _FONT_H_
#define _FONT_H_

#include "renderer.h"
#include "rect.h"
#include "color.h"

class Renderer;

class Font {

public:

    // START - to implement, device specific code
    Font(Renderer *renderer, const char *path, int size);
    virtual ~Font() {};

    virtual Font *Load(const char *path, int size) { return NULL; };
    virtual void Draw(int x, int y, const char *fmt, ...) {};
    virtual int GetWidth(const char *fmt, ...) { return 0; };
    virtual int GetHeight(const char *fmt, ...) { return 0; };
    // END - to implement, device specific code

    void Draw(const Rect &dst, const Color &color, const char *fmt, ...);
    void Draw(const Rect &dst, const Color &color, bool centerX, bool centerY, const char *fmt, ...);

    char path[512];
    Color color{255, 255, 255, 255};
    int size = 20;
    float scaling = 1;
    Renderer *renderer;
};

#endif //_FONT_H
