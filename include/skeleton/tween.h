//
// Created by cpasjuste on 23/09/18.
//

#ifndef C2D_TWEEN_H
#define C2D_TWEEN_H

#include "skeleton/tweeny/tween.h"
#include "skeleton/sfml/Clock.hpp"
#include "skeleton/sfml/Color.hpp"
#include "skeleton/sfml/Vector2.hpp"

namespace c2d {

    class Transformable;

    enum class TweenType : int {
        Position = 1, Rotation, Scale, Color, Alpha
    };

    enum class TweenLoop : int {
        None = 1, PingPong
    };

    enum class TweenDirection : int {
        Current, Forward, Backward
    };

    enum class TweenState : int {
        Playing = 1, Stopped
    };

    ///
    /// main tweener class
    ///
    class Tween {

    public:

        Tween(float from, float to,
              float duration, TweenLoop loop = TweenLoop::None);

        Tween(const Vector2f &from, const Vector2f &to,
              float duration, TweenLoop loop = TweenLoop::None);

        Tween(const Color &from, const Color &to,
              float duration, TweenLoop loop = TweenLoop::None);

        virtual ~Tween();

        void play(TweenDirection direction = TweenDirection::Current, bool reset = false);

        void reset();

        TweenState getState() const;

        void setState(TweenState state);

        TweenDirection getDirection() const;

        void setDirection(TweenDirection direction);

        void setTransform(Transformable *transform);

        void step();

    protected:
        // transform to tween
        Transformable *transform = nullptr;
        // tweener info
        TweenType type;
        TweenLoop loop;
        TweenDirection direction = TweenDirection::Current;
        TweenState state = TweenState::Playing;
        tweeny::tween<float, float, float, float> tween;
        // timer stuff
        Clock *deltaClock = nullptr;
        Time delta;
        Time time;
    };

    ///
    /// Position tweener
    ///
    class TweenPosition : public Tween {

    public:

        TweenPosition(
                const Vector2f &from, const Vector2f &to,
                float duration, TweenLoop loop = TweenLoop::None) : Tween(from, to, duration, loop) {
            this->type = TweenType::Position;
        };
    };

    ///
    /// Rotation tweener
    ///
    class TweenRotation : public Tween {

    public:

        TweenRotation(
                float from, float to,
                float duration, TweenLoop loop = TweenLoop::None) : Tween(from, to, duration, loop) {
            this->type = TweenType::Rotation;
        }
    };

    ///
    /// Scale tweener
    ///
    class TweenScale : public Tween {

    public:

        TweenScale(
                const Vector2f &from, const Vector2f &to,
                float duration, TweenLoop loop = TweenLoop::None) : Tween(from, to, duration, loop) {
            this->type = TweenType::Scale;
        }
    };

    ///
    /// Color tweener
    ///
    class TweenColor : public Tween {

    public:

        TweenColor(
                const Color &from, const Color &to,
                float duration, TweenLoop loop = TweenLoop::None) : Tween(from, to, duration, loop) {
            this->type = TweenType::Color;
        }
    };

    ///
    /// Alpha tweener
    ///
    class TweenAlpha : public Tween {

    public:

        TweenAlpha(
                float from, float to,
                float duration, TweenLoop loop = TweenLoop::None) : Tween(from, to, duration, loop) {
            this->type = TweenType::Alpha;
        }
    };
}

#endif //C2D_TWEEN_H
