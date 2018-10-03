//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_ROUNDED_RECTANGLE_H
#define CROSS2D_ROUNDED_RECTANGLE_H

#include "object.h"
#include "skeleton/sfml/RoundedRectangleShape.h"

namespace c2d {

    class RoundedRectangle : public C2DObject,
                             public sfml::RoundedRectangleShape {

    public:

        RoundedRectangle(const c2d::Vector2f &size, float radius = 5,
                         unsigned int cornerPointCount = 8);

        virtual ~RoundedRectangle();

    private:

        virtual void draw(Transform &transform);
    };
}

#endif //CROSS2D_ROUNDED_RECTANGLE_H
