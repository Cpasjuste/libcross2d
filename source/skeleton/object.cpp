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

void C2DObject::add(Tween *tweener) {

    if (tweener) {
        tweener->setObject(this);
        tweeners.push_back(tweener);
    }
}

void C2DObject::remove(Tween *tweener) {

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

    if (visibility_current == Visibility::Hidden) {
        return;
    }

    Transform combinedTransform = transform;
    Transformable *transformable = nullptr;

    // TODO: can't cast Rectangle to Transformable ?
    if (type == TRectangle) {
        transformable = (Rectangle *) this;
    } else if (type == TLine) {
        transformable = (Line *) this;
    } else if (type == TCircle) {
        transformable = (Circle *) this;
    } else if (type == TTexture) {
        transformable = (Texture *) this;
    } else if (type == TText) {
        transformable = (Text *) this;
    }

    // handle tweeners
    for (auto &tween : tweeners) {
        if (tween) {
            tween->step();
            // hide object if needed
            if (tween->getState() == Stopped && visibility_current != visibility_wanted) {
                visibility_current = visibility_wanted;
            }
        }
    }

    if (transformable) {
        combinedTransform *= transformable->getTransform();
    }

    for (auto &child : childs) {
        if (child) {
            if (child->visibility_current == Visible) {
                child->draw(combinedTransform);
            }
        }
    }
}

int C2DObject::getVisibility() {

    return visibility_current;
}

void C2DObject::setVisibility(Visibility v, bool tweenPlay) {

    if (v == visibility_current || v == visibility_wanted) {
        return;
    }

    if (tweenPlay && !tweeners.empty()) {
        if (v == Visible) {
            // we want the object to be visible immediately
            visibility_current = visibility_wanted = Visible;
        } else {
            visibility_wanted = Hidden;
        }
        for (auto &tween : tweeners) {
            if (tween) {
                tween->play(visibility_wanted == Visible ? Forward : Backward);
                // set transform/color to initial values
                tween->step();
            }
        }
    } else {
        visibility_current = visibility_wanted = v;
    }
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

    // delete tweeners
    for (auto tween = tweeners.begin(); tween != tweeners.end();) {
        if (*tween) {
            delete (*tween);
            remove(*tween);
        }
    }
    tweeners.clear();

    // delete childs
    for (auto widget = childs.begin(); widget != childs.end();) {
        if (*widget) {
            if ((*widget)->deleteMode == Auto) {
                //printf("~Widget(%p): delete child(%p)\n", this, *widget);
                delete (*widget);
            }
            remove(*widget);
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
