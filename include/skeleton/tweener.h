//
// Created by cpasjuste on 23/09/18.
//

#ifndef C2D_TWEENER_H
#define C2D_TWEENER_H

#include "skeleton/tweeny/tween.h"
#include "skeleton/sfml/Clock.hpp"
#include "skeleton/sfml/Color.hpp"
#include "skeleton/sfml/Vector2.hpp"
#include "skeleton/sfml/Transformable.hpp"

namespace c2d {

    enum TweenerType : int {
        TPosition,
        TRotation,
        TScale,
        TColor
    };

    enum TweenerLoop : int {
        None,
        PingPong
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

        void setTransform(Transformable *transform);

        void step();

    protected:
        // transform to tween
        Transformable *transform;
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
                float duration, TweenerLoop loop = None);

        void setFrom(const Vector2f &from);

        void setTo(const Vector2f &to);

        const Vector2f getFrom() const;

        const Vector2f getTo() const;
    };

    ///
    /// Rotation tweener
    ///
    class TweenRotation : public Tweener {

    public:

        TweenRotation(
                float from, float to,
                float duration, TweenerLoop loop = None);

        void setFrom(float from);

        void setTo(float to);

        const float getFrom() const;

        const float getTo() const;
    };

    ///
    /// Color tweener
    ///
    class TweenColor : public Tweener {

    public:

        TweenColor(
                const Color &from, const Color &to,
                float duration, TweenerLoop loop = None);

        void setFrom(const Color &from);

        void setTo(const Color &to);

        const Color getFrom() const;

        const Color getTo() const;
    };
}

#endif //C2D_TWEENER_H
