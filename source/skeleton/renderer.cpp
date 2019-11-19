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
    elapsedClock = new C2DClock();

    input = new C2DInput();
    input->setJoystickMapping(0, C2D_DEFAULT_JOY_KEYS);
    input->setKeyboardMapping(C2D_DEFAULT_KB_KEYS);

    io = new C2DIo();

    font = new C2DFont();
    font->loadDefault();
}

void Renderer::onUpdate() {

    deltaTime = deltaClock->restart();
    elapsedTime = elapsedClock->getElapsedTime();

    frames++;
    time_now = elapsedTime.asSeconds();
    if (time_now - time_last >= 1.0f) {
        fps = (float) frames;
        frames = 0;
        time_last = time_now;
    }

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

void Renderer::flip(bool draw, bool _process_inputs) {

    process_inputs = _process_inputs;

    // call base class (draw childs)
    if (draw) {
        Transform trans = getTransform();
        onDraw(trans, draw);
    }
}

void Renderer::setClearColor(const Color &color) {
    m_clearColor = color;
}

const Color Renderer::getClearColor() {
    return m_clearColor;
}

Time Renderer::getDeltaTime() const {

    return deltaTime;
}

Time Renderer::getElapsedTime() const {

    return elapsedTime;
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
    delete (elapsedClock);
    if (shaderList) {
        delete (shaderList);
    }
}
