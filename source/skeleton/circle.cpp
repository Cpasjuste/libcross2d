//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "c2d.h"

using namespace c2d;

Circle::Circle(float radius, std::size_t pointCount) : sfml::CircleShape(radius, pointCount) {

    type = TCircle;

    //printf("Circle(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void Circle::draw(Transform &transform) {

    //printf("Circle(%p): draw\n", this);

    // draw rect from renderer
    c2d_renderer->drawCircle(*this, transform);

    // call base class (draw childs)
    C2DObject::draw(transform);
}

Circle::~Circle() {
    //printf("~Circle(%p)\n", this);
}
