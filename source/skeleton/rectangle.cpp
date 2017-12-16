//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "skeleton/renderer.h"

using namespace C2D;

Rectangle::Rectangle(int x, int y, int w, int h,
                     const Color &color,
                     int center,
                     float rot,
                     bool fill)
        : Widget(x, y, w, h, color, center, rot) {

    this->fill = fill;

    printf("Rectangle(%p): x:%i, y:%i, w:%i, h:%i\n", this,
           (int) rect_origin.x, (int) rect_origin.y,
           (int) rect_origin.w, (int) rect_origin.h);
}

void Rectangle::draw(Renderer *renderer) {

    printf("Rectangle(%p): Draw\n", this);

    // draw
    renderer->DrawRectangle(this);

    // call base class (draw childs)
    Widget::draw(renderer);
}

Rectangle::~Rectangle() {
    printf("~Rectangle(%p)\n", this);
}
