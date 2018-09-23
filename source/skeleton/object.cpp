//
// Created by cpasjuste on 12/12/17.
//

#include <cstdio>
#include <skeleton/object.h>

#include "c2d.h"

using namespace c2d;

C2DObject::C2DObject() {
    //printf("C2DObject(%p)\n", this);
}

void C2DObject::add(C2DObject *object) {

    if (object) {
        //printf("C2DObject(%p): add(%p)\n", this, C2DObject);
        object->parent = this;
        childs.push_back(object);
    }
}

void C2DObject::remove(C2DObject *object) {

    if (object) {
        if (!childs.empty()) {
            childs.erase(std::remove(
                    childs.begin(), childs.end(), object),
                         childs.end());
        }
    }
}

void C2DObject::add(Tweener *tweener) {

    if (tweener) {
        tweener->setTransform(thisTransform);
        tweeners.push_back(tweener);
    }
}

void C2DObject::remove(Tweener *tweener) {

    if (tweener) {
        if (!tweeners.empty()) {
            tweeners.erase(std::remove(
                    tweeners.begin(), tweeners.end(), tweener),
                           tweeners.end());
        }
    }
}

void C2DObject::draw(Transform &transform) {

    //printf("C2DObject(%p): draw\n", this);

    Transform combinedTransform = transform;

    if (thisTransform) {
        combinedTransform *= thisTransform->getTransform();
        // handle tweeners
        for (auto &tweener : tweeners) {
            if (tweener) {
                tweener->step();
            }
        }
    }

    for (auto &child : childs) {
        if (child) {
            if (child->visibility == Visible) {
                child->draw(combinedTransform);
            }
        }
    }
}

int C2DObject::getVisibility() {

    return visibility;
}

void C2DObject::setVisibility(Visibility v) {

    visibility = v;
}

int C2DObject::getDeleteMode() {
    return deleteMode;
}

void C2DObject::setDeleteMode(DeleteMode mode) {
    deleteMode = mode;
}

int C2DObject::getLayer() {
    return layer;
}

static bool sortByLayer(C2DObject *o1, C2DObject *o2) {
    if (!o1 || !o2) {
        return false;
    }
    return o1->getLayer() < o2->getLayer();
}

void C2DObject::setLayer(int layer) {

    this->layer = layer;
    if (parent) {
        std::sort(parent->childs.begin(),
                  parent->childs.end(),
                  sortByLayer);
    }
}

C2DObject::~C2DObject() {

    // delete childs
    for (auto widget = childs.begin(); widget != childs.end();) {
        if (*widget) {
            if ((*widget)->deleteMode == Auto) {
                //printf("~Widget(%p): delete child(%p)\n", this, *widget);
                delete (*widget);
            } else {
                childs.erase(widget);
            }
        }
    }
    childs.clear();

    // remove from parent
    if (parent) {
        //printf("~C2DObject(%p): remove from parent(%p)\n", this, parent);
        parent->remove(this);
    }
    //printf("~C2DObject(%p)\n", this);
}


