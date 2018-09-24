//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_RECTANGLE_H
#define CROSS2D_RECTANGLE_H

#include "object.h"
#include "skeleton/sfml/RectangleShape.hpp"

namespace c2d {

    class Rectangle : public C2DObject, public sfml::RectangleShape {

    public:

        Rectangle();

        Rectangle(const FloatRect &rect = FloatRect(0, 0, 0, 0));

        Rectangle(const Vector2f &size = Vector2f(0, 0));

        virtual ~Rectangle();

    private:

        virtual void draw(Transform &transform);
    };
}

#endif //CROSS2D_RECTANGLE_H
