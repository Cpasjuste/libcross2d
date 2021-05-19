//
// Created by cpasjuste on 09/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

Renderer *c2d_renderer;

#ifdef __PHYSFS_HOOKS__

extern int romfsInit();

extern int romfsExit();

#endif

#ifdef __BOX2D__
b2Vec2 m_gravity(0.0f, 10.0f);
b2World m_world(m_gravity);
#endif

Renderer::Renderer(const Vector2f &size) : Rectangle(size) {

    printf("Renderer(%p)\n", this);

    c2d_renderer = this;

#ifdef __PHYSFS_HOOKS__
    romfsInit();
#endif

    m_input = new C2DInput();
    m_input->setJoystickMapping(0, C2D_DEFAULT_JOY_KEYS);
    m_input->setKeyboardMapping(C2D_DEFAULT_KB_KEYS);

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
        for (auto &player : m_input->players) {
            unsigned int keys = player.keys;
            if (keys > 0 && keys != Input::Key::Delay) {
                onInput(m_input->players);
                break;
            }
        }
    }

#ifdef __BOX2D__
    m_world.Step(m_deltaTime.asSeconds(), m_velocityIterations, m_positionIterations);
#endif

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

#ifdef __PHYSFS_HOOKS__
    romfsExit();
#endif
}
