//
// Created by cpasjuste on 12/12/17.
//

#include <cstdio>
#include "widget.h"

using namespace C2D;

Widget::Widget(int posX, int posY, int width, int height,
               float rotation, const Color &color) {

    this->renderer = renderer;

    SetPosition(posX, posY, width, height, rotation);
    SetColor(color);

    printf("Widget(%p): x:%x, y:%x, w:%x, h:%x\n",
           this, rect.x, rect.y, rect.w, rect.h);
}

void Widget::Add(Widget *widget) {

    if (widget) {
        widget->parent = this;
        childs.push_back(widget);
    }
}

void Widget::Draw() {

    printf("Widget(%p): Draw\n", this);
    for (Widget *widget : childs) {
        widget->Draw();
    }
}

Rect Widget::GetPosition() {
    return rect;
}

void Widget::SetPosition(const Rect &rect) {
    SetPosition(rect.x, rect.y, rect.w, rect.h, rotation);
}

void Widget::SetPosition(int posX, int posY, int width, int height, float rot) {

    rect.x = posX;
    rect.y = posY;
    rect.w = width;
    rect.h = height;
    rotation = rot;
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

void Widget::Scale(int size) {
    rect.x -= size;
    rect.y -= size;
    rect.w += size * 2;
    rect.h += size * 2;
}

Widget::~Widget() {

    printf("~Widget(%p)\n", this);

    // delete childs
    for (Widget *drawable : childs) {
        if (drawable) {
            printf("~Widget(%p): delete child(%p)\n", this, drawable);
            delete (drawable);
        }
    }

    // remove from parent
    for (int i = 0; i < parent->childs.size(); i++) {
        if (parent->childs[i] == this) {
            printf("~Widget(%p): remove from parent(%p)\n", this, parent);
            parent->childs.erase(parent->childs.begin() + i);
            break;
        }
    }

    printf("~Widget(%p)\n", this);
}
