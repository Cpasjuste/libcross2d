//
// Created by cpasjuste on 23/09/18.
//

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

void Tween::setTransform(c2d::Transformable *t) {

    transform = t;
}

void Tween::play(TweenDirection direction, bool _reset) {

    if (_reset) {
        reset();
    }

    if (direction != TweenDirection::Current) {
        setDirection(direction);
    }

    deltaClock->restart();
    state = TweenState::Playing;
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

    if (direction == TweenDirection::Backward) {
        tween.backward();
    } else if (direction == TweenDirection::Forward) {
        tween.forward();
    }
};

void Tween::step() {

    float progress = 0, duration = 0;

    delta = deltaClock->restart();

    if (!transform || state == TweenState::Stopped) {
        state = TweenState::Stopped;
        return;
    }

    progress = tween.progress();
    duration = tween.duration();

    if ((duration > 0 && (progress == 0.0f || progress == 1.0f))) {
        if (loop == TweenLoop::None) {
            if ((progress == 1.0f && direction == TweenDirection::Forward)
                || (progress == 0.0f && direction == TweenDirection::Backward)) {
                state = TweenState::Stopped;
                return;
            }
        } else if (loop == TweenLoop::PingPong) {
            if (direction == TweenDirection::Forward) {
                setDirection(TweenDirection::Backward);
            } else {
                setDirection(TweenDirection::Forward);
            }
        }
    }

    auto *object = (C2DObject *) transform;

    auto float4 = tween.step(delta.asMilliseconds(), true);
    if (type == TweenType::Position) {
        transform->setPosition(float4[0], float4[1]);
    } else if (type == TweenType::Rotation) {
        transform->setRotation(float4[0]);
    } else if (type == TweenType::Scale) {
        transform->setScale(float4[0], float4[1]);
    } else if (type == TweenType::Color) {
        Color color = {(uint8_t) (float4[0] * 255.0f), (uint8_t) (float4[1] * 255.0f),
                       (uint8_t) (float4[2] * 255.0f), (uint8_t) (float4[3] * 255.0f)};
        if (object->getType() == Type::Text) {
            ((Text *) transform)->setFillColor(color);
        } else if (object->getType() == Type::Texture) {
            ((Texture *) transform)->setColor(color);
        } else {
            ((Shape *) transform)->setFillColor(color);
        }
    } else if (type == TweenType::Alpha) {
        auto alpha = (uint8_t) (float4[0]);
        if (object->getType() == Type::Text) {
            ((Text *) transform)->setAlpha(alpha);
        } else if (object->getType() == Type::Texture) {
            ((Texture *) transform)->setAlpha(alpha);
        } else {
            ((Shape *) transform)->setAlpha(alpha);
        }
    }
}
