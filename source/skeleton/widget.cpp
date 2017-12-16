//
// Created by cpasjuste on 12/12/17.
//

#include <cmath>
#include <cstdio>
#include "C2D.h"

using namespace C2D;

Widget::Widget(int x, int y, int w, int h,
               const Color &color,
               int center,
               float rot) {

    setRect(x, y, w, h);
    setRotation(rot);
    setColor(color);
    setOrigin(center);

    printf("Widget(%p): x:%i, y:%i, w:%i, h:%i\n", this,
           (int) rect_origin.x, (int) rect_origin.y,
           (int) rect_origin.w, (int) rect_origin.h);
}

Widget::Widget(const Vec4 &rect, const Color &color, int center, float rot) {

    setRect((int) rect.x, (int) rect.y, (int) rect.w, (int) rect.h);
    setRotation(rot);
    setColor(color);
    setOrigin(center);

    printf("Widget(%p): x:%i, y:%i, w:%i, h:%i\n",
           this, (int) rect.x, (int) rect.y, (int) rect.w, (int) rect.h);
}

void Widget::add(Widget *widget) {

    if (widget) {
        printf("Widget(%p): add(%p)\n", this, widget);
        widget->parent = this;
        childs.push_back(widget);
    }
}

static Vec2 transform(const Vec2 &vec, const Vec2 &origin, const Vec2 &scaling) {

    float angle = 0;
    float cosine = std::cos(angle);
    float sine = std::sin(angle);
    float sxc = scaling.x * cosine;
    float syc = scaling.y * cosine;
    float sxs = scaling.x * sine;
    float sys = scaling.y * sine;
    float tx = -origin.x * sxc - origin.y * sys + vec.x;
    float ty = origin.x * sxs - origin.y * syc + vec.y;

    return {tx, ty};
}

void Widget::Update() {

    printf("Widget(%p): Update\n", this);

    //if (update_matrix) {

    scaling_world = scaling;
    if (parent) {
        scaling_world.x *= parent->scaling_world.x;
        scaling_world.y *= parent->scaling_world.y;
    }

    Vec2 trans = transform({rect_origin.x, rect_origin.y}, origin, scaling_world);
    rect_origin_translated.x = rect_world.x = trans.x;
    rect_origin_translated.y = rect_world.y = trans.y;
    rect_origin_translated.w = rect_world.w = rect_origin.w * scaling_world.x;
    rect_origin_translated.h = rect_world.h = rect_origin.h * scaling_world.y;

    if (parent) {

        Vec2 trans2 = transform({parent->rect_world.x, parent->rect_world.y}, parent->origin, parent->scaling);
        rect_world.x += trans2.x - rigin.w;
        rect_world.y += trans2.y - rect_origin.h;
        rect_world.w *= scaling_world.x;
        rect_world.h *= scaling_world.y;

        //rect_world.x += (parent->rect_world.x + parent->rect_origin_translated.x) * scaling_world.x;
        //rect_world.y += (parent->rect_world.y + parent->rect_origin_translated.y) * scaling_world.y;

        /*
        Vec2 trans2 = transform({parent->rect_world.x, parent->rect_world.y}, parent->origin,  parent->scaling);
        rect_world.x += trans2.x;
        rect_world.y += trans2.y;
        rect_world.w *= scaling_world.x;
        rect_world.h *= scaling_world.y;
        */
    }

    //rect_world = rect_origin_translated;


    update_matrix = false;
    //}


    /*
    rect_world.x = transform(this).x;
    rect_world.y = transform(this).y;
    rect_world.w = rect.w * scaling.x;
    rect_world.h = rect.h * scaling.y;

    if (parent) {
        rect_world.w *= parent->scaling.x;
        rect_world.h *= parent->scaling.y;
        rect_world.x += parent->rect_world.x + parent->scaling.x * (origin.x);
        rect_world.y += parent->rect_world.y + parent->scaling.y * (origin.y);
    }
    */
}

void Widget::draw(Renderer *renderer) {

    printf("Widget(%p): Draw\n", this);

    // update this widget position
    Update();

    for (Widget *widget : childs) {
        if (widget->visibility == C2D_VISIBILITY_VISIBLE) {
            widget->draw(renderer);
        }
    }
}

Vec4 Widget::getRect() {

    return rect_origin;
}

Vec4 Widget::getRectWorld() {

    return rect_world;
}

void Widget::setRect(const Vec4 &rect) {

    setRect((int) rect.x, (int) rect.y,
            rect.w < 0 ? 0 : (int) rect.w,
            rect.h < 0 ? 0 : (int) rect.h);

    update_matrix = true;
}

void Widget::setRect(int x, int y, int w, int h) {

    rect_origin.x = x;
    rect_origin.y = y;
    rect_origin.w = w < 0 ? 0 : w;
    rect_origin.h = h < 0 ? 0 : h;

    update_matrix = true;
}

void Widget::move(int deltaX, int deltaY) {

    rect_origin.x += deltaX;
    rect_origin.y += deltaY;

    update_matrix = true;
}

float Widget::getRotation() {

    return rotation;
}

void Widget::setRotation(float rot) {

    rotation = rot;

    update_matrix = true;
}

void Widget::rotate(float rot) {

    rotation += rot;

    update_matrix = true;
}

Color Widget::getColor() {

    return color;
}

void Widget::setColor(const Color &color) {

    this->color = color;
}

Vec2 Widget::getScaling() {

    return scaling;
}

void Widget::setScaling(const Vec2 &scaling) {

    this->scaling = scaling;

    update_matrix = true;
}

void Widget::scale(int pixels) {

    rect_origin.x -= pixels;
    rect_origin.y -= pixels;
    rect_origin.w += pixels * 2;
    rect_origin.h += pixels * 2;

    update_matrix = true;
}

Vec2 Widget::getOrigin() {

    return origin;
}

void Widget::setOrigin(const Vec2 &origin) {

    this->origin = origin;

    update_matrix = true;
}

void Widget::setOrigin(int pivotMode) {

    switch (pivotMode) {

        case C2D_ORIGIN_TOP_RIGHT:
            setOrigin({rect_origin.w, 0});
            break;

        case C2D_ORIGIN_CENTER:
            setOrigin({rect_origin.w / 2, rect_origin.h / 2});
            break;

        case C2D_ORIGIN_BOTTOM_LEFT:
            setOrigin({0, rect_origin.h});
            break;

        case C2D_ORIGIN_BOTTOM_RIGHT:
            setOrigin({rect_origin.w, rect_origin.h});
            break;

        case C2D_ORIGIN_TOP_LEFT:
        default:
            setOrigin({0, 0});
            break;
    }

    update_matrix = true;
}

int Widget::getVisibility() {

    return visibility;
}

void Widget::setVisibility(int visibility) {

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
