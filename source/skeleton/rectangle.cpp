//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "c2d.h"

using namespace c2d;

Rectangle::Rectangle(const FloatRect &rect) : Widget(rect) {

    printf("Rectangle(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

Rectangle::Rectangle(const Vector2f &size) : Widget(size) {

    printf("Rectangle(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void Rectangle::draw(Transform &transform) {

    printf("Rectangle(%p): draw\n", this);

    // draw rect from renderer
    c2d_renderer->drawRectangle(*this, transform);

    // call base class (draw childs)
    Widget::draw(transform);
}

Rectangle::~Rectangle() {
    printf("~Rectangle(%p)\n", this);
}
