//
// Created by cpasjuste on 09/12/16.
//

#include "c2d.h"

using namespace c2d;

Renderer *c2d_renderer;

Renderer::Renderer(const Vector2f &size) : RectangleShape(size) {

    printf("Renderer(%p)\n", this);

    c2d_renderer = this;
    setFillColor(Color::Black);

    deltaClock = new C2DClock();
    elapsedClock = new C2DClock();
}

void Renderer::setSize(const c2d::Vector2f &size) {
    RectangleShape::setSize(size);
}

void Renderer::setSize(float width, float height) {
    RectangleShape::setSize(width, height);
}

void Renderer::flip(bool draw) {

    //printf("Renderer(%p): flip\n", this);

    // call base class (draw childs)
    if (draw) {
        Transform trans = getTransform();
        Shape::draw(trans);
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

    delete (deltaClock);
    delete (elapsedClock);
    if (shaderList) {
        delete (shaderList);
    }
}
