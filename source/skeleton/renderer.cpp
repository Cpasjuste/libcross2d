//
// Created by cpasjuste on 09/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

Renderer *c2d_renderer = nullptr;

#ifdef __FUZE_FS__

#include "cross2d/platforms/posix/posix_romfs.h"

POSIXRomFs *c2d_romFs = nullptr;
#endif

#ifdef __SWITCH__

void initNxLink();

void deinitNxLink();

#endif

Renderer::Renderer(const Vector2f &size) : Rectangle(size) {
#ifdef __SWITCH__
    initNxLink();
#endif
    printf("Renderer(%p)\n", this);

    c2d_renderer = this;

    m_input = new C2DInput();
    m_io = new C2DIo();
    m_font = new C2DFont();
    m_font->loadDefault();
    m_elapsedClock = new C2DClock();
    m_deltaClock = new C2DClock();
}

void Renderer::onUpdate() {
    // time
    m_deltaTime = m_deltaClock->restart();

    // stats
    m_fpsStats += 1.f / m_deltaTime.asSeconds();
    m_statsTime += m_deltaTime;
    if (m_statsTime.asSeconds() >= 1) {
        m_fps = m_fpsStats / (float) m_frames;
        if (m_stats_print) {
            printf("fps: %f\n", m_fps);
        }
        m_statsTime = Time::Zero_;
        m_fpsStats = 0;
        m_frames = 0;
    }
    m_frames++;

    // input
    if (m_process_inputs) {
        m_input->update();
        for (int i = 0; i < PLAYER_MAX; i++) {
            unsigned int keys = m_input->getPlayer(i)->buttons;
            if (keys > 0 && keys != Input::Button::Delay) {
                onInput(m_input->getPlayers());
                break;
            }
        }
    }

    Rectangle::onUpdate();
}

void Renderer::flip(bool draw, bool inputs) {
    m_process_inputs = inputs;
    onUpdate();

    // call base class (draw childs)
    if (draw) {
        clear();
        Transform trans = Transform::Identity;
        m_draw_calls_batched = m_draw_calls = 0;
        Rectangle::onDraw(trans, draw);
        //printf("time: %f, draw call: %i, batched: %i\n",
        //       drawTimer->getElapsedTime().asSeconds(), draw_calls, draw_calls_batched);
    }
}

void Renderer::setClearColor(const Color &color) {
    m_clearColor = color;
}

Color Renderer::getClearColor() const {
    return m_clearColor;
}

Time Renderer::getElapsedTime() const {
    return m_elapsedClock->getElapsedTime();
}

Time Renderer::getDeltaTime() const {
    return m_deltaTime;
}

float Renderer::getFps() const {
    return m_fps;
}

void Renderer::setShaderList(ShaderList *list) {
    m_shaderList = list;
}

ShaderList *Renderer::getShaderList() {
    return m_shaderList;
}

Renderer::~Renderer() {
    printf("~Renderer(%p)\n", this);

    delete (m_font);
    delete (m_io);
    delete (m_input);

    delete (m_elapsedClock);
    delete (m_deltaClock);

    if (m_shaderList != nullptr) {
        delete (m_shaderList);
    }

#ifdef __FUZE_FS__
    if (c2d_romFs) {
        delete (c2d_romFs);
    }
#endif

#ifdef __SWITCH__
    deinitNxLink();
#endif
}
