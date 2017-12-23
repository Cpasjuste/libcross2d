//
// Created by cpasjuste on 12/12/17.
//

#include <cstdio>
#include "c2d.h"

using namespace c2d;

Widget::Widget() {

    printf("Widget(%p)\n", this);
}

void Widget::add(Widget *widget) {

    if (widget) {
        printf("Widget(%p): add(%p)\n", this, widget);
        widget->parent = this;
        childs.push_back(widget);
    }
}

void Widget::draw(Transform &transform) {

    printf("Widget(%p): draw\n", this);

    Transform combinedTransform = transform;

    if (thisTransform) {
        combinedTransform *= thisTransform->getTransform();
    }

    for (auto &child : childs) {
        if (child->visibility == C2D_VISIBILITY_VISIBLE) {
            child->draw(combinedTransform);
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
    for (auto &child : childs) {
        if (child) {
            printf("~Widget(%p): delete child(%p)\n", this, child);
            delete (child);
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
