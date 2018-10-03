//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "c2d.h"

using namespace c2d;

RoundedRectangle::RoundedRectangle(
        const c2d::Vector2f &size, float radius,
        unsigned int cornerPointCount)
        : sfml::RoundedRectangleShape(size, radius, cornerPointCount) {

    type = TRectangle;

    //printf("RoundedRectangle(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void RoundedRectangle::draw(Transform &transform) {

    //printf("Rectangle(%p): draw\n", this);

    // draw rect from renderer
    c2d_renderer->drawRoundedRectangle(*this, transform);

    // call base class (draw childs)
    C2DObject::draw(transform);
}

RoundedRectangle::~RoundedRectangle() {
    //printf("~Rectangle(%p)\n", this);
}
