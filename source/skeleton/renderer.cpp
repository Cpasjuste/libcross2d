//
// Created by cpasjuste on 09/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

Renderer *c2d_renderer;

Renderer::Renderer(const Vector2f &size) : Rectangle(size) {

    printf("Renderer(%p)\n", this);

    c2d_renderer = this;

    deltaClock = new C2DClock();
    fpsClock = new C2DClock();
    drawTimer = new C2DClock();

    input = new C2DInput();
    input->setJoystickMapping(0, C2D_DEFAULT_JOY_KEYS);
    input->setKeyboardMapping(C2D_DEFAULT_KB_KEYS);

    io = new C2DIo();

    font = new C2DFont();
    font->loadDefault();
}

void Renderer::onUpdate() {

    deltaTime = deltaClock->restart();
    fps = 1.f / fpsClock->restart().asSeconds();

    if (process_inputs) {
        input->update();
        for (auto &player : input->players) {
            unsigned int keys = player.keys;
            if (keys > 0 && keys != Input::Key::Delay) {
                onInput(input->players);
                break;
            }
        }
    }

    Rectangle::onUpdate();
}

void Renderer::flip(bool draw, bool inputs) {

    process_inputs = inputs;
    onUpdate();

    // call base class (draw childs)
    if (draw) {
        clear();
        Transform trans = Transform::Identity;
        draw_calls_batched = draw_calls = 0;
        drawTimer->restart();
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

Time Renderer::getDeltaTime() const {

    return deltaTime;
}

float Renderer::getFps() const {

    return fps;
}

void Renderer::setShaderList(ShaderList *list) {
    shaderList = list;
}

ShaderList *Renderer::getShaderList() {
    return shaderList;
}

Renderer::~Renderer() {

    printf("~Renderer(%p)\n", this);

    delete (font);
    delete (io);
    delete (input);
    delete (deltaClock);
    delete (fpsClock);
    delete (drawTimer);
    if (shaderList != nullptr) {
        delete (shaderList);
    }
}
