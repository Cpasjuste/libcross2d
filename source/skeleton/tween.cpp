//
// Created by cpasjuste on 23/09/18.
//

#include <skeleton/tween.h>

#include "c2d.h"

#include "skeleton/tweeny/tweeny.h"

using namespace c2d;
using namespace tweeny;

Tween::Tween(float from, float to, float duration, TweenLoop loop) {

    this->tweenFloat = tweeny::from(from).to(to).during(duration * 1000);
    this->loop = loop;
    this->deltaClock = new C2DClock();
}

Tween::Tween(const Vector2f &from, const Vector2f &to, float duration, TweenLoop loop) {

    this->tweenVector2 = tweeny::from(from.x, from.y).to(to.x, to.y).during(duration * 1000);
    this->loop = loop;
    this->deltaClock = new C2DClock();
}

Tween::Tween(const Color &from, const Color &to, float duration, TweenLoop loop) {

    this->tweenColor = tweeny::from(
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

    if (tweenFloat.duration() > 0) {
        tweenFloat.seek(0);
    }
    if (tweenVector2.duration() > 0) {
        tweenVector2.seek(0);
    }
    if (tweenColor.duration() > 0) {
        tweenColor.seek(0);
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
        tweenFloat.backward();
        tweenVector2.backward();
        tweenColor.backward();
    } else if (direction == Forward) {
        tweenFloat.forward();
        tweenVector2.forward();
        tweenColor.forward();
    }
};

void Tween::step() {

    float progress = 0, duration = 0;

    delta = deltaClock->restart();

    if (!thisObject || state == Stopped) {
        state = Stopped;
        return;
    }

    if (type == TweenType::TPosition || type == TweenType::TScale) {
        progress = tweenVector2.progress();
        duration = tweenVector2.duration();
    } else if (type == TweenType::TRotation || type == TweenType::TAlpha) {
        progress = tweenFloat.progress();
        duration = tweenFloat.duration();
    } else if (type == TweenType::TColor) {
        progress = tweenColor.progress();
        duration = tweenColor.duration();
    }

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

    if (type == TweenType::TPosition) {
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
    } else if (type == TweenType::TRotation) {
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
    } else if (type == TweenType::TScale) {
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
    } else if (type == TweenType::TColor) {
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
    } else if (type == TweenType::TAlpha) {
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
