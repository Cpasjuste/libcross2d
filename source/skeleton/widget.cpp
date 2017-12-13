//
// Created by cpasjuste on 12/12/17.
//

#include <cstdio>
#include "C2D.h"

using namespace C2D;

Widget::Widget(int x, int y, int w, int h,
               const Color &color,
               int center,
               float rot) {

    renderer = C2DMainRenderer;
    SetRect(x, y, w, h);
    SetRotation(rot);
    SetColor(color);
    SetCenter(center);

    printf("Widget(%p): x:%i, y:%i, w:%i, h:%i\n",
           this, rect.x, rect.y, rect.w, rect.h);
}

void Widget::Add(Widget *widget) {

    if (widget) {
        printf("Widget(%p): Add(%p), renderer=%p\n", this, widget, renderer);
        widget->parent = this;
        childs.push_back(widget);
    }
}

void Widget::Update() {

    printf("Widget(%p): Update\n", this);

    rect_abs.x = center ? rect.x - rect.w / 2 : rect.x;
    rect_abs.y = center ? rect.y - rect.h / 2 : rect.y;
    rect_abs.w = (int) ((float) rect.w * scaleX);
    rect_abs.h = (int) ((float) rect.h * scaleY);

    if (center && (scaleX != 1 || scaleY != 1)) {
        rect_abs.x += (rect.w - rect_abs.w) / 2;
        rect_abs.y += (rect.h - rect_abs.h) / 2;
    }

    if (parent) {
        rect_abs.x += parent->rect_abs.x;
        rect_abs.y += parent->rect_abs.y;
        rect_abs.w *= parent->scaleX;
        rect_abs.h *= parent->scaleY;
    }
}

void Widget::Draw() {

    printf("Widget(%p): Draw\n", this);
    for (Widget *widget : childs) {
        if (widget->visibility == C2D_VISIBILITY_VISIBLE) {
            widget->Draw();
        }
    }
}

Rect Widget::GetRect() {

    return rect;
}

Rect Widget::GetRectAbs() {

    return rect_abs;
}

void Widget::SetRect(const Rect &rect) {

    SetRect(rect.x, rect.y,
            rect.w < 0 ? 0 : rect.w,
            rect.h < 0 ? 0 : rect.h);
}

void Widget::SetRect(int posX, int posY, int width, int height) {

    rect.x = posX;
    rect.y = posY;
    rect.w = width < 0 ? 0 : width;
    rect.h = height < 0 ? 0 : height;
}

void Widget::Move(int deltaX, int deltaY) {

    rect.x += deltaX;
    rect.y += deltaY;
}

float Widget::GetRotation() {

    return rotation;
}

void Widget::SetRotation(float rot) {

    rotation = rot;
}

Color Widget::GetColor() {
    return color;
}

void Widget::SetColor(const Color &color) {

    this->color = color;
}

float Widget::GetScaling() {

    return scaleX;
}

void Widget::SetScaling(float s) {
    scaleX = s;
    scaleY = s;
}

void Widget::Scale(int pixels) {

    rect.x -= pixels;
    rect.y -= pixels;
    rect.w += pixels * 2;
    rect.h += pixels * 2;
}

int Widget::GetCenter() {

    return center;
}

void Widget::SetCenter(int center) {

    this->center = center;
}

int Widget::GetVisibility() {

    return visibility;
}

void Widget::SetVisibility(int visibility) {

    this->visibility = visibility;
}

Widget::~Widget() {

    // delete childs
    for (Widget *drawable : childs) {
        if (drawable) {
            printf("~Widget(%p): delete child(%p)\n", this, drawable);
            delete (drawable);
        }
    }

    // remove from parent
    if (parent) {
        for (size_t i = 0; i < parent->childs.size(); i++) {
            if (parent->childs[i] == this) {
                printf("~Widget(%p): remove from parent(%p)\n", this, parent);
                parent->childs.erase(parent->childs.begin() + i);
                break;
            }
        }
    }

    printf("~Widget(%p)\n", this);
}
