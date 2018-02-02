//
// Created by cpasjuste on 12/12/17.
//

#include <cstdio>
#include "c2d.h"

using namespace c2d;

Widget::Widget() {

    //printf("Widget(%p)\n", this);
}

void Widget::add(Widget *widget) {

    if (widget) {
        //printf("Widget(%p): add(%p)\n", this, widget);
        widget->parent = this;
        childs.push_back(widget);
    }
}

void Widget::draw(Transform &transform) {

    //printf("Widget(%p): draw\n", this);

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

int Widget::getLayer() {
    return layer;
}

static bool sortByLayer(Widget *w1, Widget *w2) {
    return w1->getLayer() < w2->getLayer();
}

void Widget::setLayer(int layer) {

    this->layer = layer;
    if (parent) {
        std::sort(parent->childs.begin(),
                  parent->childs.end(),
                  sortByLayer);
    }
}

void Widget::remove(Widget *widget) {

    childs.erase(std::remove(
            childs.begin(), childs.end(), widget),
                 childs.end());
}

Widget::~Widget() {

    // delete childs
    for (auto widget = childs.begin(); widget != childs.end();) {
        if (*widget) {
            //printf("~Widget(%p): delete child(%p)\n", this, *widget);
            delete (*widget);
        }
    }
    childs.clear();

    // remove from parent
    if (parent) {
        // printf("~Widget(%p): remove from parent(%p)\n", this, parent);
        parent->childs.erase(
                std::remove(parent->childs.begin(), parent->childs.end(), this),
                parent->childs.end());
    }

    //printf("~Widget(%p)\n", this);
}
