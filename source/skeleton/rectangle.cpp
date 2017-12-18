//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "skeleton/renderer.h"

using namespace c2d;

Rectangle::Rectangle(const Vector2f &size, bool fill) : Widget(size) {

    this->fill = fill;

    printf("Rectangle(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void Rectangle::draw(Renderer *renderer, const Transform &transform) {

    printf("Rectangle(%p): draw\n", this);

    // update (this) widget position/scaling
    // Widget::Update();

    // draw

    //sf::Transform combinedTransform = transform * getTransform();
    FloatRect combined = transform.transformRect(getGlobalBounds());
    renderer->DrawRectangle(combined, getFillColor());

    // call base class (draw childs)
    Widget::draw(renderer, transform);
}

Rectangle::~Rectangle() {
    printf("~Rectangle(%p)\n", this);
}
