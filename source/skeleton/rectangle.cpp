//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include <c2d.h>
#include "skeleton/renderer.h"

using namespace c2d;

Rectangle::Rectangle(const FloatRect &rect) : Widget(rect) {

    printf("Rectangle(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

Rectangle::Rectangle(const Vector2f &size) : Widget(size) {

    printf("Rectangle(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void Rectangle::draw(const Transform &transform, const Vector2f &scaling) {

    // draw rect from renderer
    printf("Rectangle(%p): draw: scaling = %f\n", this, scaling.x);
    c2d_renderer->drawRectangle(*this, transform, scaling);

    // call base class (draw childs)
    Widget::draw(transform, scaling);
}

Rectangle::~Rectangle() {
    printf("~Rectangle(%p)\n", this);
}
