//
// Created by cpasjuste on 23/09/18.
//

#include "c2d.h"

#include "skeleton/tweeny/tweeny.h"

using namespace c2d;
using namespace tweeny;

Tweener::Tweener(float from, float to, float duration, TweenerLoop loop) {

    this->fromFloat = from;
    this->toFloat = to;
    this->tweenFloat = tweeny::from(from).to(to).during(duration * 1000);

    this->loop = loop;
    this->duration = duration;
    this->active = true;

    this->timerClock = new C2DClock();
    this->deltaClock = new C2DClock();
}

Tweener::Tweener(const Vector2f &from, const Vector2f &to, float duration, TweenerLoop loop) {

    this->fromVector2 = from;
    this->toVector2 = to;
    this->tweenVector2 = tweeny::from(from.x, from.y).to(to.x, to.y).during(duration * 1000);

    this->loop = loop;
    this->duration = duration;
    this->active = true;

    this->timerClock = new C2DClock();
    this->deltaClock = new C2DClock();
}

Tweener::Tweener(const Color &from, const Color &to, float duration, TweenerLoop loop) {

    this->fromColor = from;
    this->toColor = to;
    this->tweenColor = tweeny::from(
            (float) from.r / 255.0f, (float) from.g / 255.0f, (float) from.b / 255.0f, (float) from.a / 255.0f)
            .to((float) to.r / 255.0f, (float) to.g / 255.0f, (float) to.b / 255.0f, (float) to.a / 255.0f)
            .during(duration * 1000);

    this->loop = loop;
    this->duration = duration;
    this->active = true;

    this->timerClock = new C2DClock();
    this->deltaClock = new C2DClock();
}

Tweener::~Tweener() {

    delete (deltaClock);
    delete (timerClock);
}

void Tweener::setObject(C2DObject *thisObject) {
    this->thisObject = thisObject;
}

