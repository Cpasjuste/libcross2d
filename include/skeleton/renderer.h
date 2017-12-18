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
#include "rectangle.h"
//#include "line.h"

//#include "font.h"
//#include "texture.h"
#include "shaders.h"

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

namespace c2d {

    class Renderer : public Widget {

    public:

        Renderer(const Vector2f &size = Vector2f(0, 0));

        virtual ~Renderer();

        virtual void DrawRectangle(const FloatRect &rect, const Color &color) {};

        virtual void Flip();

        virtual void SetShader(int shader);

        virtual void Delay(unsigned int ms);

    protected:

        Shaders *shaders;
    };
}

#endif //_RENDERER_H_
