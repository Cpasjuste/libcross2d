//
// Created by cpasjuste on 13/12/17.
//

#ifndef CROSS2D_CIRCLE_H
#define CROSS2D_CIRCLE_H

#include "object.h"
#include "skeleton/sfml/CircleShape.hpp"

namespace c2d {

    class Circle : public C2DObject, public sfml::CircleShape {

    public:

        Circle(float radius = 0, std::size_t pointCount = 30);

        virtual ~Circle();

    private:

        virtual void draw(Transform &transform);
    };
}

#endif //CROSS2D_CIRCLE_H
