//
// Created by cpasjuste on 09/12/16.
//

#include "c2d.h"

using namespace c2d;

Renderer::Renderer(const Vector2f &size) : Widget(size) {

    shaders = new Shaders("");

    printf("Renderer(%p)\n", this);
}

void Renderer::Flip() {

    printf("Renderer(%p): Flip\n", this);

    // call base class (draw childs)
    Widget::draw(this, getTransform());
}

void Renderer::SetShader(int shader) {

}

void Renderer::Delay(unsigned int ms) {

}

Renderer::~Renderer() {
    printf("~Renderer(%p)\n", this);
}
