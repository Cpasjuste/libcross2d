//
// Created by cpasjuste on 12/12/17.
//

#include "cross2d/c2d.h"

using namespace c2d;

void C2DObject::add(C2DObject *object) {

    if (object != nullptr) {
        //printf("C2DObject(%p): add(%p)\n", this, C2DObject);
        object->parent = this;
        object->setLayer(object->getLayer());
        childs.push_back(object);
    }
}

void C2DObject::remove(C2DObject *object) {

    if (!childs.empty()) {
        childs.erase(std::remove(
                childs.begin(), childs.end(), object),
                     childs.end());
    }
}

void C2DObject::add(Tween *tween) {

    if (tween != nullptr) {
        tween->setTransform((Transformable *) this);
        tweens.push_back(tween);
    }
}

void C2DObject::remove(Tween *tween) {

    if (tween != nullptr && !tweens.empty()) {
        tweens.erase(std::remove(
                tweens.begin(), tweens.end(), tween), tweens.end());
    }
}

bool C2DObject::onInput(Input::Player *players) {

    for (auto &child : childs) {
        if (child != nullptr && child->isVisible()) {
            if (child->onInput(players)) {
                return true;
            }
        }
    }

    return false;
}

void C2DObject::onUpdate() {

    for (auto &child : childs) {
        if (child != nullptr) {
            child->onUpdate();
        }
    }
}

void C2DObject::onDraw(Transform &transform, bool draw) {

    //printf("C2DObject(%p): draw\n", this);

    if (visibility_current == Visibility::Hidden) {
        return;
    }

    // handle tweeners
    for (auto &tween : tweens) {
        if (tween != nullptr) {
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
        if (child != nullptr) {
            if (child->visibility_current == Visibility::Visible) {
                child->onDraw(combinedTransform, draw);
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

    if (tweenPlay && !tweens.empty()) {
        if (v == Visibility::Visible) {
            // we want the object to be visible (drawn) immediately
            visibility_current = visibility_wanted = Visibility::Visible;
        } else {
            visibility_wanted = Visibility::Hidden;
        }
        for (auto &tween : tweens) {
            if (tween != nullptr) {
                tween->play(visibility_wanted == Visibility::Visible ?
                            TweenDirection::Forward : TweenDirection::Backward);
                // reset transform/color to initial values
                // TODO: is this really needed ?
                //tween->step();
            }
        }
    } else {
        visibility_current = visibility_wanted = v;
        for (auto &tween : tweens) {
            if (tween != nullptr) {
                tween->reset();
            }
        }
    }
}

void C2DObject::setAlpha(uint8_t alpha, bool recursive) {

    if (recursive) {
        for (auto &child : childs) {
            if (child != nullptr) {
                child->setAlpha(alpha, recursive);
            }
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
    return o1->getLayer() < o2->getLayer();
}

void C2DObject::setLayer(int l) {

    this->layer = l;
    if (parent != nullptr) {
        std::stable_sort(parent->childs.begin(),
                         parent->childs.end(),
                         sortByLayer);
    }
}

std::vector<C2DObject *> C2DObject::getChilds() {
    return childs;
}

Type C2DObject::getType() const {
    return type;
}

C2DObject::~C2DObject() {

    //printf("~C2DObject(%p): childs = %i\n", this, (int) childs.size());

    // delete tweeners
    for (auto tween = tweens.begin(); tween != tweens.end();) {
        if (*tween != nullptr) {
            delete (*tween);
            remove(*tween);
        }
    }
    tweens.clear();

    // delete childs
    for (auto widget = childs.begin(); widget != childs.end();) {
        if (*widget != nullptr) {
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
    if (parent != nullptr) {
        //printf("~C2DObject(%p): remove from parent(%p)\n", this, parent);
        parent->remove(this);
    }
}
