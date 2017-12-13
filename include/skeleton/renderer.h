//
// Created by cpasjuste on 21/11/16.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <cstdio>

#include "widget.h"
//#include "font.h"
//#include "texture.h"
#include "shaders.h"

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

namespace C2D {

    class Renderer : public Widget {

    public:

        Renderer(int x = 0, int y = 0, int w = 0, int h = 0);

        virtual ~Renderer();

        /*
        void DrawLine(int x1, int y1, int x2, int y2);
        virtual void DrawLine(int x1, int y1, int x2, int y2, const Color &color) {};  // to implement
        virtual void DrawRect(const Rect &rect, const Color &color, bool fill = true) {};  // to implement
        void DrawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool fill = true);
        */

        virtual void Draw();                    // to implement
        virtual void SetShader(int shader) {};  // to implement
        virtual void Delay(unsigned int ms) {}; // to implement

    protected:

        Shaders *shaders;
    };
}

#endif //_RENDERER_H_
