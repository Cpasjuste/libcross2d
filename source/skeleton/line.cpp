//
// Created by cpasjuste on 13/12/17.
//

#include <cmath>
#include <cstdio>
#include <c2d.h>

using namespace c2d;

Line::Line(const Vector2f &p1, const Vector2f &p2, float width) : Widget() {

    setPosition(p1);

    float distance = (float) sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    setSize(Vector2f(distance, width));

    float rotation = (float) (std::atan2(p2.y - p1.y, p2.x - p1.x) * (180 / 3.141593));
    setRotation(rotation);

    printf("Line(%p): %ix%i (r=%f)\n",
           this, (int) getSize().x, (int) getSize().y, rotation);
}

std::size_t Line::getPointCount() const {

    return getSize().y > 1 ? 4 : 2;
}

void Line::draw(Transform &transform) {

    // draw rect from renderer
    c2d_renderer->drawRectangle((Rectangle &) *this, transform);

    // call base class (draw childs)
    Widget::draw(transform);
}

Line::~Line() {
    printf("~Line(%p)\n", this);
}
