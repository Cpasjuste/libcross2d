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

void Tweener::setTransform(Transformable *transform) {
    this->transform = transform;
}

void Tweener::step() {

    delta = deltaClock->restart();
    time = timerClock->getElapsedTime();

    if (!transform) {
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
        auto pos = tweenVector2.step(delta.asMilliseconds(), true);
        transform->setPosition(pos[0], pos[1]);
    } else if (type == TweenerType::TRotation) {
        auto rot = tweenFloat.step(delta.asMilliseconds(), true);
        transform->setRotation(rot);
    }
}

TweenPosition::TweenPosition(
        const Vector2f &from, const Vector2f &to, float duration, TweenerLoop loop)
        : Tweener(from, to, duration, loop) {

    this->type = TweenerType::TPosition;
}

void TweenPosition::setFrom(const Vector2f &from) {
    fromVector2 = from;
}

void TweenPosition::setTo(const Vector2f &to) {
    toVector2 = to;
}

const Vector2f TweenPosition::getFrom() const {
    return fromVector2;
}

const Vector2f TweenPosition::getTo() const {
    return toVector2;
}

TweenRotation::TweenRotation(
        float from, float to, float duration, TweenerLoop loop)
        : Tweener(from, to, duration, loop) {

    this->type = TweenerType::TRotation;
}

void TweenRotation::setFrom(float from) {
    toFloat = from;
}

void TweenRotation::setTo(float to) {
    toFloat = to;
}

const float TweenRotation::getFrom() const {
    return fromFloat;
}

const float TweenRotation::getTo() const {
    return toFloat;
}

TweenColor::TweenColor(
        const Color &from, const Color &to, float duration, TweenerLoop loop)
        : Tweener(from, to, duration, loop) {

    this->type = TweenerType::TColor;
}

void TweenColor::setFrom(const Color &from) {
    toColor = from;
}

void TweenColor::setTo(const Color &to) {
    toColor = to;
}

const Color TweenColor::getFrom() const {
    return fromColor;
}

const Color TweenColor::getTo() const {
    return toColor;
}
