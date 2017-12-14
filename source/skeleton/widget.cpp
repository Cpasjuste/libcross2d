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
    SetOrigin(center);

    printf("Widget(%p): x:%i, y:%i, w:%i, h:%i\n",
           this, (int) rect.x, (int) rect.y, (int) rect.w, (int) rect.h);
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


    float angle  = -rotation * 3.141592654f / 180.f;
    float cosine = static_cast<float>(std::cos(angle));
    float sine   = static_cast<float>(std::sin(angle));
    float sxc    = scale.x * cosine;
    float syc    = scale.y * cosine;
    float sxs    = scale.x * sine;
    float sys    = scale.y * sine;
    float tx     = -origin.x * sxc - origin.y * sys + rect.x;
    float ty     =  origin.x * sxs - origin.y * syc + rect.y;

    rect_world.x = rect_local.x;
    rect_world.y = rect_local.y;
    rect_world.w = rect_local.w;
    rect_world.h = rect_local.h;

    /*
    switch (pivot) {

        case C2D_PIVOT_TOP_LEFT:
            rect_local.x = rect.x;
            rect_local.y = rect.y;
            rect_local.x += (rect_local.x - rect.x) * scale.x;
            rect_local.y += (rect_local.y - rect.y) * scale.y;
            break;

        case C2D_PIVOT_CENTER:
            rect_local.x = rect.x - rect.w / 2;
            rect_local.y = rect.y - rect.h / 2;
            rect_local.x += (rect_local.x + rect.w / 2) * scale.x;
            rect_local.y += (rect_local.y + rect.h / 2) * scale.y;
            break;

        case C2D_PIVOT_BOTTOM_RIGHT:
            rect_local.x = rect.x - rect.w / 2;
            rect_local.y = rect.y - rect.h / 2;
            rect_local.x += (rect_local.x - rect.x) * scale.x;
            rect_local.y += (rect_local.y - rect.y) * scale.y;
            break;

        default:
            break;
    }

    rect_local.w = rect.w * scale.x;
    rect_local.h = rect.h * scale.y;

    if (parent) {
        rect_local.x *= parent->scale.x;
        rect_local.y *= parent->scale.y;
        rect_local.w *= parent->scale.x;
        rect_local.h *= parent->scale.y;
        rect_world.x = rect_local.x + parent->rect_world.x;
        rect_world.y = rect_local.y + parent->rect_world.y;
        rect_world.w = rect_local.w;
        rect_world.h = rect_local.h;
    } else {
        rect_world.x = rect_local.x;
        rect_world.y = rect_local.y;
        rect_world.w = rect_local.w;
        rect_world.h = rect_local.h;
    }
    */
}

void Widget::Draw() {

    printf("Widget(%p): Draw\n", this);
    for (Widget *widget : childs) {
        if (widget->visibility == C2D_VISIBILITY_VISIBLE) {
            widget->Draw();
        }
    }
}

Vec4 Widget::GetRect() {

    return rect;
}

Vec4 Widget::GetRectWorld() {

    return rect_world;
}

Vec4 Widget::GetRectLocal() {

    return rect_local;
}

void Widget::SetRect(const Vec4 &rect) {

    SetRect((int) rect.x, (int) rect.y,
            rect.w < 0 ? 0 : (int) rect.w,
            rect.h < 0 ? 0 : (int) rect.h);
}

void Widget::SetRect(int x, int y, int w, int h) {

    rect.x = x;
    rect.y = y;
    rect.w = w < 0 ? 0 : w;
    rect.h = h < 0 ? 0 : h;
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

Vec2 Widget::GetScaling() {

    return scale;
}

void Widget::SetScaling(const Vec2 &scaling) {
    scale = scaling;
}

void Widget::Scale(int pixels) {

    rect.x -= pixels;
    rect.y -= pixels;
    rect.w += pixels * 2;
    rect.h += pixels * 2;
}

Vec2 Widget::GetOrigin() {

    return origin;
}

void Widget::SetOrigin(const Vec2 &origin) {

    this->origin = origin;
}

void Widget::SetOrigin(int pivotMode) {

    switch (pivotMode) {

        case C2D_ORIGIN_TOP_RIGHT:
            SetOrigin({0, 0});
            break;

        case C2D_ORIGIN_CENTER:
            SetOrigin({rect.w / 2, rect.h / 2});
            break;

        case C2D_ORIGIN_BOTTOM_LEFT:
            SetOrigin({0, rect.h});
            break;

        case C2D_ORIGIN_BOTTOM_RIGHT:
            SetOrigin({rect.w, rect.h});
            break;

        case C2D_ORIGIN_TOP_LEFT:
        default:
            SetOrigin({0, 0});
            break;
    }
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
