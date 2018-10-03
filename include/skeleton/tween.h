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

    class C2DObject;

    enum TweenType : int {
        TPosition = 1, TRotation, TScale, TColor, TAlpha
    };

    enum TweenLoop : int {
        None = 1, PingPong
    };

    enum TweenDirection : int {
        Current, Forward, Backward
    };

    enum TweenState : int {
        Playing = 1, Stopped
    };

    ///
    /// main tweener class
    ///
    class Tween {

    public:

        Tween(float from, float to,
              float duration, TweenLoop loop = None);

        Tween(const Vector2f &from, const Vector2f &to,
              float duration, TweenLoop loop = None);

        Tween(const Color &from, const Color &to,
              float duration, TweenLoop loop = None);

        virtual ~Tween();

        void play(TweenDirection direction = Current, bool reset = false);

        void reset();

        TweenState getState() const;

        void setState(TweenState state);

        TweenDirection getDirection() const;

        void setDirection(TweenDirection direction);

        void setObject(C2DObject *thisObject);

        void step();

    protected:
        // transform to tween
        C2DObject *thisObject = nullptr;
        // tweener info
        TweenType type;
        TweenLoop loop;
        TweenDirection direction = Current;
        TweenState state = Playing;
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
                float duration, TweenLoop loop = None) : Tween(from, to, duration, loop) {
            this->type = TweenType::TPosition;
        };
    };

    ///
    /// Rotation tweener
    ///
    class TweenRotation : public Tween {

    public:

        TweenRotation(
                float from, float to,
                float duration, TweenLoop loop = None) : Tween(from, to, duration, loop) {
            this->type = TweenType::TRotation;
        }
    };

    ///
    /// Scale tweener
    ///
    class TweenScale : public Tween {

    public:

        TweenScale(
                const Vector2f &from, const Vector2f &to,
                float duration, TweenLoop loop = None) : Tween(from, to, duration, loop) {
            this->type = TweenType::TScale;
        }
    };

    ///
    /// Color tweener
    ///
    class TweenColor : public Tween {

    public:

        TweenColor(
                const Color &from, const Color &to,
                float duration, TweenLoop loop = None) : Tween(from, to, duration, loop) {
            this->type = TweenType::TColor;
        }
    };

    ///
    /// Alpha tweener
    ///
    class TweenAlpha : public Tween {

    public:

        TweenAlpha(
                float from, float to,
                float duration, TweenLoop loop = None) : Tween(from, to, duration, loop) {
            this->type = TweenType::TAlpha;
        }
    };
}

#endif //C2D_TWEEN_H
