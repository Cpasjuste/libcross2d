//
// Created by cpasjuste on 23/09/18.
//

#ifndef C2D_TWEENER_H
#define C2D_TWEENER_H

#include "skeleton/tweeny/tween.h"
#include "skeleton/sfml/Clock.hpp"
#include "skeleton/sfml/Color.hpp"
#include "skeleton/sfml/Vector2.hpp"

namespace c2d {

    class C2DObject;

    enum TweenerType : int {
        TPosition, TRotation, TScale, TColor, TAlpha
    };

    enum TweenerLoop : int {
        None, PingPong
    };

    ///
    /// main tweener class
    ///
    class Tweener {

    public:

        Tweener(float from, float to,
                float duration, TweenerLoop loop = None);

        Tweener(const Vector2f &from, const Vector2f &to,
                float duration, TweenerLoop loop = None);

        Tweener(const Color &from, const Color &to,
                float duration, TweenerLoop loop = None);

        ~Tweener();

        void setObject(C2DObject *thisObject);

        void step();

    protected:
        // transform to tween
        C2DObject *thisObject;
        // tweener info
        TweenerType type;
        TweenerLoop loop;
        // float tweener
        tweeny::tween<float> tweenFloat;
        float fromFloat, toFloat;
        // vector2f tweener
        tweeny::tween<float, float> tweenVector2;
        Vector2f fromVector2, toVector2;
        // Color tweener
        tweeny::tween<float, float, float, float> tweenColor;
        Color fromColor, toColor;
        // timer stuff
        Clock *timerClock;
        Clock *deltaClock;
        Time delta;
        Time time;
        float duration;
        // active state
        bool active = false;
    };

    ///
    /// Position tweener
    ///
    class TweenPosition : public Tweener {

    public:

        TweenPosition(
                const Vector2f &from, const Vector2f &to,
                float duration, TweenerLoop loop = None) : Tweener(from, to, duration, loop) {
            this->type = TweenerType::TPosition;
        };

        void setFrom(const Vector2f &from) {
            fromVector2 = from;
        }

        void setTo(const Vector2f &to) {
            toVector2 = to;
        }

        const Vector2f getFrom() const {
            return fromVector2;
        }

        const Vector2f getTo() const {
            return toVector2;
        }
    };

    ///
    /// Rotation tweener
    ///
    class TweenRotation : public Tweener {

    public:

        TweenRotation(
                float from, float to,
                float duration, TweenerLoop loop = None) : Tweener(from, to, duration, loop) {
            this->type = TweenerType::TRotation;
        }

        void setFrom(float from) {
            fromFloat = from;
        }

        void setTo(float to) {
            toFloat = to;
        }

        float getFrom() const {
            return fromFloat;
        }

        float getTo() const {
            return toFloat;
        }
    };

    ///
    /// Scale tweener
    ///
    class TweenScale : public Tweener {

    public:

        TweenScale(
                const Vector2f &from, const Vector2f &to,
                float duration, TweenerLoop loop = None) : Tweener(from, to, duration, loop) {
            this->type = TweenerType::TScale;
        }

        void setFrom(const Vector2f &from) {
            fromVector2 = from;
        }

        void setTo(const Vector2f &to) {
            toVector2 = to;
        }

        Vector2f getFrom() const {
            return fromVector2;
        }

        Vector2f getTo() const {
            return toVector2;
        }
    };

    ///
    /// Color tweener
    ///
    class TweenColor : public Tweener {

    public:

        TweenColor(
                const Color &from, const Color &to,
                float duration, TweenerLoop loop = None) : Tweener(from, to, duration, loop) {
            this->type = TweenerType::TColor;
        }

        void setFrom(const Color &from) {
            fromColor = from;
        }

        void setTo(const Color &to) {
            toColor = to;
        }

        Color getFrom() const {
            return fromColor;
        }

        Color getTo() const {
            return toColor;
        }
    };

    ///
    /// Alpha tweener
    ///
    class TweenAlpha : public Tweener {

    public:

        TweenAlpha(
                float from, float to,
                float duration, TweenerLoop loop = None) : Tweener(from, to, duration, loop) {
            this->type = TweenerType::TAlpha;
        }

        void setFrom(float from) {
            fromFloat = from;
        }

        void setTo(float to) {
            toFloat = to;
        }

        float getFrom() const {
            return fromFloat;
        }

        float getTo() const {
            return toFloat;
        }
    };
}

#endif //C2D_TWEENER_H
