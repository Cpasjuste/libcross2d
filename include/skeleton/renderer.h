//
// Created by cpasjuste on 21/11/16.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <cstdio>

#include "rectangle.h"
#include "line.h"
#include "text.h"
#include "texture.h"
#include "shader_list.h"
#include "skeleton/sfml/Clock.hpp"

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

namespace c2d {

    class Renderer : public C2DObject, public sfml::RectangleShape {

    public:

        Renderer(const Vector2f &size = Vector2f(0, 0));

        virtual ~Renderer();

        virtual void draw(const VertexArray &vertices,
                          const Transform &transform,
                          const Texture *texture) {};

        virtual void drawText(Text &text, Transform &transform);

        virtual void drawTexture(Texture &texture, Transform &transform);

        virtual void drawLine(Line &line, Transform &transform);

        virtual void drawRectangle(Rectangle &rectangle, Transform &transform);

        virtual void flip();

        virtual void delay(unsigned int ms) {};

        virtual ShaderList *getShaderList();

        Time getDeltaTime() const;

    protected:

        ShaderList *shaderList = NULL;
        Clock *deltaClock = NULL;
        Time deltaTime;
    };
}

#endif //_RENDERER_H_
