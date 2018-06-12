//
// Created by cpasjuste on 12/12/17.
//

#include <cstdio>
#include "c2d.h"

using namespace c2d;

C2DObject::C2DObject() {

    //printf("Widget(%p)\n", this);
}

void C2DObject::add(C2DObject *object) {

    if (object) {
        //printf("Widget(%p): add(%p)\n", this, widget);
        object->parent = this;
        childs.push_back(object);
    }
}

void C2DObject::draw(Transform &transform) {

    //printf("Widget(%p): draw\n", this);

    Transform combinedTransform = transform;

    if (thisTransform) {
        combinedTransform *= thisTransform->getTransform();
    }

    for (auto &child : childs) {
        if (child->visibility == Visible) {
            child->draw(combinedTransform);
        }
    }
}

int C2DObject::getVisibility() {

    return visibility;
}

void C2DObject::setVisibility(Visibility visibility) {

    this->visibility = visibility;
}

int C2DObject::getLayer() {
    return layer;
}

static bool sortByLayer(C2DObject *w1, C2DObject *w2) {
    return w1->getLayer() < w2->getLayer();
}

void C2DObject::setLayer(int layer) {

    this->layer = layer;
    if (parent) {
        std::sort(parent->childs.begin(),
                  parent->childs.end(),
                  sortByLayer);
    }
}

void C2DObject::removeChild(C2DObject *widget) {

    childs.erase(std::remove(
            childs.begin(), childs.end(), widget),
                 childs.end());
}

C2DObject::~C2DObject() {

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
        //printf("~Widget(%p): remove from parent(%p)\n", this, parent);
        parent->childs.erase(
                std::remove(parent->childs.begin(), parent->childs.end(), this),
                parent->childs.end());
    }

    //printf("~Widget(%p)\n", this);
}
