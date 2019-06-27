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
#include "cross2d/skeleton/sfml/Rectangle.hpp"
#include "cross2d/skeleton/sfml/Clock.hpp"
#include "cross2d/skeleton/sfml/Font.hpp"
#include "cross2d/skeleton/input.h"
#include "cross2d/skeleton/io.h"

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

namespace c2d {

    class Renderer : public Rectangle {

    public:

        explicit Renderer(const Vector2f &size = Vector2f(0, 0));

        ~Renderer() override;

        virtual void draw(VertexArray *vertexArray, const Transform &transform, Texture *texture) {};

        virtual void clear() {};

        virtual void flip(bool draw = true, bool inputs = true);

        virtual void delay(unsigned int ms) {};

        virtual ShaderList *getShaderList();

        virtual void setShaderList(ShaderList *list);

        void setClearColor(const Color &color);

        const Color getClearColor();

        Time getDeltaTime() const;

        Time getElapsedTime() const;

        float getFps() const;

        virtual Io *getIo() { return io; };

        virtual void setIo(Io *_io) {
            if (io) { delete (io); }
            io = _io;
        };

        virtual Font *getFont() { return font; };

        virtual void setFont(Font *_font) {
            if (font) { delete (font); }
            font = _font;
        };

        virtual Input *getInput() { return input; };

        virtual void setInput(Input *_input) {
            if (input) { delete (input); }
            input = _input;
        };

    protected:

        Color m_clearColor = Color::Black;
        Input *input = nullptr;
        Io *io = nullptr;
        Font *font = nullptr;
        ShaderList *shaderList = nullptr;
        Clock *deltaClock = nullptr, *elapsedClock = nullptr;
        Time deltaTime, elapsedTime;
        float time_now = 0, time_last = 0, fps = 0;
        int frames = 0;
    };
}

#endif //_RENDERER_H_
