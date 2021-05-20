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
#include "cross2d/skeleton/sfml/Sprite.hpp"
#include "cross2d/skeleton/sfml/Rectangle.hpp"
#include "cross2d/skeleton/sfml/Clock.hpp"
#include "cross2d/skeleton/sfml/Font.hpp"
#include "cross2d/skeleton/input.h"
#include "cross2d/skeleton/io.h"

#ifdef __BOX2D__

#include <Box2D/Box2D.h>

extern b2World m_world;
#endif

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

namespace c2d {

    class Renderer : public Rectangle {

    public:

        explicit Renderer(const Vector2f &size = Vector2f(0, 0));

        ~Renderer() override;

        virtual void draw(VertexArray *vertexArray, const Transform &transform,
                          Texture *texture, Sprite *sprite = nullptr) {};

        virtual void clear() {};

        virtual void flip(bool draw = true, bool process_inputs = true);

        virtual void delay(unsigned int ms) {};

        virtual ShaderList *getShaderList();

        virtual void setShaderList(ShaderList *list);

        void setClearColor(const Color &color);

        Color getClearColor() const;

        Time getDeltaTime() const;

        Time getElapsedTime() const;

        float getFps() const;

        virtual Io *getIo() { return m_io; };

        virtual void setIo(Io *io) {
            if (m_io) { delete (m_io); }
            m_io = io;
        };

        virtual Font *getFont() { return m_font; };

        virtual void setFont(Font *font) {
            if (m_font) { delete (m_font); }
            m_font = font;
        };

        virtual Input *getInput() { return m_input; };

        virtual void setInput(Input *input) {
            if (m_input) { delete (m_input); }
            m_input = input;
        };

        virtual void setPrintStats(bool enable) {
            m_stats_print = enable;
        }

        virtual int getDrawCalls() {
            return m_draw_calls;
        }

        virtual int getDrawCallsBatched() {
            return m_draw_calls_batched;
        }

#ifdef __BOX2D__

        b2World *getPhysicsWorld() {
            return &m_world;
        }

        void setPhysicsPaused(bool paused) {
            physicsPaused = paused;
        }

        void setPixelsPerMeter(float ppm) {
            m_pixelsPerMeter = ppm;
        }

        float getPixelsPerMeter() {
            return m_pixelsPerMeter;
        }

#endif

    protected:

        void onUpdate() override;

        Color m_clearColor = Color::Black;
        bool m_process_inputs = true;
        Input *m_input = nullptr;
        Io *m_io = nullptr;
        Font *m_font = nullptr;
        ShaderList *m_shaderList = nullptr;
        Clock *m_elapsedClock, *m_deltaClock;
        Time m_deltaTime, m_statsTime;
        float m_fps = 0, m_fpsStats = 0;
        int m_frames = 0;
        int m_draw_calls;
        int m_draw_calls_batched;
        bool m_stats_print = false;

#ifdef __BOX2D__
        int32 m_velocityIterations = 6;
        int32 m_positionIterations = 2;
        float m_pixelsPerMeter = 32;
        bool physicsPaused = false;
#endif
    };
}

#endif //_RENDERER_H_
