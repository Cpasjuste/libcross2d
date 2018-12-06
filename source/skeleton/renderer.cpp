//
// Created by cpasjuste on 09/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

Renderer *c2d_renderer;

Renderer::Renderer(const Vector2f &size) : Transformable() {

    printf("Renderer(%p)\n", this);

    c2d_renderer = this;
    setSize(size);

    deltaClock = new C2DClock();
    elapsedClock = new C2DClock();

    input = new C2DInput();
    input->setJoystickMapping(0, C2D_DEFAULT_JOY_KEYS);
    input->setKeyboardMapping(C2D_DEFAULT_KB_KEYS);

    io = new C2DIo();

    font = new C2DFont();
    font->loadDefault();
}

void Renderer::setSize(const Vector2f &size) {
    m_size = size;
}

void Renderer::setSize(float width, float height) {
    m_size.x = width;
    m_size.y = height;
}

const Vector2f Renderer::getSize() const {
    return m_size;
}

void Renderer::onInput(Input::Player *players) {
    C2DObject::onInput(players);
}

void Renderer::flip(bool draw) {

    // update inputs
    input->update();
    onInput(input->getPlayers());

    // call base class (draw childs)
    if (draw) {
        Transform trans = getTransform();
        onDraw(trans);
    }

    deltaTime = deltaClock->restart();
    elapsedTime = elapsedClock->getElapsedTime();

    frames++;
    time_now = elapsedTime.asSeconds();
    if (time_now - time_last >= 1.0f) {
        fps = frames;
        frames = 0;
        time_last = time_now;
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
