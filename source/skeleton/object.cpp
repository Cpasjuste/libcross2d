//
// Created by cpasjuste on 12/12/17.
//

#include <skeleton/object.h>

#include "c2d.h"

using namespace c2d;

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

void C2DObject::add(Tween *tween) {

    if (tween) {
        tween->setTransform((Transformable *) this);
        tweens.push_back(tween);
    }
}

void C2DObject::remove(Tween *tween) {

    if (tween && !tweens.empty()) {
        tweens.erase(std::remove(
                tweens.begin(), tweens.end(), tween), tweens.end());
    }
}

void C2DObject::draw(Transform &transform) {

    //printf("C2DObject(%p): draw\n", this);

    if (visibility_current == Visibility::Hidden) {
        return;
    }

    // handle tweeners
    for (auto &tween : tweens) {
        if (tween) {
            tween->step();
            // hide object if needed
            if (tween->getState() == TweenState::Stopped
                && visibility_current != visibility_wanted) {
                visibility_current = visibility_wanted;
            }
        }
    }

    Transform combinedTransform = transformation = transform;
    combinedTransform *= ((Transformable *) this)->getTransform();

    for (auto &child : childs) {
        if (child) {
            if (child->visibility_current == Visibility::Visible) {
                child->draw(combinedTransform);
            }
        }
    }
}

bool C2DObject::isVisible() {
    return visibility_wanted == Visibility::Visible;
}

Visibility C2DObject::getVisibility() {

    return visibility_wanted;
}

void C2DObject::setVisibility(Visibility v, bool tweenPlay) {

    if (v == visibility_current
        || v == visibility_wanted) {
        return;
    }

    if (tweenPlay && !tweens.empty()) {
        if (v == Visibility::Visible) {
            // we want the object to be visible immediately
            visibility_current = visibility_wanted = Visibility::Visible;
        } else {
            visibility_wanted = Visibility::Hidden;
        }
        for (auto &tween : tweens) {
            if (tween) {
                tween->play(visibility_wanted == Visibility::Visible ?
                            TweenDirection::Forward : TweenDirection::Backward);
                // reset transform/color to initial values
                tween->step();
            }
        }
    } else {
        visibility_current = visibility_wanted = v;
    }
}

void C2DObject::setAlpha(uint8_t alpha) {

    for (auto &child : childs) {
        if (child) {
            child->setAlpha(alpha);
        }
    }
}

DeleteMode C2DObject::getDeleteMode() {
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

    //printf("~C2DObject(%p): childs = %i\n", this, (int) childs.size());

    // delete tweeners
    for (auto tween = tweens.begin(); tween != tweens.end();) {
        if (*tween) {
            delete (*tween);
            remove(*tween);
        }
    }
    tweens.clear();

    // delete childs
    for (auto widget = childs.begin(); widget != childs.end();) {
        if (*widget) {
            if ((*widget)->deleteMode == DeleteMode::Auto) {
                //printf("\t~C2DObject(%p): delete child(%p)\n", this, *widget);
                delete (*widget);
            }
        } else {
            //printf("\t~C2DObject(%p): delete child(%p): child is NULL\n", this, *widget);
        }
    }
    childs.clear();

    // remove from parent
    if (parent) {
        //printf("~C2DObject(%p): remove from parent(%p)\n", this, parent);
        parent->remove(this);
    }
}
