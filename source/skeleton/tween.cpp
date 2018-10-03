//
// Created by cpasjuste on 23/09/18.
//

#include <skeleton/tween.h>

#include "c2d.h"

#include "skeleton/tweeny/tweeny.h"

using namespace c2d;
using namespace tweeny;

Tween::Tween(float from, float to, float duration, TweenLoop loop) {

    this->tween = tweeny::from(from, 0.0f, 0.0f, 0.0f).to(to, 0.0f, 0.0f, 0.0f)
            .during(duration * 1000);
    this->loop = loop;
    this->deltaClock = new C2DClock();
}

Tween::Tween(const Vector2f &from, const Vector2f &to, float duration, TweenLoop loop) {

    this->tween = tweeny::from(from.x, from.y, 0.0f, 0.0f).to(to.x, to.y, 0.0f, 0.0f)
            .during(duration * 1000);
    this->loop = loop;
    this->deltaClock = new C2DClock();
}

Tween::Tween(const Color &from, const Color &to, float duration, TweenLoop loop) {

    this->tween = tweeny::from(
            (float) from.r / 255.0f, (float) from.g / 255.0f, (float) from.b / 255.0f, (float) from.a / 255.0f)
            .to((float) to.r / 255.0f, (float) to.g / 255.0f, (float) to.b / 255.0f, (float) to.a / 255.0f)
            .during(duration * 1000);

    this->loop = loop;
    this->deltaClock = new C2DClock();
}

Tween::~Tween() {

    printf("~Tween(%p)\n", this);
    if (deltaClock) {
        delete (deltaClock);
        deltaClock = nullptr;
    }
}

void Tween::setObject(C2DObject *thisObject) {

    this->thisObject = thisObject;
}

void Tween::play(TweenDirection direction, bool _reset) {

    if (_reset) {
        reset();
    }

    if (direction != Current) {
        setDirection(direction);
    }

    deltaClock->restart();
    state = Playing;
}

void Tween::reset() {

    if (tween.duration() > 0) {
        tween.seek(0);
    }
}

TweenState Tween::getState() const {
    return state;
};

void Tween::setState(TweenState state) {
    this->state = state;
};

TweenDirection Tween::getDirection() const {
    return direction;
};

void Tween::setDirection(TweenDirection direction) {

    this->direction = direction;

    if (direction == Backward) {
        tween.backward();
    } else if (direction == Forward) {
        tween.forward();
    }
};

void Tween::step() {

    float progress = 0, duration = 0;

    delta = deltaClock->restart();

    if (!thisObject || state == Stopped) {
        state = Stopped;
        return;
    }

    progress = tween.progress();
    duration = tween.duration();

    if ((duration > 0 && (progress == 0.0f || progress == 1.0f))) {
        if (loop == TweenLoop::None) {
            if ((progress == 1.0f && direction == Forward) || (progress == 0.0f && direction == Backward)) {
                state = Stopped;
                return;
            }
        } else if (loop == TweenLoop::PingPong) {
            if (direction == Forward) {
                setDirection(Backward);
            } else {
                setDirection(Forward);
            }
        }
    }

    auto float4 = tween.step(delta.asMilliseconds(), true);

    if (type == TweenType::TPosition) {
        thisObject->getTransformable()->setPosition(float4[0], float4[1]);
    } else if (type == TweenType::TRotation) {
        thisObject->getTransformable()->setRotation(float4[0]);
    } else if (type == TweenType::TScale) {
        thisObject->getTransformable()->setScale(float4[0], float4[1]);
    } else if (type == TweenType::TColor) {
        Color color = {(uint8_t) (float4[0] * 255.0f), (uint8_t) (float4[1] * 255.0f),
                       (uint8_t) (float4[2] * 255.0f), (uint8_t) (float4[3] * 255.0f)};
        if (thisObject->getType() == C2DObject::ObjectType::TRectangle) {
            ((Rectangle *) thisObject)->setFillColor(color);
        } else if (thisObject->getType() == C2DObject::ObjectType::TTexture) {
            ((Texture *) thisObject)->setColor(color);
        } else {
            ((Shape *) thisObject)->setFillColor(color);
        }
    } else if (type == TweenType::TAlpha) {
        Color color = ((Rectangle *) thisObject)->getFillColor();
        Color colorA = {color.r, color.g, color.b, (uint8_t) float4[0]};
        if (thisObject->getType() == C2DObject::ObjectType::TRectangle) {
            ((Rectangle *) thisObject)->setFillColor(colorA);
        } else if (thisObject->getType() == C2DObject::ObjectType::TTexture) {
            ((Texture *) thisObject)->setColor(color);
        } else {
            ((Shape *) thisObject)->setFillColor(color);
        }
    }
}
