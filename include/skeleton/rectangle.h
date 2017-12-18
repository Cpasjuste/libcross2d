//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_RECTANGLE_H
#define CROSS2D_RECTANGLE_H

#include "widget.h"

namespace c2d {

    class Rectangle : public Widget {

    public:

        Rectangle(const Vector2f &size = Vector2f(0, 0), bool fill = true);

        virtual ~Rectangle();

        virtual void draw(Renderer *renderer, const Transform &transform);

        bool fill = true;

    };
}

#endif //CROSS2D_RECTANGLE_H
