//
// Created by cpasjuste on 13/12/17.
//

#include <cstdio>
#include "skeleton/renderer.h"

using namespace C2D;

Line::Line(int x1, int y1, int x2, int y2,
           const Color &color,
           int center,
           float rot)
        : Widget(x1, y1, x2, y2, color, center, rot) {

    printf("Line(%p): x:%i, y:%i, w:%i, h:%i\n", this,
           (int) rect_origin.x, (int) rect_origin.y,
           (int) rect_origin.w, (int) rect_origin.h);
}

/*
void Line::Update() {

    printf("Line(%p): Update\n", this);

    rect_local.x = pivot == C2D_PIVOT_CENTER ? rect.x - rect.w / 2 : rect.x;
    rect_local.y = pivot == C2D_PIVOT_CENTER ? rect.y - rect.h / 2 : rect.y;
    rect_local.w = pivot == C2D_PIVOT_CENTER ? rect.w - rect.x / 2 : rect.w;
    rect_local.h = pivot == C2D_PIVOT_CENTER ? rect.h - rect.y / 2 : rect.h;

    if (parent) {
        rect_local.x *= parent->GetScaling().x;
        rect_local.y *= parent->GetScaling().y;
        rect_local.w *= parent->GetScaling().x;
        rect_local.h *= parent->GetScaling().y;
        rect_world.x = rect_local.x + parent->GetRectWorld().x;
        rect_world.y = rect_local.y + parent->GetRectWorld().y;
        rect_world.w = rect_local.w + parent->GetRectWorld().x;
        rect_world.h = rect_local.h + parent->GetRectWorld().y;
    } else {
        rect_world.x = rect_local.x;
        rect_world.y = rect_local.y;
        rect_world.w = rect_local.w;
        rect_world.h = rect_local.h;
    }
}
*/

void Line::draw(Renderer *renderer) {

    printf("Line(%p): Draw\n", this);

    // draw
    renderer->DrawLine(this);

    // call base class (draw childs)
    Widget::draw(renderer);
}

Line::~Line() {
    printf("~Line(%p)\n", this);
}
