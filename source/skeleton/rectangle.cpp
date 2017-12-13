//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "skeleton/renderer.h"
#include "skeleton/rectangle.h"

using namespace C2D;

Rectangle::Rectangle(
        int posX, int posY, int width, int height,
        float rotation, const Color &color, bool fill)
        : Widget(posX, posY, width, height, rotation, color) {

    this->fill = fill;

    printf("Rectangle(%p): x:%i, y:%i, w:%i, h:%i\n",
           this, rect.x, rect.y, rect.w, rect.h);
}

void Rectangle::Draw() {

    printf("Rectangle(%p): Draw\n", this);

    // update (this) widget position/scaling
    Widget::Update();

    // draw
    ((Renderer *) renderer)->DrawRectangle(this);

    // call base class (draw childs)
    Widget::Draw();
}

Rectangle::~Rectangle() {
    printf("~Rectangle(%p)\n", this);
}