void Tweener::step() {

    delta = deltaClock->restart();
    time = timerClock->getElapsedTime();

    if (!thisObject || !active) {
        active = false;
        return;
    }

    if (time.asSeconds() >= duration) {
        if (loop == TweenerLoop::None) {
            active = false;
            return;
        } else if (loop == TweenerLoop::PingPong) {
            if (tweenFloat.direction() == 1) {
                tweenFloat.backward();
                tweenVector2.backward();
                tweenColor.backward();
            } else {
                tweenFloat.forward();
                tweenVector2.forward();
                tweenColor.forward();
            }
            timerClock->restart();
        }
    }

    if (type == TweenerType::TPosition) {
        auto position = tweenVector2.step(delta.asMilliseconds(), true);
        if (thisObject->getType() == C2DObject::ObjectType::TRectangle) {
            ((Rectangle *) thisObject)->setPosition(position[0], position[1]);
        } else if (thisObject->getType() == C2DObject::ObjectType::TLine) {
            ((Line *) thisObject)->setPosition(position[0], position[1]);
        } else if (thisObject->getType() == C2DObject::ObjectType::TCircle) {
            ((Circle *) thisObject)->setPosition(position[0], position[1]);
        } else if (thisObject->getType() == C2DObject::ObjectType::TTexture) {
            ((Texture *) thisObject)->setPosition(position[0], position[1]);
        } else if (thisObject->getType() == C2DObject::ObjectType::TText) {
            ((Text *) thisObject)->setPosition(position[0], position[1]);
        }
    } else if (type == TweenerType::TRotation) {
        auto rotation = tweenFloat.step(delta.asMilliseconds(), true);
        if (thisObject->getType() == C2DObject::ObjectType::TRectangle) {
            ((Rectangle *) thisObject)->setRotation(rotation);
        } else if (thisObject->getType() == C2DObject::ObjectType::TLine) {
            ((Line *) thisObject)->setRotation(rotation);
        } else if (thisObject->getType() == C2DObject::ObjectType::TCircle) {
            ((Circle *) thisObject)->setRotation(rotation);
        } else if (thisObject->getType() == C2DObject::ObjectType::TTexture) {
            ((Texture *) thisObject)->setRotation(rotation);
        } else if (thisObject->getType() == C2DObject::ObjectType::TText) {
            ((Text *) thisObject)->setRotation(rotation);
        }
    } else if (type == TweenerType::TScale) {
        auto scale = tweenVector2.step(delta.asMilliseconds(), true);
        if (thisObject->getType() == C2DObject::ObjectType::TRectangle) {
            ((Rectangle *) thisObject)->setScale(scale[0], scale[1]);
        } else if (thisObject->getType() == C2DObject::ObjectType::TLine) {
            ((Line *) thisObject)->setScale(scale[0], scale[1]);
        } else if (thisObject->getType() == C2DObject::ObjectType::TCircle) {
            ((Circle *) thisObject)->setScale(scale[0], scale[1]);
        } else if (thisObject->getType() == C2DObject::ObjectType::TTexture) {
            ((Texture *) thisObject)->setScale(scale[0], scale[1]);
        } else if (thisObject->getType() == C2DObject::ObjectType::TText) {
            ((Text *) thisObject)->setScale(scale[0], scale[1]);
        }
    } else if (type == TweenerType::TColor) {
        auto c = tweenColor.step(delta.asMilliseconds(), true);
        Color color = {(uint8_t) (c[0] * 255.0f), (uint8_t) (c[1] * 255.0f),
                       (uint8_t) (c[2] * 255.0f), (uint8_t) (c[3] * 255.0f)};
        if (thisObject->getType() == C2DObject::ObjectType::TRectangle) {
            ((Rectangle *) thisObject)->setFillColor(color);
        } else if (thisObject->getType() == C2DObject::ObjectType::TLine) {
            ((Line *) thisObject)->setFillColor(color);
        } else if (thisObject->getType() == C2DObject::ObjectType::TCircle) {
            ((Circle *) thisObject)->setFillColor(color);
        } else if (thisObject->getType() == C2DObject::ObjectType::TTexture) {
            ((Texture *) thisObject)->setColor(color);
        } else if (thisObject->getType() == C2DObject::ObjectType::TText) {
            ((Text *) thisObject)->setFillColor(color);
        }
    } else if (type == TweenerType::TAlpha) {
        auto alpha = tweenFloat.step(delta.asMilliseconds(), true);
        if (thisObject->getType() == C2DObject::ObjectType::TRectangle) {
            Color color = ((Rectangle *) thisObject)->getFillColor();
            Color colorA = {color.r, color.g, color.b, (uint8_t) alpha};
            ((Rectangle *) thisObject)->setFillColor(colorA);
        } else if (thisObject->getType() == C2DObject::ObjectType::TLine) {
            Color color = ((Line *) thisObject)->getFillColor();
            Color colorA = {color.r, color.g, color.b, (uint8_t) alpha};
            ((Line *) thisObject)->setFillColor(colorA);
        } else if (thisObject->getType() == C2DObject::ObjectType::TCircle) {
            Color color = ((Circle *) thisObject)->getFillColor();
            Color colorA = {color.r, color.g, color.b, (uint8_t) alpha};
            ((Circle *) thisObject)->setFillColor(colorA);
        } else if (thisObject->getType() == C2DObject::ObjectType::TTexture) {
            Color color = ((Texture *) thisObject)->getColor();
            Color colorA = {color.r, color.g, color.b, (uint8_t) alpha};
            ((Texture *) thisObject)->setColor(colorA);
        } else if (thisObject->getType() == C2DObject::ObjectType::TText) {
            Color color = ((Text *) thisObject)->getFillColor();
            Color colorA = {color.r, color.g, color.b, (uint8_t) alpha};
            ((Text *) thisObject)->setFillColor(colorA);
        }
    }
}
