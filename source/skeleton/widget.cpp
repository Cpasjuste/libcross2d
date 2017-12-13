//
// Created by cpasjuste on 12/12/17.
//

#include <cstdio>
#include "C2D.h"

using namespace C2D;

Widget::Widget(int posX, int posY, int width, int height,
               float rotation, const Color &color) {

    renderer = C2DMainRenderer;
    SetRect(posX, posY, width, height);
    SetRotation(rotation);
    SetColor(color);

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

void Widget::SetRect(const Rect &rect) {
    SetRect(rect.x, rect.y, rect.w, rect.h);
}

void Widget::SetRect(int posX, int posY, int width, int height) {

    rect.x = posX;
    rect.y = posY;
    rect.w = width;
    rect.h = height;

    if (rect.w < 0) {
        rect.w = 0;
    }
    if (rect.h < 0) {
        rect.h = 0;
    }
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
    return scaling;
}

void Widget::SetScaling(float s) {
    scaling = s;
}

void Widget::Scale(int pixels) {
    rect.x -= pixels;
    rect.y -= pixels;
    rect.w += pixels * 2;
    rect.h += pixels * 2;
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
