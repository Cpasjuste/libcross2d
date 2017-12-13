//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "skeleton/renderer.h"

using namespace C2D;

Line::Line(int x1, int x2, int y1, int y2, float rotation, const Color &color)
        : Widget(x1, x2, y1, y2, rotation, color) {

    printf("Line(%p): x:%i, y:%i, w:%i, h:%i\n",
           this, rect.x, rect.y, rect.w, rect.h);
}

void Line::Draw() {

    printf("Line(%p): Draw\n", this);

    // update (this) widget position/scaling
    Widget::Update();

    // draw
    ((Renderer *) renderer)->DrawLine(this);

    // call base class (draw childs)
    Widget::Draw();
}

Line::~Line() {
    printf("~Line(%p)\n", this);
}
