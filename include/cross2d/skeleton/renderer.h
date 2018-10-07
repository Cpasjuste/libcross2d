//
// Created by cpasjuste on 21/11/16.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <cstdio>

#include "texture.h"
#include "shader_list.h"
#include "cross2d/skeleton/sfml/RectangleShape.hpp"
#include "cross2d/skeleton/sfml/Clock.hpp"

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

namespace c2d {

    class Renderer : public RectangleShape {

    public:

        explicit Renderer(const Vector2f &size = Vector2f(0, 0));

        ~Renderer() override;

        void setSize(const c2d::Vector2f &size) override;

        void setSize(float width, float height) override;

        void draw(Transform &transform) override {};

        virtual void draw(VertexArray *vertexArray,
                          const Transform &transform,
                          const Texture *texture) {};

        virtual void clear() {};

        virtual void flip(bool draw = true);

        virtual void delay(unsigned int ms) {};

        virtual ShaderList *getShaderList();

        virtual void setShaderList(ShaderList *list);

        Time getDeltaTime() const;

        Time getElapsedTime() const;

        float getFps() const;

    protected:

        ShaderList *shaderList = nullptr;
        Clock *deltaClock = nullptr, *elapsedClock = nullptr;
        Time deltaTime, elapsedTime;
        float time_now = 0, time_last = 0, fps = 0;
        int frames = 0;
    };
}

#endif //_RENDERER_H_
