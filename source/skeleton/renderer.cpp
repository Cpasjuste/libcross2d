//
// Created by cpasjuste on 09/12/16.
//

#include "c2d.h"

using namespace c2d;

Renderer *c2d_renderer;

Renderer::Renderer(const Vector2f &size) : Widget(size) {

    c2d_renderer = this;
    setFillColor(Color::Black);
    shaders = new Shaders("");

    printf("Renderer(%p)\n", this);
}

void Renderer::flip() {

    printf("Renderer(%p): flip\n", this);

    // call base class (draw childs)
    Widget::draw(getTransform(), getScale());
}

void Renderer::setShader(int shader) {

}

void Renderer::delay(unsigned int ms) {

}

Renderer::~Renderer() {

    if (shaders) {
        delete (shaders);
    }

    printf("~Renderer(%p)\n", this);
}
