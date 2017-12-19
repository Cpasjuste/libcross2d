//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_RECTANGLE_H
#define CROSS2D_RECTANGLE_H

#include "widget.h"

namespace c2d {

    class Rectangle : public Widget {

    public:

        Rectangle(const FloatRect &rect = FloatRect(0, 0, 0, 0));

        Rectangle(const Vector2f &size = Vector2f(0, 0));

        virtual ~Rectangle();

    private:

        virtual void draw(const Transform &transform, const Vector2f &scaling);
    };
}

#endif //CROSS2D_RECTANGLE_H
