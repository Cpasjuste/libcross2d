//
// Created by cpasjuste on 12/12/17.
//

#include <cstdio>
#include "c2d.h"

using namespace c2d;

Widget::Widget(const FloatRect &rect) : RectangleShape() {

    setPosition(rect.left, rect.top);
    setSize(Vector2f(rect.width, rect.height));

    printf("Widget(%p):\n", this);
}

Widget::Widget(const Vector2f &size) : RectangleShape(size) {

    printf("Widget(%p):\n", this);
}

void Widget::add(Widget *widget) {

    if (widget) {
        printf("Widget(%p): add(%p)\n", this, widget);
        widget->parent = this;
        childs.push_back(widget);
    }
}

void Widget::draw(const Transform &transform, const Vector2f &scaling) {

    printf("Widget(%p): draw\n", this);

    Transform combinedTransform = transform * getTransform();
    Vector2f combinedScaling = {scaling.x * getScale().x, scaling.y * getScale().y};

    for (Widget *widget : childs) {
        if (widget->visibility == C2D_VISIBILITY_VISIBLE) {
            widget->draw(combinedTransform, combinedScaling);
        }
    }
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
